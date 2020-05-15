//
// Created by James Landess on 2/23/20.
//

#ifndef LANDESSDEVCORE_RATIO_H
#define LANDESSDEVCORE_RATIO_H

#include "Definitions/Integer.hpp"
#include "Algorithms/CompileTimeOperations.hpp"
#include "Definitions/Common.hpp"
namespace LD
{
    template<typename T, typename U, class sfinae = void>
    class Ratio;


    template<typename T, typename U>
    class Ratio<T,U,LD::Enable_If_T<LD::Require<(sizeof(T) >= sizeof(U))>>>
    {
    private:

    public:
    };

    template<typename T, typename U>
    class Ratio<T,U,LD::Enable_If_T<LD::Require<(sizeof(T)<  sizeof(U))>>>
    {
    private:

    public:

    };

    namespace CT
    {
        template<LD::UInteger Numerator, LD::UInteger Denominator>
        class URatio;




        template<LD::UInteger Numerator, LD::UInteger Denominator>
        class Ratio
        {
        private:

            static  const constexpr LD::UInteger na = LD::CT::Abs<Numerator>;
            static  const constexpr LD::UInteger da = LD::CT::Abs<Denominator>;
            static  const constexpr LD::Integer s = LD::CT::Sign<Numerator> * LD::CT::Sign<Denominator>;
            static  const constexpr LD::UInteger GCD = LD::CT::GCD<na, da>;
        public:

            static  const constexpr  LD::UInteger Num = s * na / Ratio<Numerator,Denominator>::GCD;

            static  const LD::UInteger Den = da / Ratio<Numerator,Denominator>::GCD;
        };

        namespace Detail
        {
            template<typename T, typename V, class A = void>
            struct IsRatioGreater;

            template<LD::UInteger Num1, LD::UInteger Den1, LD::UInteger Num2, LD::UInteger Den2>
            struct IsRatioGreater<LD::CT::Ratio<Num1,Den1>,LD::CT::Ratio<Num2,Den2>,LD::Enable_If_T<true>>
            {
                using R1 = LD::CT::Ratio<Num1*Den2,Den1*Den2>;
                using R2 = LD::CT::Ratio<Num2*Den1,Den1*Den2>;

                static constexpr bool value = (R1::Num > R2::Num);
            };


            template<typename T, typename V, class A = void>
            struct IsRatioLess;

            template<LD::UInteger Num1, LD::UInteger Den1, LD::UInteger Num2, LD::UInteger Den2>
            struct IsRatioLess<LD::CT::Ratio<Num1,Den1>,LD::CT::Ratio<Num2,Den2>>
            {
                using R1 = LD::CT::Ratio<Num1*Den2,Den1*Den2>;
                using R2 = LD::CT::Ratio<Num2*Den1,Den1*Den2>;

                static constexpr bool value = (R1::Num < R2::Num);
            };


            template<typename T, typename V>
            struct IsRatioEqual;


            template<LD::UInteger Num1, LD::UInteger Den1, LD::UInteger Num2, LD::UInteger Den2>
            struct IsRatioEqual<LD::CT::Ratio<Num1,Den1>,LD::CT::Ratio<Num2,Den2>>
            {
                using R1 = LD::CT::Ratio<Num1*Den2,Den1*Den2>;
                using R2 = LD::CT::Ratio<Num2*Den1,Den1*Den2>;

                static constexpr bool value = (R1::Num == R2::Num);
            };

            template<typename T, typename V>
            struct IsRatioNotEqual;

            template<LD::UInteger Num1, LD::UInteger Den1, LD::UInteger Num2, LD::UInteger Den2>
            struct IsRatioNotEqual<LD::CT::Ratio<Num1,Den1>,LD::CT::Ratio<Num2,Den2>>
            {
                using R1 = LD::CT::Ratio<Num1*Den2,Den1*Den2>;
                using R2 = LD::CT::Ratio<Num2*Den1,Den1*Den2>;

                static constexpr bool value = (R1::Num != R2::Num);
            };

            template<typename T, typename V>
            struct IsRatioRecipricol;

            template<LD::UInteger Num1, LD::UInteger Den1, LD::UInteger Num2, LD::UInteger Den2>
            struct IsRatioRecipricol<LD::CT::Ratio<Num1,Den1>,LD::CT::Ratio<Num2,Den2>>
            {
                using R1 = LD::CT::Ratio<Num1*Den2,Den1*Den2>;
                using R2 = LD::CT::Ratio<Num2*Den1,Den1*Den2>;

                static constexpr bool value = (R1::Num == R2::Den) && (R1::Den == R2::Num);
            };

            template<typename T>
            struct IsRatioIdentity;

            template<LD::UInteger Num, LD::UInteger Den>
            struct IsRatioIdentity<LD::CT::Ratio<Num,Den>>
            {
                using R= LD::CT::Ratio<Num,Den>;

                static constexpr bool value = (R::Num == 1) && (R::Den == 1);
            };

            template<typename T, typename V>
            struct AddRatio;


            template<LD::UInteger Num1, LD::UInteger Den1, LD::UInteger Num2, LD::UInteger Den2>
            struct AddRatio<LD::CT::Ratio<Num1,Den1>,LD::CT::Ratio<Num2,Den2>>
            {
                using type = LD::CT::Ratio<(Num1*Den2)+(Num2*Den1),Den1*Den2>;
            };

        }

        template<typename T, typename V>
        constexpr bool IsRatioGreater = LD::CT::Detail::IsRatioGreater<T,V>::value;

        template<typename T, typename V>
        constexpr bool IsRatioLess = LD::CT::Detail::IsRatioLess<T,V>::value;

        template<typename T, typename V>
        constexpr bool IsRatioEqual = LD::CT::Detail::IsRatioEqual<T,V>::value;

        template<typename T, typename V>
        constexpr bool IsRatioNotEqual = LD::CT::Detail::IsRatioNotEqual<T,V>::value;

        template<typename T, typename V>
        constexpr bool IsRatioRecipricol = LD::CT::Detail::IsRatioRecipricol<T,V>::value;

        template<typename T>
        constexpr bool IsRatioIdentity = LD::CT::Detail::IsRatioIdentity<T>::value;

        template<typename T, typename V>
        using AddRatio = typename LD::CT::Detail::AddRatio <T,V>::type;


        /*
   template<LD::UInteger Numerator, LD::UInteger Denominator, LD::UInteger _MixedNum = 0, LD::UInteger _MixedDen = 1>
   class MixedRatio
   {
   private:

       static  const LD::UInteger na = LD::CT::Abs<Numerator>;
       static  const LD::UInteger da = LD::CT::Abs<Denominator>;
       static  const LD::UInteger s = LD::CT::Sign<Numerator> * LD::CT::Sign<Denominator>;
       static  const LD::UInteger GCD = LD::CT::GCD<na, da>;
       static  const LD::UInteger mixedna = LD::CT::Abs<_MixedNum>;
       static  const LD::UInteger mixedda = LD::CT::Abs<_MixedDen>;
       static  const LD::UInteger mixeds = LD::CT::Sign<_MixedNum> * LD::CT::Sign<_MixedDen>;
       static  const LD::UInteger mixedgcd = LD::CT::GCD<mixedna, mixedda>;
   public:

       static  const LD::UInteger Num = s * na / MixedRatio<Numerator,Denominator,_MixedNum,_MixedDen>::GCD;
       static  const LD::UInteger Den = da / MixedRatio<Numerator,Denominator,_MixedNum,_MixedDen>::GCD;

       static  const LD::UInteger MixedNum = mixeds * mixedna / mixedgcd;
       static  const LD::UInteger MixedDen = mixedda / mixedgcd;
   };
    */
    }





    typedef LD::CT::Ratio<1LL,LD::CT::Pow<10,24>> Yocto;
    typedef LD::CT::Ratio<1LL,LD::CT::Pow<10,21>> Zepto;
    typedef LD::CT::Ratio<1LL,LD::CT::Pow<10,18>> Atto;
    typedef LD::CT::Ratio<1LL,LD::CT::Pow<10,15>> Femto;
    typedef LD::CT::Ratio<1LL,LD::CT::Pow<10,12>> Pico;
    typedef LD::CT::Ratio<1LL,LD::CT::Pow<10,9>> Nano;
    typedef LD::CT::Ratio<1LL,LD::CT::Pow<10,6>> Micro;
    typedef LD::CT::Ratio<1LL,LD::CT::Pow<10,3>> Milli;
    typedef LD::CT::Ratio<1LL,LD::CT::Pow<10,2>> Centi;
    typedef LD::CT::Ratio<1LL,LD::CT::Pow<10,1>> Deci;
    typedef LD::CT::Ratio<LD::CT::Pow<10,1>, 1LL> Deca;
    typedef LD::CT::Ratio<LD::CT::Pow<10,2>, 1LL> Hecto;
    typedef LD::CT::Ratio<LD::CT::Pow<10,3>, 1LL> Kilo;
    typedef LD::CT::Ratio<LD::CT::Pow<10,6>, 1LL> Mega;
    typedef LD::CT::Ratio<LD::CT::Pow<10,9>, 1LL> Giga;
    typedef LD::CT::Ratio<LD::CT::Pow<10,12>, 1LL> Tera;
    typedef LD::CT::Ratio<LD::CT::Pow<10,15>, 1LL> Peta;
    typedef LD::CT::Ratio<LD::CT::Pow<10,18>, 1LL> Exa;
    typedef LD::CT::Ratio<LD::CT::Pow<10,21>, 1LL> Zetta;
    typedef LD::CT::Ratio<LD::CT::Pow<10,24>, 1LL> Yotta;
}
#endif //LANDESSDEVCORE_RATIO_H
