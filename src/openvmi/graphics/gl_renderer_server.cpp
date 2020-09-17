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

#include "openvmi/graphics/gl_renderer_server.h"
#include "openvmi/graphics/emugl/RenderApi.h"
#include "openvmi/graphics/emugl/RenderControl.h"
#include "openvmi/graphics/emugl/Renderer.h"
#include "openvmi/graphics/layer_composer.h"
#include "openvmi/graphics/multi_window_composer_strategy.h"
#include "openvmi/graphics/single_window_composer_strategy.h"
#include "openvmi/logger.h"
#include "openvmi/wm/manager.h"

#include <boost/throw_exception.hpp>
#include <boost/filesystem.hpp>
#include <cstdarg>
#include <stdexcept>

namespace fs = boost::filesystem;

namespace {
void logger_write(const emugl::LogLevel &level, const char *format, ...) {
  (void)level;

  char message[2048];
  va_list args;

  va_start(args, format);
  vsnprintf(message, sizeof(message) - 1, format, args);
  va_end(args);

  switch (level) {
  case emugl::LogLevel::WARNING:
    WARNING("%s", message);
    break;
  case emugl::LogLevel::ERROR:
    ERROR("%s", message);
    break;
  case emugl::LogLevel::FATAL:
    FATAL("%s", message);
    break;
  case emugl::LogLevel::DEBUG:
    DEBUG("%s", message);
    break;
  case emugl::LogLevel::TRACE:
    TRACE("%s", message);
    break;
  default:
    break;
  }
}
}

namespace openvmi {
namespace graphics {
GLRendererServer::GLRendererServer(const Config &config, const std::shared_ptr<wm::Manager> &wm)
    : renderer_(std::make_shared<::Renderer>()) {
  std::shared_ptr<LayerComposer::Strategy> composer_strategy;
  if (config.single_window)
    composer_strategy = std::make_shared<SingleWindowComposerStrategy>(wm);
  else
    composer_strategy = std::make_shared<MultiWindowComposerStrategy>(wm);

  composer_ = std::make_shared<LayerComposer>(renderer_, composer_strategy);

  auto gl_libs = emugl::default_gl_libraries();
  if (config.driver == Config::Driver::Software) {
    auto swiftshader_path = fs::path(utils::get_env_value("SWIFTSHADER_PATH"));
    const auto snap_path = utils::get_env_value("SNAP");
    if (!snap_path.empty())
      swiftshader_path = fs::path(snap_path) / "lib" / "openvmi" / "swiftshader";
    if (!fs::exists(swiftshader_path))
      throw std::runtime_error("Software rendering is enabled, but SwiftShader library directory is not found.");

    gl_libs = std::vector<emugl::GLLibrary>{
      {emugl::GLLibrary::Type::EGL, (swiftshader_path / "libEGL.so").string()},
      {emugl::GLLibrary::Type::GLESv1, (swiftshader_path / "libGLES_CM.so").string()},
      {emugl::GLLibrary::Type::GLESv2, (swiftshader_path / "libGLESv2.so").string()},
    };
  }

  emugl_logger_struct log_funcs;
  log_funcs.coarse = logger_write;
  log_funcs.fine = logger_write;
  if (!emugl::initialize(gl_libs, &log_funcs, nullptr)) {
      ERROR("Failed to initialize OpenGL renderer");
    BOOST_THROW_EXCEPTION(std::runtime_error("Failed to initialize OpenGL renderer"));
  }

  renderer_->initialize(0);
  registerRenderer(renderer_);
  registerLayerComposer(composer_);
}

GLRendererServer::~GLRendererServer() { renderer_->finalize(); }
}  // namespace graphics
}  // namespace openvmi
