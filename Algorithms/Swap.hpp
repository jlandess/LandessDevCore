//
// Created by phoenixflower on 5/14/20.
//

#ifndef LANDESSDEVCORE_SWAP_HPP
#define LANDESSDEVCORE_SWAP_HPP
namespace LD
{
    template<typename T>
    void Swap(T & firstObject, T & secondObject)
    {
        T tempObject = firstObject;
        firstObject = secondObject;
        secondObject = tempObject;
    }

    template<typename T>
    void Swap(T * firstObject, T * secondObject)
    {
        T tempObject = *firstObject;
        *firstObject = *secondObject;
        *secondObject = tempObject;
    }
}
#endif //LANDESSDEVCORE_SWAP_HPP
