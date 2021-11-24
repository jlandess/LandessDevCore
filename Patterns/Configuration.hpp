//
// Created by phoenixflower on 11/8/21.
//

#ifndef LANDESSDEVCORE_CONFIGURATION_HPP
#define LANDESSDEVCORE_CONFIGURATION_HPP
#include "Memory/Optional.h"
#include "TypeTraits/Detection.hpp"
#include "TypeTraits/FunctionalReflection.hpp"
#include "Primitives/General/Immutable.hpp"
#include "Primitives/General/StringView.hpp"
#include "IO/json.hpp"
#include "Memory/PolyMorphicAllocator.hpp"
#include "Memory/UniquePointer.h"
#include "Memory/MemoryResource.hpp"
namespace LD
{
    class Configuration
    {
    protected:
        LD::Mem::MemoryResource & mMemoryResource;
    public:

        Configuration(LD::Mem::MemoryResource & resource = LD::Mem::GetNewDeleteResourceReference()) noexcept:mMemoryResource{resource}
        {

        }
        virtual ~Configuration(){}
        virtual LD::Optional<LD::Integer> operator()(LD::StringView key, LD::Type<LD::Integer>) noexcept = 0;
        virtual LD::Optional<LD::UInteger> operator()(LD::StringView key, LD::Type<LD::UInteger>) noexcept = 0;
        virtual LD::Optional<LD::Float> operator()(LD::StringView key, LD::Type<LD::Float>) noexcept = 0;
        virtual LD::Optional<LD::UniquePointer<Configuration,LD::MemoryResourceDeleter>> operator()(LD::StringView key, LD::Type<Configuration>) noexcept = 0;
        virtual LD::Optional<bool> operator()(LD::StringView key, LD::Type<bool>) noexcept = 0;
        virtual LD::Optional<StringView> operator()(LD::StringView key, LD::Type<LD::StringView>) noexcept = 0;
    };

    class JsonConfiguration: public Configuration
    {
    private:
        nlohmann::json & mConfiguration;
    public:
        JsonConfiguration(LD::Mem::MemoryResource & resource, nlohmann::json & config) noexcept;


        virtual LD::Optional<LD::Integer> operator()(LD::StringView key, LD::Type<LD::Integer>) noexcept;
        virtual LD::Optional<LD::UInteger> operator()(LD::StringView key, LD::Type<LD::UInteger>) noexcept;
        virtual LD::Optional<LD::Float> operator()(LD::StringView key, LD::Type<LD::Float>) noexcept;
        virtual LD::Optional<LD::UniquePointer<Configuration,LD::MemoryResourceDeleter>> operator()(LD::StringView key, LD::Type<Configuration>) noexcept;
        virtual LD::Optional<bool> operator()(LD::StringView key, LD::Type<bool>) noexcept;
        virtual LD::Optional<StringView> operator()(LD::StringView key, LD::Type<LD::StringView>) noexcept;
    };


    class BasicConfiguration
    {
    private:
        Configuration & mConfiguration;
    public:

        class BasicConfigurationInsertion
        {
        private:
            Configuration & mConfiguration;
            LD::StringView mKey;
        public:
            BasicConfigurationInsertion(LD::StringView key,Configuration & config) noexcept:mConfiguration{config},mKey{key}
            {

            }
            template<typename T>
            LD::Enable_If_T<LD::Require<LD::IsInteger<T>>,BasicConfiguration &> operator >> (LD::Optional<T> & possibleInteger) noexcept
            {
                possibleInteger =  this->mConfiguration(this->mKey,LD::Type<LD::Integer>{});
                return (*this);
            }

            template<typename T>
            LD::Enable_If_T<LD::Require<LD::IsFloatingPoint<T>>,BasicConfigurationInsertion &> operator >> (LD::Optional<T> & possibleFloatingPointNumber) noexcept
            {
                possibleFloatingPointNumber =  this->mConfiguration(this->mKey,LD::Type<LD::Float>{});
                return (*this);
            }

            template<typename T>
            LD::Enable_If_T<LD::Require<LD::CT::IsSame(LD::Type<bool>{},LD::Type<T>{})>,BasicConfigurationInsertion &> operator >> (LD::Optional<T> & possibleBoolean) noexcept
            {
                return (*this);
            }

            template<LD::UInteger Size>
            BasicConfiguration & operator >> (LD::Optional<LD::ImmutableString<Size>> & possibleInteger) noexcept;

            BasicConfigurationInsertion & operator >> (LD::Optional<LD::UniquePointer<LD::Configuration,LD::MemoryResourceDeleter>> & possibleSubConfiguration) noexcept
            {
                //possibleSubConfiguration =  this->mConfiguration(this->mKey,LD::Type<LD::Configuration>{});
                return (*this);
            }

            template<typename BackInserterType>
            BasicConfiguration & operator >> (BackInserterType backInserter) noexcept;
        };
        BasicConfiguration(Configuration & configuration) noexcept:mConfiguration{configuration}
        {

        }

        BasicConfigurationInsertion operator[](LD::StringView key) noexcept
        {
            return BasicConfigurationInsertion{key,this->mConfiguration};
        }

        template<typename Key, typename TypeToFind>
        BasicConfiguration & operator >> (LD::Pair<Key,TypeToFind> kv) noexcept
        {
            return (*this);
        }
    };
}
#endif //LANDESSDEVCORE_CONFIGURATION_HPP
