//
// Created by James Landess on 2/4/20.
//

#ifndef LANDESSDEVCORE_ADDPOINTER_HPP
#define LANDESSDEVCORE_ADDPOINTER_HPP

#include "TypeTraits/IsSame.hpp"
#include "RemoveCV.hpp"
#include "RemoveReference.hpp"
namespace LD
{
    namespace Detail
    {
        template <class _Tp,
                bool = LD::Detail::__is_referenceable<_Tp>::value ||
                       LD::Detail::IsSame<typename LD::Detail::RemoveCV<_Tp>::type, void>::value>
        struct __add_pointer_impl
        {typedef typename LD::Detail::RemoveReference<_Tp>::type* type;};
        template <class _Tp> struct __add_pointer_impl<_Tp, false>
        {typedef _Tp type;};

        template <class _Tp> struct  add_pointer
        {typedef typename __add_pointer_impl<_Tp>::type type;};
    }
}
#endif //LANDESSDEVCORE_ADDPOINTER_HPP
