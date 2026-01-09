#pragma once

#include "download/job.h"
#include "network/tcp.h"
#include "parsing/buffer.h"
#include "parsing/torrent.h"
#include "tracker/peer.h"

class connection {

  public:
    connection(connection &&) = default;
    connection(tcp &tcp, const peer &peer, struct progress *progress, torrent *torrent);
    void handle_event();

  private:
    void handle_endgame();

    void disk_write(uint32_t block, const buffer_t &buf);
    void disk_read(uint32_t block, buffer_t &buf);

  private:
    // peer states
    bool handshaked;
    bool am_choking;
    bool am_interested;
    bool peer_choking;
    bool peer_interested;

    torrent *torrent;
    progress *progress;

    peer peer;
    buffer_t peer_bitfield;
    job j;
    tcp tcp;
};
