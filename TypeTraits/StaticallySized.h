//
// Created by phoenixflower on 5/31/20.
//

#ifndef LANDESSDEVCORE_STATICALLYSIZED_H
#define LANDESSDEVCORE_STATICALLYSIZED_H
#include "IntegralConstant.hpp"
namespace LD
{
    namespace Detail
    {
        template<typename T>
        struct StaticallySized: public LD::Detail::IntegralConstant<bool,false>
        {

        };
    }

    template<typename T>
    constexpr bool StaticallySized = LD::Detail::StaticallySized<T>::value;
}
#endif //LANDESSDEVCORE_STATICALLYSIZED_H
