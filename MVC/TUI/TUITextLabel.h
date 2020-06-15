//
// Created by phoenixflower on 6/10/20.
//

#ifndef LANDESSDEVCORE_TUITEXTLABEL_H
#define LANDESSDEVCORE_TUITEXTLABEL_H
#include "Primitives/General/Vec2.hpp"
#include "MVC/TermBoxRenderingContext.hpp"

namespace LD
{
    namespace TUI
    {
        template<LD::UInteger Size>
        class ImmutableTextLabel
        {
        private:
            LD::BasicVec2D<LD::UInteger> mPosition;
            LD::ImmutableString<Size> mString;
        public:
            constexpr ImmutableTextLabel() noexcept
            {

            }


            constexpr ImmutableTextLabel(
                    const LD::BasicVec2D<LD::UInteger> & postion,
                    const LD::ImmutableString<Size> & string = {}) noexcept :mPosition(postion),mString(string)
            {

            }

            ImmutableTextLabel & operator = (const LD::ImmutableString<Size> & string) noexcept
            {
                this->mString = string;
                return (*this);
            }

            ImmutableTextLabel & operator = (const LD::BasicVec2D<LD::UInteger> & position) noexcept
            {
                this->mPosition = position;
                return (*this);
            }

            ImmutableTextLabel & operator()(const TermBoxRenderContext & context)
            {

                LD::IF(this->mString.GetSize() > 0,[](auto && context, auto && position, auto && string)
                {
                    context.RenderWithFormat(LD::ToImmutableString("{}"),position,string);

                },context,this->mPosition,this->mString);

                return (*this);
            }
        };
    }
}
#endif //LANDESSDEVCORE_TUITEXTLABEL_H
