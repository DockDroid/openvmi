/*
 * Copyright (C) 2017 Lee Jones <lee.jones@linaro.org>
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

#ifndef OPENVMI_CMDS_GEN_IP_CONFIG_H_
#define OPENVMI_CMDS_GEN_IP_CONFIG_H_

#include "openvmi/cli.h"

#include <string>

namespace openvmi {
namespace cmds {
class GenerateIpConfig : public cli::CommandWithFlagsAndAction {
 public:
  GenerateIpConfig();

 private:
  std::string    ip_;
  std::string    gateway_;
  std::string    dns_;
  uint32_t       cidr_;
  std::string    ipcfg_path;
};
}  // namespace cmds
}  // namespace openvmi

#endif
