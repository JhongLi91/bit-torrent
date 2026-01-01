#include "network/tcp.h"
#include "network/network_utils.h"
#include "parsing/buffer.h"
#include "spdlog/spdlog.h"
#include <cstdint>
#include <spdlog/fmt/bin_to_hex.h>
#include <string>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>

tcp::tcp(const std::string &hostname, uint16_t port) : hostname(hostname), port(port), sockfd(-1) {
    buf.resize(MAX_BUF_SIZE);
    std::vector<std::string> ips = resolve_hostname(hostname);

    for (std::string &ip : ips) {
        sockfd = make_client_socket(ip, port, true);
        if (sockfd != -1)
            break;
    }
    if (sockfd == -1) {
        spdlog::error("Failed to connect to {}:{} via tcp", hostname, port);
        throw std::runtime_error("Error in creating tcp connection");
    }
    spdlog::debug("Connection established with {}:{}", hostname, port);
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
    spdlog::debug("sent {} bytes to {}:{}", sent, hostname, port);
    return sent;
}

buffer_t tcp::receive() {
    auto rv = recv(sockfd, buf.data(), MAX_BUF_SIZE, 0);
    if (rv < 0) {
        spdlog::debug("{}", strerror(errno));
        return {};
    }
    if (rv == 0) {
        spdlog::debug("Connection closed {}", sockfd);
        return {};
    }
    buffer_t res(buf.data(), buf.data() + rv);
    spdlog::debug("received {} bytes from {}:{}", rv, hostname, port);
    return res;
}
