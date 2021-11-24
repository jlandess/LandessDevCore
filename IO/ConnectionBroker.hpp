//
// Created by phoenixflower on 11/21/21.
//

#ifndef LANDESSDEVCORE_CONNECTIONBROKER_HPP
#define LANDESSDEVCORE_CONNECTIONBROKER_HPP
#include "DataLink.hpp"
#include "Patterns/Configuration.hpp"
#include "Primitives/General/StringView.hpp"
#include "Memory/OptionalReference.h"
namespace LD
{
    class ConnectionBroker
    {
    private:
        //LD::Configuration & mConfiguration;
    public:
        //ConnectionBroker(LD::Configuration & configuration) noexcept:mConfiguration{configuration}{}
        virtual LD::Optional<DataLink &> Link(LD::StringView linkName) noexcept = 0;
    };


    template<typename DataLinkBuffer>
    class BasicConnectionBroker: public LD::ConnectionBroker
    {
    private:
        LD::Configuration & mConfiguration;
        DataLinkBuffer mBuffer;
    public:

        BasicConnectionBroker(LD::Configuration & configuration) noexcept:mConfiguration{configuration}{}

        virtual LD::Optional<DataLink &> Link(LD::StringView linkName) noexcept
        {
            return LD::Optional<DataLink &>{};
        }
    };
}
#endif //LANDESSDEVCORE_CONNECTIONBROKER_HPP
