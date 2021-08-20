//
// Created by phoenixflower on 3/19/21.
//

#ifndef LANDESSDEVCORE_GETDECIMALPLACES_HPP
#define LANDESSDEVCORE_GETDECIMALPLACES_HPP
#include "Definitions/Integer.hpp"
#include "Floor.hpp"
namespace LD
{
    inline LD::UInteger GetDecimalPlaces(LD::Float dbVal)
    {
        static const LD::UInteger MAX_DP = MaximumDecimalPlaces<LD::Float>::GetMaximumAmountOfDecimalPlaces();
        static const LD::Float THRES = LD::Power((LD::Float)0.1, (LD::Integer)MAX_DP);
        if (dbVal == 0.0)
            return 0;
        LD::UInteger nDecimal = 0;
        while (dbVal - LD::Floor(dbVal) > THRES && nDecimal < MAX_DP)
        {
            dbVal *= 10.0;
            nDecimal++;
        }
        return nDecimal;
    }
}
#endif //LANDESSDEVCORE_GETDECIMALPLACES_HPP
