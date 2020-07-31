//
// Created by phoenixflower on 7/18/20.
//

#ifndef LANDESSDEVCORE_TERMBOXPROGRESSBAR_HPP
#define LANDESSDEVCORE_TERMBOXPROGRESSBAR_HPP
#include "Primitives/General/Immutable.hpp"
#include "TermBoxRenderingContext.hpp"
#include "ScopedRenderContextState.hpp"
namespace LD
{
    namespace TUI
    {
        class ProgressBar
        {
        private:
            LD::BasicVec2D<LD::Integer> mPosition;
            LD::Float mPercentage;
            unsigned short mColor;
        public:
            constexpr ProgressBar(const LD::BasicVec2D<LD::Integer> & location,const LD::Float & percentage = 0.0,const unsigned short & color = TB_YELLOW) noexcept :mPosition(location),mPercentage{percentage},mColor{color}
            {

            }
            LD::TermBoxRenderContext & operator()( LD::TermBoxRenderContext & context) const noexcept
            {
                LD::TUI::ScopedRenderingContextState scopedState{context};

                context.Render('|',this->mPosition);
                context.Render('|',this->mPosition+LD::BasicVec2D<LD::UInteger>{11,0});
                auto bar = LD::ImmutableString<10>{'_'};
                context.RenderWithFormat(LD::ToImmutableString("{}"),{this->mPosition.X()+1,this->mPosition.Y()+1},bar);
                LD::UInteger numberOfActiveBars = (LD::UInteger (LD::Abs(this->mPercentage)*10))%11;
                context.SetBackgroundColor(TB_YELLOW);
                context.SetForegroundCurrentModifier(eTBUnderline);
                for(LD::UInteger n = 0;n<numberOfActiveBars;++n)
                {

                    context.Render('_',this->mPosition+LD::BasicVec2D<LD::Integer>{n+1,0});
                }
                context.SetBackgroundColor(TB_DEFAULT);
                LD::UInteger difference = 10-numberOfActiveBars;
                for(LD::UInteger n = 0;n<difference;++n)
                {
                    context.Render('_',this->mPosition+LD::BasicVec2D<LD::Integer>{n+numberOfActiveBars+1,0});
                }
                /*
                for(LD::UInteger n = numberOfActiveBars;n<10;++n)
                {
                    context.Render(' ',this->mPosition+LD::BasicVec2D<LD::Integer>{n+numberOfActiveBars,0});
                }
                 */
                //context.Render(LD::ToImmutableString(numberOfActiveBars),this->mPosition);
                /*
                auto bar = LD::ImmutableString<10>{'_'};

                context.RenderWithFormat(LD::ToImmutableString("{}"),{this->mPosition.X()+1,this->mPosition.Y()},bar);
                context.RenderWithFormat(LD::ToImmutableString("{}"),{this->mPosition.X(),this->mPosition.Y()-1},'|');
                context.RenderWithFormat(LD::ToImmutableString("{}"),{this->mPosition.X()+11,this->mPosition.Y()-1},'|');

                LD::UInteger numberOfActiveBars = (LD::UInteger (LD::Abs(this->mPercentage)*10))%10;
                context.SetForegroundCurrentModifier(eTBUnderline);
                context.SetBackgroundColor(TB_YELLOW);
                for(LD::UInteger n = 0;n<numberOfActiveBars;++n)
                {
                    context.Render('_',{this->mPosition.X()+1+n,this->mPosition.Y()-1});
                }
                context.SetBackgroundColor(TB_DEFAULT);

                for(LD::UInteger n = numberOfActiveBars;n<10;++n)
                {
                    context.Render('_',{this->mPosition.X()+1+n,this->mPosition.Y()-1});
                }

                 */
                //context.RenderWithFormat(LD::ToImmutableString("{}"),{this->mPosition.X()+1,this->mPosition.Y()-1},LD::ImmutableString<2>{'_'});
                return context;
            }
        };
    }
}
#endif //LANDESSDEVCORE_TERMBOXPROGRESSBAR_HPP
