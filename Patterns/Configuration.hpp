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
namespace LD
{
    class Configuration
    {
    public:
        virtual LD::Optional<LD::Integer> operator()(LD::StringView key, LD::Type<LD::Integer>) noexcept = 0;
        virtual LD::Optional<LD::Float> operator()(LD::StringView key, LD::Type<LD::Float>) noexcept = 0;
        virtual LD::Optional<Configuration *> operator()(LD::StringView key, LD::Type<Configuration>) noexcept = 0;
        virtual LD::Optional<bool> operator()(LD::StringView key, LD::Type<bool>) noexcept = 0;
        virtual LD::Optional<LD::StringView> operator()(LD::StringView key, LD::Type<LD::StringView>) noexcept = 0;
    };


    class BasicConfiguration
    {
    private:
        Configuration & mConfiguration;
    public:

        class BasicConfigurationInsertion
        {
        public:
            template<typename T>
            LD::Enable_If_T<LD::Require<LD::IsInteger<T>>,BasicConfiguration &> operator >> (LD::Optional<T> & possibleInteger) noexcept;

            template<typename T>
            LD::Enable_If_T<LD::Require<LD::IsFloatingPoint<T>>,BasicConfiguration &> operator >> (LD::Optional<T> & possibleInteger) noexcept;

            template<typename T>
            LD::Enable_If_T<LD::Require<LD::CT::IsSame(LD::Type<bool>{},LD::Type<T>{})>,BasicConfiguration &> operator >> (LD::Optional<T> & possibleInteger) noexcept;

            template<LD::UInteger Size>
            BasicConfiguration & operator >> (LD::Optional<LD::ImmutableString<Size>> & possibleInteger) noexcept;

            template<typename BackInserterType>
            BasicConfiguration & operator >> (BackInserterType backInserter) noexcept;
        };
        BasicConfiguration(Configuration & configuration) noexcept:mConfiguration{configuration}
        {

        }

        BasicConfigurationInsertion operator[](LD::StringView key) noexcept
        {
            return BasicConfigurationInsertion{};
        }

    };
}
#endif //LANDESSDEVCORE_CONFIGURATION_HPP
