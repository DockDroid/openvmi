/*
 * Copyright (C) 2018 Linaro Ltd.
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

#ifndef OPENVMI_ROTATIONSTATUS_H_
#define OPENVMI_ROTATIONSTATUS_H_

// These APIs will be used to set and get the current screen
// orientation

int rotation_angle = 0;

void set_rotation_status (int status)
{
	rotation_angle = status;
}

int get_rotation_status ()
{
	return (rotation_angle);
}
#endif

