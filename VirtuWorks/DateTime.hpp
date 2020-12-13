//
// Created by phoenixflower on 11/26/20.
//

#ifndef LANDESSDEVCORE_VW_DATETIME_HPP
#define LANDESSDEVCORE_VW_DATETIME_HPP

#include "Primitives/General/DateTime.h"
#include "Primitives/General/StringView.hpp"
#include "Primitives/General/ctre.hpp"
#include "IO/FetchRequest.h"
namespace LD
{
    namespace VW
    {
        namespace Detail
        {
            static constexpr auto  DateTimeRegularExpression = ctll::basic_fixed_string{"\"(\\d{4})-(\\d{1,2})-(\\d{1,2}) (\\d{1,2}):(\\d{1,2}):(\\d{1,2})\""};
        }
        class DateTime
        {
        private:
            LD::Date mDate;
            LD::Time mTime;
        public:
            DateTime() noexcept
            {

            }
            DateTime(LD::Date date, LD::Time time) noexcept:mDate{date},mTime{time}
            {

            }

            const LD::Date & Date() const noexcept
            {
                return this->mDate;
            }

            const LD::Time & Time() const noexcept
            {
                return this->mTime;
            }
        };
    }
}

namespace LD
{
    template<typename ... A>
    LD::QueryResult<LD::VW::DateTime(A...)> FromStringView(LD::Type<LD::VW::DateTime>, LD::StringView view, A && ... args) noexcept
    {
        //std::cout << "DateTime: " << view << std::endl;
        if (auto [whole, yearView, monthView, dayView,hourView,minuteView,secondView] = ctre::match<LD::VW::Detail::DateTimeRegularExpression>(view); whole)
        {
            //std::cout << "DateTime Year: " << yearView.view() << std::endl;
            LD::UInteger year = (yearView.view()[0]-'0')*1000 + (yearView.view()[1] - '0')*100 + (yearView.view()[2] - '0')*10 + (yearView.view()[3] - '0')*1;
            //std::cout << "DateTime Calculation: " << year << std::endl;
            LD::UInteger month = (monthView.view()[0]-'0')*10 + (monthView.view()[1] - '0')*1;
            LD::UInteger day = (dayView.view()[0] -'0')*10 + (dayView.view()[1]-'0')*1;

            LD::UInteger hour = (hourView.view()[0]-'0')*10 + (hourView.view()[1]-'0')*1;
            LD::UInteger minute = (minuteView.view()[0]-'0')*10 + (minuteView.view()[1]-'0')*1;
            LD::UInteger second = (secondView.view()[0]-'0')*10 + (secondView.view()[1] - '0')*1;

            return LD::MakeContext(LD::TransactionResult{},LD::VW::DateTime{LD::Date(year,month,day),LD::Time(hour,minute,second)},LD::Forward<A>(args)...);
        }
        return LD::MakeContext(LD::TransactionError{},LD::Forward<A>(args)...);
    }
}
#endif //LANDESSDEVCORE_DATETIME_HPP
