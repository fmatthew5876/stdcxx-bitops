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
//x86: bsf
//alpha: cttz
//powerpc: 
constexpr int ctz(unsigned int t) noexcept {
  return __builtin_ctz(t);
}
constexpr int ctz(unsigned long t) noexcept {
  return __builtin_ctzl(t);
}
constexpr int ctz(unsigned long long t) noexcept {
  return __builtin_ctzll(t);
}
constexpr int ctz(unsigned char t) noexcept {
  return ctz(unsigned int(t));
}
constexpr int ctz(unsigned short t) noexcept {
  return ctz(unsigned int(t));
}

//Returns the number of leading 0-bits in x, starting at the most significant bit position. If x is 0, the result is undefined.
//x86: bsr
//alpha: ctlz
//power: cntlz[wd]
constexpr int clz(unsigned int t) noexcept {
  return __builtin_clz(t);
}
constexpr int clz(unsigned long t) noexcept {
  return __builtin_clzl(t);
}
constexpr int clz(unsigned long long t) noexcept {
  return __builtin_clzll(t);
}
constexpr int clz(unsigned char t) noexcept {
  return clz<unsigned int>(t) - ((sizeof(unsigned int) - sizeof(t)) * CHAR_BIT);
}
constexpr int clz(unsigned short t) noexcept {
  return clz<unsigned int>(t) - ((sizeof(unsigned int) - sizeof(t)) * CHAR_BIT);
}

//Return position of the first bit set in t, starting at 1 for the least significant bit, or 0 if t is 0.
//x86: bsf
//alpha: cttz
constexpr int ffs(unsigned int t) noexcept {
  return __builtin_ffs(t);
}
constexpr int ffs(unsigned long t) noexcept {
  return __builtin_ffsl(t);
}
constexpr int ffs(unsigned long long t) noexcept {
  return __builtin_ffsll(t);
}
constexpr int ffs(unsigned char t) noexcept {
  return ffs<unsigned int>(t);
}
constexpr int ffs(unsigned short t) noexcept {
  return ffs<unsigned int>(t);
}

//Returns position of the last bit set in t, starting at 1 for the least significant bit, or 0 if t is 0.
//x86: bsf, sub
//alpha: ctlz, sub
constexpr int fls(unsigned int t) noexcept {
  return t == 0U ? 0 : (sizeof(t) * CHAR_BIT) - clz(t)+1U;
}
constexpr int fls(unsigned long t) noexcept {
  return t == 0UL ? 0 : (sizeof(t) * CHAR_BIT) - clz(t)+1UL;
}
constexpr int fls(unsigned long long t) noexcept {
  return t == 0ULL ? 0 : (sizeof(t) * CHAR_BIT) - clz(t)+1ULL;
}
constexpr int fls(unsigned char t) noexcept {
  return fls(unsigned int(t));
}
constexpr int fls(unsigned short t) noexcept {
  return fls(unsigned int(t));
}

//Returns the number of leading redundant sign bits in x, i.e. the number of bits following the most significant bit that are identical to it. There are no special cases for 0 or other values
//x86: 
constexpr int clrsb(unsigned int t) noexcept {
  return __builtin_clrsb(t);
}
constexpr int clrsb(unsigned long t) noexcept {
  return __builtin_clrsbl(t);
}
constexpr int clrsb(unsigned long long t) noexcept {
  return __builtin_clrsbll(t);
}
constexpr int clrsb(unsigned char t) noexcept {
  return clrsb(unsigned int(t)) - ((sizeof(unsigned int) - sizeof(t)) * CHAR_BIT);
}
constexpr int clrsb(unsigned short t) noexcept {
  return clrsb(unsigned int(t)) - ((sizeof(unsigned int) - sizeof(t)) * CHAR_BIT);
}

//Returns the number of 1-bits in x.
//x86: popcnt
//alpha: ctpop
constexpr int popcount(unsigned int t) noexcept {
  return __builtin_popcount(t);
}
constexpr int popcount(unsigned long t) noexcept {
  return __builtin_popcount(t);
}
constexpr int popcount(unsigned long long t) noexcept {
  return __builtin_popcount(t);
}
constexpr int popcount(unsigned char t) noexcept {
  return popcount(unsigned int(t));
}
constexpr int popcount(unsigned short t) noexcept {
  return popcount(unsigned int(t));
}

//Returns the parity of x, i.e. the number of 1-bits in x modulo 2.
//x86: shrl, xorl, xorb, setnp, movzbl
constexpr int parity(unsigned int t) noexcept {
  return __builtin_parity(t);
}
constexpr int parity(unsigned long t) noexcept {
  return __builtin_parityl(t);
}
constexpr int parity(unsigned long long t) noexcept {
  return __builtin_parityll(t);
}
constexpr int parity(unsigned char t) noexcept {
  return parity(unsigned int(t));
}
constexpr int parity(unsigned short t) noexcept {
  return parity(unsigned int(t));
}

///////////////////////////
//Explicit bit shifts
///////////////////////////

//logical shift left
//x86: shl
//alpha: sll
template <typename T>
constexpr auto shl(T t, int s)
  noexcept -> typename std::enable_if<std::is_unsigned<T>::value,T>::type {
    return t << s;
  }

//logical shift right
//x86: shr
//alpha: srl
template <typename T>
constexpr auto shr(T t, int s)
  noexcept -> typename std::enable_if<std::is_unsigned<T>::value,T>::type {
    //For unsigned types, built in right shift is guaranteed to be logical
    return t >> s;
  }
//logical shift, left if s < 0, otherwise right
template <typename T>
constexpr auto sh(T t, int s)
  noexcept -> typename std::enable_if<std::is_unsigned<T>::value,T>::type {
    return s < 0 ? shl(t, -s) : shr(t, s);
  }

//left shift arithmetic
//x86: sal
//alpha: sll
template <typename T>
constexpr auto sal(T t, int s)
  noexcept -> typename std::enable_if<std::is_unsigned<T>::value,T>::type {
    //Same as logical shift
    return shl(t, s);
  }

//right shift arithmetic. 
//x86: sar
//alpha: srl
template <typename T>
constexpr auto sar(T t, int s)
  noexcept -> typename std::enable_if<std::is_unsigned<T>::value,T>::type {
    //This implementation assumes right shift on signed types is arithmetic but this cannot be assumed in general
    return static_cast<typename make_signed<T>::type>(t) >> s;
  }
//arithmetic shift, left if s < 0, otherwise right
template <typename T>
constexpr auto sa(T t, int s)
  noexcept -> typename std::enable_if<std::is_unsigned<T>::value,T>::type {
    return s < 0 ? sal(t, -s) : sar(t, s);
  }

//Left rotate shift
//x86: rol
template <typename T>
constexpr auto rotl(T t, int s)
  noexcept -> typename std::enable_if<std::is_unsigned<T>::value,T>::type {
    return shl(t, s) | shr(t, (sizeof(t) * CHAR_BIT) - s);
  }

//Right rotate shift
//x86: ror
template <typename T>
constexpr auto rotr(T t, int s)
  noexcept -> typename std::enable_if<std::is_unsigned<T>::value,T>::type {
    return shr(t, s) | shl(t, (sizeof(t) * CHAR_BIT) - s);
  }

//rotate shift, left if s < 0, otherwise right
template <typename T>
constexpr auto rot(T t, int s)
  noexcept -> typename std::enable_if<std::is_unsigned<T>::value,T>::type {
    return s < 0 ? rotl(t, -s) : rotr(t, s);
  }

///////////////////////////
//Alignment manipulation
///////////////////////////

//Returns the smallest number n when n >= val && is_aligned(n, align). align must be a power of 2!
template <typename T>
constexpr auto align_up(T val, size_t a)
  noexcept -> typename std::enable_if<std::is_integral<T>::value,T>::type {
  return ((val + (a -1)) & -a);
}
constexpr void* align_up(void* val, size_t a) noexcept {
  return (void*)align_up(uintptr_t(val), a);
}

//Returns the largest number n when n <= val && is_aligned(n, align). align must be a power of 2!
template <typename T>
constexpr auto align_down(T val, size_t a)
  noexcept -> typename std::enable_if<std::is_integral<T>::value,T>::type {
  return val & -a;
}
constexpr void* align_down(void* val, size_t a) noexcept {
  return (void*)align_down(uintptr_t(val), a);
}

//Returns true if t is aligned to a
template <typename T>
constexpr auto is_aligned(T t, size_t a)
  noexcept -> typename std::enable_if<std::is_integral<T>::value,bool>::type {
  return ((t & (a-1)) == 0);
}
constexpr bool is_aligned(void* t, size_t a) noexcept {
  return is_aligned(uintptr_t(t), a);
}

///////////////////////////
//Power of 2 manipulation
///////////////////////////

//Returns true if t == 0 or t is a power of 2
template <typename T>
constexpr auto is_pow2_or_zero(T t)
  noexcept -> typename std::enable_if<std::is_integral<T>::value,bool>::type {
  return (t & (t-1)) == 0;
}

//Returns true if t is a power of 2
template <typename T>
constexpr auto is_pow2(T t)
  noexcept -> typename std::enable_if<std::is_integral<T>::value,bool>::type {
  return (t != 0) && is_pow2_or_zero(t);
}

//Return smallest power of 2 >= t
template <typename T>
constexpr auto pow2_ge(T t)
  noexcept -> typename std::enable_if<std::is_integral<T>::value,T>::type {
  return is_pow2(t) ? t : pow2_gt(t);
}

//Return smallest power of 2 > t
template <typename T>
constexpr auto pow2_gt(T t)
  noexcept -> typename std::enable_if<std::is_integral<T>::value,T>::type {
    return (T(1) << (fls(make_unsigned<T>::type(t)) +1));
  }

//Return smallest power of 2 <= t
template <typename T>
constexpr auto pow2_le(T t)
  noexcept -> typename std::enable_if<std::is_integral<T>::value,T>::type {
  return is_pow2(t) ? t : pow2_lt(t);
}

//Return smallest power of 2 < t
template <typename T>
constexpr auto pow2_lt(T t)
  noexcept -> typename std::enable_if<std::is_integral<T>::value,T>::type {
    return T(1) << fls(make_unsigned<T>::type(t-1));
}

///////////////////////////
//Setting/Resetting bits
///////////////////////////

//Sets bit b of t, undefined if b >= number of bits in t
template <typename T>
constexpr auto set_bit(T t, int b)
  noexcept -> typename std::enable_if<std::is_unsigned<T>::value,T>::type {
  return t | (1 << b);
}

//Set all bits in t >= b
template <typename T>
constexpr auto set_bits_gt(T t, int b)
  noexcept -> typename std::enable_if<std::is_unsigned<T>::value,T>::type {
  return t | ~((1 << (b+1)) -1);
}

//Set all bits in t > b
template <typename T>
constexpr auto set_bits_ge(T t, int b)
  noexcept -> typename std::enable_if<std::is_unsigned<T>::value,T>::type {
  return t | ~((1 << b) -1);
}

//Set all bits in t <= b
template <typename T>
constexpr auto set_bits_le(T t, int b)
  noexcept -> typename std::enable_if<std::is_unsigned<T>::value,T>::type {
  return t | ((1 << (b+1)) -1);
}

//Set all bits in t < b
template <typename T>
constexpr auto set_bits_lt(T t, int b)
  noexcept -> typename std::enable_if<std::is_unsigned<T>::value,T>::type {
  return t | ((1 << b) -1);
}

//Resets bit b of t, undefined if b >= number of bits in t
template <typename T>
constexpr auto reset_bit(T t, int b)
  noexcept -> typename std::enable_if<std::is_unsigned<T>::value,T>::type {
  return t & ~(1 << b);
}

//Reset all bits in t >= b
template <typename T>
constexpr auto reset_bits_gt(T t, int b)
  noexcept -> typename std::enable_if<std::is_unsigned<T>::value,T>::type {
    return t & ~((T(1) << (b+1))-1);
  }

//Reset all bits in t > b
template <typename T>
constexpr auto reset_bits_ge(T t, int b)
  noexcept -> typename std::enable_if<std::is_unsigned<T>::value,T>::type {
    return t & ~((T(1) << b)-1);
  }

//Reset all bits in t <= b
template <typename T>
constexpr auto reset_bits_le(T t, int b)
  noexcept -> typename std::enable_if<std::is_unsigned<T>::value,T>::type {
    return t & ((T(1) << (b+1))-1);
  }

//Reset all bits in t < b
template <typename T>
constexpr auto reset_bits_lt(T t, int b)
  noexcept -> typename std::enable_if<std::is_unsigned<T>::value,T>::type {
    return t & ((T(1) << b)-1);
  }

//Flips bit b of t, undefined if b >= number of bits in t
template <typename T>
constexpr auto flip_bit(T t, int b)
  noexcept -> typename std::enable_if<std::is_unsigned<T>::value,T>::type {
  return t ^ ~(1 << b);
}

//Flips all bits in t >= b
template <typename T>
constexpr auto flip_bits_gt(T t, int b)
  noexcept -> typename std::enable_if<std::is_unsigned<T>::value,T>::type {
    return t ^ ~((T(1) << (b+1))-1);
  }

//Flips all bits in t > b
template <typename T>
constexpr auto flip_bits_ge(T t, int b)
  noexcept -> typename std::enable_if<std::is_unsigned<T>::value,T>::type {
    return t ^ ~((T(1) << b)-1);
  }

//Flips all bits in t <= b
template <typename T>
constexpr auto flip_bits_le(T t, int b)
  noexcept -> typename std::enable_if<std::is_unsigned<T>::value,T>::type {
    return t ^ ((T(1) << (b+1))-1);
  }

//Flips all bits in t < b
template <typename T>
constexpr auto flip_bits_lt(T t, int b)
  noexcept -> typename std::enable_if<std::is_unsigned<T>::value,T>::type {
    return t ^ ((T(1) << b)-1);
  }

/////////////////////
//MSB and LSB
/////////////////////

//Resets the least significant bit set, or returns 0 if t is 0.
template <typename T>
constexpr auto reset_lsb(T t)
  noexcept -> typename std::enable_if<std::is_unsigned<T>::value,T>::type {
  return t & (t -1);
}

//Resets the most significant bit set, or returns 0 if t is 0
template <typename T>
constexpr auto reset_msb(T t)
  noexcept -> typename std::enable_if<std::is_unsigned<T>::value,T>::type {
  return t & ~(T(1) << ((sizeof(t)*8) - clz(t)-1));
}

////////////////////////
//Saturated Arithmetic
////////////////////////

//Saturated addition, like normal addition except on overflow the result will be the maximum value for decltype(L + R).
template <typename L, typename R>
constexpr auto sat_add(L l, R r)
  noexcept -> typename std::enable_if<std::is_integral<L>::value && std::is_integral<R>::value,decltype(l+r)>::type {
  typedef decltype(l+r) LR;
  return static_cast<LR>(l) > numeric_limits<LR>::max() - static_cast<LR>(r) ?
    numeric_limits<LR>::max() : l + r;
}

//Saturated subtraction, like normal subtraction except on overflow the result will be the minimum value for decltype(L - R).
template <typename L, typename R>
constexpr auto sat_sub(L l, R r)
  noexcept -> typename std::enable_if<std::is_integral<L>::value && std::is_integral<R>::value,decltype(l-r)>::type {
  typedef decltype(l-r) LR;
  return static_cast<LR>(l) < numeric_limits<LR>::min() + static_cast<LR>(r) ?
    numeric_limits<LR>::min() : l - r;
}

////////////////////////
//Nibble Swap
////////////////////////


//Swaps the nibbles (4 bits) of the given byte. 
template <typename T> constexpr T swap_nibbles(T t) noexcept = delete;
template <> constexpr uint8_t swap_nibbles(uint8_t byte) {
  return (byte >> 4) | (byte << 4);
}

} //namespace std

#endif
