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

#ifndef OPENVMI_DBUS_INTERFACE_H_
#define OPENVMI_DBUS_INTERFACE_H_

#include <string>
#include "openvmi/system_configuration.h"

namespace openvmi {
namespace dbus {
namespace interface {
struct Service {
  static inline const char* path() { return "/org/openvmi"; }
  static inline const char * name() {
      static std::string id = SystemConfiguration::instance().container_name();
      std::string str;
      if (!id.empty())
        str = "org.openvmi." + id;
      else
        str = "org.openvmi";
      return str.c_str();
    }
};
struct ApplicationManager {
    static inline const char * name() {
      static std::string id = SystemConfiguration::instance().container_name();
      std::string str;
      if (!id.empty())
        str = "org.openvmi.ApplicationManager" + id;
      else
        str = "org.openvmi.ApplicationManager";
      return str.c_str();
    }
  struct Methods {
      struct Launch {
        static inline const char* name() { return "Launch"; }
      };
  };
  struct Properties {
    struct Ready {
      static inline const char* name() { return "Ready"; }
    };
  };
};
}  // namespace interface
}  // namespace dbus
}  // namespace openvmi

#endif
