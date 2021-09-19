//
// Created by phoenixflower on 9/11/21.
//

#ifndef LANDESSDEVCORE_JUST_HPP
#define LANDESSDEVCORE_JUST_HPP
#include "Then.hpp"
namespace LD
{
    namespace Async
    {
        /*
        template<typename T>
        auto Just(T && value) noexcept
        {
            auto function =  [=]()
            {

                return T{value};
            };

            //return LD::Async::Detail::Runnable<decltype(function),LD::Type<T>>{function};
            return function;
        }
         */

        template<typename T>
        auto Just(T && value) noexcept
        {
            auto function =  [=]()
            {

                return T{value};
            };

            return LD::Async::Detail::Runnable<decltype(function),LD::Type<T>>{function};
        }
    }
}
#endif //LANDESSDEVCORE_JUST_HPP
