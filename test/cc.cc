#include <bitops.hh>

#include <cstdint>

#include "driver.hh"

//All template instantiations compile test

#define INST(X, ...) \
  template int8_t std::X<int8_t>(int8_t, ##__VA_ARGS__) noexcept;\
  template uint8_t std::X<uint8_t>(uint8_t, ##__VA_ARGS__) noexcept;\
  template int16_t std::X<int16_t>(int16_t, ##__VA_ARGS__) noexcept;\
  template uint16_t std::X<uint16_t>(uint16_t, ##__VA_ARGS__) noexcept;\
  template int32_t std::X<int32_t>(int32_t, ##__VA_ARGS__) noexcept;\
  template uint32_t std::X<uint32_t>(uint32_t, ##__VA_ARGS__) noexcept;\
  template int64_t std::X<int64_t>(int64_t, ##__VA_ARGS__) noexcept;\
  template uint64_t std::X<uint64_t>(uint64_t, ##__VA_ARGS__) noexcept

#define INSTR(R, X, ...) \
  template R std::X<int8_t>(int8_t, ##__VA_ARGS__) noexcept;\
  template R std::X<uint8_t>(uint8_t, ##__VA_ARGS__) noexcept;\
  template R std::X<int16_t>(int16_t, ##__VA_ARGS__) noexcept;\
  template R std::X<uint16_t>(uint16_t, ##__VA_ARGS__) noexcept;\
  template R std::X<int32_t>(int32_t, ##__VA_ARGS__) noexcept;\
  template R std::X<uint32_t>(uint32_t, ##__VA_ARGS__) noexcept;\
  template R std::X<int64_t>(int64_t, ##__VA_ARGS__) noexcept;\
  template R std::X<uint64_t>(uint64_t, ##__VA_ARGS__) noexcept

#define INSTSAT2(R, X, L, ...) \
  template auto std::X<L,int8_t>(L l, int8_t r, ##__VA_ARGS__) noexcept -> R;\
  template auto std::X<L,uint8_t>(L l, uint8_t r, ##__VA_ARGS__) noexcept -> R;\
  template auto std::X<L,int16_t>(L l, int16_t r, ##__VA_ARGS__) noexcept -> R;\
  template auto std::X<L,uint16_t>(L l, uint16_t r, ##__VA_ARGS__) noexcept -> R;\
  template auto std::X<L,int32_t>(L l, int32_t r, ##__VA_ARGS__) noexcept -> R;\
  template auto std::X<L,uint32_t>(L l, uint32_t r, ##__VA_ARGS__) noexcept -> R;\
  template auto std::X<L,int64_t>(L l, int64_t r, ##__VA_ARGS__) noexcept -> R;\
  template auto std::X<L,uint64_t>(L l, uint64_t r, ##__VA_ARGS__) noexcept -> R

#define INSTSAT(R, X, ...) \
  INSTSAT2(R, X, int8_t, ##__VA_ARGS__); \
  INSTSAT2(R, X, uint8_t, ##__VA_ARGS__); \
  INSTSAT2(R, X, int16_t, ##__VA_ARGS__); \
  INSTSAT2(R, X, uint16_t, ##__VA_ARGS__); \
  INSTSAT2(R, X, int32_t, ##__VA_ARGS__); \
  INSTSAT2(R, X, uint32_t, ##__VA_ARGS__); \
  INSTSAT2(R, X, int64_t, ##__VA_ARGS__); \
  INSTSAT2(R, X, uint64_t, ##__VA_ARGS__)

#define INST2(X, ...) \
  template int8_t std::X<int8_t>(int8_t, int8_t, ##__VA_ARGS__) noexcept;\
  template uint8_t std::X<uint8_t>(uint8_t, uint8_t, ##__VA_ARGS__) noexcept;\
  template int16_t std::X<int16_t>(int16_t, int16_t, ##__VA_ARGS__) noexcept;\
  template uint16_t std::X<uint16_t>(uint16_t, uint16_t, ##__VA_ARGS__) noexcept;\
  template int32_t std::X<int32_t>(int32_t, int32_t, ##__VA_ARGS__) noexcept;\
  template uint32_t std::X<uint32_t>(uint32_t, uint32_t, ##__VA_ARGS__) noexcept;\
  template int64_t std::X<int64_t>(int64_t, int64_t, ##__VA_ARGS__) noexcept;\
  template uint64_t std::X<uint64_t>(uint64_t, uint64_t, ##__VA_ARGS__) noexcept

//Do explicit instantiations to check for compiler errors and warnings
INST(shll, int);
INST(shlr, int);
INST(shal, int);
INST(shar, int);
INST(rotl, int);
INST(rotr, int);
INSTR(int, cntt0);
INSTR(int, cntl0);
INSTR(int, cntt1);
INSTR(int, cntl1);
INSTR(int, popcount);
INSTR(int, parity);

INST(rstls1b);
INST(isols1b);
INST(setls0b);
INST(isols0b);
INST(rstt1);
INST(sett0);
INST(mskt0);
INST(mskt1);
INST(mskt0ls1b);
INST(mskt1ls0b);
INST(revbits, int, int);
INST(revbytes, int, int);

INST(setbit, int);
INST(rstbit, int);
INST(flipbit, int);
INSTR(bool, testbit, int);

INST(rstbitsge, int);
INST(rstbitsle, int);
INST(setbitsge, int);
INST(setbitsle, int);
INST(flipbitsge, int);
INST(flipbitsle, int);

INSTSAT(decltype(l+r), satadd);
INSTSAT(decltype(l-r), satsub);

INSTR(bool, ispow2);
INST(ceilp2);
INST(floorp2);

INST(align_up, size_t);
INST(align_down, size_t);
INSTR(bool, is_aligned, size_t);

INST2(pbits_deposit);
INST2(pbits_extract);

TEST(Compile, Test) {
}

