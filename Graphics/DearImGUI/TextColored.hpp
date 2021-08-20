#ifndef LANDESSDEVCORE_IMGUI_TEXT_COLORED_HPP
#define LANDESSDEVCORE_IMGUI_TEXT_COLORED_HPP
#include "Graphics/DearImGUI/imgui.h"
#include "Primitives/General/Immutable.hpp"
#include "TypeTraits/Detection.hpp"
//#include "Definitions/Common.hpp"
#include "Primitives/General/Vec4.hpp"
#include "RenderingContext.hpp"
#include "Algorithms/AddAll.hpp"
namespace LD
{
    namespace IMGUI
    {
        template<typename StringType, class = void>
        class TextColored;
        
        
        template<typename StringType>
        class TextColored<StringType,LD::Enable_If_T<
        LD::Require<
        true
        >>>
        {
            
            private:
                StringType mString;
                LD::Detail::tVec4<LD::Float> mColor;
        public:
            
            TextColored(StringType && textColored, LD::Detail::tVec4<LD::Float> color) noexcept: mString{LD::Forward<StringType>(textColored)},mColor{color}
            {
                
            }
            
            LD::Detail::tVec4<LD::Float> Color() const noexcept
            {
                return this->mColor;
            }
            
            StringType Text() const noexcept
            {
                return this->mString;
            }
            
            void operator ()(const LD::IMGUI::RenderingContext & context) const noexcept
            {
                ImGui::TextColored(ImVec4(this->mColor.X(), this->mColor.Y(), this->mColor.Z(), this->mColor.W()), this->mString.Data());
            }
        };




        template<typename ... A>
        class TextColored<LD::CT::TypeList<A...>,
                LD::Enable_If_T<
                        LD::Require<
                                (LD::CT::CanBeAnImmutableString(LD::CT::RemoveQualifiers(LD::Type<A>{})) && ...)
                        >>>
        {
        private:
            LD::Context<A...> mContext;
            LD::Detail::tVec4<LD::Float> mColor;
        public:
            TextColored(LD::Detail::tVec4<LD::Float> color,A  && ... args) noexcept:mColor{color}, mContext{LD::MakeContext(LD::Forward<A>(args)...)}
            {

            }

            void operator()(const LD::IMGUI::RenderingContext & context) const noexcept
            {
                auto abc = [](auto  && ... components) noexcept
                {
                    return LD::AddAll(LD::Forward<decltype(components)>(components)...);
                };
                auto result = LD::Invoke(abc,this->mContext);
                //ImGui::Text(result.Data());
                ImGui::TextColored(ImVec4(this->mColor.X(), this->mColor.Y(), this->mColor.Z(), this->mColor.W()), result.Data());
            }
        };




        template<typename StringType> TextColored(StringType &&, LD::Detail::tVec4<LD::Float>) -> TextColored<StringType>;
    }
}

#endif