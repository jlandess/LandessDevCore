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
namespace LD
{
    namespace TUI
    {
        template<LD::UInteger Size, typename TS = decltype("[a-zA-Z]+"_ts), class = void>
        class BasicImmutableTextInput;


        template<LD::UInteger Size, typename TS>
        class BasicImmutableTextInput<Size,TS,LD::Enable_If_T<LD::Require<LD::IsTypeString<TS>>>>
        {
        private:
            //constexpr const static ctll::fixed_string<TS::size()> Pattern = ctll::fixed_string<TS::size()>{ "[a-z"_ts};
            static constexpr auto Pattern = ctll::fixed_string(TS{});
            LD::BasicVec2D<LD::UInteger> mPosition;
            LD::ImmutableString<Size-1> mString;
            LD::UInteger mIndex;
            LD::BasicVec2D<LD::Integer> mTarget;
        public:
            constexpr BasicImmutableTextInput() noexcept:mIndex(0),mTarget({0,0})
            {

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
                    const LD::ImmutableString<Size-1> & string = {}) noexcept :mPosition(postion),mString(string),mIndex(0),mTarget({0,0})
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

            BasicImmutableTextInput & operator()( TermBoxRenderContext & context) noexcept
            {

                context.SetForegroundColor(TB_YELLOW);

                //the cursor is in the text region, we should now accept keyboard input
                if (context.GetMouse().Y() == this->mPosition.Y()-1
                && (context.GetMouse().X() > this->mPosition.X() && context.GetMouse().X() < this->mPosition.X()+Size))
                {
                    context.RenderWithFormat(LD::ToImmutableString("{}"),{this->mPosition.X(),this->mPosition.Y()-3},LD::ToImmutableString("found it"));


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


                        //termox will give us either a character as in (uint32_t to represent ASCII and Unicode Characters) or a key event,
                        //the below is when we get an actual character
                        auto onCharacter = [](const LD::Context<uint32_t,LD::ElementReference<LD::UInteger>,LD::ElementReference<LD::ImmutableString<Size-1>>> & context)
                        {
                            auto character = LD::Get<0>(context);
                            auto inputString = LD::Get<2>(context);
                            LD::UInteger  & currentIndex = LD::Get(LD::Get<1>(context));



                            char buffer[2];
                            buffer[0] = character;
                            buffer[1] = '\0';

                            
                            if (ctre::match<Pattern>(LD::StringView{buffer}))
                            {
                                LD::Get(inputString)[currentIndex] = LD::Get(character);
                                currentIndex = (currentIndex < Size-2)*(currentIndex+1) + (currentIndex >= Size-2)*currentIndex;
                            }

                            /*
                            if (currentIndex < Size-2)
                            {
                                ++currentIndex;
                            }else
                            {
                                currentIndex = Size-2;
                            }
                             */


                        };

                        //this is when we get a key instead of a character
                        auto onKey = [](const LD::Context<uint16_t,LD::ElementReference<LD::UInteger>,LD::ElementReference<LD::ImmutableString<Size-1>>> & context)
                        {
                            auto key = LD::Get(LD::Get<0>(context));
                            auto inputString = LD::Get<2>(context);
                            LD::UInteger  & currentIndex = LD::Get(LD::Get<1>(context));
                            if (key == TB_KEY_BACKSPACE || key == TB_KEY_BACKSPACE2)
                            {
                                LD::Get(LD::Get<2>(context))[currentIndex] = '\0';
                                currentIndex = (currentIndex > 0)*(currentIndex-1) + (currentIndex == 0)*0;
                            }
                        };

                        LD::Match(keyedEvent,onCharacter,onKey);
                    };

                    //activated when we get a mouse event
                    auto onMouse = [&](const LD::Context<LD::TermBoxMouseEvent,LD::UInteger&,LD::ImmutableString<Size-1>&,LD::TermBoxRenderContext&> & event)
                    {

                        //tb_width()/2+possibleXValue
                        const LD::UInteger & possibleX = LD::Get<0>(event).X();
                        const LD::UInteger & possibleY = LD::Get<0>(event).Y();
                        const LD::UInteger width = tb_width();
                        const LD::UInteger height = tb_height();
                        int x = possibleX - LD::Float(width)/LD::Float(2.0);
                        int y = possibleY - LD::Float(height)/LD::Float(2.0);
                        this->mTarget = {x,y};
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

                    context.RenderWithFormat(LD::ToImmutableString("{}"),{this->mPosition.X(),this->mPosition.Y()-4},
                            LD::ToImmutableString(this->mIndex));
                    LD::Match(event,onKeyBoardEvent,onMouse,onDefault,onResize);
                }





                auto bar = LD::ImmutableString<Size-1>{'_'};
                auto edge = LD::ImmutableString<1>{'|'};
                //context.SetForegroundCurrentModifier(LD::eTBUnderline);
                context.RenderWithFormat(LD::ToImmutableString("{}"),{this->mPosition.X()+1,this->mPosition.Y()},bar);
                //context.SetForegroundCurrentModifier(LD::eTBNone);
                context.RenderWithFormat(LD::ToImmutableString("{}"),{this->mPosition.X(),this->mPosition.Y()-1},edge);
                //todo add rendering the string with underlining enabled then render the below at the new offset
                //context.RenderWithFormat(LD::ToImmutableString("{}"),{this->mPosition.X()+1,this->mPosition.Y()-1},bar+edge);
                //context.GetForegroundModifier() = LD::eTBUnderline;
                //context.SetForegroundCurrentModifier(LD::eTBUnderline);

                //context.SetBackgroundColor(LD::eTBlue);

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
                //context.Render(LD::ToImmutableString("a"),{this->mPosition.X()+1,this->mPosition.Y()-1});
                //context.SetBackgroundColor(LD::eTBNone);
                //context.RenderWithFormat(LD::ToImmutableString("{}"),{this->mPosition.X()+1,this->mPosition.Y()-1},LD::StringView{this->mString.Data()});

                //context.SetForegroundCurrentModifier(LD::eTBNone);
                context.RenderWithFormat(LD::ToImmutableString("{}"),{this->mPosition.X()+Size,this->mPosition.Y()-1},edge);
                //context.SetForegroundCurrentModifier(LD::eTBNone);
                context.SetForegroundColor(TB_GREEN);
                return (*this);
            }
        };
    }
}
#endif //LANDESSDEVCORE_TUITEXTINPUT_H
