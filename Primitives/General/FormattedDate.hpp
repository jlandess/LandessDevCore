//
// Created by phoenixflower on 1/6/21.
//

#ifndef LANDESSDEVCORE_FORMATTEDDATE_HPP
#define LANDESSDEVCORE_FORMATTEDDATE_HPP
#include "Chrono/DateTime.h"
#include "TypeTraits/TypeList.hpp"
#include "TypeTraits/Detection.hpp"
#include "Immutable.hpp"
/*
namespace LD
{
    class Day
    {

    };
    class Month
    {

    };
    class Year
    {

    };
    template<typename TL, class  = void>
    class FormattedDate;

    template<typename First, typename Second, typename Third>
    class FormattedDate<LD::CT::TypeList<First,Second,Third>,
            LD::Enable_If_T<
            LD::Require<
                    LD::CT::IsSameWithoutQualifiers(LD::Type<First>{},LD::Type<Day>{}),
                    LD::CT::IsSameWithoutQualifiers(LD::Type<Second>{},LD::Type<Month>{}),
                    LD::CT::IsSameWithoutQualifiers(LD::Type<Third>{},LD::Type<Year>{})
            >>>
    {
    private:
        LD::Date * mDate;
    public:
        FormattedDate(LD::Date * date) noexcept:mDate{date}
        {

        }
        LD::Date * Date() const
        {
            return this->mDate;
        }

    };

    template<typename First, typename Second, typename Third>
    inline auto ToImmutableString(FormattedDate<LD::CT::TypeList<First,Second,Third>> formattedDate) noexcept
    {
        LD::Date * date = formattedDate.Date();
        LD::ImmutableString<(2+2+4)+2> dateAsString;
        auto yearAsImmutableString = LD::ToImmutableString(date->Year());
        auto monthAsImmutableString = LD::ToImmutableString(date->Month());
        auto dayAsImmutableString = LD::ToImmutableString(date->Day());
        LD::UInteger n = 0;
        if constexpr (
                LD::CT::IsSameWithoutQualifiers(LD::Type<First>{},LD::Type<LD::Day>{}),
                        LD::CT::IsSameWithoutQualifiers(LD::Type<Second>{},LD::Type<LD::Month>{}),
                        LD::CT::IsSameWithoutQualifiers(LD::Type<Third>{},LD::Type<LD::Year>{}))
        {
            if (date->Day() < 10)
            {
                dateAsString[n++] = '0';
                dateAsString[n++] =  dayAsImmutableString[0];
            }else
            {
                dateAsString[n++] = dayAsImmutableString[0];
                dateAsString[n++] =  dayAsImmutableString[1];
            }

            dateAsString[n++] = '-';

            if (date->Month() < 10)
            {
                dateAsString[n++] = '0';
                dateAsString[n++] =  monthAsImmutableString[0];
            }else
            {
                dateAsString[n++] = monthAsImmutableString[0];
                dateAsString[n++] =  monthAsImmutableString[1];
            }

            dateAsString[n++] = '-';

            for(LD::UInteger i =0;i<4;++i)
            {
                dateAsString[n++] = yearAsImmutableString[i];
            }

        }
        else if constexpr (
                LD::CT::IsSameWithoutQualifiers(LD::Type<Second>{},LD::Type<LD::Year>{}),
                        LD::CT::IsSameWithoutQualifiers(LD::Type<First>{},LD::Type<LD::Month>{}),
                        LD::CT::IsSameWithoutQualifiers(LD::Type<Third>{},LD::Type<LD::Day>{}))
        {
            for(LD::UInteger i=0;i<4;++i)
            {
                dateAsString[n++] = yearAsImmutableString[i];
            }

            dateAsString[n++] = '-';

            if (date->Month() < 10)
            {
                dateAsString[n++] = '0';
                dateAsString[n++] =  monthAsImmutableString[0];
            }else
            {
                dateAsString[n++] = monthAsImmutableString[0];
                dateAsString[n++] =  monthAsImmutableString[1];
            }
            dateAsString[n++] = '-';

            if (date->Day() < 10)
            {
                dateAsString[n++] = '0';
                dateAsString[n++] =  dayAsImmutableString[0];
            }else
            {
                dateAsString[n++] = dayAsImmutableString[0];
                dateAsString[n++] =  dayAsImmutableString[1];
            }


        }else if constexpr (
                LD::CT::IsSameWithoutQualifiers(LD::Type<Second>{},LD::Type<LD::Month>{}),
                        LD::CT::IsSameWithoutQualifiers(LD::Type<First>{},LD::Type<LD::Day>{}),
                        LD::CT::IsSameWithoutQualifiers(LD::Type<Third>{},LD::Type<LD::Year>{}))
        {
            if (date->Month() < 10)
            {
                dateAsString[n++] = '0';
                dateAsString[n++] =  monthAsImmutableString[0];
            }else
            {
                dateAsString[n++] = monthAsImmutableString[0];
                dateAsString[n++] =  monthAsImmutableString[1];
            }
            dateAsString[n++] = '-';
            if (date->Day() < 10)
            {
                dateAsString[n++] = '0';
                dateAsString[n++] =  dayAsImmutableString[0];
            }else
            {
                dateAsString[n++] = dayAsImmutableString[0];
                dateAsString[n++] =  dayAsImmutableString[1];
            }
            dateAsString[n++] = '-';
            for(LD::UInteger i =0;i<4;++i)
            {
                dateAsString[n++] = yearAsImmutableString[i];
            }


        }
        return dateAsString;
    }
}
 */
#endif //LANDESSDEVCORE_FORMATTEDDATE_HPP
