#ifndef STDCXX_BYTEORDER_HH
#define STDCXX_BYTEORDER_HH

#include <cstdint>
#include <cstddef>

namespace std {

//enum class for compile-time byte-order.
enum class byte_order {
  little,
  big,
  native = little //Implementation defined
};

//Macro interface. What naming convention to use for macros?
#define STD_LITTLE_ENDIAN 0
#define STD_BIG_ENDIAN 1
#define STD_BYTEORDER STD_LITTLE_ENDIAN //Implementation defined

//An efficient byte swapping routine bswap() for integral types.
//Implementation can and should be optimized for each cpu with compiler intrinsics.
//Most compilers already do this as shown below.
template <typename T> constexpr T bswap(T v);

//Specializations for integral types (see below).
template <> constexpr uint8_t bswap(uint8_t v) { return v; }
template <> constexpr uint16_t bswap(uint16_t v);
template <> constexpr uint32_t bswap(uint32_t v);
template <> constexpr uint64_t bswap(uint64_t v);
template <> constexpr uint32_t bswap(uint32_t v);

//Signed integer versions
template <> constexpr int8_t bswap(int8_t v) { return v; }
template <> constexpr int16_t bswap(int16_t v) { return int16_t(bswap(uint16_t(v))); }
template <> constexpr int32_t bswap(int32_t v) { return int32_t(bswap(uint32_t(v))); }
template <> constexpr int64_t bswap(int64_t v) { return int64_t(bswap(uint64_t(v))); }

//Char is a separate type fron signed char and unsigned char.
template <> constexpr char bswap(char v) { return v; }

//Byteswap routines for floating point.
//Side question: Does this implementation conflict with strict aliasing? Is it standard compliant?
template <>
constexpr float bswap(float v) { return *reinterpret_cast<float*>(bswap(*reinterpret_cast<uint32_t*>(&v))); }
template <>
constexpr double bswap(double v) { return *reinterpret_cast<double*>(bswap(*reinterpret_cast<uint64_t*>(&v))); }
template <>
constexpr long double bswap(long double v) { return *reinterpret_cast<long double*>(bswap(*reinterpret_cast<uint64_t*>(&v))); }

//Byteswap routines for raw buffers? Is this useful?
//Palindromes anyone?
//How to implement this efficiently (iterative loop)  but maintain constexpr (recursion)?
constexpr void bswap(void* v, size_t nbytes);
constexpr void cpu_to_le(void* v, size_t nbytes);
constexpr void cpu_to_be(void* v, size_t nbytes);

//Convert to/from host order to big endian or little endian in a cross platform manner.
template <typename T> constexpr T cpu_to_le(T v) {
  return byte_order::little == byte_order::native ? v : bswap(v);
}
template <typename T> constexpr T cpu_to_be(T v) {
  return byte_order::little == byte_order:: native ? bswap(v) : v;
}
template <typename T> constexpr T le_to_cpu(T v) {
  return cpu_to_le(v);
}
template <typename T> constexpr T be_to_cpu(T v) {
  return cpu_to_be(v);
}

//More explicit version for template meta-programming
template <typename T, byte_order in, byte_order out>
constexpr T bconvert(T t) {
  return in == out ? t : bswap(t);
}

//Runtime endian swapping
template <typename T>
constexpr T bconvert(T t, byte_order in, byte_order out) {
  return in == out ? t : bswap(t);
}

//Short hand for probably most common runtime swap.
template <typename T>
constexpr T cpu_to(T t, byte_order out) {
  return byte_order::native == out ? t : bwap(t);
}

//Modern implementation of bswap, taking advantage of compiler intrinsics available today.
#if defined(__GNUC__)
#if __GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR >= 7)
template <>
constexpr uint16_t bswap(uint16_t v) {
  return __builtin_bswap16(v);
}
#else
template <>
constexpr uint16_t bswap(uint16_t v) {
  return (v >> 8) | (v << 8);
}
#endif
template <>
constexpr uint32_t bswap(uint32_t v) {
  return __builtin_bswap32(v);
}
template <>
constexpr uint64_t bswap(uint64_t v) {
  return __builtin_bswap64(v);
}
#elif defined(_MSC_VER) 
template <>
constexpr uint16_t bswap(uint16_t v) {
  return _byteswap_ushort(v);
}
template <>
constexpr uint32_t bswap(uint32_t v) {
  return _byteswap_ulong(v);
}
template <>
constexpr uint64_t bswap(uint64_t v) {
  return _byteswap_uint64(v);
}
#else
template <>
constexpr uint16_t bswap(uint16_t v) {
  return (v >> 8) | (v << 8);
}
template <>
constexpr uint32_t bswap(uint32_t x)
{
  return
    ((((v) & 0xff000000) >> 24) | (((v) & 0x00ff0000) >>  8) |
     (((v) & 0x0000ff00) <<  8) | (((v) & 0x000000ff) << 24));
}
template <>
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

} //namespace std

#endif
