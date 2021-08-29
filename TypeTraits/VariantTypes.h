//
// Created by phoenixflower on 8/27/21.
//

#ifndef LANDESSDEVCORE_VARIANTTYPES_H
#define LANDESSDEVCORE_VARIANTTYPES_H
#include "Primitives/General/mapboxvariant.hpp"
#include "TypeList.hpp"
#include "Type.h"
namespace LD
{
    namespace CT
    {
        template<typename ... A>
        LD::CT::TypeList<A...> VariantTypes(LD::Type<LD::Variant<A...>> ) noexcept
        {
            return LD::CT::TypeList<A...>{};
        }
    }
}
#endif //LANDESSDEVCORE_VARIANTTYPES_H
