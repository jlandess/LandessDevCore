//
// Created by phoenixflower on 10/18/19.
//

#ifndef LANDESSDEVDATASTRUCTURES_MOVEIFRVALUE_H
#define LANDESSDEVDATASTRUCTURES_MOVEIFRVALUE_H

#include "Definitions/Common.h"

namespace LD
{
    template <class RetT, class T>
    constexpr decltype(auto) MoveIfRvalue(T && value) noexcept
    {

        static_assert(LD::Detail::IsReference<RetT>::type);
        using value_type = typename LD::Detail::RemoveReference<T>::type ;


        using ref_type   = LD::Detail::Conditional_T <LD::Detail::IsRValueReference<RetT>::value,
                value_type &&, value_type &>;

        return static_cast<ref_type>(value);
    }
}
#endif //LANDESSDEVDATASTRUCTURES_MOVEIFRVALUE_H
