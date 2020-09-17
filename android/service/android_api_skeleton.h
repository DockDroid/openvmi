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

#ifndef OPENVMI_PLATFORM_API_SKELETON_H_
#define OPENVMI_PLATFORM_API_SKELETON_H_

#include "android/service/activity_manager_interface.h"

namespace google {
namespace protobuf {
class Closure;
} // namespace protobuf
} // namespace google

namespace core {
namespace posix {
class ChildProcess;
} // posix
} // core

namespace openvmi {
namespace protobuf {
namespace bridge {
class InstallApplication;
class LaunchApplication;
class SetDnsServers;
class SetFocusedTask;
class RemoveTask;
class ResizeTask;
} // namespace bridge
namespace rpc {
class Void;
} // namespace rpc
} // namespace protobuf
class AndroidApiSkeleton {
public:
    AndroidApiSkeleton();
    ~AndroidApiSkeleton();

    void launch_application(openvmi::protobuf::bridge::LaunchApplication const *request,
                            openvmi::protobuf::rpc::Void *response,
                            google::protobuf::Closure *done);

    void set_focused_task(openvmi::protobuf::bridge::SetFocusedTask const *request,
                          openvmi::protobuf::rpc::Void *response,
                          google::protobuf::Closure *done);

    void remove_task(openvmi::protobuf::bridge::RemoveTask const *request,
                     openvmi::protobuf::rpc::Void *response,
                     google::protobuf::Closure *done);

    void resize_task(openvmi::protobuf::bridge::ResizeTask const *request,
                     openvmi::protobuf::rpc::Void *response,
                     google::protobuf::Closure *done);

private:
    void wait_for_process(core::posix::ChildProcess &process,
                          openvmi::protobuf::rpc::Void *response);

    void connect_services();

    android::sp<android::BpActivityManager> activity_manager_;
};
} // namespace openvmi

#endif
