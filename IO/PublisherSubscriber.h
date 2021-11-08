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
#include "Async/Then.hpp"
#include "Patterns/LazyLambda.hpp"
#include "Algorithms/Base64Encoding.h"
#include "Async/Thread.h"
namespace LD
{
    /*
     * LD::Enable_If_T<
            LD::Require<
                    LD::CT::IsPrimitive(LD::CT::RemoveQualifiers(LD::Type<V>{})),
                    LD::ConvertiblyCallable<F,void(V,int)>::Value()>,
            LD::RequestResponse<bool()>>
     */
    template<typename Subscription,typename V, typename BackInserterType ,typename KeyType ,typename ContextExecution ,class = LD::Enable_If_T<LD::Require<LD::CT::IsPrimitive(LD::Type<V>{})>>>
    auto Subscribe1(
            Subscription & subscription,
            KeyType && key,
            BackInserterType backInserter,
            ContextExecution context,
            LD::Type<V>) noexcept
    {
        return LD::Async::Then(context,[&subscription,key,backInserter]()
        {
            auto function = [](LD::StringView response, BackInserterType backInserter)
            {
                auto possibleInteger = LD::FromString(
                        LD::Type<V>{},
                        response,
                        BackInserterType{backInserter});
                auto onInteger = [](V object, BackInserterType backInserter)
                {
                    //task(object);
                    LD::Get(backInserter)  = object;
                };
                auto onFailure = [](LD::TransactionError, BackInserterType){};
                LD::InvokeVisitation(LD::Overload{onInteger,onFailure},possibleInteger);
            };
            subscription.Subscribe(LD::StringView{key.Data()},function,BackInserterType{backInserter});
            return LD::UInteger {true};
        });

        /*
        return [&subscription,key,backInserter]()  noexcept
        {
            auto function = [](LD::StringView response, BackInserterType backInserter)
            {
                auto possibleInteger = LD::FromString(
                        LD::Type<V>{},
                        response,
                        BackInserterType{backInserter});
                auto onInteger = [](V object, BackInserterType backInserter)
                {
                    //task(object);
                    LD::Get(backInserter)  = object;
                };
                auto onFailure = [](LD::TransactionError, BackInserterType){};
                LD::InvokeVisitation(LD::Overload{onInteger,onFailure},possibleInteger);
            };
            subscription.Subscribe(LD::StringView{key.Data()},function,BackInserterType{backInserter});
            return LD::UInteger {true};
        };
         */
    }


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

    template<typename Subscription,typename V, typename F,typename KeyType , typename LockType,typename ... A>
    LD::Enable_If_T<
            LD::Require<
                    LD::CT::IsTuple(LD::CT::RemoveQualifiers(LD::Type<V>{})),
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
            LD::Require<LD::CT::IsTuple(LD::CT::RemoveQualifiers(LD::Type<V>{}))>,
            LD::RequestResponse<LD::Channel<V>(A...)>
    >
    Subscribe(
            Subscription & subscription,
            KeyType && key,
            LD::Type<V>,
            A && ... args) noexcept;


    template<typename Publisher, typename Key, typename T,class = LD::Enable_If_T<LD::Require<LD::CT::IsPrimitive(LD::CT::RemoveQualifiers(LD::Type<T>{}))>>>
    auto Publish(Publisher & publisher, Key && key, T && object, LD::DateTime datetime = LD::DateTime{}) noexcept
    {
        auto jdDate = LD::DateTimeToJulianNumber(datetime);
        LD::UInteger factoredJulianDateWithoutTime = (LD::UInteger)jdDate;
        auto jdTime = jdDate-(LD::UInteger)jdDate;
        auto memee1 = LD::ToImmutableString(LD::UInteger (jdTime* LD::Pow(10,15)));
        std::cout << "Associatasdlwek;jfg;lzskd.njfgdl,s ;lfp[kasjklpd;sx,fzhj lvp;,zsjkldsfh; ifgj.h luasld.k,flhj oilkDZ<floh dslA:N<>:gf : " << jdTime << "\n";
        std::cout << "Associatasdlwek;jfg;lzskd.njfgdl,s ;lfp[kasjklpd;sx,fzhj lvp;,zsjkldsfh; ifgj.h luasld.k,flhj oilkDZ<floh dslA:N<>:gf : " << memee1.Data() << "\n";
        /**
         * LD::ImmutableString{"."}+
                LD::ToImmutableString(factredJulianDateTime)+
                LD::ImmutableString{"."}+
                LD::ToImmutableString(factoredJulianDateWithoutTime)+
                LD::ImmutableString{"."}+
         */
         //std::cout << "FactoredJulianDateTime: " << floatingPointPortion << "\n";
        auto finalizedKey = LD::Base64Encode(publisher.NodeIdentification())+
                LD::ImmutableString{"."}+
                LD::ToImmutableString(LD::UInteger(factoredJulianDateWithoutTime))+
                LD::ImmutableString{"."}+
                memee1+
                LD::ImmutableString{"."}+
                key;
        auto primitiveAsString = LD::ToImmutableString(LD::Forward<T>(object));
        publisher.Publish(key.Data(),finalizedKey.Data());
        std::cout << "finalizedKey: " << finalizedKey.Data() << "\n";
        publisher.Publish(finalizedKey.Data(),primitiveAsString.Data());
        publisher.Publish("NWQzYzljMTU5YzlkYjA0YjI3ZjJhOTYxY2M4ZjAzZmVmOGIxOGYzNQ==","7");
        return LD::Variant<LD::TransactionError,bool>{true};
    }
    template<typename Subscription,typename V,typename KeyType, typename ExecutionContext,typename Inserter,class = LD::Enable_If_T<LD::Require<LD::CT::IsPrimitive(LD::CT::RemoveQualifiers(LD::Type<V>{}))>>>
    auto Subscribe(
            Subscription & subscription,
            KeyType && key,
            LD::Type<V>,
            Inserter  inserters,
            ExecutionContext context) noexcept
    {

        LD::LazyLambda lazy([](LD::StringView view, Inserter inserter) noexcept
        {
            auto possiblePrimitive =  LD::FromString(LD::Type<V>{},view,Inserter{inserter});

            auto onPrimitive = [](V object, Inserter inserter) noexcept
            {
                inserter = object;
            };

            auto onFailure = [](LD::TransactionError, Inserter) noexcept
            {

            };

            LD::InvokeVisitation(LD::Overload{onPrimitive,onFailure},possiblePrimitive);


        },Inserter{inserters});


        return subscription.Subscribe(LD::StringView{(key).Data()},lazy);
    }

    template<typename Subscription,typename V,typename KeyType, typename ExecutionContext,typename Inserter,class = LD::Enable_If_T<LD::Require<LD::CT::IsPrimitive(LD::CT::RemoveQualifiers(LD::Type<V>{}))>>>
    auto Subscribe123(
            Subscription & subscription,
            KeyType && key,
            LD::Type<V>,
            Inserter  inserters,
            ExecutionContext context) noexcept
    {
        LD::LazyLambda lazy([](LD::StringView view, Inserter inserter, LD::ElementReference<Subscription> subscription) noexcept
        {

            std::cout << "Internal View: " << view << "\n";
            /*
            LD::Get(subscription).Subscribe(LD::StringView{view},LD::LazyLambda{[](LD::StringView internalView, Inserter inserter)
            {
                std::cout << "View: " << internalView << "\n";
                //inserter = V{};

            },Inserter{inserter}});
             */
            //sleep(10);
            //LD::Get(subscription).Subscribe(LD::StringView {LD::ImmutableString{"abc"}.Data()},LD::LazyLambda{[](LD::StringView)
           // {

            //}},Inserter{inserter});

        },Inserter{inserters},LD::ElementReference<Subscription>{subscription});
        return subscription.Subscribe(LD::StringView{(key).Data()},lazy);
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

    template<typename Subscription,typename V, typename F,typename KeyType , typename LockType,typename ... A>
    LD::Enable_If_T<
            LD::Require<
                    LD::CT::IsTuple(LD::CT::RemoveQualifiers(LD::Type<V>{})),
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
                LD::StringView{(key+LD::ImmutableString{".Length"}).Data()},[](
                        LD::StringView response,
                        LD::ElementReference<Subscription> subscription,
                        KeyType key,
                        ContextType context,
                        LD::fixed_size_function<void(V,LockType,A...)> function) noexcept
        {
            auto possibleInteger = LD::FromString(
                    LD::Type<LD::UInteger>{},
                    response,
                    LD::ElementReference<Subscription>{subscription},
                    KeyType{key},
                    ContextType{context},
                    LD::fixed_size_function<void(V,LockType,A...)>{function});

            auto onSize = [](
                    LD::UInteger size,
                    LD::ElementReference<Subscription> subscription,
                    KeyType key,
                    ContextType context,
                    LD::fixed_size_function<void(V,LockType,A...)> function) noexcept
            {
                constexpr auto CurrentTupleSize =  LD::CT::TupleSize(LD::Type<V>{});
                constexpr auto CurrentTypes = LD::CT::GetTupleTypes(LD::Type<V>{});
                V tuple;
                if (CurrentTupleSize == size)
                {
                    LD::For<CurrentTupleSize>([](
                            auto I,
                            LD::ElementReference<Subscription> subscription,
                            KeyType key,
                            ContextType context,
                            LD::fixed_size_function<void(V,LockType,A...)> function,
                            V & tuple)
                    {
                        using CurrentType =  LD::CT::TypeAtIndex<I, LD::Detail::Decay_T<decltype(CurrentTypes)>>;

                        constexpr auto CurrentTupleSize =  LD::CT::TupleSize(LD::Type<V>{});
                        LD::StaticArray<LD::UInteger ,CurrentTupleSize> deterministicEnd{false};
                        auto callBack = [](CurrentType object, LockType lock, LD::fixed_size_function<void(V,LockType,A...)> function, ContextType context, auto Index, V & tuple, LD::StaticArray<LD::UInteger ,CurrentTupleSize> & solution) noexcept
                        {
                            printf("found tuple key \n");
                            constexpr auto CurrentTupleSize =  LD::CT::TupleSize(LD::Type<V>{});
                            LD::ScopeLock<LockType>{lock};
                            //map[datestamp][key+LD::ToImmutableString(I)] are all satisfied
                            LD::Get<Index>(tuple) = object;

                            solution[LD::UInteger(Index)] = true;
                            bool hasSatisfiedSolution = true;
                            for(LD::UInteger n = 0;n<CurrentTupleSize;++n)
                            {
                                std::cout << "Solution at Index: " << n << " " << solution[n] << "\n";
                                hasSatisfiedSolution = hasSatisfiedSolution && solution[n];
                            }

                            if (hasSatisfiedSolution)
                            {
                                LD::Get(LD::Get<0>(context)) = tuple;
                                LD::Invoke(function,context);
                            }
                            //LD::Get(LD::Get<0>(context)) = tuple;
                            //LD::Invoke(function,context);
                            //LD::Get(LD::Get<0>(context)) = object;
                            //LD::Invoke(function,context);
                        };
                        LD::Subscribe(
                                LD::Get(subscription),
                                key+LD::ImmutableString{"."}+LD::ToImmutableString(LD::UInteger(I)),
                                LD::Type<CurrentType>{},
                                callBack,
                                LockType{LD::Get(LD::Get<1>(context))},
                                LD::fixed_size_function<void(V,LockType,A...)>(function),
                                ContextType{context},
                                decltype(I){I},
                                tuple,
                                deterministicEnd);
                        return true;
                    },LD::ElementReference<Subscription>{subscription},KeyType{key},ContextType{context},LD::fixed_size_function<void(V,LockType,A...)>{function},tuple);
                }
            };

            auto onError = [](
                    LD::TransactionError,
                    LD::ElementReference<Subscription> subscription,
                    KeyType key,
                    ContextType,
                    LD::fixed_size_function<void(V,LockType,A...)>) noexcept
            {

            };

            LD::InvokeVisitation(LD::Overload{onSize,onError},possibleInteger);
        },LD::ElementReference<Subscription>{subscription},KeyType{LD::Forward<KeyType>(key)},ContextType{context},LD::fixed_size_function<void(V,LockType,A...)>{function});
        return LD::CreateResponse(LD::Type<bool>{},bool{true},LD::Forward<A>(args)...);
    }
}
#endif //LANDESSDEVCORE_PUBLISHERSUBSCRIBER_H
