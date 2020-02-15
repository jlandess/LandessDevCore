//
// Created by James Landess on 2/4/20.
//

#ifndef LANDESSDEVCORE_VARIADICPACK_H
#define LANDESSDEVCORE_VARIADICPACK_H

#include "Decay.h"
#include "Conditional.h"
namespace LD
{

    template<typename ...>
    class Pack
    {

    };

    namespace Detail
    {
        template <typename V, typename T>
        struct IsInPack;

        template <typename V, typename T0, typename... T>
        struct IsInPack <V, LD::Pack<T0, T...> >
        {
            static const bool value = IsInPack<typename LD::Detail::Decay<V>::type, LD::Pack<typename LD::Detail::Decay<T>::type...> >::value;
        };

        template <typename V, typename... T>
        struct IsInPack <V, LD::Pack<V, T...> >
        {
            static const bool value = true;
        };

        template <typename V>
        struct IsInPack <V, LD::Pack<> >
        {
            static const bool value = false;
        };


        template<typename T, typename U>
        struct UniqueInsert;


        template<template<class...> class VariantType, typename TypeToInsert, typename ... T>
        struct UniqueInsert<VariantType<T...>,TypeToInsert>
        {
        public:
           using type = typename LD::Detail::Conditonal<LD::Detail::IsInPack<LD::Decay_T<TypeToInsert>,LD::Pack<LD::Decay_T<T>...>>::value,LD::Pack<LD::Decay_T<T>...>,LD::Pack<LD::Decay_T<T>...,LD::Decay_T<TypeToInsert>>>::type ;
        };

        template<class A, template<class...> class B>
        struct rebind_;

        template<template<class...> class A, class... T, template<class...> class B>
        struct rebind_<A<T...>, B> {
            using type = B<T...>;
        };
    }

    template<class A, template<class...> class B>
    using Rebind = typename LD::Detail::rebind_<A, B>::type;
    //template<typename Variant, typename TypeToInsert>
    //using UniqueInsert = typename LD::Detail::UniqueInsert<Variant,TypeToInsert>::type ;
    //template<typename Variant, typename TypeToSearchFor>
    //constexpr bool IsInPack = LD::Detail::IsInPack<TypeToSearchFor,Variant>::value;

}
#endif //LANDESSDEVCORE_VARIADICPACK_H