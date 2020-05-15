//
// Created by James Landess on 2/6/20.
//

#ifndef LANDESSDEVCORE_ADDLVALUEREFERENCE_HPP
#define LANDESSDEVCORE_ADDLVALUEREFERENCE_HPP
namespace LD
{
    namespace Detail
    {
        template <class _Tp, bool = LD::Detail::__is_referenceable<_Tp>::value> struct __add_lvalue_reference_impl            { typedef _Tp  type; };
        template <class _Tp                                       > struct __add_lvalue_reference_impl<_Tp, true> { typedef _Tp& type; };

        template <class _Tp> struct  AddLValueReference
        {typedef typename LD::Detail::__add_lvalue_reference_impl<_Tp>::type type;};
    }
}
#endif //LANDESSDEVCORE_ADDLVALUEREFERENCE_HPP
