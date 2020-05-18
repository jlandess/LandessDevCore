//
// Created by phoenixflower on 10/2/19.
//

#ifndef LANDESSDEVDATASTRUCTURES_THREADAFFINITY_H
#define LANDESSDEVDATASTRUCTURES_THREADAFFINITY_H


#include "Definitions/Integer.hpp"
namespace LD
{
    class ThreadAffinity
    {

    private:
        LD::UInteger mAffinity;
    public:
        ThreadAffinity():mAffinity(0)
        {}


        ThreadAffinity(const LD::UInteger & affinity):mAffinity(affinity){}

        constexpr operator LD::UInteger() const { return mAffinity; }




    };
}



#endif //LANDESSDEVDATASTRUCTURES_THREADAFFINITY_H
