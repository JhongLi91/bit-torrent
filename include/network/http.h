#pragma once

#include "network/tcp.h"
#include "parsing/buffer.h"
#include <string>
#include <utility>
#include <vector>

const std::string CRLF = "\r\n";

class http {
  public:
    http(const std::string &host, uint16_t port = 80,
         const std::string &user_agent = "BitTorrentClient/1.0");

    buffer_t get(const std::string &path,
                 const std::vector<std::pair<std::string, std::string>> &params);

  private:
    buffer_t get_data();

  private:
    const std::string host;
    uint16_t port;
    const std::string user_agent;

    tcp tcp;
};
