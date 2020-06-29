//
// Created by phoenixflower on 6/8/20.
//

#ifndef LANDESSDEVCORE_TYPE_H
#define LANDESSDEVCORE_TYPE_H
namespace LD
{
    template<typename T>
    struct Type
    {
        using type = T;
    };

    template<typename T>
    using GetType = typename T::type ;


}
#endif //LANDESSDEVCORE_TYPE_H
