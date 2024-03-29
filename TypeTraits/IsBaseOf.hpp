//
// Created by James Landess on 2/6/20.
//

#ifndef LANDESSDEVCORE_ISBASEOF_HPP
#define LANDESSDEVCORE_ISBASEOF_HPP
#include "IntegralConstant.hpp"
namespace LD
{
    namespace Detail
    {
        template <typename B, typename D>
        struct Host
        {
            operator B*() const;
            operator D*();
        };

        template <typename B, typename D>
        struct IsBaseOf
        {
            typedef char (&yes)[1];
            typedef char (&no)[2];

            template <typename T>
            static yes check(D*, T);
            static no check(B*, int);

            static const bool value = sizeof(check(Host<B,D>(), int())) == sizeof(yes);
        };


        //template<typename B, typename D>  constexpr bool IsBaseOf_V =  IsBaseOf<B,D>::value;
        //__is_base_of
        template<typename B, typename D>  constexpr bool IsBaseOf_V =  LD::Detail::IntegralConstant<bool,__is_base_of(B,D)>::value;

    }
}
#endif //LANDESSDEVCORE_ISBASEOF_HPP
