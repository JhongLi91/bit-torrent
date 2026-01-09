#pragma once

#include "download/connection.h"
#include "download/eventloop.h"
#include "download/progress.h"
#include "parsing/torrent.h"
#include "tracker/peer.h"
#include <unordered_map>
#include <vector>

class downloader {

  public:
    downloader(torrent &torrent, std::vector<peer> &peers);

  private:
    bool is_endgame();
    bool is_finished();
    void connect(const peer &peer);
    void handle_accept();

  private:
    eventloop event;
    progress progress;
    torrent torrent;
    std::unordered_map<int, connection> connections;
};
