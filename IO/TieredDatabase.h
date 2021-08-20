//
// Created by phoenixflower on 6/9/20.
//

#ifndef LANDESSDEVCORE_TIEREDDATABASE_H
#define LANDESSDEVCORE_TIEREDDATABASE_H
#include "TypeTraits/TypeList.hpp"
namespace LD
{
    /**
     * @brief Expresses the levels L1,L2,L3,...,LN of a series of backing data stores
     */
    class CachePolicy
    {

    };

    /**
     * @brief Expresses how to spread the keys about in multiple backing data stores (the object is now distributed)
     */
    class DistributedPolicy
    {

    };

    template<LD::UInteger ParityLevel>
    class RAIDPolicy
    {

    };
    template<typename PrimaryDataStore, typename SecondaryDataStore>
    class TieredKeyValueStore
    {

    };

    template<typename PrimaryDataStore, typename ... Tiers>
    class TieredKeyValueStore<PrimaryDataStore,LD::CT::TypeList<Tiers...>>
    {

    };
}
#endif //LANDESSDEVCORE_TIEREDDATABASE_H
