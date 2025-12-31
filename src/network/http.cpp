#include "network/http.h"
#include "network/tcp.h"
#include "parsing/buffer.h"
#include "spdlog/spdlog.h"
#include <boost/url.hpp>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

http::http(const std::string &host, uint16_t port, const std::string &user_agent)
    : host(host), port(port), user_agent(user_agent), tcp(host, port) {}

buffer_t http::get(const std::string &path,
                   const std::vector<std::pair<std::string, std::string>> &params) {
    std::stringstream payload;

    // request line
    payload << "GET " << path;

    // append all params
    for (size_t i = 0; i < params.size(); i++) {
        payload << (i == 0 ? '?' : '&');
        payload << params[i].first;
        payload << '=';
        payload << boost::urls::encode(params[i].second, boost::urls::unreserved_chars);
    }

    // end of request line
    payload << " HTTP/1.1" << CRLF;

    // header
    payload << "Host: " << host << ":" << port << CRLF;
    payload << "User-Agent: " << user_agent << CRLF;
    payload << "Connection: close" << CRLF;

    // terminator
    payload << CRLF;

    buffer_t data(begin(payload.view()), end(payload.view()));
    tcp.send_all(data);

    return get_data();
}

// private
buffer_t http::get_data() {
    buffer_t res;
    while (true) {
        buffer_t data = tcp.receive();
        if (data.empty()) // closed or error
            break;
        res.insert(res.end(), data.begin(), data.end());
    }
    spdlog::debug("HTTP Received {} bytes total.", res.size());

    // trim off header and get data
    const std::vector<uint8_t> delimiter = {'\r', '\n', '\r', '\n'};

    auto it = std::search(res.begin(), res.end(), delimiter.begin(), delimiter.end());

    if (it == res.end()) {
        spdlog::error("Invalid HTTP response: No header separator found.");
        return {};
    }

    // advance iterator past the "\r\n\r\n" (4 bytes)
    std::advance(it, 4);

    // return everything after the separator
    return buffer_t(it, res.end());
}
