//
//  Fraction.h
//  DataStructures
//
//  Created by James Landess on 3/11/17.
//  Copyright (c) 2017 James Landess. All rights reserved.
//

#ifndef DataStructures_Fraction_h
#define DataStructures_Fraction_h

#include "Definitions/Common.h"
namespace PDP
{

    template<typename T, class = void>
    class ImProperFraction
    {

    };


    //355/113 is pi as a fraction
    template<typename T>
    class ImProperFraction<T,PDP::Enable_If_T<PDP::IsInteger<T>::value || PDP::IsUnsignedInteger<T>::value>>
    {
    private:
        PDP::Integer Numerator;
        PDP::UInteger Denominator;
        //bool NegativeStatus;
        PDP::UInteger gcd(const PDP::UInteger & a, const PDP::UInteger & b) const
        {
            if (a == 0)
                return b;
            else if (b == 0)
                return a;

            if (a < b)
                return gcd(a, b % a);
            else
                return gcd(b, a % b);
        }

        PDP::UInteger lcm(const PDP::UInteger & a, const PDP::UInteger & b) const
        {
            return (a*b)/(gcd(a, b));
        }
        void rat_approx(const PDP::Float & input, const PDP::Float & md, PDP::UInteger *num, PDP::UInteger *denom)
        {
            PDP::Float f = input;

            /*  a: continued fraction coefficients. */


            PDP::UInteger a, h[3] = { 0, 1, 0 }, k[3] = { 1, 0, 0 };


            PDP::UInteger x, d, n = 1;


            PDP::Integer i, neg = 0;

            if (md <= 1) { *denom = 1; *num = (PDP::UInteger) f; return; }

            if (f < 0) { neg = 1; f = -f; }

            while (f != PDP::Floor(f)) { n <<= 1; f *= 2; }


            d = f;

            /* continued fraction and check denominator each step */
            for (i = 0; i < 64; i++)
            {
                a = n ? d / n : 0;
                if (i && !a) break;

                x = d; d = n; n = x % n;

                x = a;
                if (k[1] * a + k[0] >= md)
                {
                    x = (md - k[0]) / k[1];
                    if (x * 2 >= a || k[1] >= md)
                        i = 65;
                    else
                        break;
                }

                h[2] = x * h[1] + h[0]; h[0] = h[1]; h[1] = h[2];
                k[2] = x * k[1] + k[0]; k[0] = k[1]; k[1] = k[2];
            }
            *denom = k[1];
            *num = neg ? -h[1] : h[1];
        }




    public:


        const bool IsZero() const {return this->Numerator != 0;};
        //static ImProperFraction<T> PI = ImProperFraction<T>(355,113);


        inline ImProperFraction():Numerator(0),Denominator(0)
        {

        }


        ImProperFraction operator-() const
        {
            return ImProperFraction(-1*this->Numerator,this->Denominator);
        }
        /*
         void rat_approx(const PDP::Float & input, const PDP::Float & md, PDP::Integer * num, PDP::UInteger *denom)
         {
         PDP::Float f = input;
        PDP::UInteger a, h[3] = { 0, 1, 0 }, k[3] = { 1, 0, 0 };
        PDP::UInteger x, d, n = 1;

        PDP::Integer i, neg = 0;

        if (md <= 1) { *denom = 1; *num = (PDP::UInteger) f; return; }

        if (f < 0) { neg = 1; f = -f; }

        while (f != PDP::Floor(f)) { n <<= 1; f *= 2; }
            d = f;

            for (i = 0; i < 64; i++) {
                a = n ? d / n : 0;
                if (i && !a) break;

                x = d; d = n; n = x % n;

                x = a;
                if (k[1] * a + k[0] >= md) {
                    x = (md - k[0]) / k[1];
                    if (x * 2 >= a || k[1] >= md)
                        i = 65;
                    else
                        break;
                }

                h[2] = x * h[1] + h[0]; h[0] = h[1]; h[1] = h[2];
                k[2] = x * k[1] + k[0]; k[0] = k[1]; k[1] = k[2];
            }
            *denom = k[1];
            *num = neg ? -h[1] : h[1];
            }
         */

        void rat_approxz(const PDP::Float & input, const PDP::UInteger & md, PDP::Integer * num, PDP::UInteger *denom)
        {
            PDP::Float f = input;
            PDP::UInteger a, h[3] = { 0, 1, 0 }, k[3] = { 1, 0, 0 };
            PDP::UInteger x, d, n = 1;

            PDP::Integer i, neg = 0;

            if (md <= 1) { *denom = 1; *num = (PDP::UInteger) f; return; }

            if (f < 0) { neg = 1; f = -f; }

            while (f != PDP::Floor(f)) { n <<= 1; f *= 2; }
            d = f;

            for (i = 0; i < 64; i++) {
                a = n ? d / n : 0;
                if (i && !a) break;

                x = d; d = n; n = x % n;

                x = a;
                if (k[1] * a + k[0] >= md) {
                    x = (md - k[0]) / k[1];
                    if (x * 2 >= a || k[1] >= md)
                        i = 65;
                    else
                        break;
                }

                h[2] = x * h[1] + h[0]; h[0] = h[1]; h[1] = h[2];
                k[2] = x * k[1] + k[0]; k[0] = k[1]; k[1] = k[2];
            }
            *denom = k[1];
            *num = neg ? -h[1] : h[1];
        }
        inline ImProperFraction(const PDP::Float & input)
        {
            rat_approxz(input, 1000, &this->Numerator, &this->Denominator);

            //this->rat_approx(PDP::Abs(input),10000,&this->Numerator,&this->Denominator);
        }
        inline ImProperFraction(const PDP::UInteger & numerator, const PDP::UInteger & denominator):Numerator(numerator),Denominator(denominator)
        {

        }

        /*
         const double EXP_LIMIT = 708;

         const double PX1exp = 1.26177193074810590878E-4; <=> 0.000126177193074810590878
         const double PX2exp = 3.02994407707441961300E-2; <=> 0.0302994407707441961300
         const double PX3exp = 9.99999999999999999910E-1; <=> 0.999999999999999999910
         const double QX1exp = 3.00198505138664455042E-6;
         const double QX2exp = 2.52448340349684104192E-3;
         const double QX3exp = 2.27265548208155028766E-1;
         const double QX4exp = 2.00000000000000000009E0;

         const double LOG2E = 1.4426950408889634073599; // 1/log(2)

         const float MAXLOGF = 88.72283905206835f;
         const float MINLOGF = -88.f;

         const float C1F =   0.693359375f;
         const float C2F =  -2.12194440e-4f;

         const float PX1expf = 1.9875691500E-4f;
         const float PX2expf =1.3981999507E-3f;
         const float PX3expf =8.3334519073E-3f;
         const float PX4expf =4.1665795894E-2f;
         const float PX5expf =1.6666665459E-1f;
         const float PX6expf =5.0000001201E-1f;

         const float LOG2EF = 1.44269504088896341f;
         */

        static ImProperFraction<T> PI()
        {
            return ImProperFraction<T>(245850922,78256779);
        }



        static ImProperFraction<T> E()
        {
            return ImProperFraction<T>(1084483,398959);
        }

        static ImProperFraction<T> C()
        {
            return ImProperFraction<T>(299792458,1);
        }

        ImProperFraction & Simplify()
        {
            T commonDivisor = gcd(this->Numerator, this->Denominator);

            this->Numerator/=commonDivisor;
            this->Denominator/=commonDivisor;

            return (*this);
        }
        const bool operator == (const ImProperFraction & fraction) const
        {
            T leastCommonMultiple = lcm(this->Denominator,fraction.Denominator);


            PDP::UInteger firstMultiplier = leastCommonMultiple/this->Denominator;
            PDP::UInteger secondMultiplier = leastCommonMultiple/fraction.Denominator;



            return this->Numerator*firstMultiplier == fraction.Numerator*secondMultiplier;
        }

        const bool operator > (const ImProperFraction & fraction) const
        {
            T leastCommonMultiple = lcm(this->Denominator,fraction.Denominator);


            PDP::UInteger firstMultiplier = leastCommonMultiple/this->Denominator;
            PDP::UInteger secondMultiplier = leastCommonMultiple/fraction.Denominator;



            return this->Numerator*firstMultiplier > fraction.Numerator*secondMultiplier;
        }

        const bool operator < (const ImProperFraction & fraction) const
        {
            T leastCommonMultiple = lcm(this->Denominator,fraction.Denominator);


            PDP::UInteger firstMultiplier = leastCommonMultiple/this->Denominator;
            PDP::UInteger secondMultiplier = leastCommonMultiple/fraction.Denominator;



            return this->Numerator*firstMultiplier > fraction.Numerator*secondMultiplier;
        }

        const bool operator != (const ImProperFraction & fraction) const
        {
            return !((*this) == fraction);
        }

        ImProperFraction Simplify() const
        {
            ImProperFraction fraction;
            T commonDivisor = gcd(this->Numerator, this->Denominator);
            fraction.Numerator = this->Numerator/commonDivisor;
            fraction.Denominator = this->Denominator/commonDivisor;

            return fraction;

        }

        inline ImProperFraction & operator = (const T & input)
        {
            this->NegativeStatus = (input < 0);
            this->rat_approx(PDP::Abs(input),10000,&this->Numerator,&this->Denominator);

            return (*this);
        }

        const PDP::Integer &  GetNumerator() const
        {
            return this->Numerator;
        }

        PDP::Integer &  GetNumerator()
        {
            return this->Numerator;
        }
        const PDP::UInteger & GetDenominator() const
        {
            return this->Denominator;
        }

        PDP::UInteger & GetDenominator()
        {
            return this->Denominator;
        }

        inline const T GetFloatingPointRepresentation() const
        {
            T floatingPointNumber = T(this->Numerator)/T(this->Denominator);

            return floatingPointNumber;
        }

        const bool IsNegative() const
        {
            return this->NegativeStatus == true;
        }

        template<typename _T = T, typename U>
        inline PDP::ImProperFraction<T> operator + (const PDP::ImProperFraction<U> & improperFraction) const
        {
            const T & currentNumerator = this->Numerator;
            const T & currentDenominator = this->Denominator;

            const T  incomingNumerator = improperFraction.Numerator;
            const T  incomingDenominator = improperFraction.Denominator;




            T denominator = currentDenominator*incomingDenominator;

            T numerator =  currentNumerator*incomingDenominator + incomingNumerator*currentDenominator;




            return PDP::ImProperFraction<U,PDP::Enable_If_T<PDP::IsInteger<T>::value || PDP::IsUnsignedInteger<T>::value>>(numerator,denominator);
        }


        template<typename _T = T, typename U>
        inline PDP::ImProperFraction<T> operator - (const PDP::ImProperFraction<U> & improperFraction) const
        {
            const T & currentNumerator = this->Numerator;
            const T & currentDenominator = this->Denominator;

            const T  incomingNumerator = improperFraction.Numerator;
            const T  incomingDenominator = improperFraction.Denominator;

            T denominator = currentDenominator*incomingDenominator;

            T numerator = currentNumerator*incomingDenominator - incomingNumerator*currentDenominator;




            return  PDP::ImProperFraction<U,PDP::Enable_If_T<PDP::IsInteger<T>::value || PDP::IsUnsignedInteger<T>::value>>(numerator,denominator);
        }


        template<typename _T = T, typename U>
        inline PDP::Enable_If_T<PDP::IsConvertible<U, _T>::value,PDP::ImProperFraction<_T,PDP::Enable_If_T<PDP::IsInteger<T>::value || PDP::IsUnsignedInteger<T>::value>>> operator * (const PDP::ImProperFraction<U,PDP::Enable_If_T<PDP::IsInteger<T>::value || PDP::IsUnsignedInteger<T>::value>> & improperFraction) const
        {



            return  PDP::ImProperFraction<_T,PDP::Enable_If_T<PDP::IsInteger<T>::value || PDP::IsUnsignedInteger<T>::value>>(this->Numerator*improperFraction.Numerator,this->Denominator*improperFraction.Denominator);
        }


        inline ImProperFraction<T> operator << (const T & shift) const
        {
            return PDP::ImProperFraction<T>(this->Numerator*power(2, shift),this->Denominator);
        }



        inline ImProperFraction<T> operator >> (const T & shift) const
        {
            return PDP::ImProperFraction<T>(this->Numerator,this->Denominator*power(2, shift));
        }
        /*
        inline ImProperFraction<T> operator << (const ImProperFraction<T> & shift)
        {

        }
         */
        template<typename _T = T, typename U>
        inline PDP::Enable_If_T<PDP::IsConvertible<U, _T>::value,PDP::ImProperFraction<_T,PDP::Enable_If_T<PDP::IsInteger<T>::value || PDP::IsUnsignedInteger<T>::value>>> operator / (const PDP::ImProperFraction<U,PDP::Enable_If_T<PDP::IsInteger<T>::value || PDP::IsUnsignedInteger<T>::value>> & improperFraction) const
        {

            return  PDP::ImProperFraction<U,PDP::Enable_If_T<PDP::IsInteger<T>::value || PDP::IsUnsignedInteger<T>::value>>(this->Numerator*improperFraction.Denominator,this->Denominator*improperFraction.Numerator);
        }


        template<typename _T = T, typename U>
        inline PDP::Enable_If_T<PDP::IsConvertible<U, _T>::value,PDP::ImProperFraction<_T>&> operator += (const PDP::ImProperFraction<U,PDP::Enable_If_T<PDP::IsInteger<T>::value || PDP::IsUnsignedInteger<T>::value>> & improperFraction)
        {
            (*this) = (*this)+improperFraction;
            return (*this);
        }
        template<typename _T = T, typename U>
        inline PDP::Enable_If_T<PDP::IsConvertible<U, _T>::value,PDP::ImProperFraction<_T>&> operator -= (const PDP::ImProperFraction<U,PDP::Enable_If_T<PDP::IsInteger<T>::value || PDP::IsUnsignedInteger<T>::value>> & improperFraction)
        {
            (*this) = (*this)-improperFraction;
            return (*this);
        }
        template<typename _T = T, typename U>
        inline PDP::Enable_If_T<PDP::IsConvertible<U, _T>::value,PDP::ImProperFraction<_T>&> operator *= (const PDP::ImProperFraction<U,PDP::Enable_If_T<PDP::IsInteger<T>::value || PDP::IsUnsignedInteger<T>::value>> & improperFraction)
        {
            (*this) = (*this)*improperFraction;
            return (*this);
        }
        template<typename _T = T, typename U>
        inline PDP::Enable_If_T<PDP::IsConvertible<U, _T>::value,PDP::ImProperFraction<_T>&> operator /= (const PDP::ImProperFraction<U,PDP::Enable_If_T<PDP::IsInteger<T>::value || PDP::IsUnsignedInteger<T>::value>> & improperFraction)
        {
            (*this) = (*this)/improperFraction;
            return (*this);
        }


        inline PDP::ImProperFraction<T,PDP::Enable_If_T<PDP::IsInteger<T>::value || PDP::IsUnsignedInteger<T>::value>> Sin() const;


        inline PDP::ImProperFraction<T,PDP::Enable_If_T<PDP::IsInteger<T>::value || PDP::IsUnsignedInteger<T>::value>> Cos() const;



        inline PDP::ImProperFraction<T,PDP::Enable_If_T<PDP::IsInteger<T>::value || PDP::IsUnsignedInteger<T>::value>> Tan() const;


        inline PDP::ImProperFraction<T,PDP::Enable_If_T<PDP::IsInteger<T>::value || PDP::IsUnsignedInteger<T>::value>> Sec() const;


        inline PDP::ImProperFraction<T,PDP::Enable_If_T<PDP::IsInteger<T>::value || PDP::IsUnsignedInteger<T>::value>> Csc() const;


        inline PDP::ImProperFraction<T,PDP::Enable_If_T<PDP::IsInteger<T>::value || PDP::IsUnsignedInteger<T>::value>> CoTan() const;



        inline PDP::ImProperFraction<T,PDP::Enable_If_T<PDP::IsInteger<T>::value || PDP::IsUnsignedInteger<T>::value>> ASin() const;

        inline PDP::ImProperFraction<T,PDP::Enable_If_T<PDP::IsInteger<T>::value || PDP::IsUnsignedInteger<T>::value>> ACos() const;

        inline PDP::ImProperFraction<T,PDP::Enable_If_T<PDP::IsInteger<T>::value || PDP::IsUnsignedInteger<T>::value>> ATan() const;

        inline PDP::ImProperFraction<T,PDP::Enable_If_T<PDP::IsInteger<T>::value || PDP::IsUnsignedInteger<T>::value>> ASec() const;

        inline PDP::ImProperFraction<T,PDP::Enable_If_T<PDP::IsInteger<T>::value || PDP::IsUnsignedInteger<T>::value>> ACsc() const;

        inline PDP::ImProperFraction<T,PDP::Enable_If_T<PDP::IsInteger<T>::value || PDP::IsUnsignedInteger<T>::value>> ACoTan() const;


        inline PDP::ImProperFraction<T,PDP::Enable_If_T<PDP::IsInteger<T>::value || PDP::IsUnsignedInteger<T>::value>> Sinh() const;


        inline PDP::ImProperFraction<T,PDP::Enable_If_T<PDP::IsInteger<T>::value || PDP::IsUnsignedInteger<T>::value>> Cosh() const;


        inline PDP::ImProperFraction<T,PDP::Enable_If_T<PDP::IsInteger<T>::value || PDP::IsUnsignedInteger<T>::value>> Tanh() const;

        inline PDP::ImProperFraction<T,PDP::Enable_If_T<PDP::IsInteger<T>::value || PDP::IsUnsignedInteger<T>::value>> Sech() const;


        inline PDP::ImProperFraction<T,PDP::Enable_If_T<PDP::IsInteger<T>::value || PDP::IsUnsignedInteger<T>::value>> Csch() const;


        inline PDP::ImProperFraction<T,PDP::Enable_If_T<PDP::IsInteger<T>::value || PDP::IsUnsignedInteger<T>::value>> CoTanh() const;

        inline PDP::ImProperFraction<T,PDP::Enable_If_T<PDP::IsInteger<T>::value || PDP::IsUnsignedInteger<T>::value>> Sqrt() const;


        inline PDP::ImProperFraction<T,PDP::Enable_If_T<PDP::IsInteger<T>::value || PDP::IsUnsignedInteger<T>::value>> Exp() const;


        inline PDP::ImProperFraction<T,PDP::Enable_If_T<PDP::IsInteger<T>::value || PDP::IsUnsignedInteger<T>::value>> Floor() const;


        inline PDP::ImProperFraction<T,PDP::Enable_If_T<PDP::IsInteger<T>::value || PDP::IsUnsignedInteger<T>::value>> Ceil() const;

        inline PDP::ImProperFraction<T,PDP::Enable_If_T<PDP::IsInteger<T>::value || PDP::IsUnsignedInteger<T>::value>> Ln() const;

        inline PDP::ImProperFraction<T,PDP::Enable_If_T<PDP::IsInteger<T>::value || PDP::IsUnsignedInteger<T>::value>> Log10() const;

        inline PDP::ImProperFraction<T,PDP::Enable_If_T<PDP::IsInteger<T>::value || PDP::IsUnsignedInteger<T>::value>> Log2() const;


        template< typename U>
        inline PDP::ImProperFraction<T,PDP::Enable_If_T<PDP::IsInteger<T>::value || PDP::IsUnsignedInteger<T>::value>> Pow(const PDP::ImProperFraction<U,PDP::Enable_If_T<PDP::IsInteger<T>::value || PDP::IsUnsignedInteger<T>::value>> & improperFraction) const;


        template<typename U>
        inline PDP::ImProperFraction<T,PDP::Enable_If_T<PDP::IsInteger<T>::value || PDP::IsUnsignedInteger<T>::value>> Tan2(const PDP::ImProperFraction<U,PDP::Enable_If_T<PDP::IsInteger<T>::value || PDP::IsUnsignedInteger<T>::value>> & improperFraction) const;

    };

    //355/113 is pi as a fraction
    template<typename T>
    inline PDP::ImProperFraction<T,PDP::Enable_If_T<PDP::IsInteger<T>::value || PDP::IsUnsignedInteger<T>::value>> Sin(const PDP::ImProperFraction<T,PDP::Enable_If_T<PDP::IsInteger<T>::value || PDP::IsUnsignedInteger<T>::value>> & improperFraction);
    template<typename T>
    inline PDP::ImProperFraction<T,PDP::Enable_If_T<PDP::IsInteger<T>::value || PDP::IsUnsignedInteger<T>::value>> Cos(const PDP::ImProperFraction<T,PDP::Enable_If_T<PDP::IsInteger<T>::value || PDP::IsUnsignedInteger<T>::value>> & improperFraction);
    template<typename T>
    inline PDP::ImProperFraction<T,PDP::Enable_If_T<PDP::IsInteger<T>::value || PDP::IsUnsignedInteger<T>::value>> Tan(const PDP::ImProperFraction<T,PDP::Enable_If_T<PDP::IsInteger<T>::value || PDP::IsUnsignedInteger<T>::value>> & improperFraction);


    template<typename T>
    inline PDP::ImProperFraction<T,PDP::Enable_If_T<PDP::IsInteger<T>::value || PDP::IsUnsignedInteger<T>::value>> Csc(const PDP::ImProperFraction<T,PDP::Enable_If_T<PDP::IsInteger<T>::value || PDP::IsUnsignedInteger<T>::value>> & improperFraction);
    template<typename T>
    inline PDP::ImProperFraction<T,PDP::Enable_If_T<PDP::IsInteger<T>::value || PDP::IsUnsignedInteger<T>::value>> Sec(const PDP::ImProperFraction<T,PDP::Enable_If_T<PDP::IsInteger<T>::value || PDP::IsUnsignedInteger<T>::value>> & improperFraction);
    template<typename T>
    inline PDP::ImProperFraction<T,PDP::Enable_If_T<PDP::IsInteger<T>::value || PDP::IsUnsignedInteger<T>::value>> CoTan(const PDP::ImProperFraction<T,PDP::Enable_If_T<PDP::IsInteger<T>::value || PDP::IsUnsignedInteger<T>::value>> & improperFraction);



    template<typename T>
    inline PDP::ImProperFraction<T,PDP::Enable_If_T<PDP::IsInteger<T>::value || PDP::IsUnsignedInteger<T>::value>> ASin(const PDP::ImProperFraction<T,PDP::Enable_If_T<PDP::IsInteger<T>::value || PDP::IsUnsignedInteger<T>::value>> & improperFraction);
    template<typename T>
    inline PDP::ImProperFraction<T,PDP::Enable_If_T<PDP::IsInteger<T>::value || PDP::IsUnsignedInteger<T>::value>> ACos(const PDP::ImProperFraction<T,PDP::Enable_If_T<PDP::IsInteger<T>::value || PDP::IsUnsignedInteger<T>::value>> & improperFraction);
    template<typename T>
    inline PDP::ImProperFraction<T,PDP::Enable_If_T<PDP::IsInteger<T>::value || PDP::IsUnsignedInteger<T>::value>> ATan(const PDP::ImProperFraction<T,PDP::Enable_If_T<PDP::IsInteger<T>::value || PDP::IsUnsignedInteger<T>::value>> & improperFraction);

    template<typename T>
    inline PDP::ImProperFraction<T,PDP::Enable_If_T<PDP::IsInteger<T>::value || PDP::IsUnsignedInteger<T>::value>> ASec(const PDP::ImProperFraction<T,PDP::Enable_If_T<PDP::IsInteger<T>::value || PDP::IsUnsignedInteger<T>::value>> & improperFraction);
    template<typename T>
    inline PDP::ImProperFraction<T,PDP::Enable_If_T<PDP::IsInteger<T>::value || PDP::IsUnsignedInteger<T>::value>> ACsc(const PDP::ImProperFraction<T,PDP::Enable_If_T<PDP::IsInteger<T>::value || PDP::IsUnsignedInteger<T>::value>> & improperFraction);
    template<typename T>
    inline PDP::ImProperFraction<T,PDP::Enable_If_T<PDP::IsInteger<T>::value || PDP::IsUnsignedInteger<T>::value>> ACoTan(const PDP::ImProperFraction<T,PDP::Enable_If_T<PDP::IsInteger<T>::value || PDP::IsUnsignedInteger<T>::value>> & improperFraction);



    template<typename T>
    inline PDP::ImProperFraction<T,PDP::Enable_If_T<PDP::IsInteger<T>::value || PDP::IsUnsignedInteger<T>::value>> Sinh(const PDP::ImProperFraction<T,PDP::Enable_If_T<PDP::IsInteger<T>::value || PDP::IsUnsignedInteger<T>::value>> & improperFraction);
    template<typename T>
    inline PDP::ImProperFraction<T,PDP::Enable_If_T<PDP::IsInteger<T>::value || PDP::IsUnsignedInteger<T>::value>> Cosh(const PDP::ImProperFraction<T,PDP::Enable_If_T<PDP::IsInteger<T>::value || PDP::IsUnsignedInteger<T>::value>> & improperFraction);
    template<typename T>
    inline PDP::ImProperFraction<T,PDP::Enable_If_T<PDP::IsInteger<T>::value || PDP::IsUnsignedInteger<T>::value>> Tanh(const PDP::ImProperFraction<T,PDP::Enable_If_T<PDP::IsInteger<T>::value || PDP::IsUnsignedInteger<T>::value>> & improperFraction);


    template<typename T>
    inline PDP::ImProperFraction<T,PDP::Enable_If_T<PDP::IsInteger<T>::value || PDP::IsUnsignedInteger<T>::value>> Csch(const PDP::ImProperFraction<T,PDP::Enable_If_T<PDP::IsInteger<T>::value || PDP::IsUnsignedInteger<T>::value>> & improperFraction);
    template<typename T>
    inline PDP::ImProperFraction<T,PDP::Enable_If_T<PDP::IsInteger<T>::value || PDP::IsUnsignedInteger<T>::value>> Sech(const PDP::ImProperFraction<T,PDP::Enable_If_T<PDP::IsInteger<T>::value || PDP::IsUnsignedInteger<T>::value>> & improperFraction);
    template<typename T>
    inline PDP::ImProperFraction<T,PDP::Enable_If_T<PDP::IsInteger<T>::value || PDP::IsUnsignedInteger<T>::value>> CoTanh(const PDP::ImProperFraction<T,PDP::Enable_If_T<PDP::IsInteger<T>::value || PDP::IsUnsignedInteger<T>::value>> & improperFraction);




    template<typename T>
    inline PDP::ImProperFraction<T,PDP::Enable_If_T<PDP::IsInteger<T>::value || PDP::IsUnsignedInteger<T>::value>> ASinh(const PDP::ImProperFraction<T,PDP::Enable_If_T<PDP::IsInteger<T>::value || PDP::IsUnsignedInteger<T>::value>> & improperFraction);
    template<typename T>
    inline PDP::ImProperFraction<T,PDP::Enable_If_T<PDP::IsInteger<T>::value || PDP::IsUnsignedInteger<T>::value>> ACosh(const PDP::ImProperFraction<T,PDP::Enable_If_T<PDP::IsInteger<T>::value || PDP::IsUnsignedInteger<T>::value>> & improperFraction);
    template<typename T>
    inline PDP::ImProperFraction<T,PDP::Enable_If_T<PDP::IsInteger<T>::value || PDP::IsUnsignedInteger<T>::value>> ATanh(const PDP::ImProperFraction<T,PDP::Enable_If_T<PDP::IsInteger<T>::value || PDP::IsUnsignedInteger<T>::value>> & improperFraction);


    template<typename T>
    inline PDP::ImProperFraction<T,PDP::Enable_If_T<PDP::IsInteger<T>::value || PDP::IsUnsignedInteger<T>::value>> ACsch(const PDP::ImProperFraction<T,PDP::Enable_If_T<PDP::IsInteger<T>::value || PDP::IsUnsignedInteger<T>::value>> & improperFraction);
    template<typename T>
    inline PDP::ImProperFraction<T,PDP::Enable_If_T<PDP::IsInteger<T>::value || PDP::IsUnsignedInteger<T>::value>> ASech(const PDP::ImProperFraction<T,PDP::Enable_If_T<PDP::IsInteger<T>::value || PDP::IsUnsignedInteger<T>::value>> & improperFraction);


    template<typename T>
    inline PDP::ImProperFraction<T,PDP::Enable_If_T<PDP::IsInteger<T>::value || PDP::IsUnsignedInteger<T>::value>> ACoTanh(const PDP::ImProperFraction<T,PDP::Enable_If_T<PDP::IsInteger<T>::value || PDP::IsUnsignedInteger<T>::value>> & improperFraction);


    template<typename T>
    inline PDP::ImProperFraction<T,PDP::Enable_If_T<PDP::IsInteger<T>::value || PDP::IsUnsignedInteger<T>::value>> Ln(const PDP::ImProperFraction<T,PDP::Enable_If_T<PDP::IsInteger<T>::value || PDP::IsUnsignedInteger<T>::value>> & improperFraction);

    template<typename T>
    inline PDP::ImProperFraction<T,PDP::Enable_If_T<PDP::IsInteger<T>::value || PDP::IsUnsignedInteger<T>::value>> Log10(const PDP::ImProperFraction<T,PDP::Enable_If_T<PDP::IsInteger<T>::value || PDP::IsUnsignedInteger<T>::value>> & improperFraction);

    template<typename T>
    inline PDP::ImProperFraction<T,PDP::Enable_If_T<PDP::IsInteger<T>::value || PDP::IsUnsignedInteger<T>::value>> Log2(const PDP::ImProperFraction<T,PDP::Enable_If_T<PDP::IsInteger<T>::value || PDP::IsUnsignedInteger<T>::value>> & improperFraction);


    template<typename T>
    inline PDP::ImProperFraction<T,PDP::Enable_If_T<PDP::IsInteger<T>::value || PDP::IsUnsignedInteger<T>::value>> Ceil(const PDP::ImProperFraction<T,PDP::Enable_If_T<PDP::IsInteger<T>::value || PDP::IsUnsignedInteger<T>::value>> & improperFraction);


    template<typename T>
    inline PDP::ImProperFraction<T,PDP::Enable_If_T<PDP::IsInteger<T>::value || PDP::IsUnsignedInteger<T>::value>> Floor(const PDP::ImProperFraction<T,PDP::Enable_If_T<PDP::IsInteger<T>::value || PDP::IsUnsignedInteger<T>::value>> & improperFraction);

    template<typename T>
    inline PDP::ImProperFraction<T,PDP::Enable_If_T<PDP::IsInteger<T>::value || PDP::IsUnsignedInteger<T>::value>> Exp(const PDP::ImProperFraction<T,PDP::Enable_If_T<PDP::IsInteger<T>::value || PDP::IsUnsignedInteger<T>::value>> & improperFraction);


    template<typename T, typename U>
    inline PDP::ImProperFraction<T,PDP::Enable_If_T<(PDP::IsInteger<T>::value || PDP::IsUnsignedInteger<T>::value) &&  PDP::IsConvertible<U, T>::value>> Pow(const PDP::ImProperFraction<T,PDP::Enable_If_T<PDP::IsInteger<T>::value || PDP::IsUnsignedInteger<T>::value>> & improperFraction,const PDP::ImProperFraction<U,PDP::Enable_If_T<PDP::IsInteger<T>::value || PDP::IsUnsignedInteger<T>::value>> & improperFraction2);

    template<typename T, typename U>
    inline PDP::ImProperFraction<T,PDP::Enable_If_T<(PDP::IsInteger<T>::value || PDP::IsUnsignedInteger<T>::value) &&  PDP::IsConvertible<U, T>::value>> Tan2(const PDP::ImProperFraction<T,PDP::Enable_If_T<PDP::IsInteger<T>::value || PDP::IsUnsignedInteger<T>::value>> & improperFraction,const PDP::ImProperFraction<U,PDP::Enable_If_T<PDP::IsInteger<T>::value || PDP::IsUnsignedInteger<T>::value>> & improperFraction2);


}
#endif
