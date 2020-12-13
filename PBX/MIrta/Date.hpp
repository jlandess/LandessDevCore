//
// Created by phoenixflower on 11/28/20.
//

#ifndef LANDESSDEVCORE_Mirta_DATE_HPP
#define LANDESSDEVCORE_Mirta_DATE_HPP
#include "Primitives/General/DateTime.h"
namespace LD
{
    namespace PBX
    {
        namespace MT
        {
            class Date
            {
            private:
                LD::Date mDate;
            public:
                Date() = default;
                Date(LD::Date date) noexcept:mDate{date}
                {

                }
                Date & operator = (const LD::Date & date) noexcept
                {
                    this->mDate = date;
                    return (*this);
                }

                int Day() const noexcept{return this->mDate.Day();}
                int Month() const noexcept{return this->mDate.Month();}
                int Year() const noexcept{return this->mDate.Year();}

                int & Day() noexcept{return this->mDate.Day();}
                int & Month() noexcept{return this->mDate.Month();}
                int & Year() noexcept{return this->mDate.Year();}

                bool operator < (const LD::PBX::MT::Date & date) const noexcept
                {
                    return this->mDate < date.mDate;
                }

                bool operator > (const LD::PBX::MT::Date & date) const noexcept
                {
                    return this->mDate > date.mDate;
                }

                bool operator == (const LD::PBX::MT::Date & date) const noexcept
                {
                    return this->mDate == date.mDate;
                }
                bool operator != (const LD::PBX::MT::Date & date) const noexcept
                {
                    return this->mDate != date.mDate;
                }

                bool operator <= (const LD::PBX::MT::Date & date) const noexcept
                {
                    return this->mDate <= date.mDate;
                }

                bool operator >= (const LD::PBX::MT::Date & date) const noexcept
                {
                    return this->mDate >= date.mDate;
                }

                LD::PBX::MT::Date & operator++() noexcept
                {
                    this->mDate++;
                    return (*this);
                }
                LD::PBX::MT::Date & operator++(int) noexcept
                {
                    this->mDate++;
                    return (*this);
                }

                LD::PBX::MT::Date & operator--() noexcept
                {
                    this->mDate--;
                    return (*this);
                }
                LD::PBX::MT::Date & operator--(int) noexcept
                {
                    this->mDate--;
                    return (*this);
                }

            };
        }
    }

    //auto ToImmutableString(const LD::PBX::MT::Date & date) noexcept
    //{
     //   return LD::ToImmutableString(date.Year()) + LD::ImmutableString{"-"} + LD::ToImmutableString(date.Month()) + LD::ImmutableString{"-"} + LD::ToImmutableString(date.Day());
    //}

    inline LD::ImmutableString<(2+2+4)+2> ToImmutableString(const LD::PBX::MT::Date & date) noexcept
    {
        auto yearAsImmutableString = LD::ToImmutableString(date.Year());
        LD::ImmutableString<(2+2+4)+2> dateAsString;
        auto monthAsImmutableString = LD::ToImmutableString(date.Month());
        for(LD::UInteger n =0;n<4;++n)
        {
            dateAsString[n] = yearAsImmutableString[n];
        }
        LD::UInteger monthOffset = 0;
        dateAsString[4] = '-';
        if (date.Month() < 10)
        {
            dateAsString[5] = '0';
            monthAsImmutableString[6] = monthAsImmutableString[0];
        }else
        {
            dateAsString[5] = monthAsImmutableString[0];
            dateAsString[6] = monthAsImmutableString[1];
        }
        auto dayAsImmutableString = LD::ToImmutableString(date.Day());
        dateAsString[7] = '-';
        if (date.Day() < 10)
        {
            dateAsString[8] = '0';
            dateAsString[9] = dayAsImmutableString[0];
        }else
        {
            dateAsString[8] = dayAsImmutableString[0];
            dateAsString[9] = dayAsImmutableString[1];
        }
        return dateAsString;
        //return LD::ToImmutableString(date.Year()) + LD::ImmutableString{"-"} + LD::ToImmutableString(date.Month()) + LD::ImmutableString{"-"} + LD::ToImmutableString(date.Day());
    }
}
#endif //LANDESSDEVCORE_DATE_HPP
