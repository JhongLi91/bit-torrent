#include "parsing/buffer.h"
#include "spdlog/spdlog.h"
#include <cstdint>

namespace parser::buffer {

void check_bounds(const buffer_t &buf, uint32_t idx, uint32_t needed) {
    if (idx + needed > buf.size()) {
        spdlog::error("Index Out of Bound");
        exit(1);
    }
}

uint8_t get1B(const buffer_t &buf, uint32_t idx) {
    check_bounds(buf, idx, 1);
    return buf[idx];
}
uint16_t get2B(const buffer_t &buf, uint32_t idx) {
    check_bounds(buf, idx, 2);
    return (buf[idx] << 8) + buf[idx + 1];
}
uint32_t get4B(const buffer_t &buf, uint32_t idx) {
    check_bounds(buf, idx, 4);
    uint32_t res = 0;
    for (int i = 0; i < 4; i++) {
        res <<= 8;
        res += buf[idx + i];
    }
    return res;
}

buffer_t getNB(buffer_t &buf, uint32_t n, uint32_t idx) {
    check_bounds(buf, idx, n);
    return buffer_t(begin(buf) + idx, begin(buf) + idx + n);
}

void set1B(uint8_t n, buffer_t &buf, uint32_t idx) {
    check_bounds(buf, idx, 1);
    buf[idx] = n;
}
void set2B(uint16_t n, buffer_t &buf, uint32_t idx) {
    check_bounds(buf, idx, 2);
    buf[idx] = (n >> 8) & 0xFF;
    buf[idx + 1] = n & 0xFF;
}
void set4B(uint32_t n, buffer_t &buf, uint32_t idx) {
    check_bounds(buf, idx, 4);
    buf[idx] = (n >> 24) & 0xFF;
    buf[idx + 1] = (n >> 16) & 0xFF;
    buf[idx + 2] = (n >> 8) & 0xFF;
    buf[idx + 3] = n & 0xFF;
}

void setNB(buffer_t &dst, const void *src, uint32_t n, uint32_t idx) {
    check_bounds(dst, idx, n);
    std::memcpy(&dst[idx], src, n);
}

} // namespace parser::buffer
