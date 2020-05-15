//
// Created by phoenixflower on 10/12/19.
//

#ifndef LANDESSDEVDATASTRUCTURES_ISMEMBERPOINTER_H
#define LANDESSDEVDATASTRUCTURES_ISMEMBERPOINTER_H

#include "Definitions/Common.hpp"
namespace LD
{
    namespace Detail
    {
        template< class T >
        struct is_member_pointer_helper         : LD::FalseType{};

        template< class T, class U >
        struct is_member_pointer_helper<T U::*> : LD::TrueType {};

        template< class T >
        struct is_member_pointer :
                is_member_pointer_helper<typename LD::Detail::RemoveCV<T>::type> {};
    }

    template<typename T>
    constexpr bool IsMemberPointer = LD::Detail::is_member_pointer<T>::value;
}
#endif //LANDESSDEVDATASTRUCTURES_ISMEMBERPOINTER_H
