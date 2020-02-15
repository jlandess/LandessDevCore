//
// Created by phoenixflower on 10/18/19.
//

#ifndef LANDESSDEVDATASTRUCTURES_COMPILETIMERANGE_H
#define LANDESSDEVDATASTRUCTURES_COMPILETIMERANGE_H


#include "Definitions/Integer.h"
namespace LD
{

    template<LD::UInteger Beg, LD::UInteger End>
    struct CompileTimeRange{

        constexpr static LD::UInteger Beginning = Beg;

        constexpr static LD::UInteger Ending = End;

    };

}
#endif //LANDESSDEVDATASTRUCTURES_COMPILETIMERANGE_H
