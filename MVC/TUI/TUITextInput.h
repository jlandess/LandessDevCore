//
// Created by phoenixflower on 6/11/20.
//

#ifndef LANDESSDEVCORE_TUITEXTINPUT_H
#define LANDESSDEVCORE_TUITEXTINPUT_H
#include "Definitions/Integer.hpp"
#include "Primitives/General/Vec2.hpp"
#include "Primitives/General/Immutable.hpp"
#include "MVC/TermBoxRenderingContext.hpp"
namespace LD
{
    namespace TUI
    {
        template<LD::UInteger Size>
        class BasicImmutableTextInput
        {
        private:
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

                    auto onKey = [](const LD::Context<LD::TermBoxKeyboardEvent,LD::UInteger&,LD::ImmutableString<Size-1>&,LD::TermBoxRenderContext&> & event)
                    {
                        auto character = LD::Get<0>(event).Character();
                        auto key = LD::Get<0>(event).Key();
                        auto modifier = LD::Get<0>(event).Modifier();
                        if (character >= 'a' && character <= 'z')
                        {
                            LD::UInteger & currentIndex = LD::Get(LD::Get<1>(event));
                            LD::Get(LD::Get<2>(event))[currentIndex] = character;

                            if (currentIndex < Size-2)
                            {
                                ++currentIndex;
                            }else
                            {
                                currentIndex = Size-2;
                            }

                        }


                        if (key == TB_KEY_BACKSPACE || key == TB_KEY_BACKSPACE2)
                        {
                            LD::UInteger & currentIndex = LD::Get(LD::Get<1>(event));
                            LD::Get(LD::Get<2>(event))[currentIndex] = '\0';
                            //--currentIndex;

                            currentIndex = (currentIndex > 0)*(currentIndex-1) + (currentIndex == 0)*0;

                        }

                        //currentIndex = LD::Max(currentIndex,Size);
                    };

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

                    auto onDefault = [&](const LD::Context<LD::TermBoxEmptyEvent,LD::UInteger&,LD::ImmutableString<Size-1>&,LD::TermBoxRenderContext&> & event)
                    {
                        //++this->mTarget;

                    };

                    auto onResize = [](const LD::Context<LD::TermBoxResizingEvent,LD::UInteger&,LD::ImmutableString<Size-1>&,LD::TermBoxRenderContext&> & event)
                    {

                    };

                    auto defaultCase = [&](auto &&)
                    {
                        context.RenderWithFormat(LD::ToImmutableString("{}"),{this->mPosition.X(),this->mPosition.Y()-4},LD::ToImmutableString("no key"));
                    };

                    context.RenderWithFormat(LD::ToImmutableString("{}"),{this->mPosition.X(),this->mPosition.Y()-4},
                            LD::ToImmutableString(this->mIndex));
                    LD::Match(event,onKey,onMouse,onDefault,onResize);
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
