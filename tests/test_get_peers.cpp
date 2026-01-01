#include "parsing/torrent.h"
#include "spdlog/spdlog.h"
#include "tracker/tracker.h"
#include <gtest/gtest.h>
#include <iostream>
#include <vector>

TEST(TrackerIntegrationTest, FetchesPeersFromLiveTracker) {
    torrent t;
    t.info_hash = "51fe0a3e0f53991d5e38aa4571368797f45e0e9e";
    t.length = 4554303488;
    t.piece_length = 262144;
    t.announce_url = "http://tracker.opentrackr.org:1337/announce";

    // Act
    std::vector<peer> peers = tracker::get_peers(t);

    ASSERT_FALSE(peers.empty())
        << "Expected to receive at least one peer from the tracker, but got none.";

    for (size_t i = 0; i < peers.size(); i++) {
        spdlog::info("Peer {}: {}:{}\n", i + 1, peers[i].ip, peers[i].port);
    }
}
