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

#ifndef OPENVMI_DBUS_SKELETON_SERVICE_H_
#define OPENVMI_DBUS_SKELETON_SERVICE_H_

#include "openvmi/application/manager.h"
#include "openvmi/dbus/bus.h"
#include "openvmi/do_not_copy_or_move.h"

#include <atomic>
#include <memory>
#include <thread>

#include <systemd/sd-bus.h>

namespace openvmi {
namespace dbus {
namespace skeleton {
class ApplicationManager;
class Service : public DoNotCopyOrMove {
 public:
  static std::shared_ptr<Service> create_for_bus(const BusPtr& bus, const std::shared_ptr<openvmi::application::Manager> &impl);

  ~Service();

 private:
  Service(const BusPtr& bus, const std::shared_ptr<openvmi::application::Manager> &impl);

  BusPtr bus_;
  std::shared_ptr<application::Manager> application_manager_;
};
}  // namespace skeleton
}  // namespace dbus
}  // namespace openvmi

#endif
