//
// Created by phoenixflower on 1/20/21.
//

#ifndef LANDESSDEVCORE_RENDERINGCONTEXT_HPP
#define LANDESSDEVCORE_RENDERINGCONTEXT_HPP
#include "Definitions/Integer.hpp"
namespace LD
{
    namespace IMGUI
    {
        class RenderingContext
        {
        private:
            LD::UInteger mWidth;
            LD::UInteger mHeight;
        public:
            RenderingContext(LD::UInteger width, LD::UInteger height) noexcept:mWidth{width},mHeight{height}
            {

            }

            LD::UInteger Width() const noexcept
            {
                return this->mWidth;
            }

            LD::UInteger Height() const noexcept
            {
                return this->mHeight;
            }
        };
    }
}
#endif //LANDESSDEVCORE_RENDERINGCONTEXT_HPP
