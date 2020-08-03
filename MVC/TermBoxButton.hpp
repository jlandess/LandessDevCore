//
// Created by phoenixflower on 7/20/20.
//

#ifndef LANDESSDEVCORE_TERMBOXBUTTON_HPP
#define LANDESSDEVCORE_TERMBOXBUTTON_HPP
#include "Primitives/General/Immutable.hpp"
#include "Primitives/General/Vec2.hpp"
#include "TermBoxRenderingContext.hpp"
#include "ScopedRenderContextState.hpp"
namespace LD
{
    namespace TUI
    {
        template<LD::UInteger Size, typename T>
        class ImmutableButton;
        template<LD::UInteger Size,typename T ,typename ... Context>
        class ImmutableButton<Size,T(Context...)>
        {
        private:
            using Executor = LD::Detail::Conditional_T<(LD::Detail::IsLValueReference<T>::value),LD::ElementReference <LD::Detail::Decay_T<T>>,LD::Detail::Decay_T<T>>;
            LD::ImmutableString<Size> mLabel;
            LD::BasicVec2D<LD::Integer> mPosition;
            Executor mObject;
            unsigned short mColor;
            LD::Context<Context...> mContext;
        public:
            constexpr ImmutableButton(
                    const LD::BasicVec2D<LD::Integer> & location,
                    const LD::ImmutableString<Size> & str,
                    T & object,
                    Context && ... arguments) noexcept :mLabel{str},mPosition{location},mObject{object},mContext{LD::Forward<Context>(arguments)...}
            {

            }

            constexpr ImmutableButton(
                    const LD::BasicVec2D<LD::Integer> & location,
                    const LD::ImmutableString<Size> & str,
                    T & object,
                    const  LD::Tuple<Context...> arguments) noexcept :mLabel{str},mPosition{location},mObject{object},mContext{arguments}
            {

            }

            constexpr ImmutableButton(
                    const LD::BasicVec2D<LD::Integer> & postion,
                    const char (&lit)[Size + 1],
                    T & object,
                    Context && ... arguments,
                    const unsigned short & color = TB_GREEN) noexcept :mPosition{postion},mLabel(lit),mColor{color},mObject{object},mContext{LD::Forward<Context>(arguments)...}
            {

            }

            ImmutableButton & operator()( LD::TermBoxRenderContext & context) noexcept
            {
                LD::ImmutableString<Size> bar = {'_'};
                context.RenderWithFormat(LD::ToImmutableString("{}"),{this->mPosition.X()+1,this->mPosition.Y()+1},bar);
                context.Render('|',this->mPosition);
                context.Render(this->mLabel,this->mPosition+LD::BasicVec2D<LD::Integer>{1,0});
                context.Render('|',this->mPosition+LD::BasicVec2D<LD::Integer>{Size,0});

                if (context.GetMouse().Y() == this->mPosition.Y()
                    && (context.GetMouse().X() > this->mPosition.X() && context.GetMouse().X() < this->mPosition.X()+Size))
                {
                    auto event = context.Event();

                    //(LD::Get(this->mObject).*CallBack)(this->mContext);
                    LD::Get(this->mObject)(this->mContext);
                }
                return (*this);
            }
        };


        template<LD::UInteger Size, typename T ,typename ... Context>
        auto MakeButton(const LD::ImmutableString<Size> & text,T && executor ,const LD::BasicVec2D<LD::Integer> & location,const LD::Tuple<Context...> & context) noexcept
        {
            return ImmutableButton<Size,T(Context...)>{location,text,LD::Forward<T>(executor),context};
        }

        //template <LD::UInteger N> ImmutableButton(LD::BasicVec2D<LD::Integer >,ImmutableString<N>) -> ImmutableButton<N>;
        //template <LD::UInteger N> ImmutableButton(LD::BasicVec2D<LD::Integer >,const char (&lit)[N]) -> ImmutableButton<N-1>;
    }
}
#endif //LANDESSDEVCORE_TERMBOXBUTTON_HPP
