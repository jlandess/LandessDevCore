//
// Created by James Landess on 2/4/20.
//

#ifndef LANDESSDEVCORE_ISCLASSTYPE_H
#define LANDESSDEVCORE_ISCLASSTYPE_H

#include "TypeTraits/IsIntegralType.h"
#include "TypeTraits/IsUnion.h"
namespace LD
{
    namespace Detail
    {
        template<typename T>
        struct IsClassType
        {
            static const bool value = !IsIntegrelType<T>::value;
        };

        //template <bool _Bp, class _Tp = void> using Enable_If_T = typename EnableIf<_Bp, _Tp>::type;
        template<typename T> using IsClassType_V = typename IsClassType<T>::value;
    }
}

namespace LD
{
    namespace Detail
    {
        template <class T>
        LD::Detail::IntegralConstant<bool, !LD::Detail::IsUnion<T>::value> test(int T::*);

        template <class>
        LD::FalseType test(...);

        template <class T>
        struct IsClass : decltype(LD::Detail::test<T>(nullptr))
        {};
    }


    template<typename T>
    constexpr bool IsClass = LD::Detail::IsClass<T>::value;




}








#endif //LANDESSDEVCORE_ISCLASSTYPE_H
