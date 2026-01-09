#pragma once

#include <cstdint>
#include <string>
#include <utility>

using ll = long long;
const uint32_t HASH_LENGTH = 20;

class torrent {
  public:
    torrent();
    torrent(const std::string &path);

    ~torrent();

    bool verify_piece(const std::string &hash, uint32_t piece_idx);
    std::pair<std::string, uint16_t> get_hostname_and_port();

    std::string get_unhex_info_hash();

  private:
    std::string loadFile(const std::string &path);
    std::string sha1(const std::string &input);

  public:
    int server_fd;
    uint16_t port;

    std::string peer_id;

    std::string announce_url;
    std::string file_name;

    ll length;
    ll piece_length;
    std::string pieces;

    std::string info_hash;
};
