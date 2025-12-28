#pragma once

#include "parsing/buffer.h"

namespace tcp {
void send_all(buffer_t &msg);
void receive_n(uint32_t n, buffer_t &buf);

int setup();

}; // namespace tcp
