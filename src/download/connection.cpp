#include "download/connection.h"
#include "download/message.h"
#include "download/progress.h"
#include "network/tcp.h"
#include "parsing/torrent.h"
#include <sys/socket.h>

connection::connection(class tcp &tcp, const peer_t &peer, progress_t *progress,
                       class torrent *torrent)
    : handshaked(false), am_choking(true), am_interested(false), peer_choking(true),
      peer_interested(false), tcp(std::move(tcp)), peer(peer), progress(progress),
      peer_bitfield(progress->total), torrent(torrent) {

    // sends handshake on connection creation
    buffer_t msg = message::build_handshake(torrent->info_hash, torrent->peer_id);
    tcp.send_all(msg);
}

void connection::handle_event() {
    if (progress->is_endgame()) {
        handle_endgame();
        return;
    }
}

void connection::handle_endgame() {}
