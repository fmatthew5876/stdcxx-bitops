#ifndef BITOPS_HH
#define BITOPS_HH

#include <limits>
#include <type_traits>
#include <cstdint>
#include <climits>

namespace std {

///////////////////////////
//Basic bitwise operations
///////////////////////////

//Returns the number of trailing 0-bits in x, starting at the least significant bit position. If x is 0, the result is undefined.
template <typename T> constexpr int ctz(T t) noexcept = delete;
template <> constexpr int ctz(unsigned int t) noexcept {
  return __builtin_ctz(t);
}
template <> constexpr int ctz(unsigned long t) noexcept {
  return __builtin_ctzl(t);
}
template <> constexpr int ctz(unsigned long long t) noexcept {
  return __builtin_ctzll(t);
}
template <> constexpr int ctz(unsigned char t) noexcept {
  return ctz<unsigned int>(t);
}
template <> constexpr int ctz(unsigned short t) noexcept {
  return ctz<unsigned int>(t);
}

//Returns the number of leading 0-bits in x, starting at the most significant bit position. If x is 0, the result is undefined.
template <typename T> constexpr int clz(T t) noexcept = delete;
template <> constexpr int clz(unsigned int t) noexcept {
  return __builtin_clz(t);
}
template <> constexpr int clz(unsigned long t) noexcept {
  return __builtin_clzl(t);
}
template <> constexpr int clz(unsigned long long t) noexcept {
  return __builtin_clzll(t);
}
template <> constexpr int clz(unsigned char t) noexcept {
  return clz<unsigned int>(t) - ((sizeof(unsigned int) - sizeof(t)) * CHAR_BIT);
}
template <> constexpr int clz(unsigned short t) noexcept {
  return clz<unsigned int>(t) - ((sizeof(unsigned int) - sizeof(t)) * CHAR_BIT);
}

//Return position of the first bit set in t.
template <typename T> constexpr int ffs(T t) noexcept = delete;
template <> constexpr int ffs(unsigned int t) noexcept {
  return __builtin_ffs(t);
}
template <> constexpr int ffs(unsigned long t) noexcept {
  return __builtin_ffsl(t);
}
template <> constexpr int ffs(unsigned long long t) noexcept {
  return __builtin_ffsll(t);
}
template <> constexpr int ffs(unsigned char t) noexcept {
  return ffs<unsigned int>(t);
}
template <> constexpr int ffs(unsigned short t) noexcept {
  return ffs<unsigned int>(t);
}

//Returns position of the last bit set in t
template <typename T> constexpr int fls(T t) noexcept {
  static_assert(std::is_unsigned<T>::value, "T must be unsigned!");
  return (sizeof(t) * CHAR_BIT) - clz(t);
}

//Returns the number of leading redundant sign bits in x, i.e. the number of bits following the most significant bit that are identical to it. There are no special cases for 0 or other values
template <typename T> constexpr int clrsb(T t) noexcept = delete;
template <> constexpr int clrsb(unsigned int t) noexcept {
  return __builtin_clrsb(t);
}
template <> constexpr int clrsb(unsigned long t) noexcept {
  return __builtin_clrsbl(t);
}
template <> constexpr int clrsb(unsigned long long t) noexcept {
  return __builtin_clrsbll(t);
}
template <> constexpr int clrsb(unsigned char t) noexcept {
  return clrsb<unsigned int>(t) - ((sizeof(unsigned int) - sizeof(t)) * CHAR_BIT);
}
template <> constexpr int clrsb(unsigned short t) noexcept {
  return clrsb<unsigned int>(t) - ((sizeof(unsigned int) - sizeof(t)) * CHAR_BIT);
}

//Returns the number of 1-bits in x.
template <typename T> constexpr int popcount(T t) noexcept = delete;
template <> constexpr int popcount(unsigned int t) noexcept {
  return __builtin_popcount(t);
}
template <> constexpr int popcount(unsigned long t) noexcept {
  return __builtin_popcount(t);
}
template <> constexpr int popcount(unsigned long long t) noexcept {
  return __builtin_popcount(t);
}
template <> constexpr int popcount(unsigned char t) noexcept {
  return popcount<unsigned int>(t);
}
template <> constexpr int popcount(unsigned short t) noexcept {
  return popcount<unsigned int>(t);
}

//Returns the parity of x, i.e. the number of 1-bits in x modulo 2.
template <typename T> constexpr int parity(T t) noexcept = delete;
template <> constexpr int parity(unsigned int t) noexcept {
  return __builtin_parity(t);
}
template <> constexpr int parity(unsigned long t) noexcept {
  return __builtin_parityl(t);
}
template <> constexpr int parity(unsigned long long t) noexcept {
  return __builtin_parityll(t);
}
template <> constexpr int parity(unsigned char t) noexcept {
  return parity<unsigned int>(t);
}
template <> constexpr int parity(unsigned short t) noexcept {
  return parity<unsigned int>(t);
}

///////////////////////////
//Explicit bit shifts
///////////////////////////

//logical shift left
template <typename T>
constexpr auto shl(T t, unsigned int s)
  noexcept -> typename std::enable_if<std::is_integral<T>::value,T>::type {
    return t << s;
  }

//logical shift right
template <typename T>
constexpr auto shr(T t, unsigned int s) {
  noexcept -> typename std::enable_if<std::is_unsigned<T>::value,T>::type {
    //For unsigned types, built in right shift is guaranteed to be logical
    return t >> s;
  }
template <typename T> {
constexpr auto shr(T t, unsigned int s) {
  noexcept -> typename std::enable_if<std::is_signed<T>::value,T>::type {
    //For signed types, built in right shift is implementation defined. Cast to unsigned and shift.
    return static_cast<T>(shr(typename make_unsigned<T>::type(t), s));
  }
template <typename T>
constexpr auto shift(T t, int s)
  noexcept -> typename std::enable_if<std::is_integral<T>::value,T>::type {
    return s < 0 ? shl(t, -s) : shr(t, s);
}

//left shift arithmetic
template <typename T>
constexpr T sal(T t, unsigned int s) noexcept {
  return shl(t, s);
}

//right shift arithmetic. This implementation assumes right shift on signed types is arithmetic.
template <typename T>
constexpr auto sar(T t, unsigned int s)
  noexcept -> typename std::enable_if<std::is_signed<T>::value,T>::type {
    return t >> s;
  }
template <typename T>
constexpr auto sar(T t, unsigned int s)
  noexcept -> typename std::enable_if<std::is_unsigned<T>::value,T>::type {
    return sar(typename make_signed<T>::type(t), s);
  }
template <typename T>
constexpr auto sa(T t, int s)
  noexcept -> typename std::enable_if<std::is_integral<T>::value,T>::type {
    return s < 0 ? sal(t, -s) : sar(t, s);
}

//Left rotate shift
template <typename T>
constexpr T rotl(T t, unsigned int s)
  noexcept -> typename std::enable_if<std::is_integral<T>::value,T>::type {
  return shl(t, s) | shr(t, (sizeof(t) * CHAR_BIT) - s);
}

//Right rotate shift
template <typename T>
constexpr T rotr(T t, unsigned int s)
  noexcept -> typename std::enable_if<std::is_integral<T>::value,T>::type {
  return shr(t, s) | shl(t, (sizeof(t) * CHAR_BIT) - s);
}

template <typename T>
constexpr T rot(T t, int s)
  noexcept -> typename std::enable_if<std::is_integral<T>::value,T>::type {
  return s < 0 ? rotl(t, -s) : rotr(t, s);
}

///////////////////////////
//Alignment manipulation
///////////////////////////

//Returns the smallest number n when n >= val && is_aligned(n, align). align must be a power of 2!
//Question: Provide a version of this for char* pointers? Or require user to cast to uint_ptr_t?
template <typename T>
constexpr T align_up(T val, size_t a) noexcept {
  return ((val + (a -1)) & -a);
}

//Returns the largest number n when n <= val && is_aligned(n, align). align must be a power of 2!
//Question: Provide a version of this for char* pointers? Or require user to cast to uint_ptr_t?
template <typename T>
constexpr T align_down(T val, size_t a) noexcept {
  return val & -a;
}

//Returns true if t is aligned to a
//Question: Provide a version of this for char* pointers? Or require user to cast to uint_ptr_t?
template <typename T>
constexpr bool is_aligned(T t, size_t a) noexcept {
  return ((t & (a-1)) == 0);
}

///////////////////////////
//Power of 2 manipulation
///////////////////////////

//Returns true if t == 0 or t is a power of 2
template <typename T> constexpr bool is_pow2_or_zero(T t) noexcept {
  static_assert(is_unsigned<T>::value, "T must be an unsigned type!");
  return (t & (t-1)) == 0;
}

//Returns true if t is a power of 2
template <typename T> constexpr bool is_pow2(T t) noexcept {
  static_assert(is_unsigned<T>::value, "T must be an unsigned type!");
  return (t != 0) && is_pow2_or_zero(t);
}

//Return smallest power of 2 >= t
template <typename T>
constexpr T pow2_ge(T t) noexcept {
  static_assert(is_unsigned<T>::value, "T must be an unsigned type!");
  return is_pow2(t) ? t : pow2_gt(t);
}

//Return smallest power of 2 > t
template <typename T> constexpr T pow2_gt(T t) noexcept;

//Return smallest power of 2 <= t
template <typename T>
constexpr T pow2_le(T t) noexcept {
  return is_pow2(t) ? t : pow2_lt(t);
}

//Return smallest power of 2 < t
template <typename T>
constexpr T pow2_lt(T t) noexcept;


///////////////////////////
//Setting/Resetting bits
///////////////////////////

//Sets bit b of t, no effect if b >= number of bits in t
template <typename T, typename B>
constexpr T set_bit(T t, B b) noexcept {
  return t | (1 << b);
}

//Set all bits in t >= b
template <typename T, typename B>
constexpr T set_bits_gt(T t, B b) noexcept {
  return t | ~((1 << (b+1)) -1);
}

//Set all bits in t > b
template <typename T, typename B>
constexpr T set_bits_ge(T t, B b) noexcept {
  return t | ~((1 << b) -1);
}

//Set all bits in t <= b
template <typename T, typename B>
constexpr T set_bits_le(T t, B b) noexcept {
  return t | ((1 << (b+1)) -1);
}

//Set all bits in t < b
template <typename T, typename B>
constexpr T set_bits_lt(T t, B b) noexcept {
  return t | ((1 << b) -1);
}

//Resets bit b of t, no effect if b >= number of bits in t
template <typename T, typename B>
constexpr T reset_bit(T t, B b) noexcept {
  return t & ~(1 << b);
}

//Reset all bits in t >= b
template <typename T, typename B>
constexpr T reset_bits_gt(T t, B b) noexcept ;

//Reset all bits in t > b
template <typename T, typename B>
constexpr T reset_bits_ge(T t, B b) noexcept ;

//Reset all bits in t <= b
template <typename T, typename B>
constexpr T reset_bits_le(T t, B b) noexcept ;

//Reset all bits in t < b
template <typename T, typename B>
constexpr T reset_bits_lt(T t, B b) noexcept ;

//Resets the least significant bit set
template <typename T>
constexpr T reset_lsb(T t) noexcept {
  return t & (t -1);
}

//Resets the most significant bit set
template <typename T>
constexpr T reset_msb(T t);

////////////////////////
//Saturated Arithmetic
////////////////////////

//Saturated addition, like normal addition except on overflow the result will be the maximum value for decltype(L + R).
template <typename L, typename R>
constexpr auto sat_add(L l, R r) -> decltype(l+r) {
  typedef decltype(l+r) LR;
  return static_cast<LR>(l) > numeric_limits<LR>::max() - static_cast<LR>(r) ?
    numeric_limits<LR>::max() : l + r;
}

//Saturated subtraction, like normal subtraction except on overflow the result will be the minimum value for decltype(L - R).
template <typename L, typename R>
constexpr auto sat_sub(L l, R r) -> decltype(l-r) {
  typedef decltype(l-r) LR;
  return static_cast<LR>(l) < numeric_limits<LR>::min() + static_cast<LR>(r) ?
    numeric_limits<LR>::min() : l - r;
}

////////////////////////
//Misc
////////////////////////

//Reverses all of the bits in t
template <typename T>
constexpr T reverse_bits(T t) noexcept ;

//Returns a value whos even bits are set to the even bits of even, and odd bits set to the odd bits of odd.
template <typename T>
constexpr T interleave_bits(T even, T odd);


//Swaps the nibbles (4 bits) of the given byte
constexpr uint8_t swap_nibbles(uint8_t byte) {
  return (byte >> 4) | (byte << 4);
}

} //namespace std

#endif
