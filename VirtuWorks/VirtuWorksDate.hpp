//
// Created by phoenixflower on 11/21/20.
//

#ifndef LANDESSDEVCORE_VIRTUWORKSDATE_HPP
#define LANDESSDEVCORE_VIRTUWORKSDATE_HPP
#include "Chrono/DateTime.h"
#include "Primitives/General/StringView.hpp"
#include "Primitives/General/ctre.hpp"
#include "IO/FetchRequest.h"
//#include "Algorithms/FromString.hpp"
namespace LD
{
    namespace VW
    {
        namespace Detail
        {
            static constexpr auto  DateRegularExpression = ctll::basic_fixed_string{"(\\d{4})-(\\d{1,2})-(\\d{1,2})"};
        }
        class Date
        {
        private:
            LD::Date mDate;
        public:
            Date()
            {

            }
            Date(const LD::Date & date) noexcept:mDate{date}
            {

            }
        };
    }
}
namespace LD
{
    template<typename ... A>
    LD::QueryResult<LD::VW::Date(A...)> FromStringView(LD::Type<LD::VW::Date>, LD::StringView view, A && ... args) noexcept
    {

        if (auto [whole, year, month, day] = ctre::match<LD::VW::Detail::DateRegularExpression>(view); whole)
        {
            //LD::StringToPrimitive<LD::UInteger > converter{};
            //LD::UInteger yearAsNumber = LD::Match(converter(year),[](auto){ return LD::UInteger {0};},[](const LD::UInteger & obj){return obj;});
            //LD::UInteger monthAsNumber = LD::Match(converter(month),[](auto){ return LD::UInteger {0};},[](const LD::UInteger & obj){return obj;});
            //LD::UInteger dayAsNumber = LD::Match(converter(day),[](auto){ return LD::UInteger {0};},[](const LD::UInteger & obj){return obj;});
            return LD::MakeContext(LD::TransactionResult{},LD::VW::Date{LD::Date{7,7,7}},LD::Forward<A>(args)...);
        }
        return LD::MakeContext(LD::TransactionError{},LD::Forward<A>(args)...);
    }
}
#endif //LANDESSDEVCORE_VIRTUWORKSDATE_HPP
