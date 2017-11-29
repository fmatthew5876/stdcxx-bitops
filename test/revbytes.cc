#include "driver.hh"

#include <bitops.hh>

using namespace std;

TEST(RevBytesTest, Rev8) {
  ASSERT_EQ(uint8_t(0xAA), reverse_bytes(uint8_t(0xAA)));
  ASSERT_EQ(uint8_t(0xAA), reverse_bytes(uint8_t(0xAA), 1, 1));
}

TEST(RevBytesTest, Rev16) {
  ASSERT_EQ(uint16_t(0xAABB), reverse_bytes(uint16_t(0xBBAA)));
  ASSERT_EQ(uint16_t(0xBBAA), reverse_bytes(uint16_t(0xBBAA), 1, 1));
  ASSERT_EQ(uint16_t(0xAABB), reverse_bytes(uint16_t(0xBBAA), 1, 2));
  ASSERT_EQ(uint16_t(0xBBAA), reverse_bytes(uint16_t(0xBBAA), 2));
  ASSERT_EQ(uint16_t(0xBBAA), reverse_bytes(uint16_t(0xBBAA), 2, 1));
}

TEST(RevBytesTest, Rev32) {
  ASSERT_EQ(uint32_t(0xAABBCCDDUL), reverse_bytes(uint32_t(0xDDCCBBAAUL)));
  ASSERT_EQ(uint32_t(0xDDCCBBAAUL), reverse_bytes(uint32_t(0xDDCCBBAAUL), 1, 1));
  ASSERT_EQ(uint32_t(0xCCDDAABBUL), reverse_bytes(uint32_t(0xDDCCBBAAUL), 1, 2));
  ASSERT_EQ(uint32_t(0xAABBCCDDUL), reverse_bytes(uint32_t(0xDDCCBBAAUL), 1, 4));
  ASSERT_EQ(uint32_t(0xBBAADDCCUL), reverse_bytes(uint32_t(0xDDCCBBAAUL), 2));
  ASSERT_EQ(uint32_t(0xDDCCBBAAUL), reverse_bytes(uint32_t(0xDDCCBBAAUL), 2, 1));
  ASSERT_EQ(uint32_t(0xBBAADDCCUL), reverse_bytes(uint32_t(0xDDCCBBAAUL), 2, 2));
  ASSERT_EQ(uint32_t(0xDDCCBBAAUL), reverse_bytes(uint32_t(0xDDCCBBAAUL), 4));
  ASSERT_EQ(uint32_t(0xDDCCBBAAUL), reverse_bytes(uint32_t(0xDDCCBBAAUL), 4, 1));
}

TEST(RevBytesTest, Rev64) {
  ASSERT_EQ(uint64_t(0xAABBCCDD11223344UL), reverse_bytes(uint64_t(0x44332211DDCCBBAAUL)));
  ASSERT_EQ(uint64_t(0x44332211DDCCBBAAUL), reverse_bytes(uint64_t(0x44332211DDCCBBAAUL), 1, 1));
  ASSERT_EQ(uint64_t(0x33441122CCDDAABBUL), reverse_bytes(uint64_t(0x44332211DDCCBBAAUL), 1, 2));
  ASSERT_EQ(uint64_t(0x11223344AABBCCDDUL), reverse_bytes(uint64_t(0x44332211DDCCBBAAUL), 1, 4));
  ASSERT_EQ(uint64_t(0xAABBCCDD11223344UL), reverse_bytes(uint64_t(0x44332211DDCCBBAAUL), 1, 8));
  ASSERT_EQ(uint64_t(0xBBAADDCC22114433UL), reverse_bytes(uint64_t(0x44332211DDCCBBAAUL), 2));
  ASSERT_EQ(uint64_t(0x44332211DDCCBBAAUL), reverse_bytes(uint64_t(0x44332211DDCCBBAAUL), 2, 1));
  ASSERT_EQ(uint64_t(0x22114433BBAADDCCUL), reverse_bytes(uint64_t(0x44332211DDCCBBAAUL), 2, 2));
  ASSERT_EQ(uint64_t(0xBBAADDCC22114433UL), reverse_bytes(uint64_t(0x44332211DDCCBBAAUL), 2, 4));
  ASSERT_EQ(uint64_t(0xDDCCBBAA44332211UL), reverse_bytes(uint64_t(0x44332211DDCCBBAAUL), 4));
  ASSERT_EQ(uint64_t(0x44332211DDCCBBAAUL), reverse_bytes(uint64_t(0x44332211DDCCBBAAUL), 4, 1));
  ASSERT_EQ(uint64_t(0xDDCCBBAA44332211UL), reverse_bytes(uint64_t(0x44332211DDCCBBAAUL), 4, 2));
}


TEST(RevBytesTest, RevI8) {
  ASSERT_EQ(int8_t(0xAA), reverse_bytes(int8_t(0xAA)));
  ASSERT_EQ(int8_t(0xAA), reverse_bytes(int8_t(0xAA), 1, 1));
}

TEST(RevBytesTest, RevI16) {
  ASSERT_EQ(int16_t(0xAABB), reverse_bytes(int16_t(0xBBAA)));
  ASSERT_EQ(int16_t(0xBBAA), reverse_bytes(int16_t(0xBBAA), 1, 1));
  ASSERT_EQ(int16_t(0xAABB), reverse_bytes(int16_t(0xBBAA), 1, 2));
  ASSERT_EQ(int16_t(0xBBAA), reverse_bytes(int16_t(0xBBAA), 2));
  ASSERT_EQ(int16_t(0xBBAA), reverse_bytes(int16_t(0xBBAA), 2, 1));
}

TEST(RevBytesTest, RevI32) {
  ASSERT_EQ(int32_t(0xAABBCCDDUL), reverse_bytes(int32_t(0xDDCCBBAAUL)));
  ASSERT_EQ(int32_t(0xDDCCBBAAUL), reverse_bytes(int32_t(0xDDCCBBAAUL), 1, 1));
  ASSERT_EQ(int32_t(0xCCDDAABBUL), reverse_bytes(int32_t(0xDDCCBBAAUL), 1, 2));
  ASSERT_EQ(int32_t(0xAABBCCDDUL), reverse_bytes(int32_t(0xDDCCBBAAUL), 1, 4));
  ASSERT_EQ(int32_t(0xBBAADDCCUL), reverse_bytes(int32_t(0xDDCCBBAAUL), 2));
  ASSERT_EQ(int32_t(0xDDCCBBAAUL), reverse_bytes(int32_t(0xDDCCBBAAUL), 2, 1));
  ASSERT_EQ(int32_t(0xBBAADDCCUL), reverse_bytes(int32_t(0xDDCCBBAAUL), 2, 2));
  ASSERT_EQ(int32_t(0xDDCCBBAAUL), reverse_bytes(int32_t(0xDDCCBBAAUL), 4));
  ASSERT_EQ(int32_t(0xDDCCBBAAUL), reverse_bytes(int32_t(0xDDCCBBAAUL), 4, 1));
}

TEST(RevBytesTest, RevI64) {
  ASSERT_EQ(int64_t(0xAABBCCDD11223344UL), reverse_bytes(int64_t(0x44332211DDCCBBAAUL)));
  ASSERT_EQ(int64_t(0x44332211DDCCBBAAUL), reverse_bytes(int64_t(0x44332211DDCCBBAAUL), 1, 1));
  ASSERT_EQ(int64_t(0x33441122CCDDAABBUL), reverse_bytes(int64_t(0x44332211DDCCBBAAUL), 1, 2));
  ASSERT_EQ(int64_t(0x11223344AABBCCDDUL), reverse_bytes(int64_t(0x44332211DDCCBBAAUL), 1, 4));
  ASSERT_EQ(int64_t(0xAABBCCDD11223344UL), reverse_bytes(int64_t(0x44332211DDCCBBAAUL), 1, 8));
  ASSERT_EQ(int64_t(0xBBAADDCC22114433UL), reverse_bytes(int64_t(0x44332211DDCCBBAAUL), 2));
  ASSERT_EQ(int64_t(0x44332211DDCCBBAAUL), reverse_bytes(int64_t(0x44332211DDCCBBAAUL), 2, 1));
  ASSERT_EQ(int64_t(0x22114433BBAADDCCUL), reverse_bytes(int64_t(0x44332211DDCCBBAAUL), 2, 2));
  ASSERT_EQ(int64_t(0xBBAADDCC22114433UL), reverse_bytes(int64_t(0x44332211DDCCBBAAUL), 2, 4));
  ASSERT_EQ(int64_t(0xDDCCBBAA44332211UL), reverse_bytes(int64_t(0x44332211DDCCBBAAUL), 4));
  ASSERT_EQ(int64_t(0x44332211DDCCBBAAUL), reverse_bytes(int64_t(0x44332211DDCCBBAAUL), 4, 1));
  ASSERT_EQ(int64_t(0xDDCCBBAA44332211UL), reverse_bytes(int64_t(0x44332211DDCCBBAAUL), 4, 2));
}

