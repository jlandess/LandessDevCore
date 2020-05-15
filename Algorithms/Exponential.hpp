//
//  Exp.h
//  DataStructures
//
//  Created by James Landess on 11/28/16.
//  Copyright (c) 2016 James Landess. All rights reserved.
//

#ifndef __DataStructures__Exp__
#define __DataStructures__Exp__

#include <stdio.h>

#include "Definitions/Float.hpp"
#include "Definitions/Common.hpp"
#include "vdt/vdtmath.h"
#include "Primitives/General/ImproperFraction.hpp"
#include "Primitives/General/StaticArray.hpp"
#include "Functor/LightWeightDelegate.h"

namespace LD
{
    inline LD::Float Exp(const LD::Float & a)
    {
        return vdt::fast_exp(a);
    }
    inline LD::Float Ln(const LD::Float & a)
    {
        return vdt::fast_log(a);
    }
    inline LD::Float Pow(const LD::Float & a, const LD::Float & b)
    {
        return vdt::fast_exp(vdt::fast_log(a)*b);
    }

    inline LD::Float Log(const LD::Float & a, const LD::Float & base = 10)
    {
        return vdt::fast_log(a)/vdt::fast_log(base);
    }
    inline LD::Float Log10(const LD::Float & a)
    {
        return vdt::fast_log(a)/vdt::fast_log(10);
    }
    inline LD::Float Log2(const LD::Float & a)
    {
        return vdt::fast_log(a)/vdt::fast_log(2);
    }
    inline LD::Float NRoot(const LD::Float & a, const LD::Float & degree)
    {
        LD::Float usablePower = (LD::Float(1.0)/degree);

        return vdt::fast_exp(vdt::fast_log(a)*usablePower);
    }


    template<typename T, typename U>
    PDP::Enable_If_T<(PDP::IsInteger<T>::value || PDP::IsUnsignedInteger<T>::value) && (PDP::IsInteger<U>::value || PDP::IsUnsignedInteger<U>::value),T> GCD(const T & a, const U & b)
    {
        auto firstPath = [](const T & a, const U & b)
        {
            return b;
        };

        auto secondPath = [](const T & a, const U & b)
        {
            return a;
        };

        auto thirdPath = [](const T & a, const U & b)
        {
            return GCD(a, b % a);
        };

        auto fourthPath = [](const T & a, const U & b)
        {
            return GCD(b, a % b);
        };



        PDP::LightWeightDelegate<T(const T & a, const T & b)> codePaths[4];
        codePaths[0] = PDP::LightWeightDelegate<T(const T & a, const U & b)>{&firstPath, &decltype(firstPath)::operator()};
        codePaths[1] = PDP::LightWeightDelegate<T(const T & a, const U & b)>{&secondPath, &decltype(secondPath)::operator()};
        codePaths[2] = PDP::LightWeightDelegate<T(const T & a, const U & b)>{&thirdPath, &decltype(thirdPath)::operator()};
        codePaths[3] = PDP::LightWeightDelegate<T(const T & a, const U & b)>{&fourthPath, &decltype(fourthPath)::operator()};

        LD::UInteger selectedIndex = (a==0 && b!=0 )*0; + (b==0 && a!=1)*1 + (a < b && a!=0 && b!=0)*2 + (!(a< b) && a!=0 && b!=0)*3;


        if ( a == 0)
            return b;
        else if (b == 0)
            return a;

        if (a < b)
            return LD::GCD(a, b % a);
        else
            return LD::GCD(b, a % b);

        return codePaths[selectedIndex](a,b);
    }
    //PDP::IsConvertible<const char *, PDP::DataStructures::BasicString<char>>::value

    template<typename T, typename U>
    PDP::Enable_If_T<PDP::IsFloatingPoint<T>::value && PDP::IsFloatingPoint<U>::value && PDP::IsConvertible<U, T>::value && PDP::IsConvertible<T, U>::value,T> GCD(const T & a, const U & b)
    {

        using FractionType = PDP::ImProperFraction<typename PDP::UnsignedIntegerVariantForInteger<T>::UnsignedVariant>;
        FractionType first(a);
        FractionType second(b);

        auto firstCodePath = [](const T & a, const U & b, const FractionType & first, const FractionType & second)->T
        {
            return b;
        };

        auto secondCodePath = [](const T & a, const U & b, const FractionType & first, const FractionType & second)->T
        {
            return a;
        };


        auto thirdCodePath = [](const T & a, const U & b, const FractionType & firstInput, const FractionType & secondInput)->T
        {

            FractionType first = FractionType {firstInput.GetNumerator()*second.GetDenominator(),firstInput.GetDenominator()};
            FractionType second = FractionType {secondInput.GetNumerator(),second.GetNumerator()*first.GetDenominator()};
            typename PDP::UnsignedIntegerVariantForInteger<T>::UnsignedVariant resultant = LD::GCD(first.GetNumerator(), second.GetNumerator());
            return T(T(resultant)/T(first.GetDenominator()*second.GetDenominator()));
        };


        PDP::LightWeightDelegate<T(const T &, const U &, const FractionType &, const FractionType &)> codePaths[3];
        codePaths[0] = PDP::LightWeightDelegate<T(const T &, const U &, const FractionType &, const FractionType &)>{&firstCodePath, &decltype(firstCodePath)::operator()};
        codePaths[1] = PDP::LightWeightDelegate<T(const T &, const U &, const FractionType &, const FractionType &)>{&secondCodePath, &decltype(secondCodePath)::operator()};
        codePaths[2] = PDP::LightWeightDelegate<T(const T &, const U &, const FractionType &, const FractionType &)>{&thirdCodePath, &decltype(thirdCodePath)::operator()};


        if (first.GetNumerator() == 0 && first.GetDenominator() == 0)
        {
            return b;
        }else if(second.GetNumerator() == 0 && second.GetDenominator() == 0)
        {
            return a;
        }


        first.GetNumerator()*=second.GetDenominator();
        second.GetNumerator()*=first.GetDenominator();


        typename PDP::UnsignedIntegerVariantForInteger<T>::UnsignedVariant resultant = LD::GCD(first.GetNumerator(), second.GetNumerator());

        return T(T(resultant)/T(first.GetDenominator()*second.GetDenominator()));

    }


    template<typename T, typename U>
    PDP::Enable_If_T<PDP::IsFloatingPoint<T>::value && PDP::IsFloatingPoint<U>::value && PDP::IsConvertible<U, T>::value && !PDP::IsConvertible<T, U>::value,T> GCD(const T & a, const U & b)
    {

        PDP::ImProperFraction<typename PDP::UnsignedIntegerVariantForInteger<T>::UnsignedVariant> first(a);
        PDP::ImProperFraction<typename PDP::UnsignedIntegerVariantForInteger<T>::UnsignedVariant> second(b);

        if (first.GetNumerator() == 0 && first.GetDenominator() == 0)
        {
            return b;
        }else if(second.GetNumerator() == 0 && second.GetDenominator() == 0)
        {
            return a;
        }


        first.GetNumerator()*=second.GetDenominator();
        second.GetNumerator()*=first.GetDenominator();


        typename PDP::UnsignedIntegerVariantForInteger<T>::UnsignedVariant resultant = LD::GCD(first.GetNumerator(), second.GetNumerator());

        return T(T(resultant)/T(first.GetDenominator()*second.GetDenominator()));
    }

    template<typename T, typename U>
    PDP::Enable_If_T<PDP::IsFloatingPoint<T>::value && PDP::IsFloatingPoint<U>::value && !PDP::IsConvertible<U, T>::value && PDP::IsConvertible<T, U>::value,T> GCD(const T & a, const U & b)
    {

        PDP::ImProperFraction<typename PDP::UnsignedIntegerVariantForInteger<U>::UnsignedVariant> first(a);
        PDP::ImProperFraction<typename PDP::UnsignedIntegerVariantForInteger<U>::UnsignedVariant> second(b);

        if (first.GetNumerator() == 0 && first.GetDenominator() == 0)
        {
            return b;
        }else if(second.GetNumerator() == 0 && second.GetDenominator() == 0)
        {
            return a;
        }


        first.GetNumerator()*=second.GetDenominator();
        second.GetNumerator()*=first.GetDenominator();


        typename PDP::UnsignedIntegerVariantForInteger<T>::UnsignedVariant resultant = LD::GCD(first.GetNumerator(), second.GetNumerator());

        return T(T(resultant)/T(first.GetDenominator()*second.GetDenominator()));
    }

    template<typename T, typename U>
    PDP::Enable_If_T<PDP::IsConvertible<U, T>::value && PDP::IsConvertible<T, U>::value, T> LCM(const T & a, const U & b)
    {
        return (a*b)/(LD::GCD(a, b));
    }


    template<typename T, typename U>
    PDP::Enable_If_T<PDP::IsConvertible<U, T>::value && !PDP::IsConvertible<T, U>::value, T> LCM(const T & a, const U & b)
    {
        return (a*b)/(LD::GCD(a, b));
    }

    template<typename T, typename U>
    PDP::Enable_If_T<!PDP::IsConvertible<U, T>::value && PDP::IsConvertible<T, U>::value, U> LCM(const T & a, const U & b)
    {
        return (a*b)/(LD::GCD(a, b));
    }



    namespace Detail
    {
        inline float sqrt_fast(float x)
        {
            union {float f; int i; } X, Y;
            float ScOff;
            uint8_t e;

            X.f = x;
            e = (X.i >> 23);           // f.SFPbits.e;

            if(x <= 0) return(0.0f);

            ScOff = ((e & 1) != 0) ? 1.0f : 0x1.6a09e6p0;  // NOTE: If exp=EVEN, b/c (exp-127) a (EVEN - ODD) := ODD; but a (ODD - ODD) := EVEN!!

            e = ((e + 127) >> 1);                            // NOTE: If exp=ODD,  b/c (exp-127) then flr((exp-127)/2)
            X.i = (X.i & ((1uL << 23) - 1)) | (0x7F << 23);  // Mask mantissa, force exponent to zero.
            Y.i = (((uint32_t) e) << 23);

            // Error grows with square root of the exponent. Unfortunately no work around like inverse square root... :(
            // Y.f *= ScOff * (0x9.5f61ap-4 + X.f*(0x6.a09e68p-4));        // Error = +-1.78e-2 * 2^(flr(log2(x)/2))
            // Y.f *= ScOff * (0x7.2181d8p-4 + X.f*(0xa.05406p-4 + X.f*(-0x1.23a14cp-4)));      // Error = +-7.64e-5 * 2^(flr(log2(x)/2))
            // Y.f *= ScOff * (0x5.f10e7p-4 + X.f*(0xc.8f2p-4 +X.f*(-0x2.e41a4cp-4 + X.f*(0x6.441e6p-8))));     // Error =  8.21e-5 * 2^(flr(log2(x)/2))
            // Y.f *= ScOff * (0x5.32eb88p-4 + X.f*(0xe.abbf5p-4 + X.f*(-0x5.18ee2p-4 + X.f*(0x1.655efp-4 + X.f*(-0x2.b11518p-8)))));   // Error = +-9.92e-6 * 2^(flr(log2(x)/2))
            // Y.f *= ScOff * (0x4.adde5p-4 + X.f*(0x1.08448cp0 + X.f*(-0x7.ae1248p-4 + X.f*(0x3.2cf7a8p-4 + X.f*(-0xc.5c1e2p-8 + X.f*(0x1.4b6dp-8))))));   // Error = +-1.38e-6 * 2^(flr(log2(x)/2))
            // Y.f *= ScOff * (0x4.4a17fp-4 + X.f*(0x1.22d44p0 + X.f*(-0xa.972e8p-4 + X.f*(0x5.dd53fp-4 + X.f*(-0x2.273c08p-4 + X.f*(0x7.466cb8p-8 + X.f*(-0xa.ac00ep-12)))))));    // Error = +-2.9e-7 * 2^(flr(log2(x)/2))
            Y.f *= ScOff * (0x3.fbb3e8p-4 + X.f*(0x1.3b2a3cp0 + X.f*(-0xd.cbb39p-4 + X.f*(0x9.9444ep-4 + X.f*(-0x4.b5ea38p-4 + X.f*(0x1.802f9ep-4 + X.f*(-0x4.6f0adp-8 + X.f*(0x5.c24a28p-12 ))))))));   // Error = +-2.7e-6 * 2^(flr(log2(x)/2))

            return(Y.f);
        }
    }
    inline LD::Float Sqrt(const LD::Float & a)
    {
        return Detail::sqrt_fast(a);
    }
    inline LD::Float CubicRoot(const PDP::Float & a)
    {
        LD::Float usablePower = PDP::Float(1.0)/LD::Float(3.0);

        return vdt::fast_exp(vdt::fast_log(a)*usablePower);
    }
    inline LD::UInteger Ceil(const LD::Float & x)
    {
        if(x<0)return (PDP::UInteger)x;
        return ((int)x)+1;
    }
    //extern PDP::Float Floor(const PDP::Float & a,const PDP::Precision & precision = PDP::eHIgh);
    inline LD::Float Radians(const LD::Float & degrees)
    {
        return degrees*57.2958;
    }
    inline LD::Float Degrees(const LD::Float & radians)
    {
        return radians/57.2958;
    }
    inline LD::Float Fmod(const LD::Float & a, const LD::Float & b)
    {
        return (LD::Integer)((((a/b)-((LD::Integer)(a/b)))*b)+LD::Float(0.5));
    }
    inline LD::Float Fraction(const PDP::Float & a)
    {
        return a -  (PDP::UInteger)a;
    }

    template<typename T, typename U>
    inline const T & Max(const T & a, const U & b)
    {

        //return (a>b)?a:b;
        return (a>b)*a + (!(a>b))*b;
    }

    template<typename T, typename U, typename V>
    inline const T & Max(const T & a, const U & b, const V & c)
    {

        T primaryResult = (a>b)*a + (!(a>b))*b;
        return (c > primaryResult)*c + (!(c> primaryResult))*primaryResult;
        //return (a>b)?(a>c)?a:c:(b>c)?b:c;
    }



    template<typename T, typename U>
    inline const T & Min(const T & a, const U & b)
    {

        return (a>b)*a + (!(a > b))*b;
    }

    template<typename T, typename U, typename V>
    inline const T & Min(const T & a, const U & b, const V & c)
    {
        T primaryResult = (a < b)*a + (!(a<b))*b;

        //return (a<b)?(a<c)?a:c:(b<c)?b:c;

        return (primaryResult < c)*primaryResult + (!(primaryResult < c))*c;
    }
    template<typename T, typename U, typename V, typename W>
    inline const T & Min(const T & a, const U & b, const V & c, const W & d)
    {
        T primaryResult = (a < b)*a + (!(a<b))*b;
        T secondaryResult = (primaryResult < c)*primaryResult + (!(primaryResult < c))*c;

        return (secondaryResult < d)*secondaryResult + ((!secondaryResult < d))*d;
        //return (a<b)?(a<c)?(a<d)?a:d:(c<d)?c:d:(b<c)?(b<d)?b:d:(c<d)?c:d;
    }


    template <typename T>
    inline const T & Clamp(const T& n, const T& lower, const T& upper)
    {
        return LD::Max(lower, LD::Min(n, upper));
    }


    template<typename T>
    inline PDP::Enable_If_T<PDP::IsFloatingPoint<T>::value,T> Round(const T & number)
    {
        LD::UInteger temp = number;
        T floatingPoint = number-temp;

        return (floatingPoint >= 0.5)?number+1:number;
    }

    template<typename T>
    inline PDP::Enable_If_T<PDP::IsInteger<T>::value || PDP::IsUnsignedInteger<T>::value,T> Round(const T & number)
    {
        return number;
    }

    //extern PDP::Float Signum(const PDP::Float & t);
    //extern PDP::Float IsNan(const PDP::Float & a);
    //extern PDP::Float IsInfinite(const PDP::Float & a);
    inline const bool IsNegative(const LD::Float & a)
    {
        return a < 0;
    }

    template<typename T, typename U>
    T NCR(const T & n, const U & r)

    {
        if(r > n / 2) r = n - r; // because C(n, r) == C(n, n - r)
        T ans = 1;
        T i;

        for(i = 1; i <= r; i++)
        {
            ans *= n - r + i;
            ans /= i;
        }

        return ans;
    }

    template<typename T>
    const T Factorial(const T & a)
    {
        T number = 1;

        for (T n = 1; n<=a; ++n)
        {
            number = number*n;
        }
        return number;
    }


    template<typename T, typename U>
    static T NPR(const T & n, const U & r)
    {
        return LD::NCR(n, r) * LD::Factorial(r);
    }
}
#endif /* defined(__DataStructures__Exp__) */
