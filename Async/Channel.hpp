//
// Created by phoenixflower on 9/1/21.
//

#ifndef LANDESSDEVCORE_CHANNEL_HPP
#define LANDESSDEVCORE_CHANNEL_HPP
#include <memory>
#include "concurrentqueue.h"
#include "Memory/shared_ptr.hpp"
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
    };

    template<typename T, typename ... A>
    class Channel<T(A...)>
    {

    };
}
#endif //LANDESSDEVCORE_CHANNEL_HPP
