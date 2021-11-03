//
// Created by phoenixflower on 12/4/20.
//

#ifndef LANDESSDEVCORE_USESALLOC_H
#define LANDESSDEVCORE_USESALLOC_H
#include "TypeTraits/__or.hpp"
#include "TypeTraits/IsConstructible.hpp"
#include "Detection.hpp"
#include "TypeTraits/IntegralConstant.hpp"
#include "IsErasedOrConvertible.hpp"
namespace LD
{
    struct allocator_arg_t{};

    template<typename _Tp, typename _Alloc, typename = LD::Void_T<>>
    struct __uses_allocator_helper
            : LD::FalseType { };

    template<typename _Tp, typename _Alloc>
    struct __uses_allocator_helper<_Tp, _Alloc,
            LD::Void_T<typename _Tp::allocator_type>>
            : LD::IsErasedOrConvertible<_Alloc, typename _Tp::allocator_type>::type
    { };


    template<typename _Tp, typename _Alloc>
    struct uses_allocator
            : __uses_allocator_helper<_Tp, _Alloc>::type
    { };

    struct __uses_alloc_base { };

    struct __uses_alloc0 : __uses_alloc_base
    {
        struct _Sink { void operator=(const void*) { } } _M_a;
    };

    template<typename _Alloc>
    struct __uses_alloc1 : __uses_alloc_base { const _Alloc* _M_a; };

    template<typename _Alloc>
    struct __uses_alloc2 : __uses_alloc_base { const _Alloc* _M_a; };

    template<bool, typename _Tp, typename _Alloc, typename... _Args>
    struct __uses_alloc;


    template<typename _Tp, typename _Alloc, typename... _Args>
    struct __uses_alloc<true, _Tp, _Alloc, _Args...>
            : LD::Conditonal<
                    LD::IsConstructible<_Tp, allocator_arg_t, const _Alloc&, _Args...>::value,
                    __uses_alloc1<_Alloc>,
                    __uses_alloc2<_Alloc>>::type
    {
// _GLIBCXX_RESOLVE_LIB_DEFECTS
// 2586. Wrong value category used in scoped_allocator_adaptor::construct
        static_assert(LD::CT::Or<
                              LD::IsConstructible<_Tp, allocator_arg_t, const _Alloc&, _Args...>,
                              LD::IsConstructible<_Tp, _Args..., const _Alloc&>>::value,
                      "construction with an allocator must be possible"
                      " if uses_allocator is true");
    };
    template<typename _Tp, typename _Alloc, typename... _Args>
    struct __uses_alloc<false, _Tp, _Alloc, _Args...>
            : __uses_alloc0 { };
    template<typename _Tp, typename _Alloc, typename... _Args>
    using __uses_alloc_t = __uses_alloc<uses_allocator<_Tp, _Alloc>::value, _Tp, _Alloc, _Args...>;


}
#endif //LANDESSDEVCORE_USESALLOC_H
