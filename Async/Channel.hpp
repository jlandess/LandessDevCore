//
// Created by phoenixflower on 9/1/21.
//

#ifndef LANDESSDEVCORE_CHANNEL_HPP
#define LANDESSDEVCORE_CHANNEL_HPP
#include <memory>
#include "concurrentqueue.h"
#include "Memory/shared_ptr.hpp"
#include "TypeTraits/Iterable.h"
namespace LD
{
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
        LD::Channel<T> * mChannel;
    public:

        BackInserter() noexcept:mChannel{nullptr}{}
        BackInserter(LD::Channel<T> * channel) noexcept:mChannel{channel}
        {

        }

        BackInserter(const BackInserter & backSerter) noexcept:mChannel{backSerter.mChannel}{}

        BackInserter & operator = (const BackInserter & backInserter) noexcept
        {
            this->mChannel = backInserter.mChannel;
            return (*this);
        }


        BackInserter & operator = (const T & object)  noexcept
        {
            (*this->mChannel) << object;
            return (*this);
        }
    };
}
#endif //LANDESSDEVCORE_CHANNEL_HPP
