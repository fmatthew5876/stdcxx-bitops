#include <bitops.hh>

#include <cstdlib>
#include <cstdint>
#include <cstdio>

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
INSTR(int, ctzb);
INSTR(int, clzb);
INSTR(int, clrsb);
INSTR(int, popcount);
INSTR(int, parity);
INST(resetls1b);
INST(isolatels1b);
INST(maskls1b);
INST(setlszb);
INST(revbits, int, int);
INST(revbytes, int, int);

INSTSAT(decltype(l+r), satadd);
INSTSAT(decltype(l-r), satsub);

INST(setbit, int);
INST(resetbit, int);
INST(flipbit, int);
INSTR(bool, testbit, int);

INST(resetbitsge, int);
INST(resetbitsle, int);
INST(setbitsge, int);
INST(setbitsle, int);
INST(flipbitsge, int);
INST(flipbitsle, int);

INST(resett1b);
INST(settzb);

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

#define PRINT(X) printf(#X "(v) = %08Xu %u\n", X(val), X(val))
#define PRINTB(X) printf(#X "(v) = %d\n", X(val))
#define PRINT2(X, A) printf(#X "(v,%d) = %08Xu %u\n", A, X(val, A), X(val, A))
#define PRINTB2(X, A) printf(#X "(v,%d) = %d\n", A, X(val, A))

using namespace std;

void foo(int x) {
  printf("X %d\n", x);
}

int main(int argc, char** argv) {
  if(argc < 2) {
    fprintf(stderr, "Please specify an integer!\n");
    return 1;
  }

  foo(sizeof(int));

  uint32_t val = atoi(argv[1]);

  printf("v = %08X %u\n", val, val);

#if 0
  PRINT(ctz);
  PRINT(clz);
  PRINT(ffs);
  PRINT(fls);
  PRINT(clrsb);
  PRINT(popcount);
  PRINT(parity);

  //PRINT(pow2_ge);
  //PRINT(pow2_gt);
  //PRINT(pow2_le);
  //PRINT(pow2_lt);

  PRINTB(is_pow2_or_zero);
  PRINTB(is_pow2);

  PRINT2(align_up, 2);
  PRINT2(align_up, 4);
  PRINT2(align_up, 8);
  PRINT2(align_up, 16);

  PRINT2(align_down, 2);
  PRINT2(align_down, 4);
  PRINT2(align_down, 8);
  PRINT2(align_down, 16);

  PRINTB2(is_aligned, 2);
  PRINTB2(is_aligned, 4);
  PRINTB2(is_aligned, 8);
  PRINTB2(is_aligned, 16);

  //PRINT(reverse_bits);

  PRINT2(shl, 8U);
  PRINT2(shr, 8U);

  PRINT2(sal, 8U);
  PRINT2(sar, 8U);

  //PRINT2(rotl, 8);
  //PRINT2(rotr, 8);
  //PRINT2(rot, -16);
  //PRINT2(rot, 16);

  PRINT2(set_bit, 4);
  //PRINT2(set_bits_ge, 4);
  //PRINT2(set_bits_gt, 4);
  //PRINT2(set_bits_le, 4);
  //PRINT2(set_bits_lt, 4);

  PRINT2(reset_bit, 4);
  //PRINT2(reset_bits_ge, 4);
  //PRINT2(reset_bits_gt, 4);
  //PRINT2(reset_bits_le, 4);
  //PRINT2(reset_bits_lt, 4);

  PRINT(reset_lsb);
  //PRINT(reset_msb);
  //PRINT2(interleave_bits, 0xFF);

  PRINT2(sat_add, 0xFFFFFF00);
  PRINT2(sat_sub, 0xFFFFFF00);

  PRINT(swap_nibbles);
#endif

  return 0;
}
