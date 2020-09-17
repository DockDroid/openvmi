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

#ifndef OPENVMI_SENSORS_SENSOR_H_
#define OPENVMI_SENSORS_SENSOR_H_

#include "openvmi/sensors/sensors_data.h"
#include "openvmi/sensors/sensors_message_processor.h"

#include <thread>

namespace openvmi {
namespace sensors {

class SensorsMessageProcessor;

class Sensor {
 public:
    Sensor(const std::string name, const int id, const int groupsize);
    ~Sensor();

    void set_message_processor(std::shared_ptr<SensorsMessageProcessor> processor);
    void set_power(bool value);
    void set_delay(int delay);

 private:
    std::shared_ptr<sensors::Data> data_;

    void thread();
    std::thread thread_;
    std::shared_ptr<SensorsMessageProcessor> processor_;

    const std::string name_;
    bool powered_on_ = false;
    bool thread_running_ = false;
    unsigned int delay_ = 200; // Default delay between messages is 200ms
    int id_;
    int groupsize_;
};
}  // namespace sensors
}  // namespace openvmi

#endif // OPENVMI_SENSORS_SENSOR_H_
