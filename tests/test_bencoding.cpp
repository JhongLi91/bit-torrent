#include "parsing/bencoding.h"
#include <gtest/gtest.h>
#include <map>
#include <string>
#include <variant>
#include <vector>

template <typename T> const T &get(const parsing::bencoding::Bitem &item) {
    return std::get<T>(item.val);
}

TEST(BencodingTest, DecodesIntegers) {
    // Positive
    auto res1 = parsing::bencoding::decode("i42e");
    EXPECT_EQ(get<long long>(res1), 42);

    // Negative
    auto res2 = parsing::bencoding::decode("i-100e");
    EXPECT_EQ(get<long long>(res2), -100);

    // Zero
    auto res3 = parsing::bencoding::decode("i0e");
    EXPECT_EQ(get<long long>(res3), 0);
}

TEST(BencodingTest, DecodesStrings) {
    // Standard
    auto res1 = parsing::bencoding::decode("5:hello");
    EXPECT_EQ(get<std::string>(res1), "hello");

    // Empty
    auto res2 = parsing::bencoding::decode("0:");
    EXPECT_EQ(get<std::string>(res2), "");

    // Binary safe (null byte inside)
    using namespace std::string_literals;
    std::string input = "3:a\0b"s;

    auto res3 = parsing::bencoding::decode(input);

    // We expect "a\0b" (length 3)
    std::string expected = "a\0b"s;
    EXPECT_EQ(get<std::string>(res3), expected);
}

TEST(BencodingTest, DecodesLists) {
    // l 4:spam i42e e
    auto res = parsing::bencoding::decode("l2:hii42ee");

    // Check if it holds the correct type first (optional, but good practice)
    ASSERT_TRUE(std::holds_alternative<std::vector<parsing::bencoding::Bitem>>(res.val));

    const auto &list = get<std::vector<parsing::bencoding::Bitem>>(res);

    // Use ASSERT for size checks to prevent segfaults on index access below
    ASSERT_EQ(list.size(), 2);

    EXPECT_EQ(get<std::string>(list[0]), "hi");
    EXPECT_EQ(get<long long>(list[1]), 42);
}

TEST(BencodingTest, DecodesDictionaries) {
    // d 3:bar 4:spam 3:foo i42e e
    // Keys must be sorted: bar comes before foo
    auto res = parsing::bencoding::decode("d3:bar4:spam3:fooi42ee");
    const auto &map = get<std::map<std::string, parsing::bencoding::Bitem>>(res);

    ASSERT_EQ(map.size(), 2);

    EXPECT_EQ(get<std::string>(map.at("bar")), "spam");
    EXPECT_EQ(get<long long>(map.at("foo")), 42);
}

TEST(BencodingTest, DecodesNestedComplex) {
    // A mock torrent structure
    // { "announce": "http://tracker", "info": { "length": 1000 } }
    std::string mock = "d8:announce14:http://tracker4:infod6:lengthi1000eee";

    auto res = parsing::bencoding::decode(mock);
    const auto &root = get<std::map<std::string, parsing::bencoding::Bitem>>(res);

    EXPECT_EQ(get<std::string>(root.at("announce")), "http://tracker");

    // Check nested info dict
    ASSERT_TRUE(root.contains("info")); // C++20 check
    const auto &info = get<std::map<std::string, parsing::bencoding::Bitem>>(root.at("info"));

    ASSERT_TRUE(info.contains("length"));
    EXPECT_EQ(get<long long>(info.at("length")), 1000);
}
