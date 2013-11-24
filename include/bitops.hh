#ifndef BITOPS_HH
#define BITOPS_HH

#ifndef HAS_CXX14_CONSTEXPR
#define HAS_CXX14_CONSTEXPR 0
#endif

#if HAS_CXX14_CONSTEXPR
#define constexpr14 constexpr
#else
#define constexpr14
#endif

#include <cstdint>
#include <cstddef>
#include <climits>
#include <limits>
#include <type_traits>
#include <algorithm>
#include <iostream>

namespace std {

//This implementation makes the following platform assumptions:
//* signed right shift is an arithmetic shift
//* CHAR_BIT == 8
//* Native integer types are exactly 8, 16, 32, and 64 bits wide. No support is added for larger types.
//* Signed numbers are implemented using 2's compliment
//
//The implementation is not designed to be efficient. The purpose is only to prove that each proposed function is implementable.
//A real implementation may use techniques such as SFINAE, static_assert, overloading, and/or = delete to limit the set of overloads.
//These have been omitted here to improve readability.

////////////////////////////////////
//Explicit shifts
////////////////////////////////////

//Logical left shift, undefined if s < 0 or x > sizeof(x) * CHAR_BIT
//Just about every processor in existance has this
//Included for symmetry
template <typename Integral>
  constexpr Integral shll(Integral x, int s) noexcept {
    return Integral(typename std::make_unsigned<Integral>::type(x) << s);
  }

//Logical right shift, undefined if s < 0 or x > sizeof(x) * CHAR_BIT
//Just about every processor in existance has this
//Included for symmetry, also can right shift a signed number easily without a cast to unsigned
template <typename Integral>
  constexpr Integral shlr(Integral x, int s) noexcept {
    return Integral(typename std::make_unsigned<Integral>::type(x) >> s);
  }

//Arithmetic left shift, undefined if s < 0 or x > sizeof(x) * CHAR_BIT
//Just about every processor in existance has this
//Included for symmetry
template <typename Integral>
  constexpr Integral shal(Integral x, int s) noexcept {
    return shll(x, s);
  }

//Arithmetic right shift, undefined if s < 0 or x > sizeof(x) * CHAR_BIT
//Just about every processor in existance has this, signed right shift is implementation defined. There is no standards compliant alternative to shar().
template <typename Integral>
  constexpr Integral shar(Integral x, int s) noexcept {
    //Assumes signed right shift is arithmetic. If it is not the platform will need to implement this another way.
    return Integral(typename std::make_signed<Integral>::type(x) >> s);
  }

//Circular left shift (rotate), undefined if s < 0 or x > sizeof(x) * CHAR_BIT
//Just about every processor in existance has this, including the PDP-11 (1969) and yet C or C++ never included a way to get at this instruction.
template <typename Integral>
  constexpr Integral rotl(Integral x, int s) noexcept {
    return (x << s) | shlr(x, (sizeof(x)*CHAR_BIT-s));
  }

//Circular right shift (rotate), undefined if s < 0 or x > sizeof(x) * CHAR_BIT
//Just about every processor in existance has this, including the PDP-11 (1969) and yet C or C++ never included a way to get at this instruction.
template <typename Integral>
  constexpr Integral rotr(Integral x, int s) noexcept {
    return shlr(x, s) | ( x << (sizeof(x)*CHAR_BIT-s));
  }

////////////////////////////////////
//Zero and One Counting algorithms
////////////////////////////////////

//Returns the number of trailing zeros in x, or sizeof(x) * CHAR_BIT if x is 0
//i386 bsf, cmov
//x86_64 w/ BMI1: tzcnt
//Alpha: cttz
//MIPS: CLZ
//gcc: x == 0 ? sizeof(x) * CHAR_BIT :__builtin_ctz(x)
//Applications: SSE2 strlen, Howard Hinnant's gcd example
template <typename Integral>
  constexpr14 int cntt0(Integral x) noexcept {
    constexpr int nbits = int(sizeof(x) * CHAR_BIT);
    if(x == 0) { return nbits; }
    Integral n = 0;
    if(sizeof(x) > 1) {
      if(sizeof(x) > 2) {
        if(sizeof(x) > 4) {
          if((x & Integral(0xFFFFFFFFUL)) == 0) { n = n + 32; x = shlr(x, 32); }
        }
        if((x & Integral(0xFFFFUL)) == 0) { n = n + 16; x = shlr(x, 16); }
      }
      if((x & Integral(0xFFUL)) == 0) { n = n + 8; x = shlr(x, 8); }
    }
    if((x & Integral(0xFUL)) == 0) { n = n + 4; x = shlr(x, 4); }
    if((x & Integral(0x3UL)) == 0) { n = n + 2; x = shlr(x, 2); }
    return n - (x & 1);
  }

//Returns the number of leading zeroes in x, or sizeof(x) * CHAR_BIT if x is 0
//i386 bsr, cmov
//x86_64 w/ SSE4: lzcnt 
//ARMv5: CLZ
//IA64: clz
//Alpha: CTLZ
//PowerPC: cntlz[dw]
//gcc: x == 0 ? sizeof(x) * CHAR_BIT :__builtin_clz(x)
template <typename Integral>
  constexpr14 int cntl0(Integral x) noexcept {
    constexpr int nbits = int(sizeof(x) * CHAR_BIT);
    if(x == 0) { return nbits; }
    Integral n = 1;
    if(sizeof(x) > 1) {
      if(sizeof(x) > 2) {
        if(sizeof(x) > 4) {
          if((shlr(x, nbits-32)) == 0) { n = n + 32; x = x << 32; }
        }
        if((shlr(x, nbits-16)) == 0) { n = n + 16; x = x << 16; }
      }
      if((shlr(x, nbits-8)) == 0) { n = n + 8; x = x << 8; }
    }
    if((shlr(x, nbits-4)) == 0) { n = n + 4; x = x << 4; }
    if((shlr(x, nbits-2)) == 0) { n = n + 2; x = x << 2; }
    n = n - (shlr(x, 31));
    return n;
  }

//Returns the number of leading 1 bits in x.
//ARMv8: CLS
//Blackfin: SIGNBITS
//C6X: NORM
//Picochip: SBC
//MIPS: CTO
//gcc: __builtin_clrsb(x)
template <typename Integral>
  constexpr14 int cntl1(Integral x) noexcept {
    return cntl0(~x);
  }

//Returns the number of trailing 1 bits in x.
template <typename Integral>
  constexpr14 int cntt1(Integral x) noexcept {
    return cntt0(~x);
  }

//Returns the number of 1 bits in x.
//x86_64 SSE4: popcnt
//IA64: popcnt
//Alpha: CTPOP
//PowerPC: popcntb
//SparcV9: POPC
//Blackfin: ONES
//gcc: __builtin_popcount(x)
template <typename Integral>
  constexpr14 int popcount(Integral x) noexcept {
    x = (x & Integral(0x5555555555555555UL)) + (shlr(x, 1) & Integral(0x5555555555555555UL));
    x = (x & Integral(0x3333333333333333UL)) + (shlr(x, 2) & Integral(0x3333333333333333UL));
    x = (x & Integral(0x0F0F0F0F0F0F0F0FUL)) + (shlr(x, 4) & Integral(0x0F0F0F0F0F0F0F0FUL));
    if(sizeof(x) > 1) {
      x = (x & Integral(0x00FF00FF00FF00FFUL)) + (shlr(x, 8) & Integral(0x00FF00FF00FF00FFUL));
      if(sizeof(x) > 2) {
        x = (x & Integral(0x0000FFFF0000FFFFUL)) + (shlr(x, 16) & Integral(0x0000FFFF0000FFFFUL));
        if(sizeof(x) > 4) {
          x = (x & Integral(0x00000000FFFFFFFFUL)) + (shlr(x, 32) & Integral(0x00000000FFFFFFFFUL));
        }
      }
    }
    return x;
  }

//Returns the number of 1 bits in x mod 2
//gcc: __builtin_parity(x)
template <typename Integral>
  constexpr14 int parity(Integral x) noexcept {
    x = x ^ shlr(x, 1);
    x = x ^ shlr(x, 2);
    x = x ^ shlr(x, 4);
    if(sizeof(x) > 1) {
      x = x ^ shlr(x, 8);
      if(sizeof(x) > 2) {
        x = x ^ shlr(x, 16);
        if(sizeof(x) > 4) {
          x = x ^ shlr(x, 32);
        }
      }
    }
    return x;
  }

////////////////////////////////////
//Rightmost bit manipulation
////////////////////////////////////

//Reset least siginificant 1 bit
//Resets the least siginificant 1 bit of x. Returns 0 if x is 0.
//x86_64 BMI1: BLSR
template <typename Integral>
  constexpr Integral rstls1b(Integral x) {
    return x & (x-1);
  }

//Set the least significant 0 bit
//x86_64 AMD TBM: BLCS
template <typename Integral>
  constexpr Integral setls0b(Integral x) {
    return x | (x + 1);
  }

//Isolate least siginificant 1 bit
//Isolates the least significant 1 bit of x and returns it. Returns 0 if x is 0.
//x86_64 BMI1: BLSI
//x86_64 AMD TBM: BLSIC, NOT
template <typename Integral>
  constexpr Integral isols1b(Integral x) {
    return x & -x;
  }

//Set the least significant zero bit to 1 and all of the rest to 0.
template <typename Integral>
  constexpr Integral isols0b(Integral x) {
    return (~x) & (x + 1);
  }

//Reset the trailing 1's in x
//x86_64 AMD TBM: BLCFILL
template <typename Integral>
  constexpr Integral rstt1(Integral x) {
    return x & (x + 1);
  }

//Set all of the trailing 0's in x
//x86_64 AMD TBM: BLSFILL
template <typename Integral>
  constexpr Integral sett0(Integral x) {
    return x | (x - 1);
  }

//Returns a mask with all of the trailing 0's set.
template <typename Integral>
  constexpr Integral maskt0(Integral x) {
    return (~x) & (x-1);
  }

//Returns a mask with all of the trailing 1's set.
template <typename Integral>
  constexpr Integral maskt1(Integral x) {
    return ~((~x) | (x + 1));
  }

//Returns a mask with all of the trailing 0's  and the least significant 1 bit set.
//x86_64 BMI1: BLSMSK
//x86_64 AMD TBM: TZMSK
template <typename Integral>
  constexpr Integral maskt0ls1b(Integral x) {
    return (x-1) ^ x;
  }

//Returns a mask with all of the trailing 1's and the least significant 0 bit set.
template <typename Integral>
  constexpr Integral maskt1ls0b(Integral x) {
    return x ^ (x + 1);
  }

////////////////////////////////////
//Bit and Byte reversal algorithms
////////////////////////////////////

//Reverse each group of blocks of bits in x.
//
//bits_per_block == 1: reverses the bits of x 
//ARMv7: RBIT
//EPIPHANY: BITR
//bits_per_block == 2: reverses each pair of bits in x
//bits_per_block == 4: reverses the nibbles in x
//AVR: SWAP
//bits_per_block == 8: reverses the bytes in x (assuming CHAR_BIT == 8). This is the traditional byte swap.
//i386: bswap
//ARMv5: REV
//PDP-11: SWAB
//gcc: __builtin_bswap[16|32|64](x)
//(blocks_per_group == 2) ARMv6: REV16
//(blocks_per_group == 4) ARMv8: REV32
//bits_per_block == 16,32,etc.. reverses the words in x.
//(bits_per_block == 16) MC68020: SWAP
template <typename Integral>
  constexpr14 auto revbits(Integral x,
      int bits_per_block = 1,
      int blocks_per_group = sizeof(Integral) * CHAR_BIT)
  noexcept -> typename std::enable_if<std::is_unsigned<Integral>::value, Integral>::type {
    int group_sz = std::min(bits_per_block * blocks_per_group, int(sizeof(Integral) * CHAR_BIT));
    int k = group_sz - bits_per_block;
    cout << "K IS " << k << std::endl;
    if(k & 1) x = shll(x & Integral(0x5555555555555555UL), 1) | shlr(x & Integral(0xAAAAAAAAAAAAAAAAUL), 1);
    if(k & 2) x = shll(x & Integral(0x3333333333333333UL), 2) | shlr(x & Integral(0xCCCCCCCCCCCCCCCCUL), 2);
    if(k & 4) x = shll(x & Integral(0x0F0F0F0F0F0F0F0FUL), 4) | shlr(x & Integral(0xF0F0F0F0F0F0F0F0UL), 4);
    //sizeof comparisons added to help compiler remove these checks for small integers
    if(sizeof(x) > 1 && k & 8) x = shll(x & Integral(0x00FF00FF00FF00FFUL), 8) | shlr(x & Integral(0xFF00FF00FF00FF00UL), 8);
    if(sizeof(x) > 2 && k & 16) x = shll(x & Integral(0x0000FFFF0000FFFFUL), 16) | shlr(x & Integral(0xFFFF0000FFFF0000UL), 16);
    if(sizeof(x) > 4 && k & 32) x = shll(x & Integral(0x00000000FFFFFFFFUL), 32) | shlr(x & Integral(0xFFFFFFFF00000000UL), 32);
    return x;
  }

//Signed version calls unsigned to avoid sign extension issues
template <typename Integral>
  constexpr14 auto revbits(Integral x,
      int bits_per_block = 1,
      int blocks_per_group = sizeof(Integral) * CHAR_BIT)
  noexcept -> typename std::enable_if<std::is_signed<Integral>::value, Integral>::type {
    return Integral(revbits(typename std::make_unsigned<Integral>::type(x), bits_per_block, blocks_per_group));
  }


//Byte reversal, simple wrapper around revbits
template <typename Integral>
  constexpr14 Integral revbytes(Integral x,
      int bytes_per_block=1,
      int blocks_per_group = sizeof(Integral)) noexcept {
    return revbits(x, CHAR_BIT * bytes_per_block, blocks_per_group);
  }

////////////////////////////////////
//Single bit manipulation
////////////////////////////////////

//Sets bit b in x, undefined behavior if b < 0 or b >= sizeof(x) * CHAR_BIT
template <typename Integral>
  constexpr Integral setbit(Integral x, int b) noexcept {
    return x | (Integral(1) << b);
  }

//Resets bit b in x, undefined behavior if b < 0 or b >= sizeof(x) * CHAR_BIT
template <typename Integral>
  constexpr Integral rstbit(Integral x, int b) noexcept {
    return x & ~(Integral(1) << b);
  }

//Flips bit b in x, undefined behavior if b < 0 or b >= sizeof(x) * CHAR_BIT
template <typename Integral>
  constexpr Integral flipbit(Integral x, int b) noexcept {
    return x ^ (Integral(1) << b);
  }

//Return whether or not bit b is set in x, undefined behavior if b < 0 or b >= sizeof(x) * CHAR_BIT
template <typename Integral>
  constexpr bool testbit(Integral x, int b) noexcept {
    return x & (Integral(1) << b);
  }

////////////////////////////////////
//Range of bits manipulation
////////////////////////////////////

//Resets all bits >= position b, nop if b > sizeof(x) * CHAR_BIT
//x86_64 w/ BMI2: BZHI
template <typename Integral>
  constexpr Integral rstbitsge(Integral x, int b) noexcept {
    return x & ((Integral(1) << b)-1);
  }

//Resets all bits < position b, nop if b > sizeof(x) * CHAR_BIT
template <typename Integral>
  constexpr Integral rstbitsle(Integral x, int b) noexcept {
    return x & ~((Integral(1) << (b+1))-1);
  }

//Set all bits >= position b, nop if b > sizeof(x) * CHAR_BIT
template <typename Integral>
  constexpr Integral setbitsge(Integral x, int b) noexcept {
    return x | ~((Integral(1) << b)-1);
  }

//Sets all bits < position b, nop if b > sizeof(x) * CHAR_BIT
template <typename Integral>
  constexpr Integral setbitsle(Integral x, int b) noexcept {
    return x | ((Integral(1) << (b+1))-1);
  }

//Flip all bits >= position b, nop if b > sizeof(x) * CHAR_BIT
template <typename Integral>
  constexpr Integral flipbitsge(Integral x, int b) noexcept {
    return x ^ ~((Integral(1) << b)-1);
  }

//Flip all bits < position b, nop if b > sizeof(x) * CHAR_BIT
template <typename Integral>
  constexpr Integral flipbitsle(Integral x, int b) noexcept {
    return x ^ ((Integral(1) << (b+1))-1);
  }

////////////////////////////////////
//Power of 2 manipulation
////////////////////////////////////

//Returns true if x is a power of 2
//Application: Integral template arguments, add optimizations for power of 2
template <typename Integral>
  constexpr bool ispow2(Integral x) noexcept {
    return x > 0 && (x & (x-1)) == 0;
    //return popcount(x) == 1;
  }

//Round up to the next power of 2
//Application: Growable containers whose size must be a power of 2
//Application: Extending a 2d image size to a power of 2 for 3d graphics libraries (OpenGL/DirectX)
template <typename Integral>
constexpr14 Integral ceilp2(Integral x) noexcept {
  x = x-1;
  x |= shlr(x, 1);
  x |= shlr(x, 2);
  x |= shlr(x, 4);
  if(sizeof(x) > 1) {
    x |= shlr(x, 8);
    if(sizeof(x) > 2) {
      x |= shlr(x, 16);
      if(sizeof(x) > 4) {
        x |= shlr(x, 32);
      }
    }
  }
  return x + 1;
}

//Round down to the previous power of 2
//Application: See ceilp2
template <typename Integral>
constexpr14 Integral floorp2(Integral x) noexcept {
  x |= shlr(x, 1);
  x |= shlr(x, 2);
  x |= shlr(x, 4);
  if(sizeof(x) > 1) {
    x |= shlr(x, 8);
    if(sizeof(x) > 2) {
      x |= shlr(x, 16);
      if(sizeof(x) > 4) {
        x |= shlr(x, 32);
      }
    }
  }
  return x - shlr(x, 1);
}

////////////////////////////////////
//Saturated Arithmetic
////////////////////////////////////

//Perform saturated addition on l and r.
//ARMv7 DSP extensions: QADD
template <typename IntegralL, typename IntegralR>
  constexpr auto satadd(IntegralL l, IntegralR r) noexcept -> decltype(l + r) {
    typedef decltype(l + r) LR;
    return LR(l) > std::numeric_limits<LR>::max() - LR(r) ? std::numeric_limits<LR>::max() : l + r;
  }

//Perform saturated subtraction on l and r.
//ARMv7 DSP extensions: QSUB
template <typename IntegralL, typename IntegralR>
  constexpr auto satsub(IntegralL l, IntegralR r) noexcept -> decltype(l - r) {
    typedef decltype(l + r) LR;
    return LR(l) < std::numeric_limits<LR>::min() + LR(r) ? std::numeric_limits<LR>::min() : l - r;
  }



////////////////////////////////////
//Pointer and size alignment helpers
////////////////////////////////////

//Returns true if t is aligned to a
template <typename Integral>
constexpr bool is_aligned(Integral t, size_t a) noexcept {
  return ((t & (a-1)) == 0);
}
bool is_aligned(void* t, size_t a) noexcept {
  return is_aligned(uintptr_t(t), a);
}

//Returns the smallest number n when n >= val && is_aligned(n, align). align must be a power of 2!
template <typename Integral>
constexpr Integral align_up(Integral val, size_t a) noexcept {
  return ((val + (a -1)) & -a);
}
void* align_up(void* val, size_t a) noexcept {
  return (void*)align_up(uintptr_t(val), a);
}

//Returns the largest number n when n <= val && is_aligned(n, align). align must be a power of 2!
template <typename Integral>
constexpr Integral align_down(Integral val, size_t a) noexcept {
  return val & -a;
}
void* align_down(void* val, size_t a) noexcept {
  return (void*)align_down(uintptr_t(val), a);
}

///////////////////////////////////
//Bit Shuffling
///////////////////////////////////

//Outer Perfect Shuffle
template <typename Integral>
constexpr14 Integral outer_pshuffle(Integral x) noexcept {
  Integral t = 0;
  if(sizeof(x) > 4) {
    t = (x ^ shlr(x, 16)) & Integral(0x00000000FFFF0000UL);
    x = x ^ t ^ shll(t, 16);
  }
  if(sizeof(x) > 2) {
    t = (x ^ shlr(x, 8)) & Integral(0x0000FF000000FF00UL);
    x = x ^ t ^ shll(t, 8);
  }
  if(sizeof(x) > 1) {
    t = (x ^ shlr(x, 4)) & Integral(0x00F000F000F000F0UL);
    x = x ^ t ^ shll(t, 4);
  }
  t = (x ^ shlr(x, 2)) & Integral(0x0C0C0C0C0C0C0C0CUL);
  x = x ^ t ^ shll(t, 2);
  t = (x ^ shlr(x, 1)) & Integral(0x2222222222222222UL);
  x = x ^ t ^ shll(t, 1);
  return x;
}

template <typename Integral>
constexpr14 Integral outer_punshuffle(Integral x) noexcept {
  Integral t = 0;
  t = (x ^ shlr(x, 1)) & Integral(0x2222222222222222UL);
  x = x ^ t ^ shll(t, 1);
  t = (x ^ shlr(x, 2)) & Integral(0x0C0C0C0C0C0C0C0CUL);
  x = x ^ t ^ shll(t, 2);
  if(sizeof(x) > 1) {
    t = (x ^ shlr(x, 4)) & Integral(0x00F000F000F000F0UL);
    x = x ^ t ^ shll(t, 4);
  }
  if(sizeof(x) > 2) {
    t = (x ^ shlr(x, 8)) & Integral(0x0000FF000000FF00UL);
    x = x ^ t ^ shll(t, 8);
  }
  if(sizeof(x) > 4) {
    t = (x ^ shlr(x, 16)) & Integral(0x00000000FFFF0000UL);
    x = x ^ t ^ shll(t, 16);
  }
  return x;
}

template <typename Integral>
constexpr14 Integral inner_pshuffle(Integral x) noexcept {
  return outer_pshuffle(revbits(x, sizeof(x)*CHAR_BIT/2, 2));
}

template <typename Integral>
constexpr14 Integral inner_punshuffle(Integral x) noexcept {
  return revbits(outer_punshuffle(x), sizeof(x)*CHAR_BIT/2, 2);
}

///////////////////////////////////
//Bits Deposit and Extract
///////////////////////////////////

//Parallel Bits Deposit
//x    HGFEDCBA
//mask 01100100
//res  0CB00A00
//x86_64 BMI2: PDEP
template <typename Integral>
constexpr14 Integral deposit_bits(Integral x, Integral mask) {
  Integral res = 0;
  for(Integral bb = 1; mask != 0; bb += bb) {
    if(x & bb) {
      res |= mask & (-mask);
    }
    mask &= (mask - 1);
  }
  return res;
}

//Parallel Bits Extract
//x    HGFEDCBA
//mask 01100100
//res  00000GFC
//x86_64 BMI2: PEXT
template <typename Integral>
constexpr14 Integral extract_bits(Integral x, Integral mask) {
  Integral res = 0;
  for(Integral bb = 1; mask != 0; bb += bb) {
    if(x & mask & -mask) {
      res |= bb;
    }
    mask &= (mask - 1);
  }
  return res;
}

} //namespace std

#endif
