//
// Created by phoenixflower on 11/13/21.
//

#include "Configuration.hpp"
namespace LD
{
    JsonConfiguration::JsonConfiguration(LD::Mem::MemoryResource & resource, nlohmann::json & config) noexcept: Configuration(resource),
    mConfiguration{config}
    {

    }

    LD::Optional<LD::Integer> JsonConfiguration::operator()(LD::StringView key, LD::Type<LD::Integer>) noexcept
    {
        auto it = mConfiguration.find(key.data());
        if (it != mConfiguration.end() && it->is_number_integer())
        {
            auto result = it->get<LD::Integer>();
            return LD::Optional<LD::Integer>{result};
        }
        return LD::nullopt;
    }

    LD::Optional<LD::UInteger> JsonConfiguration::operator()(LD::StringView key, LD::Type<LD::UInteger>) noexcept
    {
        auto it = mConfiguration.find(key.data());
        if (it != mConfiguration.end() && it->is_number_unsigned())
        {
            auto result = it->get<LD::UInteger>();
            return LD::Optional<LD::UInteger>{result};
        }
        return LD::nullopt;
    }
    LD::Optional<LD::Float> JsonConfiguration::operator()(LD::StringView key, LD::Type<LD::Float>) noexcept
    {
        auto it = mConfiguration.find(key.data());
        if (it != mConfiguration.end() && it->is_number_float())
        {
            auto result = it->get<LD::Float>();
            return LD::Optional<LD::Float>{result};
        }
        return LD::nullopt;}
     LD::Optional<LD::UniquePointer<Configuration,LD::MemoryResourceDeleter>> JsonConfiguration::operator()(LD::StringView key, LD::Type<Configuration>) noexcept
    {
        auto it = mConfiguration.find(key.data());


        if (it != mConfiguration.end() && it->is_object())
        {
            LD::Mem::PolymorphicAllocator<Configuration> polymorphicAllocator{this->mMemoryResource};
            //new JsonConfiguration{this->mMemoryResource,*it};

            //LD::MakeUnique<JsonConfiguration>(this->mMemoryResource,*it);
            LD::UniquePointer<Configuration,LD::MemoryResourceDeleter> ptr = LD::AllocateUnique<JsonConfiguration>(JsonConfiguration{this->mMemoryResource,*it});
            //LD::UniquePointer<Configuration,LD::MemoryResourceDeleter> ptr = LD::AllocateUnique<JsonConfiguration>(
            //&this->mMemoryResource,
            //this->mMemoryResource,
            //*it);
            return LD::Optional<LD::UniquePointer<Configuration,LD::MemoryResourceDeleter>>{LD::Move(ptr)};
            return LD::nullopt;
        }
        return LD::nullopt;
    }

    LD::Optional<bool> JsonConfiguration::operator()(LD::StringView key, LD::Type<bool>) noexcept
    {
        auto it = mConfiguration.find(key.data());
        if (it != mConfiguration.end() && it->is_boolean())
        {
            auto result = it->get<bool>();
            return LD::Optional<bool>{result};
        }
        return LD::nullopt;
    }

    LD::Optional<LD::StringView> JsonConfiguration::operator()(LD::StringView key, LD::Type<LD::StringView>) noexcept
    {

        auto it = mConfiguration.find(key.data());
        if (it != mConfiguration.end() && it->is_string())
        {
            //LD::ImmutableString<64> ret;
            return LD::Optional<LD::StringView>{it->get<LD::StringView>()};
        }
        return LD::nullopt;
    }
}