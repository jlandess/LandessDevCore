//
// Created by phoenixflower on 6/10/20.
//

#ifndef LANDESSDEVCORE_TUITEXTLABEL_H
#define LANDESSDEVCORE_TUITEXTLABEL_H
#include "Primitives/General/Vec2.hpp"
#include "MVC/TermBoxRenderingContext.hpp"

/*
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

            constexpr ImmutableTextLabel(
                    const LD::BasicVec2D<LD::UInteger> & postion,
                    const char (&lit)[Size + 1]) noexcept :mPosition(postion),mString(lit)
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



                return (*this);
            }
        };

        //template <LD::UInteger N> ImmutableTextLabel(LD::BasicVec2D<LD::UInteger>,ImmutableString<N>) -> ImmutableTextLabel<N>;
        //template <LD::UInteger N> ImmutableTextLabel(LD::BasicVec2D<LD::UInteger>,const char (&lit)[N]) -> ImmutableTextLabel<N-1>;
    }
}
 */
#endif //LANDESSDEVCORE_TUITEXTLABEL_H
