#include "driver.hh"

#include <bitops.hh>

using namespace std;

TEST(RevBytesTest, Rev8) {
  ASSERT_EQ(uint8_t(0xAA), revbytes(uint8_t(0xAA)));
  ASSERT_EQ(uint8_t(0xAA), revbytes(uint8_t(0xAA), 1, 1));
}

TEST(RevBytesTest, Rev16) {
  ASSERT_EQ(uint16_t(0xAABB), revbytes(uint16_t(0xBBAA)));
  ASSERT_EQ(uint16_t(0xBBAA), revbytes(uint16_t(0xBBAA), 1, 1));
  ASSERT_EQ(uint16_t(0xAABB), revbytes(uint16_t(0xBBAA), 1, 2));
  ASSERT_EQ(uint16_t(0xBBAA), revbytes(uint16_t(0xBBAA), 2));
  ASSERT_EQ(uint16_t(0xBBAA), revbytes(uint16_t(0xBBAA), 2, 1));
}

TEST(RevBytesTest, Rev32) {
  ASSERT_EQ(uint32_t(0xAABBCCDDUL), revbytes(uint32_t(0xDDCCBBAAUL)));
  ASSERT_EQ(uint32_t(0xDDCCBBAAUL), revbytes(uint32_t(0xDDCCBBAAUL), 1, 1));
  ASSERT_EQ(uint32_t(0xCCDDAABBUL), revbytes(uint32_t(0xDDCCBBAAUL), 1, 2));
  ASSERT_EQ(uint32_t(0xAABBCCDDUL), revbytes(uint32_t(0xDDCCBBAAUL), 1, 4));
  ASSERT_EQ(uint32_t(0xBBAADDCCUL), revbytes(uint32_t(0xDDCCBBAAUL), 2));
  ASSERT_EQ(uint32_t(0xDDCCBBAAUL), revbytes(uint32_t(0xDDCCBBAAUL), 2, 1));
  ASSERT_EQ(uint32_t(0xBBAADDCCUL), revbytes(uint32_t(0xDDCCBBAAUL), 2, 2));
  ASSERT_EQ(uint32_t(0xDDCCBBAAUL), revbytes(uint32_t(0xDDCCBBAAUL), 4));
  ASSERT_EQ(uint32_t(0xDDCCBBAAUL), revbytes(uint32_t(0xDDCCBBAAUL), 4, 1));
}

TEST(RevBytesTest, Rev64) {
  ASSERT_EQ(uint64_t(0xAABBCCDD11223344UL), revbytes(uint64_t(0x44332211DDCCBBAAUL)));
  ASSERT_EQ(uint64_t(0x44332211DDCCBBAAUL), revbytes(uint64_t(0x44332211DDCCBBAAUL), 1, 1));
  ASSERT_EQ(uint64_t(0x33441122CCDDAABBUL), revbytes(uint64_t(0x44332211DDCCBBAAUL), 1, 2));
  ASSERT_EQ(uint64_t(0x11223344AABBCCDDUL), revbytes(uint64_t(0x44332211DDCCBBAAUL), 1, 4));
  ASSERT_EQ(uint64_t(0xAABBCCDD11223344UL), revbytes(uint64_t(0x44332211DDCCBBAAUL), 1, 8));
  ASSERT_EQ(uint64_t(0xBBAADDCC22114433UL), revbytes(uint64_t(0x44332211DDCCBBAAUL), 2));
  ASSERT_EQ(uint64_t(0x44332211DDCCBBAAUL), revbytes(uint64_t(0x44332211DDCCBBAAUL), 2, 1));
  ASSERT_EQ(uint64_t(0x22114433BBAADDCCUL), revbytes(uint64_t(0x44332211DDCCBBAAUL), 2, 2));
  ASSERT_EQ(uint64_t(0xBBAADDCC22114433UL), revbytes(uint64_t(0x44332211DDCCBBAAUL), 2, 4));
  ASSERT_EQ(uint64_t(0xDDCCBBAA44332211UL), revbytes(uint64_t(0x44332211DDCCBBAAUL), 4));
  ASSERT_EQ(uint64_t(0x44332211DDCCBBAAUL), revbytes(uint64_t(0x44332211DDCCBBAAUL), 4, 1));
  ASSERT_EQ(uint64_t(0xDDCCBBAA44332211UL), revbytes(uint64_t(0x44332211DDCCBBAAUL), 4, 2));
}


TEST(RevBytesTest, RevI8) {
  ASSERT_EQ(int8_t(0xAA), revbytes(int8_t(0xAA)));
  ASSERT_EQ(int8_t(0xAA), revbytes(int8_t(0xAA), 1, 1));
}

TEST(RevBytesTest, RevI16) {
  ASSERT_EQ(int16_t(0xAABB), revbytes(int16_t(0xBBAA)));
  ASSERT_EQ(int16_t(0xBBAA), revbytes(int16_t(0xBBAA), 1, 1));
  ASSERT_EQ(int16_t(0xAABB), revbytes(int16_t(0xBBAA), 1, 2));
  ASSERT_EQ(int16_t(0xBBAA), revbytes(int16_t(0xBBAA), 2));
  ASSERT_EQ(int16_t(0xBBAA), revbytes(int16_t(0xBBAA), 2, 1));
}

TEST(RevBytesTest, RevI32) {
  ASSERT_EQ(int32_t(0xAABBCCDDUL), revbytes(int32_t(0xDDCCBBAAUL)));
  ASSERT_EQ(int32_t(0xDDCCBBAAUL), revbytes(int32_t(0xDDCCBBAAUL), 1, 1));
  ASSERT_EQ(int32_t(0xCCDDAABBUL), revbytes(int32_t(0xDDCCBBAAUL), 1, 2));
  ASSERT_EQ(int32_t(0xAABBCCDDUL), revbytes(int32_t(0xDDCCBBAAUL), 1, 4));
  ASSERT_EQ(int32_t(0xBBAADDCCUL), revbytes(int32_t(0xDDCCBBAAUL), 2));
  ASSERT_EQ(int32_t(0xDDCCBBAAUL), revbytes(int32_t(0xDDCCBBAAUL), 2, 1));
  ASSERT_EQ(int32_t(0xBBAADDCCUL), revbytes(int32_t(0xDDCCBBAAUL), 2, 2));
  ASSERT_EQ(int32_t(0xDDCCBBAAUL), revbytes(int32_t(0xDDCCBBAAUL), 4));
  ASSERT_EQ(int32_t(0xDDCCBBAAUL), revbytes(int32_t(0xDDCCBBAAUL), 4, 1));
}

TEST(RevBytesTest, RevI64) {
  ASSERT_EQ(int64_t(0xAABBCCDD11223344UL), revbytes(int64_t(0x44332211DDCCBBAAUL)));
  ASSERT_EQ(int64_t(0x44332211DDCCBBAAUL), revbytes(int64_t(0x44332211DDCCBBAAUL), 1, 1));
  ASSERT_EQ(int64_t(0x33441122CCDDAABBUL), revbytes(int64_t(0x44332211DDCCBBAAUL), 1, 2));
  ASSERT_EQ(int64_t(0x11223344AABBCCDDUL), revbytes(int64_t(0x44332211DDCCBBAAUL), 1, 4));
  ASSERT_EQ(int64_t(0xAABBCCDD11223344UL), revbytes(int64_t(0x44332211DDCCBBAAUL), 1, 8));
  ASSERT_EQ(int64_t(0xBBAADDCC22114433UL), revbytes(int64_t(0x44332211DDCCBBAAUL), 2));
  ASSERT_EQ(int64_t(0x44332211DDCCBBAAUL), revbytes(int64_t(0x44332211DDCCBBAAUL), 2, 1));
  ASSERT_EQ(int64_t(0x22114433BBAADDCCUL), revbytes(int64_t(0x44332211DDCCBBAAUL), 2, 2));
  ASSERT_EQ(int64_t(0xBBAADDCC22114433UL), revbytes(int64_t(0x44332211DDCCBBAAUL), 2, 4));
  ASSERT_EQ(int64_t(0xDDCCBBAA44332211UL), revbytes(int64_t(0x44332211DDCCBBAAUL), 4));
  ASSERT_EQ(int64_t(0x44332211DDCCBBAAUL), revbytes(int64_t(0x44332211DDCCBBAAUL), 4, 1));
  ASSERT_EQ(int64_t(0xDDCCBBAA44332211UL), revbytes(int64_t(0x44332211DDCCBBAAUL), 4, 2));
}

