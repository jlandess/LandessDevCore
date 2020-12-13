//
// Created by phoenixflower on 11/15/20.
//

#ifndef LANDESSDEVCORE_PHONENUMBER_HPP
#define LANDESSDEVCORE_PHONENUMBER_HPP
#include "Immutable.hpp"
#include "StringView.hpp"
#include "Sovereignty/Countries.h"
#include "Memory/Optional.h"
#include "IO/FetchRequest.h"
#include "Primitives/General/ctre.hpp"
#include "Algorithms/FromString.hpp"
namespace LD
{
    namespace Detail
    {
        static constexpr auto  PhoneRegularExpression0 = ctll::basic_fixed_string{"(\\d{3})-(\\d{3})-(\\d{4})"};
        static constexpr auto  PhoneRegularExpression1 = ctll::basic_fixed_string{"(\\(\\d{3}\\)) (\\d{3})-(\\d{4})"};
        static constexpr auto  PhoneRegularExpression2 = ctll::basic_fixed_string{"(\\d{10,15})"};
    }
    class PhoneNumber
    {
    private:
        LD::Country mCountryOfOrigin;
        LD::UInteger mNumber;
        LD::Optional<unsigned short> Extension;
        LD::Optional<char> mStartDigit;
    public:
        PhoneNumber() noexcept:mCountryOfOrigin{LD::UnitedStatesOfAmerica}
        {

        }
        PhoneNumber( char startingDigit, LD::UInteger number) noexcept:mNumber{number},mCountryOfOrigin{LD::UnitedStatesOfAmerica},mStartDigit{startingDigit}
        {

        }
        PhoneNumber(LD::UInteger number) noexcept:mNumber{number},mCountryOfOrigin{LD::UnitedStatesOfAmerica},mStartDigit{}
        {

        }
        PhoneNumber(LD::StringView number) noexcept :mCountryOfOrigin{LD::UnitedStatesOfAmerica}
        {

        }

        template<typename ... A>
        LD::QueryResult<PhoneNumber(A...)> operator()(LD::Type<PhoneNumber>, LD::StringView, A && ... args) noexcept
        {
            return LD::MakeContext(LD::TransactionError{},LD::Forward<A>(args)...);
        }

        LD::Country CountryOfOrigin() const noexcept
        {
            return this->mCountryOfOrigin;
        }
        const auto  Number() const noexcept
        {
            return LD::ToImmutableString(this->mNumber);
        }
    };


    template<typename ... A>
    LD::QueryResult<LD::PhoneNumber(A...)> FromStringView(LD::Type<LD::PhoneNumber>, LD::StringView view, A && ... args) noexcept
    {
        if (auto [whole, zipCode,Prefix,LineNumber] = ctre::match<LD::Detail::PhoneRegularExpression0>(view); whole)
        {

        }else if (auto [whole, zipCode,Prefix,LineNumber] = ctre::match<LD::Detail::PhoneRegularExpression1>(view); whole)
        {

        }else if (auto [whole, phoneNumber] = ctre::match<LD::Detail::PhoneRegularExpression2>(view); whole)
        {
            LD::PhoneNumber number;
            if(phoneNumber.view()[0] == '0')
            {
                auto restuOfNumber = phoneNumber.view().substr(1,phoneNumber.view().size());
                auto parsedNumber = LD::FromStringView(LD::Type<LD::UInteger>{},restuOfNumber);
                auto onParseError = [](const LD::Context<LD::TransactionError> & context) noexcept -> LD::Optional<LD::UInteger>
                {
                    return LD::Optional<LD::UInteger>{};
                };
                auto onParser = [](const LD::Context<LD::TransactionResult,LD::UInteger> & context) noexcept -> LD::Optional<LD::UInteger>
                {
                    //unsigned short value = LD::Get(LD::Get<1>(context));
                    return LD::Optional<LD::UInteger>{LD::Get(LD::Get<1>(context))};
                };
                LD::Optional<LD::UInteger> possibleNumber = LD::MultiMatch(LD::Overload{onParseError,onParser},parsedNumber);
                if (possibleNumber)
                {
                    LD::UInteger number = (*possibleNumber);
                    //std::cout << "Phone Number: " << view << "       Number:" << LD::UInteger(number) << std::endl;
                    return LD::MakeContext(LD::TransactionResult{},LD::PhoneNumber{char{phoneNumber.view()[0]},LD::UInteger {number}},LD::Forward<A>(args)...);
                }
            }else
            {
                auto number = phoneNumber.view();
                auto parsedNumber = LD::FromStringView(LD::Type<LD::UInteger>{},number);
                auto onParseError = [](const LD::Context<LD::TransactionError> & context) noexcept -> LD::Optional<LD::UInteger>
                {
                    return LD::Optional<LD::UInteger>{};
                };
                auto onParser = [](const LD::Context<LD::TransactionResult,LD::UInteger> & context) noexcept -> LD::Optional<LD::UInteger>
                {
                    //unsigned short value = LD::Get(LD::Get<1>(context));
                    return LD::Optional<LD::UInteger>{LD::Get(LD::Get<1>(context))};
                };
                LD::Optional<LD::UInteger> possibleNumber = LD::MultiMatch(LD::Overload{onParseError,onParser},parsedNumber);
                if (possibleNumber)
                {
                    LD::UInteger number = (*possibleNumber);
                    //std::cout << "Phone Number: " << view << "       Number:" << LD::UInteger(number) << std::endl;
                    return LD::MakeContext(LD::TransactionResult{},LD::PhoneNumber{number},LD::Forward<A>(args)...);
                }
            }
        }
        return LD::MakeContext(LD::TransactionError{},LD::Forward<A>(args)...);
    }
    //auto ToImmutableString(const PhoneNumber & number) noexcept
    //{
     //   return LD::ImmutableString{"+"}+LD::ToImmutableString(number.CountryCode())+number.Number();
    //}
}
#endif //LANDESSDEVCORE_PHONENUMBER_HPP
