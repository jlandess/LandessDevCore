//
// Created by James Landess on 2/6/20.
//

#ifndef LANDESSDEVCORE_REMOVECONST_HPP
#define LANDESSDEVCORE_REMOVECONST_HPP
namespace LD
{
    template <class _Tp> struct  RemoveConst            {typedef _Tp type;};
    template <class _Tp> struct  RemoveConst<const _Tp> {typedef _Tp type;};
    template<typename T>
    struct RemoveConst<volatile T> {
        typedef T type;
    };
    template<typename T>
    struct RemoveConst<const volatile T> {
        typedef T type;
    };
}
#endif //LANDESSDEVCORE_REMOVECONST_HPP
