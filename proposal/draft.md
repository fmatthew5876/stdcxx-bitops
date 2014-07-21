A constexpr bitwise operations library for C++
==========================================

* Document Number: N3864
* Date: 2014-01-08
* Programming Language C++, Numerics Working Group
* Reply-to: Matthew Fioravante <fmatthew5876@gmail.com>

The latest draft, reference header (incomplete), and links to past discussions on github: 

* https://github.com/fmatthew5876/stdcxx-bitops

Introduction
=============================

This proposal adds support for low level bitwise and logical operations to C++.

Impact on the standard
=============================

This proposal is a pure library extension. 
It does not require any changes in the core language and does not depend on any other library extensions.
The proposal is composed entirely of free functions. The proposed functions are added to the `<cmath>` and `<memory>`
headers. No new headers are introduced.

While this proposal can be implemented entirely in standard C++14,
optimal implementations will require additional support from the compiler to detect and
replace function calls with native instructions when available.
See \[[BitOpsRef](#BitOpsRef)\] for a reference implementation written in C++14.

Motivation
================

The C and C++ languages provide an abstraction over the machine.
The machine provides the common arithmetic and logical operations, which are
accessed using the built in operators inherited from C. These operations are the primitives
which are used to implement higher level abstractions.

We construct algorithms by combining these basic operations.
Sometimes significant performance benefits can be gained by
directly manipulating the binary quantities contained
within the registers.
Many online and print references including \[[Anderson01](#Anderson01)\],
\[[Dietz01](#Dietz01)\], \[[Neumann01](#Neumann01)\], \[[Warren01](#Warren01)\], and \[[HAKMEM](#HAKMEM)\]
are devoted to discovering these algorithms and implementing
them efficiently.

Hardware vendors understand the importance of high performance
bitwise manipulation routines. Many of them have provided
additional hardware which can perform these bitwise operations directly with
a single instruction. These instructions are often much more efficient than
computing the algorithm manually in C or assembly.

Other bitwise manipulation algorithms can be implemented using
clever but non-intuitive combinations of arithmetic and logical operations.
Most importantly, for some bitwise algorithms, the most
efficient implementation varies between hardware platforms.
These differences create an unreasonably large maintenance burden on the programmer
who wishes to write efficient and portable code.

As a motivating example, consider the various implementations of
the count trailing zeros algorithm presented in \[[Kostjuchenko01](#Kostjuchenko01)\].
In order to implement an SSE2 optimized `strlen()` function,
the author had to implement, test, and profile many different versions of
count trailing zeros.  None of them take advantage of native instructions.

One who wishes to exploit 
the `bsf` or `tzcnt` instructions on Intel must rely on non-standard
compiler intrinsics or inline assembly. One must also provide 
backup implementations for other platforms which do not have such instructions.
Adding support for native instructions requires a nest of `#ifdef`s and
deep knowledge of different processor architectures. This is a heavy cost
in programmer time.

Bitwise algorithms can be used in a wide variety of domains and are the key
to unlocking high performance in many important algorithms. A bitwise operations 
library has been badly needed in the C and C++ standard libraries for many years.

We present a bitwise operations library which exposes these native instructions
and provides backup implementations in C++ if they do not exist.
Because this library would be part of the standard, these commonly used routines
could be implemented once and profiled on each platform. Finally, this library 
offers an interface which takes full advantage of the latest processor features
including ARMv8 and Intel BMI2.

Design Goals and Scope
========================

There are seemingly endless ways one can manipulate binary quantities. How does one 
choose which functions to include in a library? 
How does one choose proper names for each function? Which algorithms can
require the programmer to declare their use through a function call?
We will address these questions with the following design goals.

Design Goal 1: Provide the programmer with better access to the machine
----------------------------

Digital Equipment Corporation announced the PDP11 \[[pdp11](#pdp11)\] in 1970. 
This 16 bit machine has 3 instructions of interest, `ROR` (rotate right), 
`ROL` (rotate left), and `SWAB` (swap bytes).
These operations along with their later 32 and 64 bit variants are provided 
by many modern architectures. As of 2013,
programmers still do not have direct access to these instructions in modern C and C++.

Therefore, the first and most important goal of this proposal is to provide programmers with better
access to the machine via a new set of primitives which go beyond simple arithmetic
and logical operations. We will present new functions for the standard library which can be implemented
using only a few instructions if supported by the machine, using backup implementations
if no such support is provided by the hardware.

Design Goal 2: Provide a reusable library of generic bitwise manipulation routines
--------------------------

In designing this proposal, we wish not just to limit ourselves to operations which may have
native machine instruction implementations on at least one platform. We would like to provide
a standard library of primitives which are frequently reimplemented in different code bases.
The standard library already provides a rich set of generic containers and algorithms. What is missing is a 
set of bitwise manipulation primitives.

Of particular emphasis are algorithms whose most efficient implementations depend on the implementations of
other bitwise operations. A motivating example is checking whether a number is a power of 2. 

Consider the following implementations:

    bool ispow2(unsigned x) { return popcount(x) == 1; }
    bool ispow2(unsigned x) { return (x & (x -1)) == 0 && x != 0; }

In the above example, `popcount()` is the population count or number of 1 bits in `x`. 
On a machine with a popcount instruction, the first implementation uses less instructions
and no branches. Without a popcount instruction, the second version is the better choice
as computing popcount requires much more than a few logical operations and comparisons 
\[[Dietz01](#Dietz01)\]. In order to implement `ispow2()`, the programmer is faced with
the same set of dilemmas as with the count trailing zeros example from the [Motivation](#motivation)
section.

Glossary of Terms
=====================

The following terminology is used in the remainder of this document to describe the technical aspects of this proposal.

* *set*: If we say that a bit has been "set", we mean that we will change its value to 1. We can also say "set the bit to x", which changes the value to 0 or 1, depending on the value of x.
* *reset*: To reset a bit is to change its value to 0.
* *flip*: To flip a bit is to invert its value. That is set the bit if it is currently 0 and likewise reset the bit if it is currently 1.
* *test*: To test a bit is to return `true` if its value is 1, otherwise return `false`.
* *subword*: A collection of contiguous bits of a given size. Some commonly found examples:
 * *nibble*: a subword, usually 4 bits.
 * *byte*: a subword of size `CHAR_BIT`, usually 8 bits.
 * *word*: Depending on the platform terminology, often a subword of size 16, 32 or 64. 
* *most significant bit (msb)*: The highest order bit in a binary quantity.
* *most significant X bit (msXb)*: The highest order bit in a binary quantity with a value of X.
* *least significant bit (lsb)*: The lowest order bit in a binary quantity.
* *least significant X bit (lsXb)*: The lowest order bit in a binary quantity with a value of X.
* `~T(0)`: This statement represents a quantity of type `T` where all of the bits are 1. We avoid the more commonly used `T(-1)` as it assumes 2's complement signed integers and is somewhat less intuitive to the uninitiated.

Technical Specification
====================

We will now describe the additions to `<cmath>` and `<memory>`. This is a procedural library implemented
entirely using `constexpr` templated free functions.
All functions have been qualified with `noexcept`. These
operations are most often used in highly optimized numerical code where the overhead of exception
handling would be inappropriate. For functions which have pre-conditions on their inputs, we
have opted for undefined return values if these pre-conditions are ever violated.
The functions are classified into different groups to aid analysis and discussion
and each group will be presented one at a time. 

We have chosen to support all signed and unsigned integral types in this proposal. It
is often suggested that signed integers represent "numbers" and unsigned integers
represent "bit fields" and that they should never be used together. While we 
generally agree with this philosophy, many of these algorithms have real use cases
for signed and unsigned integral values.
The primary danger of using both
signed and unsigned integers comes from the pitfalls of comparing signed and unsigned values. None of the functions in this
proposal require or encourage comparing or combining of signed and unsigned types.
The template arguments for each proposed function are named `integral` to indicate generic support
for all builtin integral types, signed and unsigned. Functions which take more than one
integral argument of different types will use a single letter suffix, for example
`integrall` and `integralr` for left and right hand arguments.

This proposal does not require signed integers be implemented using 2's complement. However, the design of this proposal considers
the practical reality that almost all modern hardware does in fact use 2's complement.
All example code, including the reference implementation \[[BitOpsRef](#BitOpsRef)\]
assume 2's complement signed integers with undefined behavior on overflow and underflow.
Adding support for other signed representations is an exercise left for the reader.

Each section will describe the full technical specifications of the functions in that group, noting their
return values and undefined behavior if any. We will also discuss the background
and justification for each of the functions and list some applications where
necessary. For the more complicated algorithms, examples will be provided to
help illustrate how they work.

cmath Header Additions
-------------------------

The following sections describe the additions to the `<cmath>` header.

### Explicit shifts

Bit shifting is provided in C++ with `operator<<` and `operator>>` for integral types. It is
a very simplistic abstraction with many deficiencies and some subtle caveats.

First as noted earlier, there is no primitive for circular shifts even though these shifts can be found in the instruction 
sets of most architectures. Second, 
`operator>>` for signed types has implementation defined behavior with regards to filling in the high order bits, making
it nearly useless when writing portable code.
Writing a portable arithmetic right shift is cumbersome at best and inefficient at worst. Finally, performing a logical right shift on a signed
quantity is also cumbersome because it requires casts which obscure the meaning of the code.

#### List of Functions

    //SHift Logical Left
    template <class integral>
    constexpr integral shll(integral x, int s) noexcept;

* *Returns:* `x << s`
* *Remarks:* result is undefined if `s < 0 || s >= sizeof(x) * CHAR_BIT`

<!-- -->

    //SHift Logical Right
    template <class integral>
    constexpr integral shlr(integral x, int s) noexcept;

* *Returns:* `x` with all of its bits shifted right by `s` positions. The `s` high order bits of the result are reset.
* *Remarks:* result is undefined if `s < 0 || s >= sizeof(x) * CHAR_BIT`

<!-- -->

    //SHift Arithmetic Left
    template <class integral>
    constexpr integral shal(integral x, int s) noexcept;

* *Returns:* `x << s`
* *Remarks:* result is undefined if `s < 0 || s >= sizeof(x) * CHAR_BIT`
* *Remarks:* This function is identical to `shll()` and is only provided for symmetry.

<!-- -->

    //SHift Arithmetic Right
    template <class integral>
    constexpr integral shar(integral x, int s) noexcept;
    
* *Returns:* `x` with all of its bits shifted right by `s` positions. The `s` high order bits of the result are set to the value of most significant bit of `x`.
* *Remarks:* result is undefined if `s < 0 || s >= sizeof(x) * CHAR_BIT`

<!-- -->

    //ROTate Left
    template <class integral>
    constexpr integral rotl(integral x, int s) noexcept;

* *Returns:* `x` with all of its bits shifted left by `s` positions.
    The `s` low order bits are set to the `s` high order bits of `x`.
* *Remarks:* result is undefined if `s < 0 || s >= sizeof(x) * CHAR_BIT`

<!-- -->

    //ROTate Right
    template <class integral>
    constexpr integral rotr(integral x, int s) noexcept;

* *Returns:* `x` with all of its bits shifted right by `s` positions.
    The `s` high order bits are set to the `s` low order bits of `x`.
* *Remarks:* result is undefined if `s < 0 || s >= sizeof(x) * CHAR_BIT`

### Bit Counting Algorithms

Bit counting is used to construct efficient implementations of other higher level algorithms.
Many of these functions have native support on a wide variety of architectures.
Some example applications will be provided below.

#### List of functions
    
    //CouNT Trailing 0's
    template <class integral>
    constexpr int cntt0(integral x) noexcept;

* *Returns:* The number of trailing 0 bits in `x`, or `sizeof(x) * CHAR_BIT` if `x == 0`.
    
<!-- -->
    
    //CouNT Leading 0's
    template <class integral>
    constexpr int cntl0(integral x) noexcept;

* *Returns:* The number of leading 0 bits in `x`, or `sizeof(x) * CHAR_BIT` if `x == 0`.
    
<!-- -->
    
    //CouNT Trailing 1's
    template <class integral>
    constexpr int cntt1(integral x) noexcept;

* *Returns:* The number of trailing 1 bits in `x`, or `sizeof(x) * CHAR_BIT` if `x == ~integral(0)`.
    
<!-- -->
    
    //CouNT Leading 1's
    template <class integral>
    constexpr int cntl1(integral x) noexcept;

* *Returns:* The number of leading 1 bits in `x`, or `sizeof(x) * CHAR_BIT` if `x == ~integral(0)`.
    
<!-- -->
    
    //POPulation COUNT
    template <class integral>
    constexpr int popcount(integral x) noexcept;

* *Returns:* The number of 1 bits in `x`.
    
<!-- -->
    
    //PARITY
    template <class integral>
    constexpr int parity(integral x) noexcept;

* *Returns:* 1 if the number of 1 bits in `x` is odd, otherwise 0.

#### Applications

One application of `cntt0` is in computing the greatest common divisor of 2 numbers. Credit goes
to Howard Hinnant for bringing this to our attention.

    template <typename unsigned-integral>
    unsigned-integral gcd(unsigned-integral x, unsigned-integral y) 
    { 
        if (x == 0) 
            return y; 
        if (y == 0) 
            return x; 
        int cf2 = std::cntt0(x | y); 
        x >>= std::cntt0(x); 
        while (true) 
        { 
            y >>= std::cntt0(y); 
            if (x == y) 
                break; 
            if (x > y) 
                std::swap(x, y); 
            if (x == 1) 
                break; 
            y -= x; 
        } 
        return x << cf2; 
    } 
 
As mentioned earlier, we can use `popcount()` to detect whether or not an integer (signed or unsigned) is a power of 2.

    template <typename integral>
    bool ispow2(integral x) {
      return x >= 0 && popcount(x) == 1;
    }

### Rightmost bit manipulation

The following functions perform simple manipulations on the rightmost bits of the given quantity.
All of these operations can be trivially implemented using a few arithmetic and logical operations. Therefore,
these functions are only provided as usability wrappers in order to allow the programmer to avoid having
to spend time looking them up, reimplementing, and/or unit testing them. Because their simple implementations,
we have included the implementations in the function listing. Credit goes to \[[Warren01](#Warren01)\] and
\[[ChessProg](#ChessProg)\] for providing these implementations and insight into their usage.

Most of the operations in the section are implemented in hardware on Intel and AMD
machines which have Intel BMI and/or AMD TBM extensions 
(see [Survey of Hardware Support](#survey-of-hardware-support)).
All of these functions were tested on gcc 4.8 using the provided C++ implementations.
We found that on BMI and TBM enabled compilation, the optimizer
was successfully able to compile the C++ expression a single BMI or TBM instruction.
Therefore an implementation of this section can simply use the provided trivial implementations
and rely on the optimizer for hardware support if available.

#### List of Functions
    
    //ReSeT Least Significant 1 Bit
    template <class integral>
    constexpr integral rstls1b(integral x) noexcept;

* *Returns:* `x` with its least significant 1 bit reset, or 0 if `x == 0`.
* *Implementation:* `x & (x - 1)`
    
<!-- -->
    
    //SET Least Significant 0 Bit
    template <class integral>
    constexpr integral setls0b(integral x) noexcept;

* *Returns:* `x` with its least significant 0 bit set, or `x` if `x == ~integral(0)`.
* *Implementation:* `x | (x + 1)`
    
<!-- -->
    
    //ISOlate Least Significant 1 Bit
    template <class integral>
    constexpr integral isols1b(integral x) noexcept;

* *Returns:* A quantity where the bit in the position of the least significant 1 bit of `x` is set and all of the other bits reset. Returns 0 if `x == 0`.
* *Implementation:* `(~x) & (-x)`
    
<!-- -->
    
    //ISOlate Least Significant 0 Bit
    template <class integral>
    constexpr integral isols0b(integral x) noexcept;

* *Returns:* A quantity where the bit in the position of the least significant 0 bit of `x` is set and all of the other bits reset. Returns `x` if `x == ~integral(0)`.
* *Implementation:* `(~x) & (x + 1)`
    
<!-- -->
    
    //ReSeT Trailing 1's
    template <class integral>
    constexpr integral rstt1(integral x) noexcept;

* *Returns:* resets all of the trailing 1's in `x`, or 0 if `x == ~integral(0)`.
* *Implementation:* `x & (x + 1)`
    
<!-- -->
    
    //SET Trailing 0's
    template <class integral>
    constexpr integral sett0(integral x) noexcept;

* *Returns:* sets all of the trailing 0's in `x`, or `x` if `x == 0`.
* *Implementation:* `x | (x - 1)`
    
<!-- -->
    
    //MaSK Trailing 0's
    template <class integral>
    constexpr integral maskt0(integral x) noexcept;

* *Returns:* a quantity where all of the bits corresponding to the trailing 0 bits of `x` are set, the remaining bits reset.
* *Implementation:* `(~x) & (x - 1)`
    
<!-- -->
    
    //MaSK Trailing 1's
    template <class integral>
    constexpr integral maskt1(integral x) noexcept;

* *Returns:* a quantity where all of the bits corresponding to the trailing 1 bits of `x` are set, the remaining bits reset.
* *Implementation:* `x & ~(x + 1)`
    
<!-- -->
    
    //MaSK Trailing 0's and Least Significant 1 Bit
    template <class integral>
    constexpr integral maskt0ls1b(integral x) noexcept;

* *Returns:* a quantity where all of the bits corresponding to the trailing 0 bits and the least significant 1 bit of `x` are set, the remaining bits reset.
* *Implementation:* `x ^ (x - 1)`
    
<!-- -->
    
    //MaSK Trailing 1's and Least Significant 0 Bit
    template <class integral>
    constexpr integral maskt1ls0b(integral x) noexcept;

* *Returns:* a quantity where all of the bits corresponding to the trailing 1 bits and the least significant 0 bit of `x` are set, the remaining bits are reset.
* *Implementation:* `x ^ (x + 1)`
    
### Single Bit Manipulation

Most programmers at some point have needed to index and manipulate a single bit
within a word.
These functions are trivial to implement and are provided only for usability.

#### List of Functions
    
    //SET BIT
    template <class integral>
    constexpr integral setbit(integral x, int b) noexcept;

* *Returns:* Sets bit at position `b` of `x`.
* *Remarks:* Result is undefined if `b < 0 || b >= sizeof(x) * CHAR_BIT`.
* *Implementation:* `x | (integral(1) << b)`
    
<!-- -->
    
    //ReSeT BIT
    template <class integral>
    constexpr integral rstbit(integral x, int b) noexcept;

* *Returns:* Resets bit at position `b` of `x`.
* *Remarks:* Result is undefined if `b < 0 || b >= sizeof(x) * CHAR_BIT`.
* *Implementation:* `x & ~(integral(1) << b)`
    
<!-- -->
    
    //FLIP BIT
    template <class integral>
    constexpr integral flipbit(integral x, int b) noexcept;

* *Returns:* Flips bit at position `b` of `x`.
* *Remarks:* Result is undefined if `b < 0 || b >= sizeof(x) * CHAR_BIT`.
* *Implementation:* `x ^ (integral(1) << b)`
    
<!-- -->
    
    //TEST BIT
    template <class integral>
    constexpr bool testbit(integral x, int b) noexcept;

* *Returns:* Returns true if the bit at position `b` of `x` is set, otherwise 0.
* *Remarks:* Result is undefined if `b < 0 || b >= sizeof(x) * CHAR_BIT`.
* *Implementation:* `bool((x >> b) & 1) != 0)`
    
### Range of bits manipulation

The following functions manipulate ranges of bits above or below a given index.
One of them is implemented in hardware
(see [Survey of Hardware Support](#survey-of-hardware-support)),
the rest are provided for usability and completeness.

#### List of functions
    
    //ReSeT BITS Greater than or Equal to
    template <class integral>
    constexpr integral rstbitsge(integral x, int b) noexcept;

* *Returns:* Reset all bits of `x` in positions greater than or equal to `b`.
* *Remarks:* Result is undefined if `b < 0 || b >= sizeof(x) * CHAR_BIT`.
    
<!-- -->
    
    //ReSeT BITS Less than or Equal to
    template <class integral>
    constexpr integral rstbitsle(integral x, int b) noexcept;

* *Returns:* Reset all bits of `x` in positions less than or equal to `b`.
* *Remarks:* Result is undefined if `b < 0 || b >= sizeof(x) * CHAR_BIT`.
    
<!-- -->
    
    //SET BITS Greater than or Equal to
    template <class integral>
    constexpr integral setbitsge(integral x, int b) noexcept;

* *Returns:* Set all bits of `x` in positions greater than or equal to `b`.
* *Remarks:* Result is undefined if `b < 0 || b >= sizeof(x) * CHAR_BIT`.
    
<!-- -->
    
    //SET BITS Less than or Equal to
    template <class integral>
    constexpr integral setbitsle(integral x, int b) noexcept;

* *Returns:* Set all bits of `x` in positions less than or equal to `b`.
* *Remarks:* Result is undefined if `b < 0 || b >= sizeof(x) * CHAR_BIT`.
    
<!-- -->
    
    //FLIP BITS Greater than or Equal to
    template <class integral>
    constexpr integral flipbitsge(integral x, int b) noexcept;

* *Returns:* Flip all bits of `x` in positions greater than or equal to `b`.
* *Remarks:* Result is undefined if `b < 0 || b >= sizeof(x) * CHAR_BIT`.
    
<!-- -->
    
    //FLIP BITS Less than or Equal to
    template <class integral>
    constexpr integral flipbitsle(integral x, int b) noexcept;

* *Returns:* Flip all bits of `x` in positions less than or equal to `b`.
* *Remarks:* Result is undefined if `b < 0 || b >= sizeof(x) * CHAR_BIT`.
    
### Bitwise and Bytewise Permutations

These functions provide a generic interface for permuting the bits and bytes in a word.
Each function takes the following form:

    template <typename integral>
    constexpr integral permute_bits(integral x, /*arg2, arg3, ...*/ subword_bits=1, num_swar_words=1) noexcept;

In the above example, `x` is the value to permute, with additional arguments following it if needed.
Each function operates on subwords of size `subword_bits` measured in bits. In this case,
`permute_bits(x)` will perform the permutation on all of the bits of `x`, `permute_bits(x, 2)` will
permute each pair of bits in `x`, `permute_bits(x, 4)` each nibble in `x`, and finally `permute_bits(x, CHAR_BIT)`
will permute the bytes of `x`.

The `num_swar_words` parameter (Number of Simd Within A Register Words) enables parallel operation
on multiple words within `x`. The size in bits of each individual word to permute will be `sizeof(integral) * CHAR_BIT / num_swar_words`.
For example, `permute_bits<uint32_t>(x, 1, 2)` will independently permute the 16 high order bits of `x` 
and the 16 low order bits of `x`. Another example, `permute_bits<uint32_t>(x, 2, 4)` will permute the
pairs of bits in each byte (assuming `CHAR_BIT == 8`) of `x`.

Finally, for each bitwise permutation routine, we also provide a corresponding bytewise
permutation routine. These are provided for usability. They operate exactly like their
bitwise cousins, except that their subword size is computed in bytes instead of bits.

    template <typename integral>
    constexpr integral permute_bytes(integral x, /*arg2, arg3, ...*/ subword_bytes=1, num_swar_words=1) noexcept;

The bytewise routines are trivially implemented as simple wrappers over the bitwise routines where
we perform the simple conversion: `subword_bits = subword_bytes * CHAR_BITS`.

#### List of Functions

* *Remarks:* For all functions defined in this section, the result is undefined if any of the following hold:
 * `num_swar_words < 0 || num_swar_words > sizeof(integral) * CHAR_BIT`
 * `(sizeof(integral) * CHAR_BIT) % num_swar_words != 0`
 * `subword_bits < 0 || subword_bits > ((sizeof(integral) * CHAR_BIT) / num_swar_words)`
 * `((sizeof(integral) * CHAR_BIT) / num_swar_words) % subword_bits != 0`
    
<!-- -->

    template <class integral>
    constexpr integral reverse_bits(integral x, int subword_bits=1, int num_swar_words=1) noexcept;

* *Returns:* Split `x` into `num_swar_words` "words" of equal size and then independently reverse the subwords of size `subword_bits` bits in each word.
    
<!-- -->

    template <class integral>
    constexpr integral reverse_bytes(integral x, int subword_bytes=1, int num_swar_words=1) noexcept;

* *Returns:* `reverse_bits(x, subword_bytes * CHAR_BIT, num_swar_words);`
    
<!-- -->
    
    template <class integral>
    constexpr integral outer_perfect_shuffle_bits(integral x, int subword_bits=1, int num_swar_words=1) noexcept;

* *Returns:* Split `x` into `num_swar_words` "words" of equal size and then independently outer perfect shuffle the subwords of size `subword_bits` bits in each word.
    
<!-- -->

    template <class integral>
    constexpr integral outer_perfect_shuffle_bytes(integral x, int subword_bytes=1, int num_swar_words=1) noexcept;

* *Returns:* `outer_perfect_shuffle_bits(x, subword_bytes * CHAR_BIT, num_swar_words);`
    
<!-- -->
    
    template <class integral>
    constexpr integral outer_perfect_unshuffle_bits(integral x, int subword_bits=1, int num_swar_words=1) noexcept;

* *Returns:* Split `x` into `num_swar_words` "words" of equal size and then independently outer perfect unshuffle the subwords of size `subword_bits` bits in each word.
    
<!-- -->

    template <class integral>
    constexpr integral outer_perfect_unshuffle_bytes(integral x, int subword_bytes=1, int num_swar_words=1) noexcept;

* *Returns:* `outer_perfect_unshuffle_bits(x, subword_bytes * CHAR_BIT, num_swar_words);`
    
<!-- -->
    
    template <class integral>
    constexpr integral inner_perfect_shuffle_bits(integral x, int subword_bits=1, int num_swar_words=1) noexcept;

* *Returns:* Split `x` into `num_swar_words` "words" of equal size and then independently inner perfect shuffle the subwords of size `subword_bits` bits in each word.
    
<!-- -->

    template <class integral>
    constexpr integral inner_perfect_shuffle_bytes(integral x, int subword_bits=1, int num_swar_words=1) noexcept;

* *Returns:* `inner_perfect_shuffle_bits(x, subword_bytes * CHAR_BIT, num_swar_words);`
    
<!-- -->
    
    template <class integral>
    constexpr integral inner_perfect_unshuffle_bits(integral x, int subword_bits=1, int num_swar_words=1) noexcept;

* *Returns:* Split `x` into `num_swar_words` "words" of equal size and then independently inner perfect unshuffle the subwords of size `subword_bits` bits in each word.
    
<!-- -->

    template <class integral>
    constexpr integral inner_perfect_unshuffle_bytes(integral x, int subword_bytes=1, int num_swar_words=1) noexcept;

* *Returns:* `inner_perfect_unshuffle_bits(x, subword_bytes * CHAR_BIT, num_swar_words);`
    
<!-- -->
    
    template <class integral>
    constexpr integral deposit_bits_right(integral x, integral mask, int subword_bits=1, int num_swar_words=1) noexcept;

* *Returns:* Split `x` into `num_swar_words` "words" of equal size and then independently deposit the subwords of size `subword_bits` bits in each word identified by the bits of `mask` to the low order subwords of the result. The remaining subwords are set to 0.
    
<!-- -->

    template <class integral>
    constexpr integral deposit_bytes_right(integral x, integral mask, int subword_right=1, int num_swar_words=1) noexcept;

* *Returns:* `deposit_bits_right(x, mask, subword_bytes * CHAR_BIT, num_swar_words);`
    
<!-- -->

    template <class integral>
    constexpr integral deposit_bits_left(integral x, integral mask, int subword_bits=1, int num_swar_words=1) noexcept;

* *Returns:* Split `x` into `num_swar_words` "words" of equal size and then independently deposit the subwords of size `subword_bits` bits in each word identified by the bits of `mask` to the high order subwords of the result. The remaining subwords are set to 0.
    
<!-- -->

    template <class integral>
    constexpr integral deposit_bytes_left(integral x, integral mask, int subword_bytes=1, int num_swar_words=1) noexcept;

* *Returns:* `deposit_bits_left(x, mask, subword_bytes * CHAR_BIT, num_swar_words);`
    
<!-- -->
    
    template <class integral>
    constexpr integral extract_bits_right(integral x, integral mask, int subword_bits=1, int num_swar_words=1) noexcept;

* *Returns:* Split `x` into `num_swar_words` "words" of equal size and then independently extract the low order subwords of size `subword_bits` bits in each word to the subwords of the result identified by the bits of `mask`. The remaining subwords are set to 0.
    
<!-- -->

    template <class integral>
    constexpr integral extract_bytes_right(integral x, integral mask, int subword_bytes=1, int num_swar_words=1) noexcept;

* *Returns:* `extract_bits_right(x, mask, subword_bytes * CHAR_BIT, num_swar_words);`
    
<!-- -->

    template <class integral>
    constexpr integral extract_bits_left(integral x, integral mask, int subword_bits=1, int num_swar_words=1) noexcept;

* *Returns:* Split `x` into `num_swar_words` "words" of equal size and then independently extract the high order subwords of size `subword_bits` bits in each word to the subwords of the result identified by the bits of `mask`. The remaining subwords are set to 0.
    
<!-- -->

    template <class integral>
    constexpr integral extract_bytes_left(integral x, integral mask, int subword_bytes=1, int num_swar_words=1) noexcept;

* *Returns:* `extract_bits_left(x, mask, subword_bytes * CHAR_BIT, num_swar_words);`

#### Examples

The following table shows how some example 8 bit binary values would be permuted by each function.
We use the C++14 binary literal syntax here. The bits of a given value are represented by
letters to show how a generic value would be permuted.
For a more detailed treatment of these operations, refer to \[[Neumann01](#Neumann01)\] and Chapter 7 of \[[Warren01](#Warren01)\].

* `reverse_bits(ABCDEFGHb)` -> `GHFEDCBA`
* `reverse_bits(ABCDEFGHb, 1, 2)` -> `DCBAHGFEb`
* `reverse_bits(ABCDEFGHb, 1, 4)` -> `BADCFEHGb`
* `reverse_bits(ABCDEFGHb, 2)` -> `GHEFCDABb`
* `reverse_bits(ABCDEFGHb, 2, 2)` -> `CDABGHEFb`
* `reverse_bits(ABCDEFGHb, 4)` -> `EFGHABCDb`

* `outer_perfect_shuffle(ABCDEFGHb)` -> `EAFBGCHDb`
* `inner_perfect_shuffle(ABCDEFGHb)` -> `AEBFCGEHb`
* `outer_perfect_unshuffle(ABCDEFGHb)` -> `BDFHACEGb`
* `inner_perfect_unshuffle(ABCDEFGHb)` -> `ACEGBDFHb`
* `outer_perfect_unshuffle(outer_perfect_shuffle(x))` -> `x`
* `inner_perfect_unshuffle(inner_perfect_shuffle(x))` -> `x`

* `deposit_bits_right(ABCDEFGHb, 01110110b)` -> `0000CDFGb`
* `deposit_bits_right(ABCDEFGHb, 10001000b)` -> `000000AEb`
* `deposit_bits_left(ABCDEFGHb, 00010110b)` -> `DEF00000b`
* `deposit_bits_left(ABCDEFGHb, 10111000b)` -> `ACDE0000b`
* `extract_bits_right(ABCDEFGHb, 11000110b)` -> `EF000GH0b`
* `extract_bits_right(ABCDEFGHb, 10101010b)` -> `E0F0G0H0b`
* `extract_bits_left(ABCDEFGHb, 00110110b)` -> `00AB0CD0b`
* `extract_bits_left(ABCDEFGHb, 11111001b)` -> `ABCDE00Fb`

#### Applications

* Endian conversion for binary protocols and networking \[[N3646](#N3646)\].
* Cryptography (nibble swapping) \[[EmbedGuru01](#EmbedGuru01)\].
* Network topology definitions and routing.
* Bioinformatics, image processing, steganography, cryptanalysis, and coding \[[Hilewitz01](#Hilewitz01)\]
* Chess Board Programming \[[ChessProg](#ChessProg)\]

### Power of 2 manipulation

The following functions detect and compute powers of 2.

#### List of Functions

    //IS POWer of 2
    template <class integral>
    constexpr bool ispow2(integral x) noexcept;

* *Returns:* `true` if `x` is a positive power of 2, otherwise `false`.
    
<!-- -->
    
    //CEILing Power of 2
    template <class integral>
    constexpr integral ceilp2(integral x) noexcept;

* *Returns:* Returns the unique quantity `n` where `ispow2(n) && n >= x`.
* *Remarks:* Result is undefined if the value of `n` is too large to be represented by type `integral`.
    
<!-- -->
    
    //FLOOR Power of 2
    template <class integral>
    constexpr integral floorp2(integral x) noexcept;

* *Returns:* Returns the unique quantity `n` where `ispow2(n) && N <= x`.
* *Remarks:* Result is undefined if `x <= 0`.

#### Applications

* Data structures whose capacity must be a power of 2 (example: circular queue).
* Scaling an image dimensions to the next power of 2 for texturing in 3d rendering.

    
### Saturated arithmetic

Saturated arithmetic is useful in digital signal processing applications \[[EmbedGuru01](#EmbedGuru01)\].
It is also provided as a hardware instruction
on some machines. In our efforts to better expose hardware features, we have included saturated addition and subtraction functions in this proposal.

#### List of Functions
    
    //SATurated ADDition
    template <class integral_l, class integral_r>
    constexpr auto satadd(integral_l l, integral_r r) noexcept -&gt; decltype(l + r);

* *Returns:* `l + r`
* *Remarks:* On overflow, will return `std::numeric_limits<decltype(l + r)>::max()`
* *Remarks:* On underflow, will return `std::numeric_limits<decltype(l + r)>::min()`
    
<!-- -->
    
    //SATurated SUBtraction
    template <class integral_l, class integral_r>
    constexpr auto satsub(integral_l l, integral_r r) noexcept -&gt; decltype(l - r);

* *Returns:* `l - r`
* *Remarks:* On overflow, will return `std::numeric_limits<decltype(l + r)>::max()`
* *Remarks:* On underflow, will return `std::numeric_limits<decltype(l + r)>::min()`
    
memory Header Additions
------------------------

This section describes the additions to the `<memory>` header.

### Alignment helpers

These functions are used for aligning objects in memory. They supplement use cases with which `std::align` is
not designed to handle. These are useful operations and all of them have trivial implementations.
They can often be found in operating system kernels and device drivers reimplemented time and time
again as macros.

#### List of Functions
    
    template <class integral>
    constexpr bool is_aligned(integral x, size_t align) noexcept;

* *Returns:* `true` if `x` is a multiple of `align`.
* *Implementation:* `(x & (a - 1)) == 0`
    
<!-- -->

    bool is_aligned(void* val, size_t align) noexcept;

* *Returns:* `is_aligned(uintptr_t(val), align)`.
    
<!-- -->

    template <class integral>
    constexpr integral align_up(integral x, size_t align) noexcept;

* *Returns:* The unique value `n` such that `is_aligned(n, align) && n >= x`.
* *Implementation:* `(x + (a - 1)) & -a`
    
<!-- -->

    void* align_up(void* val, size_t align) noexcept;

* *Returns:* `(void*)align_up(uintptr_t(val), align)`.
    
<!-- -->
    
    template <class integral>
    constexpr integral align_down(integral x, size_t align) noexcept;

* *Returns:* The unique value `n` such that `is_aligned(n, align) && n <= x`.
* *Implementation:* `x & (-a)`
    
<!-- -->

    void* align_down(void* val, size_t align) noexcept;

* *Returns:* `(void*)align_down(uintptr_t(val), align)`.

#### Applications and std::align

We currently have `std::align` in the standard for doing alignment calculations.
The function `std::align`
has one specific use case, that is to carve out an aligned buffer of a known size within a larger buffer.
In order to use `std::align`, the user must a priori know the size of the aligned buffer
they require. Unfortunately in some use cases, even calculating the size of this buffer
as an input to `std::align` itself requires doing alignment calculations.
Consider the following example of using aligned SIMD registers to process a memory buffer.
The alignment calculations here cannot be done with `std::align`.

    void process(char* b, char* e) {
      char* pb = std::min((char*)std::align_up(b, sizeof(simd16)), e);
      char* pe = (char*)std::align_down(e, sizeof(simd16));
    
      for(char* p = b; p < pb; ++p) {
        process1(p);
      }
      for(char* p = pb; p < pe; p += sizeof(simd16)) {
        simd16 x = simd16_aligned_load(p);
        process16(x);
        simd16_aligned_store(x, p);
      }
      for(char* p = pe; p < e; ++p) {
        process1(p);
      }
    }

We conclude that `std::align` is much too specific for general alignment calculations. It has a narrow
use case and should only be considered as a helper function for when that use case is needed.
    
Implementation
===================

## Guidelines for Implementors

Those who wish to implement the functions provided by this proposal must consider the following guidelines:

* Implementors must at a minimum provide support for 1, 2, 4, and 8 byte integral signed and unsigned types.
* Implementors should use optimized hardware instructions wherever possible.
 * *Prefer* compiler intrinsics to inline assembly.
    The former allows important optimizations while the later does not.
    As a motivating example, consider the count trailing zeros algorithm.
    On older Intel machines, this is implemented with a `bsf` instruction followed by a `cmov`
    instruction to handle the case where the input is 0. In many contexts
    the optimizer is able to prove that the input is never 0 and thus the
    cmov instruction can be omitted. These optimizations are often impossible with inline assembly.
* Implementors are encouraged to add support for wider integral types where it makes sense to do so.

Survey of Hardware Support
-----------------------------

The following is a list of compiler intrinsics and native instructions which can be used to implement
this proposal on various platforms. 
Several machine architectures were surveyed for their instruction references.
The purpose of this section is to demonstrate the current state
of the art on many different machines. We have also noted when one operation is
trivially implementable from another bitops proposal operation.
* `cntt0(x)`
 * i386: `bsf`, `cmov`
 * x86\_64 w/ BMI1: `tzcnt`
 * Alpha: `cttz`
 * gcc: `x == 0 ? sizeof(x) * CHAR_BIT : __builtin_ctz(x)`
 * bitops: `cntt1(~x)`

* `cntl0(x)`
 * i386: `bsr`, `cmov`
 * x86\_64 w AMD SSE4a / Intel BMI1: `lzcnt`
 * Alpha: `ctlz`
 * ARMv5: `CLZ`
 * IA64: `clz`
 * PowerPC: `cntlzd`
 * MIPS: `CLZ`
 * gcc: `(x == 0 ? sizeof(x) * CHAR_BIT : __builtin_clz(x))`
 * bitops: `cntl1(~x)`

* `cntt1(x)`
 * bitops: `cntt0(~x)`

* `cntl1(x)`
 * ARMv8: `CLS`
 * Blackfin: `SIGNBITS`
 * C6X: `NORM`
 * Picochip: `SBC`
 * MIPS: `CLO`
 * bitops: `cntl0(~x)`

* `popcount(x)`
 * x86\_64 SSE4: `popcnt`
 * IA64: `popcnt`
 * Alpha: `ctpop`
 * PowerPC: `popcntb`
 * SparcV9: `POPC`
 * gcc: `__builtin_popcount(x)`

* `parity(x)`
 * gcc: `__builtin_parity(x)`
 * bitops: `popcount(x) & 1`

* `rstls1b(x)`
 * x86\_64 w/ BMI1: `BLSR`

* `setls0b(x)`
 * x86\_64 w/ AMD TBM: `BLCS`

* `isols0b(x)`
 * x86\_64 w/ AMD TBM: `BLCI`, `NOT`
 * x86\_64 w/ AMD TBM: `BLCIC`

* `isols1b(x)`
 * x86\_64 w/ BMI1: `BLSI`
 * x86\_64 w/ AMD TBM: `BLSIC`, `NOT`

* `rstt1(x)`
 * x86\_64 w/ AMD TBM: `BLCFILL`

* `sett0(x)`
 * x86\_64 w/ AMD TBM: `BLSFILL`

* `maskt0(x)`
 * x86\_64 w/ AMD TBM: `TZMSK`

* `maskt1(x)`
 * x86\_64 w/ AMD TBM: `T1MSKC`, `NOT`

* `maskt0ls1b(x)`
 * x86\_64 w/ BMI1: `BLSMSK`

* `maskt1ls0b(x)`
 * x86\_64 w/ AMD TBM: `BLCMSK`

* `rstbitsge(x, b)`
 * x86\_64 w/ BMI2: `BZHI`

* `reverse_bits<uint32_t>(x)`
 * ARMv7: `RBIT`
 * EPIPHANY: `BITR`
* `reverse_bits<uint64_t>(x)`
 * ARMv8: `RBIT`
* `reverse_bits<uint8_t>(x, 4)`
 * AVR: `SWAP`
* `reverse_bytes<uint16_t>(x)`
 * PDP11: `SWAB`
 * gcc: `__builtin_bswap16(x)`
 * rotr<uint_16_t>(x, 8);
 * rotl<uint_16_t>(x, 8);
* `reverse_bytes<uint32_t>(x)`
 * i486: `bswap`
 * ARMv5: `REV`
 * gcc: `__builtin_bswap32(x)`
* `reverse_bytes<uint64_t>(x)`
 * x86\_64: `bswap`
 * ARMv8: `REV`
 * gcc: `__builtin_bswap64(x)`
* `reverse_bytes<uint32_t>(x, 1, 2)`
 * ARMv6: `REV16`
* `reverse_bytes<uint64_t>(x, 1, 4)`
 * ARMv8: `REV16`
* `reverse_bytes<uint64_t>(x, 1, 2)`
 * ARMv8: `REV32`
* `reverse_bytes<uint32_t>(x, 2)`
 * MC68020: `SWAP`
* `int32_t(reverse_bytes<int16_t>(x))`
 * ARMv5: `REVSH`
* `int32_t(reverse_bytes<uint16_t>(x))`
 * ARMv5: `REVSH`
* `deposit_bits_right(x)`
 * x86\_64 w/ BMI2: PDEP
* `extract_bits_right(x)`
 * x86\_64 w/ BMI2: PEXT
* `satadd(l, r)`
 * ARMv7: QADD
* `satsub(l, r)`
 * ARMv7: QSUB
    
Open Questions
=====================

Naming
----------------

Naming is one of the most difficult problems in software development. 
On one extreme are terse names such as `std::ctz()` for Count
Trailing Zeroes. This naming style mimics assembler mnemonics and is also
an artifact of the old days when programming languages had limits on the
length of names of identifiers.

These short names do have some merits. They reduce the amount of typing required by
the programmer and more importantly they can be used within complex expressions.
The downside is that short names can be ambiguous. Consider
a hypothetical *Count Leading Sign bits* function `std::cls()`. This name could
be interpreted in other contexts such as *CLear Screen*.

On the other extreme are verbose names such as `std::mask_least_significant_1_bit_and_trailing_zeros()`. While these names
remove all ambiguity they are cumbersome to type. They also cannot be
used easily in complex expressions with other operations.

We have opted to make a compromise. The current naming scheme adheres to the
following rules:

* All functions start with a verb. Each verb can be abbreviated to at a minimum of 3 characters. That is instead of 'ctz', we would say 'cnttz'.
* Use 0 and 1 for 0 and 1, not Z and O or some other combination of letters. While the z in `cnttz()` is pretty obviously zero, the o in `cntto()` is not so obviously meant to be 1. Therefore we stick to the numbers to remove all ambiguity.
* Nouns can be abbreviated to one character per word, as long as they are reused consistently. We reuse the following nouns:
 * t0: trailing 0s
 * t1: trailing 1s
 * l0: leading 0s
 * l1: leading 1s
 * ls1b: least significant 1 bit
 * ms1b: most significant 0 bit
 * ls0b: least significant 1 bit
 * ms0b: most significant 0 bit

As always, the naming question is continuously up for debate and reconsideration.
Some other styles have been suggested on the std-proposals discussion forum.

* `ctz()`
* `ct0()`
* `cntt0()`
* `countt0()`
* `count_t0()`
* `count_trailing_zeros()`
* `count_trailing_0_bits()`
* `count_trailing<bool>()`

Support for std::bitset
------------------------

Many people have suggesting adding support for `std::bitset`. While this is certainly a good idea, we believe that is outside of the scope of
this proposal. Once this proposal is finished and the interface is agreed upon, adding a follow up proposal for `std::bitset` would be easy to do.

Support for C
-----------------

This library would also be useful for the C community. Many of these bitwise operations
are used by embedded developers and they often choose to implement in C.  While C compatibility is a noble goal, we
do not want to make sacrifices to the C++ interface in the name of C compatibility. Particularly with regards to
templates, overloading, and `constexpr`.
This is first and foremost a C++ proposal which takes advantage of the latest C++ techniques
to provide a modern procedural interface.

If the C community shows interest we may consider a C interface that uses the generic macro feature. This
may allow interoperability, using macros for C and templates for C++. The `constexpr` qualifier
could be used in the C++ version while `inline` is used in the C version. If the C community shows interest,
we will consider a joint C proposal and flesh out the technical details of the interface and compatibility.

Acknowledgments
====================

Thank you to everyone on the std proposals forum for feedback and suggestions.
Thank you to Jasper Neumann and Harold Aptroot for their valuable insight and experience in this field and feedback about the paper.

References
==================

* <a name="BitOpsRef"></a>[BitOpsRef] *GitHub: BitOps Proposal and Reference Implementation*, (still under development) Available online at
	<https://github.com/fmatthew5876/stdcxx>
* <a name="Anderson01"></a>[Anderson01] Anderson, Sean Eron. *Bit Twiddling Hacks*, Available online at <http://graphics.stanford.edu/~seander/bithacks.html>
* <a name="Dietz01"></a>[Dietz01] Dietz, Hendry Gordon. *The Aggregate Magic Algorithms*, University of Kentucky. 
	Available online at <http://aggregate.org/MAGIC/>
* <a name="Neumann01"></a>[Neumann01] Neumann, Jasper. *Bit permutations*, Available online at
	<http://programming.sirrida.de/bit_perm.html>
* <a name="Warren01"></a>[Warren01] Warren, Henry S. Jr. *Hacker's Delight Second Edition*,
	Addison-Wesley, Oct 2012, ISBN 0-321-84268-5.
* <a name="pdp11"></a>[pdp11] *pdp11/40 processor handbook*, Digital Equipment Corporation, 1972. Available online at <http://pdos.csail.mit.edu/6.828/2005/readings/pdp11-40.pdf>
* <a name="Kostjuchenko01"></a>[Kostjuchenko01] Kostjuchenko, Dmitry. *SSE2 optimized strlen*, Available online at
	<http://www.strchr.com/sse2_optimised_strlen>
* <a name="N3646"></a>[N3646] Pratte, Robert. *Network Byte Order Conversion Document Number: N3646*.
	Available online at <http://www.open-std.org/JTC1/SC22/WG21/docs/papers/2013/n3646.pdf>
* <a name="HAKMEM"></a>[HAKMEM] *HAKMEM, AI Memo 239*, Available online at <http://www.inwap.com/pdp10/hbaker/hakmem/hakmem.html>.
* <a name="ChessProg"></a>[ChessProg] *Chess Programming WIKI*, Available online at <http://chessprogramming.wikispaces.com/>.
* <a name="Hilewitz01"></a>[Hilewitz01] Hilewitz, Yedidya and Lee, Ruby B. *Fast Bit Compression and Expansion with Parallel Extract and Parallel Deposit Instructions*, 2006.
* <a name="EmbedGuru01"></a>[EmbedGuru01] *Optimizing for the CPU / compiler / Stack Overflow*, Available online at <http://embeddedgurus.com/stack-overflow/2012/06/optimizing-for-the-cpu-compiler/>.
