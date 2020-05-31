//
// Created by phoenixflower on 5/25/20.
//

#ifndef LANDESSDEVCORE_ISIMMUTABLE_H
#define LANDESSDEVCORE_ISIMMUTABLE_H
namespace LD
{
    namespace Detail
    {
        template<typename T>
        struct IsImmutable
        {
            constexpr static bool value = false;
        };
    }

    template<typename T>
    constexpr bool IsImmutable = LD::Detail::IsImmutable<T>::value;
}
#endif //LANDESSDEVCORE_ISIMMUTABLE_H
