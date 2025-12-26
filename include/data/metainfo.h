#pragma once

#include <string>
#include <vector>

using ll = long long;

struct file {
    ll length;        // Total file size (in bytes)
    std::string path; // For multi files
};

struct metainfo {
    std::string announce; // The Tracker URL

    ll pieceLength;     // Number of bytes per piece
    std::string pieces; // Massive binary string of concatenated SHA-1 hashes

    std::string name; // Filename or Directory name
    ll length;        // Total file size (in bytes)
    std::optional<std::vector<file>> files;
};
