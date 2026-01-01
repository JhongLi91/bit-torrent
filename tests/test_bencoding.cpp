#include "parsing/bencoding.h"
#include "spdlog/spdlog.h"
#include <cassert>
#include <map>
#include <string>
#include <vector>

// Helper to easily access variants in tests
template <typename T> const T &get(const parsing::bencoding::Bitem &item) {
    return std::get<T>(item.val);
}

void test_integers() {
    // Positive
    auto res1 = parsing::bencoding::decode("i42e");
    assert(get<long long>(res1) == 42);

    // Negative
    auto res2 = parsing::bencoding::decode("i-100e");
    assert(get<long long>(res2) == -100);

    // Zero
    auto res3 = parsing::bencoding::decode("i0e");
    assert(get<long long>(res3) == 0);
}

void test_strings() {
    // Standard
    auto res1 = parsing::bencoding::decode("5:hello");
    assert(get<std::string>(res1) == "hello");

    // Empty
    auto res2 = parsing::bencoding::decode("0:");
    assert(get<std::string>(res2) == "");

    // Binary safe (null byte inside)
    std::string binData = "3:a\0b";
}

void test_lists() {
    // l 4:spam i42e e
    auto res = parsing::bencoding::decode("l2:hii42ee");
    const auto &list = get<std::vector<parsing::bencoding::Bitem>>(res);

    assert(list.size() == 2);
    assert(get<std::string>(list[0]) == "hi");
    assert(get<long long>(list[1]) == 42);
}

void test_dictionaries() {
    // d 3:bar 4:spam 3:foo i42e e
    // Keys must be sorted: bar comes before foo
    auto res = parsing::bencoding::decode("d3:bar4:spam3:fooi42ee");
    const auto &map = get<std::map<std::string, parsing::bencoding::Bitem>>(res);

    assert(map.size() == 2);
    assert(get<std::string>(map.at("bar")) == "spam");
    assert(get<long long>(map.at("foo")) == 42);
}

void test_nested_complex() {
    // A mock torrent structure
    // { "announce": "http://tracker", "info": { "length": 1000 } }
    std::string mock = "d8:announce14:http://tracker4:infod6:lengthi1000eee";

    auto res = parsing::bencoding::decode(mock);
    const auto &root = get<std::map<std::string, parsing::bencoding::Bitem>>(res);

    assert(get<std::string>(root.at("announce")) == "http://tracker");

    // Check nested info dict
    const auto &info = get<std::map<std::string, parsing::bencoding::Bitem>>(root.at("info"));
    assert(get<long long>(info.at("length")) == 1000);
}

int main() {
    test_integers();
    test_strings();
    test_lists();
    test_dictionaries();
    test_nested_complex();
    spdlog::info("Success!");
    return 0;
}
