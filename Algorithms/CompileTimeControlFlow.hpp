//
//  CompileTimeControlFlow.h
//  DataStructures5
//
//  Created by James Landess on 3/16/19.
//  Copyright © 2019 James Landess. All rights reserved.
//

#ifndef CompileTimeControlFlow_h
#define CompileTimeControlFlow_h

#include <Functor/LightWeightDelegate.h>
#include "Memory/ElementReference.h"
#include "TypeTraits/Declval.hpp"
#include "TypeTraits/Detection.hpp"
#include "Primitives/General/Range.h"
#include "Algorithms/Move.hpp"
namespace LD
{
    namespace Detail
    {
        template<typename F, typename ... Args>
        using IsNoThrowInvocable = decltype(noexcept(LD::Declval<F>()(LD::Declval<Args>()...)));

    }

    template<typename F, typename T>
    struct CovertiblyCallableWithNoThrow;




    template<typename F,typename T, typename ... Args>
    struct CovertiblyCallableWithNoThrow<F,T(Args...)>
    {
        static constexpr bool Value() noexcept
        {
            return (LD::Coverts_To<T,LD::Ops::CallableFunction, F,Args...>) && (LD::Exists<LD::Detail::IsNoThrowInvocable, F,Args...>);
        }


        constexpr  bool operator()() const noexcept { return  LD::Coverts_To<T,LD::Ops::CallableFunction, F,Args...>; }

        constexpr operator  bool () const noexcept
        {
            return (LD::Coverts_To<T,LD::Ops::CallableFunction, F,Args...>) && (LD::Exists<LD::Detail::IsNoThrowInvocable, F,Args...>);
        }
    };




    template<typename F, typename ... Args>
    struct CovertiblyCallableWithNoThrow<F,void(Args...)>
    {
        static constexpr bool Value()
        {
            return (LD::Exists<LD::Ops::CallableFunction, F,Args...>)&& (LD::Exists<LD::Detail::IsNoThrowInvocable, F,Args...>);
        }


        constexpr  bool operator()() const noexcept { return  LD::Exists<LD::Ops::CallableFunction, F,Args...>; }

        constexpr operator  bool () const noexcept
        {
            return LD::Exists<LD::Ops::CallableFunction, F,Args...>;
        }
    };

    template<LD::UInteger Index>
    class CompileTimeIndex
    {
    private:

    public:
        static constexpr const LD::UInteger CurrentIndex = Index;
        constexpr operator LD::UInteger() const noexcept { return Index; }

        constexpr operator const LD::UInteger&&() const && noexcept { return LD::Move(CurrentIndex); }
    };
    class Index
    {
    private:
        LD::UInteger Value;
    public:
        inline constexpr Index() noexcept :Value(0){}
        inline constexpr Index(const LD::UInteger & index) noexcept :Value(index){}


        template<LD::UInteger I>
        inline constexpr Index(const CompileTimeIndex<I> & index) noexcept :Value(I){}

        template<LD::UInteger I>
        constexpr Index & operator = (const CompileTimeIndex<I> & index) noexcept
        {
            this->Value = I;
            return (*this);
        }

        constexpr operator const LD::UInteger &() const noexcept
        {
            return this->Value;
        }

        constexpr operator LD::UInteger &() noexcept
        {
            return this->Value;
        }

    };

    namespace Detail
    {
        template<LD::UInteger CurrentIndex, LD::UInteger Max , LD::UInteger Step ,typename F, typename ... A>
        constexpr const bool CompileTimeSteppedRangedBaseBackward(F && function, A && ... arguements) noexcept (noexcept(LD::Declval<F>()(LD::Declval<LD::CompileTimeIndex<Max - (CurrentIndex*Step) -1>>(),LD::Declval<A>()...)))
        {
            //LD::Declval<LD::CompileTimeIndex<Max - (CurrentIndex*Step) -1>>();
            return function(LD::CompileTimeIndex<Max - (CurrentIndex*Step) -1>{},LD::Forward<A>(arguements)...);
        }
        template<LD::UInteger Max, LD::UInteger Step ,LD::UInteger ... Indices,typename F, typename ... A>
        constexpr void CompileTimeFoldSteppedRangedBackward(LD::IndexSequence<Indices...>,F && function, A && ... arguements) noexcept (noexcept(CompileTimeSteppedRangedBaseBackward<0,Max,Step>(LD::Declval<F>(),LD::Declval<A>()...)))
        {

            (CompileTimeSteppedRangedBaseBackward<Indices,Max,Step>(LD::Forward<F>(function), LD::Forward<A>(arguements)...) && ...);
        }


        template<LD::UInteger Offset,LD::UInteger Step,LD::UInteger  CurrentIndex,typename F, typename ... A>
        constexpr bool CompileTimeForBase(F && function, A && ... arguements) noexcept (noexcept(LD::Declval<F>()(LD::Declval<LD::CompileTimeIndex<(CurrentIndex+Offset)*Step>>(),LD::Declval<A>()...)))
        {
            return function(LD::CompileTimeIndex<(CurrentIndex+Offset)*Step>{},LD::Forward<A>(arguements)...);
        }


        template<LD::UInteger  Offset,LD::UInteger Step,typename F,LD::UInteger ... Indices,typename ... A>
        constexpr void CompileTimeForFoldForward(LD::IndexSequence<Indices...>,F && function, A && ... arguements) noexcept (noexcept(CompileTimeForBase<Offset,Step,0>(LD::Declval<F>(),LD::Declval<A>()...)))
        {

            (CompileTimeForBase<Offset,Step,Indices>(LD::Forward<F>(function), LD::Forward<A>(arguements)...) && ...);
        }

        template<LD::UInteger CurrentIndex, LD::UInteger Min , LD::UInteger Step ,typename F, typename ... A>
        constexpr const bool CompileTimeSteppedRangedBase(F && function, A && ... arguements) noexcept (noexcept(LD::Declval<F>()(LD::Declval<LD::CompileTimeIndex<Min + CurrentIndex*Step>>(),LD::Declval<A>()...)))
        {
            return function(LD::CompileTimeIndex<Min + CurrentIndex*Step>{},LD::Forward<A>(arguements)...);
        }


        template<LD::UInteger Min, LD::UInteger Step ,LD::UInteger ... Indices,typename F, typename ... A>
        constexpr void CompileTimeFoldSteppedRanged(LD::IndexSequence<Indices...>,F && function, A && ... arguements) noexcept (noexcept(CompileTimeSteppedRangedBase<0,Min,Step>(LD::Declval<F>(),LD::Declval<A>()...)))
        {
            (CompileTimeSteppedRangedBase<Indices,Min,Step>(LD::Forward<F>(function), LD::Forward<A>(arguements)...) && ...);
        }

        template <LD::UInteger A, LD::UInteger B, LD::UInteger  Step, class = void>
        struct Loop;
        template <LD::UInteger A, LD::UInteger B, LD::UInteger  Step>
        struct Loop<A,B,Step,LD::Enable_If_T <
                LD::Require<
                        (B > A),
                        Step>=1
                >>>
        {
            template <class F, typename ... Pack>
            constexpr static void run(F &&f, Pack  && ...arguements) noexcept (noexcept(CompileTimeFoldSteppedRanged<A,Step>(LD::Declval<LD::MakeIndexSequence_T<(B-A)/Step>>(),LD::Declval<F>(),LD::Declval<Pack>()...)))
            {
                //LD::Declval<LD::MakeIndexSequence_T<(B-A)/Step>>();
                CompileTimeFoldSteppedRanged<A,Step>(LD::MakeIndexSequence_T<(B-A)/Step>{},LD::Forward<F>(f),LD::Forward<Pack>(arguements)...);
            }

        };

        template <LD::UInteger A, LD::UInteger B, LD::UInteger  Step>
        struct Loop<A,B,Step,LD::Enable_If_T <
                LD::Require<
                        (A > B),
                        Step>=1
                >>>
        {
            template <class F, typename ... Pack>
            constexpr static void run(F &&f, Pack  && ...arguements) noexcept(noexcept(CompileTimeFoldSteppedRanged<A,Step>(LD::Declval<LD::MakeIndexSequence_T<(A-B)/Step>>(),LD::Declval<F>(),LD::Declval<Pack>()...)))
            {

                CompileTimeFoldSteppedRangedBackward<A,Step>(LD::MakeIndexSequence_T<(A-B)/Step>{},LD::Forward<F>(f),LD::Forward<Pack>(arguements)...);
            }

        };
    }



    template<LD::UInteger A, LD::UInteger B,typename F, typename ... Pack>
    constexpr LD::Enable_If_T <
            LD::Require<
                    (A!=B),
                    LD::ConvertiblyCallable<F,bool(LD::CompileTimeIndex<0>,Pack...)>::Value()
            >, void> For(F && f, Pack && ... arguements) noexcept (noexcept(LD::Detail::Loop<A,B,1>::run(LD::Declval<F>(),LD::Declval<Pack>()...)))
    {
        Detail::Loop<A,B,1>::run(LD::Forward<F>(f),LD::Forward<Pack>(arguements)...);
    }

    template<LD::UInteger A, LD::UInteger B,LD::UInteger  Step,typename F, typename ... Pack>
    constexpr LD::Enable_If_T <
            LD::Require<
                    (A!=B),
                    LD::ConvertiblyCallable<F,bool(LD::CompileTimeIndex<0>,Pack...)>::Value()
            >, void> For(F && f, Pack && ... arguements) noexcept (noexcept(Detail::Loop<A,B,Step>::run(LD::Declval<F>(),LD::Declval<Pack>()...)))
    {
        Detail::Loop<A,B,Step>::run(LD::Forward<F>(f),LD::Forward<Pack>(arguements)...);
    }

    template<LD::UInteger A, LD::UInteger B,typename F, typename ... Pack>
    constexpr LD::Enable_If_T <
            LD::Require<
                    (A==B),
                    LD::ConvertiblyCallable<F,bool(LD::CompileTimeIndex<0>,Pack...)>::Value()
            >,void> For(F && f, Pack && ... arguements) noexcept
    {
        //Detail::Loop<A,B,1>::run(LD::Forward<F>(f),LD::Forward<Pack>(arguements)...);
    }

    template<LD::UInteger A, LD::UInteger B,LD::UInteger  Step,typename F, typename ... Pack>
    constexpr LD::Enable_If_T <
            LD::Require<
                    (A==B),
                    LD::ConvertiblyCallable<F,bool(LD::CompileTimeIndex<0>,Pack...)>::Value()
            >,void> For(F && f, Pack && ... arguements) noexcept
    {
        //Detail::Loop<A,B,1>::run(LD::Forward<F>(f),LD::Forward<Pack>(arguements)...);
    }


    template<LD::UInteger B,typename F, typename ... Pack>
    constexpr LD::Enable_If_T <
            LD::Require<
                    (B!=0),
                    LD::ConvertiblyCallable<F,bool(LD::CompileTimeIndex<0>,Pack...)>::Value()
            >,void> For(F && f, Pack && ... arguements) noexcept (noexcept(Detail::Loop<0,B,1>::run(LD::Declval<F>(),LD::Declval<Pack>()...)))
    {
        Detail::Loop<0,B,1>::run(LD::Forward<F>(f),LD::Forward<Pack>(arguements)...);
    }

    template<LD::UInteger B,typename F, typename ... Pack>
    constexpr LD::Enable_If_T <
            LD::Require<
                    (B==0),
                    LD::ConvertiblyCallable<F,bool(LD::CompileTimeIndex<0>,Pack...)>::Value()
            >,void> For(F && f, Pack && ... arguements) noexcept
    {

    }

    template<LD::UInteger Beg, LD::UInteger End, typename F, typename ... Pack>
    constexpr void For(const LD::CT::Range<Beg,End> & range, F && f, Pack && ... arguments) noexcept
    {
        Detail::Loop<Beg,End,1>::run(LD::Forward<F>(f),LD::Forward<Pack>(arguments)...);
    }

    template<typename F, typename ... Args>
    auto IF(const bool & predicate,F && functor, Args && ... arguements) -> void
    {
        PDP::LightWeightDelegate<void(F && functor, Args && ...)> codePaths[2];

        auto emptyCodePath = [](F && functor, Args && ... arguements){};

        auto usableCodePath = [](F && functor, Args && ... arguements){functor(LD::Forward<Args>(arguements)...);};

        codePaths[0] = PDP::LightWeightDelegate<void(F && functor, Args && ...)>{&emptyCodePath, &decltype(emptyCodePath)::operator()};

        codePaths[1] = PDP::LightWeightDelegate<void(F && functor, Args && ...)>{&usableCodePath, &decltype(usableCodePath)::operator()};
        return codePaths[predicate](LD::Forward<F>(functor),LD::Forward<Args>(arguements)...);
    }

}

#endif /* CompileTimeControlFlow_h */
