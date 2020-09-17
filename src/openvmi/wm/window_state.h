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

#ifndef OPENVMI_WM_WINDOW_STATE_H_
#define OPENVMI_WM_WINDOW_STATE_H_

#include "openvmi/graphics/rect.h"
#include "openvmi/wm/display.h"
#include "openvmi/wm/stack.h"
#include "openvmi/wm/task.h"

#include <string>
#include <vector>

namespace openvmi {
namespace wm {
class WindowState {
 public:
  typedef std::vector<WindowState> List;

  WindowState();
  WindowState(const Display::Id &display, bool has_surface,
              const graphics::Rect &frame, const std::string &package_name,
              const Task::Id &task, const Stack::Id &stack, int rotation);
  ~WindowState();

  Display::Id display() const { return display_; }
  bool has_surface() const { return has_surface_; }
  graphics::Rect frame() const { return frame_; }
  std::string package_name() const { return package_name_; }
  Task::Id task() const { return task_; }
  Stack::Id stack() const { return stack_; }
  int rotation_angle() const { return rotation_angle_; }


 private:
  Display::Id display_;
  bool has_surface_;
  graphics::Rect frame_;
  std::string package_name_;
  Task::Id task_;
  Stack::Id stack_;
  int rotation_angle_;
};
}  // namespace wm
}  // namespace openvmi

#endif
