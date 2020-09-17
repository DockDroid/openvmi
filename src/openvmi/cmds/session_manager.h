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

#ifndef OPENVMI_CMDS_RUN_H_
#define OPENVMI_CMDS_RUN_H_

#include "openvmi/cli.h"

#include <functional>
#include <iostream>
#include <memory>

#include "openvmi/graphics/gl_renderer_server.h"
#include "openvmi/graphics/rect.h"
extern  unsigned int openvmiadbport; 
extern  unsigned int unique_ID;
extern  char unique_Name[50];
namespace openvmi {
namespace bridge {
class AndroidApiStub;
} // namespace bridge

namespace platform {
    class BasePlatform;
}  // namespace platform

namespace sensors {
    class SensorsManager;
} // namespace sensors
namespace network {
    class PublishedSocketConnector;
}  // namespace network

namespace cmds {
class SessionManager : public cli::CommandWithFlagsAndAction {
 public:
  SessionManager();

 private:
  void launch_appmgr_if_needed(const std::shared_ptr<bridge::AndroidApiStub> &android_api_stub);
  unsigned int adb_port_=6664;
  std::shared_ptr<platform::BasePlatform> platform_;
  std::shared_ptr<sensors::SensorsManager> sensors_;
  std::shared_ptr<network::PublishedSocketConnector> qemu_pipe_connector_;

  std::string desktop_file_hint_;
  bool single_window_ = false;
  graphics::Rect window_size_;
  bool experimental_ = false;
  std::string container_id_;
  bool use_system_dbus_ = false;
  bool use_software_rendering_ = false;
  bool no_touch_emulation_ = false;
};
}  // namespace cmds
}  // namespace openvmi

#endif
