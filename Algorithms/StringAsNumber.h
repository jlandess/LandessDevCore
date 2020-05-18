//
// Created by phoenixflower on 5/17/20.
//

#ifndef LANDESSDEVCORE_STRINGASNUMBER_H
#define LANDESSDEVCORE_STRINGASNUMBER_H
#include "Primitives/General/ctre.hpp"
#include "Primitives/General/mapboxvariant.hpp"
#include "Primitives/General/mapboxvariantvisitor.h"
#include "Definitions/Integer.hpp"
#include "Primitives/General/StringView.hpp"
#include "Algorithms/Exponential.hpp"
namespace LD
{
    namespace Detail
    {
        static constexpr auto UnsignedNumberPattern = ctll::fixed_string{ "^\\d+$" };
        static constexpr auto SignedNumberPatern = ctll::fixed_string{ "^-?\\d+$" };
        static constexpr auto FloatingPointNumberPattern = ctll::fixed_string{ "^-?\\d*\\.{0,1}\\d+$" };
        static constexpr auto NotCommaPattern = ctll::fixed_string{ "[^\\.]+" };
        constexpr const bool IsIntegerNumber(const LD::StringView & view) noexcept
        {

            return ctre::match<UnsignedNumberPattern>(view);
        }

        constexpr const bool IsSignedIntegerNumber(const LD::StringView & view) noexcept
        {

            return ctre::match<SignedNumberPatern>(view);
        }

        constexpr const bool IsFloatingPointNumber(const LD::StringView & view) noexcept
        {
            return  ctre::match<FloatingPointNumberPattern>(view);;
        }
    }
    class NotANumber
    {

    };
    template<typename T>
    constexpr LD::Enable_If_T<
            LD::Require<
                    LD::IsUnsignedInteger<T>
            >,
            LD::Variant<LD::NotANumber,T>> StringAsNumber(const LD::StringView & view) noexcept
    {
        const LD::UInteger stringSize = view.size();
        if (view.data() == nullptr || stringSize == 0)
        {
            return {};
        }
        if(!LD::Detail::IsIntegerNumber(view))
        {
            return {};
        }


        T sum = {0};
        //sum = view[0]-'0';
        for (LD::UInteger i = 0; i < view.size(); ++i)
        {
            sum += LD::Power(10,view.size()-i-1)*(view[i]-'0');
        }
        return {sum};
    }
    template<typename T>
    constexpr LD::Enable_If_T<
            LD::Require<
                    LD::IsInteger<T>
            >,
            LD::Variant<LD::NotANumber,T>> StringAsNumber(const LD::StringView & view) noexcept
    {

        const LD::UInteger stringSize = view.size();
        if (view.data() == nullptr || stringSize == 0)
        {
            return {};
        }
        if(!LD::Detail::IsSignedIntegerNumber(view))
        {
            return {};
        }
        const bool isNegative = (view[0] == '-');
        T sign = 1*(!isNegative) + -1*(isNegative);
        T sum = {0};
        //sum = view[0]-'0';
        for (LD::UInteger i = isNegative; i < view.size(); ++i)
        {
            sum += LD::Power(10,view.size()-i-1)*(view[i]-'0');
        }
        return {sum*sign};
    }
    template<typename T>
    constexpr LD::Enable_If_T<
            LD::Require<
                    LD::IsFloatingPoint<T>
            >,
            LD::Variant<LD::NotANumber,T>> StringAsNumber(const LD::StringView & view) noexcept
    {
        if (LD::Detail::IsSignedIntegerNumber(view))
        {
            const LD::UInteger stringSize = view.size();
            if (view.data() == nullptr || stringSize == 0)
            {
                return {};
            }
            const bool isNegative = (view[0] == '-');
            T sign = 1*(!isNegative) + -1*(isNegative);
            T sum = {0};
            //sum = view[0]-'0';
            for (LD::UInteger i = isNegative; i < view.size(); ++i)
            {
                sum += LD::Power(10,view.size()-i-1)*(view[i]-'0');
            }
            return {sum*sign};
        }else if(LD::Detail::IsFloatingPointNumber(view))
        {
            auto match = ctre::range<LD::Detail::NotCommaPattern>(view);
            auto it = match.begin();
            LD::StringView firstSegment = (*it).get<0>();
            ++it;
            LD::StringView secondSegment = (*it).get<0>();
            T sum = {0};
            const bool isNegative = (firstSegment[0] == '-');
            T sign = 1*(!isNegative) + -1*(isNegative);
            for (LD::UInteger i = isNegative; i < firstSegment.size(); ++i)
            {
                sum += LD::Power(10,firstSegment.size()-i-1)*(firstSegment[i]-'0');
            }
            T secondarySum = {0};
            for (LD::UInteger i = 0; i < secondSegment.size(); ++i)
            {
                secondarySum += LD::Power(10,secondSegment.size()-i-1)*(secondSegment[i]-'0');
            }
            sum+=(secondarySum/LD::Power(10,secondSegment.size()));
            return sum*sign;
        }
        return {};
    }
    template<typename T>
    constexpr LD::Enable_If_T<
            LD::Require<
                    LD::IsSame<T,bool>
            >,
            LD::Variant<LD::NotANumber,T>> StringAsNumber(const LD::StringView & view) noexcept
    {
        const LD::UInteger stringSize = view.size();
        if (view.data() == nullptr || stringSize == 0)
        {
            return {};
        }
        if(!LD::Detail::IsIntegerNumber(view))
        {
            return {};
        }
        T sum = {0};
        for (LD::UInteger i = 0; i < view.size(); ++i)
        {
            sum += LD::Power(10,view.size()-i-1)*(view[i]-'0');
        }
        return {sum};
    }
}
#endif //LANDESSDEVCORE_STRINGASNUMBER_H
