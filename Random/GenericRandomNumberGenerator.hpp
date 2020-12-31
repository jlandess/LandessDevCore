//
// Created by phoenixflower on 12/22/20.
//

#ifndef LANDESSDEVCORE_GENERICRANDOMNUMBERGENERATOR_HPP
#define LANDESSDEVCORE_GENERICRANDOMNUMBERGENERATOR_HPP
#include "TypeTraits/Detection.hpp"
#include "TypeTraits/Declval.hpp"
#include "TypeTraits/Conditional.hpp"
#include "Primitives/General/Context.h"
namespace LD
{
    namespace Detail
    {
        template<typename T>
        using GeneratedIntegerType = decltype(LD::Declval<T>().NextInteger());

        template<typename T>
        using GeneratedFloatingType = decltype(LD::Declval<T>().NextFloatingPoint());
    }
    template<typename T, class = void>
    class GenericRandomIntegerGenerator;

    template<typename T, class = void>
    class GenericRandomFloatingPointGenerator;
    template<typename T>
    class GenericRandomIntegerGenerator<T,LD::Enable_If_T<
            LD::Require<
                    LD::Exists<LD::Detail::GeneratedIntegerType,T>
            >>>
    {
    private:
        using UnderlyingType = LD::Type<LD::Detail::Conditional_T<(LD::Detail::IsLValueReference<T>::value),LD::ElementReference <LD::Detail::Decay_T<T>>,LD::Detail::Decay_T<T>>>;
        LD::Detail::Conditional_T<(LD::Detail::IsLValueReference<T>::value),LD::ElementReference <LD::Detail::Decay_T<T>>,LD::Detail::Decay_T<T>> mGen;
    public:
        inline GenericRandomIntegerGenerator(T && random) noexcept:mGen{LD::Forward<T>(random)}
        {

        }
        typename LD::Detail::GeneratedIntegerType<T> operator()()  noexcept
        {
            if constexpr (LD::CT::IsElementReference(UnderlyingType{}))
            {
                return this->mGen->NextInteger();
            }else
            {
                return this->mGen.NextInteger();
            }
        }
    };

    template<typename T>
    class GenericRandomFloatingPointGenerator<T,
            LD::Enable_If_T<
                    LD::Require<
                            LD::Exists<LD::Detail::GeneratedFloatingType,T>
                    >>>
    {
    private:
        using UnderlyingType = LD::Type<LD::Detail::Conditional_T<(LD::Detail::IsLValueReference<T>::value),LD::ElementReference <LD::Detail::Decay_T<T>>,LD::Detail::Decay_T<T>>>;
        LD::Detail::Conditional_T<(LD::Detail::IsLValueReference<T>::value),LD::ElementReference <LD::Detail::Decay_T<T>>,LD::Detail::Decay_T<T>> mGen;
    public:
        inline GenericRandomFloatingPointGenerator(T && random) noexcept:mGen{LD::Forward<T>(random)}
        {

        }
        typename LD::Detail::GeneratedFloatingType<T>  operator()()  noexcept
        {
            if constexpr (LD::CT::IsElementReference(UnderlyingType{}))
            {
                return this->mGen->NextFloatingPoint();
            }else
            {
                return this->mGen.NextFloatingPoint();
            }
        }
    };

    template<typename T> GenericRandomFloatingPointGenerator(T && ) -> GenericRandomFloatingPointGenerator<T>;
    template<typename T> GenericRandomIntegerGenerator(T && ) -> GenericRandomIntegerGenerator<T>;
}
#endif //LANDESSDEVCORE_GENERICRANDOMNUMBERGENERATOR_HPP
