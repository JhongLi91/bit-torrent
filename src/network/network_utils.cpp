#include "network/network_utils.h"
#include "spdlog/spdlog.h"
#include <arpa/inet.h>
#include <cstring>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <spdlog/spdlog.h>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <vector>

int make_client_socket(const std::string &ip_address, uint16_t port, bool blocking) {
    // TODO: udp
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        spdlog::error("Socket creation failed: {}", strerror(errno));
        return -1;
    }

    // non-Blocking
    int flags = fcntl(sockfd, F_GETFL, 0);
    if (flags == -1) {
        spdlog::error("fcntl get failed");
        close(sockfd);
        return -1;
    }
    if (fcntl(sockfd, F_SETFL, flags | (blocking ? 0 : O_NONBLOCK)) == -1) {
        spdlog::error("fcntl set nonblock failed");
        close(sockfd);
        return -1;
    }

    // setup address
    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);

    // check if IP is valid
    if (inet_pton(AF_INET, ip_address.c_str(), &server_addr.sin_addr) <= 0) {
        spdlog::error("Invalid IP address: {}", ip_address);
        close(sockfd);
        return -1;
    }

    // connect
    int ret = connect(sockfd, (sockaddr *)(&server_addr), sizeof(server_addr));
    if (ret < 0) {
        if (errno != EINPROGRESS) {
            spdlog::error("Connect failed: {}", strerror(errno));
            close(sockfd);
            return -1;
        }
    }
    spdlog::debug("Connection initiated to {}:{}", ip_address, port);

    return sockfd;
}

std::vector<std::string> resolve_hostname(const std::string &hostname) {

    struct addrinfo hints, *servinfo;
    std::memset(&hints, 0, sizeof(hints));

    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM; // TODO: allow udp protocol

    // DNS lookup
    int status;
    if ((status = getaddrinfo(hostname.c_str(), nullptr, &hints, &servinfo)) != 0) {
        spdlog::error("DNS Error: {}", gai_strerror(status));
        exit(1);
    }

    // get all resolved Ip
    std::vector<std::string> res;
    std::string ips = "";
    for (struct addrinfo *p = servinfo; p != nullptr; p = p->ai_next) {
        char ip_str[INET6_ADDRSTRLEN];
        void *addr;

        if (p->ai_family == AF_INET) {
            struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr;
            addr = &(ipv4->sin_addr);
        }
        else {
            struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)p->ai_addr;
            addr = &(ipv6->sin6_addr);
        }

        inet_ntop(p->ai_family, addr, ip_str, sizeof(ip_str));
        res.push_back(std::string(ip_str));
        ips += std::string(ip_str) + " ";
    }

    // free the linked list memory
    freeaddrinfo(servinfo);

    spdlog::debug("Resolved IP(s): {}", ips);
    return res;
}
