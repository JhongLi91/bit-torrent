#pragma once

#include "parsing/buffer.h"
#include "parsing/torrent.h"
#include "tracker/peer.h"
#include <vector>

namespace tracker {
std::vector<peer_t> get_peers(torrent &torrent);
}; // namespace tracker

std::vector<peer_t> extract_peers(buffer_t &buf);
