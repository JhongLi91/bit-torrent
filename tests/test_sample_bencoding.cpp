#include "parsing/bencoding.h"
#include <fstream>
#include <iomanip>
#include <iostream>

#include <CommonCrypto/CommonDigest.h>
#include <sstream>

std::string loadFile(const std::string &path) {
    std::ifstream file(path, std::ios::binary | std::ios::ate);
    if (!file)
        throw std::runtime_error("File not found: " + path);
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);
    std::string buffer(size, '\0');
    if (!file.read(buffer.data(), size))
        throw std::runtime_error("Read error");
    return buffer;
}

std::string sha1(const std::string &input) {
    unsigned char hash[CC_SHA1_DIGEST_LENGTH];
    CC_SHA1(input.data(), input.size(), hash);

    std::stringstream ss;
    ss << std::hex << std::setfill('0');
    for (unsigned char c : hash) {
        ss << std::setw(2) << (int)c;
    }
    return ss.str();
}

int main() {
    try {
        // 1. Load File
        std::string path = "../assets/ubuntu-24.04.1-live-server-amd64.iso.torrent";
        std::string data = loadFile(path);

        // 2. Decode
        auto root = parsing::bencoding::decode(data);

        // 3. Extract Info Dictionary
        auto &rootMap = std::get<std::map<std::string, parsing::bencoding::Bitem>>(root.val);

        if (rootMap.find("info") == rootMap.end()) {
            std::cerr << "Error: Invalid torrent file (no info dictionary)\n";
            exit(1);
        }

        const auto &infoNode = rootMap.at("info");

        // 4. Encode Info Dictionary back to bytes
        std::string infoBytes = parsing::bencoding::encode(infoNode);

        // 5. Hash it
        std::string hash = sha1(infoBytes);
        std::string expected = "41e6cd50ccec55cd5704c5e3d176e7b59317a3fb";

        std::cout << "Calculated Info Hash: " << hash << "\n";
        std::cout << "Expected Info Hash:   " << expected << "\n";

        if (hash == expected) {
            std::cout << "SUCCESS!\n";
        }
        else {
            std::cout << "FAILURE! Hashes do not match.\n";
            exit(1);
        }

    } catch (const std::exception &e) {
        std::cerr << "Exception: " << e.what() << "\n";
        exit(1);
    }

    return 0;
}
