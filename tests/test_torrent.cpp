#include "parsing/torrent.h"
#include <fstream>
#include <gtest/gtest.h>
#include <string>

TEST(TorrentTest, ParsesUbuntuTorrentFile) {
    std::string path = "../assets/ubuntu-24.04.1-live-server-amd64.iso.torrent";

    std::ifstream f(path);
    ASSERT_TRUE(f.good()) << "Test failed to find the assets file at: " << path;

    torrent torr(path);

    std::string expected_hash = "41e6cd50ccec55cd5704c5e3d176e7b59317a3fb";

    EXPECT_EQ(torr.info_hash, expected_hash);
    EXPECT_EQ(torr.announce_url, "https://torrent.ubuntu.com/announce");
    EXPECT_EQ(torr.file_name, "ubuntu-24.04.1-live-server-amd64.iso");
    EXPECT_EQ(torr.piece_length, 262144);

    auto [hostname, port] = torr.get_hostname_and_port();
    EXPECT_EQ(hostname, "torrent.ubuntu.com");
    EXPECT_EQ(port, 80);
}

TEST(TorrentTest, ParsesHostnameAndPortFromUrl) {
    torrent torr;

    torr.announce_url = "https://torrent.ubuntu.com:6969/announce";
    auto [hostname, port] = torr.get_hostname_and_port();

    EXPECT_EQ(hostname, "torrent.ubuntu.com");
    EXPECT_EQ(port, 6969);

    torr.announce_url = "http://tracker.opentrackr.org/announce";
    auto [host2, port2] = torr.get_hostname_and_port();
    EXPECT_EQ(host2, "tracker.opentrackr.org");
    EXPECT_EQ(port2, 80);
}
