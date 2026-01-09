#pragma once

#include <cstdint>
#include <string>
#include <utility>
#include <vector>

std::pair<int, uint16_t> make_server_socket();
int make_client_socket(const std::string &ip_address, uint16_t port, bool blocking);
std::vector<std::string> resolve_hostname(const std::string &hostname);
