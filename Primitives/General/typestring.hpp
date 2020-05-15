/*~
 * Copyright (C) 2015, 2016 George Makrydakis <george@irrequietus.eu>
 *
 * The 'typestring' header is a single header C++ library for creating types
 * to use as type parameters in template instantiations, repository available
 * at https://github.com/irrequietus/typestring. Conceptually stemming from
 * own implementation of the same thing (but in a more complicated manner to
 * be revised) in 'clause': https://github.com/irrequietus/clause.
 *
 * File subject to the terms and conditions of the Mozilla Public License v 2.0.
 * If a copy of the MPLv2 license text was not distributed with this file, you
 * can obtain it at: http://mozilla.org/MPL/2.0/.
 */

#ifndef IRQUS_TYPESTRING_HH_
#define IRQUS_TYPESTRING_HH_

#include "Definitions/Common.hpp"
namespace LD {

/*~
 * @desc A class 'storing' strings into distinct, reusable compile-time types that
 *       can be used as type parameters in a template parameter list.
 * @tprm C... : char non-type parameter pack whose ordered sequence results
 *              into a specific string.
 * @note Could have wrapped up everything in a single class, eventually will,
 *       once some compilers fix their class scope lookups! I have added some
 *       utility functions because asides being a fun little project, it is of
 *       use in certain constructs related to template metaprogramming
 *       nonetheless.
 */
    template<char... C>
    struct TypeString final
    {
    private:
        static constexpr char const   vals[sizeof...(C)+1] = { C...,'\0' };
        static constexpr unsigned int sval = sizeof...(C);
    public:

        static constexpr char const * data() noexcept
        { return &vals[0]; }

        static constexpr unsigned int size() noexcept
        { return sval; };

        static constexpr char const * cbegin() noexcept
        { return &vals[0]; }

        static constexpr char const * cend() noexcept
        { return &vals[sval]; }




    };



    template<char... C>
    constexpr char const TypeString<C...>::vals[sizeof...(C)+1];

//*~ part 1: preparing the ground, because function templates are awesome.

/*~
 * @note While it is easy to resort to constexpr strings for use in constexpr
 *       metaprogramming, what we want is to convert compile time string in situ
 *       definitions into reusable, distinct types, for use in advanced template
 *       metaprogramming techniques. We want such features because this kind of
 *       metaprogramming constitutes a pure, non-strict, untyped functional
 *       programming language with pattern matching where declarative semantics
 *       can really shine.
 *
 *       Currently, there is no feature in C++ that offers the opportunity to
 *       use strings as type parameter types themselves, despite there are
 *       several, different library implementations. This implementation is a
 *       fast, short, single-header, stupid-proof solution that works with any
 *       C++11 compliant compiler and up, with the resulting type being easily
 *       reusable throughout the code.
 *
 * @usge Just include the header and enable -std=c++11 or -std=c++14 etc, use
 *       like in the following example:
 *
 *            typestring_is("Hello!")
 *
 *       is essentially identical to the following template instantiation:
 *
 *            irqus::typestring<'H', 'e', 'l', 'l', 'o', '!'>
 *
 *       By passing -DUSE_TYPESTRING=<power of 2> during compilation, you can
 *       set the maximum length of the 'typestring' from 1 to 1024 (2^0 to 2^10).
 *       Although all preprocessor implementations tested are capable of far
 *       more with this method, exceeding this limit may cause internal compiler
 *       errors in most, with at times rather hilarious results.
 */

    template<int N, int M>
    constexpr char tygrab(char const(&c)[M]) noexcept
    { return c[N < M ? N : M-1]; }

//*~ part2: Function template type signatures for type deduction purposes. In
//          other words, exploiting the functorial nature of parameter packs
//          while mixing them with an obvious catamorphism through pattern
//          matching galore (partial ordering in this case in C++ "parlance").

    template<char... X>
    auto typoke(TypeString<X...>) // as is...
    -> TypeString<X...>;

    template<char... X, char... Y>
    auto typoke(TypeString<X...>, TypeString<'\0'>, TypeString<Y>...)
    -> TypeString<X...>;

    template<char A, char... X, char... Y>
    auto typoke(TypeString<X...>, TypeString<A>, TypeString<Y>...)
    -> decltype(typoke(TypeString<X...,A>(), TypeString<Y>()...));

    template<char... C>
    auto typeek(TypeString<C...>)
    -> decltype(typoke(TypeString<C>()...));

    template<char... A, char... B, typename... X>
    auto tycat_(TypeString<A...>, TypeString<B...>, X... x)
    -> decltype(tycat_(TypeString<A..., B...>(), x...));

    template<char... X>
    auto tycat_(TypeString<X...>)
    -> TypeString<X...>;

/*
 * Some people actually using this header as is asked me to include
 * a typestring "cat" utility given that it is easy enough to implement.
 * I have added this functionality through the template alias below. For
 * the obvious implementation, nothing more to say. All T... must be
 * of course, "typestrings".
 */
    template<typename... T>
    using tycat
    = decltype(tycat_(T()...));



    namespace Detail
    {
        template<typename T>
        struct IsTypeString: LD::Detail::IntegralConstant<bool,false>
        {

        };



        template<char ... A>
        struct IsTypeString<LD::TypeString<A...>>: LD::Detail::IntegralConstant<bool,true>
        {

        };
    }


    template<typename T>
    constexpr bool IsTypeString = LD::Detail::IsTypeString<T>::value;
} /* irqus */

template<typename CharT, CharT ...String> LD::TypeString<String...> operator""_ts()
{
    constexpr static CharT data[] = { String..., 0 };
    //static_assert(SpecialString::IsValidString(data), "not a valid string");
    return LD::TypeString<String...>();
}
//*~ part3: some necessary code generation using preprocessor metaprogramming!
//          There is functional nature in preprocessor metaprogramming as well.

/*~
 * @note Code generation block. Undoubtedly, the preprocessor implementations
 *       of both clang++ and g++ are relatively competent in producing a
 *       relatively adequate amount of boilerplate for implementing features
 *       that the language itself will probably be having as features in a few
 *       years. At times, like herein, the preprocessor is able to generate
 *       boilerplate *extremely* fast, but over a certain limit the compiler is
 *       incapable of compiling it. For the record, only certain versions of
 *       g++ where capable of going beyond 4K, so I thought of going from base
 *       16 to base 2 for USE_TYPESTRING power base. For the record, it takes
 *       a few milliseconds to generate boilerplate for several thousands worth
 *       of "string" length through such an 'fmap' like procedure.
 */

/* 2^0 = 1 */
#define TYPESTRING1(n,x) LD::tygrab<0x##n##0>(x)

/* 2^1 = 2 */
#define TYPESTRING2(n,x) LD::tygrab<0x##n##0>(x), LD::tygrab<0x##n##1>(x)

/* 2^2 = 2 */
#define TYPESTRING4(n,x) \
        LD::tygrab<0x##n##0>(x), LD::tygrab<0x##n##1>(x) \
      , LD::tygrab<0x##n##2>(x), LD::tygrab<0x##n##3>(x)

/* 2^3 = 8 */
#define TYPESTRING8(n,x) \
        LD::tygrab<0x##n##0>(x), LD::tygrab<0x##n##1>(x) \
      , LD::tygrab<0x##n##2>(x), LD::tygrab<0x##n##3>(x) \
      , LD::tygrab<0x##n##4>(x), LD::tygrab<0x##n##5>(x) \
      , LD::tygrab<0x##n##6>(x), LD::tygrab<0x##n##7>(x)

/* 2^4 = 16 */
#define TYPESTRING16(n,x) \
        LD::tygrab<0x##n##0>(x), LD::tygrab<0x##n##1>(x) \
      , LD::tygrab<0x##n##2>(x), LD::tygrab<0x##n##3>(x) \
      , LD::tygrab<0x##n##4>(x), LD::tygrab<0x##n##5>(x) \
      , LD::tygrab<0x##n##6>(x), LD::tygrab<0x##n##7>(x) \
      , LD::tygrab<0x##n##8>(x), LD::tygrab<0x##n##9>(x) \
      , LD::tygrab<0x##n##A>(x), LD::tygrab<0x##n##B>(x) \
      , LD::tygrab<0x##n##C>(x), LD::tygrab<0x##n##D>(x) \
      , LD::tygrab<0x##n##E>(x), LD::tygrab<0x##n##F>(x)

/* 2^5 = 32 */
#define TYPESTRING32(n,x) \
        TYPESTRING16(n##0,x),TYPESTRING16(n##1,x)

/* 2^6 = 64 */
#define TYPESTRING64(n,x) \
        TYPESTRING16(n##0,x), TYPESTRING16(n##1,x), TYPESTRING16(n##2,x) \
      , TYPESTRING16(n##3,x)

/* 2^7 = 128 */
#define TYPESTRING128(n,x) \
        TYPESTRING16(n##0,x), TYPESTRING16(n##1,x), TYPESTRING16(n##2,x) \
      , TYPESTRING16(n##3,x), TYPESTRING16(n##4,x), TYPESTRING16(n##5,x) \
      , TYPESTRING16(n##6,x), TYPESTRING16(n##7,x)

/* 2^8 = 256 */
#define TYPESTRING256(n,x) \
        TYPESTRING16(n##0,x), TYPESTRING16(n##1,x), TYPESTRING16(n##2,x) \
      , TYPESTRING16(n##3,x), TYPESTRING16(n##4,x), TYPESTRING16(n##5,x) \
      , TYPESTRING16(n##6,x), TYPESTRING16(n##7,x), TYPESTRING16(n##8,x) \
      , TYPESTRING16(n##9,x), TYPESTRING16(n##A,x), TYPESTRING16(n##B,x) \
      , TYPESTRING16(n##C,x), TYPESTRING16(n##D,x), TYPESTRING16(n##E,x) \
      , TYPESTRING16(n##F,x)

/* 2^9 = 512 */
#define TYPESTRING512(n,x) \
        TYPESTRING256(n##0,x), TYPESTRING256(n##1,x)

/* 2^10 = 1024 */
#define TYPESTRING1024(n,x) \
        TYPESTRING256(n##0,x), TYPESTRING256(n##1,x), TYPESTRING256(n##2,x) \
      , TYPESTRING128(n##3,x), TYPESTRING16(n##38,x), TYPESTRING16(n##39,x) \
      , TYPESTRING16(n##3A,x), TYPESTRING16(n##3B,x), TYPESTRING16(n##3C,x) \
      , TYPESTRING16(n##3D,x), TYPESTRING16(n##3E,x), TYPESTRING16(n##3F,x)

//*~ part4 : Let's give some logic with a -DUSE_TYPESTRING flag!

#ifdef USE_TYPESTRING
#if USE_TYPESTRING == 0
#define LDNamedType(x) \
    decltype(LD::typeek(LD::TypeString<TYPESTRING1(,x)>()))
#elif USE_TYPESTRING == 1
#define LDNamedType(x) \
    decltype(LD::typeek(LD::TypeString<TYPESTRING2(,x)>()))
#elif USE_TYPESTRING == 2
#define LDNamedType(x) \
    decltype(LD::typeek(LD::TypeString<TYPESTRING4(,x)>()))
#elif USE_TYPESTRING == 3
#define LDNamedType(x) \
    decltype(LD::typeek(LD::TypeString<TYPESTRING8(,x)>()))
#elif USE_TYPESTRING == 4
#define LDNamedType(x) \
    decltype(LD::typeek(LD::TypeString<TYPESTRING16(,x)>()))
#elif USE_TYPESTRING == 5
#define LDNamedType(x) \
    decltype(LD::typeek(LD::TypeString<TYPESTRING32(,x)>()))
#elif USE_TYPESTRING == 6
#define LDNamedType(x) \
    decltype(LD::typeek(LD::TypeString<TYPESTRING64(,x)>()))
#elif USE_TYPESTRING == 7
#define LDNamedType(x) \
    decltype(LD::typeek(LD::TypeString<TYPESTRING128(,x)>()))
#elif USE_TYPESTRING == 8
#define LDNamedType(x) \
    decltype(LD::typeek(LD::TypeString<TYPESTRING256(,x)>()))
#elif USE_TYPESTRING == 9
#define LDNamedType(x) \
    decltype(LD::typeek(LD::TypeString<TYPESTRING512(,x)>()))
#elif USE_TYPESTRING == 10
#define LDNamedType(x) \
    decltype(LD::typeek(LD::TypeString<TYPESTRING1024(,x)>()))
#elif USE_TYPESTRING > 10

#warning !!!: custom typestring length exceeded allowed (1024)            !!!
#warning !!!: all typestrings to default maximum typestring length of 64  !!!
#warning !!!: you can use -DUSE_TYPESTRING=<power of two> to set length   !!!

#define LDNamedType(x) \
    decltype(LD::typeek(LD::TypeString<TYPESTRING64(,x)>()))

#elif USE_TYPESTRING < 0

#warning !!!: You used USE_TYPESTRING with a negative size specified      !!!
#warning !!!: all typestrings to default maximum typestring length of 64  !!!
#warning !!!: you can use -DUSE_TYPESTRING=<power of two> to set length   !!!

#define LDNamedType(x) \
    decltype(LD::typeek(LD::TypeString<TYPESTRING64(,x)>()))

#endif
#else
#define LDNamedType(x) \
    decltype(LD::typeek(LD::TypeString<TYPESTRING64(,x)>()))
#endif
#endif /* IRQUS_TYPESTRING_HH_ */
