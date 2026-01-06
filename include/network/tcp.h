#pragma once

#include "parsing/buffer.h"
#include <cstdint>

const size_t MAX_BUF_SIZE = 1500;

class tcp {
  public:
    tcp(const std::string &hostname, uint16_t port, bool blocking);
    ~tcp();

    int get_sockfd();
    int send_all(buffer_t &msg);
    buffer_t receive();

  private:
    const std::string hostname;
    uint16_t port;

    int sockfd;
    buffer_t buf;
};
