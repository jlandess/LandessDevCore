#ifndef LANDESSDEVCORE_GRAPHICS_IMGUI_TREE_NODE_HPP
#define LANDESSDEVCORE_GRAPHICS_IMGUI_TREE_NODE_HPP
#include "Graphics/DearImGUI/imgui.h"
#include "TypeTraits/Detection.hpp"
//#include "Definitions/Common.hpp"
#include "Memory/ElementReference.h"
#include "Graphics/DearImGUI/imgui.h"
#include "Primitives/General/Context.h"
#include "TypeTraits/Iterable.h"
namespace LD
{
    namespace IMGUI
    {
        template<typename StringType,typename T, class = void>
        class TreeNode;
        
        template<typename StringType,typename T>
        class TreeNode<StringType,T,LD::Enable_If_T<
        LD::Require<
        LD::Either<LD::Require<LD::Exists<LD::Detail::CamelCaseBegin,T>,LD::Exists<LD::Detail::CameCaseEnd,T>>,LD::Require<LD::Exists<LD::Detail::ReverseCamelCaseBegin,T>,LD::Exists<LD::Detail::ReverseCamelCaseEnd,T>>>
        >>>
        {
        private:
            using BackendType = LD::Detail::Conditional_T<(LD::Detail::IsLValueReference<T>::value),LD::ElementReference <LD::Detail::Decay_T<T>>,LD::Detail::Decay_T<T>>;
            StringType mName;
            BackendType mBackend;
        public:
            TreeNode() = default;
            
            TreeNode(StringType && stringType,T && iterable) noexcept:mName{LD::Forward<StringType>(stringType)}, mBackend{LD::Forward<T>(iterable)}
            {
            }
            
            void operator()(const LD::IMGUI::RenderingContext & context) const noexcept
            {
                if (ImGui::TreeNode(this->mName.Data()))
                {
                    if constexpr (LD::CT::IsElementReference(LD::Type<BackendType>{}))
                    {
                        for(auto it = LD::Begin(*this->mBackend);it!=LD::End(*this->mBackend);++it)
                        {
                            const auto & object = (*it);
                            using UsableType = LD::Detail::Decay_T<decltype(object)>;
                            if constexpr (LD::CT::CanBeAnImmutableString(LD::CT::RemoveQualifiers(LD::Type<UsableType>{})))
                            {
                                auto string = LD::ToImmutableString((object));
                                ImGui::Text(string.Data());

                            }else if constexpr (LD::ConvertiblyCallable<UsableType,void(const LD::IMGUI::RenderingContext&)>::Value())
                            {
                                object(context);
                            }
                        }

                    }else
                    {
                        for(auto it = LD::Begin(this->mBackend);it!=LD::End(this->mBackend);++it)
                        {
                            const auto & object = (*it);
                            using UsableType = decltype(object);
                            if constexpr (LD::CT::CanBeAnImmutableString(LD::CT::RemoveQualifiers(LD::Type<UsableType>{})))
                            {
                                auto string = LD::ToImmutableString((object));
                                ImGui::Text(string.Data());

                            }else if constexpr (LD::ConvertiblyCallable<UsableType,void(const LD::IMGUI::RenderingContext&)>::Value())
                            {
                                object(context);
                            }
                        }
                    }
                    
                    ImGui::TreePop();
                }
            }
        };

        template<typename StringType,typename T> TreeNode(StringType &&, T &&) -> TreeNode<StringType,T>;
        
    }
}

#endif