//
// Created by phoenixflower on 5/25/20.
//

#ifndef LANDESSDEVCORE_RESIZEABLE_H
#define LANDESSDEVCORE_RESIZEABLE_H
#include "Declval.hpp"
#include "Definitions/Integer.hpp"
#include "EnableIf.hpp"
namespace LD
{
    namespace Detail
    {
        template<typename T>
        using CameCaseResize = decltype(LD::Declval<T>().resize(LD::Declval<LD::UInteger>()));

        template<typename T>
        using ReverseCamelCaseReize = decltype(LD::Declval<T>().ReSize(LD::Declval<LD::UInteger>()));
    }

    template<typename Container, class = void>
    class Resizer;



}
#endif //LANDESSDEVCORE_RESIZEABLE_H
