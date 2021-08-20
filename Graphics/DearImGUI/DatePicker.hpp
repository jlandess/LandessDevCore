//
// Created by phoenixflower on 8/2/21.
//

#ifndef LANDESSDEVCORE_DATEPICKER_HPP
#define LANDESSDEVCORE_DATEPICKER_HPP
#include <iostream>
#include "imgui_impl_datepicker.h"
#include "RenderingContext.hpp"
#include "Primitives/General/DateTime.h"
#include "Memory/ElementReference.h"
#include "TypeTraits/Conditional.hpp"
/*
         * template<typename LabelType,typename First>
        class BasicDatePicker;
        template<typename LabelType,typename First, typename Second, typename Third>
        class BasicDatePicker<LabelType,LD::BasicDate<First,Second,Third>>
        {
        private:
            LD::BasicDate<First,Second,Third> mDate;
        public:

            ////IMGUI_API bool DateChooser(const char* label,tm& dateOut,const char* dateFormat="%d/%m/%Y",bool closeWhenMouseLeavesIt=true,bool* pSetStartDateToDateOutThisFrame=NULL,const char* leftArrow="<",const char* rightArrow=">",const char* upArrowString="   ^   ",const char* downArrowString="   v   ");
            void operator()(const LD::IMGUI::RenderingContext & context) const noexcept
            {
                tm collectedDate;
                ImGui::DateChooser("Date Label",collectedDate);

            }
        };
         */
namespace LD
{
    namespace IMGUI
    {
        namespace Detail
        {
            template<typename First, typename Second, typename Third>
            constexpr LD::StringView BasicDatePickerFormatRequest(LD::Type<LD::BasicDate<First,Second,Third>>) noexcept
            {
                if constexpr (
                        LD::CT::IsSame(LD::Type<First>{},LD::Type<LD::Month<LD::UInteger>>{}) &&
                        LD::CT::IsSame(LD::Type<Second>{},LD::Type<LD::Day<LD::UInteger>>{}) &&
                        LD::CT::IsSame(LD::Type<Third>{},LD::Type<LD::Year<LD::UInteger>>{}))
                {
                    //printf("being called at all \n");
                    //printf("rawrbunnies %s \n",date.Day().ToImmutableString().Data());
                    //return date.Month().ToImmutableString()+LD::ImmutableString{"-"}+date.Day().ToImmutableString()+LD::ImmutableString{"-"}+date.Year().ToImmutableString();
                }
                else if constexpr(LD::CT::IsSame(LD::Type<First>{},LD::Type<LD::Day<LD::UInteger>>{}) &&
                                  LD::CT::IsSame(LD::Type<Second>{},LD::Type<LD::Month<LD::UInteger>>{}) &&
                                  LD::CT::IsSame(LD::Type<Third>{},LD::Type<LD::Year<LD::UInteger>>{}))
                {
                    return LD::StringView{"%d/%m/%Y"};
                }
                else if constexpr(LD::CT::IsSame(LD::Type<First>{},LD::Type<LD::Month<LD::UInteger>>{}) &&
                                  LD::CT::IsSame(LD::Type<Second>{},LD::Type<LD::Day<LD::UInteger>>{}) &&
                                  LD::CT::IsSame(LD::Type<Third>{},LD::Type<LD::Year<LD::UInteger>>{}))
                {
                    return LD::StringView{"%m/%d/%Y"};
                }
                else if constexpr(LD::CT::IsSame(LD::Type<First>{},LD::Type<LD::Year<LD::UInteger>>{}) &&
                                  LD::CT::IsSame(LD::Type<Second>{},LD::Type<LD::Month<LD::UInteger>>{}) &&
                                  LD::CT::IsSame(LD::Type<Third>{},LD::Type<LD::Day<LD::UInteger>>{}))
                {
                    return LD::StringView{"%Y/%m/%d"};
                }
                return LD::StringView{""};
            }
        }

        template<typename LabelType,typename CallBack ,typename DateType>
        class BasicDatePicker;

        template<typename LabelType, typename CallBack,typename First, typename Second, typename Third, typename ... A>
        class BasicDatePicker<LabelType,CallBack,LD::BasicDate<First,Second,Third>(A...)>
        {
        private:
            using BackendType = LD::Detail::Conditional_T<(LD::Detail::IsLValueReference<CallBack>::value),LD::ElementReference <LD::Detail::Decay_T<CallBack>>,LD::Detail::Decay_T<CallBack>>;
            BackendType mCurrentCallBack;
            LabelType mLabel;
            tm mDateBuffer;
            LD::Context<A...> mContext;

        public:
            BasicDatePicker() noexcept:mDateBuffer{}
            {

            }
            BasicDatePicker(LabelType && string, CallBack && callback,LD::Type<LD::BasicDate<First,Second,Third>> ,A && ... args) noexcept:mLabel{LD::Forward<LabelType>(string)},mCurrentCallBack{LD::Forward<CallBack>(callback)},mContext{LD::Forward<A>(args)...},mDateBuffer{}
            {

            }
            ////IMGUI_API bool DateChooser(const char* label,tm& dateOut,const char* dateFormat="%d/%m/%Y",bool closeWhenMouseLeavesIt=true,bool* pSetStartDateToDateOutThisFrame=NULL,const char* leftArrow="<",const char* rightArrow=">",const char* upArrowString="   ^   ",const char* downArrowString="   v   ");
            void operator()(const LD::IMGUI::RenderingContext & context)  noexcept
            {

                auto requestedFormat = LD::IMGUI::Detail::BasicDatePickerFormatRequest(LD::Type<LD::BasicDate<LD::Day<LD::UInteger>,LD::Month<LD::UInteger>,LD::Year<LD::UInteger>>>{});
                if(ImGui::DateChooser(this->mLabel.Data(),mDateBuffer,requestedFormat.data()))
                {
                    LD::BasicDate<LD::Day<LD::UInteger>,LD::Month<LD::UInteger>,LD::Year<LD::UInteger>> selectedDate;
                    selectedDate.Day() = LD::Day<LD::UInteger>{LD::UInteger (mDateBuffer.tm_mday)};
                    selectedDate.Month() = LD::Month<LD::UInteger>{LD::UInteger (mDateBuffer.tm_mon)+1};
                    selectedDate.Year() = LD::Year<LD::UInteger>{LD::UInteger (mDateBuffer.tm_year)+1900};
                    //std::cout << "Year: " << mDateBuffer.tm_year << std::endl;
                    //selectedDate.Year() = LD::Year<LD::UInteger>{LD::UInteger (mDateBuffer.tm_year)};
                    //strftime(buffer, 26, "%m:%d:%Y %H:%M:%S", (const tm*)&mDateBuffer);
                    //std::cout << "Buffer: " << buffer << std::endl;
                    //day = local->tm_mday;            // get day of month (1 to 31)
                    //month = local->tm_mon + 1;      // get month of year (0 to 11)
                    //year = local->tm_year + 1900;   // get year since 1900
                    LD::Get(this->mCurrentCallBack)(selectedDate);
                }
            }
        };

        template<typename StringType, typename CallBack, typename First, typename Second, typename Third, typename ... A> BasicDatePicker(StringType &&, CallBack && ,LD::Type<LD::BasicDate<First,Second,Third>>,A &&...) -> BasicDatePicker<StringType,CallBack,LD::BasicDate<First,Second,Third>(A...)>;
    }
}
#endif //LANDESSDEVCORE_DATEPICKER_HPP
