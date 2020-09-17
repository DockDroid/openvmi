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

#ifndef OPENVMI_SENSORS_MESSAGE_PROCESSOR_H_
#define OPENVMI_SENSORS_MESSAGE_PROCESSOR_H_

// LEE: Once building, start to strip out the unused headers

#include "openvmi/sensors/sensors_manager.h"
#include "openvmi/network/message_processor.h"
#include "openvmi/network/socket_messenger.h"

#include <mutex>

namespace openvmi {
namespace sensors {

class SensorsManager;

class SensorsMessageProcessor : public network::MessageProcessor,
                                public std::enable_shared_from_this<SensorsMessageProcessor> {
 public:
  SensorsMessageProcessor(
	  const std::shared_ptr<SensorsManager> &sensors,
	  const std::shared_ptr<network::SocketMessenger> &messenger);
  ~SensorsMessageProcessor();

  bool process_data(const std::vector<std::uint8_t> &data) override;
  void send_message(char *header, char *data, int datasize);

 private:
  mutable std::mutex mutex_;
  std::shared_ptr<SensorsManager> sensors_;
  std::shared_ptr<network::SocketMessenger> messenger_;

  void process_commands();
  void handle_command(const std::string &command);

  std::vector<std::uint8_t> buffer_;
};
}  // namespace sensors
}  // namespace openvmi

#endif
