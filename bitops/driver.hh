#ifndef DRIVER_HH
#define DRIVER_HH
#include "gtest/gtest.h"

typedef ::testing::Types<int8_t, uint8_t, int16_t, uint16_t, int32_t, uint32_t, int64_t, uint64_t> IntTypes;
typedef ::testing::Types<
  std::pair<int8_t, int8_t>,
  std::pair<int8_t, uint8_t>,
  std::pair<int8_t, int16_t>,
  std::pair<int8_t, uint16_t>,
  std::pair<int8_t, int32_t>,
  std::pair<int8_t, uint32_t>,
  std::pair<int8_t, int32_t>,
  std::pair<int8_t, uint32_t>,

  std::pair<uint8_t, int8_t>,
  std::pair<uint8_t, uint8_t>,
  std::pair<uint8_t, int16_t>,
  std::pair<uint8_t, uint16_t>,
  std::pair<uint8_t, int32_t>,
  std::pair<uint8_t, uint32_t>,
  std::pair<uint8_t, int32_t>,
  std::pair<uint8_t, uint32_t>,

  std::pair<int16_t, int8_t>,
  std::pair<int16_t, uint8_t>,
  std::pair<int16_t, int16_t>,
  std::pair<int16_t, uint16_t>,
  std::pair<int16_t, int32_t>,
  std::pair<int16_t, uint32_t>,
  std::pair<int16_t, int32_t>,
  std::pair<int16_t, uint32_t>,

  std::pair<uint16_t, int8_t>,
  std::pair<uint16_t, uint8_t>,
  std::pair<uint16_t, int16_t>,
  std::pair<uint16_t, uint16_t>,
  std::pair<uint16_t, int32_t>,
  std::pair<uint16_t, uint32_t>,
  std::pair<uint16_t, int32_t>,
  std::pair<uint16_t, uint32_t>,

  std::pair<int32_t, int8_t>,
  std::pair<int32_t, uint8_t>,
  std::pair<int32_t, int16_t>,
  std::pair<int32_t, uint16_t>,
  std::pair<int32_t, int32_t>,
  std::pair<int32_t, uint32_t>,
  std::pair<int32_t, int32_t>,
  std::pair<int32_t, uint32_t>,

  std::pair<uint32_t, int8_t>,
  std::pair<uint32_t, uint8_t>,
  std::pair<uint32_t, int16_t>,
  std::pair<uint32_t, uint16_t>,
  std::pair<uint32_t, int32_t>,
  std::pair<uint32_t, uint32_t>,
  std::pair<uint32_t, int32_t>,
  std::pair<uint32_t, uint32_t>,

  std::pair<int64_t, int8_t>,
  std::pair<int64_t, uint8_t>,
  std::pair<int64_t, int16_t>,
  std::pair<int64_t, uint16_t>,
  std::pair<int64_t, int32_t>,
  std::pair<int64_t, uint32_t>,
  std::pair<int64_t, int32_t>,
  std::pair<int64_t, uint32_t>,

  std::pair<uint64_t, int8_t>,
  std::pair<uint64_t, uint8_t>,
  std::pair<uint64_t, int16_t>,
  std::pair<uint64_t, uint16_t>,
  std::pair<uint64_t, int32_t>,
  std::pair<uint64_t, uint32_t>,
  std::pair<uint64_t, int32_t>,
  std::pair<uint64_t, uint32_t>
  > IntPairTypes;

#endif
