//
// Created by phoenixflower on 10/20/19.
//

#ifndef LANDESSDEVDATASTRUCTURES_HASINSERTIONOPERATOR_H
#define LANDESSDEVDATASTRUCTURES_HASINSERTIONOPERATOR_H

#include "Definitions/Common.hpp"
namespace LD
{
    template<typename T, typename U>
    using LeftShift = decltype(LD::Declval<LD::Decay_T <T> &>() << LD::Declval<LD::Decay_T <U> const&>());
}
#endif //LANDESSDEVDATASTRUCTURES_HASINSERTIONOPERATOR_H
