#include "parsing/torrent.h"
#include "spdlog/spdlog.h"
#include "tracker/tracker.h"
#include <cassert>
#include <iostream>
#include <string>
#include <vector>

int main() {
    spdlog::set_level(spdlog::level::debug);

    torrent t;
    t.info_hash = "51fe0a3e0f53991d5e38aa4571368797f45e0e9e";
    t.length = 4554303488;
    t.piece_length = 262144;
    t.announce_url = "http://tracker.opentrackr.org:1337/announce";

    std::vector<peer> peers = tracker::get_peers(t);
    if (peers.empty())
        exit(1);

    for (size_t i = 0; i < peers.size(); i++) {
        std::cout << "  Peer " << i + 1 << ": " << peers[i].ip << ":" << peers[i].port << "\n";
    }

    spdlog::info("Success!");
    return 0;
}
