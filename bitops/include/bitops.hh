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
  constexpr Integral lshl(Integral x, int s) noexcept {
    return Integral(typename std::make_unsigned<Integral>::type(x) << s);
  }

//Logical right shift, undefined if s < 0 or x > sizeof(x) * CHAR_BIT
//Just about every processor in existance has this
//Included for symmetry, also can right shift a signed number easily without a cast to unsigned
template <typename Integral>
  constexpr Integral rshl(Integral x, int s) noexcept {
    return Integral(typename std::make_unsigned<Integral>::type(x) << s);
  }

//Arithmetic left shift, undefined if s < 0 or x > sizeof(x) * CHAR_BIT
//Just about every processor in existance has this
//Included for symmetry
template <typename Integral>
  constexpr Integral lsha(Integral x, int s) noexcept {
    return lshl(x, s);
  }

//Arithmetic right shift, undefined if s < 0 or x > sizeof(x) * CHAR_BIT
//Just about every processor in existance has this, signed right shift is implementation defined. There is no standards compliant alternative to shar().
template <typename Integral>
  constexpr Integral rsha(Integral x, int s) noexcept {
    //Assumes signed right shift is arithmetic. If it is not the platform will need to implement this another way.
    return Integral(typename std::make_signed<Integral>::type(x) >> s);
  }

//Circular left shift (rotate), undefined if s < 0 or x > sizeof(x) * CHAR_BIT
//Just about every processor in existance has this, including the PDP-11 (1969) and yet C or C++ never included a way to get at this instruction.
template <typename Integral>
  constexpr Integral lrot(Integral x, int s) noexcept {
    return (x << s) | rshl(x, (sizeof(x)*CHAR_BIT-s));
  }

//Circular right shift (rotate), undefined if s < 0 or x > sizeof(x) * CHAR_BIT
//Just about every processor in existance has this, including the PDP-11 (1969) and yet C or C++ never included a way to get at this instruction.
template <typename Integral>
  constexpr Integral rrot(Integral x, int s) noexcept {
    return rshl(x, s) | ( x << (sizeof(x)*CHAR_BIT-s));
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
  constexpr14 int count_tzb(Integral x) noexcept {
    constexpr int nbits = int(sizeof(x) * CHAR_BIT);
    if(x == 0) { return nbits; }
    Integral n = 0;
    if(sizeof(x) > 1) {
      if(sizeof(x) > 2) {
        if(sizeof(x) > 4) {
          if((x & Integral(0xFFFFFFFFUL)) == 0) { n = n + 32; x = rshl(x, 32); }
        }
        if((x & Integral(0xFFFFUL)) == 0) { n = n + 16; x = rshl(x, 16); }
      }
      if((x & Integral(0xFFUL)) == 0) { n = n + 8; x = rshl(x, 8); }
    }
    if((x & Integral(0xFUL)) == 0) { n = n + 4; x = rshl(x, 4); }
    if((x & Integral(0x3UL)) == 0) { n = n + 2; x = rshl(x, 2); }
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
  constexpr14 int count_lzb(Integral x) noexcept {
    constexpr int nbits = int(sizeof(x) * CHAR_BIT);
    if(x == 0) { return nbits; }
    Integral n = 1;
    if(sizeof(x) > 1) {
      if(sizeof(x) > 2) {
        if(sizeof(x) > 4) {
          if((rshl(x, nbits-32)) == 0) { n = n + 32; x = x << 32; }
        }
        if((rshl(x, nbits-16)) == 0) { n = n + 16; x = x << 16; }
      }
      if((rshl(x, nbits-8)) == 0) { n = n + 8; x = x << 8; }
    }
    if((rshl(x, nbits-4)) == 0) { n = n + 4; x = x << 4; }
    if((rshl(x, nbits-2)) == 0) { n = n + 2; x = x << 2; }
    n = n - (rshl(x, 31));
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
  constexpr14 int count_l1b(Integral x) noexcept {
    return count_lzb(~x);
  }

//Returns the number of trailing 1 bits in x.
template <typename Integral>
  constexpr14 int count_t1b(Integral x) noexcept {
    return count_tzb(~x);
  }

//Returns the number of 1 bits in x.
//i386: popcnt
//IA64: popcnt
//Alpha: CTPOP
//PowerPC: popcntb
//SparcV9: POPC
//Blackfin: ONES
//gcc: __builtin_popcount(x)
template <typename Integral>
  constexpr14 int popcount(Integral x) noexcept {
    x = (x & Integral(0x5555555555555555UL)) + (rshl(x, 1) & Integral(0x5555555555555555UL));
    x = (x & Integral(0x3333333333333333UL)) + (rshl(x, 2) & Integral(0x3333333333333333UL));
    x = (x & Integral(0x0F0F0F0F0F0F0F0FUL)) + (rshl(x, 4) & Integral(0x0F0F0F0F0F0F0F0FUL));
    if(sizeof(x) > 1) {
      x = (x & Integral(0x00FF00FF00FF00FFUL)) + (rshl(x, 8) & Integral(0x00FF00FF00FF00FFUL));
      if(sizeof(x) > 2) {
        x = (x & Integral(0x0000FFFF0000FFFFUL)) + (rshl(x, 16) & Integral(0x0000FFFF0000FFFFUL));
        if(sizeof(x) > 4) {
          x = (x & Integral(0x00000000FFFFFFFFUL)) + (rshl(x, 32) & Integral(0x00000000FFFFFFFFUL));
        }
      }
    }
    return x;
  }

//Returns the number of 1 bits in x mod 2
//gcc: __builtin_parity(x)
template <typename Integral>
  constexpr14 int parity(Integral x) noexcept {
    x = x ^ rshl(x, 1);
    x = x ^ rshl(x, 2);
    x = x ^ rshl(x, 4);
    if(sizeof(x) > 1) {
      x = x ^ rshl(x, 8);
      if(sizeof(x) > 2) {
        x = x ^ rshl(x, 16);
        if(sizeof(x) > 4) {
          x = x ^ rshl(x, 32);
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
  constexpr Integral reset_ls1b(Integral x) {
    return x & (x-1);
  }

//Isolate least siginificant 1 bit
//Isolates the least significant 1 bit of x and returns it. Returns 0 if x is 0.
//x86_64 BMI1: BLSI
//x86_64 AMD TBM: BLSIC, NOT
template <typename Integral>
  constexpr Integral isolate_ls1b(Integral x) {
    return x & -x;
  }

//Set the least significant 0 bit
//x86_64 AMD TBM: BLCS
template <typename Integral>
  constexpr Integral set_lszb(Integral x) {
    return x | (x + 1);
  }

//Set the least significant zero bit to 1 and all of the rest to 1.
template <typename Integral>
  constexpr Integral isolate_lszb(Integral x) {
    return (~x) & (x + 1);
  }

//Set the least significant 1 bit to 0 and all of the rest to 0.
template <typename Integral>
  constexpr Integral inv_isolate_ls1b(Integral x) {
    return (~x) | (x -1);
  }

//Reset the trailing 1's in x
//x86_64 AMD TBM: BLCFILL
template <typename Integral>
  constexpr Integral reset_t1b(Integral x) {
    return x & (x + 1);
  }

//Set all of the trailing 0's in x
//x86_64 AMD TBM: BLSFILL
template <typename Integral>
  constexpr Integral set_tzb(Integral x) {
    return x | (x - 1);
  }

//Returns a mask with all of the trailing 0's set.
template <typename Integral>
  constexpr Integral mask_tzb(Integral x) {
    return (~x) & (x-1);
  }

//Returns a mask with all of the trailing 0's  and the least significant 1 bit set.
//x86_64 BMI1: BLSMSK
//x86_64 AMD TBM: TZMSK
template <typename Integral>
  constexpr Integral mask_tzb_ls1b(Integral x) {
    return (x-1) ^ x;
  }

//Returns a mask with all of the trailing 1's reset and the other bits set.
template <typename Integral>
  constexpr Integral inv_mask_t1b(Integral x) {
    return (~x) | (x + 1);
  }

//Returns a mask with all of the trailing 1's and the least significant 0 bit set.
template <typename Integral>
  constexpr Integral mask_t1b_lszb(Integral x) {
    return x ^ (x + 1);
  }

//Resets the rightmost string of 1 bits in x.
template <typename Integral>
  constexpr Integral reset_rstr1b(Integral x) {
    return (((x | (x-1)) + 1) & x);
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
  constexpr14 Integral revbits(Integral x,
      int bits_per_block = 1,
      int blocks_per_group = INT_MAX) noexcept
  {
    if(bits_per_block >= int(sizeof(x) * CHAR_BIT)) { return x; }
    constexpr size_t nbits = sizeof(Integral) * CHAR_BIT;
    int nbits_per_group = nbits / blocks_per_group;

    //Create a mask for the first block of bits in each group
    Integral lmask = mask_tzb(1 << bits_per_block);
    for(int i = 1; i < blocks_per_group; ++i) {
      lmask <<= nbits_per_group;
    }
    //Another mask for the last block of bits in each group
    Integral rmask = lmask << (nbits_per_group - bits_per_block);

    Integral r = 0;
    for(int boff = 0; boff < nbits_per_group/2; boff += bits_per_block) {
      //Move the rightmost block left
      r |= (lmask << (nbits_per_group-boff)) & (x << (nbits_per_group-boff));
      //Move the leftmost block right
      r |= rshl(rmask, boff) & rshl(x, boff);
    }
    return r;
  }

//Byte reversal, simple wrapper around revbits
template <typename Integral>
  constexpr14 Integral revbytes(Integral x,
      int bytes_per_block=1,
      int blocks_per_group = INT_MAX) noexcept {
    return revbits(x, CHAR_BIT * bytes_per_block, blocks_per_group);
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
//Single bit manipulation
////////////////////////////////////


//Sets bit b in x, undefined behavior if b < 0 or b >= sizeof(x) * CHAR_BIT
template <typename Integral>
  constexpr Integral set_bit(Integral x, int b) noexcept {
    return x | (Integral(1) << b);
  }

//Resets bit b in x, undefined behavior if b < 0 or b >= sizeof(x) * CHAR_BIT
template <typename Integral>
  constexpr Integral reset_bit(Integral x, int b) noexcept {
    return x & ~(Integral(1) << b);
  }

//Flips bit b in x, undefined behavior if b < 0 or b >= sizeof(x) * CHAR_BIT
template <typename Integral>
  constexpr Integral flip_bit(Integral x, int b) noexcept {
    return x ^ (Integral(1) << b);
  }

//Return whether or not bit b is set in x, undefined behavior if b < 0 or b >= sizeof(x) * CHAR_BIT
template <typename Integral>
  constexpr bool test_bit(Integral x, int b) noexcept {
    return x & (Integral(1) << b);
  }

////////////////////////////////////
//Range of bits manipulation
////////////////////////////////////

//Resets all bits >= position b, nop if b > sizeof(x) * CHAR_BIT
//x86_64 w/ BMI2: BZHI
template <typename Integral>
  constexpr Integral reset_bitsge(Integral x, int b) noexcept {
    return x & ((Integral(1) << b)-1);
  }

//Resets all bits < position b, nop if b > sizeof(x) * CHAR_BIT
template <typename Integral>
  constexpr Integral reset_bitsle(Integral x, int b) noexcept {
    return x & ~((Integral(1) << (b+1))-1);
  }

//Set all bits >= position b, nop if b > sizeof(x) * CHAR_BIT
template <typename Integral>
  constexpr Integral set_bitsge(Integral x, int b) noexcept {
    return x | ~((Integral(1) << b)-1);
  }

//Sets all bits < position b, nop if b > sizeof(x) * CHAR_BIT
template <typename Integral>
  constexpr Integral set_bitsle(Integral x, int b) noexcept {
    return x | ((Integral(1) << (b+1))-1);
  }

//Flip all bits >= position b, nop if b > sizeof(x) * CHAR_BIT
template <typename Integral>
  constexpr Integral flip_bitsge(Integral x, int b) noexcept {
    return x ^ ~((Integral(1) << b)-1);
  }

//Flip all bits < position b, nop if b > sizeof(x) * CHAR_BIT
template <typename Integral>
  constexpr Integral flip_bitsle(Integral x, int b) noexcept {
    return x ^ ((Integral(1) << (b+1))-1);
  }

////////////////////////////////////
//Power of 2 manipulation
////////////////////////////////////

//Returns true if x is a power of 2 or zero
template <typename Integral>
  constexpr bool ispow2oz(Integral x) noexcept {
    return (x & (x-1)) == 0;
    //return popcount(x) <= 1
  }

//Returns true if x is a power of 2
//Application: Integral template arguments, add optimizations for power of 2
template <typename Integral>
  constexpr bool ispow2(Integral x) noexcept {
    return x != 0 && ispow2oz(x);
    //return popcount(x) == 1;
  }

//Round up to the next power of 2
//Application: Growable containers whose size must be a power of 2
//Application: Extending a 2d image size to a power of 2 for 3d graphics libraries (OpenGL/DirectX)
template <typename Integral>
constexpr14 Integral ceilp2(Integral x) noexcept {
  x = x-1;
  x |= rshl(x, 1);
  x |= rshl(x, 2);
  x |= rshl(x, 4);
  if(sizeof(x) > 1) {
    x |= rshl(x, 8);
    if(sizeof(x) > 2) {
      x |= rshl(x, 16);
      if(sizeof(x) > 4) {
        x |= rshl(x, 32);
      }
    }
  }
  return x + 1;
}

//Round down to the previous power of 2
//Application: See ceilp2
template <typename Integral>
constexpr14 Integral floorp2(Integral x) noexcept {
  x |= rshl(x, 1);
  x |= rshl(x, 2);
  x |= rshl(x, 4);
  if(sizeof(x) > 1) {
    x |= rshl(x, 8);
    if(sizeof(x) > 2) {
      x |= rshl(x, 16);
      if(sizeof(x) > 4) {
        x |= rshl(x, 32);
      }
    }
  }
  return x - rshl(x, 1);
}

////////////////////////////////////
//Pointer and size alignment helpers
////////////////////////////////////

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

//Returns true if t is aligned to a
template <typename Integral>
constexpr bool is_aligned(Integral t, size_t a) noexcept {
  return ((t & (a-1)) == 0);
}
bool is_aligned(void* t, size_t a) noexcept {
  return is_aligned(uintptr_t(t), a);
}

///////////////////////////////////
//Others
///////////////////////////////////

//Parallel Bits Deposit
//x    HGFEDCBA
//mask 01100100
//res  0CB00A00
//x86_64 BMI2: PDEP
template <typename Integral>
constexpr14 Integral pbits_deposit(Integral x, Integral mask) {
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
constexpr14 Integral pbits_extract(Integral x, Integral mask) {
  Integral res = 0;
  for(Integral bb = 1; mask != 0; bb += bb) {
    if(x & mask & -mask) {
      res |= bb;
    }
    mask &= (mask - 1);
  }
  return res;
}

//Returns the next higher number with the same number of 1 bits.
template <typename Integral>
constexpr14 Integral snoob(Integral x) {
  Integral smallest = x & -x;
  Integral ripple = x + smallest;
  Integral ones = x ^ ripple;
  ones = (ones >> 2) / smallest;
  return ripple | ones;
}


} //namespace std

#endif
