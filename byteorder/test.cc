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
  printf("byte_order<decltype(v)> = %d\n", byte_order<decltype(val)>::value);
  printf("swap_bytes(v) = %08Xu (%u)\n", swap_bytes(val), swap_bytes(val));
  printf("host_to_le(v) = %08Xu (%u)\n", host_to_le(val), host_to_le(val));
  printf("host_to_be(v) = %08Xu (%u)\n", host_to_be(val), host_to_be(val));

  return 0;
}
