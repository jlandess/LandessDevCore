//
// Created by James Landess on 2/4/20.
//

#ifndef LANDESSDEVCORE_INTEGRALCONSTANT_HPP
#define LANDESSDEVCORE_INTEGRALCONSTANT_HPP
namespace LD
{
    namespace Detail
    {
        template <class _Tp, _Tp __v>
        struct  IntegralConstant
        {
            static constexpr const _Tp      value = __v;
            typedef _Tp               value_type;
            typedef IntegralConstant type;
            constexpr operator value_type() const {return value;}
            constexpr value_type operator ()() const {return value;}

        };

        template<typename T>
        struct IsIntegralConstant: public LD::Detail::IntegralConstant<bool,false>
        {

        };

        template<typename T, T value>
        struct IsIntegralConstant<LD::Detail::IntegralConstant<T,value>>: public LD::Detail::IntegralConstant<bool,true>
        {

        };
    }


    template<typename T>
    constexpr bool IsIntegralConstant = LD::Detail::IsIntegralConstant<T>::value;

    typedef LD::Detail::IntegralConstant<bool, true>  TrueType;
    typedef LD::Detail::IntegralConstant<bool, false> FalseType;


}
#endif //LANDESSDEVCORE_INTEGRALCONSTANT_HPP
