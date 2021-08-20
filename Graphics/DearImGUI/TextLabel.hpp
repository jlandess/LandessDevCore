#ifndef LANDESSDEVCORE_IMGUI_TEXTLABEL
#define LANDESSDEVCORE_IMGUI_TEXTLABEL
#include "Graphics/DearImGUI/imgui.h"
#include "TypeTraits/Detection.hpp"
#include "Primitives/General/Immutable.hpp"
#include "RenderingContext.hpp"
#include "Algorithms/AddAll.hpp"
namespace LD
{
    namespace IMGUI
    {
        /*
        namespace Detail
        {
            template<typename T>
            auto AddAll(const T& arg)
            {
                return LD::ToImmutableString(LD::Get(arg));
            }

            template<typename T, typename... ARGS>
            auto AddAll(const T& arg, const ARGS&... args)
            {
                return LD::ToImmutableString(LD::Get(arg)) + LD::IMGUI::Detail::AddAll(args...);
            }
        }
         */
        template<typename StringType, class = void>
        class TextLabel;
        
        
        template<typename StringType>
        class TextLabel<StringType,
        LD::Enable_If_T<
        LD::Require<
        LD::CT::IsImmutableString(LD::CT::RemoveQualifiers(LD::Type<StringType>{}))
        >>>
        {
        private:
            StringType mString;
        public:
            TextLabel(StringType && string) noexcept:mString{LD::Forward<StringType>(string)}
            {
                
            }
            
            void operator()(const LD::IMGUI::RenderingContext & context) const noexcept
            {
                ImGui::Text(this->mString.Data());
            }
        };

        template<typename ... A>
        class TextLabel<LD::CT::TypeList<A...>,
                LD::Enable_If_T<
                        LD::Require<
                                (LD::CT::CanBeAnImmutableString(LD::CT::RemoveQualifiers(LD::Type<A>{})) && ...)
                        >>>
        {
        private:
            LD::Context<A...> mContext;
        public:
            TextLabel(A  && ... args) noexcept:mContext{LD::MakeContext(LD::Forward<A>(args)...)}
            {

            }

            void operator()(const LD::IMGUI::RenderingContext & context) const noexcept
            {
                auto abc = [](auto  && ... components) noexcept
                {
                    return LD::AddAll(LD::Forward<decltype(components)>(components)...);
                };
                auto result = LD::Invoke(abc,this->mContext);
                ImGui::Text(result.Data());
            }
        };
        
        template<typename StringType> TextLabel(StringType &&) -> TextLabel<StringType>;

        template<typename ... A> TextLabel(A &&... ) -> TextLabel<LD::CT::TypeList<A...>>;
    }
}
#endif