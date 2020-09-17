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

#include <signal.h>
#include <sys/prctl.h>

#include "openvmi/system_configuration.h"
#include "openvmi/daemon.h"
#include "openvmi/logger.h"

#include "openvmi/cmds/session_manager.h"
#include "openvmi/cmds/system_info.h"
#include "openvmi/cmds/generate_ip_config.h"
#include "openvmi/cmds/version.h"
#include "openvmi/cmds/wait_ready.h"

#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;

namespace openvmi {
Daemon::Daemon()
    : cmd{cli::Name{"openvmi"}, cli::Usage{"openvmi"},
          cli::Description{"The Android in a Box runtime"}} {
     cmd.command(std::make_shared<cmds::Version>())
     .command(std::make_shared<cmds::SessionManager>())
     .command(std::make_shared<cmds::GenerateIpConfig>())
     .command(std::make_shared<cmds::SystemInfo>())
     .command(std::make_shared<cmds::WaitReady>());

  Log().Init(openvmi::Logger::Severity::kWarning);

  const auto log_level = utils::get_env_value("OPENVMI_LOG_LEVEL", "");
  if (!log_level.empty() && !Log().SetSeverityFromString(log_level))
    WARNING("Failed to set logging severity to '%s'", log_level);
}

int Daemon::Run(const std::vector<std::string> &arguments) try {
  auto argv = arguments;
  if (arguments.size() == 0) argv = {"run"};
  return cmd.run({std::cin, std::cout, argv});
} catch (std::exception &err) {
  ERROR("%s", err.what());

  return EXIT_FAILURE;
}
}  // namespace openvmi
