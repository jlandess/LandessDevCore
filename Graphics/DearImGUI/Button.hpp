#ifndef LANDESSDEVCORE_IMGUI_BUTTON
#define LANDESSDEVCORE_IMGUI_BUTTON
#include "Graphics/DearImGUI/imgui.h"
#include "TypeTraits/Detection.hpp"
//#include "Definitions/Common.hpp"
#include "TypeTraits/Conditional.hpp"
#include "Memory/ElementReference.h"
#include "Primitives/General/Context.h"
#include "Primitives/General/Immutable.hpp"
#include "Algorithms/Invoke.hpp"
#include "RenderingContext.hpp"
namespace LD
{
    namespace IMGUI
    {
        template<typename StringType, typename CallBack, typename ArgList,class = void>
        class Button;
        
        
        template<typename StringType, typename CallBack, typename ... Args>
        class Button<StringType,CallBack,LD::CT::TypeList<Args...>,LD::Enable_If_T<
        LD::Require<
        LD::ConvertiblyCallable<LD::Detail::Decay_T<CallBack>,void(LD::Detail::Conditional_T<(LD::Detail::IsLValueReference<Args>::value),LD::ElementReference <LD::Detail::Decay_T<Args>>,LD::Detail::Decay_T<Args>>...)>::Value()
        >>>
        {
        private:
            using BackendType = LD::Detail::Conditional_T<(LD::Detail::IsLValueReference<CallBack>::value),LD::ElementReference <LD::Detail::Decay_T<CallBack>>,LD::Detail::Decay_T<CallBack>>;
            StringType mString;
            BackendType mCurrentCallBack;
            LD::Context<Args...> mContext;
        public:
            
            Button() = default;
            
            Button(StringType && string, CallBack && callback, Args && ... args) noexcept:mString{LD::Forward<StringType>(string)},mCurrentCallBack{LD::Forward<CallBack>(callback)},mContext{LD::Forward<Args>(args)...}
            {
                
            }
            
            
            void operator()(const LD::IMGUI::RenderingContext & context) noexcept
            {
                if(ImGui::Button(mString.Data()))
                {
                    LD::Invoke(this->mCurrentCallBack, this->mContext);
                    //todo invoke with context
                }
            }
        };
        
        template<typename StringType, typename CallBack, typename ... A> Button(StringType &&, CallBack && ,A &&...) -> Button<StringType,CallBack,LD::CT::TypeList<A...>>;
        
    }
}

#endif