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


#include "openvmi/system_configuration.h"
#include "openvmi/utils.h"
#include "openvmi/build/config.h"

#include "external/xdg/xdg.h"

#include <cstring>

namespace fs = boost::filesystem;

namespace {
static std::string runtime_dir() {
  static std::string path;
  if (path.empty()) {
    const auto snap_user_common = openvmi::utils::get_env_value("SNAP_USER_COMMON");
    if (!snap_user_common.empty())
      path = (fs::path(snap_user_common) / "runtime").string();
    else {
 //     path = openvmi::utils::get_env_value("XDG_RUNTIME_DIR");
      path = "/opt/openvmi/android-socket";
      if (path.empty())
        BOOST_THROW_EXCEPTION(std::runtime_error("No runtime directory specified"));
    }
  }
  return path;
}
}

void openvmi::SystemConfiguration::set_container_id(const std::string &id, bool nothrow) {
  static std::string dir = openvmi::utils::string_format("%s/%s", runtime_dir(), id);
  if (fs::is_directory(fs::path(dir)) && !nothrow)
    BOOST_THROW_EXCEPTION(std::runtime_error("Container name provided already in use"));
  container_id = id;
}

void openvmi::SystemConfiguration::set_data_path(const std::string &path) {
  data_path = path;
}

fs::path openvmi::SystemConfiguration::data_dir() const {
  return data_path;
}

std::string openvmi::SystemConfiguration::container_name() const {
  return container_id;
}

std::string openvmi::SystemConfiguration::rootfs_dir() const {
  return (data_path / "rootfs").string();
}

std::string openvmi::SystemConfiguration::overlay_dir() const {
  return (data_path / "rootfs-overlay").string();
}

std::string openvmi::SystemConfiguration::combined_rootfs_dir() const {
  return (data_path / "combined-rootfs").string();
}

std::string openvmi::SystemConfiguration::log_dir() const {
  return (data_path / container_id / "logs").string();
}

std::string openvmi::SystemConfiguration::container_config_dir() const {
  return (data_path / container_id / "containers").string();
}

std::string openvmi::SystemConfiguration::container_socket_path() const {
  static std::string path;
  if (path.empty()) {
    const auto snap_common = openvmi::utils::get_env_value("SNAP_COMMON");
    if (!snap_common.empty())
      path = (fs::path(snap_common) / "sockets" / "openvmi-container.socket").string();
    else
      path = "/run/openvmi-container.socket";
  }
  return path;
}

std::string openvmi::SystemConfiguration::instance_dir() const {
  static std::string dir = openvmi::utils::string_format("%s/%s",
                               runtime_dir(), container_id);
  return dir;
}

std::string openvmi::SystemConfiguration::container_devices_dir() const {
  return (data_path / "devices").string();
}

std::string openvmi::SystemConfiguration::container_state_dir() const {
  return (data_path / "state").string();
}

std::string openvmi::SystemConfiguration::socket_dir() const {
  static std::string dir = openvmi::utils::string_format("%s/%s/sockets",
                               runtime_dir(), container_id);
  return dir;
}

std::string openvmi::SystemConfiguration::input_device_dir() const {
  static std::string dir = openvmi::utils::string_format("%s/%s/input",
                               runtime_dir(), container_id);
  return dir;
}

std::string openvmi::SystemConfiguration::application_item_dir() const {
  static auto dir = xdg::data().home() / "applications" / "openvmi" / container_id;
  if (openvmi::utils::get_env_value("OPENVMI_NO_DESKTOP_SUBDIR").length() > 0)
    dir = xdg::data().home() / "applications";
  return dir.string();
}

std::string openvmi::SystemConfiguration::resource_dir() const {
  return resource_path.string();
}

openvmi::SystemConfiguration& openvmi::SystemConfiguration::instance() {
  static SystemConfiguration config;
  return config;
}

openvmi::SystemConfiguration::SystemConfiguration() {
  auto detect_resource_path = [] () -> fs::path {
    const auto snap_path = utils::get_env_value("SNAP");
    if (!snap_path.empty()) {
      return fs::path(snap_path) / "usr" / "share" / "openvmi";
    } else {
      return openvmi::build::default_resource_path;
    }
  };

  resource_path = detect_resource_path();
  data_path = openvmi::build::default_data_path;
}
