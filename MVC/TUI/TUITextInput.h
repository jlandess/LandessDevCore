//
// Created by phoenixflower on 6/11/20.
//

#ifndef LANDESSDEVCORE_TUITEXTINPUT_H
#define LANDESSDEVCORE_TUITEXTINPUT_H
#include "Definitions/Integer.hpp"
#include "Primitives/General/Vec2.hpp"
#include "Primitives/General/Immutable.hpp"
#include "MVC/TermBoxRenderingContext.hpp"
#include "Primitives/General/ctre.hpp"
#include "Algorithms/StringToPrimitive.hpp"
#include "TUIEvent.h"
namespace LD
{
    namespace TUI
    {
        template<LD::UInteger Size, typename TS = decltype("[a-zA-Z0-9]+"_ts), class = void>
        class BasicImmutableTextInput;


        /**
         *
         * @tparam Size
         * @tparam TS Represent the regular expression that dictates what input is allowed to be entered into the text input
         * @brief Provides the core component of accepting text input as a textbox on any part of the terminal screen
         */
        template<LD::UInteger Size, typename TS>
        class BasicImmutableTextInput<Size,TS,LD::Enable_If_T<LD::Require<LD::IsTypeString<TS>>>>
        {
        private:
            static constexpr auto Pattern = ctll::fixed_string(TS{});
            LD::BasicVec2D<LD::UInteger> mPosition;
            LD::ImmutableString<Size-1> mString;
            LD::UInteger mIndex;
            LD::Variant<LD::TUI::StableState,LD::TUI::StateChanged> mState;
        public:
            constexpr BasicImmutableTextInput() noexcept:mIndex(0)
            {

            }
            /**
             *
             * @tparam Args The arguements you want to be made available when the event is being considered in the caller
             * @param arguements The actual arguements being passed, rvalues will be copied, lvalues will not be copied
             * @return
             * @brief Provides a way to respond to changes in the state of the textbox input without having to poll it or over mutate objects
             */
            template<typename ... Args>
            LD::ContextualVariant<LD::Variant<LD::TUI::StateChanged,LD::TUI::StableState>(Args...)> Event(Args && ... arguements) const noexcept
            {
                LD::ContextualVariant<LD::Variant<LD::TUI::StateChanged,LD::TUI::StableState>(Args...)> ret[2];
                ret[0] = LD::MakeContext(LD::TUI::StableState{},LD::Forward<Args>(arguements)...);
                ret[1] = LD::MakeContext(LD::TUI::StateChanged{},LD::Forward<Args>(arguements)...);
                auto onStableState = [](const LD::TUI::StableState & stableState) noexcept
                {
                    return 0;
                };

                auto onStateChanged = [](const LD::TUI::StateChanged & changedState) noexcept
                {
                    return 1;
                };
                return ret[LD::Match(this->mState,onStableState,onStateChanged)];
            }

            const LD::UInteger Width() const noexcept
            {
                return Size;
            }

            const LD::UInteger Height() const noexcept
            {
                return 2;
            }

            constexpr BasicImmutableTextInput(
                    const LD::BasicVec2D<LD::UInteger> & postion,
                    const LD::ImmutableString<Size-1> & string = {}) noexcept :mPosition(postion),mString(string),mIndex(0)
            {

            }

            const LD::ImmutableString<Size-1> & String() const noexcept
            {
                return this->mString;
            }
            BasicImmutableTextInput & operator = (const LD::ImmutableString<Size-1> & string) noexcept
            {
                this->mString = string;
                return (*this);
            }

            BasicImmutableTextInput & operator = (const LD::BasicVec2D<LD::UInteger> & position) noexcept
            {
                this->mPosition = position;
                return (*this);
            }

            /**
             *
             * @param context The context which maintains the state of the termbox application alongside the necessary infastructure to render stuff to the screen
             *
             * @return
             */
            BasicImmutableTextInput & operator()( TermBoxRenderContext & context) noexcept
            {

                context.SetForegroundColor(TB_YELLOW);

                //the cursor is in the text region, we should now accept keyboard input
                if (context.GetMouse().Y() == this->mPosition.Y()-1
                && (context.GetMouse().X() > this->mPosition.X() && context.GetMouse().X() < this->mPosition.X()+Size))
                {
                    //context.RenderWithFormat(LD::ToImmutableString("{}"),{this->mPosition.X(),this->mPosition.Y()-3},LD::ToImmutableString("found it"));


                    //this->mString[0] = (rand()%96)+32;

                    auto event = context.Event(this->mIndex,this->mString,context);

                    //when we get an event from the keyboard this event is called
                    auto onKeyBoardEvent = [](const LD::Context<LD::TermBoxKeyboardEvent,LD::UInteger&,LD::ImmutableString<Size-1>&,LD::TermBoxRenderContext&> & event)
                    {
                        auto currentIndex = LD::Get<1>(event);
                        auto inputString = LD::Get<2>(event);

                        auto keyedEvent = LD::Get<0>(event)(
                                LD::ElementReference<LD::UInteger>{currentIndex},
                                LD::ElementReference<LD::ImmutableString<Size-1>>{inputString});


                        //termbox will give us either a character as in (uint32_t to represent ASCII and Unicode Characters) or a key event,
                        //the below is when we get an actual character
                        using CharacterContext = LD::Context<uint32_t,LD::ElementReference<LD::UInteger>,LD::ElementReference<LD::ImmutableString<Size-1>>>;
                        using KeyContext = LD::Context<uint16_t,LD::ElementReference<LD::UInteger>,LD::ElementReference<LD::ImmutableString<Size-1>>>;
                        auto onCharacter = [](const CharacterContext & context)
                        {
                            auto character = LD::Get<0>(context);
                            auto inputString = LD::Get<2>(context);
                            LD::UInteger  & currentIndex = LD::Get(LD::Get<1>(context));

                            LD::Get(inputString)[currentIndex] = LD::Get(character);
                            const bool matchesPattern = ctre::match<Pattern>(LD::StringView{inputString->Data()});

                            //only keep the "appended character" if it matches the regular expression
                            LD::Get(inputString)[currentIndex] = (matchesPattern)*LD::Get(inputString)[currentIndex] + (!matchesPattern)*'\0';

                            //increment the index if we had a match
                            currentIndex = (matchesPattern)*(currentIndex+1) + (!matchesPattern)*currentIndex;

                            //if the index is greater than the size of the string, just keep it at the end
                            currentIndex = (currentIndex >= Size-2)*(Size-2) + (currentIndex < Size-2)*currentIndex;

                            //todo update state to reflect we had a character event
                        };

                        //this is when we get a key instead of a character
                        auto onKey = [](const KeyContext & context)
                        {
                            auto key = LD::Get(LD::Get<0>(context));
                            auto inputString = LD::Get<2>(context);
                            LD::UInteger  & currentIndex = LD::Get(LD::Get<1>(context));
                            const bool isBackspace = (key == TB_KEY_BACKSPACE || key == TB_KEY_BACKSPACE2);

                            //delete the character by assigning it to 0 ('\0') if the backpsace key was pressed
                            LD::Get(LD::Get<2>(context))[currentIndex] = (isBackspace)*'\0' + (!isBackspace)*LD::Get(LD::Get<2>(context))[currentIndex] ;

                            //decrement the indicator's index when we press the backspace key, or keep it in place when anything else occurs
                            //if the value is about to go below 0, simply keep keep it 0 (eg the beginning of the string)
                            currentIndex = (currentIndex > 0 && isBackspace)*(currentIndex-1) + (currentIndex == 0)*0 + (!isBackspace)*currentIndex;

                            //todo update state to reflect we had a key event
                        };

                        LD::Match(keyedEvent,onCharacter,onKey);
                    };

                    //activated when we get a mouse event
                    auto onMouse = [](const LD::Context<LD::TermBoxMouseEvent,LD::UInteger&,LD::ImmutableString<Size-1>&,LD::TermBoxRenderContext&> & event)
                    {

                        /*
                        //tb_width()/2+possibleXValue
                        const LD::UInteger & possibleX = LD::Get<0>(event).X();
                        const LD::UInteger & possibleY = LD::Get<0>(event).Y();
                        const LD::UInteger width = tb_width();
                        const LD::UInteger height = tb_height();
                        int x = possibleX - LD::Float(width)/LD::Float(2.0);
                        int y = possibleY - LD::Float(height)/LD::Float(2.0);
                        this->mTarget = {x,y};
                         */
                    };

                    //activated when we go through a frame but no event has been picked up
                    auto onDefault = [](const LD::Context<LD::TermBoxEmptyEvent,LD::UInteger&,LD::ImmutableString<Size-1>&,LD::TermBoxRenderContext&> & event)
                    {
                        //++this->mTarget;

                    };

                    //activates when the terminal windows has been resized
                    auto onResize = [](const LD::Context<LD::TermBoxResizingEvent,LD::UInteger&,LD::ImmutableString<Size-1>&,LD::TermBoxRenderContext&> & event)
                    {

                    };


                    LD::Match(event,onKeyBoardEvent,onMouse,onDefault,onResize);
                }

                auto bar = LD::ImmutableString<Size-1>{'_'};
                auto edge = LD::ImmutableString<1>{'|'};
                //context.SetForegroundCurrentModifier(LD::eTBUnderline);
                context.RenderWithFormat(LD::ToImmutableString("{}"),{this->mPosition.X()+1,this->mPosition.Y()},bar);
                //context.SetForegroundCurrentModifier(LD::eTBNone);
                context.RenderWithFormat(LD::ToImmutableString("{}"),{this->mPosition.X(),this->mPosition.Y()-1},edge);

                LD::For<Size>([](
                        auto I,
                        auto && input,
                        auto && context,
                        auto && position,
                        auto && index)
                {
                    if (I == index)
                    {
                        context.SetBackgroundColor(LD::eTBlue);
                    }
                    context.Render(LD::ToImmutableString(input[I]),{position.X()+I+1,position.Y()-1});

                    if (I == index)
                    {
                        context.SetBackgroundColor(LD::eTBNone);
                    }
                    return true;
                },this->mString,context,this->mPosition,this->mIndex);

                context.RenderWithFormat(LD::ToImmutableString("{}"),{this->mPosition.X()+Size,this->mPosition.Y()-1},edge);
                context.SetForegroundColor(TB_GREEN);
                return (*this);
            }
        };

        namespace Detail
        {
            template<typename T, class = void >
            class GenerateRegex;

            template<typename T>
            class GenerateRegex<T,LD::Enable_If_T<
                    LD::Require<
                    LD::IsFloatingPoint<T>
                    >>>
            {
                using type = decltype("[+-]?([0-9]*[.])?[0-9]+"_ts);
            };

            template<typename T>
            class GenerateRegex<T,LD::Enable_If_T<
                    LD::Require<
                            LD::IsUnsignedInteger<T>
                    >>>
            {
                using type = decltype("^\\d+$"_ts);
            };

            template<typename T>
            class GenerateRegex<T,LD::Enable_If_T<
                    LD::Require<
                            LD::IsInteger<T>
                    >>>
            {
                using type = decltype("^-?\\d+$"_ts);
            };
            template<typename T>
            class GenerateRegex<T,LD::Enable_If_T<
                    LD::Require<
                            LD::IsSame<T,char>
                    >>>
            {
                using type = decltype("[a-zA-Z0-9]"_ts);
            };
            template<typename T>
            class GenerateRegex<T,LD::Enable_If_T<
                    LD::Require<
                            LD::IsSame<T,unsigned char>
                    >>>
            {
                using type = decltype("[a-zA-Z0-9]"_ts);
            };

            template<typename T>
            class GenerateRegex<T,LD::Enable_If_T<
                    LD::Require<
                            LD::IsSame<T,bool>
                    >>>
            {
                using type = decltype("[0-1]"_ts);
            };
        }

        template<typename T>
        using GenerateRegex = typename LD::TUI::Detail::GenerateRegex<T>::type;
        /**
        *
        * @tparam Size The width of the proposed Text input
        * @tparam T The type you expect to be returned from the text input
         * @brief All the primitives types are considered with the various regular expressions
        */
        template<LD::UInteger Size, typename T, class = void>
        class ImmutableTextInput;


        template<LD::UInteger Size, typename T>
        class ImmutableTextInput<Size,T,LD::Enable_If_T<LD::Require<LD::IsPrimitive<T>>>>: public LD::TUI::BasicImmutableTextInput<Size,LD::TUI::GenerateRegex<T>>
        {
        private:

        public:
            const T operator()() const noexcept
            {
                auto resultVariant = LD::StringAsNumber<T>(LD::StringView{this->GetString().Data(),this->GetString().Size()});
                T result = LD::Match(resultVariant,[](const T & obj){ return obj;},[](auto &&){ return T{};});
                return result;
            }
        };

        template<LD::UInteger Size, typename T, class = void>
        class ReferencedImmutableTextInput;


        template<LD::UInteger Size, typename T>
        class ReferencedImmutableTextInput<Size,T,LD::Enable_If_T<LD::Require<LD::IsPrimitive<T>>>>: public LD::TUI::BasicImmutableTextInput<Size,LD::TUI::GenerateRegex<T>>
        {
        private:
            LD::ElementReference<T> mReference;
            using BaseClass = LD::TUI::BasicImmutableTextInput<Size,LD::TUI::GenerateRegex<T>>;
        public:
            inline constexpr explicit ReferencedImmutableTextInput(const LD::ElementReference<T> & reference) noexcept:mReference{reference}
            {

            }

            ReferencedImmutableTextInput & operator()( TermBoxRenderContext & context) noexcept
            {
                BaseClass::operator()(context);
                LD::Ref<LD::ImmutableString<Size-1>> referencedString = LD::Ref<LD::ImmutableString<Size-1>>{this->GetString()};
                auto possibleEvent = this->Event(LD::Ref<T>{this->mReference},LD::Ref<LD::ImmutableString<Size-1>>{referencedString});

                auto onChange = [](const LD::Context<LD::TUI::StateChanged,LD::Ref<T>,LD::Ref<LD::ImmutableString<Size-1>>> & context)
                {
                    LD::Ref<LD::ImmutableString<Size-1>> referencedString = LD::Get<2>(context);
                    T & referencedObject = LD::Get(LD::Get<1>(context));
                    auto resultVariant = LD::StringAsNumber<T>(LD::StringView{referencedString->Data(),referencedString->Size()});
                    referencedObject = LD::Match(resultVariant,[](const T & obj){ return obj;},[](auto &&){ return T {};});

                };

                auto onNoChange = [](const LD::Context<LD::TUI::StableState,LD::Ref<T>,LD::Ref<LD::ImmutableString<Size-1>>> & context)
                {

                };

                LD::Match(possibleEvent,onChange,onNoChange);

                return (*this);
            }
        };

    }
}
#endif //LANDESSDEVCORE_TUITEXTINPUT_H
