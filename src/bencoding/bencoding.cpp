#include "bencoding/bencoding.h"
#include <cctype>
#include <stdexcept>

namespace bencoding {

// private helper function
namespace {
ll get_number(std::string_view data, size_t start, size_t end) {
    ll n = 0;
    for (; start < end; start++) {
        n *= 10;
        n += data[start] - '0';
    }
    return n;
}
} // namespace

Bitem decode(std::string_view data, size_t &pos);
long long decode_int(std::string_view data, size_t &pos);
std::string decode_string(std::string_view data, size_t &pos);
std::vector<Bitem> decode_list(std::string_view data, size_t &pos);
std::map<std::string, Bitem> decode_dict(std::string_view data, size_t &pos);

Bitem decode(std::string_view data, size_t &pos) {
    if (pos >= data.size())
        throw std::runtime_error("Unexpected end of data");

    char curr = data[pos];
    if (curr == 'i')
        return decode_int(data, pos);
    else if (curr == 'l')
        return decode_list(data, pos);
    else if (curr == 'd')
        return decode_dict(data, pos);
    else if (isdigit(curr))
        return decode_string(data, pos);

    throw std::runtime_error("Unknown type identifier");
}

// Format: i<number>e  (e.g., i42e, i-3e)
ll decode_int(std::string_view data, size_t &pos) {
    pos++; // consume 'i'
    size_t end = data.find('e', pos);
    if (end == std::string_view::npos) {
        throw std::runtime_error("Error in Bencoding Int Format");
    }

    bool isNeg = data[pos] == '-';
    pos += isNeg;

    ll res = get_number(data, pos, end);
    pos = end + 1;

    return isNeg ? -1 * res : res;
}

// Format: <length>:<contents> (e.g., 4:spam)
std::string decode_string(std::string_view data, size_t &pos) {
    size_t end = data.find(':', pos);
    if (end == std::string_view::npos) {
        throw std::runtime_error("Error in Bencoding String Format");
    }

    ll len = get_number(data, pos, end);
    pos = end + 1;

    if (pos + len > data.size()) {
        throw std::runtime_error("String length out of bounds");
    }

    std::string res(data.substr(pos, len));
    pos += len;

    return res;
}

// Format: l<item1><item2>...e (e.g., l4:spami42ee)
std::vector<Bitem> decode_list(std::string_view data, size_t &pos) {
    pos++; // Consume 'l'

    // TODO: error checking
    std::vector<Bitem> res;
    while (data[pos] != 'e') {
        res.push_back(decode(data, pos));
    }
    pos++; // Consume 'e'
    return res;
}

// Format: d<key1><value1>...e
std::map<std::string, Bitem> decode_dict(std::string_view data, size_t &pos) {
    pos++; // Consume 'd'

    // TODO: error checking
    std::map<std::string, Bitem> res;
    while (data[pos] != 'e') {
        std::string key = decode_string(data, pos);
        Bitem val = decode(data, pos);
        res[key] = val;
    }

    pos++; // Consume 'e'
    return res;
}

} // namespace bencoding
