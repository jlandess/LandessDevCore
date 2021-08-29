//
// Created by phoenixflower on 8/29/21.
//

#ifndef LANDESSDEVCORE_PUBLISHERSUBSCRIBER_H
#define LANDESSDEVCORE_PUBLISHERSUBSCRIBER_H
#include "Core/RequestResponse.hpp"
#include "Algorithms/FromString.hpp"
namespace LD
{
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
}
#endif //LANDESSDEVCORE_PUBLISHERSUBSCRIBER_H
