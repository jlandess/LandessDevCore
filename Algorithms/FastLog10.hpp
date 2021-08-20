//
// Created by phoenixflower on 3/19/21.
//

#ifndef LANDESSDEVCORE_FASTLOG10_HPP
#define LANDESSDEVCORE_FASTLOG10_HPP
#include "Definitions/Integer.hpp"
namespace LD
{
    inline LD::UInteger FastLog10(LD::UInteger v)
    {
        return (v >= 1000000000u) ? 9 : (v >= 100000000u) ? 8 :
                                        (v >= 10000000u) ? 7 : (v >= 1000000u) ? 6 :
                                                               (v >= 100000u) ? 5 : (v >= 10000u) ? 4 :
                                                                                    (v >= 1000u) ? 3 : (v >= 100u) ? 2 : (v >= 10u) ? 1u : 0u;
    }
}
#endif //LANDESSDEVCORE_FASTLOG10_HPP
