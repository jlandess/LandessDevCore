//
// Created by James Landess on 2/6/20.
//

#ifndef LANDESSDEVCORE_INTEGERSEQUENCE_HPP
#define LANDESSDEVCORE_INTEGERSEQUENCE_HPP

#include "Definitions/Integer.hpp"
#include "IsIntegralType.hpp"
namespace LD
{
    template<class _Tp, _Tp... _Ip>
    struct  IntegerSequence
    {
        typedef _Tp value_type;
        static_assert( LD::Detail::IsPrimitiveType<_Tp>::value,
                       "PDP::IntegerSequence can only be instantiated with an integral type" );
        static
        constexpr
        LD::UInteger
        size() noexcept { return sizeof...(_Ip); }
    };

    template<LD::UInteger... _Ip>
    using IndexSequence = IntegerSequence<LD::UInteger, _Ip...>;

    template<typename S1, typename S2>
    struct BuildIndexSequence;

    template<typename S1, typename S2>
    using BuildIndexSequence_T = typename BuildIndexSequence<S1, S2>::type;

    template<
            LD::UInteger... Cs1,
            LD::UInteger... Cs2
    >
    struct BuildIndexSequence<
            IndexSequence<Cs1...>,
            IndexSequence<Cs2...>
    >
    {
        using type = IndexSequence<Cs1..., sizeof...(Cs1) + Cs2...>;
    };

    template<LD::UInteger N>
    struct MakeIndexSequence;

    template<LD::UInteger N>
    using MakeIndexSequence_T =
    typename MakeIndexSequence<N>::type;

    template<LD::UInteger N>
    struct MakeIndexSequence
    {
        using type = BuildIndexSequence_T<
                MakeIndexSequence_T<N / 2>,
                MakeIndexSequence_T<N - N / 2>
        >;
    };

    template<>
    struct MakeIndexSequence<1>
    {
        using type = IndexSequence<0>;
    };

    template<>
    struct MakeIndexSequence<0>
    {
        using type = IndexSequence<>;
    };
}
#endif //LANDESSDEVCORE_INTEGERSEQUENCE_HPP
