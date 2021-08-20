//
// Created by James Landess on 2/6/20.
//

#ifndef LANDESSDEVCORE_ISKINDOF_HPP
#define LANDESSDEVCORE_ISKINDOF_HPP
#include "IsBaseOf.hpp"
#include "IsSame.hpp"
namespace LD
{
    namespace Detail
    {
        template<typename B, typename D>
        class IsKindOf
        {
        public:
            static const bool value = LD::Detail::IsBaseOf<B, D>::value || LD::Detail::IsSame<B, D>::value;
        };


        template<typename B, typename D>  using IsKindOf_V = typename IsKindOf<B,D>::value;
    }

}
#endif //LANDESSDEVCORE_ISKINDOF_HPP
