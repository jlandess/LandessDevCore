//
// Created by phoenixflower on 3/19/21.
//

#ifndef LANDESSDEVCORE_MOVE_HPP
#define LANDESSDEVCORE_MOVE_HPP
#include "TypeTraits/IsSame.hpp"
#include "TypeTraits/RemoveReference.hpp"
#include "TypeTraits/Detection.hpp"
namespace LD
{
    namespace Detail
    {
        template<class _Ty>
        struct _Remove_reference
        {   // remove reference
            typedef _Ty _Type;
        };



        template<class _Ty>
        struct _Remove_reference<_Ty&>
        {   // remove reference
            typedef _Ty _Type;
        };

        template<class _Ty>
        struct _Remove_reference<_Ty&&>
        {   // remove rvalue reference
            typedef _Ty _Type;
        };
    }
    //typename _Remove_reference<_Ty>::_Type&&
    template<class _Ty> inline constexpr LD::Enable_If_T<!LD::Detail::IsSame<_Ty, void>::value,typename LD::Detail::_Remove_reference<_Ty>::_Type&&> Move(_Ty&& _Arg) noexcept
    {   // forward _Arg as movable
        return ((typename LD::Detail::_Remove_reference<_Ty>::_Type&&)_Arg);
    }
}
#endif //LANDESSDEVCORE_MOVE_HPP
