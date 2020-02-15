//
// Created by James Landess on 2/4/20.
//

#ifndef LANDESSDEVCORE_ISCLASSTYPE_H
#define LANDESSDEVCORE_ISCLASSTYPE_H

#include "TypeTraits/IsIntegralType.h"
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
#endif //LANDESSDEVCORE_ISCLASSTYPE_H
