#pragma once

#include <string>

using ll = long long;

struct TorrentFile {
    std::string announce; // The Tracker URL
    ll length;            // Total file size (in bytes)
    std::string name;     // Filename
    ll pieceLength;       // Number of bytes per piece
    std::string pieces;   // Massive binary string of concatenated SHA-1 hashes
    std::string infoHash; // The 20-byte ID of this torrent
};
