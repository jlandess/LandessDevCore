//
// Created by phoenixflower on 10/25/19.
//

#ifndef LANDESSDEVDATASTRUCTURES_HASEXTRACTIONOPERATOR_H
#define LANDESSDEVDATASTRUCTURES_HASEXTRACTIONOPERATOR_H

#include "Definitions//Common.h"
namespace LD
{
    template<typename T, typename U>
    using RightShift = decltype(LD::Declval<LD::Decay_T <T> &>() >> LD::Declval<LD::Decay_T <U> &>());
}
#endif //LANDESSDEVDATASTRUCTURES_HASEXTRACTIONOPERATOR_H
