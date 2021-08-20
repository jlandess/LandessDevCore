//
// Created by phoenixflower on 3/19/21.
//

#ifndef LANDESSDEVCORE_ISUTF8COMPLIANT_HPP
#define LANDESSDEVCORE_ISUTF8COMPLIANT_HPP
#include "UTF8.hpp"
#include "TypeTraits/Detection.hpp"
#include "TypeTraits/Declval.hpp"
namespace LD
{
    namespace Detail
    {
        template<typename T>
        using UTFBeginTest = decltype(LD::UTF8::Begin(LD::Declval<T>()));
        template<typename T>
        using UTFBEndTest = decltype(LD::UTF8::End(LD::Declval<T>()));
    }

    template<typename T>
    constexpr bool IsUTF8Compliant = LD::Exists<LD::Detail::UTFBEndTest,T> && LD::Exists<LD::Detail::UTFBeginTest,T>;
}
#endif //LANDESSDEVCORE_ISUTF8COMPLIANT_HPP
