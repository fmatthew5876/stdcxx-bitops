#include <byteorder.hh>

#include <cstdlib>
#include <cstdint>
#include <cstdio>

using namespace std;

int main(int argc, char** argv) {
  if(argc < 2) {
    fprintf(stderr, "Please specify an integer!\n");
    return 1;
  }

  uint32_t val = atoi(argv[1]);

  printf("v = %08Xu (%u)\n", val, val);
  printf("bswap(v) = %08Xu (%u)\n", bswap(val), bswap(val));
  printf("cpu_to_le(v) = %08Xu (%u)\n", cpu_to_le(val), cpu_to_le(val));
  printf("cpu_to_be(v) = %08Xu (%u)\n", cpu_to_be(val), cpu_to_be(val));

  return 0;
}
