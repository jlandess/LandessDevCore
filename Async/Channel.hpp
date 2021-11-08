//
// Created by phoenixflower on 9/1/21.
//

#ifndef LANDESSDEVCORE_CHANNEL_HPP
#define LANDESSDEVCORE_CHANNEL_HPP
#include <memory>
#include "concurrentqueue.h"
#include "Memory/shared_ptr.hpp"
#include "TypeTraits/Iterable.h"
#include "TypeTraits/IsReference.hpp"
#include "Memory/ElementReference.h"
#include "TypeTraits/FunctionalReflection.hpp"
namespace LD
{
    namespace Detail
    {
        template<typename X, typename Y>
        using DefferedExtraction = decltype(LD::Declval<X&>() =  LD::Declval<decltype(LD::Declval<decltype(LD::ExtractionIterator{LD::Declval<Y&>()})>())>());
    }
    template<typename QueueType, class = void>
    class BasicChannel;
    template<typename QueueType>
    class BasicChannel<QueueType,LD::Enable_If_T<LD::Require<
            LD::CT::IsReference(LD::Type<QueueType>{})
            >>>
    {
    private:
        LD::ElementReference<LD::Detail::Decay_T<QueueType>> mQueue;
    public:
        BasicChannel(LD::Detail::Decay_T<QueueType> * queue) noexcept:mQueue{queue} {}

        BasicChannel(LD::Detail::Decay_T<QueueType> & queue) noexcept:mQueue{queue}{}



        template<typename U, typename QType = QueueType>
        LD::Enable_If_T<LD::Require<true>,BasicChannel &>  operator << (U && object) noexcept
        {
            LD::BackInserter backInserter{*mQueue};
            backInserter = LD::Forward<U>(object);
            return (*this);
        }



        template<typename U, typename QType = QueueType>
        LD::Enable_If_T<LD::Require<LD::Exists<LD::Detail::DefferedExtraction,U,QType>>,BasicChannel &> operator >> (U & object) noexcept
        {
            LD::ExtractionIterator extractionIterator{*mQueue};
            object = extractionIterator;
            return (*this);
        }
        template<typename U, typename QType = QueueType>
        LD::Enable_If_T<LD::Require<LD::Exists<LD::Detail::DefferedExtraction,U,QType>>,BasicChannel &> operator >> (LD::Optional<U> & object) noexcept
        {
            LD::ExtractionIterator extractionIterator{*mQueue};
            object = extractionIterator;
            return (*this);
        }
    };


    template<typename QueueType>
    class BasicChannel<QueueType,LD::Enable_If_T<LD::Require<
            LD::Negate<LD::CT::IsReference(LD::Type<QueueType>{})>
    >>>
    {
    private:
        LD::SharedPointer<QueueType> mQueue;
    public:
        BasicChannel() noexcept:mQueue{LD::MakeShared<QueueType>()}
        {

        }
        BasicChannel(LD::Detail::Decay_T<QueueType>  queue, LD::Mem::MemoryResource * resource = LD::Mem::GetNewDeleteResource()) noexcept:mQueue{LD::AllocateShared(resource,queue)}
        {

        }


    };

    template<typename T> BasicChannel(T &) -> BasicChannel<T&>;
    template<typename T> BasicChannel(T *) -> BasicChannel<T&>;


    /*
    template<typename QueueType>
    class BasicChannel<QueueType,LD::Enable_If_T<LD::Require<
            !LD::CT::IsReference(LD::Type<QueueType>{})
    >>>
    {

    private:
        LD::SharedPointer<QueueType> mQueue;
    public:
    };
     */
    template<typename T>
    class Channel
    {
    private:
        LD::SharedPointer<moodycamel::ConcurrentQueue<T>> mQueue;
        //std::shared_ptr<moodycamel::ConcurrentQueue<T>> mQueue;
    public:

        Channel() noexcept:mQueue{LD::MakeShared<moodycamel::ConcurrentQueue<T>>()}
        {

        }
        Channel<T> & operator << (const T & object) noexcept
        {
            this->mQueue->enqueue(object);
            return (*this);
        }

        Channel<T> & operator >> (T & object) noexcept
        {
            while(!this->mQueue->try_dequeue(object))
            {

            }
            return (*this);
        }

        LD::UInteger Size() const noexcept
        {
            return this->mQueue->size_approx();
        }

        template<typename F, typename V = T>
        LD::Enable_If_T<
                LD::Require<
                        LD::ConvertiblyCallable<F,void(V)>::Value(),
                        LD::IsDefaultConstructible<T>
                        >,
         Channel<T>&>  operator >> (F && function) noexcept
        {
            T object;
            if(this->mQueue->try_dequeue(object))
            {
                function(object);
            }
            return (*this);
        }

        template<typename F, typename V = T>
        LD::Enable_If_T<
                LD::Either<
                        LD::Negate<LD::ConvertiblyCallable<F,void(V)>::Value()>,
                        LD::Negate<LD::IsDefaultConstructible<T>>
                >,
                Channel<T>&>  operator >> (F && function) noexcept;
    };

    template<typename T, typename ... A>
    class Channel<T(A...)>
    {

    };
}

namespace LD
{
    template<typename T>
    class BackInserter<LD::Channel<T>>
    {
    private:
        LD::ElementReference<LD::Channel<T>>  mChannel;
    public:

        BackInserter() noexcept:mChannel{nullptr}{}
        BackInserter(LD::Channel<T> * channel) noexcept:mChannel{channel}
        {

        }

        BackInserter(LD::Channel<T> & channel) noexcept:mChannel{LD::ElementReference<LD::Channel<T>>{channel}}
        {

        }

        BackInserter(const BackInserter & backSerter) noexcept:mChannel{backSerter.mChannel}{}

        BackInserter & operator = (const BackInserter & backInserter) noexcept
        {
            this->mChannel = backInserter.mChannel;
            return (*this);
        }


        template<typename U, typename V = T, typename = LD::Enable_If_T<LD::Require<LD::IsConstructible<V,U>::value>>>
        BackInserter & operator = (const U & object)  noexcept
        {
            (*this->mChannel) << T{object};
            return (*this);
        }
    };
}
#endif //LANDESSDEVCORE_CHANNEL_HPP
