//
// Created by James Landess on 2/23/20.
//

#ifndef LANDESSDEVCORE_COMPILETIMEOPERATIONS_HPP
#define LANDESSDEVCORE_COMPILETIMEOPERATIONS_HPP


#include "Definitions/Integer.hpp"
namespace LD
{
    namespace CT
    {
        namespace Detail
        {
            template <LD::UInteger _Xp, LD::UInteger _Yp>
            struct GCD
            {
                static const constexpr LD::UInteger value = GCD<_Yp, _Xp % _Yp>::value;
            };

            template <LD::UInteger _Xp>
            struct GCD<_Xp, 0>
            {
                static const constexpr LD::UInteger value = _Xp;
            };

            template <>
            struct GCD<0, 0>
            {
                static const constexpr LD::UInteger value = 1;
            };

            // __static_lcm

            template <LD::UInteger _Xp, LD::UInteger _Yp>
            struct LCD
            {
                static const constexpr LD::UInteger value = _Xp / GCD<_Xp, _Yp>::value * _Yp;
            };

            template <LD::Integer _Xp>
            struct Abs
            {
                static const constexpr LD::Integer value = _Xp < 0 ? -_Xp : _Xp;
            };


            template <LD::Integer _Xp>
            struct Sign
            {
                static const constexpr LD::Integer value = _Xp == 0 ? 0 : (_Xp < 0 ? -1 : 1);
            };


            template<LD::UInteger A, LD::UInteger B>
            struct Subtract
            {
                static const constexpr LD::UInteger value = (B-A);
            };

            template<LD::UInteger A, LD::UInteger B>
            struct Add
            {
                static const constexpr LD::UInteger value = (A+B);
            };
            template <LD::UInteger N, LD::UInteger base=2>
            struct Log
            {
                //enum { value = 1 + LD::CT::Detail::Log<N/base, base>::value };
                static const constexpr LD::UInteger value = 1 + LD::CT::Detail::Log<N/base, base>::value;
            };

            template <LD::UInteger base>
            struct Log<1, base>
            {
                //enum { value = 0 };
                static const constexpr LD::UInteger value = 0;
            };

            template <LD::UInteger base>
            struct Log<0, base>
            {
                //enum { value = 0 };
                static const constexpr LD::UInteger value = 0;
            };




            template <LD::UInteger A, LD::UInteger B>
            struct Max
            {
                static const constexpr LD::UInteger value = (A > B)?A:B;
            };


            template <LD::UInteger A, LD::UInteger B>
            struct Pow
            {
                static const constexpr LD::UInteger value = A * LD::CT::Detail::Pow<A, B - 1>::value;
            };
            template <LD::UInteger A>
            struct Pow<A, 0>
            {
                static const LD::UInteger value = 1;
            };




            template<LD::UInteger Primary, LD::UInteger Secondary>
            struct Modulus
            {
                static const constexpr bool value = !(Primary%Secondary);
            };


            template<LD::UInteger Primary, LD::UInteger Secondary>
            struct Divide
            {
                static const constexpr LD::UInteger value = Primary/Secondary;
            };


            template<LD::UInteger Primary, LD::UInteger Secondary>
            struct Multiply
            {
                static const constexpr LD::UInteger value = Primary*Secondary;
            };


            template<LD::UInteger Size>
            struct Align
            {
                static const constexpr LD::UInteger value = (Size<2 || Size&(Size-1))?LD::CT::Detail::Pow<2, LD::CT::Detail::Log<Size,2>::value+1>::value:Size;
            };

            template<LD::UInteger Size, LD::UInteger Alignment = 8>
            struct AlignmentSize
            {
                static const constexpr LD::UInteger value = LD::CT::Detail::Modulus<Size, Alignment>::value?Size:LD::CT::Detail::Multiply<Alignment, LD::CT::Detail::Divide<Size, Alignment>::value+1>::value;
            };
        }

        template<LD::UInteger A, LD::UInteger B>
        constexpr LD::UInteger Pow = LD::CT::Detail::Pow<A,B>::value;

        template<LD::UInteger A, LD::UInteger Base>
        constexpr LD::UInteger Log = LD::CT::Detail::Log<A,Base>::value;

        template<LD::UInteger A, LD::UInteger B>
        constexpr LD::UInteger Modulus = LD::CT::Detail::Modulus<A,B>::value;

        template<LD::UInteger A, LD::UInteger B>
        constexpr LD::UInteger Multiply = LD::CT::Detail::Multiply<A,B>::value;

        template<LD::UInteger A, LD::UInteger B>
        constexpr LD::UInteger Divide = LD::CT::Detail::Divide<A,B>::value;

        template<LD::UInteger A>
        constexpr LD::UInteger Align = LD::CT::Detail::Align<A>::value;

        template<LD::UInteger Size, LD::UInteger AlignedTo>
        constexpr LD::UInteger Alignment = LD::CT::Detail::AlignmentSize<Size,AlignedTo>::value;

        template<LD::UInteger A, LD::UInteger B>
        constexpr LD::UInteger GCD = LD::CT::Detail::GCD<A,B>::value;


        template<LD::UInteger A, LD::UInteger B>
        constexpr LD::UInteger LCM = LD::CT::Detail::LCD<A,B>::value;

        template<LD::Integer A>
        constexpr LD::Integer Abs = LD::CT::Detail::Abs<A>::value;


        template<LD::Integer A>
        constexpr LD::Integer Sign = LD::CT::Detail::Sign<A>::value;




    }
}
#endif //LANDESSDEVCORE_COMPILETIMEOPERATIONS_HPP
