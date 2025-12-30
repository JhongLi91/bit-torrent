#include "network/tcp.h"
#include "network/network_utils.h"
#include "parsing/buffer.h"
#include "spdlog/spdlog.h"
#include <cstdint>
#include <spdlog/fmt/bin_to_hex.h>
#include <string>
#include <sys/socket.h>
#include <unistd.h>

tcp::tcp(const std::string &ip_address, uint16_t port) {
    buf.resize(MAX_BUF_SIZE);
    sockfd = make_client_socket(ip_address, port, true);
}

tcp::~tcp() { close(sockfd); }

int tcp::send_all(buffer_t &msg) {
    size_t sent = 0;
    while (sent < msg.size()) {
        auto n = send(sockfd, msg.data() + sent, msg.size() - sent, 0);
        if (n <= 0)
            return -1;
        sent += n;
    }
    spdlog::debug("Msg sent: {:X}", spdlog::to_hex(msg));
    return sent;
}

buffer_t tcp::receive_n(uint32_t n) {
    if (n > MAX_BUF_SIZE) {
        spdlog::debug("n too big on recv");
        return {};
    }

    uint32_t received = 0;
    while (received != n) {
        auto rv = recv(sockfd, buf.data() + received, n - received, 0);
        if (rv < 0) {
            spdlog::debug("{}", strerror(errno));
            return {};
        }
        if (rv == 0) {
            spdlog::debug("Connection closed {}", sockfd);
            return {};
        }
        received += rv;
    }
    buffer_t res(buf.data(), buf.data() + received);
    spdlog::debug("Msg received: {:X}", spdlog::to_hex(res));
    return res;
}
