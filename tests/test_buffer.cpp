#include "parsing/buffer.h"
#include <cassert>

using namespace parser::buffer;

void test_1byte() {
    buffer_t buf(10, 0);
    uint8_t val = 0xFF;
    set1B(val, buf, 0);

    assert(buf[0] == 0xFF);
    assert(get1B(buf, 0) == val);

    set1B(0xAA, buf, 5);
    assert(get1B(buf, 5) == 0xAA);
}

void test_2byte() {
    buffer_t buf(10, 0);
    uint16_t val = 0x1234;
    set2B(val, buf, 0);

    assert(buf[0] == 0x12);
    assert(buf[1] == 0x34);

    assert(get2B(buf, 0) == 0x1234);
}

void test_4byte() {
    buffer_t buf(10, 0);
    uint32_t val = 0xDEADBEEF;
    set4B(val, buf, 0);

    assert(buf[0] == 0xDE);
    assert(buf[1] == 0xAD);
    assert(buf[2] == 0xBE);
    assert(buf[3] == 0xEF);

    assert(get4B(buf, 0) == 0xDEADBEEF);
}

void test_boundaries() {
    buffer_t buf(4, 0);

    set4B(0xAABBCCDD, buf, 0);
    assert(get4B(buf, 0) == 0xAABBCCDD);

    set1B(0x77, buf, 3);
    assert(get1B(buf, 3) == 0x77);
}

int main() {
    test_1byte();
    test_2byte();
    test_4byte();
    test_boundaries();
    return 0;
}
