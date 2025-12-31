#include "network/tcp.h"
#include "parsing/buffer.h"
#include "spdlog/spdlog.h"
#include <cassert>
#include <string>

int main() {
    spdlog::set_level(spdlog::level::debug);

    // create client to tcpbin.com
    tcp client("tcpbin.com", 4242);

    std::string msg = "hello, world\n";
    buffer_t send_buf(msg.begin(), msg.end());

    if (client.send_all(send_buf) < 0) {
        spdlog::error("Send failed! Error: {}", strerror(errno));
        exit(1);
    }

    auto rv = client.receive();
    assert(!rv.empty());

    for (int i = 0; i < msg.size(); i++) {
        assert(send_buf[i] == rv[i]);
    }

    return 0;
}
