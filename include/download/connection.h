#pragma once

#include "tracker/peer.h"
#include "tracker/peer_id.h"
#include <memory>

class connection {

  public:
    connection(int fd, peer &peer);
    void handle_event();

  private:
    bool handshaked;

    bool am_choking;
    bool am_interested;
    bool peer_choking;
    bool peer_interested;

    int fd;
    std::unique_ptr<peer> peer;
    peer_id peer_id;
};
