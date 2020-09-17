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

#ifndef OPENVMI_SENSORS_DATA_H_
#define OPENVMI_SENSORS_DATA_H_

namespace openvmi {
namespace sensors {
class Data {
 public:
    Data(const int id, const int groupsize);
    ~Data();

    void get_data(float data[3]);

 private:
    void pluck(const float samples[][3], unsigned int rows, float data[3]);

    unsigned int select_ = 0;
    unsigned int updown_ = 0;
    const int id_;
    const int groupsize_;
};
}  // namespace sensors
}  // namespace openvmi

#endif // OPENVMI_SENSORS_DATA_H_
