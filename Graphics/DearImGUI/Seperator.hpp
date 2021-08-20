//
// Created by phoenixflower on 1/20/21.
//

#ifndef LANDESSDEVCORE_SEPERATOR_HPP
#define LANDESSDEVCORE_SEPERATOR_HPP
#include "Graphics/DearImGUI/imgui.h"
#include "RenderingContext.hpp"
namespace LD
{
    namespace IMGUI
    {
        class Seperator
        {
        private:

        public:

            void operator()(const LD::IMGUI::RenderingContext & context) const noexcept
            {
                ImGui::Separator();
            }

        };
    }
}
#endif //LANDESSDEVCORE_SEPERATOR_HPP
