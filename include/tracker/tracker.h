#pragma once

#include "parsing/buffer.h"
#include "parsing/torrent.h"
#include "tracker/peer.h"
#include <vector>

namespace tracker {
std::vector<peer> get_peers(torrent &torrent);
}; // namespace tracker

std::vector<peer> extract_peers(buffer_t &buf);
