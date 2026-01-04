#pragma once

#include "download/connection.h"
#include "download/eventloop.h"
#include "parsing/buffer.h"
#include "parsing/torrent.h"
#include "tracker/peer.h"
#include <memory>
#include <unordered_map>
#include <vector>

class downloader {

  public:
    downloader(torrent &torrent, std::vector<peer> &peers);

  private:
    bool is_finished();
    bool is_endgame();
    void handle_endgame();

    void connect(const peer &peer);
    void close_connection(int fd);

  private:
    torrent torrent_file;
    std::shared_ptr<buffer_t> bitfield;

    eventloop event;
    std::unordered_map<int, connection> connections;
};
