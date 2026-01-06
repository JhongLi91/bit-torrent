#pragma once

#include "network/tcp.h"
#include "parsing/buffer.h"
#include "parsing/torrent.h"
#include "tracker/peer.h"
#include <memory>

class connection {

  public:
    connection(tcp &tcp, const peer &peer, std::shared_ptr<buffer_t> bitfield, torrent *torrent);
    void handle_event();

  private:
    bool is_endgame();
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

    peer peer;
    buffer_t peer_bitfield;
    std::shared_ptr<buffer_t> bitfield;

    torrent *torrent;
    tcp tcp;
};
