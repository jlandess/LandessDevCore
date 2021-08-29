//
// Created by phoenixflower on 8/27/21.
//

#ifndef LANDESSDEVCORE_OVERLOAD_HPP
#define LANDESSDEVCORE_OVERLOAD_HPP
namespace LD
{
    template<class... Ts> struct Overload : Ts... { using Ts::operator()...; };
    template<class... Ts> Overload(Ts...) -> Overload<Ts...>;
}
#endif //LANDESSDEVCORE_OVERLOAD_HPP
