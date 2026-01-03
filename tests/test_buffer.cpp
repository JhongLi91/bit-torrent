#include "parsing/buffer.h"
#include <cstdint>
#include <gtest/gtest.h>

using namespace parser::buffer;

TEST(BufferTest, Handles1Byte) {
    buffer_t buf(10, 0);
    uint8_t val = 0xFF;

    set1B(val, buf, 0);

    EXPECT_EQ(buf[0], 0xFF);
    EXPECT_EQ(get1B(buf, 0), val);

    set1B(0xAA, buf, 5);
    EXPECT_EQ(get1B(buf, 5), 0xAA);
}

TEST(BufferTest, Handles2BytesBigEndian) {
    buffer_t buf(10, 0);
    uint16_t val = 0x1234;

    set2B(val, buf, 0);

    // Verify Big Endian (Network Byte Order)
    // High byte (0x12) should be at low address (0)
    EXPECT_EQ(buf[0], 0x12);
    EXPECT_EQ(buf[1], 0x34);

    EXPECT_EQ(get2B(buf, 0), 0x1234);
}

TEST(BufferTest, Handles4BytesBigEndian) {
    buffer_t buf(10, 0);
    uint32_t val = 0xDEADBEEF;

    set4B(val, buf, 0);

    // Verify Big Endian breakdown
    EXPECT_EQ(buf[0], 0xDE);
    EXPECT_EQ(buf[1], 0xAD);
    EXPECT_EQ(buf[2], 0xBE);
    EXPECT_EQ(buf[3], 0xEF);

    EXPECT_EQ(get4B(buf, 0), 0xDEADBEEF);
}

TEST(BufferTest, HandlesBoundaries) {
    // Test filling a buffer exactly to its size
    buffer_t buf(4, 0);

    set4B(0xAABBCCDD, buf, 0);
    EXPECT_EQ(get4B(buf, 0), 0xAABBCCDD);

    // Test overwriting the last byte explicitly
    set1B(0x77, buf, 3);
    EXPECT_EQ(get1B(buf, 3), 0x77);

    // Verify the overwrite didn't corrupt the previous bytes
    // 0xAABBCC77
    EXPECT_EQ(buf[0], 0xAA);
    EXPECT_EQ(buf[1], 0xBB);
    EXPECT_EQ(buf[2], 0xCC);
}

TEST(BufferTest, HandlesSetNB) {
    buffer_t buf(10, 0);

    uint8_t src[] = {0x01, 0x02, 0x03};
    setNB(buf, src, 3, 2);

    EXPECT_EQ(buf[2], 0x01);
    EXPECT_EQ(buf[3], 0x02);
    EXPECT_EQ(buf[4], 0x03);
    EXPECT_EQ(buf[5], 0x00);

    const char *msg = "Hi";
    setNB(buf, (void *)msg, 2, 8);

    EXPECT_EQ(buf[8], 'H');
    EXPECT_EQ(buf[9], 'i');
}

TEST(BufferTest, HandlesGetNB) {
    buffer_t buf(10, 0);

    buf[0] = 0xAA;
    buf[1] = 0xBB;
    buf[2] = 0xCC;
    buf[3] = 0xDD;

    buffer_t slice = getNB(buf, 2, 1);

    ASSERT_EQ(slice.size(), 2);
    EXPECT_EQ(slice[0], 0xBB);
    EXPECT_EQ(slice[1], 0xCC);
}

TEST(BufferTest, HandlesGetSetNBPairing) {
    buffer_t buf(20, 0xFF);

    std::string secret = "BitTorrent";

    setNB(buf, (void *)secret.data(), secret.size(), 5);

    buffer_t read_back = getNB(buf, secret.size(), 5);

    std::string result(read_back.begin(), read_back.end());

    EXPECT_EQ(result, secret);
}
