//
// Created by phoenixflower on 3/19/21.
//

#ifndef LANDESSDEVCORE_FORWARD_HPP
#define LANDESSDEVCORE_FORWARD_HPP
#include "TypeTraits/RemoveReference.hpp"
namespace LD
{
    template <class _Tp>
    inline constexpr
    _Tp&& Forward(typename LD::Detail::RemoveReference<_Tp>::type& __t) noexcept
    {
        return static_cast<_Tp&&>(__t);
    }
}
#endif //LANDESSDEVCORE_FORWARD_HPP
