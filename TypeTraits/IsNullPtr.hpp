//
// Created by James Landess on 2/4/20.
//

#ifndef LANDESSDEVCORE_ISNULLPTR_HPP
#define LANDESSDEVCORE_ISNULLPTR_HPP

#include "RemoveCV.hpp"
#include "IntegralConstant.hpp"
namespace LD
{
    namespace Detail
    {
        typedef decltype(nullptr) nullptr_t;



        template <class _Tp> struct __is_nullptr_t_impl       : public LD::FalseType {};
        template <>          struct __is_nullptr_t_impl<nullptr_t> : public LD::TrueType {};

        template <class _Tp> struct  IsNullPtr
                : public __is_nullptr_t_impl<typename LD::Detail::RemoveCV<_Tp>::type> {};
    }
}
#endif //LANDESSDEVCORE_ISNULLPTR_HPP
