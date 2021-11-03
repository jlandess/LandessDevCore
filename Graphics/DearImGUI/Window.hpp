//
// Created by phoenixflower on 1/20/21.
//

#ifndef LANDESSDEVCORE_WINDOW_HPP
#define LANDESSDEVCORE_WINDOW_HPP
#include "TypeTraits/Detection.hpp"
#include "Primitives/General/Context.h"
#include "Graphics/DearImGUI/imgui.h"
#include "TypeTraits/CompileTimeControlFlow.hpp"
#include "RenderingContext.hpp"
#include "Primitives/General/Vec2.hpp"
namespace LD
{
    namespace IMGUI
    {
        template<typename StringType,typename Type, class = void>
        class Window;

        template<typename StringType,typename ... Objects>
        class Window<StringType,LD::CT::TypeList<Objects...>,LD::Enable_If_T<LD::Require<true>>>
        {
        private:
            LD::Context<Objects...> mComponents;
            StringType mName;
            LD::Float mNextWindowAlpha;
            LD::Detail::tVec2<LD::UInteger> mLocation;
            LD::Detail::tVec2<LD::UInteger> mSize;
        public:

            Window(LD::Detail::tVec2<LD::UInteger> location, LD::Detail::tVec2<LD::UInteger> size,StringType && name,Objects && ... objects) noexcept: mLocation{location}, mSize{size},mName{LD::Forward<StringType>(name)},mComponents{LD::MakeContext(LD::Forward<Objects>(objects)...)},mNextWindowAlpha{0.7}
            {

            }

            const LD::Detail::tVec2<LD::UInteger> & Location() const noexcept
            {
                return this->mLocation;
            }

            LD::Detail::tVec2<LD::UInteger> & Location() noexcept
            {
                return this->mLocation;
            }

            const LD::Detail::tVec2<LD::UInteger> & Size() const noexcept
            {
                return this->mSize;
            }

            LD::Detail::tVec2<LD::UInteger> & Size() noexcept
            {
                return this->mSize;
            }
            void operator()(const LD::IMGUI::RenderingContext & context) const noexcept
            {
                ImGui::SetNextWindowPos(ImVec2(this->mLocation.X(), this->mLocation.Y()), ImGuiCond_Always);
                // here we set the calculated width and also make the height to be
                // be the height of the main window also with some margin
                //LD::UInteger controls_width = context.Width();
                ImGui::SetNextWindowSize(
                        ImVec2(static_cast<float>(this->mSize.X()), static_cast<float>(this->mSize.Y())),
                        ImGuiCond_Always
                );
                ImGui::SetNextWindowBgAlpha(this->mNextWindowAlpha);
                ImGui::Begin(this->mName.Data(), NULL, ImGuiWindowFlags_NoResize);
                LD::For<sizeof...(Objects)>([](
                        auto I,
                        const LD::Context<Objects...> & context,
                        const LD::IMGUI::RenderingContext & renderingContext)
                {
                    LD::Get(LD::Get<I>(context))(renderingContext);
                    return true;
                },this->mComponents,context);

                ImGui::End();
            }
        };


        template<typename StringType,typename ... Components> Window(LD::Detail::tVec2<LD::UInteger> , LD::Detail::tVec2<LD::UInteger> ,StringType && ,Components && ... ) -> Window<StringType,LD::CT::TypeList<Components...>>;
    }
}
#endif //LANDESSDEVCORE_WINDOW_HPP
