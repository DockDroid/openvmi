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
#include "openvmi/sensors/sensors_message_processor.h"
#include "openvmi/sensors/sensors_sensor.h"
#include "openvmi/logger.h"

#include <thread>

namespace openvmi {
namespace sensors {

Sensor::Sensor(const std::string name, const int id, const int groupsize)
  : name_(name), id_(id), groupsize_(groupsize) {
  thread_ = std::thread(&Sensor::thread, this);
  data_ = std::make_shared<sensors::Data>(id_, groupsize_);
}

Sensor::~Sensor() {
  thread_running_ = false;
  thread_.join();
}

void Sensor::set_message_processor(std::shared_ptr<SensorsMessageProcessor> processor)
{
  processor_ = processor;
}

void Sensor::set_power(bool value) {
  powered_on_ = value;

  DEBUG("%s %s", powered_on_ ? "Enabling" : "Disabling", name_);
}

void Sensor::set_delay(int delay) {
  delay_ = delay;

  DEBUG("Set %s delay to: %d", name_, delay_);
}

void Sensor::thread() {
  thread_running_ = true;

  while (thread_running_) {
    float values[3];

    std::this_thread::sleep_for(std::chrono::milliseconds(delay_));

    if (powered_on_) {
      char header[header_size + 1];
      char data[data_size + 1];
      size_t datasize;

      data_->get_data(values);

      if (groupsize_ == 3)
	snprintf(data, data_size + 1, "%s:%.6f:%.6f:%.6f",
		 name_.c_str(), values[0], values[1], values[2]);
      else
	snprintf(data, data_size + 1, "%s:%.6f",
		 name_.c_str(), values[0]);

      datasize = strlen(data);
      snprintf(header, header_size + 1, "%04lx", datasize);

      processor_->send_message(header, data, datasize);
    } else
      delay_ = 200; // Default delay between messages is 200ms
  }
}

} // namespace sensors
} // namespace openvmi
