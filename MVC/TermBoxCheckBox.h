//
// Created by phoenixflower on 8/4/20.
//

#ifndef LANDESSDEVCORE_TERMBOXCHECKBOX_H
#define LANDESSDEVCORE_TERMBOXCHECKBOX_H
#include "TermBoxRenderingContext.hpp"
#include "Primitives/General/Vec2.hpp"
namespace LD
{
    namespace TUI
    {
        class CheckBox
        {
        private:
            wchar_t mBoxState[2] = {9634,9635};
            LD::BasicVec2D<LD::Integer> mPosition;
            wchar_t mSelectionKey;
            LD::UInteger mState;
        public:
            constexpr CheckBox(const LD::BasicVec2D<LD::Integer> & loc = {0,0},const bool & state = false) noexcept:mState{state},mPosition{loc},mSelectionKey{'a'}
            {

            }

            LD::TermBoxRenderContext  & operator()( LD::TermBoxRenderContext & context) noexcept
            {
                if (context.GetCursor().X() == this->mPosition.X() && context.GetCursor().Y() == this->mPosition.Y())
                {

                }


                context.Render(LD::UInteger{this->mState},this->mPosition+LD::BasicVec2D<LD::Integer>{0,-1});

                //context.Render(this->mBoxState[this->mState],this->mPosition);
                return context;
            }
        };
    }
}
#endif //LANDESSDEVCORE_TERMBOXCHECKBOX_H
