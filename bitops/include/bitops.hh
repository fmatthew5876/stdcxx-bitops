namespace std {

//Returns the number of trailing zeros in x, or sizeof(x) * CHAR_BIT if x is 0
//x86_64 w/ BMI1: tzcnt
//Alpha: cttz
//MIPS: CLZ
//gcc: x == 0 ? sizeof(x) * CHAR_BIT :__builtin_ctz(x)
template <typename Integral>
  constexpr int ctz(Integral x) noexcept;

//Returns the number of trailing zeroes in x, result is undefined if x is 0
//i386: bsf
//x86_64 w/ BMI1: tzcnt
//Alpha: cttz
//MIPS: CLZ
//gcc: __builtin_ctz()
template <typename Integral>
  constexpr int ctznz(Integral x) noexcept;

//Returns the number of leading zeroes in x, or sizeof(x) * CHAR_BIT if x is 0
//x86_64 w/ SSE4: lzcnt 
//ARMv5: CLZ
//IA64: clz
//Alpha: CTLZ
//PowerPC: cntlz[dw]
//gcc: x == 0 ? sizeof(x) * CHAR_BIT :__builtin_clz(x)
template <typename Integral>
  constexpr int clz(Integral x) noexcept;

//Returns the number of leading zeroes in x, result is undefined if x is 0
//i386: bsr
//x86_64 w/ SSE4: lzcnt 
//ARMv5: CLZ
//IA64: clz
//Alpha: CTLZ
//PowerPC: cntlz[dw]
//gcc: __builtin_clz(x)
template <typename Integral>
  constexpr int clznz(Integral x) noexcept;

//Returns the number of leading redundant sign bits in x
//ARMv8: CLS
//Blackfin: SIGNBITS
//C6X: NORM
//Picochip: SBC
//gcc: __builtin_clrsb(x)
template <typename Integral>
  constexpr int clrsb(Integral x) noexcept;

//Returns the number of 1 bits in x.
//i386: popcnt
//IA64: popcnt
//Alpha: CTPOP
//PowerPC: popcntb
//SparcV9: POPC
//Blackfin: ONES
//gcc: __builtin_popcount(x)
template <typename Integral>
  constexpr int popcount(Integral x) noexcept;

//Returns the value when each group of packets has it's packets reversed.
//A packet is a contiguous set of bits, a nibble is a packet of size 4, a byte is a packet of size 8 (or CHAR_BIT).
//This is the generic function which drives all of the bit and byte reversal helpers.
template <typename Integral, size_t packet_size, size_t groupby>
  constexpr Integral binary_reverse(Integral x);

//Reverse the bits in x
//static_assert(ispow2oz(groupby) && (groupby == 0 || (sizeof(x) * CHAR_BIT) % groupby == 0))
//groupby=0, reverses all of the bits in x, same as groupby == sizeof(x) * CHAR_BIT
//groupby=1, return x, do nothing
//groupby>1, reverses the bits in each 
//ARMv7: RBIT
//EPIPHANY: BITR
template <typename Integral, size_t groupby=0>
  constexpr Integral revbits(Integral x) noexcept {
    return binary_reverse<Integral, 1, groupby>(x);
  }

//Reverse the nibbles in x
//AVR: SWAP
template <typename Integral, size_t groupby=0>
  constexpr Integral revnibbles(Integral x) noexcept {
    return binary_reverse<Integral, 4, groupby>(x);
  }

//Reverse the bytes in x (byte swap). Can be used to implement an endian library.
//static_assert(grouby == 0 || sizeof(x) % groupby == 0);
//groupby=0, reverses all of the bytes in x, same as groupby==sizeof(x)
//groupby=1, return x, do nothing
//groupby>1, reverses the bytes in each n byte word in x
//0: i386: bswap
//0: ARMv5: REV
//0: PDP-11: SWAB
//2: ARMv6: REV16
//4: ARMv8: REV32
//gcc: __builtin_bswap[16|32|64](x)
template <typename Integral, size_t groupby=0>
  constexpr Integral revbytes(Integral x) noexcept {
    return binary_reverse<Integral, CHAR_BIT, groupby>(x);
  }

//Reverses the words in x, where each word wordsz bytes long
//static_assert(wordsz > 0 && sizeof(x) % wordsz == 0);
//static_assert(grouby == 0 || sizeof(x/wordsz) % groupby == 0);
//if wordsz == 1, then call revbytes<Integral, groupby>(x);
//wordsz = 2, groupby=0: MC68020 SWAP
template <typenme Integral, size_t wordsz, size_t groupby=0>
  constexpr Integral revwords(Integral x) noexcept {
    static_assert(wordsz > 0, "Cannot have a word of size 0!");
    return binary_reverse<Integral, wordsz * CHAR_BIT, groupby>(x);
  }

//Perform saturated addition on l and r.
//ARMv7 DSP extensions: QADD
template <typename IntegralL, typename integralR>
  constexpr auto satadd(IntegralL l, IntegralR r) noexcept -> decltype(l + r);

//Perform saturated subtraction on l and r.
//ARMv7 DSP extensions: QSUB
template <typename IntegralL, typename integralR>
  constexpr auto satsub(IntegralL l, IntegralR r) noexcept -> decltype(l - r);

//Logical left shift, undefined if s < 0 or x > sizeof(x) * CHAR_BIT
//Just about every processor in existance has this
//Included for symmetry
template <typename Integral>
  constexpr Integral shll(Integral x, int s) noexcept;

//Logical right shift, undefined if s < 0 or x > sizeof(x) * CHAR_BIT
//Just about every processor in existance has this
//Included for symmetry, also can right shift a signed number easily without a cast to unsigned
template <typename Integral>
  constexpr Integral shlr(Integral x, int s) noexcept;

//Arithmetic left shift, undefined if s < 0 or x > sizeof(x) * CHAR_BIT
//Just about every processor in existance has this
//Included for symmetry
template <typename Integral>
  constexpr Integral shal(Integral x, int s) noexcept;

//Arithmetic right shift, undefined if s < 0 or x > sizeof(x) * CHAR_BIT
//Just about every processor in existance has this, signed right shift is implementation defined. There is no standards compliant alternative to shar().
template <typename Integral>
  constexpr Integral shar(Integral x, int s) noexcept;

//Circular left shift (rotate), undefined if s < 0 or x > sizeof(x) * CHAR_BIT
//Just about every processor in existance has this, including the PDP-11 (1969) and yet C or C++ never included a way to get at this instruction.
template <typename Integral>
  constexpr Integral rotl(Integral x, int s) noexcept;

//Circular right shift (rotate), undefined if s < 0 or x > sizeof(x) * CHAR_BIT
//Just about every processor in existance has this, including the PDP-11 (1969) and yet C or C++ never included a way to get at this instruction.
template <typename Integral>
  constexpr Integral rotr(Integral x, int s) noexcept;

//Sets bit b in x
template <typename Integral>
  constexpr Integral set_bit(Integral x, int b) noexcept {
    return x | (Integral(1) << b);
  }

//Resets bit b in x
template <typename Integral>
  constexpr Integral reset_bit(Integral x, int b) noexcept {
    return x & ~(Integral(1) << b);
  }

//Flips bit b in x
template <typename Integral>
  constexpr Integral flip_bit(Integral x, int b) noexcept {
    return x ^ (Integral(1) << b);
  }

//Resets the least significant bit, or nop if x is 0.
//x86_64 w/ BMI1: BLSR
template <typename Integral>
  constexpr Integral reset_lsb(Integral x) noexcept {
    return x & (x-1);
  }

//Resets the most significant bit, or nop if x is 0.
template <typename Integral>
  constexpr Integral reset_msb(Integral x) noexcept {
    return x & ~(Integral(1) << ((sizeof(x) *CHAR_BIT) - clz(x)-1));
  }

//Resets all bits >= position b, nop if b > sizeof(x) * CHAR_BIT
//x86_64 w/ BMI2: BZHI
template <typename Integral>
  constexpr Integral reset_bits_ge(Integral x, int b) noexcept {
    return x & ((Integral(1) << b)-1);
  }

//Resets all bits < position b, nop if b > sizeof(x) * CHAR_BIT
template <typename Integral>
  constexpr Integral reset_bits_le(Integral x, int b) noexcept {
    return x & ~((Integral(1) << (b+1))-1);
  }

//Set all bits >= position b, nop if b > sizeof(x) * CHAR_BIT
//x86_64 w/ BMI2: BZHI
template <typename Integral>
  constexpr Integral set_bits_ge(Integral x, int b) noexcept {
    return x | ~((Integral(1) << b)-1);
  }

//Sets all bits < position b, nop if b > sizeof(x) * CHAR_BIT
template <typename Integral>
  constexpr Integral set_bits_le(Integral x, int b) noexcept {
    return x | ((Integral(1) << (b+1))-1);
  }

//Flip all bits >= position b, nop if b > sizeof(x) * CHAR_BIT
//x86_64 w/ BMI2: BZHI
template <typename Integral>
  constexpr Integral flip_bits_ge(Integral x, int b) noexcept {
    return x ^ ~((Integral(1) << b)-1);
  }

//Flip all bits < position b, nop if b > sizeof(x) * CHAR_BIT
template <typename Integral>
  constexpr Integral flip_bits_le(Integral x, int b) noexcept {
    return x ^ ((Integral(1) << (b+1))-1);
  }

//Returns true if x is a power of 2 or zero
template <typename Integral>
  constexpr bool is_pow2_or_zero(Integral x) noexcept {
    return x & (x-1);
    //return popcount(x) <= 1
  }

//Returns true if x is a power of 2
template <typename Integral>
  constexpr bool is_pow2(Integral x) noexcept {
    return x != 0 && is_pow2_or_zero(x);
    //return popcount(x) == 1;
  }

//Returns the number of 1 bits in x mod 2
//gcc: __builtin_parity(x)
template <typename Integral>
  constexpr int parity(Integral x) noexcept {
    return popcount(x) & 1;
  }

//Returns the smallest number n when n >= val && is_aligned(n, align). align must be a power of 2!
template <typename Integral>
constexpr auto align_up(Integral val, size_t a) noexcept {
  return ((val + (a -1)) & -a);
}
constexpr void* align_up(void* val, size_t a) noexcept {
  return (void*)align_up(uintptr_t(val), a);
}

//Returns the largest number n when n <= val && is_aligned(n, align). align must be a power of 2!
template <typename Integral>
constexpr auto align_down(Integral val, size_t a) noexcept {
  return val & -a;
}
constexpr void* align_down(void* val, size_t a) noexcept {
  return (void*)align_down(uintptr_t(val), a);
}

//Returns true if t is aligned to a
template <typename Integral>
constexpr auto is_aligned(Integral t, size_t a) noexcept {
  return ((t & (a-1)) == 0);
}
constexpr bool is_aligned(void* t, size_t a) noexcept {
  return is_aligned(uintptr_t(t), a);
}

//Return smallest power of 2 >= x
template <typename Integral>
constexpr auto pow2ge(Integral x) noexcept;

//Return smallest power of 2 > x
template <typename Integral>
constexpr auto pow2gt(Integral x) noexcept;

//Return smallest power of 2 <= x
template <typename Integral>
constexpr auto pow2le(Integral x) noexcept;

//Return smallest power of 2 < x
template <typename Integral>
constexpr auto pow2lt(Integral x) noexcept;


} //namespace std
