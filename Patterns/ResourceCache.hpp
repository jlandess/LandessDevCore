//
// Created by phoenixflower on 11/11/21.
//

#ifndef LANDESSDEVCORE_RESOURCECACHE_HPP
#define LANDESSDEVCORE_RESOURCECACHE_HPP
#include "Algorithms/Forward.hpp"
#include "Primitives/General/mapboxvariant.hpp"
#include "Primitives/General/StringView.hpp"
#include "Memory/Optional.h"
#include "Memory/OptionalReference.h"
namespace LD
{
    template<typename AssociativeArray>
    class ResourceCache;

    template<typename AssociativeArray, typename ... Types>
    class ResourceCache<AssociativeArray(Types...)>
    {
    private:
        AssociativeArray mAssociativeArray;
    public:
        ResourceCache(AssociativeArray && associativeArray) noexcept:mAssociativeArray{LD::Forward<AssociativeArray>(associativeArray)}
        {

        }

        const LD::OptionalReference<LD::Variant<Types...> &> operator[](LD::StringView key) const noexcept
        {
            auto it = this->mAssociativeArray.find(key);
            if (it != this->mAssociativeArray.end())
            {
                return LD::OptionalReference<LD::Variant<Types...> &>{(*it).second};
            }
            return LD::OptionalReference<LD::Variant<Types...>&>{};
        }

        LD::Variant<Types...> & operator[](LD::StringView key) noexcept
        {
            return this->mAssociativeArray[key];
        }
    };
}
#endif //LANDESSDEVCORE_RESOURCECACHE_HPP
