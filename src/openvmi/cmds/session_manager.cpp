/*
 * Copyright (C) 2016 Simon Fels <morphis@gravedo.de>
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 3, as published
 * by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranties of
 * MERCHANTABILITY, SATISFACTORY QUALITY, or FITNESS FOR A PARTICULAR
 * PURPOSE.  See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wswitch-default"
#include <boost/filesystem.hpp>

#include "core/posix/signal.h"

#include "openvmi/application/launcher_storage.h"
#include "openvmi/application/database.h"
#include "openvmi/bridge/android_api_stub.h"
#include "openvmi/bridge/platform_api_skeleton.h"
#include "openvmi/bridge/platform_message_processor.h"
#include "openvmi/graphics/gl_renderer_server.h"
#include "openvmi/sensors/sensors_manager.h"

#include "openvmi/cmds/session_manager.h"
#include "openvmi/common/dispatcher.h"
#include "openvmi/system_configuration.h"
#include "openvmi/dbus/bus.h"
#include "openvmi/dbus/skeleton/service.h"
#include "openvmi/input/manager.h"
#include "openvmi/logger.h"
#include "openvmi/network/published_socket_connector.h"
#include "openvmi/qemu/pipe_connection_creator.h"
#include "openvmi/rpc/channel.h"
#include "openvmi/rpc/connection_creator.h"
#include "openvmi/runtime.h"
#include "openvmi/platform/base_platform.h"
#include "openvmi/wm/multi_window_manager.h"
#include "openvmi/wm/single_window_manager.h"

#include "external/xdg/xdg.h"

#include <sys/prctl.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

unsigned int openvmiadbport = 6664;
unsigned int unique_ID = 0;
char unique_Name[50]={0};
#pragma GCC diagnostic pop


std::string _G_stBuf="";

namespace fs = boost::filesystem;

namespace {
constexpr const char *default_appmgr_package{"org.openvmi.appmgr"};
constexpr const char *default_appmgr_component{"org.openvmi.appmgr.AppViewActivity"};
const boost::posix_time::milliseconds default_appmgr_startup_delay{50};
const openvmi::graphics::Rect default_single_window_size{0, 0, 1024, 768};

class NullConnectionCreator : public openvmi::network::ConnectionCreator<
                                  boost::asio::local::stream_protocol> {
 public:
  void create_connection_for(
      std::shared_ptr<boost::asio::local::stream_protocol::socket> const
          &socket) override {
    WARNING("Not implemented");
    socket->close();
  }
};
}

void openvmi::cmds::SessionManager::launch_appmgr_if_needed(const std::shared_ptr<bridge::AndroidApiStub> &android_api_stub) {
  if (!single_window_)
    return;

  android::Intent launch_intent;
  launch_intent.package = default_appmgr_package;
  launch_intent.component = default_appmgr_component;
  // As this will only be executed in single window mode we don't have
  // to specify and launch bounds.
  android_api_stub->launch(launch_intent, graphics::Rect::Invalid, wm::Stack::Id::Default);
}

openvmi::cmds::SessionManager::SessionManager()
    : CommandWithFlagsAndAction{cli::Name{"session-manager"}, cli::Usage{"session-manager"},
                                cli::Description{"Run the the openvmi session manager"}},
      window_size_(default_single_window_size) {
  // Just for the purpose to allow QtMir (or unity8) to find this on our
  // /proc/*/cmdline
  // for proper confinement etc.
  flag(cli::make_flag(cli::Name{"desktop_file_hint"},
                      cli::Description{"Desktop file hint for QtMir/Unity8"},
                      desktop_file_hint_));
  flag(cli::make_flag(cli::Name{"single-window"},
                      cli::Description{"Start in single window mode."},
                      single_window_));
  flag(cli::make_flag(cli::Name{"window-size"},
                      cli::Description{"Size of the window in single window mode, e.g. --window-size=1024,768"},
                      window_size_));
  flag(cli::make_flag(cli::Name{"experimental"},
                      cli::Description{"Allows users to use experimental features"},
                      experimental_));
  flag(cli::make_flag(cli::Name{"run-multiple"},
                      cli::Description{"Allows more than one session-manager to run simultaneously, e.g. --run-multiple=<uniq_container_id>"},
                      container_id_));
  flag(cli::make_flag(cli::Name{"use-system-dbus"},
                      cli::Description{"Use system instead of session DBus"},
                      use_system_dbus_));
  flag(cli::make_flag(cli::Name{"adb-port"},
                      cli::Description{"adb port number"},
                      adb_port_));
  flag(cli::make_flag(cli::Name{"software-rendering"},
                      cli::Description{"Use software rendering instead of hardware accelerated GL rendering"},
                      use_software_rendering_));
  flag(cli::make_flag(cli::Name{"no-touch-emulation"},
                      cli::Description{"Disable touch emulation applied on mouse inputs"},
                      no_touch_emulation_));

  action([this](const cli::Command::Context &) {
    auto trap = core::posix::trap_signals_for_process(
        {core::posix::Signal::sig_term, core::posix::Signal::sig_int});
    trap->signal_raised().connect([this, trap](const core::posix::Signal &signal) {
      INFO("Signal %i received. Good night.", static_cast<int>(signal));
      qemu_pipe_connector_.reset();
      platform_->unset_window_manager();
      platform_->unset_renderer();
      trap->stop();
    });

    if (!experimental_) {
        ERROR("Experimental features selected, but --experimental flag not set");
      return EXIT_FAILURE;
    }
    
    std::string bindername="";
    if (!container_id_.empty()) {
      auto id = container_id_;
      size_t    begin = id.find_first_of (':');
      size_t    end = id.find_last_not_of('\0');
      id = id.substr(begin + 1, end);
      unique_ID = atoi(id.c_str());
      bindername = "/dev/binder" + id;
    }

    if (!fs::exists(bindername) || !fs::exists("/dev/ashmem")) {
        ERROR("Failed to start as either binder or ashmem kernel or encoder drivers are not loaded");
        return EXIT_FAILURE;
    }

    std::string pid_path = "/opt/openvmi/android-env/docker/data/";
    if (!container_id_.empty()) {
        auto    idnum = container_id_;
        size_t    stEnd = idnum.find_first_of(':');
        auto new_id = idnum.substr(0, stEnd);
        strcpy(unique_Name, new_id.c_str());
        pid_path = pid_path + new_id;
        INFO("unique_Name=%s",new_id);

        _G_stBuf = new_id;

        SystemConfiguration::instance().set_container_id(new_id, false);
    }

    utils::ensure_paths({pid_path});

    pid_t openvmi_pid = getpid();
    INFO("PID=%d", openvmi_pid);
    pid_path = pid_path + "/openvmi.pid";
    INFO("%s", pid_path);
    FILE *fp = NULL;
    fp = fopen(pid_path.c_str(), "w+");
    if (fp == NULL) {
        ERROR("open .openvmi.pid failed!");
        return EXIT_FAILURE;
    }

    fprintf(fp, "%d", openvmi_pid);
    fclose(fp);

    openvmiadbport = adb_port_;
    INFO("container id : %s", container_id_);
    INFO("openvmi adbport=%d", openvmiadbport);

    utils::ensure_paths({
        SystemConfiguration::instance().socket_dir(),
        SystemConfiguration::instance().input_device_dir(),
    });

    auto rt = Runtime::create();
    if (!rt.get()) {                       //add 20191105
        ERROR("new runtime failed!");
        utils::remove_paths({SystemConfiguration::instance().socket_dir(),
        SystemConfiguration::instance().input_device_dir()});
        return EXIT_FAILURE;
    }
    auto dispatcher = openvmi::common::create_dispatcher_for_runtime(rt);

    auto input_manager = std::make_shared<input::Manager>(rt);
    auto android_api_stub = std::make_shared<bridge::AndroidApiStub>();

    auto display_frame = graphics::Rect::Invalid;
    if (single_window_)
      display_frame = window_size_;

    const auto should_enable_touch_emulation = utils::get_env_value("OPENVMI_ENABLE_TOUCH_EMULATION", "true");
    if (should_enable_touch_emulation == "false" || no_touch_emulation_)
      no_touch_emulation_ = true;

    platform::Configuration platform_config;
    platform_config.single_window = single_window_;
    platform_config.no_touch_emulation = no_touch_emulation_;
    platform_config.display_frame = display_frame;

    platform_ = platform::create(utils::get_env_value("OPENVMI_PLATFORM", "sdl"),
                                     input_manager,
                                     platform_config);
    if (!platform_) {
        ERROR("platform::create failed!");
        return EXIT_FAILURE;
    }

    auto app_db = std::make_shared<application::Database>();

    std::shared_ptr<wm::Manager> window_manager;
    bool using_single_window = false;
    if (platform_->supports_multi_window() && !single_window_)
        window_manager = std::make_shared<wm::MultiWindowManager>(platform_, android_api_stub, app_db);
    else {
        INFO("using sigle window manager!");
        window_manager = std::make_shared<wm::SingleWindowManager>(platform_, display_frame, app_db);
        using_single_window = true;
    }

    const auto should_force_software_rendering = utils::get_env_value("OPENVMI_FORCE_SOFTWARE_RENDERING", "false");
    auto gl_driver = graphics::GLRendererServer::Config::Driver::Host;
    if (should_force_software_rendering == "true" || use_software_rendering_)
     gl_driver = graphics::GLRendererServer::Config::Driver::Software;

    graphics::GLRendererServer::Config renderer_config {
      gl_driver,
      single_window_
    };

    auto gl_server = std::make_shared<graphics::GLRendererServer>(renderer_config, window_manager);
    platform_->set_window_manager(window_manager);
    platform_->set_renderer(gl_server->renderer());
    window_manager->setup();

    auto app_manager = std::static_pointer_cast<application::Manager>(android_api_stub);
    if (!using_single_window) {
      // When we're not running single window mode we need to restrict ourself to
      // only launch applications in freeform mode as otherwise the window tracking
      // doesn't work.
      app_manager = std::make_shared<application::RestrictedManager>(
            android_api_stub, wm::Stack::Id::Freeform);
    }

    const auto socket_path = SystemConfiguration::instance().socket_dir();

    sensors_ = sensors::SensorsManager::create();

    // The qemu pipe is used as a very fast communication channel between guest
    // and host for things like the GLES emulation/translation, the RIL or ADB.
    qemu_pipe_connector_ =
        std::make_shared<network::PublishedSocketConnector>(
            utils::string_format("%s/qemu_pipe", socket_path), rt,
            std::make_shared<qemu::PipeConnectionCreator>(gl_server->renderer(), rt, sensors_));

    boost::asio::deadline_timer appmgr_start_timer(rt->service());

    auto bridge_connector = std::make_shared<network::PublishedSocketConnector>(
        utils::string_format("%s/openvmi_bridge", socket_path), rt,
        std::make_shared<rpc::ConnectionCreator>(
            rt, [&](const std::shared_ptr<network::MessageSender> &sender) {
              auto pending_calls = std::make_shared<rpc::PendingCallCache>();
              auto rpc_channel =
                  std::make_shared<rpc::Channel>(pending_calls, sender);
              // This is safe as long as we only support a single client. If we
              // support
              // more than one one day we need proper dispatching to the right
              // one.
              android_api_stub->set_rpc_channel(rpc_channel);

              auto server = std::make_shared<bridge::PlatformApiSkeleton>(
                  pending_calls, platform_, window_manager, app_db);
              server->register_boot_finished_handler([&]() {
                DEBUG("Android successfully booted");
                android_api_stub->ready().set(true);
                appmgr_start_timer.expires_from_now(default_appmgr_startup_delay);
                appmgr_start_timer.async_wait([&](const boost::system::error_code &err) {
                  if (err != boost::system::errc::success)
                    return;
                  launch_appmgr_if_needed(android_api_stub);
                });
              });
              return std::make_shared<bridge::PlatformMessageProcessor>(
                  sender, server, pending_calls);
            }));

    rt->start();
    trap->run();

    rt->stop();

    return EXIT_SUCCESS;
  });
}
