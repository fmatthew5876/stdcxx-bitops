#ifndef STDCXX_BYTEORDER_HH
#define STDCXX_BYTEORDER_HH

#include <cstdint>
#include <cstddef>

//Macro interface. What naming convention to use for macros?
#define STD_LITTLE_ENDIAN 0
#define STD_BIG_ENDIAN 1
#define STD_BYTEORDER STD_LITTLE_ENDIAN //Implementation defined

namespace std {

//List of possible byte orders
enum class endian {
  little,
  big
};

//The byte order on this system. This can be specialized
//for different types if the system uses different byte
//orders for those types.
template <typename T>
struct byte_order {
  static constexpr endian value = endian::little; //Implementation defined value
};

//An efficient byte swapping routine swap_bytes() for integral types.
//Implementation can and should be optimized for each cpu with compiler intrinsics.
//Most compilers already do this as shown below.
template <typename T> constexpr T swap_bytes(T v) = delete;

//Specializations for integral types (see below).
template <> constexpr uint8_t swap_bytes(uint8_t v) { return v; }
template <> constexpr uint16_t swap_bytes(uint16_t v);
template <> constexpr uint32_t swap_bytes(uint32_t v);
template <> constexpr uint64_t swap_bytes(uint64_t v);
template <> constexpr uint32_t swap_bytes(uint32_t v);

//Signed integer versions
template <> constexpr int8_t swap_bytes(int8_t v) { return v; }
template <> constexpr int16_t swap_bytes(int16_t v) { return int16_t(swap_bytes(uint16_t(v))); }
template <> constexpr int32_t swap_bytes(int32_t v) { return int32_t(swap_bytes(uint32_t(v))); }
template <> constexpr int64_t swap_bytes(int64_t v) { return int64_t(swap_bytes(uint64_t(v))); }

//Char is a separate type fron signed char and unsigned char.
template <> constexpr char swap_bytes(char v) { return v; }

//Byteswap routines for floating point.
//Side question: Does this implementation conflict with strict aliasing? Is it standard compliant?
template <>
constexpr float swap_bytes(float v) { return *reinterpret_cast<float*>(swap_bytes(*reinterpret_cast<uint32_t*>(&v))); }
template <>
constexpr double swap_bytes(double v) { return *reinterpret_cast<double*>(swap_bytes(*reinterpret_cast<uint64_t*>(&v))); }
template <>
constexpr long double swap_bytes(long double v);

//Convert to/from host order to big endian or little endian in a cross platform manner.
template <typename T> constexpr T host_to_le(T v) {
  return byte_order<T>::value == endian::little ? v : swap_bytes(v);
}
template <typename T> constexpr T host_to_be(T v) {
  return byte_order<T>::value == endian::big ? v : swap_bytes(v);
}
template <typename T> constexpr T le_to_host(T v) {
  return host_to_le(v);
}
template <typename T> constexpr T be_to_host(T v) {
  return host_to_be(v);
}

//More explicit version for template meta-programming
template <typename T, endian in, endian out>
constexpr T reorder_bytes(T t) {
  return in == out ? t : swap_bytes(t);
}

//Runtime endian swapping, with a sensible default
template <typename T>
constexpr T reorder_bytes(T t, endian in, endian out=byte_order<T>::value) {
  return in == out ? t : swap_bytes(t);
}

//Modern implementation of swap_bytes, taking advantage of compiler intrinsics available today.
#if defined(__GNUC__)
#if __GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR >= 7)
template <>
constexpr uint16_t swap_bytes(uint16_t v) {
  return __builtin_bswap16(v);
}
#else
template <>
constexpr uint16_t swap_bytes(uint16_t v) {
  return (v >> 8) | (v << 8);
}
#endif
template <>
constexpr uint32_t swap_bytes(uint32_t v) {
  return __builtin_bswap32(v);
}
template <>
constexpr uint64_t swap_bytes(uint64_t v) {
  return __builtin_bswap64(v);
}
#elif defined(_MSC_VER) 
template <>
constexpr uint16_t swap_bytes(uint16_t v) {
  return _byteswap_ushort(v);
}
template <>
constexpr uint32_t swap_bytes(uint32_t v) {
  return _byteswap_ulong(v);
}
template <>
constexpr uint64_t swap_bytes(uint64_t v) {
  return _byteswap_uint64(v);
}
#else
template <>
constexpr uint16_t swap_bytes(uint16_t v) {
  return (v >> 8) | (v << 8);
}
template <>
constexpr uint32_t swap_bytes(uint32_t x)
{
  return
    ((((v) & 0xff000000) >> 24) | (((v) & 0x00ff0000) >>  8) |
     (((v) & 0x0000ff00) <<  8) | (((v) & 0x000000ff) << 24));
}
template <>
constexpr uint64_t swap_bytes(uint64_t x)
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
