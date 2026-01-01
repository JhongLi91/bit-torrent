#include "tracker/tracker.h"
#include "network/http.h"
#include "parsing/bencoding.h"
#include "parsing/buffer.h"
#include "parsing/torrent.h"
#include "spdlog/spdlog.h"
#include "tracker/peer.h"
#include "tracker/peer_id.h"
#include <boost/algorithm/hex.hpp>
#include <string>
#include <utility>
#include <vector>

std::vector<peer> tracker::get_peers(torrent &torrent) {
    auto [host, port] = torrent.get_hostname_and_port();
    http client(host, port);

    std::vector<std::pair<std::string, std::string>> params;
    params.push_back({"info_hash", torrent.get_unhex_info_hash()});
    params.push_back({"peer_id", generate_peer_id()});
    params.push_back({"port", "6881"});
    params.push_back({"uploaded", "0"});
    params.push_back({"downloaded", "0"});
    params.push_back({"compact", "1"});
    params.push_back({"left", std::to_string(torrent.length)});

    buffer_t data = client.get("/announce", params);

    // decoding bencoded data
    std::string_view view(reinterpret_cast<const char *>(data.data()), data.size());
    auto root = parsing::bencoding::decode(view);
    auto &dict = std::get<parsing::bencoding::Bmap>(root.val);

    const std::string &peers_blob = std::get<std::string>(dict["peers"].val);
    buffer_t peers_buf(peers_blob.begin(), peers_blob.end());

    return extract_peers(peers_buf);
}

std::vector<peer> extract_peers(buffer_t &buf) {
    if (buf.size() % 6 != 0) {
        spdlog::error("Peers blob size not modulo 6: {}", buf.size());
        return {};
    }

    std::vector<peer> peers;
    for (uint32_t i = 0; i < buf.size(); i += 6) {
        uint8_t oct1 = parser::buffer::get1B(buf, i);
        uint8_t oct2 = parser::buffer::get1B(buf, i + 1);
        uint8_t oct3 = parser::buffer::get1B(buf, i + 2);
        uint8_t oct4 = parser::buffer::get1B(buf, i + 3);

        uint16_t port = parser::buffer::get2B(buf, i + 4);

        std::string ip = fmt::format("{}.{}.{}.{}", oct1, oct2, oct3, oct4);
        peers.emplace_back(ip, port);
    }

    spdlog::info("Tracker returned {} peers.", peers.size());
    return peers;
}
