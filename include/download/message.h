#pragma once

#include <string>

enum MESSAGE_TYPE {
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
void build_handshake(int fd, std::string &info_hash);
void build_message(int fd, MESSAGE_TYPE msg);
}; // namespace message
