//
// Created by phoenixflower on 10/26/20.
//

#ifndef LANDESSDEVCORE_BUTTON_HPP
#define LANDESSDEVCORE_BUTTON_HPP
#include "TypeTraits/Detection.hpp"
#include "Reflection//Reflection.hpp"
namespace LD
{
    namespace SPA
    {
        template<typename Compositor,typename NameStringType>
        class Button
        {
        private:
            NameStringType mName;
            LD::UInteger mID;
            LD::ElementReference<Compositor> mCompositor;
        public:
            Button(Compositor & compositor,NameStringType && name) noexcept :mName{name},mID{compositor.GetNextValidComponentID()},mCompositor{compositor}
            {

            }

            LD::UInteger ID() noexcept
            {
                return this->mID;
            }


            template<typename F, typename ... B>
            LD::Enable_If_T<LD::Require<LD::ConvertiblyCallable<F,void(Compositor &,B && ...)>::Value()>,void> OnClick(F && function, B && ... arguments) noexcept
            {
            }
        };

        template <typename Compositor,typename String> Button(Compositor &,String &&) -> Button<Compositor,String>;
    }
}
#endif //LANDESSDEVCORE_BUTTON_HPP
