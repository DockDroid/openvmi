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
#include "openvmi/sensors/sensors_data.h"
#include "openvmi/sensors/simulated_sensor_data.h"
#include "openvmi/logger.h"

#include <array>

namespace openvmi {
namespace sensors {

Data::Data(const int id, const int groupsize)
  : id_(id), groupsize_(groupsize) {}

Data::~Data() {}

void Data::pluck(const float samples[][3], unsigned int rows, float data[3]) {
  if (select_ >= rows - 1)
    updown_ = -1;
  if (select_ <= 0)
    updown_ = 1;

  select_ += updown_;

  data[0] = samples[select_][0];

  if (groupsize_ == 3) {
    data[1] = samples[select_][1];
    data[2] = samples[select_][2];
  }
}

void Data::get_data(float data[3]) {
  unsigned int data_len;

  switch (id_) {
  case ID_ACCELERATION :
    data_len = sizeof(simulated_accelerometer_data) / sizeof(simulated_accelerometer_data[0]);
    Data::pluck(simulated_accelerometer_data, data_len, data);
    break;
  case ID_MAGNETIC_FIELD :
    data_len = sizeof(simulated_magnetometer_data) / sizeof(simulated_magnetometer_data[0]);
    Data::pluck(simulated_magnetometer_data, data_len, data);
    break;
  case ID_ORIENTATION :
    data_len = sizeof(simulated_orientation_data) / sizeof(simulated_orientation_data[0]);
    Data::pluck(simulated_orientation_data, data_len, data);
    break;
  case ID_PROXIMITY :
    data_len = sizeof(simulated_proximity_data) / sizeof(simulated_proximity_data[0]);
    Data::pluck(simulated_proximity_data, data_len, data);
    break;
  default :
    WARNING("Sensor ID %d not supported");
  }
}

} // namespace sensors
} // namespace openvmi
