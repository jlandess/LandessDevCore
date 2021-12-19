//
// Created by phoenixflower on 11/21/21.
//

#ifndef LANDESSDEVCORE_CONNECTIONBROKER_HPP
#define LANDESSDEVCORE_CONNECTIONBROKER_HPP
#include "DataLink.hpp"
#include "Patterns/Configuration.hpp"
#include "Primitives/General/StringView.hpp"
#include "Memory/OptionalReference.h"
#include "Memory/Optional.h"
#include "DHTBackend.hpp"
#include "Primitives/General/mapboxvariant.hpp"
#include "Memory/UniquePointer.h"
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
        LD::Configuration & mBrokerConfiguration;
        LD::Configuration & mConnectionsConfiguration;
        DataLinkBuffer mBuffer;
        LD::Variant<LD::UniquePointer<LD::OpenDHTBackend,LD::MemoryResourceDeleter>> mBackend;
        LD::Mem::MemoryResource & mMemoryResource;
    public:

        BasicConnectionBroker(LD::Configuration & config, LD::Configuration & connections,LD::Mem::MemoryResource & resource = LD::Mem::GetNewDeleteResourceReference()) noexcept:mBrokerConfiguration{config},mConnectionsConfiguration{connections},mMemoryResource{resource}//,mBackend{LD::IPV6Address{"fd00:1700:81b8:401e:0:d9:191:4a34"},LD::Port{4225},LD::Port{4222}}
        {

            LD::Optional<LD::StringView> type = config("type",LD::Type<LD::StringView>{});
            if(type)
            {
                LD::StringView typeAsText = *type;
                if (typeAsText == "OpenDHT")
                {
                    LD::Optional<LD::Integer> bootstrapPort = config("bootstrap",LD::Type<LD::Integer>{});
                    LD::Optional<LD::Integer> bindPort = config("bind",LD::Type<LD::Integer>{});
                    LD::Optional<LD::StringView> host = config("host",LD::Type<LD::StringView>{});
                    if (host && bootstrapPort && bindPort)
                    {

                        this->mBackend =  LD::Move(LD::AllocateUnique<LD::OpenDHTBackend>(
                                &this->mMemoryResource,
                                LD::IPV6Address{"fd00:1700:81b8:401e:0:d9:191:4a34"},
                                LD::Port{4225},
                                LD::Port{4222}));
                        printf("OpenDHT node can be created from message broker\n");

                    }
                }else if(type && *type == "MQTT")
                {

                }else if(type && *type == "AMQP")
                {

                }
            }
        }

        virtual LD::Optional<DataLink &> Link(LD::StringView linkName) noexcept
        {

            //determine if the link is pre-defined in the connections list
            LD::Optional<LD::UniquePointer<Configuration,LD::MemoryResourceDeleter>> possibleConnections = this->mConnectionsConfiguration(linkName,LD::Type<Configuration>{});

            if(possibleConnections)
            {
                LD::Configuration & connectionConfiguration = *(*possibleConnections);
                LD::Optional<LD::StringView> type = connectionConfiguration("type",LD::Type<LD::StringView>{});
                if(type)
                {
                    LD::StringView connectionType = (*type);
                    if(connectionType == "TCP")
                    {

                    }else if(connectionType == "UDP")
                    {

                    }
                    return LD::Optional<DataLink &>{};
                }
            }else
            {
                LD::BackInserter inserter{this->mBuffer};

                LD::ImmutableString<64> topicHolder{linkName};
                using LinkType = LD::TopicalOpenDHTConnection<LD::ImmutableString<64>, LD::Channel<LD::ImmutableString<64>>, LD::ImmutableString<64>>;

                auto onOpenDHT = [&](LD::UniquePointer<LD::OpenDHTBackend,LD::MemoryResourceDeleter> & backend) noexcept
                {
                    LD::SharedPointer<LD::TopicalOpenDHTConnection<LD::ImmutableString<64>, LD::Channel<LD::ImmutableString<64>>, LD::ImmutableString<64>>> link = backend->TopicalDataLink(
                            LD::ImmutableString<64>{topicHolder},
                            LD::Type<LD::Channel<LD::ImmutableString<64>>>{},
                            LD::Type<LD::ImmutableString<64>>{});
                    this->mBuffer.PushBack(link);
                    return LD::Optional<DataLink &>{link.get()};
                };
                return LD::Visit(onOpenDHT,this->mBackend);
            }
            return LD::Optional<DataLink &>{};
        }
    };
}
#endif //LANDESSDEVCORE_CONNECTIONBROKER_HPP
