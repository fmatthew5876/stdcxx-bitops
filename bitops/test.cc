#include <bitops.hh>

#include <cstdlib>
#include <cstdint>
#include <cstdio>

#define PRINT(X) printf(#X "(v) = %08Xu %u\n", X(val), X(val))
#define PRINTB(X) printf(#X "(v) = %d\n", X(val))
#define PRINT2(X, A) printf(#X "(v,%d) = %08Xu %u\n", A, X(val, A), X(val, A))
#define PRINTB2(X, A) printf(#X "(v,%d) = %d\n", A, X(val, A))

using namespace std;

int main(int argc, char** argv) {
  if(argc < 2) {
    fprintf(stderr, "Please specify an integer!\n");
    return 1;
  }

  uint32_t val = atoi(argv[1]);
  PRINT(is_pow2_or_zero);
  PRINT(is_pow2);

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

  PRINT(ctz);
  PRINT(clz);
  PRINT(ffs);
  PRINT(popcount);
  PRINT(parity);

  return 0;
}
