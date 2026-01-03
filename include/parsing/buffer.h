#pragma once

#include <vector>

typedef std::vector<uint8_t> buffer_t;

namespace parser::buffer {

uint8_t get1B(const buffer_t &buf, uint32_t idx);
uint16_t get2B(const buffer_t &buf, uint32_t idx);
uint32_t get4B(const buffer_t &buf, uint32_t idx);
buffer_t getNB(buffer_t &buf, uint32_t n, uint32_t idx);

void set1B(uint8_t n, buffer_t &buf, uint32_t idx);
void set2B(uint16_t n, buffer_t &buf, uint32_t idx);
void set4B(uint32_t n, buffer_t &buf, uint32_t idx);
void setNB(buffer_t &dst, const void *src, uint32_t n, uint32_t idx);

} // namespace parser::buffer
