//
// Created by phoenixflower on 11/16/21.
//

#ifndef LANDESSDEVCORE_DATALINK_HPP
#define LANDESSDEVCORE_DATALINK_HPP
#include "Primitives/General/Span.hpp"
#include "Definitions/Integer.hpp"
#include "IO/DatabaseOperationResult.h"
#include "Primitives/General/mapboxvariant.hpp"
#include "Algorithms/ToJSON.hpp"
#include "Algorithms/FromJSON.hpp"
#include "Functor/LightWeightDelegate.h"
#include "Patterns/LazyLambda.hpp"
#include "Examples/ReflectionDemoTypes.h"
namespace LD
{
    namespace Detail
    {
        template<typename T>
        class BackInserterBindToVoid
        {
        private:
            T & mObject;
        public:
            BackInserterBindToVoid(T & object) noexcept:mObject{object}{}
            bool operator()(unsigned char data) noexcept
            {
                LD::BackInserter<T> inserter(this->mObject);
                inserter = data;
                return true;
            }
        };
    }

    class DataLink
    {
    private:

    public:

        virtual LD::UInteger Write(unsigned char * data, LD::UInteger size) noexcept = 0;
        virtual LD::UInteger Read(unsigned char * data, LD::UInteger size) noexcept = 0;
        //virtual LD::UInteger Read(unsigned char *& data, LD::Mem::MemoryResource * resource = LD::Mem::GetNewDeleteResource()) noexcept{return 0;}
        virtual LD::UInteger Read(LD::LightWeightDelegate<bool(unsigned char)>) noexcept = 0;

        template<typename Functor>
        LD::UInteger Read(Functor & functor) noexcept
        {
            LD::Detail::BackInserterBindToVoid<Functor> binder{functor};
            LD::LightWeightDelegate<bool(unsigned char)> callBack = LD::LightWeightDelegate<bool(unsigned char)>::Bind(binder,&LD::Detail::BackInserterBindToVoid<Functor>::operator());
            return this->Read(callBack);
            return 0;
        }
        virtual ~DataLink(){}
        //virtual LD::Variant<LD::UInteger,LD::TransactionError> Write1(unsigned char * data, LD::UInteger size) noexcept = 0;


    };

    template<typename T>
    inline auto ToJson(DataLink & link, T && object) noexcept
    {
        nlohmann::json document;

        LD::ToJson(document,LD::Forward<T>(object));

        auto dumpedData = document.dump();

        link.Write((unsigned char*)dumpedData.c_str(),dumpedData.size());
    }



    template<typename T, typename BufferType>
    inline LD::Optional<T&> FromJSON(DataLink & dataLink, T & object,LD::Type<BufferType>) noexcept
    {
        BufferType buffer;
        LD::BackInserter inserter{buffer};
        LD::Detail::BackInserterBindToVoid binder{buffer};
        auto callBack = LD::LightWeightDelegate<bool(unsigned char)>::Bind(binder,&LD::Detail::BackInserterBindToVoid<LD::ImmutableString<1024>>::operator());


        LD::LazyLambda lazylambda{[](unsigned char character, LD::BackInserter<BufferType> & inserter) noexcept
        {
            inserter = character;
                                      //std::cout << character;
                                      //LD::BasicLogger log{logger};
                                      //log << character;
        },inserter};
        LD::UInteger amountRead = dataLink.Read(lazylambda);
        if(amountRead > 0)
        {
            simdjson::dom::parser parser;

            //std::cout << "\n";
            //std::cout << buffer.Data() << "\n";
            //std::cout << "link buffer: " << buffer.Data() << "\n";
            simdjson::dom::element parsedResponse = parser.parse(buffer.Data(),buffer.GetSize());
            LD::FromJSON(object,parsedResponse);
            return LD::Optional<T&>{object};
        }
        return LD::Optional<T&>{};
    }

    template<typename T>
    inline LD::Optional<T&> FromJSON(DataLink & dataLink, T & object) noexcept
    {

        return LD::FromJSON(dataLink,object,LD::Type<LD::ImmutableString<1024>>{});
    }
}
#endif //LANDESSDEVCORE_DATALINK_HPP
