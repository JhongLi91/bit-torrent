#pragma once

#include "parsing/buffer.h"

namespace tcp {
void send_all(buffer &msg);
void receive_n(uint32_t n, buffer &buf);

int setup();

}; // namespace tcp
