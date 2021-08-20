//
// Created by phoenixflower on 1/20/21.
//

#ifndef LANDESSDEVCORE_DUMMY_HPP
#define LANDESSDEVCORE_DUMMY_HPP
#include "Primitives/General/Vec2.hpp"
#include "RenderingContext.hpp"
namespace LD
{
    namespace IMGUI
    {
        class Dummy
        {
        private:
            LD::Detail::tVec2<LD::Float> mLocation;
        public:

            Dummy(LD::Detail::tVec2<LD::Float> location) noexcept:mLocation{location}
            {

            }

            void operator()(const LD::IMGUI::RenderingContext & context) const noexcept
            {
                ImGui::Dummy(ImVec2(this->mLocation.X(), this->mLocation.Y()));
            }
        };
    }
}
#endif //LANDESSDEVCORE_DUMMY_HPP
