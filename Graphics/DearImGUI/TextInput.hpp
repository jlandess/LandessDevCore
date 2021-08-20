#ifndef LANDESSDEVCORE_IMGUI_TEXTINPUT
#define LANDESSDEVCORE_IMGUI_TEXTINPUT
#include "Graphics/DearImGUI/imgui.h"
#include "TypeTraits/Detection.hpp"
#include "Primitives/General/Immutable.hpp"
#include "RenderingContext.hpp"
namespace LD
{
    namespace IMGUI
    {
        
        template<typename LabelType,typename StringType, class = void>
        class TextInput;
        template<typename LabelType,typename StringType>
        class TextInput<LabelType,StringType,
        LD::Enable_If_T<
        LD::Require<
        true
        >>>
        {
        private:
            LabelType mLabel;
            StringType mBuffer;
        public:
            TextInput(LabelType && label,StringType && string) noexcept:mLabel{LD::Forward<LabelType>(label)},mBuffer{LD::Forward<StringType>(string)}
            {
                
            }
            
            
            const StringType & Text() const noexcept
            {
                return this->mBuffer;
            }
            
            void operator()(const LD::IMGUI::RenderingContext & context) noexcept
            {
                //LD::ImmutableSize(LD::Type<StringType>{});
                ImGui::InputText((char*)this->mLabel.Data(),(char*)this->mBuffer.Data(),LD::ImmutableSize(LD::Type<StringType>{}));
            }
        };
        
        template<typename LabelType,typename StringType> TextInput(LabelType &&, StringType &&) -> TextInput<LabelType,StringType>;
    }
}
#endif