#include <byteorder.hh>

#include <cstdlib>
#include <cstdint>
#include <iostream>

using namespace std;

int main(int argc, char** argv) {
  if(argc < 2) {
    cerr << "Please specify an integer!" << std::endl;
    return 1;
  }

  uint32_t val = atoi(argv[1]);

  cout << "v = " << val << std::endl;
  cout << "bswap(v) = " << bswap(val) << std::endl;
  cout << "cpu_to_le(v) = " << cpu_to_le(val) << std::endl;
  cout << "cpu_to_be(v) = " << cpu_to_be(val) << std::endl;

  return 0;
}
