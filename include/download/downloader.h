#pragma once

#include "download/connection.h"
#include "download/event_loop.h"
#include "parsing/buffer.h"
#include "parsing/torrent.h"
#include "tracker/peer.h"
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

class downloader {

  public:
    downloader(torrent *tfile, std::vector<peer> &peers);

  private:
    void download();

    bool is_finished();
    bool is_endgame();
    void handle_endgame();

    /* returns the file descriptor for this connection */
    int connect(peer &peer);
    void close_connection(int fd);

    void disk_write(uint32_t block, const buffer_t &buf);
    void disk_read(uint32_t block, buffer_t &buf);

  private:
    torrent *torrent_file;
    std::string bit_field;

    std::unique_ptr<event_loop> event;
    std::unordered_map<int, std::unique_ptr<connection>> connections;
};
