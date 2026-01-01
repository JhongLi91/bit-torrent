#include "tracker/peer_id.h"
#include "spdlog/spdlog.h"
#include <string>

std::string generate_peer_id() {
    static const std::string pref = "-JL0001-";
    static const char *characters =
        "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

    std::string id(PEER_ID_LEN, '0');
    std::copy(begin(pref), end(pref), id.begin());

    for (std::size_t i = pref.size(); i < PEER_ID_LEN; i++) {
        id[i] = characters[rd() % 62];
    }
    spdlog::debug("Peer Id generated: {}", id);
    return id;
}
