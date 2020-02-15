//
// Created by James Landess on 2/6/20.
//

#ifndef LANDESSDEVCORE_DECLVAL_H
#define LANDESSDEVCORE_DECLVAL_H
namespace LD
{
    template<typename _Tp, typename _Up = _Tp&&>
    _Up __declval(int);

    template<typename _Tp>
    _Tp __declval(long);

    template<typename _Tp>
    constexpr auto Declval() noexcept -> decltype(__declval<_Tp>(0));
}
#endif //LANDESSDEVCORE_DECLVAL_H
