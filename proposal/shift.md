Explicit shift operations for C++
==========================================

* Document Number: XXXX
* Date: 2016-10-10
* Programming Language C++, Numerics Working Group
* Reply-to: Matthew Fioravante <fmatthew5876@gmail.com>

Introduction
=============================

This proposal adds support for explicit bit shift operations to the standard library.
This proposal is adapted from \[[N3864](#N3864)\].

Impact on the standard
=============================

This proposal is a pure library extension. 
It does not require any changes in the core language and does not depend on any other library extensions.
The proposal is composed entirely of free functions. The proposed functions are added to the `<cmath>` header. No new headers are introduced.
Implementation of this proposal may require platform specific support from the compiler via intrinsics.

Motivation
================

The C++ language provides an abstraction over many machines.
One of the most fundamental instructions available is the bit shift operation. One virtually every machine ever to run a C or C++ program,
bit shifts are provided as native instructions and are very fast.
They are used in many numeric algorithms. Bit shifts are also heavily used in hashing and cryptography.

Signed integers can be represented using majority of schemes including sign bit representation, 1's complement, and 2's complement.
In an attempt to be generic and support as many hardware platforms as
possible, both C and C++ technically have support for all 3 of these
representations. This is so even in the face of the practical reality
that all modern machines have been using 2's complement for decades now.

Supporting multiple signed integer representations has resulted in a technicality with respect
to operator>>() for signed integral types. In particular, right shifting a negative signed integer
has an implementation defined result.
Every modern platform known to the author uses 2's complement arithemetic, and happens to define
operator>>() to do an arithmetic right shift. In addition, most software packages in use today
have accepted this practicality and casually use operator>>() on signed integers, expecting
arithmetic shift behaviors. Despite this scenario, careful programmers with knowledge of the
standard still feel uneasy using operator>>() in what is intended to be portable code. They
would prefer to use something more explicit with a guarantee for portability.

Integer representational issues aside, sometimes we still want to be explicit with which kind of
shift we would like to perform on a variable regardless of its signedness. We may want to
perform an arithmetic shift on an unsigned value or a logical shift on a signed value.
operator>>() changes its behavior depending on the signedness of its operand. If we want to be explicit
and avoid a silent change of behavior if the type of the variable changes, we need a way to 
explicitly say which kind of shift we want.

Finally, the rotational shift has been available as a native cpu instruction on many
machines since the PDP11 \[[pdp11](#pdp11)\] was introduced in 1970. It's been 46 years since 
then and still neither C nor C++ has provided the programmer with the ability to efficiently
invoke a rotational shift on a native integer type.

To resolve all of these concerns, I propose a simple set of free functions to
explicitly expose shift operations to the programmer. These can be thought of standard
names for what would usually be implemented as compiler intrinsics or wrappers
around platform dependent calls to operator>>().

Technical Specification
====================

This is a procedural library implemented entirely using `constexpr` templated free functions
within the `<cmath>` header. These functions follow the same semantics as the builtin
bitshift operators for maximal performance. In particular shifting by a value greater
than the number of bits results in undefined behavior. In addition, each function is
qualified with `noexcept`.

cmath Header Additions
-------------------------

The following sections describe the additions to the `<cmath>` header.
Each function is defined with the 'Integral' concept, which represents both signed and unsigned integers.

### List of Functions

    //SHift Logical Left
    template <Integral T>
    constexpr T shll(T x, int s) noexcept;

* *Returns:* Shift `x` by `s` positions to the left, filling the low order bits with 0.
* *Remarks:* result is undefined if `s < 0 || s >= sizeof(x) * CHAR_BIT`

<!-- -->

    //SHift Logical Right
    template <Integral T>
    constexpr T shlr(T x, int s) noexcept;

* *Returns:* Shift `x` by `s` positions to the right, filling the high order bits with 0.
* *Remarks:* result is undefined if `s < 0 || s >= sizeof(x) * CHAR_BIT`

<!-- -->

    //SHift Arithmetic Right
    template <Integral T>
    constexpr T shar(T x, int s) noexcept;
    
* *Returns:* Shift `x` by `s` positions to the right, filling the high order bits with with the value of the original most significant bit of `x`.
* *Remarks:* result is undefined if `s < 0 || s >= sizeof(x) * CHAR_BIT`

<!-- -->

    //ROTate Left
    template <Integral T>
    constexpr T rotl(T x, int s) noexcept;

* *Returns:* `x` with all of its bits shifted left by `s` positions.
    The `s` low order bits are set to the `s` high order bits of `x`.
* *Remarks:* result is undefined if `s < 0 || s >= sizeof(x) * CHAR_BIT`

<!-- -->

    //ROTate Right
    template <Integral T>
    constexpr T rotr(T x, int s) noexcept;

* *Returns:* `x` with all of its bits shifted right by `s` positions.
    The `s` high order bits are set to the `s` low order bits of `x`.
* *Remarks:* result is undefined if `s < 0 || s >= sizeof(x) * CHAR_BIT`

Survey of Hardware Support
-----------------------------

The following is a list of compiler intrinsics and native instructions which can be used to implement
this proposal on various platforms. 
Several machine architectures were surveyed for their instruction references.

* `shll(x)`
 * PDP11: `ASL`
 * i386: `shl`
 * powerpc: `slw`
 * ARM: `LSL`
 * Itanium: `shl`
 * MIPS: `SLL`
 * Sparc: `SLL`

* `shlr(x)`
 * i386: `shr`
 * powerpc: `srw`
 * ARM: `LSR`
 * Itanium: `shr.u`
 * MIPS: `SRL`
 * Sparc: `SRL`

* `shar(x)`
 * PDP11: `ASR`
 * i386: `sar`
 * powerpc: `sraw`
 * ARM: `ASR`
 * Itanium: `shr`
 * MIPS: `SRA`
 * Sparc: `SRA`

* `rotr(x)`
 * PDP11: `ROR`
 * i386: `ror`
 * powerpc: `rotrwi`
 * ARM: `ROR`
 * Itanium: `shrp`

* `rotl(x)`
 * PDP11: `ROL`
 * i386: `rol`
 * powerpc: `rotlwi`

Acknowledgments
====================

Thank you to everyone on the std proposals forum for feedback and suggestions.

References
==================

* <a name="N3864"></a>[N3864] Fioravante, Matthew. *A constexpr bitwise operations library for C++* Available online at <http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2014/n3864.html>
* <a name="pdp11"></a>[pdp11] *pdp11/40 processor handbook*, Digital Equipment Corporation, 1972. Available online at <http://pdos.csail.mit.edu/6.828/2005/readings/pdp11-40.pdf>
