//
// Created by phoenixflower on 1/23/21.
//

#ifndef LANDESSDEVCORE_CPUCORECOUNT_HPP
#define LANDESSDEVCORE_CPUCORECOUNT_HPP
#include "Definitions/Integer.hpp"
namespace LD
{
    class CPUCoreCount
    {
    private:
        LD::UInteger mCoreCount;
    public:
        CPUCoreCount(LD::UInteger count) noexcept:mCoreCount{count}
        {

        }

        LD::UInteger Count() const noexcept
        {
            return this->mCoreCount;
        }
    };
}
#endif //LANDESSDEVCORE_CPUCORECOUNT_HPP
