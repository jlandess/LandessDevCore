//
// Created by phoenixflower on 7/19/20.
//

#ifndef LANDESSDEVCORE_TERMBOXTEXTLABEL_H
#define LANDESSDEVCORE_TERMBOXTEXTLABEL_H
#include "Primitives/General/Immutable.hpp"
#include "Primitives/General/mapboxvariant.hpp"
#include "Primitives/General/mapboxvariantvisitor.h"
#include "TermBoxRenderingContext.hpp"
#include "ScopedRenderContextState.hpp"
namespace LD
{
    namespace TUI
    {
        class TextLabel
        {
        private:
            LD::Variant<LD::ImmutableString<1024>,char*> mLabel;
        public:
            TextLabel & operator()( LD::TermBoxRenderContext & context, const unsigned short & color = TB_GREEN) noexcept
            {

                return (*this);
            }
        };
        template<LD::UInteger Size>
        class ImmutableTextLabel
        {
        private:
            LD::ImmutableString<Size> mLabel;
            LD::BasicVec2D<LD::Integer> mLocation;
            unsigned short mColor;
        public:

            constexpr ImmutableTextLabel(const LD::BasicVec2D<LD::Integer> & location,const LD::ImmutableString<Size> & label, const unsigned short & color = TB_GREEN) noexcept:mLabel{label},mLocation{location},mColor{color}
            {

            }

            constexpr ImmutableTextLabel(
                    const LD::BasicVec2D<LD::Integer> & postion,
                    const char (&lit)[Size + 1],const unsigned short & color = TB_GREEN) noexcept :mLocation(postion),mLabel(lit),mColor{color}
            {

            }
            LD::TermBoxRenderContext & operator()( LD::TermBoxRenderContext & context) const noexcept
            {
                LD::TUI::ScopedRenderingContextState scopedState{context};
                context.SetForegroundColor(this->mColor);
                context.Render(this->mLabel,{this->mLocation.X(),this->mLocation.Y()});
                return context;
            }
        };


        template <LD::UInteger N> ImmutableTextLabel(LD::BasicVec2D<LD::Integer>,ImmutableString<N>) -> ImmutableTextLabel<N>;
        template <LD::UInteger N> ImmutableTextLabel(LD::BasicVec2D<LD::Integer>,const char (&lit)[N]) -> ImmutableTextLabel<N-1>;
    }

}
#endif //LANDESSDEVCORE_TERMBOXTEXTLABEL_H
