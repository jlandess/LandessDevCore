//
// Created by phoenixflower on 9/20/21.
//

#ifndef LANDESSDEVCORE_SHAREDSTATE_HPP
#define LANDESSDEVCORE_SHAREDSTATE_HPP
#include "Memory/ElementReference.h"
namespace LD
{
    namespace Async
    {
        template<typename T>
        class SharedState
        {
        private:
            LD::ElementReference<T> mState;
        public:
            SharedState(LD::ElementReference<T> state) noexcept:mState{state}
            {

            }
            SharedState(T & state) noexcept:mState{state}
            {

            }

            SharedState(T * state) noexcept:mState{state}
            {

            }

            T * operator -> () noexcept {return this->mState.GetPointer();}
            const T * operator -> () const noexcept {return (const T*)this->mState.GetPointer();}

            constexpr T & operator*() noexcept {return (*this->mState);}
            constexpr const T & operator*() const noexcept {return (*this->mState);}
        };

        //template<typename T> SharedState(T & state) -> LD::Async::SharedState<T>;
    }
}
#endif //LANDESSDEVCORE_SHAREDSTATE_HPP
