#pragma once

#include <string>
#include <utility>
#include <vector>

using ll = long long;
const uint32_t HASH_LENGTH = 20;

class torrent {
  public:
    torrent() = default;
    torrent(const std::string &path);
    bool verify_piece(const std::string &hash, uint32_t piece_idx);
    std::pair<std::string, uint16_t> get_hostname_and_port();

    std::string get_unhex_info_hash();

  private:
    std::string loadFile(const std::string &path);
    std::string sha1(const std::string &input);

  public:
    std::string announce_url;
    std::string file_name;

    ll piece_length;
    std::string pieces;
    ll length;

    std::string info_hash;
};
