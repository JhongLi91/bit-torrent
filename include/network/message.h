#pragma once

#include <string>

enum MESSAGE {
    CHOKE = 0,
    UNCHOKE = 1,
    INTERESTED = 2,
    UNINTERESTED = 3,
    HAVE = 4,
    BITFIELD = 5,
    REQUEST = 6,
    PIECE = 7,
    CANCEL = 8,
};

namespace message {
void send_tracker(int fd, std::string &info_hash);
void send_handshake(int fd, std::string &info_hash);

void send_message(int fd, MESSAGE msg);

}; // namespace message
