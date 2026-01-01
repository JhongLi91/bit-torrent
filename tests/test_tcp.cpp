#include "network/tcp.h"
#include "parsing/buffer.h"
#include <gtest/gtest.h>
#include <string>

TEST(TcpIntegrationTest, EchoServerReturnsSameData) {
    tcp client("tcpbin.com", 4242);

    std::string msg = "hello, world\n";
    buffer_t send_buf(msg.begin(), msg.end());

    ssize_t sent_bytes = client.send_all(send_buf);
    ASSERT_GE(sent_bytes, 0) << "Send failed! Check internet connection or if tcpbin.com is down.";

    auto rv = client.receive();

    ASSERT_FALSE(rv.empty()) << "Received empty response from echo server.";
    EXPECT_EQ(send_buf, rv) << "Received data did not match sent data.";
}
