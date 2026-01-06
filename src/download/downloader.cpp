#include "download/downloader.h"
#include "parsing/torrent.h"
#include "tracker/peer.h"
#include <cassert>
#include <memory>
#include <utility>
#include <vector>

downloader::downloader(torrent &torrent, std::vector<peer> &peers)
    : torrent_file(std::move(torrent)),
      bitfield(std::make_shared<buffer_t>(torrent.pieces.size() / 20, 0)) {

    // make connection to all peers
    for (peer &peer : peers)
        connect(peer);

    // main loop
    while (!is_finished()) {
        std::vector<int> events = event.poll_events();
        for (int e : events)
            connections[e].handle_event();
    }
}

bool downloader::is_finished() {
    for (char b : *bitfield)
        if (b == '0')
            return false;
    return true;
}

void downloader::connect(const peer &peer) {
    tcp tcp(peer.ip, peer.port, false); // non blocking

    // register sockfd to eventloop
    int sockfd = tcp.get_sockfd();
    event.register_fd(sockfd);

    // map each peer socket with a new connection object
    connections.emplace(sockfd, connection(tcp, peer, bitfield, &torrent_file));
}
