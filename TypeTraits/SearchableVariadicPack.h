//
// Created by phoenixflower on 10/7/19.
//

#ifndef LANDESSDEVDATASTRUCTURES_SEARCHABLEVARIADICPACK_H
#define LANDESSDEVDATASTRUCTURES_SEARCHABLEVARIADICPACK_H

#include "Definitions/Common.h"s
namespace LD
{
    template <typename Head, typename ... Tail>
    struct SearchableVariadicPack
    {
        using head = Head;
        using tail =  SearchableVariadicPack<Tail...>;
    };

    template <typename Single>
    struct SearchableVariadicPack <Single>
    {
        using head = Single;
        using tail = LD::NullClass;
    };

    namespace Detail
    {
        template <size_t index, typename typelist>
        struct Extract
        {
            using result = typename Extract<index - 1, typename typelist::tail>::result;
        };

        template <typename typelist>
        struct Extract <0, typelist>
        {
            using result = typename typelist::head;
        };
    }


    template<size_t Index, typename typelist>
    using Extract = typename LD::Detail::Extract<Index,typelist>::result ;
}
#endif //LANDESSDEVDATASTRUCTURES_SEARCHABLEVARIADICPACK_H
