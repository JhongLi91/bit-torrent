#pragma once

#include <string>
#include <vector>

using ll = long long;

class torrent {
  public:
    torrent(std::string &filename);
    bool verify_piece(const std::string &hash, uint32_t piece_idx);

  public:
    std::string announce_url;
    std::string file_name;

    ll piece_length;
    std::vector<std::string> piece;

    std::string info_hash;
};
