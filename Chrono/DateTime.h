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
#include "Core/NullClass.hpp"
#include "Core/Unit.hpp"
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
    namespace TZ
    {

        //Universal Coordinated Time
        template<typename DateTimeType>
        class UTC
        {
        private:
            DateTimeType mDateTime;
        public:
            operator UTC<DateTimeType> () const noexcept
            {
                return this->mDateTime;
            }
        };

        //Greenwich Mean Time
        class GMT{};
        //European Central Time
        class ECT{};
        //Eastern European Time
        class EET{};
        //(Arabic) Egypt Standard Time
        class ART{};
        //Eastern African Time
        class EAT{};
        //Middle East Time
        class MET{};
        //Near East Time
        class NET{};
        //Pakistan Lahore Time
        class PLT{};
        //India Standard Time
        class IST{};
        //Bangladesh Standard Time
        class BST{};
        //Vietnam Standard Time
        class VST{};
        //China Taiwan Time
        class CTT{};
        //Japan Standard Time
        class JST{};
        //Australia Central Time
        class ACT{};
        //Australia Eastern Time
        class AET{};
        //Solomon Standard Time
        class SST{};
        //New Zealand Standard Time
        class NST{};
        //Midway Islands Time
        class MIT{};
        //Hawaii Standard Time
        class HST{};
        //Alaska Standard Time
        class AST{};
        //Pacific Standard Time
        class PST{};
        //Phoenix Standard Time
        class PNT{};
        //Mountain Standard Time
        class MST{};
        //Central Standard Time
        class CST{};
        //Eastern Standard Time
        class EST{};
        //Indiana Eastern Standard Time
        class IET{};
        //Puerto Rico and US Virgin Islands Time
        class PRT{};
        //Canada Newfoundland Time
        class CNT{};
        //Argentina Standard Time
        class AGT{};
        //Brazil Eastern Time
        class BET{};
        //Central African Time
        class CAT{};
    }
    namespace CT
    {
        template<typename T>
        inline constexpr bool IsHour(LD::Type<T>) noexcept
        {
            return false;
        }
        template<typename T>
        inline constexpr bool IsHour(LD::Type<LD::Hour<T>> ) noexcept
        {
            return true;
        }
        template<typename T>
        inline constexpr bool IsMinute(LD::Type<T>) noexcept
        {
            return false;
        }
        template<typename T>
        inline constexpr bool IsMinute(LD::Type<LD::Minute<T>> ) noexcept
        {
            return true;
        }
        template<typename T>
        inline constexpr bool IsSecond(LD::Type<T>) noexcept
        {
            return false;
        }
        template<typename T>
        inline constexpr bool IsSecond(LD::Type<LD::Second<T>> ) noexcept
        {
            return true;
        }
        template<typename T>
        inline constexpr bool IsMiliSecond(LD::Type<T>) noexcept
        {
            return false;
        }
        template<typename T>
        inline constexpr bool IsMiliSecond(LD::Type<LD::Milisecond<T>> ) noexcept
        {
            return true;
        }
        template<typename T>
        inline constexpr bool IsDay(LD::Type<T>) noexcept
        {
            return false;
        }
        template<typename T>
        inline constexpr bool IsDay(LD::Type<LD::Day<T>> ) noexcept
        {
            return true;
        }
        template<typename T>
        inline constexpr bool IsMonth(LD::Type<T>) noexcept
        {
            return false;
        }
        template<typename T>
        inline constexpr bool IsMonth(LD::Type<LD::Month<T>> ) noexcept
        {
            return true;
        }
        template<typename T>
        inline constexpr bool IsYear(LD::Type<T>) noexcept
        {
            return false;
        }
        template<typename T>
        inline constexpr bool IsYear(LD::Type<LD::Year<T>> ) noexcept
        {
            return true;
        }
        namespace Detail
        {
            template<typename First, typename Second, typename Third, class = void>
            struct ExtractDayTypeImpl;

            template<typename First, typename Second, typename Third, class = void>
            struct ExtractMonthTypeImpl;

            template<typename First, typename Second, typename Third, class = void>
            struct ExtractYearTypeImpl;

            template<typename First, typename Second, typename Third, typename Fourth ,class = void>
            struct ExtractHourTypeImpl;

            template<typename First, typename Second, typename Third, typename Fourth, class = void>
            struct ExtractMinuteTypeImpl;

            template<typename First, typename Second, typename Third, typename Fourth, class = void>
            struct ExtractSecondTypeImpl;

            template<typename First, typename Second, typename Third, typename Fourth, class = void>
            struct ExtractMilisecondTypeImpl;

            template<typename First, typename Second, typename Third>
            struct ExtractDayTypeImpl<First,Second,Third,LD::Enable_If_T<
                    LD::Require<
                    LD::CT::IsDay(LD::Type<First>{}),
                    !LD::CT::IsDay(LD::Type<Second>{}),
                    !LD::CT::IsDay(LD::Type<Third>{})
                    >>>
            {
                using type = First;
            };
            template<typename First, typename Second, typename Third>
            struct ExtractDayTypeImpl<First,Second,Third,LD::Enable_If_T<
                    LD::Require<
                            !LD::CT::IsDay(LD::Type<First>{}),
                            LD::CT::IsDay(LD::Type<Second>{}),
                            !LD::CT::IsDay(LD::Type<Third>{})
                    >>>
            {
                using type = Second;
            };
            template<typename First, typename Second, typename Third>
            struct ExtractDayTypeImpl<First,Second,Third,LD::Enable_If_T<
                    LD::Require<
                            !LD::CT::IsDay(LD::Type<First>{}),
                            !LD::CT::IsDay(LD::Type<Second>{}),
                            LD::CT::IsDay(LD::Type<Third>{})
                    >>>
            {
                using type = Third;
            };
            template<typename First, typename Second, typename Third>
            struct ExtractMonthTypeImpl<First,Second,Third,LD::Enable_If_T<
                    LD::Require<
                            LD::CT::IsMonth(LD::Type<First>{}),
                            !LD::CT::IsMonth(LD::Type<Second>{}),
                            !LD::CT::IsMonth(LD::Type<Third>{})
                    >>>
            {
                using type = First;
            };
            template<typename First, typename Second, typename Third>
            struct ExtractMonthTypeImpl<First,Second,Third,LD::Enable_If_T<
                    LD::Require<
                            !LD::CT::IsMonth(LD::Type<First>{}),
                            LD::CT::IsMonth(LD::Type<Second>{}),
                            !LD::CT::IsMonth(LD::Type<Third>{})
                    >>>
            {
                using type = Second;
            };
            template<typename First, typename Second, typename Third>
            struct ExtractMonthTypeImpl<First,Second,Third,LD::Enable_If_T<
                    LD::Require<
                            !LD::CT::IsMonth(LD::Type<First>{}),
                            !LD::CT::IsMonth(LD::Type<Second>{}),
                            LD::CT::IsMonth(LD::Type<Third>{})
                    >>>
            {
                using type = Third;
            };
            template<typename First, typename Second, typename Third>
            struct ExtractYearTypeImpl<First,Second,Third,LD::Enable_If_T<
                    LD::Require<
                            LD::CT::IsYear(LD::Type<First>{}),
                            !LD::CT::IsYear(LD::Type<Second>{}),
                            !LD::CT::IsYear(LD::Type<Third>{})
                    >>>
            {
                using type = First;
            };
            template<typename First, typename Second, typename Third>
            struct ExtractYearTypeImpl<First,Second,Third,LD::Enable_If_T<
                    LD::Require<
                            !LD::CT::IsYear(LD::Type<First>{}),
                            LD::CT::IsYear(LD::Type<Second>{}),
                            !LD::CT::IsYear(LD::Type<Third>{})
                    >>>
            {
                using type = Second;
            };
            template<typename First, typename Second, typename Third>
            struct ExtractYearTypeImpl<First,Second,Third,LD::Enable_If_T<
                    LD::Require<
                            !LD::CT::IsYear(LD::Type<First>{}),
                            !LD::CT::IsYear(LD::Type<Second>{}),
                            LD::CT::IsYear(LD::Type<Third>{})
                    >>>
            {
                using type = Third;
            };
            template<typename First, typename Second, typename Third, typename Fourth>
            struct ExtractHourTypeImpl<First,Second,Third,Fourth,LD::Enable_If_T<
                    LD::Require<
                            LD::CT::IsHour(LD::Type<First>{}),
                            !LD::CT::IsHour(LD::Type<Second>{}),
                            !LD::CT::IsHour(LD::Type<Third>{}),
                            !LD::CT::IsHour(LD::Type<Fourth>{})
                    >>>
            {
                using type = First;
            };
            template<typename First, typename Second, typename Third, typename Fourth>
            struct ExtractHourTypeImpl<First,Second,Third,Fourth,LD::Enable_If_T<
                    LD::Require<
                            !LD::CT::IsHour(LD::Type<First>{}),
                            LD::CT::IsHour(LD::Type<Second>{}),
                            !LD::CT::IsHour(LD::Type<Third>{}),
                            !LD::CT::IsHour(LD::Type<Fourth>{})
                    >>>
            {
                using type = Second;
            };
            template<typename First, typename Second, typename Third, typename Fourth>
            struct ExtractHourTypeImpl<First,Second,Third,Fourth,LD::Enable_If_T<
                    LD::Require<
                            !LD::CT::IsHour(LD::Type<First>{}),
                            !LD::CT::IsHour(LD::Type<Second>{}),
                            LD::CT::IsHour(LD::Type<Third>{}),
                            !LD::CT::IsHour(LD::Type<Fourth>{})
                    >>>
            {
                using type = Third;
            };
            template<typename First, typename Second, typename Third, typename Fourth>
            struct ExtractHourTypeImpl<First,Second,Third,Fourth,LD::Enable_If_T<
                    LD::Require<
                            !LD::CT::IsHour(LD::Type<First>{}),
                            !LD::CT::IsHour(LD::Type<Second>{}),
                            !LD::CT::IsHour(LD::Type<Third>{}),
                            LD::CT::IsHour(LD::Type<Fourth>{})
                    >>>
            {
                using type = Fourth;
            };
            template<typename First, typename Second, typename Third, typename Fourth>
            struct ExtractMinuteTypeImpl<First,Second,Third,Fourth,LD::Enable_If_T<
                    LD::Require<
                            LD::CT::IsMinute(LD::Type<First>{}),
                            !LD::CT::IsMinute(LD::Type<Second>{}),
                            !LD::CT::IsMinute(LD::Type<Third>{}),
                            !LD::CT::IsMinute(LD::Type<Fourth>{})
                    >>>
            {
                using type = First;
            };
            template<typename First, typename Second, typename Third, typename Fourth>
            struct ExtractMinuteTypeImpl<First,Second,Third,Fourth,LD::Enable_If_T<
                    LD::Require<
                            !LD::CT::IsMinute(LD::Type<First>{}),
                            LD::CT::IsMinute(LD::Type<Second>{}),
                            !LD::CT::IsMinute(LD::Type<Third>{}),
                            !LD::CT::IsMinute(LD::Type<Fourth>{})
                    >>>
            {
                using type = Second;
            };
            template<typename First, typename Second, typename Third, typename Fourth>
            struct ExtractMinuteTypeImpl<First,Second,Third,Fourth,LD::Enable_If_T<
                    LD::Require<
                            !LD::CT::IsMinute(LD::Type<First>{}),
                            !LD::CT::IsMinute(LD::Type<Second>{}),
                            LD::CT::IsMinute(LD::Type<Third>{}),
                            !LD::CT::IsMinute(LD::Type<Fourth>{})
                    >>>
            {
                using type = Third;
            };
            template<typename First, typename Second, typename Third, typename Fourth>
            struct ExtractMinuteTypeImpl<First,Second,Third,Fourth,LD::Enable_If_T<
                    LD::Require<
                            !LD::CT::IsMinute(LD::Type<First>{}),
                            !LD::CT::IsMinute(LD::Type<Second>{}),
                            !LD::CT::IsMinute(LD::Type<Third>{}),
                            LD::CT::IsMinute(LD::Type<Fourth>{})
                    >>>
            {
                using type = Fourth;
            };
            template<typename First, typename Second, typename Third, typename Fourth>
            struct ExtractSecondTypeImpl<First,Second,Third,Fourth,LD::Enable_If_T<
                    LD::Require<
                            LD::CT::IsSecond(LD::Type<First>{}),
                            !LD::CT::IsSecond(LD::Type<Second>{}),
                            !LD::CT::IsSecond(LD::Type<Third>{}),
                            !LD::CT::IsSecond(LD::Type<Fourth>{})
                    >>>
            {
                using type = First;
            };
            template<typename First, typename Second, typename Third, typename Fourth>
            struct ExtractSecondTypeImpl<First,Second,Third,Fourth,LD::Enable_If_T<
                    LD::Require<
                            !LD::CT::IsSecond(LD::Type<First>{}),
                            LD::CT::IsSecond(LD::Type<Second>{}),
                            !LD::CT::IsSecond(LD::Type<Third>{}),
                            !LD::CT::IsSecond(LD::Type<Fourth>{})
                    >>>
            {
                using type = Second;
            };
            template<typename First, typename Second, typename Third, typename Fourth>
            struct ExtractSecondTypeImpl<First,Second,Third,Fourth,LD::Enable_If_T<
                    LD::Require<
                            !LD::CT::IsSecond(LD::Type<First>{}),
                            !LD::CT::IsSecond(LD::Type<Second>{}),
                            LD::CT::IsSecond(LD::Type<Third>{}),
                            !LD::CT::IsSecond(LD::Type<Fourth>{})
                    >>>
            {
                using type = Third;
            };
            template<typename First, typename Second, typename Third, typename Fourth>
            struct ExtractSecondTypeImpl<First,Second,Third,Fourth,LD::Enable_If_T<
                    LD::Require<
                            !LD::CT::IsSecond(LD::Type<First>{}),
                            !LD::CT::IsSecond(LD::Type<Second>{}),
                            !LD::CT::IsSecond(LD::Type<Third>{}),
                            LD::CT::IsSecond(LD::Type<Fourth>{})
                    >>>
            {
                using type = Fourth;
            };
            template<typename First, typename Second, typename Third, typename Fourth>
            struct ExtractMilisecondTypeImpl<First,Second,Third,Fourth,LD::Enable_If_T<
                    LD::Require<
                            LD::CT::IsMiliSecond(LD::Type<First>{}),
                            !LD::CT::IsMiliSecond(LD::Type<Second>{}),
                            !LD::CT::IsMiliSecond(LD::Type<Third>{}),
                            !LD::CT::IsMiliSecond(LD::Type<Fourth>{})
                    >>>
            {
                using type = First;
            };
            template<typename First, typename Second, typename Third, typename Fourth>
            struct ExtractMilisecondTypeImpl<First,Second,Third,Fourth,LD::Enable_If_T<
                    LD::Require<
                            !LD::CT::IsMiliSecond(LD::Type<First>{}),
                            LD::CT::IsMiliSecond(LD::Type<Second>{}),
                            !LD::CT::IsMiliSecond(LD::Type<Third>{}),
                            !LD::CT::IsMiliSecond(LD::Type<Fourth>{})
                    >>>
            {
                using type = Second;
            };
            template<typename First, typename Second, typename Third, typename Fourth>
            struct ExtractMilisecondTypeImpl<First,Second,Third,Fourth,LD::Enable_If_T<
                    LD::Require<
                            !LD::CT::IsMiliSecond(LD::Type<First>{}),
                            !LD::CT::IsMiliSecond(LD::Type<Second>{}),
                            LD::CT::IsMiliSecond(LD::Type<Third>{}),
                            !LD::CT::IsMiliSecond(LD::Type<Fourth>{})
                    >>>
            {
                using type = Third;
            };
            template<typename First, typename Second, typename Third, typename Fourth>
            struct ExtractMilisecondTypeImpl<First,Second,Third,Fourth,LD::Enable_If_T<
                    LD::Require<
                            !LD::CT::IsMiliSecond(LD::Type<First>{}),
                            !LD::CT::IsMiliSecond(LD::Type<Second>{}),
                            !LD::CT::IsMiliSecond(LD::Type<Third>{}),
                            LD::CT::IsMiliSecond(LD::Type<Fourth>{})
                    >>>
            {
                using type = Fourth;
            };
            template <typename First, typename Second, typename Third> using ExtractDayType = typename LD::CT::Detail::ExtractDayTypeImpl<First,Second,Third>::type ;

            template <typename First, typename Second, typename Third> using ExtractMonthType = typename LD::CT::Detail::ExtractMonthTypeImpl<First,Second,Third>::type ;

            template <typename First, typename Second, typename Third> using ExtractYearType = typename LD::CT::Detail::ExtractYearTypeImpl<First,Second,Third>::type ;

            template <typename First, typename Second, typename Third, typename Fourth> using ExtractHourType = typename LD::CT::Detail::ExtractHourTypeImpl<First,Second,Third,Fourth>::type ;

            template <typename First, typename Second, typename Third, typename Fourth> using ExtractMinuteType = typename LD::CT::Detail::ExtractMinuteTypeImpl<First,Second,Third,Fourth>::type ;

            template <typename First, typename Second, typename Third, typename Fourth> using ExtractSecondType = typename LD::CT::Detail::ExtractSecondTypeImpl<First,Second,Third,Fourth>::type ;

            template <typename First, typename Second, typename Third, typename Fourth> using ExtractMilisecondType = typename LD::CT::Detail::ExtractMilisecondTypeImpl<First,Second,Third,Fourth>::type ;
        }
    }
    template<typename First, typename Second, typename Third, class = void>
    struct BasicDateRegexGenerator
    {
        static constexpr auto  DateDetectionRegex = ctll::basic_fixed_string{"(\\d{4})-(\\d{1,2})-(\\d{1,2})"};
    };
    template<typename First, typename Second, typename Third>
    struct BasicDateRegexGenerator<First,Second,Third,LD::Enable_If_T<
            LD::Require<
                    LD::CT::IsMonth(LD::Type<First>{}),
                    LD::CT::IsDay(LD::Type<Second>{}),
                    LD::CT::IsYear(LD::Type<Third>{})
            >>>
    {
        static constexpr auto  DateDetectionRegex = ctll::basic_fixed_string{"(\\d{4})-(\\d{1,2})-(\\d{1,2})"};
    };
    template<typename First, typename Second, typename Third>
    struct BasicDateRegexGenerator<First,Second,Third,LD::Enable_If_T<
            LD::Require<
                    LD::CT::IsDay(LD::Type<Second>{}),
                    LD::CT::IsMonth(LD::Type<First>{}),
                    LD::CT::IsYear(LD::Type<Third>{})
            >>>
    {
        static constexpr auto  DateDetectionRegex = ctll::basic_fixed_string{"(\\d{4})-(\\d{1,2})-(\\d{1,2})"};
    };
    template<typename First, typename Second, typename Third, class = void>
    class BasicDate;

    template<typename First, typename Second, typename Third>
    class BasicDate<First,Second,Third,LD::Enable_If_T<
            LD::Require<
                    true
            >>>
    {
    private:
        First mFirst;
        Second mSecond;
        Third mThird;
    public:

        //static constexpr auto  DateDetectionRegex = ctll::basic_fixed_string{"(\\d{4})-(\\d{1,2})-(\\d{1,2})"};
        BasicDate() noexcept:mFirst{0ul},mSecond{0ul},mThird{0ul}
        {
            time_t t = time(NULL);
            struct tm tm = *localtime(&t);
            if constexpr (LD::CT::IsDay(LD::Type<First>{}))
            {

                this->mFirst = LD::Day<LD::UInteger>{LD::UInteger (tm.tm_mday)};
            }else if constexpr (LD::CT::IsMonth(LD::Type<First>{}))
            {
                this->mFirst = LD::Month<LD::UInteger>{LD::UInteger (tm.tm_mon +1)};
            }else if constexpr (LD::CT::IsYear(LD::Type<First>{}))
            {
                this->mFirst = LD::Year<LD::UInteger>{LD::UInteger (tm.tm_year + 1900)};
            }

            if constexpr (LD::CT::IsDay(LD::Type<Second>{}))
            {
                this->mSecond = LD::Day<LD::UInteger>{LD::UInteger (tm.tm_mday)};
            }else if constexpr (LD::CT::IsMonth(LD::Type<Second>{}))
            {
                this->mSecond = LD::Month<LD::UInteger>{LD::UInteger (tm.tm_mon +1)};
            }else if constexpr (LD::CT::IsYear(LD::Type<Second>{}))
            {
                this->mSecond = LD::Year<LD::UInteger>{LD::UInteger (tm.tm_year + 1900)};
            }

            if constexpr (LD::CT::IsDay(LD::Type<Third>{}))
            {
                this->mThird = LD::Day<LD::UInteger>{LD::UInteger (tm.tm_mday)};
            }else if constexpr (LD::CT::IsMonth(LD::Type<Third>{}))
            {
                this->mThird = LD::Month<LD::UInteger>{LD::UInteger (tm.tm_mon +1)};
            }else if constexpr (LD::CT::IsYear(LD::Type<Third>{}))
            {
                this->mThird = LD::Year<LD::UInteger>{LD::UInteger (tm.tm_year + 1900)};
            }
        }
        BasicDate(First year, Second month, Third day) noexcept:mFirst{year},mSecond{month},mThird{day}{}

        template<typename V, typename W, typename X>
        BasicDate(const BasicDate<V,W,X> & date) noexcept
        {
            (*this) = date;
        }
        template<typename V, typename W, typename X>
        BasicDate & operator = (const BasicDate<V,W,X> & date) noexcept
        {
            this->Day() = date.Day();
            this->Month() = date.Month();
            this->Year() = date.Year();
            return (*this);
        }

        const LD::CT::Detail::ExtractDayType<First,Second,Third> & Day() const noexcept
        {
            if constexpr (LD::CT::IsDay(LD::Type<First>{}))
            {
                return this->mFirst;
            } else if constexpr (LD::CT::IsDay(LD::Type<Second>{}))
            {
                return this->mSecond;
            }else
            {
                return this->mThird;
            }

        }
        const LD::CT::Detail::ExtractMonthType<First,Second,Third> & Month() const noexcept
        {
            if constexpr (LD::CT::IsMonth(LD::Type<First>{}))
            {
                return this->mFirst;
            } else if constexpr (LD::CT::IsMonth(LD::Type<Second>{}))
            {
                return this->mSecond;
            }else
            {
                return this->mThird;
            }

        }
        const LD::CT::Detail::ExtractYearType<First,Second,Third> & Year() const noexcept
        {
            if constexpr (LD::CT::IsYear(LD::Type<First>{}))
            {
                return this->mFirst;
            } else if constexpr (LD::CT::IsYear(LD::Type<Second>{}))
            {
                return this->mSecond;
            }else
            {
                return this->mThird;
            }

        }

        LD::CT::Detail::ExtractDayType<First,Second,Third> & Day() noexcept
        {
            if constexpr (LD::CT::IsDay(LD::Type<First>{}))
            {
                return this->mFirst;
            } else if constexpr (LD::CT::IsDay(LD::Type<Second>{}))
            {
                return this->mSecond;
            }else
            {
                return this->mThird;
            }

        }
        LD::CT::Detail::ExtractMonthType<First,Second,Third> & Month() noexcept
        {
            if constexpr (LD::CT::IsMonth(LD::Type<First>{}))
            {
                return this->mFirst;
            } else if constexpr (LD::CT::IsMonth(LD::Type<Second>{}))
            {
                return this->mSecond;
            }else
            {
                return this->mThird;
            }
        }
        LD::CT::Detail::ExtractYearType<First,Second,Third> & Year() noexcept
        {
            if constexpr (LD::CT::IsYear(LD::Type<First>{}))
            {
                return this->mFirst;
            } else if constexpr (LD::CT::IsYear(LD::Type<Second>{}))
            {
                return this->mSecond;
            }else
            {
                return this->mThird;
            }

        }
        BasicDate & operator++() noexcept
        {
            //(*this) = this->NextDate();
            time_t t = time(NULL);
            struct tm tm = *localtime(&t);
            tm.tm_year = this->Year().NativeRepresentation().Value() - 1900;
            tm.tm_mon = this->Month().NativeRepresentation().Value() - 1;
            tm.tm_mday = this->Day().NativeRepresentation().Value();
            tm.tm_sec+=1;
            mktime(&tm);
            this->Year() = LD::Year<LD::UInteger>{LD::UInteger (tm.tm_year + 1900)};
            this->Month() = LD::Month<LD::UInteger>{LD::UInteger (tm.tm_mon +1)};
            this->Day() = LD::Day<LD::UInteger>{LD::UInteger (tm.tm_mday)};
            return (*this);
        }
        BasicDate & operator++(int) noexcept
        {
            time_t t = time(NULL);
            struct tm tm = *localtime(&t);
            tm.tm_year = this->Year().NativeRepresentation().Value() - 1900;
            tm.tm_mon = this->Month().NativeRepresentation().Value() - 1;
            tm.tm_mday = this->Day().NativeRepresentation().Value();
            tm.tm_sec+=1;
            mktime(&tm);
            this->Year() = LD::Year<LD::UInteger>{LD::UInteger (tm.tm_year + 1900)};
            this->Month() = LD::Month<LD::UInteger>{LD::UInteger(tm.tm_mon +1)};
            this->Day() = LD::Day<LD::UInteger>{LD::UInteger (tm.tm_mday)};
            return (*this);
        }
        BasicDate & operator--() noexcept
        {
            time_t t = time(NULL);
            struct tm tm = *localtime(&t);
            tm.tm_year = this->Year().NativeRepresentation().Value() - 1900;
            tm.tm_mon = this->Month().NativeRepresentation().Value() - 1;
            tm.tm_mday = this->Day().NativeRepresentation().Value();
            tm.tm_sec-=1;
            mktime(&tm);
            this->Year() = LD::Year<LD::UInteger>{LD::UInteger (tm.tm_year + 1900)};
            this->Month() = LD::Month<LD::UInteger>{LD::UInteger (tm.tm_mon +1)};
            this->Day() = LD::Day<LD::UInteger>{LD::UInteger (tm.tm_mday)};
            return (*this);
        }
        BasicDate & operator--(int) noexcept
        {
            //(*this) = this->PreviousDate();
            time_t t = time(NULL);
            struct tm tm = *localtime(&t);
            tm.tm_year = this->Year().NativeRepresentation().Value() - 1900;
            tm.tm_mon = this->Month().NativeRepresentation().Value() - 1;
            tm.tm_mday = this->Day().NativeRepresentation().Value();
            tm.tm_sec-=1;
            mktime(&tm);
            this->Year() = LD::Year<LD::UInteger>{LD::UInteger (tm.tm_year + 1900)};
            this->Month() = LD::Month<LD::UInteger>{LD::UInteger (tm.tm_mon +1)};
            this->Day() = LD::Day<LD::UInteger>{LD::UInteger (tm.tm_mday)};
            return (*this);
        }

        BasicDate & operator+=(const LD::Second<LD::Float> & timeOffset)  noexcept
        {
            time_t t = time(NULL);
            struct tm tm = *localtime(&t);
            tm.tm_year = this->Year().NativeRepresentation().Value() - 1900;
            tm.tm_mon = this->Month().NativeRepresentation().Value() - 1;
            tm.tm_mday = this->Day().NativeRepresentation().Value();
            tm.tm_sec += timeOffset.NativeRepresentation().Value();
            mktime(&tm);
            this->Year() = LD::Year<LD::UInteger>{LD::UInteger (tm.tm_year + 1900)};
            this->Month() = LD::Month<LD::UInteger>{LD::UInteger (tm.tm_mon +1)};
            this->Day() = LD::Day<LD::UInteger>{LD::UInteger (tm.tm_mday)};
            return (*this);
        }
        BasicDate & operator-=(const LD::Second<LD::Float> & timeOffset)  noexcept
        {
            time_t t = time(NULL);
            struct tm tm = *localtime(&t);
            tm.tm_year = this->Year().NativeRepresentation().Value() - 1900;
            tm.tm_mon = this->Month().NativeRepresentation().Value() - 1;
            tm.tm_mday = this->Day().NativeRepresentation().Value();
            tm.tm_sec -= timeOffset.NativeRepresentation().Value();
            mktime(&tm);
            this->Year() = LD::Year<LD::UInteger>{LD::UInteger (tm.tm_year + 1900)};
            this->Month() = LD::Month<LD::UInteger>{LD::UInteger (tm.tm_mon +1)};
            this->Day() = LD::Day<LD::UInteger>{LD::UInteger (tm.tm_mday)};
            return (*this);
        }
        BasicDate operator+(const LD::Second<LD::Float> timeOffset) const noexcept
        {
            BasicDate instance{*this};
            time_t t = time(NULL);
            struct tm tm = *localtime(&t);
            tm.tm_year = instance.Year().NativeRepresentation().Value() - 1900;
            tm.tm_mon = instance.Month().NativeRepresentation().Value() - 1;
            tm.tm_mday = instance.Day().NativeRepresentation().Value();
            tm.tm_sec += timeOffset.NativeRepresentation().Value();
            mktime(&tm);
            instance.Year() = LD::Year<LD::UInteger>{LD::UInteger (tm.tm_year + 1900)};
            instance.Month() = LD::Month<LD::UInteger>{LD::UInteger (tm.tm_mon +1)};
            instance.Day() = LD::Day<LD::UInteger>{LD::UInteger (tm.tm_mday)};
            return instance;
        }
        BasicDate operator-(const LD::Second<LD::Float> timeOffset) const noexcept
        {
            BasicDate instance{*this};
            time_t t = time(NULL);
            struct tm tm = *localtime(&t);
            tm.tm_year = instance.Year().NativeRepresentation().Value() - 1900;
            tm.tm_mon = instance.Month().NativeRepresentation().Value() - 1;
            tm.tm_mday = instance.Day().NativeRepresentation().Value();
            tm.tm_sec -= timeOffset.NativeRepresentation().Value();
            mktime(&tm);
            instance.Year() = LD::Year<LD::UInteger>{LD::UInteger (tm.tm_year + 1900)};
            instance.Month() = LD::Month<LD::UInteger>{LD::UInteger (tm.tm_mon +1)};
            instance.Day() = LD::Day<LD::UInteger>{LD::UInteger (tm.tm_mday)};
            return instance;
        }
        template<typename First1, typename Second1, typename Third1>
        bool operator == (const BasicDate<First1,Second1,Third1> & date) const noexcept
        {
            return this->Day() == date.Day() && this->Month() == date.Month() && this->Year() == date.Year();
        }
        template<typename First1, typename Second1, typename Third1>
        bool operator != (const BasicDate<First1,Second1,Third1> & date) const noexcept
        {
            return !((*this)==date);
        }
        template<typename First1, typename Second1, typename Third1>
        bool operator < (const BasicDate<First1,Second1,Third1> & date) const noexcept
        {
            return (LD::Second<LD::UInteger>{this->Year()}+LD::Second<LD::UInteger>{this->Month()}+LD::Second<LD::UInteger>{this->Day()}) <
                   (LD::Second<LD::UInteger>{date.Year()} + LD::Second<LD::UInteger>{date.Month()} + LD::Second<LD::UInteger>{date.Day()});
        }
        template<typename First1, typename Second1, typename Third1>
        bool operator > (const BasicDate<First1,Second1,Third1> & date) const noexcept
        {
            return (LD::Second<LD::UInteger>{this->Year()}+LD::Second<LD::UInteger>{this->Month()}+LD::Second<LD::UInteger>{this->Day()}) >
                   (LD::Second<LD::UInteger>{date.Year()} + LD::Second<LD::UInteger>{date.Month()} + LD::Second<LD::UInteger>{date.Day()});
        }
        template<typename First1, typename Second1, typename Third1>
        bool operator <= (const BasicDate<First1,Second1,Third1> & date) const noexcept
        {
            return ((*this) == date) || ((*this) < date);
        }
        template<typename First1, typename Second1, typename Third1>
        bool operator >= (const BasicDate<First1,Second1,Third1> & date) const noexcept
        {
            return ((*this) == date) || ((*this) > date);
        }
    };
    using Date = BasicDate<LD::Year<LD::UInteger>,LD::Month<LD::UInteger>,LD::Day<LD::UInteger>>;

    template<typename First, typename Second, typename Third>
    inline LD::ImmutableString<75> ToImmutableString(const LD::BasicDate<First,Second,Third> & date) noexcept
    {

        if constexpr (
                LD::CT::IsMonth(LD::Type<First>{}) &&
                LD::CT::IsDay(LD::Type<Second>{})&&
                LD::CT::IsYear(LD::Type<Second>{}))
        {
            //printf("being called at all \n");
            //printf("rawrbunnies %s \n",date.Day().ToImmutableString().Data());
            return date.Month().ToImmutableString()+LD::ImmutableString{"-"}+date.Day().ToImmutableString()+LD::ImmutableString{"-"}+date.Year().ToImmutableString();
        }
        else if constexpr(LD::CT::IsDay(LD::Type<First>{}) &&
                          LD::CT::IsMonth(LD::Type<Second>{}) &&
                          LD::CT::IsYear(LD::Type<Third>{}))
        {

            return date.Day().ToImmutableString()+LD::ImmutableString{"-"}+date.Month().ToImmutableString()+LD::ImmutableString{"-"}+date.Year().ToImmutableString();
        }
        else if constexpr(LD::CT::IsMonth(LD::Type<First>{}) &&
                          LD::CT::IsDay(LD::Type<Second>{}) &&
                          LD::CT::IsYear(LD::Type<Third>{}))
        {

            return date.Month().ToImmutableString()+LD::ImmutableString{"-"}+date.Day().ToImmutableString()+LD::ImmutableString{"-"}+date.Year().ToImmutableString();
        }
        else if constexpr(LD::CT::IsYear(LD::Type<First>{}) &&
                          LD::CT::IsMonth(LD::Type<Second>{}) &&
                          LD::CT::IsDay(LD::Type<Third>{}))
        {
            return date.Year().ToImmutableString()+LD::ImmutableString{"-"}+((date.Month().NativeRepresentation().Value() < 10)?LD::ImmutableString{"0"}:LD::ImmutableString{""})+date.Month().ToImmutableString()+LD::ImmutableString{"-"}+((date.Day().NativeRepresentation().Value() < 10)?LD::ImmutableString{"0"}:LD::ImmutableString{""})+date.Day().ToImmutableString();
        }

        return date.Month().ToImmutableString()+LD::ImmutableString{"-"}+date.Day().ToImmutableString()+LD::ImmutableString{"-"}+date.Year().ToImmutableString();
    }

    template<typename First, typename Second, typename Third, typename Fourth = LD::NullClass ,class = void>
    class BasicTime;


    template<typename T1, typename T2, typename T3, typename T4>
    class BasicTime<T1,T2,T3,T4,
            LD::Enable_If_T<
            LD::Either<
                    LD::Require<
                            LD::CT::IsHour(LD::Type<T1>{}),
                            LD::CT::IsMinute(LD::Type<T2>{}),
                            LD::CT::IsSecond(LD::Type<T3>{}),
                            LD::CT::IsMiliSecond(LD::Type<T4>{})
                    >
            >>>
    {
    private:
        T1 mFirst;
        T2 mSecond;
        T3 mThird;
        T4 mFourth;
    public:

        BasicTime(T1 && first, T2 && second, T3 && third) noexcept:mFirst{LD::Forward<T1>(first)},mSecond{LD::Forward<T2>(second)},mThird{LD::Forward<T3>(third)},mFourth{}
        {

        }
        BasicTime() noexcept:mFirst{},mSecond{},mThird{},mFourth{}
        {
            static constexpr LD::UInteger SEC_PER_DAY = 86400;
            static constexpr LD::UInteger SEC_PER_HOUR = 3600;
            static constexpr LD::UInteger SEC_PER_MIN = 60;

            using HourType = LD::CT::Detail::ExtractHourType<T1,T2,T3,T4>;
            using MinuteType = LD::CT::Detail::ExtractMinuteType<T1,T2,T3,T4>;
            using SecondType = LD::CT::Detail::ExtractSecondType<T1,T2,T3,T4>;
            using MiliSecondType = LD::CT::Detail::ExtractMilisecondType<T1,T2,T3,T4>;

            timeval tv;
            struct timezone   tz;
            gettimeofday(&tv, &tz);
            int milli = tv.tv_usec / 1000;

            // Form the seconds of the day
            long hms = tv.tv_sec % SEC_PER_DAY;
            hms += tz.tz_dsttime * SEC_PER_HOUR;
            hms -= tz.tz_minuteswest * SEC_PER_MIN;
            // mod `hms` to insure in positive range of [0...SEC_PER_DAY)
            hms = (hms + SEC_PER_DAY) % SEC_PER_DAY;

            // Tear apart hms into h:m:s
            int hour = hms / SEC_PER_HOUR;
            int min = (hms % SEC_PER_HOUR) / SEC_PER_MIN;
            int sec = (hms % SEC_PER_HOUR) % SEC_PER_MIN; // or hms % SEC_PER_MIN

            this->Hour() = HourType{LD::UInteger (hour)};
            this->Minute() = MinuteType{LD::UInteger (min)};
            this->Second() = SecondType{LD::UInteger (sec)};
            this->MiliSecond() = MiliSecondType{LD::UInteger (milli)};
        }
        const LD::CT::Detail::ExtractHourType<T1,T2,T3,T4> & Hour() const noexcept
        {
            if constexpr (LD::CT::IsHour(LD::Type<T1>{}))
            {
                return this->mFirst;
            } else if constexpr (LD::CT::IsHour(LD::Type<T2>{}))
            {
                return this->mSecond;
            }else if constexpr(LD::CT::IsHour(LD::Type<T3>{}))
            {
                return this->mThird;
            }else
            {
                return this->mFourth;
            }
        }
        LD::CT::Detail::ExtractHourType<T1,T2,T3,T4> & Hour()  noexcept
        {
            if constexpr (LD::CT::IsHour(LD::Type<T1>{}))
            {
                return this->mFirst;
            } else if constexpr (LD::CT::IsHour(LD::Type<T2>{}))
            {
                return this->mSecond;
            }else if constexpr(LD::CT::IsHour(LD::Type<T3>{}))
            {
                return this->mThird;
            }else
            {
                return this->mFourth;
            }
        }
        const LD::CT::Detail::ExtractMinuteType<T1,T2,T3,T4> & Minute() const noexcept
        {
            if constexpr (LD::CT::IsMinute(LD::Type<T1>{}))
            {
                return this->mFirst;
            } else if constexpr (LD::CT::IsMinute(LD::Type<T2>{}))
            {
                return this->mSecond;
            }else if constexpr(LD::CT::IsMinute(LD::Type<T3>{}))
            {
                return this->mThird;
            }else
            {
                return this->mFourth;
            }
        }
        LD::CT::Detail::ExtractMinuteType<T1,T2,T3,T4> & Minute()  noexcept
        {
            if constexpr (LD::CT::IsMinute(LD::Type<T1>{}))
            {
                return this->mFirst;
            } else if constexpr (LD::CT::IsMinute(LD::Type<T2>{}))
            {
                return this->mSecond;
            }else if constexpr(LD::CT::IsMinute(LD::Type<T3>{}))
            {
                return this->mThird;
            }else
            {
                return this->mFourth;
            }
        }

        const LD::CT::Detail::ExtractSecondType<T1,T2,T3,T4> & Second() const noexcept
        {
            if constexpr (LD::CT::IsSecond(LD::Type<T1>{}))
            {
                return this->mFirst;
            } else if constexpr (LD::CT::IsSecond(LD::Type<T2>{}))
            {
                return this->mSecond;
            }else if constexpr(LD::CT::IsSecond(LD::Type<T3>{}))
            {
                return this->mThird;
            }else
            {
                return this->mFourth;
            }
        }
        LD::CT::Detail::ExtractSecondType<T1,T2,T3,T4> & Second()  noexcept
        {
            if constexpr (LD::CT::IsSecond(LD::Type<T1>{}))
            {
                return this->mFirst;
            } else if constexpr (LD::CT::IsSecond(LD::Type<T2>{}))
            {
                return this->mSecond;
            }else if constexpr(LD::CT::IsSecond(LD::Type<T3>{}))
            {
                return this->mThird;
            }else
            {
                return this->mFourth;
            }
        }

        const LD::CT::Detail::ExtractMilisecondType<T1,T2,T3,T4> & MiliSecond() const noexcept
        {
            if constexpr (LD::CT::IsMiliSecond(LD::Type<T1>{}))
            {
                return this->mFirst;
            } else if constexpr (LD::CT::IsMiliSecond(LD::Type<T2>{}))
            {
                return this->mSecond;
            }else if constexpr(LD::CT::IsMiliSecond(LD::Type<T3>{}))
            {
                return this->mThird;
            }else
            {
                return this->mFourth;
            }
        }

        LD::CT::Detail::ExtractMilisecondType<T1,T2,T3,T4> & MiliSecond()  noexcept
        {
            if constexpr (LD::CT::IsMiliSecond(LD::Type<T1>{}))
            {
                return this->mFirst;
            } else if constexpr (LD::CT::IsMiliSecond(LD::Type<T2>{}))
            {
                return this->mSecond;
            }else if constexpr(LD::CT::IsMiliSecond(LD::Type<T3>{}))
            {
                return this->mThird;
            }else
            {
                return this->mFourth;
            }
        }
    };
    using Time = LD::BasicTime<LD::Hour<LD::UInteger>,LD::Minute<LD::UInteger>,LD::Second<LD::UInteger>,LD::Milisecond<LD::UInteger>>;

    template<typename T1, typename T2, typename T3, typename T4>
    inline auto ToImmutableString(const LD::BasicTime<T1,T2,T3,T4> & time) noexcept
    {
        using HourType = LD::CT::Detail::ExtractHourType<T1,T2,T3,T4>;
        using MinuteType = LD::CT::Detail::ExtractMinuteType<T1,T2,T3,T4>;
        using SecondType = LD::CT::Detail::ExtractSecondType<T1,T2,T3,T4>;
        using UnderlyingHourType = LD::GetType<decltype(LD::CT::RetrieveUnitUnderlyingType(LD::Type<HourType>{}))>;
        using UnderlyingMinuteType = LD::GetType<decltype(LD::CT::RetrieveUnitUnderlyingType(LD::Type<MinuteType>{}))>;
        using UnderlyingSecondType = LD::GetType<decltype(LD::CT::RetrieveUnitUnderlyingType(LD::Type<SecondType>{}))>;

        return ((time.Hour() < HourType (UnderlyingHourType (10)))?LD::ImmutableString{"0"}:LD::ImmutableString{""})+LD::ToImmutableString(time.Hour())+LD::ImmutableString{":"}+((time.Minute() < MinuteType (UnderlyingMinuteType (10))?LD::ImmutableString{"0"}:LD::ImmutableString{""})+LD::ToImmutableString(time.Minute())+LD::ImmutableString{":"}+((time.Second() < SecondType (UnderlyingSecondType (10)))?LD::ImmutableString{"0"}:LD::ImmutableString{""})+LD::ToImmutableString(time.Second()));
    }

    namespace Detail
    {
        static constexpr auto  TimeRegularExpression0 = ctll::basic_fixed_string{"(\\d{1,2})-(\\d{1,2})-(\\d{1,2})"};
        static constexpr auto  TimeRegularExpression1 = ctll::basic_fixed_string{"(\\d{1,2}):(\\d{1,2}):(\\d{1,2})"};
        static constexpr auto  TimeRegularExpression2 = ctll::basic_fixed_string{"(\\d{1,2}),(\\d{1,2}),(\\d{1,2})"};
    }

    template<typename T1, typename T2, typename T3, typename T4,typename ... A>
    LD::QueryResult<LD::Time(A...)> FromStringView(LD::Type<LD::BasicTime<T1,T2,T3,T4>>, LD::StringView view, A && ... args) noexcept
    {
        using HourType = LD::CT::Detail::ExtractHourType<T1,T2,T3,T4>;
        using MinuteType = LD::CT::Detail::ExtractMinuteType<T1,T2,T3,T4>;
        using SecondType = LD::CT::Detail::ExtractSecondType<T1,T2,T3,T4>;
        using MiliSecondType = LD::CT::Detail::ExtractMilisecondType<T1,T2,T3,T4>;


        if (auto [whole, hour, minute, second] = ctre::match<LD::Detail::TimeRegularExpression0>(view); whole)
        {
            LD::MakeContext(LD::TransactionResult{},LD::BasicTime<T1,T2,T3,T4>{LD::Hour<LD::UInteger>{LD::UInteger (hour)},LD::Minute<LD::UInteger>{LD::UInteger (hour)},LD::Second<LD::UInteger>{LD::UInteger (hour)}},LD::Forward<A>(args)...);
        }else if(auto [whole, hour, minute, second] = ctre::match<LD::Detail::TimeRegularExpression1>(view); whole)
        {
            LD::MakeContext(LD::TransactionResult{},LD::BasicTime<T1,T2,T3,T4>{LD::Hour<LD::UInteger>{LD::UInteger (hour)},LD::Minute<LD::UInteger>{LD::UInteger (hour)}},LD::Forward<A>(args)...);
        }else if(auto [whole, hour, minute, second] = ctre::match<LD::Detail::TimeRegularExpression2>(view); whole)
        {
            LD::MakeContext(LD::TransactionResult{},LD::BasicTime<T1,T2,T3,T4>{LD::Hour<LD::UInteger>{LD::UInteger (hour)},LD::Minute<LD::UInteger>{LD::UInteger (hour)}},LD::Forward<A>(args)...);
        }

        return LD::MakeContext(LD::TransactionError{},LD::Forward<A>(args)...);
    }




    template<typename Date, typename Time, class = void>
    class BasicDateTime;


    template<typename DateType, typename TimeType>
    class BasicDateTime<DateType,TimeType,LD::Enable_If_T<LD::Require<true>>>
    {
    private:
        TimeType mTime;
        DateType mDate;
    public:
        BasicDateTime() noexcept:mTime{},mDate{}
        {

        }
        BasicDateTime(DateType && date, TimeType && time) noexcept
        {

        }

        const DateType & Date () const noexcept {return this->mDate;}
        DateType & Date() noexcept {return this->mDate;}


        const TimeType & Time() const noexcept{return this->mTime;}
        TimeType & Time() noexcept{return this->mTime;}
    };



    using DateTime = LD::BasicDateTime<LD::Date,LD::BasicTime<LD::Hour<LD::UInteger>,LD::Minute<LD::UInteger>,LD::Second<LD::UInteger>,LD::Milisecond<LD::UInteger>>>;



    inline auto ToImmutableString(const LD::DateTime & dateTime) noexcept
    {
        return LD::ToImmutableString(dateTime.Date()) +
        LD::ImmutableString{" "} +
        LD::ToImmutableString(dateTime.Time());
    }

    inline static LD::Float  DateTimeToJulianDate(LD::DateTime datetime) noexcept
    {
        auto D = datetime.Date().Day().NativeRepresentation().Value();
        auto M = datetime.Date().Month().NativeRepresentation().Value();
        auto Y = datetime.Date().Year().NativeRepresentation().Value();
        auto hour = datetime.Time().Hour().NativeRepresentation().Value();
        auto minute = datetime.Time().Minute().NativeRepresentation().Value();
        auto second = datetime.Time().Second().NativeRepresentation().Value();

        //std::cout << "Muffins: " << LD::Float((hour-12.0)/24.0) + LD::Float((minute)/1440.0) + LD::Float (second/(86400.0)) << "\n";
        return (1461 * (Y + 4800 + (M - 14)/12))/4 +(367 * (M - 2 - 12 * ((M - 14)/12)))/12 - (3 * ((Y + 4900 + (M - 14)/12)/100))/4 + D - 32075 + LD::Float((hour)/24.0) + LD::Float((minute)/1440.0) + LD::Float (second/(86400.0));
    }

    inline static LD::Float  DateTimeToJulianNumber(LD::DateTime datetime) noexcept
    {
        auto D = datetime.Date().Day().NativeRepresentation().Value();
        auto M = datetime.Date().Month().NativeRepresentation().Value();
        auto Y = datetime.Date().Year().NativeRepresentation().Value();
        auto hour = datetime.Time().Hour().NativeRepresentation().Value();
        auto minute = datetime.Time().Minute().NativeRepresentation().Value();
        auto second = datetime.Time().Second().NativeRepresentation().Value();

        //std::cout << "Hour: " << hour << "\n";
        const LD::Integer a = (14-M)/12;
        const LD::Integer y = Y + 4800 -a;
        const LD::Integer m = M+12*a -3;

        LD::Integer result = D + LD::Floor(((153*m)+2)/5) +365*y + LD::Floor(y/4) - 32083;
        return (LD::Float)result + ((hour*3600.0) + (minute*60.0) + second)/86400.0;//+ (((LD::Float)hour-12.0f)/24.0f)+((LD::Float)minute/1440.0f)+((LD::Float)second/86400.0f);
        //return (1461 * (Y + 4800 + (M - 14)/12))/4 +(367 * (M - 2 - 12 * ((M - 14)/12)))/12 - (3 * ((Y + 4900 + (M - 14)/12)/100))/4 + D - 32075;
    }

    class GregorianDate
    {
    private:
        unsigned long        lJulianDay;


        static const LD::Float YmdToJd( const LD::Integer & Y, const LD::Integer & M, const LD::Integer & D, const LD::Float &  hour, const LD::Float & minute, const LD::Float & second ) noexcept
        {
            /*
            const LD::Integer a = (14-iMonth)/12;
            const LD::Integer y = iYear + 4800 -a;
            const LD::Integer m = iMonth+12*a -3;

            LD::Integer result = iDay + LD::Floor(((153*m)+2)/5) +365*y + LD::Floor(y/4) - 32083;
            const LD::Float  date = (LD::Float)result + (((LD::Float)hour-12.0f)/24.0f)+((LD::Float)minute/1440.0f)+((LD::Float)second/86400.0f);
             */
            return (1461 * (Y + 4800 + (M - 14)/12))/4 +(367 * (M - 2 - 12 * ((M - 14)/12)))/12 - (3 * ((Y + 4900 + (M - 14)/12)/100))/4 + D - 32075 + (hour-12)/24.0f + (minute)/1440.0f + second/(86400.0f);
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