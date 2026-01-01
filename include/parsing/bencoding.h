#pragma once

#include <map>
#include <string>
#include <string_view>
#include <utility>
#include <variant>
#include <vector>

using ll = long long;

namespace parsing::bencoding {

struct Bitem;

using Bmap = std::map<std::string, Bitem>;
using Blist = std::vector<Bitem>;

// clang-format off
using Bvalue = std::variant<
    ll,
    std::string, 
    Blist,
    Bmap
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
Bitem decode(std::string_view data);
std::string encode(const Bitem &bitem);

}; // namespace parsing::bencoding
