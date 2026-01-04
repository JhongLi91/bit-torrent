#pragma once

#include "parsing/buffer.h"
#include <cstdint>
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

const uint8_t PSTRLEN = 19;
const std::string PSTR = "BitTorrent protocol";

namespace message {
buffer_t build_handshake(const std::string &info_hash, const std::string &peer_id);
buffer_t build_choke();
buffer_t build_unchoke();
buffer_t build_interested();
buffer_t build_uninterested();
buffer_t build_have(uint32_t piece_idx);
buffer_t build_bitfield(const buffer_t &bitfield);
buffer_t build_request(uint32_t idx, uint32_t begin, uint32_t len, uint8_t type);
buffer_t build_piece(uint32_t idx, uint32_t begin, buffer_t &block);

}; // namespace message
