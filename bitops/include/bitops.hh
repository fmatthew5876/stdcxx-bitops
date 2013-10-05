#ifndef BITOPS_HH
#define BITOPS_HH

namespace std {

//Return smallest power of 2 >= t
template <typename T>
  constexpr T pow2_ge(T t);

//Return smallest power of 2 > t
template <typename T>
  constexpr T pow2_gt(T t);

//Return smallest power of 2 <= t
template <typename T>
  constexpr T pow2_le(T t);

//Return smallest power of 2 < t
template <typename T>
  constexpr T pow2_lt(T t);

//Returns true if t == 0 or t is a power of 2
template <typename T>
  constexpr T is_pow2_or_zero(T t) {
    return t & (t-1);
  }

//Returns true if t is a power of 2
template <typename T>
  constexpr T is_pow2(T t) {
    return (t != 0) && is_pow2_or_zero(t);
  }

//Returns the smallest number n when n >= val && is_aligned(n, align). align must be a power of 2!
template <typename T, typename A>
  constexpr T align_up(T val, A align) {
    return ((val + (align -1)) & -align);
  }

//Returns the largest number n when n <= val && is_aligned(n, align). align must be a power of 2!
template <typename T, typename A>
  constexpr T align_down(T val, A align) {
    return val & -align;
  }

//Returns true if t is aligned to a
template <typename T, typename A>
  constexpr bool is_aligned(T t, A a) {
    return ((t & (a-1)) == 0);
  }

//Reverses all of the bits in t
template <typename T>
  constexpr T reverse_bits(T t);

//Count trailing zeroes
template <typename T>
  constexpr T ctz(T t) {
    return __builtin_ctz(t);
  }

//Count leading zeroes
template <typename T>
  constexpr T clz(T t) {
    return __builtin_clz(t);
  }

//Returns one plus the index of the least significant 1-bit of x, or if x is zero, returns zero.
template <typename T>
  constexpr T ffs(T t) {
    return __builtin_ffs(t);
  }

//Returns the number of leading redundant sign bits in x, i.e. the number of bits following the most significant bit that are identical to it. There are no special cases for 0 or other values
template <typename T>
  constexpr T clrsb(T t) {
    return __builtin_clrsb(t);
  }

//Returns the number of 1-bits in x.
template <typename T>
  constexpr T popcount(T t) {
    return __builtin_popcount(t);
  }

//Returns the parity of x, i.e. the number of 1-bits in x modulo 2.
template <typename T>
  constexpr T parity(T t) {
    return __builtin_parity(t);
  }

//Compute the log base 2 of t.
template <typename T>
  constexpr T lg(T t);

//left shift logical
template <typename T, typename S>
  constexpr T lshl(T t, S shift);

//right shift logical
template <typename T, typename S>
  constexpr T rshl(T t, S shift);

//left shift arithmetic
template <typename T, typename S>
  constexpr T lsha(T t, S shift);

//right shift arithmetic
template <typename T, typename S>
  constexpr T rsha(T t, S shift);

//left shift circular
template <typename T, typename S>
  constexpr T lshc(T t, S shift);

//right shift circular
template <typename T, typename S>
  constexpr T rshc(T t, S shift);

//Return most significant bit set in t
template <typename T>
  constexpr T msb(T t);

//Return least significant bit set in t
template <typename T>
  constexpr T lsb(T t);

//Sets bit b of t, no effect if b >= number of bits in t
template <typename T, typename B>
  constexpr T set_bit(T t, B b) {
    return t | (1 << b);
  }

//Set all bits in t >= b
template <typename T, typename B>
  constexpr T set_bits_gt(T t, B b);

//Set all bits in t > b
template <typename T, typename B>
  constexpr T set_bits_ge(T t, B b);

//Set all bits in t <= b
template <typename T, typename B>
  constexpr T set_bits_le(T t, B b);

//Set all bits in t < b
template <typename T, typename B>
  constexpr T set_bits_lt(T t, B b);

//Resets bit b of t, no effect if b >= number of bits in t
template <typename T, typename B>
  constexpr T reset_bit(T t, B b) {
    return t & ~(1 << b);
  }

//Reset all bits in t >= b
template <typename T, typename B>
  constexpr T reset_bits_gt(T t, B b);

//Reset all bits in t > b
template <typename T, typename B>
  constexpr T reset_bits_ge(T t, B b);

//Reset all bits in t <= b
template <typename T, typename B>
  constexpr T reset_bits_le(T t, B b);

//Reset all bits in t < b
template <typename T, typename B>
  constexpr T reset_bits_lt(T t, B b);

//Resets the least significant bit set
template <typename T>
  constexpr T reset_lsb(T t);

//Resets the most significant bit set
template <typename T>
  constexpr T reset_msb(T t);

//Returns a value whos even bits are set to the even bits of even, and odd bits set to the odd bits of odd.
template <typename T>
  constexpr T interleave_bits(T even, T odd);


} //namespace std

#endif
