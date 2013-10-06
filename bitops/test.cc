#include <bitops.hh>

#include <cstdlib>
#include <cstdint>
#include <cstdio>

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

  return 0;
}
