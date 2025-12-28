#include <iostream>
#include <string>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cout << "usage: bit_torrent <torrent_file>\n";
        exit(1);
    }

    // read torrent file
    // torrent torrent_file(argv[1]);

    // get peers from tracker
    // vector<peer> peers = tracker::get_peers();

    // init a downloader
    // downloader d(torrent_file, peers);

    // run downloader

    return 0;
}
