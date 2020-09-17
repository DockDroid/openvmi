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

#include "openvmi/platform/null/platform.h"
#include "openvmi/wm/window.h"
#include "openvmi/logger.h"

namespace {
class NullWindow : public openvmi::wm::Window {
 public:
  NullWindow(const openvmi::wm::Task::Id &task,
             const openvmi::graphics::Rect &frame,
             const std::string &title)
      : openvmi::wm::Window(nullptr, task, frame, title) {}
};
}

namespace openvmi {
namespace platform {
NullPlatform::NullPlatform() {}

std::shared_ptr<wm::Window> NullPlatform::create_window(
    const openvmi::wm::Task::Id &task, const openvmi::graphics::Rect &frame, const std::string &title) {
  return std::make_shared<::NullWindow>(task, frame, title);
}

void NullPlatform::set_clipboard_data(const ClipboardData &data) {
  (void)data;
  ERROR("Not implemented");
}

NullPlatform::ClipboardData NullPlatform::get_clipboard_data() {
  ERROR("Not implemented");
  return ClipboardData{};
}

void NullPlatform::set_renderer(const std::shared_ptr<Renderer> &renderer) {
  (void) renderer;
  ERROR("Not implemented");
}

void NullPlatform::set_window_manager(const std::shared_ptr<wm::Manager> &window_manager) {
  (void) window_manager;
  ERROR("Not implemented");
}

void NullPlatform::unset_renderer() {
  ERROR("Not implemented");
}

void NullPlatform::unset_window_manager() {
  ERROR("Not implemented");
}

bool NullPlatform::supports_multi_window() const {
  return false;
}
}  // namespace wm
}  // namespace openvmi
