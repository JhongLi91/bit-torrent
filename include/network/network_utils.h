#pragma once

#include <string>
#include <vector>

int make_client_socket(const std::string &ip_address, uint16_t port);
std::vector<std::string> resolve_hostname(const std::string &hostname);
