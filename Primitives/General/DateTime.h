//
// Created by phoenixflower on 11/14/20.
//

#ifndef LANDESSDEVCORE_DATETIME_HPP
#define LANDESSDEVCORE_DATETIME_HPP
#ifndef H__DATE_H
#define H__DATE_H

#include <ctime>
#include "Algorithms/Exponential.hpp"
#include "Primitives/General/ctre.hpp"
#include "Primitives/General/StringView.hpp"
//#include "Algorithms/FromString.hpp"
//
//  Date.h
//  DataStructures
//
//  Created by James Landess on 10/23/14.
//  Copyright (c) 2014 LandessDev. All rights reserved.
//
namespace LD
{
    class Date
    {
    private:
        int mYear;
        int mMonth;
        int mDay;
        LD::Date  NextDate() const noexcept
        {
            LD::Date date = (*this);
            if (this->Valid())
            {
                LD::Date date = LD::Date{this->mYear,this->mMonth,this->mDay+1};
                if (date.Valid())
                {
                    return date;
                }
                date = LD::Date{this->mYear,this->mMonth+1,1};
                if (date.Valid())
                {
                    return date;
                }
                date = LD::Date{this->mYear+1,1,1};
                return date;
            }
            date = (*this);
            return date;
        }
        LD::Date PreviousDate() const noexcept
        {
            LD::Date ndat = (*this);
            if (!ndat.Valid())
            {
                return ndat;
            }
            ndat = LD::Date{this->mYear,this->mMonth,this->mDay-1};
            if (ndat.Valid())
            {
                return ndat;
            }
            ndat = LD::Date{this->mYear,this->mMonth-1,31};
            if (ndat.Valid())
            {
                return ndat;
            }
            ndat = LD::Date{this->mYear,this->mMonth-1,30};
            if (ndat.Valid())
            {
                return ndat;
            }
            ndat = LD::Date{this->mYear,this->mMonth-1,29};
            if (ndat.Valid())
            {
                return ndat;
            }
            ndat = LD::Date{this->mYear,this->mMonth-1,28};
            if (ndat.Valid())
            {
                return ndat;
            }
            ndat = LD::Date{this->mYear-1,12,31};
            return ndat;

        }
    public:
        Date() noexcept:mYear{0},mMonth{0},mDay{0}{}
        Date(int year, int month, int day) noexcept:mYear{year},mMonth{month},mDay{day}{}

        bool Valid() const
        {
            if (this->mYear < 0)
            {
                return false;
            }
            if (this->mMonth > 12 || this->mMonth < 1)
            {
                return false;
            }
            if (this->mDay > 31 || this->mDay < 1)
            {
                return false;
            }
            if (this->mDay == 31 && (this->mMonth == 2 || this->mMonth == 4 || this->mMonth == 6 || this->mMonth == 9 || this->mMonth == 11))
            {
                return false;
            }
            if (this->mDay == 30 && this->mMonth == 2)
            {
                return false;
            }

            if (this->mYear < 2000)
            {
                if ((this->mDay == 29 && this->mMonth == 2) && !(this->mYear-1900)%4 == 0)
                {
                    return false;
                }
            }

            if (this->mYear > 2000)
            {
                if ((this->mDay == 29 && this->mMonth == 2) && !(this->mYear-2000)%4 == 0)
                {
                    return false;
                }
            }

            return true;
        }

        int Day() const noexcept{return this->mDay;}
        int Month() const noexcept{return this->mMonth;}
        int Year() const noexcept{return this->mYear;}

        int & Day() noexcept{return this->mDay;}
        int & Month() noexcept{return this->mMonth;}
        int & Year() noexcept{return this->mYear;}

        LD::Date & operator++() noexcept
        {
            (*this) = this->NextDate();
            return (*this);
        }
        LD::Date & operator++(int) noexcept
        {
            (*this) = this->NextDate();
            return (*this);
        }
        LD::Date & operator--() noexcept
        {
            (*this) = this->PreviousDate();
            return (*this);
        }
        LD::Date & operator--(int) noexcept
        {
            (*this) = this->PreviousDate();
            return (*this);
        }
        //DateTest & operator++() noexcept;
        //DateTest & operator++(int) noexcept;
        //DateTest & operator--() noexcept;
        //DateTest & operator--(int) noexcept;

        bool operator == (const Date & date) const noexcept
        {
            if (!this->Valid() || !date.Valid())
            {
                return false;
            }

            if (this->mDay == date.mDay && this->mMonth == date.mMonth && this->mYear == date.mYear)
            {
                return true;
            }
            return false;
        }
        bool operator != (const Date & date) const noexcept
        {
            return !((*this)==date);
        }
        bool operator < (const Date & date) const noexcept
        {
            if (!this->Valid() || !date.Valid())
            {
                return false;
            }
            if (this->mYear < date.mYear)
            {
                return true;
            }else if(this->mYear > date.mYear)
            {
                return false;
            }else
            {
                if (this->mMonth < date.mMonth)
                {
                    return true;
                }else if(this->mMonth > date.mMonth)
                {
                    return true;
                }else
                {
                    if (this->mDay < date.mDay)
                    {
                        return true;
                    }else
                    {
                        return false;
                    }
                }
            }
            return false;
        }
        bool operator > (const Date & date) const noexcept
        {
            if ((*this) == date)
            {
                return false;
            }
            if ((*this) < date)
            {
                return false;
            }
            return true;
        }
        bool operator <= (const Date & date) const noexcept
        {
            if ((*this) == date)
            {
                return true;
            }
            return ((*this) < date);
        }
        bool operator >= (const Date & date) const noexcept
        {
            if ((*this) == date)
            {
                return true;
            }
            return ((*this) > date);
        }
    };


    inline LD::ImmutableString<(2+2+4)+2> ToImmutableString(const LD::Date & date) noexcept
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
    //class Date;
    struct Time
    {
    private:
        unsigned short mHour;
        unsigned short mMinute;
        unsigned short mSecond;
    public:
        Time() noexcept:Time(time(0))
        {

        }
        Time(const unsigned  short hour, const unsigned short minute, const unsigned short second) noexcept:mHour{hour},mMinute{minute},mSecond{second}
        {

        }
        Time(const time_t tSysTime ) noexcept
        {
            struct tm *ptm;
            int y, m, d;
            short hour,minute,second;

            ptm = localtime( &tSysTime );

            y = ptm->tm_year + 1900;
            m = ptm->tm_mon + 1;
            d = ptm->tm_mday;
            hour = ptm->tm_hour;
            minute = ptm->tm_min;
            second = ptm->tm_sec;
        }

        unsigned short & Hour()  noexcept
        {
            return this->mHour;
        }
        unsigned short & Minute()  noexcept
        {
            return this->mMinute;
        }
        unsigned short & Second()  noexcept
        {
            return this->mSecond;
        }
        const unsigned short & Hour() const noexcept
        {
            return this->mHour;
        }
        const unsigned short & Minute() const noexcept
        {
            return this->mMinute;
        }
        const unsigned short & Second() const noexcept
        {
            return this->mSecond;
        }

        friend class Date;
    };


    namespace Detail
    {
        static constexpr auto  TimeRegularExpression0 = ctll::basic_fixed_string{"(\\d{1,2})-(\\d{1,2})-(\\d{1,2})"};
        static constexpr auto  TimeRegularExpression1 = ctll::basic_fixed_string{"(\\d{1,2}):(\\d{1,2}):(\\d{1,2})"};
        static constexpr auto  TimeRegularExpression2 = ctll::basic_fixed_string{"(\\d{1,2}),(\\d{1,2}),(\\d{1,2})"};
    }

    template<typename ... A>
    LD::QueryResult<LD::Time(A...)> FromStringView(LD::Type<LD::Time>, LD::StringView view, A && ... args) noexcept
    {
        if (auto [whole, hour, minute, second] = ctre::match<LD::Detail::TimeRegularExpression0>(view); whole)
        {
            LD::MakeContext(LD::TransactionResult{},LD::Time{hour,minute,second},LD::Forward<A>(args)...);
        }else if(auto [whole, hour, minute, second] = ctre::match<LD::Detail::TimeRegularExpression1>(view); whole)
        {
            LD::MakeContext(LD::TransactionResult{},LD::Time{hour,minute,second},LD::Forward<A>(args)...);
        }else if(auto [whole, hour, minute, second] = ctre::match<LD::Detail::TimeRegularExpression2>(view); whole)
        {
            LD::MakeContext(LD::TransactionResult{},LD::Time{hour,minute,second},LD::Forward<A>(args)...);
        }

        return LD::MakeContext(LD::TransactionError{},LD::Forward<A>(args)...);
    }


    /*
    class Date
    {

    private:

        long        lJulianDay;
        //Time CurrentTime;
        //
        // Function      : YmdToJd
        //
        // Author        : Todd Knarr
        //
        // Creation date : 29 Nov 1995
        //
        // Parameters    : int year, month, day
        //
        // Return values : long julian day
        //
        // Description   :
        //
        // Internal routine that does the physical conversion
        // from YMD form to Julian day number.
        //
        static const LD::Float YmdToJd( const LD::Integer & iYear, const LD::Integer & iMonth, const LD::Integer & iDay, const LD::Float &  hour, const LD::Float & minute, const LD::Float & second ) noexcept
        {
            const LD::Integer a = (14-iMonth)/12;
            const LD::Integer y = iYear + 4800 -a;
            const LD::Integer m = iMonth+12*a -3;

            LD::Integer result = iDay + LD::Floor(((153*m)+2)/5) +365*y + LD::Floor(y/4) - 32083;
            const LD::Float  date = (LD::Float)result + (((LD::Float)hour-12.0f)/24.0f)+((LD::Float)minute/1440.0f)+((LD::Float)second/86400.0f);
            return date;
        }

        //
        // Function      : JdToYmd
        //
        // Author        : Todd Knarr
        //
        // Creation date : 29 Nov 1995
        //
        // Parameters    : long julian day, pointers to int year, month, day
        //
        // Return values : none
        //
        // Description   :
        //
        // Internal routine that reverses the conversion, turning a Julian
        // day number into YMD values.
        //
        static void JdToYmd( const LD::UInteger & J, LD::Integer *piYear, LD::Integer *piMonth,
                             LD::Integer *piDay ) noexcept
        {
            const LD::Integer b =274277;
            const LD::Integer c = -38;
            const LD::Integer v = 3;
            const LD::Integer r = 4;
            const LD::Integer j = 1401;
            const LD::Integer f = (LD::Integer)(J + j + (((4 * J + b) / 146097) * 3) / 4 + c);
            const LD::Integer e =  r * f + v;
            const LD::Integer p = 1461;
            const LD::Integer w = 2;
            const LD::Integer g = (e%p)/r;
            const LD::Integer u = 5;
            const LD::Integer h = u * g + w;
            const LD::Integer m = 2;
            const LD::Integer n = 12;
            const LD::Integer y = 4716;
            const LD::Integer s = 153;
            *piDay = (h%s)/u +1;
            *piMonth = (h/2+m)%n +1;
            *piYear = (e/p) - y+(n+m-*piMonth)/n;
        }

    public:

        //
        // Function      : IsLeapYear
        //
        // Author        : Todd Knarr
        //
        // Creation date : 5 Dec 1995
        //
        // Parameters    : int year
        //
        // Return values : 1 if the year given is a leap year
        //                 0 if it is not
        //
        static const bool IsLeapYear( const LD::Integer & iYear ) noexcept
        {
            LD::Integer jd1, jd2;
            jd1 = YmdToJd( iYear, 2, 28 ,0,0,0);
            jd2 = YmdToJd( iYear, 3, 1 ,0,0,0);
            return ( ( jd2 - jd1 ) > 1 );
        }

        //
        // Function      : default constructor
        //
        // Author        : Todd Knarr
        //
        // Creation date : 29 Nov 1995
        //
        // Parameters    : none
        //
        // Return values : none
        //
        // Description   :
        //
        // Constructs a new object initialized to 1 Jan 4713BC
        //
        Date() noexcept:Date(time(0))
        {

        }

        //
        // Function      : copy constructor
        //
        // Author        : Todd Knarr
        //
        // Creation date : 13 Mar 1996
        //
        // Parameters    :
        //
        // Return values :
        //
        Date( const Date& Orig ) noexcept { lJulianDay = Orig.lJulianDay; }
        Date& operator=( const Date& Orig ) noexcept
        {
            lJulianDay = Orig.lJulianDay;
            return *this;
        }

        //
        // Function      : time_t constructor
        //
        // Author        : Todd Knarr
        //
        // Creation date : 29 Nov 1995
        //
        // Parameters    : none
        //
        // Return values : none
        //
        // Description   :
        //
        // Constructs an object initialized to the date
        // represented by a system time value.
        //
        Date( const time_t tSysTime ) noexcept
        {
            struct tm *ptm;
            int y, m, d;
            short hour,minute,second;


            ptm = localtime( &tSysTime );

            y = ptm->tm_year + 1900;
            m = ptm->tm_mon + 1;
            d = ptm->tm_mday;
            hour = ptm->tm_hour;
            minute = ptm->tm_min;
            second = ptm->tm_sec;
            //this->CurrentTime.Hour = hour;
            //this->CurrentTime.Minute = minute;
            //this->CurrentTime.Second = second;
            lJulianDay = YmdToJd( y, m, d ,0,0,0);
        }

        //
        // Function      : char* ( string ) constructor
        //
        // Author        : Todd Knarr
        //
        // Creation date : 30 Nov 1995
        //
        // Parameters    :
        //
        // Return values :
        //
        // Description   :
        //
        // Constructs an object from a string.
        // The string is formatted as the ASCII representation
        // of the long Julian day number.
        //
        Date( const char *String ) noexcept
        {
            lJulianDay = atol( String );
        }

        //
        // Function      : year/month/day constructor
        //
        // Author        : Todd Knarr
        //
        // Creation date : 29 Nov 1995
        //
        // Parameters    : int year, month, day
        //
        // Return values : none
        //
        // Description   :
        //
        // Constructs an object initialized to
        // the date given by the arguments
        //
        Date( const LD::Integer & iDay, const LD::Integer &  iMonth, const LD::Integer &  iYear ) noexcept
        {
            lJulianDay = YmdToJd( iYear, iMonth, iDay ,0,0,0);
        }

        //
        // Function      : Year, Month, Day, DayOfYear, DayOfWeek, DayOfWorkWeek,
        //                 IsLeapYear, YMD
        //
        // Author        : Todd Knarr
        //
        // Creation date : 29 Nov 1995
        //
        // Parameters    : none
        //
        // Return values : int or none
        //
        // Description   :
        //
        // Functions to return various parts of the date. The Year(),
        // Month() and Day() functions return the corresponding parts
        // of the date. The DayOfYear() returns the day of the year,
        // with 1 Jan being day 1. The DayOfWeek() and DayOfWorkWeek()
        // functions return the day of the week, from 0 through 6, with
        // the DayOfWeek() matching localtime()'s convention ( 0 = Sunday,
        // 6 = Saturday ) and DayOfWorkWeek() starting the week on
        // Monday ( 0 = Monday, 6 = Sunday ). IsLeapYear() returns 1 if
        // the date is in a leap year, 0 otherwise. YMD() puts the
        // year, month and day values directly into three integer variables,
        // for times when you need all three at the same time.
        //
        LD::Integer GetYear( void ) const noexcept
        {
            LD::Integer y, m, d;

            JdToYmd( lJulianDay, &y, &m, &d );
            return y;
        }
        LD::Integer GetMonth( void ) const noexcept
        {
            LD::Integer y, m, d;

            JdToYmd( lJulianDay, &y, &m, &d );
            return m;
        }
        LD::Integer GetDay( void ) const noexcept
        {
            LD::Integer y, m, d;

            JdToYmd( lJulianDay, &y, &m, &d );
            return d;
        }
        void SetJulianDay(const unsigned long data) noexcept
        {
            this->lJulianDay = data;
        }
        //void SetTime(const Time & time) noexcept
        //{
            //this->CurrentTime = time;
        //}
        const unsigned long GetTimeAsNumber() const noexcept
        {
            unsigned long time = 0;
            //((short*)&time)[0] = this->CurrentTime.Hour;
            //((short*)&time)[1] = this->CurrentTime.Minute;
            //((short*)&time)[2] = this->CurrentTime.Second;
            //((short*)&time)[3] = 1;
            return time;
        }
        void SetTime(const unsigned long possibleTime) noexcept
        {
            //this->CurrentTime.Hour = ((short*)&possibleTime)[0];
            //this->CurrentTime.Minute = ((short*)&possibleTime)[1];
            //this->CurrentTime.Second = ((short*)&possibleTime)[2];
        }
        LD::Integer GetDayOfYear( void ) const noexcept;
        LD::Integer GetDayOfWeek( void ) const noexcept
        {
            return (LD::Integer)   fmodf(    fmodf(lJulianDay, 7.0f)+1.0f,7.0f)      ;
        }
        LD::Integer GetDayOfWorkWeek( void ) const
        {
            return (LD::Integer)fmodf(lJulianDay, 7.0f);
        }
        bool IsLeapYear( void ) const noexcept
        {
            LD::Integer y, m, d;
            JdToYmd( lJulianDay, &y, &m, &d );
            return (bool)IsLeapYear( y );
        }
        void YMD( LD::Integer *pY, LD::Integer *pM, LD::Integer *pD ) noexcept
        {
            JdToYmd( lJulianDay, pY, pM, pD );
            return;
        }
        const LD::UInteger & GetJulianDate() const noexcept
        {
            return this->lJulianDay;
        }
        //
        // Function      : Additive operators
        //
        // Author        : Todd Knarr
        //
        // Creation date : 30 Nov 1995
        //
        // Parameters    : Date value and integer, or
        //                 Two date values
        //
        // Return values : New Date, or difference between two Dates
        //
        // Description   :
        //
        // Adding an integral type to a Date adds the number of
        // days in the integral type to the Date. Adding two Dates
        // is not defined. Subtracting two dates yields the difference
        // between them in days.
        //
        friend Date operator+( const Date& Left, const long Right ) noexcept
        {
            Date Temp = Left;
            Temp.lJulianDay += Right;
            return Temp;
        }
        friend Date operator+( const long Left, const Date& Right ) noexcept
        {
            Date Temp = Right;
            Temp.lJulianDay += Left;
            return Temp;
        }
        Date& operator+=( const long Right ) noexcept
        {
            lJulianDay += Right;
            return *this;
        }
        friend Date operator-( const Date& Left, const long Right ) noexcept
        {
            Date Temp = Left;
            Temp.lJulianDay -= Right;
            return Temp;
        }
        friend Date operator-( const long Left, const Date& Right ) noexcept
        {
            Date Temp = Right;
            Temp.lJulianDay -= Left;
            return Temp;
        }
        Date& operator-=( const long Right ) noexcept
        {
            lJulianDay -= Right;
            return *this;
        }
        long operator-( const Date& Right ) noexcept
        {
            return lJulianDay - Right.lJulianDay;
        }

        //
        // Function      : ++ and -- operators, prefix and postfix forms
        //
        // Author        : Todd Knarr
        //
        // Creation date : 1 Dec 1995
        //
        // Parameters    : none
        //
        // Return values : New Date
        //
        Date& operator++() noexcept
        {
            lJulianDay++;
            return *this;
        }
        Date operator++( int ) noexcept
        {
            Date Temp = *this;
            lJulianDay++;
            return Temp;
        }
        Date& operator--() noexcept
        {
            lJulianDay--;
            return *this;
        }
        Date operator--( int ) noexcept
        {
            Date Temp = *this;
            lJulianDay--;
            return Temp;
        }

        int operator==( const Date& Right ) const noexcept
        {
            return lJulianDay == Right.lJulianDay;
        }
        int operator!=( const Date& Right ) const noexcept
        {
            return lJulianDay != Right.lJulianDay;
        }
        int operator<( const Date& Right ) const noexcept
        {
            return lJulianDay < Right.lJulianDay;
        }
        int operator<=( const Date& Right ) const noexcept
        {
            return lJulianDay <= Right.lJulianDay;
        }
        int operator>( const Date& Right ) const noexcept
        {
            return lJulianDay > Right.lJulianDay;
        }
        int operator>=( const Date& Right ) const noexcept
        {
            return lJulianDay >= Right.lJulianDay;
        }

    };
     */

    class DateTime
    {
    private:
        LD::Date mDate;
        LD::Time mTime;
    public:
    };

    class GregorianDate
    {
    private:
        unsigned long        lJulianDay;


        static const LD::Float YmdToJd( const LD::Integer & iYear, const LD::Integer & iMonth, const LD::Integer & iDay, const LD::Float &  hour, const LD::Float & minute, const LD::Float & second ) noexcept
        {
            const LD::Integer a = (14-iMonth)/12;
            const LD::Integer y = iYear + 4800 -a;
            const LD::Integer m = iMonth+12*a -3;

            LD::Integer result = iDay + LD::Floor(((153*m)+2)/5) +365*y + LD::Floor(y/4) - 32083;
            const LD::Float  date = (LD::Float)result + (((LD::Float)hour-12.0f)/24.0f)+((LD::Float)minute/1440.0f)+((LD::Float)second/86400.0f);
            return date;
        }


        static void JdToYmd( const LD::UInteger & J, LD::Integer *piYear, LD::Integer *piMonth,
                             LD::Integer *piDay ) noexcept
        {
            const LD::Integer b =274277;
            const LD::Integer c = -38;
            const LD::Integer v = 3;
            const LD::Integer r = 4;
            const LD::Integer j = 1401;
            const LD::Integer f = (LD::Integer)(J + j + (((4 * J + b) / 146097) * 3) / 4 + c);
            const LD::Integer e =  r * f + v;
            const LD::Integer p = 1461;
            const LD::Integer w = 2;
            const LD::Integer g = (e%p)/r;
            const LD::Integer u = 5;
            const LD::Integer h = u * g + w;
            const LD::Integer m = 2;
            const LD::Integer n = 12;
            const LD::Integer y = 4716;
            const LD::Integer s = 153;
            *piDay = (h%s)/u +1;
            *piMonth = (h/2+m)%n +1;
            *piYear = (e/p) - y+(n+m-*piMonth)/n;
        }


        static const bool IsLeapYear( const LD::Integer & iYear ) noexcept
        {
            LD::Integer jd1, jd2;
            jd1 = YmdToJd( iYear, 2, 28 ,0,0,0);
            jd2 = YmdToJd( iYear, 3, 1 ,0,0,0);
            return ( ( jd2 - jd1 ) > 1 );
        }
    public:

        inline GregorianDate();
        inline GregorianDate( const LD::Integer & iDay, const LD::Integer &  iMonth, const LD::Integer &  iYear );
        inline GregorianDate( const time_t tSysTime );
    };
}



#endif
#endif //LANDESSDEVCORE_DATETIME_HPP
