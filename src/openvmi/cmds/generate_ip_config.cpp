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

#include "openvmi/cmds/generate_ip_config.h"
#include "openvmi/logger.h"
#include "openvmi/wm/single_window_manager.h"
#include "openvmi/android/ip_config_builder.h"

#include <fstream>

openvmi::cmds::GenerateIpConfig::GenerateIpConfig()
    : CommandWithFlagsAndAction{cli::Name{"generate-ip-config"}, cli::Usage{"generate-ip-config"},
         cli::Description{"Create ipconfig.txt needed for network configuation"}} {

    flag(cli::make_flag(cli::Name{"ip"},
                      cli::Description{"IP address (defaults to 192.168.250.2) for DHCP use 'dhcp'"},
                      ip_));
    flag(cli::make_flag(cli::Name{"cidr"},
                      cli::Description{"Netmask using CIDR notation (defaults to C Class [24])"},
                      cidr_));
    flag(cli::make_flag(cli::Name{"gateway"},
                      cli::Description{"Gateway IP address (defaults to 192.168.250.1)"},
                      gateway_));
    flag(cli::make_flag(cli::Name{"dns"},
                      cli::Description{"Primary DNS server IP address (defaults to  8.8.8.8)"},
                      dns_));
    flag(cli::make_flag(cli::Name{"ipcfg"},
                      cli::Description{"network config file (defaults to  $(pwd)/ipconfig.txt)"},
                      ipcfg_path));

  action([this](const cli::Command::Context&) {

    if (ip_ == "dhcp" && !gateway_.empty()) {
        ERROR("Overriding Android's gateway settings when DHCP specified is not supported");
        return EXIT_FAILURE;
    }

    if (ip_ == "dhcp" && !dns_.empty()) {
        ERROR("Overriding Android's DNS settings when DHCP specified is not supported");
        return EXIT_FAILURE;
    }

    if (ip_.empty())
        ip_ = "192.168.250.2";

    if (!cidr_)
        cidr_ = 24;

    if (gateway_.empty())
        gateway_ = "192.168.250.1";

    if (dns_.empty())
        dns_ = "8.8.8.8";

    android::IpConfigBuilder ip_conf;

    ip_conf.set_version(android::IpConfigBuilder::Version::Version2);

    if (ip_ == "dhcp") {
        ip_conf.set_assignment(android::IpConfigBuilder::Assignment::DHCP);
    } else {
        ip_conf.set_assignment(android::IpConfigBuilder::Assignment::Static);
        ip_conf.set_link_address(ip_, cidr_);
        ip_conf.set_gateway(gateway_);
        ip_conf.set_dns_servers({dns_});
    }

    ip_conf.set_id(0);

    std::vector<std::uint8_t> buffer(512);
    common::BinaryWriter writer(buffer.begin(), buffer.end());
    const auto size = ip_conf.write(writer);

    std::string ip_conf_path;
    if (ipcfg_path.empty()) {
        ip_conf_path = "ipconfig.txt";
    } else {
        ip_conf_path = ipcfg_path;
    }
    std::ofstream f(ip_conf_path, std::ofstream::binary);
    if (f.is_open()) {
        f.write(reinterpret_cast<const char*>(buffer.data()), size);
        f.close();
    } else {
        ERROR("Failed to open %s for writing", ip_conf_path);
    }

    return EXIT_SUCCESS;
  });
}
