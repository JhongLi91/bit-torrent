#include "download/downloader.h"
#include "network/network_utils.h"
#include "parsing/torrent.h"
#include "tracker/peer.h"
#include <cassert>
#include <utility>
#include <vector>

downloader::downloader(class torrent &torrent, std::vector<peer> &peers)
    : torrent(std::move(torrent)), progress(torrent.pieces.size() / 20) {

    // register listener fd to event
    event.register_fd(torrent.server_fd);

    // make connection to all peers existing peers
    for (peer &peer : peers)
        connect(peer);

    // main loop
    while (!progress.is_complete()) {
        std::vector<int> events = event.poll_events();
        for (int e : events)
            if (e == torrent.server_fd)
                handle_accept();
            else
                connections.at(e).handle_event();
    }
}

void downloader::connect(const peer &peer) {
    tcp tcp(peer.ip, peer.port, false); // non blocking

    // register sockfd to eventloop
    int sockfd = tcp.get_sockfd();
    event.register_fd(sockfd);

    // map each peer socket with a new connection object
    connections.try_emplace(sockfd, tcp, peer, &progress, &torrent);
}

void downloader::handle_accept() {
    auto [client_fd, peer] = accept_client(torrent.server_fd);
    if (client_fd == -1)
        return;
    connect(peer);
}
