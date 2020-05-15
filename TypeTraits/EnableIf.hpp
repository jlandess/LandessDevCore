//
// Created by phoenixflower on 5/14/20.
//

#ifndef LANDESSDEVCORE_ENABLEIF_HPP
#define LANDESSDEVCORE_ENABLEIF_HPP
namespace LD
{
    template <bool, class _Tp = void> struct  EnableIf {};
    template <class _Tp> struct  EnableIf<true, _Tp> {typedef _Tp type;};

//#if _LIBCPP_STD_VER > 11
    template <bool _Bp, class _Tp = void> using Enable_If_T = typename EnableIf<_Bp, _Tp>::type;
//#endif
}
#endif //LANDESSDEVCORE_ENABLEIF_HPP
