//
// Created by phoenixflower on 1/21/21.
//

#ifndef LANDESSDEVCORE_ADDALL_HPP
#define LANDESSDEVCORE_ADDALL_HPP
#include "Invoke.hpp"
#include "Primitives/General/Immutable.hpp"
//#include "Definitions/Common.hpp"
namespace LD
{
    //namespace IMGUI
    //{
        //namespace Detail
        //{
            template<typename T>
            auto AddAll(const T &arg) noexcept
            {
                return LD::ToImmutableString(LD::Get(arg));
            }
            template<typename T>
            auto AddAllWithDelimeter( T && arg) noexcept
            {
                return arg;
            }

            template<typename T, typename... ARGS>
            auto AddAll(const T &arg, const ARGS &... args) noexcept
            {
                return LD::ToImmutableString(LD::Get(arg)) + LD::AddAll(args...);
            }

            template<typename T, typename... A>
            auto AddAllWithDelimeter(const T &arg, A &&... args) noexcept
            {
                return arg +  LD::ImmutableString{","} + LD::AddAllWithDelimeter(LD::Forward<A>(args)...);
            }

       // }
    //}
}

#endif //LANDESSDEVCORE_ADDALL_HPP
