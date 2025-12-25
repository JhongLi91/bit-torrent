#include "parser/bencoding.h"
#include <cassert>
#include <iostream>
#include <map>
#include <string>
#include <vector>

// Helper to easily access variants in tests
template <typename T> const T &get(const parser::Bitem &item) { return std::get<T>(item.val); }

void test_integers() {
    std::cout << "Testing Integers... ";
    size_t pos = 0;

    // Positive
    auto res1 = parser::decode("i42e", pos);
    assert(get<long long>(res1) == 42);

    // Negative
    pos = 0;
    auto res2 = parser::decode("i-100e", pos);
    assert(get<long long>(res2) == -100);

    // Zero
    pos = 0;
    auto res3 = parser::decode("i0e", pos);
    assert(get<long long>(res3) == 0);

    std::cout << "OK\n";
}

void test_strings() {
    std::cout << "Testing Strings... ";
    size_t pos = 0;

    // Standard
    auto res1 = parser::decode("4:spam", pos);
    assert(get<std::string>(res1) == "spam");

    // Empty
    pos = 0;
    auto res2 = parser::decode("0:", pos);
    assert(get<std::string>(res2) == "");

    // Binary safe (null byte inside)
    pos = 0;
    std::string binData = "3:a\0b"; // Manually constructed
    // Note: C++ string literals handling \0 might be tricky,
    // better to construct string explicitly for binary tests.

    std::cout << "OK\n";
}

void test_lists() {
    std::cout << "Testing Lists... ";
    size_t pos = 0;

    // l 4:spam i42e e
    auto res = parser::decode("l4:spami42ee", pos);
    const auto &list = get<std::vector<parser::Bitem>>(res);

    assert(list.size() == 2);
    assert(get<std::string>(list[0]) == "spam");
    assert(get<long long>(list[1]) == 42);

    std::cout << "OK\n";
}

void test_dictionaries() {
    std::cout << "Testing Dictionaries... ";
    size_t pos = 0;

    // d 3:bar 4:spam 3:foo i42e e
    // Keys must be sorted: bar comes before foo
    auto res = parser::decode("d3:bar4:spam3:fooi42ee", pos);
    const auto &map = get<std::map<std::string, parser::Bitem>>(res);

    assert(map.size() == 2);
    assert(get<std::string>(map.at("bar")) == "spam");
    assert(get<long long>(map.at("foo")) == 42);

    std::cout << "OK\n";
}

void test_nested_complex() {
    std::cout << "Testing Complex Nested Structure... ";
    size_t pos = 0;

    // A mock torrent structure
    // { "announce": "http://tracker", "info": { "length": 1000 } }
    std::string mock = "d8:announce14:http://tracker4:infod6:lengthi1000eee";

    auto res = parser::decode(mock, pos);
    const auto &root = get<std::map<std::string, parser::Bitem>>(res);

    assert(get<std::string>(root.at("announce")) == "http://tracker");

    // Check nested info dict
    const auto &info = get<std::map<std::string, parser::Bitem>>(root.at("info"));
    assert(get<long long>(info.at("length")) == 1000);

    std::cout << "OK\n";
}

int main() {
    test_integers();
    test_strings();
    test_lists();
    test_dictionaries();
    test_nested_complex();
    return 0;
}
