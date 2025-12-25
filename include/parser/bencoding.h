#pragma once

#include <string>
#include <string_view>
#include <map>
#include <utility>
#include <variant>
#include <vector>

using ll = long long;

namespace parser {

struct Bitem;

// clang-format off
using Bvalue = std::variant<
    ll,
    std::string, 
    std::vector<Bitem>,
    std::map<std::string, Bitem>
>; // clang-format on

struct Bitem {
    Bitem() = default;
    Bitem(const Bitem &) = default;
    Bitem(Bitem &&) = default;
    Bitem &operator=(const Bitem &) = default;
    Bitem &operator=(Bitem &&) = default;

    template <typename T>
        requires(!std::same_as<std::remove_cvref_t<T>, Bitem>)
    Bitem(T &&v) : val(std::forward<T>(v)) {}

    Bvalue val;
};

// public interfaces
Bitem decode(std::string_view data, size_t &pos);

}; // namespace parser
