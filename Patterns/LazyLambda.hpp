//
// Created by phoenixflower on 9/27/21.
//

#ifndef LANDESSDEVCORE_LAZYLAMBDA_HPP
#define LANDESSDEVCORE_LAZYLAMBDA_HPP
#include "TypeTraits/TypeList.hpp"
#include "TypeTraits/FunctionalReflection.hpp"
#include "Primitives/General/Tuple.hpp"
#include "Algorithms/Invoke.hpp"
#include "TypeTraits/Iterable.h"
namespace LD
{
    template<typename T,
            typename TL = decltype(GetSignatureArguments(LD::CT::FunctionSignature<decltype(&T::operator())>{}))>
    class LazyLambda
    {
    private:
        T mFunctor;
        LD::CT::RebindList<LD::CT::Prepend<LD::Detail::Decay_T<LD::CT::TypeAtIndex<0,TL>>,typename LD::tlist_erase_at_impl<0,TL>::type>,LD::Context> mContext;
    public:
        using AssignableType = LD::CT::TypeAtIndex<0,TL>;
        LazyLambda(T && functor) noexcept:mFunctor{LD::Forward<T>(functor)}
        {
        }

        template<typename ... A>
        LazyLambda(T && functor, A && ... args) noexcept:mFunctor{LD::Forward<T>(functor)},mContext{LD::Detail::Decay_T<LD::CT::TypeAtIndex<0,TL>>{},LD::Forward<A>(args)...}
        {

        }

        LazyLambda & operator = (const LD::CT::TypeAtIndex<0,TL> & value) noexcept
        {
            LD::Get(LD::Get<0>(this->mContext)) = value;
            LD::Invoke(this->mFunctor,this->mContext);
            return (*this);
        }
        LazyLambda & operator()(const LD::CT::TypeAtIndex<0,TL> & value) noexcept
        {
            LD::Get(LD::Get<0>(this->mContext)) = value;
            LD::Invoke(this->mFunctor,this->mContext);
            return (*this);
        }
    };

    template<typename T>
    class LazyLambda1;
    template<typename T,
            typename ... Args>
    class LazyLambda1<T(Args...)>
    {
    public:
        using TL = decltype(GetSignatureArguments(LD::CT::FunctionSignature<decltype(&T::operator())>{}));
    private:
        T mFunctor;
        LD::CT::RebindList<LD::CT::Prepend<LD::Detail::Decay_T<LD::CT::TypeAtIndex<0,TL>>,typename LD::tlist_erase_at_impl<0,TL>::type>,LD::Context> mContext;
    public:

        using AssignableType = LD::CT::TypeAtIndex<0,TL>;
        LazyLambda1(T && functor) noexcept:mFunctor{LD::Forward<T>(functor)}
        {
        }

        template<typename ... A>
        LazyLambda1(T && functor, A && ... args) noexcept:mFunctor{LD::Forward<T>(functor)},mContext{LD::Detail::Decay_T<LD::CT::TypeAtIndex<0,TL>>{},LD::Forward<A>(args)...}
        {

        }

        LazyLambda1 & operator = (const LD::CT::TypeAtIndex<0,TL> & value) noexcept
        {
            LD::Get(LD::Get<0>(this->mContext)) = value;
            LD::Invoke(this->mFunctor,this->mContext);
            return (*this);
        }
        LazyLambda1 & operator()(const LD::CT::TypeAtIndex<0,TL> & value) noexcept
        {
            LD::Get(LD::Get<0>(this->mContext)) = value;
            LD::Invoke(this->mFunctor,this->mContext);
            return (*this);
        }
    };

    template<typename T> LazyLambda(T && functor) -> LazyLambda<T>;

    template<typename T, typename ... A> LazyLambda(T && functor, A && ...) -> LazyLambda<T(A...)>;
}
namespace LD
{
    template<typename T>
    class BackInserter<LD::LazyLambda<T>>
    {
    private:
        LD::LazyLambda<T> & mLambda;
    public:
        BackInserter(LD::LazyLambda<T> & lambda) noexcept:mLambda(lambda)
        {

        }
        BackInserter& operator=(const typename LD::LazyLambda<T>::AssignableType & object) noexcept
        {
            this->mLambda = object;
            return (*this);
        }
    };
}
#endif //LANDESSDEVCORE_LAZYLAMBDA_HPP
