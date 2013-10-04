#ifndef STDCXX_BYTEORDER_HH
#define STDCXX_BYTEORDER_HH

#include <cstdint>

namespace std {

//Required: a compile time variable telling us the endianness of the machine.
struct byte_order {
  enum order {
    little_endian = 1234,
    big_endian = 4321,
    //Optional: pdp endian? Linux defines this.
    pdp_endian = 3412,
  };
  constexpr static order value = little_endian; //Implementation defined value little_endian or big_endian.
};

//Required: an efficient byte swapping routine bswap() for integral types.
//This can and should be optimized for each cpu with compiler intrinsics.
//Most compilers already do this as shown below.
template <typename T> constexpr T bswap(T v);
#if defined(__GNUC__)
#if __GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR >= 7)
constexpr uint16_t bswap(uint16_t v) {
  return __builtin_bswap16(v);
}
#else
constexpr uint16_t bswap(uint16_t v) {
  return (v >> 8) | (v << 8);
}
#endif
constexpr uint32_t bswap(uint32_t v) {
  return __builtin_bswap32(v);
}
constexpr uint64_t bswap(uint64_t v) {
  return __builtin_bswap64(v);
}
#elif defined(_MSC_VER) 
constexpr uint16_t bswap(uint16_t v) {
  return _byteswap_ushort(v);
}
constexpr uint32_t bswap(uint32_t v) {
  return _byteswap_ulong(v);
}
constexpr uint64_t bswap(uint64_t v) {
  return _byteswap_uint64(v);
}
#else
constexpr uint16_t bswap(uint16_t v) {
  return (v >> 8) | (v << 8);
}
constexpr uint32_t bswap(uint32_t x)
{
  return
    ((((v) & 0xff000000) >> 24) | (((v) & 0x00ff0000) >>  8) |
     (((v) & 0x0000ff00) <<  8) | (((v) & 0x000000ff) << 24));
}
constexpr uint64_t bswap(uint64_t x)
{
  return
    ((((v) & 0xff00000000000000ULL) >> 56) |
     (((v) & 0x00ff000000000000ULL) >> 40) |
     (((v) & 0x0000ff0000000000ULL) >> 24) |
     (((v) & 0x000000ff00000000ULL) >>  8) |
     (((v) & 0x00000000ff000000ULL) <<  8) |
     (((v) & 0x0000000000ff0000ULL) << 24) |
     (((v) & 0x000000000000ff00ULL) << 40) |
     (((v) & 0x00000000000000ffULL) << 56));
}
#endif
//Optional: Suggest implementations provide byteswapping routines for 128 and larger
//implementation defined data types like simd vector types.

//Required: Convert to/from host order to big endian or little endian in a cross platform manner.
template <typename T>
constexpr T cpu_to_le(T t) { return byte_order::value == byte_order::little_endian ? t : bswap(t); }

template <typename T>
constexpr T cpu_to_be(T t) { return byte_order::value == byte_order::little_endian ? bswap(t) : t; }

//Optional: Do we want XX_to_cpu as well for symmetry?
template <typename T>
constexpr T le_to_cpu(T t) { return cpu_to_le(t); }
template <typename T>
constexpr T be_to_cpu(T t) { return cpu_to_be(t); }

//Optional: Specifically sized conversion methods? Is this useful? Maybe for templates?
//Why not just cast if you want to change the size?
//bswap(static_cast<uint32_t>(t)); vs bswap32(t);
constexpr uint16_t bswap16(uint16_t t) { return bswap(t); }
constexpr uint32_t bswap32(uint32_t t) { return bswap(t); }
constexpr uint64_t bswap64(uint64_t t) { return bswap(t); }
//Optional: Implementation may define bswap128(), bswap(256), etc..

//Optional: Similar to bswap
constexpr uint16_t cpu_to_le16(uint16_t t) { return cpu_to_le(t); }
constexpr uint32_t cpu_to_le32(uint32_t t) { return cpu_to_le(t); }
constexpr uint64_t cpu_to_le64(uint64_t t) { return cpu_to_le(t); }

constexpr uint16_t cpu_to_be16(uint16_t t) { return cpu_to_le(t); }
constexpr uint32_t cpu_to_be32(uint32_t t) { return cpu_to_le(t); }
constexpr uint64_t cpu_to_be64(uint64_t t) { return cpu_to_le(t); }

//Optional: Should we support signed integers as well?
constexpr int16_t bswap(int16_t t) { return int16_t(bswap(uint16_t(t))); }
constexpr int32_t bswap(int32_t t) { return int32_t(bswap(uint32_t(t))); }
constexpr int64_t bswap(int64_t t) { return int64_t(bswap(uint64_t(t))); }

//Optional: Should we support floating point types? Do binary formats or hardware devices need this?
constexpr float bswap(float f);
constexpr double bswap(double f);
constexpr long double bswap(long double f);

//Optional: Byte Swapping an arbitrary sized buffer? Is this at all useful?
constexpr void bswap(void* v, size_t nbytes);
constexpr void cpu_to_le(void* v, size_t nbytes);
constexpr void cpu_to_be(void* v, size_t nbytes);

//Optional: Do we need/want a macro interface?
//I think its worth having. How does it hurt?
#define STD_LITTLE_ENDIAN 1234
#define STD_BIG_ENDIAN 4321
#define STD_BYTEORDER STD_LITTLE_ENDIAN /* Implementation defined */

} //namespace std

#endif
