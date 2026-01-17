#include "download/message.h"
#include "parsing/buffer.h"
#include <cstdint>
#include <cstring>
#include <string>

buffer_t message::build_handshake(const std::string &info_hash, const std::string &peer_id) {
    buffer_t handshake;

    handshake.push_back(19);                                            // [0-1)
    handshake.insert(end(handshake), begin(PSTR), end(PSTR));           // [1-20)
    handshake.insert(end(handshake), 8, 0);                             // [20-29)
    handshake.insert(end(handshake), begin(info_hash), end(info_hash)); // [29-49)
    handshake.insert(end(handshake), begin(peer_id), end(peer_id));     // [49-69)

    return handshake;
}

buffer_t message::build_choke() { return buffer_t({0, 0, 0, 1, CHOKE}); }

buffer_t message::build_unchoke() { return buffer_t({0, 0, 0, 1, UNCHOKE}); }

buffer_t message::build_interested() { return buffer_t({0, 0, 0, 1, INTERESTED}); }

buffer_t message::build_uninterested() { return buffer_t({0, 0, 0, 1, UNINTERESTED}); }

buffer_t message::build_have(uint32_t piece_idx) {
    buffer_t msg = {0, 0, 0, 5, HAVE};

    piece_idx = htonl(piece_idx);
    const uint8_t *bytes = reinterpret_cast<const uint8_t *>(&piece_idx);
    msg.insert(end(msg), bytes, bytes + 4);

    return msg;
}

buffer_t message::build_bitfield(const buffer_t &bitfield) {
    uint32_t payload_len = 1 + bitfield.size(); // ID (1) + bitfield (N)
    buffer_t msg(4 + payload_len);              // 4 bytes for payload_len

    set4B(htonl(payload_len), msg, 0);               // payload_len
    set1B(BITFIELD, msg, 4);                         // id
    setNB(msg, bitfield.data(), bitfield.size(), 5); // bitfield

    return msg;
}

buffer_t message::build_request(uint32_t idx, uint32_t begin, uint32_t len, uint8_t type) {
    buffer_t msg(4 + 1 + 12); //  payload_len (4) + ID (1) + idx (4), begin (4), len (4)

    set4B(htonl(13), msg, 0);    // payload_len
    set1B(type, msg, 4);         // id
    set4B(htonl(idx), msg, 5);   // idx
    set4B(htonl(begin), msg, 9); // begin
    set4B(htonl(len), msg, 13);  // req len

    return msg;
}

buffer_t message::build_piece(uint32_t idx, uint32_t begin, buffer_t &block) {
    uint32_t payload_len = 1 + 8 + block.size(); // ID (1) + idx (4) + begin (4) + block (N)
    buffer_t msg(4 + payload_len);

    set4B(htonl(payload_len), msg, 0); // payload_len
    set1B(PIECE, msg, 4);              // id
    set4B(htonl(idx), msg, 5);         // idx
    set4B(htonl(begin), msg, 9);       // begin

    // TODO:: terribly ineffienct with copy
    setNB(msg, block.data(), block.size(), 13); // data

    return msg;
}
