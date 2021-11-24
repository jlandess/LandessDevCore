//
// Created by phoenixflower on 11/16/21.
//

#ifndef LANDESSDEVCORE_DATALINK_HPP
#define LANDESSDEVCORE_DATALINK_HPP
#include "Primitives/General/Span.hpp"
#include "Definitions/Integer.hpp"
#include "IO/DatabaseOperationResult.h"
#include "Primitives/General/mapboxvariant.hpp"
namespace LD
{
    class DataLink
    {
    private:

    public:

        virtual LD::UInteger Write(unsigned char * data, LD::UInteger size) noexcept = 0;
        virtual LD::UInteger Read(unsigned char * data, LD::UInteger size) noexcept = 0;

        //virtual LD::Variant<LD::UInteger,LD::TransactionError> Write1(unsigned char * data, LD::UInteger size) noexcept = 0;
    };
}
#endif //LANDESSDEVCORE_DATALINK_HPP
