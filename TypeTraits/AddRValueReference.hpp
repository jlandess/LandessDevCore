//
// Created by James Landess on 2/6/20.
//

#ifndef LANDESSDEVCORE_ADDRVALUEREFERENCE_HPP
#define LANDESSDEVCORE_ADDRVALUEREFERENCE_HPP
namespace LD
{
    namespace Detail
    {
        template <class _Tp> struct  AddRvalueReference                     {typedef _Tp&& type;};
    }
}
#endif //LANDESSDEVCORE_ADDRVALUEREFERENCE_HPP
