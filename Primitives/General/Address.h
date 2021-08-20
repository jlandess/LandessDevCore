//
// Created by phoenixflower on 1/5/21.
//

#ifndef LANDESSDEVCORE_ADDRESS_H
#define LANDESSDEVCORE_ADDRESS_H
#include "Definitions/Integer.hpp"
#include "Immutable.hpp"
#include "Memory/Optional.h"
namespace LD
{
    class Address
    {
    private:
        LD::UInteger mNumber;
        LD::ImmutableString<2> mCardinalDirection;
        LD::ImmutableString<30> mStreetName;
        LD::ImmutableString<30> mStreetType;
        LD::ImmutableString<25> mCity;
        LD::UInteger mPostalCode;
        LD::Optional<LD::UInteger> mApartmentNumber;
    public:
    };
}
#endif //LANDESSDEVCORE_ADDRESS_H
