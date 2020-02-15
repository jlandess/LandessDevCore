//
// Created by James Landess on 2/4/20.
//

#ifndef LANDESSDEVCORE_REMOVEREFERENCE_H
#define LANDESSDEVCORE_REMOVEREFERENCE_H
namespace LD
{
    namespace Detail
    {
        template <class _Tp> struct  RemoveReference        {typedef _Tp type;};
        template <class _Tp> struct  RemoveReference<_Tp&>        {typedef _Tp type;};
        template <class _Tp> struct  RemoveReference<_Tp&&>        {typedef _Tp type;};
    }


    template<typename _Tp>
    using RemoveReference = typename LD::Detail::RemoveReference<_Tp>::type ;

}
#endif //LANDESSDEVCORE_REMOVEREFERENCE_H
