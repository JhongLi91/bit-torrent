#include "network/http.h"
#include "parsing/buffer.h"
#include <cassert>
#include <spdlog/spdlog.h>
#include <string>
#include <vector>

int main() {
    spdlog::set_level(spdlog::level::debug);

    std::string host = "httpbin.org";
    uint16_t port = 80;
    std::string user_agent = "CppBitTorrent/1.0";

    http client(host, port, user_agent);

    std::vector<std::pair<std::string, std::string>> params = {
        {"peer_id", "-BT0001-1234567890"},
        {"custom_msg", "hello world"}, // Should become "hello%20world" or "hello+world"
        {"port", "6881"}};

    // send Request
    buffer_t res = client.get("/get", params);
    if (res.empty())
        exit(1);

    std::string body(res.begin(), res.end());

    assert(body.find("{") != std::string::npos);
    assert(body.find("-BT0001-1234567890") != std::string::npos);
    assert(body.find("hello world") != std::string::npos);
    assert(body.find("6881") != std::string::npos);

    spdlog::info("Success!");
    return 0;
}
