//
// Created by James Landess on 3/2/20.
//

#ifndef LANDESSDEVCORE_EXCLUSIVEOR_H
#define LANDESSDEVCORE_EXCLUSIVEOR_H
namespace LD
{
    namespace Detail
    {
        template<bool A, bool B>
        struct XOR
        {
            static constexpr bool value = (!A && B) || (A && !B);
        };
    }

    template<bool A, bool B>
    constexpr bool XOR = LD::Detail::XOR<A,B>::value;
}
#endif //LANDESSDEVCORE_EXCLUSIVEOR_H
