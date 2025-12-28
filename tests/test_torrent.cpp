#include "parsing/torrent.h"
#include <cassert>
#include <iostream>

#include <CommonCrypto/CommonDigest.h>

int main() {
    std::string path = "../assets/ubuntu-24.04.1-live-server-amd64.iso.torrent";
    torrent torr(path);

    std::string hash = torr.info_hash;
    std::string expected = "41e6cd50ccec55cd5704c5e3d176e7b59317a3fb";

    std::cout << "Calculated Info Hash: " << hash << "\n";
    std::cout << "Expected Info Hash:   " << expected << "\n";

    assert(hash == expected);
    assert(torr.announce_url == "https://torrent.ubuntu.com/announce");
    assert(torr.file_name == "ubuntu-24.04.1-live-server-amd64.iso");
    assert(torr.piece_length == 262144);

    std::cout << "Good\n";

    return 0;
}
