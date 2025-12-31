#pragma once

#include "parsing/buffer.h"
#include <cstdint>

const size_t MAX_BUF_SIZE = 1500;

class tcp {
  public:
    tcp(const std::string &hostname, uint16_t port);
    ~tcp();
    int setup(std::string &ip, uint16_t port);

    int send_all(buffer_t &msg);
    buffer_t receive();

  private:
    int sockfd;
    buffer_t buf;
};
