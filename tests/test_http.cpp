#include "network/http.h"
#include "parsing/buffer.h"
#include <gtest/gtest.h>
#include <string>
#include <vector>

TEST(HttpIntegrationTest, SendsGetRequestWithParams) {
    std::string host = "httpbin.org";
    uint16_t port = 80;
    std::string user_agent = "CppBitTorrent/1.0";

    http client(host, port, user_agent);

    std::vector<std::pair<std::string, std::string>> params = {
        {"peer_id", "-BT0001-1234567890"}, {"custom_msg", "hello world"}, {"port", "6881"}};

    buffer_t res = client.get("/get", params);

    ASSERT_FALSE(res.empty()) << "HTTP response was empty. Network down or host unreachable?";

    std::string body(res.begin(), res.end());

    EXPECT_NE(body.find("{"), std::string::npos) << "Response body does not look like JSON.";

    EXPECT_NE(body.find("-BT0001-1234567890"), std::string::npos)
        << "Response did not contain the peer_id.";

    EXPECT_NE(body.find("hello world"), std::string::npos)
        << "Response did not contain the decoded custom message.";

    EXPECT_NE(body.find("6881"), std::string::npos) << "Response did not contain the port number.";
}
