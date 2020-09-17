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

#ifndef OPENVMI_INPUT_MANAGER_H_
#define OPENVMI_INPUT_MANAGER_H_

#include <map>
#include <memory>
#include <vector>

namespace openvmi {
class Runtime;
namespace input {
class Device;
class Manager {
 public:
  Manager(const std::shared_ptr<Runtime> &runtime);
  ~Manager();

  std::shared_ptr<Device> create_device();

 private:
  std::uint32_t next_id();
  std::string build_device_path(const std::uint32_t &id);

  std::shared_ptr<Runtime> runtime_;
  std::map<std::uint32_t, std::shared_ptr<Device>> devices_;

  std::string inputdir_;
  std::string instancedir_;
  std::vector<std::string> files_;
};
}  // namespace input
}  // namespace openvmi

#endif
