//
// Created by phoenixflower on 1/20/21.
//

#ifndef LANDESSDEVCORE_SAMELINE_HPP
#define LANDESSDEVCORE_SAMELINE_HPP
#include "Graphics/DearImGUI/imgui.h"
#include "RenderingContext.hpp"
#include "Graphics/DearImGUI/imgui.h"
namespace LD
{
    namespace IMGUI
    {
        class SameLine
        {
        private:

        public:

            void operator()(const LD::IMGUI::RenderingContext & context) const noexcept
            {
                ImGui::SameLine();
            }

        };
    }
}
#endif //LANDESSDEVCORE_SAMELINE_HPP
