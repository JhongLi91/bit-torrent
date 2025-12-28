#include "parsing/torrent.h"
#include "parsing/bencoding.h"

#include <CommonCrypto/CommonDigest.h>
#include <fstream>
#include <spdlog/spdlog.h>
#include <sstream>

torrent::torrent(const std::string &path) {
    // load torrent file
    std::string data = loadFile(path);

    // decode and extract root
    auto root = parsing::bencoding::decode(data);
    auto &rootMap = std::get<std::map<std::string, parsing::bencoding::Bitem>>(root.val);
    if (rootMap.find("announce") == end(rootMap) || rootMap.find("info") == end(rootMap)) {
        spdlog::error("Error: Invalid torrent file");
        exit(1);
    }

    // extract info dictionary
    auto &info = std::get<std::map<std::string, parsing::bencoding::Bitem>>(rootMap["info"].val);

    // populate data
    announce_url = std::get<std::string>(rootMap["announce"].val);
    file_name = std::get<std::string>(info["name"].val);
    piece_length = std::get<ll>(info["piece length"].val);
    pieces = std::get<std::string>(info["pieces"].val);

    // re-encode info dictionary to get info hash
    std::string infoBytes = parsing::bencoding::encode(rootMap["info"]);
    info_hash = sha1(infoBytes);
}

bool torrent::verify_piece(const std::string &hash, uint32_t piece_idx) {
    return hash == pieces.substr(piece_idx * HASH_LENGTH, HASH_LENGTH);
}

// Private
std::string torrent::loadFile(const std::string &path) {
    std::ifstream file(path, std::ios::binary | std::ios::ate);
    if (!file) {
        spdlog::error("File not found: " + path);
        exit(1);
    }

    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);
    std::string buffer(size, '\0');
    if (!file.read(buffer.data(), size)) {
        spdlog::error("Read Error");
        exit(1);
    }
    return buffer;
}

std::string torrent::sha1(const std::string &input) {
    unsigned char hash[CC_SHA1_DIGEST_LENGTH];
    CC_SHA1(input.data(), input.size(), hash);

    std::stringstream ss;
    ss << std::hex << std::setfill('0');
    for (unsigned char c : hash) {
        ss << std::setw(2) << (int)c;
    }
    return ss.str();
}
