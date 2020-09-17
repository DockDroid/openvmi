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

#ifndef OPENVMI_SENSORS_COMMON_H_
#define OPENVMI_SENSORS_COMMON_H_

#define  ID_NONE           (-1)
#define  ID_BASE           (0)
#define  ID_ACCELERATION   (ID_BASE+0)
#define  ID_MAGNETIC_FIELD (ID_BASE+1)
#define  ID_ORIENTATION    (ID_BASE+2)
#define  ID_TEMPERATURE    (ID_BASE+3)
#define  ID_PROXIMITY      (ID_BASE+4)
#define  ID_LIGHT          (ID_BASE+5)
#define  ID_PRESSURE       (ID_BASE+6)
#define  ID_HUMIDITY       (ID_BASE+7)

#define  SENSORS_ACCELERATION    (1 << ID_ACCELERATION)
#define  SENSORS_MAGNETIC_FIELD  (1 << ID_MAGNETIC_FIELD)
#define  SENSORS_ORIENTATION     (1 << ID_ORIENTATION)
#define  SENSORS_TEMPERATURE     (1 << ID_TEMPERATURE)
#define  SENSORS_PROXIMITY       (1 << ID_PROXIMITY)
#define  SENSORS_LIGHT           (1 << ID_LIGHT)
#define  SENSORS_PRESSURE        (1 << ID_PRESSURE)
#define  SENSORS_HUMIDITY        (1 << ID_HUMIDITY)

static constexpr const long header_size{4};
static constexpr const long data_size{48};

#endif // OPENVMI_SENSORS_COMMON_H_
