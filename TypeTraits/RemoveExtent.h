//
// Created by James Landess on 2/4/20.
//

#ifndef LANDESSDEVCORE_REMOVEEXTENT_H
#define LANDESSDEVCORE_REMOVEEXTENT_H

#include "Definitions/Integer.h"
namespace LD
{
    namespace Detail
    {
        template <class _Tp> struct  RemoveExtent
        {typedef _Tp type;};
        template <class _Tp> struct  RemoveExtent<_Tp[]>
        {typedef _Tp type;};
        template <class _Tp, LD::UInteger _Np> struct  RemoveExtent<_Tp[_Np]>
        {typedef _Tp type;};

#if _LIBCPP_STD_VER > 11
        template <class _Tp> using RemoveExtent_T = typename RemoveExtent<_Tp>::type;
#endif
    }

    template<typename _Tp>
    using RemoveExtent = typename LD::Detail::RemoveExtent<_Tp>::type ;
}
#endif //LANDESSDEVCORE_REMOVEEXTENT_H
