//
// Created by phoenixflower on 11/20/20.
//

#ifndef LANDESSDEVCORE_IDENTIFICATION_HPP
#define LANDESSDEVCORE_IDENTIFICATION_HPP
#include "Definitions/Integer.hpp"
#include "Primitives/General/ctre.hpp"
#include "Algorithms/StringAsNumber.h"
//#include "Algorithms/FromString.hpp"
namespace LD
{
    namespace VW
    {
        namespace Detail
        {
            static constexpr auto  HexademicalRegex = ctll::basic_fixed_string{"(0x|0X)?[a-fA-F0-9]+"};
            //ctll::fixed_string{ "^-?\\d*\\.{0,1}\\d+$" };
            static constexpr auto  CallIDRegularExpression = ctll::basic_fixed_string{"(vir-ast-fe-1.virtuworks.net)-(^-?\\d*\\.{0,1}\\d+$)"};
        }
        class CallIdentification
        {
        private:
            LD::UInteger mStart;
            LD::UInteger mEnd;
        public:
        };
    }

    /*
    template<typename ... A>
    LD::QueryResult<LD::VW::CallIdentification(A...)> FromStringView(LD::Type<LD::VW::CallIdentification>, LD::StringView view, A && ... args) noexcept
    {
        if (auto [whole, hexID] = ctre::match<LD::VW::Detail::HexademicalRegex>(view); whole)
        {
            auto onError = [](auto) noexcept
            {
                return LD::Optional<LD::Float>{};
            };

            auto onCompanyID = [](const LD::Context<LD::TransactionResult,LD::Float> & context) noexcept
            {
                return LD::Optional<LD::Float>{LD::Get(LD::Get<1>(context))};
            };
            auto toDouble = LD::FromStringView(LD::Type<LD::Float>{},hexID.view());
            LD::Optional<LD::Float> maybeFloatingPoint = LD::MultiMatch(LD::Overload{onError,onCompanyID},toDouble);
            if (maybeFloatingPoint)
            {

            }
            return LD::MakeContext(LD::TransactionError{},LD::Forward<A>(args)...);

        }else if (auto [whole, companyID,phonecallID] = ctre::match<LD::VW::Detail::CallIDRegularExpression>(view); whole)
        {

            auto onError = [](auto) noexcept
            {
                return LD::Optional<LD::Float>{};
            };

            auto onCompanyID = [](const LD::Context<LD::TransactionResult,LD::Float> & context) noexcept
            {
                return LD::Optional<LD::Float>{LD::Get(LD::Get<1>(context))};
            };


            auto toDouble = LD::FromStringView(LD::Type<LD::Float>{},phonecallID.view());
            LD::Optional<LD::Float> maybeFloatingPoint = LD::MultiMatch(LD::Overload{onError,onCompanyID},toDouble);
            return LD::MakeContext(LD::TransactionError{},LD::Forward<A>(args)...);
        }
        return LD::MakeContext(LD::TransactionError{},LD::Forward<A>(args)...);
    }
     */
}
#endif //LANDESSDEVCORE_IDENTIFICATION_HPP
