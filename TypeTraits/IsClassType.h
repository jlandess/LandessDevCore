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
    }

    template <class T>
    struct IsClassEx : decltype(LD::Detail::test<T>(nullptr))
    {};
}

namespace detail
{
    template<typename T>
    constexpr char test_my_bad_is_class_call(int T::*) { return {}; }

    struct two { char _[2]; };

    template<typename T>
    constexpr two test_my_bad_is_class_call(...) { return {}; }
}


template<typename T>
struct my_bad_is_class
        : LD::Detail::IntegralConstant<bool,sizeof(detail::test_my_bad_is_class_call<T>(nullptr)) == 1>
{
};



#endif //LANDESSDEVCORE_ISCLASSTYPE_H
