//
// Created by James Landess on 2/6/20.
//

#ifndef LANDESSDEVCORE_REMOVEALLEXTENTS_H
#define LANDESSDEVCORE_REMOVEALLEXTENTS_H

#include "Definitions/Integer.h"
namespace LD
{
    template <class _Tp> struct  RemoveAllExtents
    {typedef _Tp type;};
    template <class _Tp> struct  RemoveAllExtents<_Tp[]>
    {typedef typename RemoveAllExtents<_Tp>::type type;};
    template <class _Tp, LD::UInteger _Np> struct  RemoveAllExtents<_Tp[_Np]>
    {typedef typename RemoveAllExtents<_Tp>::type type;};
}


#if _LIBCPP_STD_VER > 11
template <class _Tp> using remove_all_extents_t = typename LD::RemoveAllExtents<_Tp>::type;
#endif
#endif //LANDESSDEVCORE_REMOVEALLEXTENTS_H
