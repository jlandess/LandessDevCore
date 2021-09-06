//
// Created by phoenixflower on 8/29/21.
//

#ifndef LANDESSDEVCORE_PUBLISHERSUBSCRIBER_H
#define LANDESSDEVCORE_PUBLISHERSUBSCRIBER_H
#include "Core/RequestResponse.hpp"
#include "Algorithms/FromString.hpp"
#include "Primitives/General/Immutable.hpp"
#include "TypeTraits/FunctionalReflection.hpp"
#include "TypeTraits/IsVariant.hpp"
#include "Primitives/General/Span.hpp"
#include "Reflection/Reflection.hpp"
#include "Database1.h"
#include "Async/Promise.h"
#include "Async/Commitment.h"
#include "Async/Channel.hpp"
#include <Functor/fixed_size_function.hpp>
#include "Async/Locks/SharedLock.hpp"
#include "Async/Locks/DummyLock.hpp"
#include "Async/Locks/DummySharedLock.hpp"
namespace LD
{

    template<typename Subscription,typename V, typename F,typename KeyType , typename LockType,typename ... A>
    LD::Enable_If_T<
            LD::Require<
                    LD::CT::IsPrimitive(LD::CT::RemoveQualifiers(LD::Type<V>{})),
                    LD::ConvertiblyCallable<F,void(V,LockType,A...)>::Value(),
                    LD::CT::IsSharedLock(LD::Type<LockType>{})>,
            LD::RequestResponse<bool(A...)>>
    Subscribe(
            Subscription & subscription,
            KeyType && key,
            LD::Type<V>,
            F && callBack,
            LockType  lockType,
            A && ... args) noexcept;

    template<typename Subscription,typename V, typename KeyType ,typename ... A>
    LD::Enable_If_T<
            LD::Require<LD::CT::IsPrimitive(LD::CT::RemoveQualifiers(LD::Type<V>{}))>,
            LD::RequestResponse<LD::Channel<V>(A...)>
             >
    Subscribe(
            Subscription & subscription,
            KeyType && key,
            LD::Type<V>,
            A && ... args) noexcept;


    template<typename Subscription,typename V, typename F,typename KeyType , typename LockType,typename ... A>
    LD::Enable_If_T<
            LD::Require<
                    LD::CT::IsImmutableString(LD::CT::RemoveQualifiers(LD::Type<V>{})),
                    LD::ConvertiblyCallable<F,void(V,LockType,A...)>::Value(),
                    LD::CT::IsSharedLock(LD::Type<LockType>{})>,
            LD::RequestResponse<bool(A...)>>
    Subscribe(
            Subscription & subscription,
            KeyType && key,
            LD::Type<V>,
            F && callBack,
            LockType  lockType,
            A && ... args) noexcept;


    template<typename Subscription,typename V, typename KeyType ,typename ... A>
    LD::Enable_If_T<
            LD::Require<LD::CT::IsImmutableString(LD::CT::RemoveQualifiers(LD::Type<V>{}))>,
            LD::RequestResponse<LD::Channel<V>(A...)>
    >
    Subscribe(
            Subscription & subscription,
            KeyType && key,
            LD::Type<V>,
            A && ... args) noexcept;


    template<typename Subscription,typename V, typename F,typename KeyType , typename LockType,typename ... A>
    LD::Enable_If_T<
            LD::Require<
                    LD::CT::IsVariant(LD::CT::RemoveQualifiers(LD::Type<V>{})),
                    LD::ConvertiblyCallable<F,void(V,LockType,A...)>::Value(),
                    LD::CT::IsSharedLock(LD::Type<LockType>{})>,
            LD::RequestResponse<bool(A...)>>
    Subscribe(
            Subscription & subscription,
            KeyType && key,
            LD::Type<V>,
            F && callBack,
            LockType  lockType,
            A && ... args) noexcept;

    template<typename Subscription,typename V, typename KeyType ,typename ... A>
    LD::Enable_If_T<
            LD::Require<LD::CT::IsVariant(LD::CT::RemoveQualifiers(LD::Type<V>{}))>,
            LD::RequestResponse<LD::Channel<V>(A...)>
    >
    Subscribe(
            Subscription & subscription,
            KeyType && key,
            LD::Type<V>,
            A && ... args) noexcept;


    /*
    template<typename Subscription,typename V, typename KeyType ,typename ... A>
    LD::Enable_If_T<
            LD::Require<LD::CT::IsVariant(LD::CT::RemoveQualifiers(LD::Type<V>{}))>,
            LD::RequestResponse<LD::Channel<V>(A...)>
    >
    Subscribe(
            Subscription & subscription,
            KeyType && key,
            LD::Type<V>,
            A && ... args) noexcept;
            */

    /*
    template<typename Subscription,typename V, typename KeyType ,typename ... A>
    LD::Enable_If_T<
            LD::CT::IsReflectable<LD::Detail::Decay_T<V>>(),
            LD::RequestResponse<bool(A...)>>
    Subscribe(
            Subscription & subscription,
            KeyType && key,
            V & object,
            A && ... args) noexcept;
            */

    /*
    template<typename Subscription,typename V, typename KeyType ,typename ... A>
    LD::Enable_If_T<
            LD::Require<LD::CT::IsPrimitive(LD::CT::RemoveQualifiers(LD::Type<V>{}))>,
            LD::RequestResponse<bool(A...)>>
    Subscribe(
            Subscription & subscription,
            KeyType && key,
            V & object,
            A && ... args) noexcept;
            */


    /*
    template<typename Subscription,typename V, typename KeyType ,typename ... A>
    LD::Enable_If_T<
            LD::Require<LD::CT::IsImmutableString(LD::CT::RemoveQualifiers(LD::Type<V>{}))>,
            LD::RequestResponse<bool(A...)>>
    Subscribe(
            Subscription & subscription,
            KeyType && key,
            V & object,
            A && ... args) noexcept;


    template<typename Subscription,typename V, typename KeyType ,typename ... A>
    LD::Enable_If_T<
            LD::Require<LD::CT::IsTuple(LD::CT::RemoveQualifiers(LD::Type<V>{}))>,
            LD::RequestResponse<bool(A...)>>
    Subscribe(
            Subscription & subscription,
            KeyType && key,
            V & object,
            A && ... args) noexcept;

    template<typename Subscription,typename V, typename KeyType ,typename ... A>
    LD::Enable_If_T<
            LD::Require<LD::CT::IsVariant(LD::CT::RemoveQualifiers(LD::Type<V>{}))>,
            LD::RequestResponse<bool(A...)>>
    Subscribe(
            Subscription & subscription,
            KeyType && key,
            V & object,
            A && ... args) noexcept;

    template<typename Subscription,typename V, typename KeyType ,typename ... A>
    LD::Enable_If_T<
            LD::Require<LD::CT::Spannable(LD::CT::RemoveQualifiers(LD::Type<V>{}))>,
            LD::RequestResponse<bool(A...)>>
    Subscribe(
            Subscription & subscription,
            KeyType && key,
            V & object,
            A && ... args) noexcept;

     */

    template<typename Subscription,typename V, typename F,typename KeyType , typename LockType ,typename ... A>
    LD::Enable_If_T<
            LD::Require<
                    LD::CT::IsPrimitive(LD::CT::RemoveQualifiers(LD::Type<V>{})),
                    LD::ConvertiblyCallable<F,void(V,LockType,A...)>::Value(),
                    LD::CT::IsSharedLock(LD::Type<LockType>{})>,
            LD::RequestResponse<bool(A...)>>
    Subscribe(
            Subscription & subscription,
            KeyType && key,
            LD::Type<V>,
            F && callBack,
            LockType  lock,
            A && ... args) noexcept
    {
        auto context = LD::MakeTuple(V{},LockType{lock},LD::Forward<A>(args)...);
        using ContextType = decltype(context);
        LD::fixed_size_function<void(V,LockType,A...)> function{LD::Forward<F>(callBack)};
        subscription.Subscribe(
                LD::StringView{key.Data()},
        [](
                LD::StringView response,
                ContextType passedInContext,
                LD::fixed_size_function<void(V,LockType,A...)>  callback)
        {
            auto possibleInteger = LD::FromString(
                    LD::Type<V>{},
                    response,passedInContext,callback);

            auto onPrimitive = [](
                    V primitive,
                    ContextType & context,
                    LD::fixed_size_function<void(V,LockType,A...)> & callback) noexcept
            {
                LD::Get(LD::Get<0>(context)) = primitive;
                LD::Invoke(callback,context);
            };

            auto onError = [](LD::TransactionError, ContextType & context, LD::fixed_size_function<void(V,LockType,A...)> &) noexcept
            {

            };

            LD::InvokeVisitation(LD::Overload{onPrimitive,onError},possibleInteger);
        },ContextType{context},LD::fixed_size_function<void(V,LockType,A...)>{function});
        return LD::CreateResponse(LD::Type<bool>{},bool{true},LD::Forward<A>(args)...);
    }

    template<typename Subscription,typename V, typename KeyType ,typename ... A>
    LD::Enable_If_T<
            LD::Require<LD::CT::IsPrimitive(LD::CT::RemoveQualifiers(LD::Type<V>{}))>,
            LD::RequestResponse<LD::Channel<V>(A...)>>
    Subscribe(
            Subscription & subscription,
            KeyType && key,
            LD::Type<V>,
            A && ... args) noexcept
    {
        LD::Channel<V> queue;
        subscription.Subscribe(
                LD::StringView{key.Data()},[](LD::StringView response, LD::Channel<V> & queue) noexcept
                {

                    auto possibleInteger = LD::FromString(LD::Type<V>{},response,queue);

                    auto onPrimitive = [](V primitive, LD::Channel<V> & queue) noexcept
                    {
                        queue << primitive;
                    };

                    auto onError = [](LD::TransactionError, LD::Channel<V> & queue) noexcept
                    {

                    };


                    LD::InvokeVisitation(LD::Overload{onPrimitive,onError},possibleInteger);
                },queue);

        return LD::CreateResponse(LD::Type<LD::Channel<V>>{},LD::Channel<V>{queue},LD::Forward<A>(args)...);
    }

    template<typename Subscription,typename V, typename F,typename KeyType , typename LockType,typename ... A>
    LD::Enable_If_T<
            LD::Require<
                    LD::CT::IsImmutableString(LD::CT::RemoveQualifiers(LD::Type<V>{})),
                    LD::ConvertiblyCallable<F,void(V,LockType,A...)>::Value(),
                    LD::CT::IsSharedLock(LD::Type<LockType>{})>,
            LD::RequestResponse<bool(A...)>>
    Subscribe(
            Subscription & subscription,
            KeyType && key,
            LD::Type<V>,
            F && callBack,
            LockType  lock,
            A && ... args) noexcept
    {
        auto context = LD::MakeTuple(V{},LockType{lock},LD::Forward<A>(args)...);
        using ContextType = decltype(context);
        LD::fixed_size_function<void(V,LockType,A...)> function{LD::Forward<F>(callBack)};
        subscription.Subscribe(
                LD::StringView{key.Data()},
                [](
                        LD::StringView response,
                        ContextType passedInContext,
                        LD::fixed_size_function<void(V,LockType,A...)>  callback)
        {
           constexpr LD::UInteger MaxCapacity = LD::CT::ImmutableStringCapacity(LD::Type<V>{});
           LD::UInteger  responseSize = response.size();
           if (responseSize <= MaxCapacity)
           {
               V immutableString;
               for(LD::UInteger n = 0;n<responseSize;++n)
               {
                   immutableString[n] = response[n];
               }
               LD::Get(LD::Get<0>(context)) = immutableString;
               LD::Invoke(callback,context);
           }
        },ContextType{context},LD::fixed_size_function<void(V,LockType,A...)>{function});
        return LD::CreateResponse(LD::Type<bool>{},bool{true},LD::Forward<A>(args)...);
    }

    template<typename Subscription,typename V, typename KeyType ,typename ... A>
    LD::Enable_If_T<
            LD::Require<LD::CT::IsImmutableString(LD::CT::RemoveQualifiers(LD::Type<V>{}))>,
            LD::RequestResponse<LD::Channel<V>(A...)>
    >
    Subscribe(
            Subscription & subscription,
            KeyType && key,
            LD::Type<V>,
            A && ... args) noexcept
    {
        LD::Channel<V> queue;
        subscription.Subscribe(
                LD::StringView{key.Data()},[](LD::StringView response, LD::Channel<V> & queue) noexcept
        {
           constexpr LD::UInteger MaxCapacity = LD::CT::ImmutableStringCapacity(LD::Type<V>{});
           LD::UInteger responseSize = response.size();
            if (responseSize <= MaxCapacity)
            {
                V immutableString;
                for(LD::UInteger n = 0;n<responseSize;++n)
                {
                    immutableString[n] = response[n];
                }
                queue << immutableString;
            }
        },queue);
        return LD::CreateResponse(LD::Type<LD::Channel<V>>{},LD::Channel<V>{queue},LD::Forward<A>(args)...);
    }

    template<typename Subscription,typename V, typename F,typename KeyType , typename LockType,typename ... A>
    LD::Enable_If_T<
            LD::Require<
                    LD::CT::IsVariant(LD::CT::RemoveQualifiers(LD::Type<V>{})),
                    LD::ConvertiblyCallable<F,void(V,LockType,A...)>::Value(),
                    LD::CT::IsSharedLock(LD::Type<LockType>{})>,
            LD::RequestResponse<bool(A...)>>
    Subscribe(
            Subscription & subscription,
            KeyType && key,
            LD::Type<V>,
            F && callBack,
            LockType  lock,
            A && ... args) noexcept
    {
        auto context = LD::MakeTuple(V{},LockType{lock},LD::Forward<A>(args)...);
        using ContextType = decltype(context);
        LD::fixed_size_function<void(V,LockType,A...)> function{LD::Forward<F>(callBack)};
        subscription.Subscribe(
                LD::StringView{(key+LD::ImmutableString{".Type"}).Data()},[](
                        LD::StringView response,
                        LD::ElementReference<Subscription> sub,
                        KeyType  key,
                        LD::fixed_size_function<void(V,LockType,A...)> function,
                        ContextType context) noexcept
        {
            constexpr auto ListOfTypes = LD::CT::GetVariantTypes(LD::Type<V>{});
            constexpr auto SizeOfList = LD::CT::TypeListSize(LD::CT::RemoveQualifiers(LD::Type<decltype(ListOfTypes)>{}));
            LD::For<SizeOfList>([](
                    auto I,
                    LD::StringView response,
                    LD::ElementReference<Subscription> sub,
                    KeyType key,
                    LD::fixed_size_function<void(V,LockType,A...)> function,
                    ContextType context) noexcept
            {
                using CurrentType = LD::CT::TypeAtIndex<I, LD::Detail::Decay_T<decltype(ListOfTypes)>>;

                auto constexpr className = LD::CT::GetClassName(LD::Type<CurrentType>{});
                if(response == LD::StringView {className.Data()})
                {

                    auto callBack = [](
                            CurrentType returnable,
                            LockType lock,
                            LD::fixed_size_function<void(V,LockType,A...)> function,
                            ContextType context)
                    {
                        LD::Get(LD::Get<0>(context)) = returnable;
                        LD::Invoke(function,context);
                    };
                    LD::Subscribe(
                            LD::Get(sub),
                            key,
                            LD::Type<CurrentType>{},
                            callBack,
                            LockType{LD::Get(LD::Get<1>(context))},
                            LD::fixed_size_function<void(V,LockType,A...)>{function},
                            ContextType{context});
                    return false;
                }

                return true;
            },response,LD::ElementReference<Subscription>{sub},key,LD::fixed_size_function<void(V,LockType,A...)>{function},ContextType{context});
        },LD::ElementReference<Subscription>{subscription},KeyType{LD::Forward<KeyType>(key)},LD::fixed_size_function<void(V,LockType,A...)>{function},ContextType{context});
        return LD::CreateResponse(LD::Type<bool>{},bool{true},LD::Forward<A>(args)...);
    }
    template<typename Subscription,typename V, typename KeyType ,typename ... A>
    LD::Enable_If_T<
            LD::Require<LD::CT::IsVariant(LD::CT::RemoveQualifiers(LD::Type<V>{}))>,
            LD::RequestResponse<LD::Channel<V>(A...)>
    >
    Subscribe(
            Subscription & subscription,
            KeyType && key,
            LD::Type<V>,
            A && ... args) noexcept
    {
        LD::Channel<V> queue;

        subscription.Subscribe(
                LD::StringView{key.Data()},[](
                        LD::StringView response,
                        LD::Channel<V> & queue,
                        LD::ElementReference<Subscription> sub,
                        KeyType key) noexcept
        {

            constexpr auto ListOfTypes = LD::CT::GetVariantTypes(LD::Type<V>{});
            constexpr auto SizeOfList = LD::CT::TypeListSize(LD::CT::RemoveQualifiers(LD::Type<decltype(ListOfTypes)>{}));

            LD::For<SizeOfList>([](
                    auto I,
                    LD::StringView response,
                    KeyType key,
                    LD::ElementReference<Subscription> sub,
                    LD::Channel<V> & queue)
            {
                using CurrentType = LD::CT::TypeAtIndex<I, LD::Detail::Decay_T<decltype(ListOfTypes)>>;
                auto constexpr className = LD::CT::GetClassName(LD::Type<CurrentType>{});
                if (response == LD::StringView {className.Data()})
                {
                    LD::DummySharedLock<LD::DummyLock> lock;
                    auto callBack = [](CurrentType object, LD::DummySharedLock<LD::DummyLock> lock, LD::Channel<V> & queue) noexcept
                    {
                        queue << object;
                    };
                    LD::Subscribe(
                            LD::Get(sub),
                            key,
                            LD::Type<CurrentType>{},
                            callBack,
                            LD::DummySharedLock<LD::DummyLock>{},
                            queue);
                    return false;
                }
                return true;
            },LD::StringView{response},KeyType{key},LD::ElementReference<Subscription>{sub},queue);

        },queue,LD::ElementReference<Subscription>{subscription},KeyType{LD::Forward<KeyType>(key)});
        return LD::CreateResponse(LD::Type<LD::Channel<V>>{},LD::Channel<V>{queue},LD::Forward<A>(args)...);
    }
}
#endif //LANDESSDEVCORE_PUBLISHERSUBSCRIBER_H
