#pragma once

#include "parsing/buffer.h"
#include <cstdint>
#include <vector>

class progress_t {

  public:
    progress_t(uint32_t n);
    bool is_endgame();
    bool is_complete();
    int next_piece_to_request(buffer_t &peer_bitfield);
    void update_have(uint32_t piece);
    void update_receive(uint32_t piece);

  public:
    uint32_t total;
    uint32_t num_received;

    buffer_t pending;
    buffer_t received;
    std::vector<uint32_t> have;
};
