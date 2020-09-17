/*
 * Copyright © 2012 Canonical Ltd.
 *           © 2016 Simon Fels <morphis@gravedo.de>
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License version 3,
 * as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Authored by: Alan Griffiths <alan@octopull.co.uk>
 */

#include "openvmi/rpc/channel.h"
#include "openvmi/common/variable_length_array.h"
#include "openvmi/network/message_sender.h"
#include "openvmi/rpc/constants.h"
#include "openvmi/rpc/pending_call_cache.h"

#include "openvmi_rpc.pb.h"

namespace openvmi {
namespace rpc {
Channel::Channel(const std::shared_ptr<PendingCallCache> &pending_calls,
                 const std::shared_ptr<network::MessageSender> &sender)
    : pending_calls_(pending_calls), sender_(sender) {}

Channel::~Channel() {}

void Channel::call_method(std::string const &method_name,
                          google::protobuf::MessageLite const *parameters,
                          google::protobuf::MessageLite *response,
                          google::protobuf::Closure *complete) {
  auto const &invocation = invocation_for(method_name, parameters);
  pending_calls_->save_completion_details(invocation, response, complete);
  send_message(MessageType::invocation, invocation);
}

void Channel::send_event(google::protobuf::MessageLite const &event) {
  VariableLengthArray<2048> buffer{static_cast<size_t>(event.ByteSize())};
  event.SerializeWithCachedSizesToArray(buffer.data());

  openvmi::protobuf::rpc::Result response;
  response.add_events(buffer.data(), buffer.size());

  send_message(MessageType::response, response);
}

protobuf::rpc::Invocation Channel::invocation_for(
    std::string const &method_name,
    google::protobuf::MessageLite const *request) {
  openvmi::VariableLengthArray<2048> buffer{
      static_cast<size_t>(request->ByteSize())};

  request->SerializeWithCachedSizesToArray(buffer.data());

  openvmi::protobuf::rpc::Invocation invoke;

  invoke.set_id(next_id());
  invoke.set_method_name(method_name);
  invoke.set_parameters(buffer.data(), buffer.size());
  invoke.set_protocol_version(1);

  return invoke;
}

void Channel::send_message(const std::uint8_t &type,
                           google::protobuf::MessageLite const &message) {
  const size_t size = message.ByteSize();
  const unsigned char header_bytes[header_size] = {
      static_cast<unsigned char>((size >>16) & 0xff),
      static_cast<unsigned char>((size >> 8) & 0xff),
      static_cast<unsigned char>((size >> 0) & 0xff), type,
  };

  std::vector<std::uint8_t> send_buffer(sizeof(header_bytes) + size);
  std::copy(header_bytes, header_bytes + sizeof(header_bytes),
            send_buffer.begin());
  message.SerializeToArray(send_buffer.data() + sizeof(header_bytes), size);

  try {
    std::lock_guard<std::mutex> lock(write_mutex_);
    sender_->send(reinterpret_cast<const char *>(send_buffer.data()),
                  send_buffer.size());
  } catch (std::runtime_error const &) {
    notify_disconnected();
    throw;
  }
}

void Channel::notify_disconnected() { pending_calls_->force_completion(); }

std::uint32_t Channel::next_id() {
  static std::uint32_t next_message_id = 0;
  return next_message_id++;
}
}  // namespace rpc
}  // namespace openvmi
