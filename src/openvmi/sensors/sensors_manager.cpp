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

#include "openvmi/sensors/sensors_common.h"
#include "openvmi/sensors/sensors_manager.h"
#include "openvmi/sensors/sensors_message_processor.h"

#include "openvmi/logger.h"

namespace openvmi {
namespace sensors {

#define  SUPPORTED_SENSORS    \
  SENSORS_ACCELERATION   |    \
  SENSORS_MAGNETIC_FIELD |    \
  SENSORS_ORIENTATION    |    \
  SENSORS_PROXIMITY

SensorsManager::SensorsManager() {
  accelerometer_ = std::make_shared<sensors::Sensor>("acceleration", ID_ACCELERATION, 3);
  magnetometer_ = std::make_shared<sensors::Sensor>("magnetic", ID_MAGNETIC_FIELD, 3);
  orientation_ = std::make_shared<sensors::Sensor>("orientation", ID_ORIENTATION, 3);
  proximity_ = std::make_shared<sensors::Sensor>("proximity", ID_PROXIMITY, 1);

  current_ = ID_NONE;
}

SensorsManager::~SensorsManager() {}

std::shared_ptr<SensorsManager> SensorsManager::create() {
    ////return std::shared_ptr<SensorsManager>(new SensorsManager());
    return std::make_shared<SensorsManager>();
}

void SensorsManager::set_message_processor(std::shared_ptr<SensorsMessageProcessor> processor)
{
  processor_ = processor;
  accelerometer_->set_message_processor(processor_);
  magnetometer_ ->set_message_processor(processor_);
  orientation_->set_message_processor(processor_);
  proximity_->set_message_processor(processor_);
}

int SensorsManager::handle_command(const std::string &command)
{
  DEBUG("Received Command: %s", command);

  if (command.find("list-sensors") != std::string::npos)
    return list_sensors();
  else if (command.find("set-delay:") != std::string::npos)
    return set_delay(command);
  else if (command.find("set:") != std::string::npos)
    return toggle_on_off(command);

  WARNING("Unhanded command: %s", command);
  return -1;
}

int SensorsManager::list_sensors() {
  char header[header_size + 1];
  char data[data_size + 1];
  size_t datasize;

  snprintf(data, data_size + 1, "%d", SUPPORTED_SENSORS);
  datasize = strlen(data);
  snprintf(header, header_size + 1, "%04lx", datasize);

  processor_->send_message(header, data, datasize);

  return 0;
}

int SensorsManager::toggle_on_off(const std::string &command) {
  std::string accelerometer = "set:acceleration";
  std::string magnetometer = "set:magnetic-field";
  std::string orientation = "set:orientation";
  std::string proximity = "set:proximity";
  bool value;

  if (command.find(accelerometer) != std::string::npos) {
    value = stoi(command.substr(accelerometer.size() + 1));
    accelerometer_->set_power(value);
    current_ = ID_ACCELERATION;
  } else if (command.find(magnetometer) != std::string::npos) {
    value = stoi(command.substr(magnetometer.size() + 1));
    magnetometer_->set_power(value);
    current_ = ID_MAGNETIC_FIELD;
  } else if (command.find(orientation) != std::string::npos) {
    value = stoi(command.substr(orientation.size() + 1));
    orientation_->set_power(value);
    current_ = ID_ORIENTATION;
  } else if (command.find(proximity) != std::string::npos) {
    value = stoi(command.substr(proximity.size() + 1));
    proximity_->set_power(value);
    current_ = ID_PROXIMITY;
  } else {
    WARNING("Sensor not supported %s", command);
    return -1;
  }

  return 0;
}

int SensorsManager::set_delay(const std::string &command) {
  std::string setdelay = "set-delay";
  int delay = stoi(command.substr(setdelay.size() + 1));

  switch (current_) {
  case ID_ACCELERATION:
    accelerometer_->set_delay(delay);
    break;
  case ID_MAGNETIC_FIELD:
    magnetometer_->set_delay(delay);
    break;
  case ID_ORIENTATION:
    orientation_->set_delay(delay);
    break;
  case ID_PROXIMITY:
    proximity_->set_delay(delay);
    break;
  default:
    WARNING("LEE: Delay setting attempt with no current sensor");
    return -1;
  }

  return 0;
}

} // namespace sensors
} // namespace openvmi
