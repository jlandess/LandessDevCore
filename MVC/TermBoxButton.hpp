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
            LD::UInteger mChoiceKey;
        public:
            constexpr ImmutableButton(
                    const LD::BasicVec2D<LD::Integer> & location,
                    const LD::ImmutableString<Size> & str,
                    T && object,
                    Context && ... arguments) noexcept :mLabel{str},mPosition{location},mObject{LD::Forward<T>(object)},mContext{LD::Forward<Context>(arguments)...},mChoiceKey{' '}
            {

            }

            constexpr ImmutableButton(
                    const LD::BasicVec2D<LD::Integer> & location,
                    const LD::ImmutableString<Size> & str,
                    T && object,
                    const  LD::Tuple<Context...> arguments) noexcept :mLabel{str},mPosition{location},mObject{LD::Forward<T>(object)},mContext{arguments},mChoiceKey{' '}
            {

            }

            constexpr ImmutableButton(
                    const LD::BasicVec2D<LD::Integer> & postion,
                    const char (&lit)[Size + 1],
                    T & object,
                    Context && ... arguments,
                    const unsigned short & color = TB_GREEN) noexcept :mPosition{postion},mLabel(lit),mColor{color},mObject{object},mContext{LD::Forward<Context>(arguments)...},mChoiceKey{' '}
            {

            }

            LD::TermBoxRenderContext  & operator()( LD::TermBoxRenderContext & context) noexcept
            {
                LD::ImmutableString<Size> bar = {'_'};
                context.RenderWithFormat(LD::ToImmutableString("{}"),{this->mPosition.X()+1,this->mPosition.Y()+1},bar);
                context.Render('|',this->mPosition);
                context.Render(this->mLabel,this->mPosition+LD::BasicVec2D<LD::Integer>{1,0});
                context.Render('|',this->mPosition+LD::BasicVec2D<LD::Integer>{Size+1,0});

                if (context.GetCursor().Y() == this->mPosition.Y()
                    && (context.GetCursor().X() > this->mPosition.X() && context.GetCursor().X() < this->mPosition.X()+Size+1))
                {
                    auto event = context.Event(this->mChoiceKey,this->mObject,this->mContext);

                    auto onKeyBoardEvent = [](const LD::Context<LD::TermBoxKeyboardEvent,LD::UInteger&,Executor&,LD::Context<Context...>&> & event) noexcept
                    {
                        auto keyedEvent = LD::Get<0>(event)(
                                LD::ElementReference<LD::UInteger>{LD::Get<1>(event)},
                                LD::ElementReference<Executor>{LD::Get<2>(event)},
                                LD::ElementReference<LD::Context<Context...>>{LD::Get<3>(event)});

                        using CharacterContext = LD::Context<uint32_t,LD::ElementReference<LD::UInteger>,LD::ElementReference<Executor>,LD::ElementReference<LD::Context<Context...>>>;
                        using KeyContext = LD::Context<uint16_t,LD::ElementReference<LD::UInteger>,LD::ElementReference<Executor>,LD::ElementReference<LD::Context<Context...>>>;

                        auto onCharacter = [](const CharacterContext & context) noexcept
                        {
                            if(LD::Get(LD::Get<0>(context)) == LD::Get(LD::Get<1>(context)))
                            {
                                if constexpr(sizeof...(Context) > 0)
                                {
                                    LD::Get(LD::Get(LD::Get<2>(context)))(LD::Get(LD::Get<3>(context)));
                                }else
                                {

                                    LD::Get(LD::Get(LD::Get<2>(context)))();
                                }
                            }
                        };

                        auto onKey = [](const KeyContext & context) noexcept
                        {

                            if(LD::Get(LD::Get<0>(context)) == LD::Get(LD::Get<1>(context)))
                            {
                                if constexpr(sizeof...(Context) > 0)
                                {
                                    LD::Get(LD::Get(LD::Get<2>(context)))(LD::Get(LD::Get<3>(context)));
                                }else
                                {

                                    LD::Get(LD::Get(LD::Get<2>(context)))();
                                }
                            }
                        };

                        LD::Match(keyedEvent,onCharacter,onKey);

                    };

                    auto onMouse = [](const LD::Context<LD::TermBoxMouseEvent,LD::UInteger&,Executor&,LD::Context<Context...>&> & event) noexcept
                    {

                    };

                    auto onDefault = [](const LD::Context<LD::TermBoxEmptyEvent,LD::UInteger&,Executor&,LD::Context<Context...>&> & event) noexcept
                    {

                    };

                    auto onResize = [](const LD::Context<LD::TermBoxResizingEvent,LD::UInteger&,Executor&,LD::Context<Context...>&> & event) noexcept
                    {

                    };

                    LD::Match(event,onKeyBoardEvent,onMouse,onDefault,onResize);

                    //if constexpr(sizeof...(Context) > 0)
                    //{
                     //   LD::Get(this->mObject)(this->mContext);
                    //}else
                    //{
                      //  LD::Get(this->mObject)();
                    //}
                    //(LD::Get(this->mObject).*CallBack)(this->mContext);

                }
                return context;
            }
        };


        template<LD::UInteger Size, typename T ,typename ... Context>
        auto MakeButton(const LD::ImmutableString<Size> & text,T && executor ,const LD::BasicVec2D<LD::Integer> & location,const LD::Tuple<Context...> & context) noexcept
        {
            return ImmutableButton<Size,T(Context...)>{location,text,LD::Forward<T>(executor),context};
        }

        template<LD::UInteger Size, typename T>
        auto MakeButton(const LD::ImmutableString<Size> & text,T && executor ,const LD::BasicVec2D<LD::Integer> & location) noexcept
        {
            return ImmutableButton<Size,T()>{location,text,LD::Forward<T>(executor)};
        }

        //template <LD::UInteger N> ImmutableButton(LD::BasicVec2D<LD::Integer >,ImmutableString<N>) -> ImmutableButton<N>;
        //template <LD::UInteger N> ImmutableButton(LD::BasicVec2D<LD::Integer >,const char (&lit)[N]) -> ImmutableButton<N-1>;

        template <LD::UInteger N, typename T, typename ... A> ImmutableButton(LD::BasicVec2D<LD::Integer>,ImmutableString<N>, T &&, A && ...) -> ImmutableButton<N,T(A...)>;

        template <LD::UInteger N, typename T, typename ... A> ImmutableButton(LD::BasicVec2D<LD::Integer>,const char (&lit)[N], T &&, A && ...) -> ImmutableButton<N-1,T(A...)>;
    }
}
#endif //LANDESSDEVCORE_TERMBOXBUTTON_HPP
