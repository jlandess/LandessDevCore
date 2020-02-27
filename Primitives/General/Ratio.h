//
// Created by James Landess on 2/23/20.
//

#ifndef LANDESSDEVCORE_RATIO_H
#define LANDESSDEVCORE_RATIO_H

#include "Definitions/Integer.h"
#include "Algorithms/CompileTimeOperations.h"
#include "Definitions/Common.h"
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
        class Ratio
        {
        private:

            static  const constexpr LD::UInteger na = LD::CT::Abs<Numerator>;
            static  const constexpr LD::UInteger da = LD::CT::Abs<Denominator>;
            static  const constexpr LD::UInteger s = LD::CT::Sign<Numerator> * LD::CT::Sign<Denominator>;
            static  const constexpr LD::UInteger GCD = LD::CT::GCD<na, da>;
        public:

            static  const constexpr  LD::UInteger Num = s * na / Ratio<Numerator,Denominator>::GCD;

            //constexpr static LD::UInteger Num = Numerator;
            //constexpr static LD::UInteger Den = Denominator;

            static  const LD::UInteger Den = da / Ratio<Numerator,Denominator>::GCD;
        };

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
