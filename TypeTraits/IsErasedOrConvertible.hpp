//
// Created by phoenixflower on 12/4/20.
//

#ifndef LANDESSDEVCORE_ISERASEDORCONVERTIBLE_HPP
#define LANDESSDEVCORE_ISERASEDORCONVERTIBLE_HPP
#include "TypeTraits/__or.hpp"
#include "IsConvertible.hpp"
#include "IsSame.hpp"
namespace LD
{
    // This is used for std::experimental::erased_type from Library Fundamentals.
    struct __erased_type { };

    // This also supports the "type-erased allocator" protocol from the
    // Library Fundamentals TS, where allocator_type is erased_type.
    // The second condition will always be false for types not using the TS.
    template<typename _Alloc, typename _Tp>
    using IsErasedOrConvertible
    = LD::CT::Or<LD::Detail::IsConvertible<_Alloc, _Tp>, LD::Detail::IsSame<_Tp, LD::__erased_type>>;
}
#endif //LANDESSDEVCORE_ISERASEDORCONVERTIBLE_HPP
