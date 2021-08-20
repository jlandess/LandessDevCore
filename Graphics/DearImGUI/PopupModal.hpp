//
// Created by phoenixflower on 1/20/21.
//

#ifndef LANDESSDEVCORE_POPUPMODAL_HPP
#define LANDESSDEVCORE_POPUPMODAL_HPP
#include "Graphics/DearImGUI/imgui.h"
#include "TypeTraits/Detection.hpp"
#include "RenderingContext.hpp"
//#include "Definitions/Common.hpp"
#include "Primitives/General/Context.h"
namespace LD
{
    namespace IMGUI
    {
        template<typename StringType,typename T, class = void>
        class PopupModal;


        template<typename StringType,typename ... A>
        class PopupModal<StringType,LD::CT::TypeList<A...>,LD::Enable_If_T<
                LD::Require<
                true
                >>>
        {
        private:
            StringType mLabel;
            LD::Context<A...> mContext;
            bool mStatus;
        public:
            PopupModal(StringType && label, A && ... objects) noexcept: mLabel{LD::Forward<StringType>(label)},mContext{LD::MakeContext(LD::Forward<A>(objects)...)},mStatus{true}
            {

            }

            void Open()
            {
                ImGui::OpenPopup(this->mLabel.Data());
            }

            void Close()
            {
                ImGui::CloseCurrentPopup();
            }

            const bool Status() const noexcept
            {
                return this->mStatus;
            }

            void operator()(const LD::IMGUI::RenderingContext & context)  noexcept
            {
                if (ImGui::BeginPopupModal(this->mLabel.Data(), &this->mStatus))
                {
                    if constexpr(sizeof...(A) > 0)
                    {
                        LD::For<sizeof...(A)>([](auto I, LD::Context<A...> & context, const LD::IMGUI::RenderingContext & renderingContext)
                        {
                            LD::Get(LD::Get<I>(context))(renderingContext);
                            return true;
                        },this->mContext,context);
                    }

                    ImGui::EndPopup();
                }
            }
        };

        template<typename StringType,typename ... A> PopupModal(StringType && ,A &&...) -> PopupModal<StringType,LD::CT::TypeList<A...>>;
    }
}
#endif //LANDESSDEVCORE_POPUPMODAL_HPP
