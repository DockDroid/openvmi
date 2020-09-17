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
#include "openvmi/sensors/sensors_manager.h"
#include "openvmi/network/connections.h"

#include "openvmi/logger.h"

#include <mutex>

namespace openvmi {
namespace sensors {

SensorsMessageProcessor::SensorsMessageProcessor(
  const std::shared_ptr<SensorsManager> &sensors,
  const std::shared_ptr<network::SocketMessenger> &messenger)
  : sensors_(sensors),
    messenger_(messenger) {}

SensorsMessageProcessor::~SensorsMessageProcessor() {}

bool SensorsMessageProcessor::process_data(const std::vector<std::uint8_t> &data)
{
  sensors_->set_message_processor(shared_from_this());

  for (const auto &byte : data) buffer_.push_back(byte);

  process_commands();

  return true;
}

void SensorsMessageProcessor::process_commands() {
  int ret;

  while (true) {
    if (buffer_.size() < header_size) break;

    char header[header_size] = {0};
    ::memcpy(header, buffer_.data(), header_size);

    unsigned int body_size = 0;
    ::sscanf(header, "%04x", &body_size);

    unsigned int total_size = header_size + body_size;
    if (buffer_.size() < total_size) break;

    std::string command;
    command.insert(0,
                   reinterpret_cast<const char *>(buffer_.data()) + header_size,
                   body_size);

    ret = sensors_->handle_command(command);
    if (ret) {
      DEBUG("Trashed messages, clearing buffer");
      buffer_.clear();
      return;
    }

    buffer_.erase(buffer_.begin(), buffer_.begin() + total_size);

    const auto remaining = buffer_.size() - total_size;
    if (remaining <= 0) break;
  }
}

void SensorsMessageProcessor::send_message(char *header, char *data, int datasize) {
  std::lock_guard<decltype(mutex_)> lock(mutex_);

  messenger_->send(header, header_size);
  messenger_->send(data, datasize);
  messenger_->send(static_cast<const char *>("0004wake"), 8);
}

} // namespace sensors
} // namespace openvmi
