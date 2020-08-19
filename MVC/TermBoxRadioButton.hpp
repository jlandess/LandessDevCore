//
// Created by phoenixflower on 8/6/20.
//

#ifndef LANDESSDEVCORE_TERMBOXRADIOBUTTON_HPP
#define LANDESSDEVCORE_TERMBOXRADIOBUTTON_HPP
#include "TermBoxRenderingContext.hpp"
#include "Primitives/General/Vec2.hpp"
namespace LD
{
    namespace TUI
    {
        class CheckBox
        {
        private:
            //9,673
            wchar_t mBoxState[2] = {9634,9635};
            LD::BasicVec2D<LD::Integer> mPosition;
            wchar_t mSelectionKey;
            bool mState;
        public:
            constexpr CheckBox(const LD::BasicVec2D<LD::Integer> & loc = {0,0},const bool & state = false) noexcept:mState{state},mPosition{loc},mSelectionKey{' '}
            {

            }

            LD::TermBoxRenderContext  & operator()( LD::TermBoxRenderContext & context) noexcept
            {
                if (context.GetCursor().X() == this->mPosition.X() && context.GetCursor().Y() == this->mPosition.Y())
                {
                    auto event = context.Event(this->mSelectionKey,this->mState);

                    using CharacterContext = LD::Context<uint32_t,wchar_t&,bool&>;
                    using KeyContext = LD::Context<uint16_t,wchar_t&,bool&>;

                    auto onKeyBoardEvent = [](const LD::Context<LD::TermBoxKeyboardEvent,wchar_t&,bool&> & event) noexcept
                    {
                        auto keyedEvent = LD::Get<0>(event)(LD::ElementReference<wchar_t>{LD::Get<1>(event)},LD::ElementReference<bool>{LD::Get<2>(event)});


                        auto onCharacter = [](const CharacterContext & context) noexcept
                        {
                            auto character = LD::Get(LD::Get<0>(context));
                            auto selectionKey = LD::Get(LD::Get<1>(context));

                            if (selectionKey == character)
                            {
                                LD::Get(LD::Get<2>(context)) = not LD::Get(LD::Get<2>(context));
                            }
                        };

                        auto onKey = [](const KeyContext & context) noexcept
                        {
                            auto key = LD::Get(LD::Get<0>(context));
                            auto selectionKey = LD::Get(LD::Get<1>(context));

                            if (key == selectionKey)
                            {
                                LD::Get(LD::Get<2>(context)) = not LD::Get(LD::Get<2>(context));
                            }

                        };

                        LD::Match(keyedEvent,onCharacter,onKey);
                    };

                    auto onMouse = [](const LD::Context<LD::TermBoxMouseEvent,wchar_t&,bool&> & event) noexcept
                    {

                    };

                    auto onDefault = [](const LD::Context<LD::TermBoxEmptyEvent,wchar_t&,bool&> & event)
                    {

                    };

                    auto onResize = [](const LD::Context<LD::TermBoxResizingEvent,wchar_t&,bool&> & event) noexcept
                    {

                    };

                    LD::Match(event,onKeyBoardEvent,onMouse,onDefault,onResize);
                }


                //context.Render(LD::UInteger{this->mState},this->mPosition+LD::BasicVec2D<LD::Integer>{0,-1});

                context.Render(this->mBoxState[this->mState],this->mPosition);
                return context;
            }
        };
    }
}
#endif //LANDESSDEVCORE_TERMBOXRADIOBUTTON_HPP
