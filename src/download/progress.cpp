#include "download/progress.h"
#include <cstdint>
#include <vector>

progress_t::progress_t(uint32_t n) {}

bool progress_t::is_endgame() { return pending.size() + num_received == total; }

bool progress_t::is_complete() { return num_received == total; }

void progress_t::update_have(uint32_t piece) {
    if (piece >= total)
        return;
    have[piece]++;
}

void progress_t::update_receive(uint32_t piece) {
    if (received[piece] == 1)
        return;
    received[piece] = 1;
    num_received++;
}

int progress_t::next_piece_to_request(buffer_t &peer_bitfield) {
    if (is_complete())
        return -1;

    std::vector<uint32_t> rarest;
    uint32_t least = INT32_MAX;

    for (uint32_t i = 0; i < total; i++) {
        if (have[i] < least) {
            least = have[i];
            rarest = {i};
        }
        else if (have[i] == least) {
            rarest.push_back(i);
        }
    }

    // randomly select only of the equally rare pieces
    return rarest[rd() % rarest.size()];
}
