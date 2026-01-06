#include "download/connection.h"
#include "download/message.h"
#include "network/tcp.h"
#include "parsing/torrent.h"
#include <memory>
#include <sys/socket.h>

connection::connection(class tcp &tcp, const struct peer &peer, std::shared_ptr<buffer_t> bitfield,
                       class torrent *torrent)
    : handshaked(false), am_choking(true), am_interested(false), peer_choking(true),
      peer_interested(false), tcp(std::move(tcp)), peer(peer), bitfield(bitfield),
      peer_bitfield(bitfield->size(), 0), torrent(torrent) {

    // sends handshake on connection creation
    buffer_t msg = message::build_handshake(torrent->info_hash, torrent->peer_id);
    tcp.send_all(msg);
}

void connection::handle_event() {}

bool connection::is_endgame() {
    double cnt = 0;
    for (char b : *bitfield)
        cnt += b - '0';

    // end game if more than 80 percent downloaded
    return 0.8 >= cnt / bitfield->size();
}

void connection::handle_endgame() {}
