//
// Created by phoenixflower on 5/14/20.
//

#ifndef LANDESSDEVCORE_ISPOINTER_HPP
#define LANDESSDEVCORE_ISPOINTER_HPP
#include "IntegralConstant.hpp"
#include "RemoveCV.hpp"
namespace LD
{
    namespace Detail
    {
        template <class _Tp> struct __libcpp_is_pointer       : public LD::FalseType {};

        template <class _Tp> struct __libcpp_is_pointer<_Tp*> : public LD::TrueType {};
        template <class _Tp> struct  IsPointer : public LD::Detail::__libcpp_is_pointer<typename LD::Detail::RemoveCV<_Tp>::type> {};
    }

    template<typename T>
    constexpr bool IsPointer = LD::Detail::IsPointer<T>::value;
}
#endif //LANDESSDEVCORE_ISPOINTER_HPP
