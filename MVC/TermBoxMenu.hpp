//
// Created by James Landess on 1/20/20.
//

#ifndef LANDESSDEVCORE_TERMBOXMENU_HPP
#define LANDESSDEVCORE_TERMBOXMENU_HPP

//#include "Definitions/Common.hpp"
#include "Primitives/General/Immutable.hpp"
#include "Primitives/General/StaticArray.hpp"
#include "MVC/TermBoxRenderingContext.hpp"
#include "Functor/LightWeightDelegate.h"
#include "Memory/ElementReference.h"
#include "TypeTraits/Decay.hpp"
namespace LD
{
    namespace TB
    {
        template< typename Context ,typename  ImmutableStrings, typename Predicate ,class sfinae = void>
        class ImmutableMenu;


        template<typename ... Context ,typename ... ImmutableStrings,typename Predicate>
        class ImmutableMenu<LD::VariadicPack<Context...>,LD::VariadicPack<ImmutableStrings...>,Predicate,LD::Enable_If_T<
                LD::Require<
                        (LD::IsImmutableString<LD::Detail::Decay_T<ImmutableStrings>> && ...),
                        LD::IsInTuple<LD::TermBoxRenderContext,LD::VariadicPack<typename LD::Decay<Context>::type...>>::value,
                        (sizeof...(ImmutableStrings) > 0),
                        ((LD::ConvertiblyCallable<Predicate,bool(const LD::ApplicationExecutionEvent<Context...> & )>::Value()))
                >>>
        {
        private:
            LD::StaticArray<PDP::LightWeightDelegate<void(const LD::UInteger  &,const LD::ApplicationExecutionEvent<Context...> &)>,(sizeof...(ImmutableStrings)+1)> mEventHandlers;
            LD::Tuple<ImmutableStrings...> mMenuItems;
            LD::Detail::tVec2<LD::Integer> mLocation;
            LD::ElementReference<Predicate> mPredicate;

            void InternalEmptyHandler(const LD::UInteger &,const LD::ApplicationExecutionEvent<Context...> &){}
        public:
            typedef LD::StaticArray<PDP::LightWeightDelegate<void(const LD::UInteger  &,const LD::ApplicationExecutionEvent<Context...> &)>,(sizeof...(ImmutableStrings)+1)> EventHanlderType;
            typedef PDP::LightWeightDelegate<void(const LD::UInteger  &,const LD::ApplicationExecutionEvent<Context...> &)> CallBackType;
            inline explicit constexpr ImmutableMenu() noexcept :mLocation{0,0}
            {
                mEventHandlers[sizeof...(ImmutableStrings)] = PDP::LightWeightDelegate<void(const LD::UInteger &,const LD::ApplicationExecutionEvent<Context...> &)>{this,&ImmutableMenu::InternalEmptyHandler};
            }


            /*
            inline explicit constexpr ImmutableMenu(ImmutableStrings && ... immutableString) noexcept :mMenuItems{LD::Forward<ImmutableStrings>(immutableString)...},mLocation{0,0}
            {
                mEventHandlers[sizeof...(ImmutableStrings)] = PDP::LightWeightDelegate<void(const LD::UInteger &,const LD::ApplicationExecutionEvent<Context...> &)>{this,&ImmutableMenu::InternalEmptyHandler};
            }

            inline explicit constexpr ImmutableMenu(ImmutableStrings && ... immutableString,const PDP::Detail::tVec2<LD::Integer> & origin) noexcept :mMenuItems{LD::Forward<ImmutableStrings>(immutableString)...},mLocation{origin}
            {
                mEventHandlers[sizeof...(ImmutableStrings)] = PDP::LightWeightDelegate<void(const LD::UInteger &,const LD::ApplicationExecutionEvent<Context...> &)>{this,&ImmutableMenu::InternalEmptyHandler};
            }
             */


            template<typename ... Functors,typename =  LD::Enable_If_T<
                    LD::Require<
                            ((LD::Detail::IsLValueReference<Functors>::value || LD::Detail::IsPointer<Functors>::value) && ...),
                            (LD::ConvertiblyCallable<Functors,void(const LD::UInteger  &,const LD::ApplicationExecutionEvent<Context...> & )>::Value() && ...),
                            (sizeof...(Functors) == sizeof...(ImmutableStrings))
                            >>>
            inline explicit constexpr ImmutableMenu(ImmutableStrings && ... immutableString,const LD::Detail::tVec2<LD::Integer> & origin, LD::Tuple<Functors...> && functors,Predicate & predicate) noexcept :mMenuItems{LD::Forward<ImmutableStrings>(immutableString)...},mLocation{origin},mPredicate(predicate)
            {
                LD::For<sizeof...(ImmutableStrings)>([](auto Index,const LD::Tuple<Functors...> & functors,EventHanlderType & eventHandler)
                {
                    using CurrentType = LD::Detail::Decay_T<decltype(LD::Get<Index>(functors))>;
                    auto & member = LD::Get<Index>(functors);
                    CurrentType * ptr = (CurrentType*)&member;
                    eventHandler[Index] = PDP::LightWeightDelegate<void(const LD::UInteger  &,const LD::ApplicationExecutionEvent<Context...> &)>{ptr,&CurrentType::operator()};
                    return true;
                },functors,mEventHandlers);

                mEventHandlers[sizeof...(ImmutableStrings)] = PDP::LightWeightDelegate<void(const LD::UInteger &,const LD::ApplicationExecutionEvent<Context...> &)>{this,&ImmutableMenu::InternalEmptyHandler};

            }


            const bool operator()(const LD::ApplicationStartedEvent<Context...> & applicationStartedEvent)
            {
                return true;
            }

            const bool operator()(const LD::ApplicationFrameStartedEvent<Context...> & applicationFrameStartedEvent) noexcept
            {
                return true;
            }

            template<typename ... Handlers>
            LD::Enable_If_T<
                    LD::Require<
                            sizeof...(Handlers) == sizeof...(ImmutableStrings),
                            (sizeof...(Handlers) > 1),
                            (LD::ConvertiblyCallable<Handlers,void(const LD::UInteger  &,const LD::ApplicationExecutionEvent<Context...> & )>::Value() && ...)

                    >
            ,void> SetHandlers(Handlers & ... handlers)
            {

                auto tiedTuple = LD::Tie(handlers...);
                LD::For<sizeof...(ImmutableStrings)>([](auto Index,
                        auto & tuple,
                        LD::StaticArray<PDP::LightWeightDelegate<void(const LD::UInteger  &,const LD::ApplicationExecutionEvent<Context...> &)>,(sizeof...(ImmutableStrings)+1)> & handlers)
                {
                    using CurrentType = LD::Detail::Decay_T<decltype(LD::Get<Index>(tuple))>;
                    CurrentType * ptr = (CurrentType*)&LD::Get<Index>(tuple);
                    //handlers[Index] = PDP::LightWeightDelegate<void(const LD::UInteger &,const LD::ApplicationExecutionEvent<Context...> &)>{ptr,&CurrentType ::operator()};
                    return true;
                },tiedTuple,this->mEventHandlers);
            }

            template<typename Handler>
            LD::Enable_If_T<LD::Require<
                    LD::ConvertiblyCallable<Handler,void(const LD::UInteger  &,const LD::ApplicationExecutionEvent<Context...> & )>::Value()>
            ,void> SetHandlers(Handler & handler)
            {
                LD::For<sizeof...(ImmutableStrings)>([](auto Index,
                        Handler & handler,
                        LD::StaticArray<PDP::LightWeightDelegate<void(const LD::UInteger  &,const LD::ApplicationExecutionEvent<Context...> &)>,(sizeof...(ImmutableStrings)+1)> & handlers)
                {
                    Handler * ptr = (Handler*)&handler;
                    handlers[Index] = PDP::LightWeightDelegate<void(const LD::UInteger &,const LD::ApplicationExecutionEvent<Context...> &)>{ptr,&Handler::operator()};
                    return true;
                },handler,this->mEventHandlers);
            }

            PDP::Second<LD::Float> operator()(const LD::ApplicationPeriodEvent<Context...> & sleepingEvent) noexcept
            {
                PDP::Second<LD::Float>(1.0);
            }

            constexpr LD::UInteger Height() const noexcept
            {
                return sizeof...(ImmutableStrings);
            }

            constexpr LD::UInteger Width() const noexcept
            {
                LD::UInteger width = LD::Get<0>(mMenuItems).GetSize();

                LD::For<1,sizeof...(ImmutableStrings)>([](auto Index,LD::UInteger & width,const LD::Tuple<ImmutableStrings...> & menuItems)
                {
                    const LD::UInteger currentStringLength = LD::Get<Index>(menuItems).GetSize();
                    width = (currentStringLength > width)*currentStringLength + (currentStringLength <= width)*width;
                    return true;
                },width,this->mMenuItems);

                return width;
            }

            void operator()(const LD::ApplicationExecutionEvent<Context...> & executionEvent) noexcept
            {

                const LD::ElementReference<LD::TermBoxRenderContext> context = LD::Get<LD::TermBoxRenderContext>(executionEvent);
                const LD::Detail::tVec2<LD::Integer> & mouse = context->GetMouse();

                LD::Detail::tVec2<LD::UInteger> boundingBox;


                boundingBox.X() = (mouse.X() >= this->mLocation.X()) && (mouse.X() < (this->mLocation.X() + this->Width()));
                boundingBox.Y() = (mouse.Y() <= this->mLocation.Y()) && (mouse.Y() > (this->mLocation.Y() - this->Height()));


                const LD::UInteger isInBoundingBox = boundingBox.X() && boundingBox.Y();
                LD::Integer distance = LD::Abs(mouse.Y()-mLocation.Y());

                const bool canBeSelected = (*mPredicate)(executionEvent);


                //const LD::UInteger selectedIndex = ((distance < this->Height() && isInBoundingBox)*distance) + (!(distance < this->Height() && isInBoundingBox))*this->Height();
                const LD::UInteger selectedIndex = (( isInBoundingBox && canBeSelected)*distance) + (!(isInBoundingBox) || !canBeSelected)*this->Height();


                //LD::Get<LD::TermBoxRenderContext>(executionEvent)->Render(sizeof...(ImmutableStrings)+1,{9,5});
                //LD::Get<LD::TermBoxRenderContext>(executionEvent)->Render(selectedIndex,{9,8});


                mEventHandlers[selectedIndex](selectedIndex,executionEvent);

                LD::For<sizeof...(ImmutableStrings)>([&](auto Index)
                {
                    LD::Get<LD::TermBoxRenderContext>(executionEvent)->Render(LD::Get<Index>(mMenuItems),{mLocation.X(),mLocation.Y()-Index});
                    return true;
                });
            }

            void operator()(const LD::ApplicationFrameEndedEvent<Context...> & frameEndedEvent) noexcept
            {

            }

            void operator()(const LD::ApplicationQuittingEvent<Context...> & quittingEvent) noexcept
            {

            }

        };



        template<typename ... Context,typename ... ImmutableStrings, typename Predicate>
        auto MakeMenu(Predicate &&,ImmutableStrings  && ... titles) noexcept -> ImmutableMenu<LD::VariadicPack<Context...> ,LD::VariadicPack<LD::Detail::Decay_T<ImmutableStrings>...>,Predicate>
        {

            return ImmutableMenu<LD::VariadicPack<Context...> ,LD::VariadicPack<LD::Detail::Decay_T<ImmutableStrings>...>,Predicate>{LD::Forward<LD::Detail::Decay_T<ImmutableStrings>>(titles)...,{0,0}};
        }


        template<typename ... Context,typename ... ImmutableStrings,typename Predicate>
        auto MakeImmutableMenu(Predicate &&,const LD::Detail::tVec2<LD::Integer> & origin,ImmutableStrings  && ... titles) noexcept -> ImmutableMenu<LD::VariadicPack<Context...> ,LD::VariadicPack<LD::Detail::Decay_T<ImmutableStrings>...>,Predicate>
        {

            return ImmutableMenu<LD::VariadicPack<Context...> ,LD::VariadicPack<LD::Detail::Decay_T<ImmutableStrings>...>,Predicate>{LD::Forward<LD::Detail::Decay_T<ImmutableStrings>>(titles)...,origin};
        }

        template<typename ... Context,typename ... ImmutableStrings, typename ... Functors, typename Predicate>
        LD::Enable_If_T<
        LD::Require<
        ((LD::Detail::IsLValueReference<Functors>::value || LD::Detail::IsPointer<Functors>::value) && ...),
        sizeof...(Functors) == sizeof...(ImmutableStrings)
        >
        , ImmutableMenu<LD::VariadicPack<Context...> ,LD::VariadicPack<LD::Detail::Decay_T<ImmutableStrings>...>,Predicate>> MakeImmutableMenu(Predicate & predicate,LD::Tuple<Functors...> && tuple,const LD::Detail::tVec2<LD::Integer> & origin,ImmutableStrings  && ... titles) noexcept
        {

            return ImmutableMenu<LD::VariadicPack<Context...> ,LD::VariadicPack<LD::Detail::Decay_T<ImmutableStrings>...>,Predicate>{LD::Forward<LD::Detail::Decay_T<ImmutableStrings>>(titles)...,origin,LD::Forward<LD::Tuple<Functors...>>(tuple),predicate};
        }

        template<typename ... Context,typename ... ImmutableStrings, typename ... Functors, typename Predicate>
        LD::Enable_If_T<
                LD::Require<
                        ((LD::Detail::IsLValueReference<Functors>::value || LD::Detail::IsPointer<Functors>::value) && ...),
                        sizeof...(Functors) == sizeof...(ImmutableStrings)
                >
                , ImmutableMenu<LD::VariadicPack<Context...> ,LD::VariadicPack<LD::Detail::Decay_T<ImmutableStrings>...>,Predicate>> MakeImmutableMenu(const LD::TermBoxApplication<Context...> &,Predicate & predicate,LD::Tuple<Functors...> && tuple,const LD::Detail::tVec2<LD::Integer> & origin,ImmutableStrings  && ... titles) noexcept
        {

            return ImmutableMenu<LD::VariadicPack<Context...> ,LD::VariadicPack<LD::Detail::Decay_T<ImmutableStrings>...>,Predicate>{LD::Forward<LD::Detail::Decay_T<ImmutableStrings>>(titles)...,origin,LD::Forward<LD::Tuple<Functors...>>(tuple),predicate};
        }
    }
}
#endif //LANDESSDEVCORE_TERMBOXMENU_HPP
