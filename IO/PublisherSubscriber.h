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
namespace LD
{
    template<typename Subscription,typename V, typename KeyType ,typename ... A>
    LD::Enable_If_T<
            LD::CT::IsReflectable<LD::Detail::Decay_T<V>>(),
            LD::RequestResponse<bool(A...)>>
    Subscribe(
            Subscription & subscription,
            KeyType && key,
            V & object,
            A && ... args) noexcept;

    template<typename Subscription,typename V, typename KeyType ,typename ... A>
    LD::Enable_If_T<
            LD::Require<LD::CT::IsPrimitive(LD::CT::RemoveQualifiers(LD::Type<V>{}))>,
            LD::RequestResponse<bool(A...)>>
    Subscribe(
            Subscription & subscription,
            KeyType && key,
            V & object,
            A && ... args) noexcept;

    template<typename Subscription,typename V, typename CallBackFunctor ,typename KeyType ,typename ... A>
    LD::Enable_If_T<
            LD::Require<LD::CT::IsPrimitive(LD::CT::RemoveQualifiers(LD::Type<V>{})),
            LD::ConvertiblyCallable<CallBackFunctor,void(V value, A && ...)>::Value()
            >,
            LD::RequestResponse<LD::Promise<V>(A...)>>
    Subscribe(
            Subscription & subscription,
            KeyType && key,
            LD::Type<V> ,
            CallBackFunctor && functor,
            A && ... args) noexcept;

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


    template<typename Subscription,typename V, typename KeyType ,typename ... A>
    LD::Enable_If_T<
            LD::Require<LD::CT::IsPrimitive(LD::CT::RemoveQualifiers(LD::Type<V>{}))>,
            LD::RequestResponse<bool(A...)>>
    Subscribe(
            Subscription & subscription,
            KeyType && key,
            V & object,
            A && ... args) noexcept
    {
        subscription.Subscribe(
                LD::StringView{key.Data()},[](LD::StringView response, V & object) noexcept
        {
            auto possibleInteger = LD::FromString(LD::Type<V>{},response,object);

            auto onPrimitive = [](V primitive, V & assignablePrimitive) noexcept
            {
                assignablePrimitive = primitive;
            };


            auto onError = [](LD::TransactionError, V &) noexcept
            {

            };

            LD::InvokeVisitation(LD::Overload{onPrimitive,onError},possibleInteger);
        },object);
        return LD::CreateResponse(LD::Type<bool>{},LD::TransactionError{},LD::Forward<A>(args)...);
    }

    template<typename Subscription,typename V, typename KeyType ,typename ... A>
    LD::Enable_If_T<
            LD::Require<LD::CT::IsPrimitive(LD::CT::RemoveQualifiers(LD::Type<V>{}))>,
            LD::RequestResponse<LD::Channel<V>(A...)>>
    Subscribe(
            Subscription & subscription,
            KeyType && key,
            LD::Type<V>,
            LD::Channel<V> queue,
            A && ... args) noexcept
    {
        //LD::Commitment<V> primitivePromise;

        subscription.Subscribe(
                LD::StringView{key.Data()},[](LD::StringView response, LD::Channel<V> & queue) noexcept
                {

                    auto possibleInteger = LD::FromString(LD::Type<V>{},response,queue);

                    auto onPrimitive = [](V primitive, LD::Channel<V> & queue) noexcept
                    {
                        //queue->enqueue(primitive);
                        queue << primitive;
                        //assignablePrimitive.AddItem(primitive);
                        //assignablePrimitive = primitive;
                    };


                    auto onError = [](LD::TransactionError, LD::Channel<V> & queue) noexcept
                    {

                    };


                    LD::InvokeVisitation(LD::Overload{onPrimitive,onError},possibleInteger);
                },queue);

        return LD::CreateResponse(LD::Type<LD::Channel<V>>{},LD::Channel<V>{queue},LD::Forward<A>(args)...);
    }
    /*
    template<typename Subscription,typename V, typename KeyType ,typename ... A>
    LD::Enable_If_T<
            LD::Require<LD::CT::IsImmutableString(LD::CT::RemoveQualifiers(LD::Type<V>{}))>,
            LD::RequestResponse<bool(A...)>>
    Subscribe(
            Subscription & subscription,
            KeyType && key,
            V & object,
            A && ... args) noexcept
    {
        subscription.Subscribe(
                LD::StringView{key.Data()},[](LD::StringView response, V & object) noexcept
        {

            LD::UInteger viewSize = response.size();
            constexpr LD::UInteger ImmutableCapacity = LD::CT::ImmutableStringCapacity(LD::Type<V>{});
            if (viewSize <= ImmutableCapacity)
            {
                for(LD::UInteger viewCharacterIndex = 0;viewCharacterIndex<viewSize;++viewCharacterIndex)
                {
                    object[viewCharacterIndex] = response[viewCharacterIndex];
                }
            }


        },object);
        return LD::CreateResponse(LD::Type<bool>{},LD::TransactionError{},LD::Forward<A>(args)...);
    }

    template<typename Subscription,typename V, typename KeyType ,typename ... A>
    LD::Enable_If_T<
            LD::Require<LD::CT::IsVariant(LD::CT::RemoveQualifiers(LD::Type<V>{}))>,
            LD::RequestResponse<bool(A...)>>
    Subscribe(
            Subscription & subscription,
            KeyType && key,
            V & object,
            A && ... args) noexcept
    {

        auto onVariant = [&]( auto & heldObjectInVariant) noexcept
        {
            return LD::Subscribe(
                    subscription,
                    LD::Forward<KeyType>(key),
                    heldObjectInVariant,
                    LD::Forward<A>(args)...);
        };

        return LD::MultiMatch(LD::Overload{onVariant},object);
    }

    template<typename Subscription,typename V, typename KeyType ,typename ... A>
    LD::Enable_If_T<
            LD::Require<LD::CT::IsTuple(LD::CT::RemoveQualifiers(LD::Type<V>{}))>,
            LD::RequestResponse<bool(A...)>>
    Subscribe(
            Subscription & subscription,
            KeyType && key,
            V & object,
            A && ... args) noexcept
    {
        constexpr LD::UInteger CurrentTupleSize = LD::CT::TupleSize(LD::Type<V>{});
        LD::For<CurrentTupleSize>([](
                auto I,
                Subscription & subscription,
                KeyType && key,
                V & object)
        {
            LD::Subscribe(
                    subscription,
                    key+LD::ImmutableString{"."}+LD::ToImmutableString(LD::UInteger(I)),
                    LD::Get(LD::Get<I>(object)));
            return true;
        },subscription,LD::Forward<KeyType>(key),object);
        return LD::CreateResponse(LD::Type<bool>{},LD::TransactionError{},LD::Forward<A>(args)...);
    }
    template<typename Subscription,typename V, typename KeyType ,typename ... A>
    LD::Enable_If_T<
            LD::Require<LD::CT::Spannable(LD::CT::RemoveQualifiers(LD::Type<V>{}))>,
            LD::RequestResponse<bool(A...)>>
    Subscribe(
            Subscription & subscription,
            KeyType && key,
            V & object,
            A && ... args) noexcept
    {
        subscription.Subscribe(key+LD::ImmutableString{".LandessDevLength"},
        [](LD::StringView response, KeyType && key ,V & spannableObject, Subscription & subscription) noexcept
        {
            auto possibleInteger = LD::FromString(LD::Type<LD::UInteger>{},response);

            auto onPrimitive = [](LD::UInteger primitive) noexcept -> LD::Optional<LD::UInteger>
            {
                return LD::Optional<LD::UInteger>{primitive};
            };


            auto onError = [](LD::TransactionError) noexcept -> LD::Optional<LD::UInteger>
            {
                return LD::Optional<LD::UInteger>{};
            };

            LD::Optional<LD::UInteger> possibleSize = LD::InvokeVisitation(
                    LD::Overload{onPrimitive,onError},
                    possibleInteger);
            if (possibleSize.HasValue())
            {
                LD::UInteger receivedArraySize = *possibleSize;

                for(LD::UInteger objectsToReceiveIndex = 0;
                objectsToReceiveIndex<receivedArraySize;
                ++objectsToReceiveIndex)
                {
                    int abc;
                    LD::BackInserter inserter{spannableObject};
                    LD::Subscribe(subscription,key+LD::ImmutableString{"."}+LD::ToImmutableString(objectsToReceiveIndex),
                    abc);
                }
            }
        },LD::Forward<KeyType>(key),object,subscription);

        return LD::CreateResponse(LD::Type<bool>{},LD::TransactionError{},LD::Forward<A>(args)...);
    }
     */
}
#endif //LANDESSDEVCORE_PUBLISHERSUBSCRIBER_H
