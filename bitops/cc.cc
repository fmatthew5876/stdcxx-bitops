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
INST(lshl, int);
INST(rshl, int);
INST(lsha, int);
INST(rsha, int);
INST(lrot, int);
INST(rrot, int);
INSTR(int, count_tzb);
INSTR(int, count_lzb);
INSTR(int, count_t1b);
INSTR(int, count_l1b);
INSTR(int, popcount);
INSTR(int, parity);

INST(reset_ls1b);
INST(isolate_ls1b);
INST(set_lszb);
INST(isolate_lszb);
INST(inv_isolate_ls1b);
INST(reset_t1b);
INST(set_tzb);
INST(mask_tzb);
INST(mask_tzb_ls1b);
INST(inv_mask_t1b);
INST(mask_t1b_lszb);
INST(reset_rstr1b);
INST(revbits, int, int);
INST(revbytes, int, int);

INSTSAT(decltype(l+r), satadd);
INSTSAT(decltype(l-r), satsub);

INST(set_bit, int);
INST(reset_bit, int);
INST(flip_bit, int);
INSTR(bool, test_bit, int);

INST(reset_bitsge, int);
INST(reset_bitsle, int);
INST(set_bitsge, int);
INST(set_bitsle, int);
INST(flip_bitsge, int);
INST(flip_bitsle, int);

INSTR(bool, ispow2oz);
INSTR(bool, ispow2);
INST(ceilp2);
INST(floorp2);

INST(align_up, size_t);
INST(align_down, size_t);
INSTR(bool, is_aligned, size_t);

INST2(pbits_deposit);
INST2(pbits_extract);
INST(snoob);

TEST(Compile, Test) {
}

