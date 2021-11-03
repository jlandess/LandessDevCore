//
// Created by phoenixflower on 3/19/21.
//

#ifndef LANDESSDEVCORE_TUPLE_HPP
#define LANDESSDEVCORE_TUPLE_HPP
#include "Algorithms/Forward.hpp"
#include "Algorithms/Move.hpp"
#include "TypeTraits/IsVolatile.hpp"
#include "TypeTraits/TypeList.hpp"
#include "TypeTraits/Type.h"
namespace LD
{
    template<typename ... Args> class Tuple;

    namespace Detail
    {
        template<class... _Types> class TupleImpl;
        // empty tuple
        template<> class TupleImpl<> {};
        // recursive tuple definition
        template<class _This,
                class... _Rest>
        class TupleImpl<_This, _Rest...>
                : private TupleImpl<_Rest...>
        {
        public:
            _This _Myfirst;


            constexpr TupleImpl() noexcept {}
            constexpr TupleImpl(_This first, _Rest... rest) noexcept : TupleImpl<_Rest...>(rest...), _Myfirst(first) {}
        };

        // tuple_element
        template<LD::UInteger _Index, class _Tuple> struct TupleElement;

        // select first element
        template<class _This, class... _Rest>
        struct TupleElement<0, TupleImpl<_This, _Rest...>>
        {

            typedef _This& type;
            typedef TupleImpl<_This, _Rest...> _Ttype;

        };

        template<class _This, class... _Rest>
        struct TupleElement<0, Tuple<_This, _Rest...>>
        {
            typedef _This& type;
            typedef Tuple<_This, _Rest...> _Ttype;
        };

        // recursive tuple_element definition
        template <LD::UInteger _Index, class _This, class... _Rest>
        struct TupleElement<_Index, Tuple<_This, _Rest...>>
                : public TupleElement<_Index - 1, Tuple<_Rest...> >
        {

        };

        // recursive tuple_element definition
        template <LD::UInteger _Index, class _This, class... _Rest>
        struct TupleElement<_Index, TupleImpl<_This, _Rest...>>
                : public TupleElement<_Index - 1, TupleImpl<_Rest...> >
        {

        };

        template <class Tuple, class T, LD::UInteger Index = 0>
        struct FindFirstElementIndex;

        template <LD::UInteger Index, bool Valid>
        struct FindFirstElementIndexFinalTest
                : public LD::Detail::IntegralConstant<LD::UInteger, Index>
        {
        };

        template <LD::UInteger Index>
        struct FindFirstElementIndexFinalTest<Index, false>
        {
            static_assert(Index == -1, "Type not found");
        };

        /*
        template <typename T>
        struct Decay
        {
            template <typename U> static U impl(U);
            using type = decltype(impl(LD::Declval<T>()));
            //using type = typename LD::Detail::decay<T>::type;
        };
         */





//#if _LIBCPP_STD_VER > 11
        //template <class _Tp> using Decay_T = typename Decay<_Tp>::type;
//#endif


        template <class Head, class T, LD::UInteger Index>
        struct FindFirstElementIndex<Tuple<Head>, T, Index>
                : public FindFirstElementIndexFinalTest<Index, LD::Detail::IsSame<typename LD::Detail::Decay<Head>::type, T>::value>
        {
        };

        template <class Head, class ...Rest, class T, LD::UInteger Index>
        struct FindFirstElementIndex<Tuple<Head, Rest...>, T, Index>
                : public LD::Detail::Conditonal<LD::Detail::IsSame<typename LD::Detail::Decay<Head>::type, T>::value,
                        LD::Detail::IntegralConstant<LD::UInteger, Index>,
                        FindFirstElementIndex<Tuple<Rest...>, T, Index+1>>::type
        {

        };

        template<LD::UInteger index, typename ... Args>
        Tuple<Args...> & Assign(Tuple<Args...> & tuple)
        {
            return tuple;
        }
    }

    template<LD::UInteger _Index, class... _Types> inline
    constexpr typename Detail::TupleElement<_Index, Tuple<_Types...>>::type
    Get(Tuple<_Types...>& _Tuple);

    template<LD::UInteger _Index, class... _Types> inline
    constexpr const typename Detail::TupleElement<_Index, Tuple<_Types...>>::type
    Get(const Tuple<_Types...>& _Tuple);

    template<typename Type, class... _Types> inline
    constexpr Type & Get(Tuple<_Types...>& _Tuple);

    template<typename Type, class... _Types> inline
    constexpr const Type & Get(const Tuple<_Types...>& _Tuple);

    namespace Detail
    {
        template<LD::UInteger index, typename T  ,typename ... Args, typename ... A>
        Tuple<Args...> & Assign(Tuple<Args...> & tuple, T && a,  A && ... arguements)
        {
            Get<index>(tuple) = a;
            return Assign<index+1>(tuple,LD::Forward<A>(arguements)...);
        }

        template<LD::UInteger index,typename ... Args, typename ... A>
        Tuple<Args...> & Assign(Tuple<Args...> & tuple, A && ... arguements)
        {
            return LD::Detail::Assign<index>(tuple,LD::Forward<A>(arguements)...);
        }

        template<int ...S, typename ... Arguements, typename ... Args>
        void TupleAssign(Tuple<typename LD::Detail::Decay<Args>::type...> & tuple, Args && ... arguements)
        {

            LD::Detail::Assign<0>(tuple, LD::Forward<Args>(arguements)...);
        }

        template<typename T, typename U>
        void AssignTupleElementImpl(T & tupleElement,  U && element)
        {
            tupleElement = LD::Move(element);
        }

        template<typename T, typename U, typename ... Args>
        void AssignTupleElementImpl(T & tupleElement,  U && element, Args && ... arguements)
        {
            AssignTupleElementImpl(tupleElement,element,LD::Forward<Args>(arguements)...);
        }

        template<LD::UInteger ... Index, typename ... Args>
        void AssignTupleElement(LD::IndexSequence<Index...>,Tuple<typename LD::Detail::Decay<Args>::type...> & tuple, Args && ... arguements)
        {
            (...,AssignTupleElementImpl(Get<Index>(tuple),LD::Forward<Args>(arguements)));
        }

        template<typename ... Args>
        void AssignTuple(Tuple<typename LD::Detail::Decay<Args>::type...> & tuple, Args && ... arguements)
        {
            AssignTupleElement(typename LD::MakeIndexSequence<sizeof...(Args)>::type{},tuple,LD::Forward<Args>(arguements)...);
        }

        template<typename T, typename U>
        void AssignTupleElementImpl1(T && tupleElement,  U && element)
        {
            tupleElement = element;
        }

        template<typename T, typename U, typename ... Args>
        void AssignTupleElementImpl1(T && tupleElement,  U && element, Args && ... arguements)
        {
            AssignTupleElementImpl(tupleElement,element,LD::Forward<Args>(arguements)...);
        }

        template<LD::UInteger ... Index, typename ... Args, typename ... B>
        void AssignTupleElement1(LD::IndexSequence<Index...>,Tuple<Args...> & tuple, B && ... arguements)
        {
            (...,AssignTupleElementImpl1(Get<Index>(tuple),LD::Forward<Args>(arguements)));
        }

        template<typename ... Args, typename ... B>
        void AssignTuple1(Tuple<Args...> & tuple, B && ... arguements)
        {
            AssignTupleElement1(typename LD::MakeIndexSequence<sizeof...(Args)>::type{},tuple,LD::Forward<B>(arguements)...);
        }

        template<LD::UInteger Index, typename T, typename U>
        void AssignTupelFromTupleBase(T && a, const U & b)
        {
            Get<Index>(a) = Get<Index>(b);
        }

        template<typename T, typename U, LD::UInteger ... Index>
        void AssignTupleFromTupleRec(LD::IndexSequence<Index...>, T && a, const U & b)
        {
            (...,AssignTupelFromTupleBase<Index>(a,b));
        }
        template<typename ... A, typename ... B>
        void AssignTupleFromTuple(Tuple<A...> & a, const Tuple<B...> & b)
        {
            return AssignTupleFromTupleRec(typename LD::MakeIndexSequence<sizeof...(A)>::type{},a,b);
        }

        template<template<typename> class Functor ,typename ... Args, int ...S>
        void InternalInvoke(const Functor<void(Args...)> & function, const Tuple<typename LD::Detail::Decay<Args>::type...> & tuple,LD::IndexSequence<S...>)
        {
            function(Get<S>(tuple) ...);
        }
        template<template<typename> class Functor, typename T ,typename ... Args,int ...S>
        T InternalInvoke(const Functor<T(Args...)> & function, const Tuple<typename LD::Detail::Decay<Args>::type...> & tuple,LD::IndexSequence<S...>)
        {
            return LD::Move(function(Get<S>(tuple) ...));
        }

        template<typename T, typename F, LD::UInteger... Is>
        void for_each(T&& t, F f, LD::IndexSequence<Is...>)
        {
            auto l = { (f(Get<Is>(t)), 0)... };
        }

        template<typename T, typename F, typename U, template<typename> class Functor,  LD::UInteger... Is>
        void for_each(T&& t, F f,T & state, const Functor<U(const U & a, const U & b)> & accumulator, LD::IndexSequence<Is...>)
        {
            T retValue = LD::Move({ (f(Get<Is>(t)), 0)... });
            state = LD::Move(accumulator(state,retValue));
        }
    }


    template <typename P0, typename P1, typename Predicate = void>
    struct ArePacksConvertible;

    template <typename... T0s, typename... T1s>
    struct ArePacksConvertible<LD::VariadicPack<T0s...>, LD::VariadicPack<T1s...>,LD::Enable_If_T<(true && (sizeof...(T0s) == sizeof...(T1s)))>>
            : LD::Detail::IntegralConstant<bool,(sizeof...(T0s) == sizeof...(T1s))
                                                && (LD::Detail::IsConvertible<T0s, T1s>::value && ...)>
    {
    };

    template <typename... T0s, typename... T1s>
    struct ArePacksConvertible<LD::VariadicPack<T0s...>, LD::VariadicPack<T1s...>,LD::Enable_If_T<(true && (sizeof...(T0s) != sizeof...(T1s)))>>
            : LD::Detail::IntegralConstant<bool, false>
    {

    };


    template<typename ... Args>
    class Tuple: public Detail::TupleImpl<Args...>
    {
    public:
        inline constexpr Tuple() noexcept {}
        inline constexpr Tuple(Args && ... arguements) noexcept ;
        template <typename ... B ,typename = LD::Enable_If_T< LD::ArePacksConvertible<LD::VariadicPack<B...>, LD::VariadicPack<Args...>>::value>>
        explicit constexpr Tuple(const Tuple<B...> & tuple) noexcept
        {
            (*this) = tuple;

        }

        template <typename ... B ,typename = LD::Enable_If_T< LD::ArePacksConvertible<LD::VariadicPack<Args...>, LD::VariadicPack<B...>>::value>>
        constexpr Tuple<Args...> & operator = (const Tuple<B...> & tuple) noexcept
        {
            return Assignment(tuple);
        }

        template<typename ... B>
        constexpr LD::Enable_If_T< LD::ArePacksConvertible<LD::VariadicPack<Args...>, LD::VariadicPack<B...>>::value,Tuple<Args...> &> Assignment(const Tuple<B...> & tuple) noexcept ;
    };

    namespace Detail
    {
        template<typename T>
        struct IsTuple
        {
            static constexpr bool value = false;
        };

        template<typename ... T>
        struct IsTuple<LD::Tuple<T...>>
        {
            static constexpr bool value = true;
        };
    }

    template<typename T>
    constexpr bool IsTuple = LD::Detail::IsTuple<T>::value;

    template <> struct Tuple<> {};
// select first element
// get reference to _Index element of tuple
    template<LD::UInteger _Index, class... _Types> inline
    constexpr typename Detail::TupleElement<_Index, Tuple<_Types...>>::type
    Get(Tuple<_Types...>& _Tuple)
    {
        typedef typename Detail::TupleElement<_Index, Tuple<_Types...>>::_Ttype _Ttype;
        return (((_Ttype&) _Tuple)._Myfirst);
    }




    template<LD::UInteger _Index> inline
    constexpr void Get(Tuple<>& _Tuple)
    {

    }

// get reference to _Index element of tuple
    template<LD::UInteger _Index, class... _Types> inline
    constexpr const typename Detail::TupleElement<_Index, Tuple<_Types...>>::type
    Get(const Tuple<_Types...>& _Tuple)
    {
        typedef typename Detail::TupleElement<_Index, Tuple<_Types...>>::_Ttype _Ttype;
        return (((_Ttype&) _Tuple)._Myfirst);
    }

    template<LD::UInteger _Index> inline
    constexpr void Get(const Tuple<>& _Tuple)
    {

    }


    template<typename Type, class... _Types> inline
    constexpr Type & Get(Tuple<_Types...>& _Tuple)
    {
        return  Get<Detail::FindFirstElementIndex<Tuple<_Types...>, Type>::value>(_Tuple);
    }

    template<typename Type> inline
    constexpr void Get(Tuple<>& _Tuple)
    {

    }

    template<typename Type, class... _Types> inline
    constexpr const Type & Get(const Tuple<_Types...>& _Tuple)
    {
        return  Get<Detail::FindFirstElementIndex<Tuple<_Types...>, Type>::value>(_Tuple);
    }

    template<typename Type> inline
    constexpr void Get(const Tuple<>& _Tuple)
    {

    }

    template <class _Tp> class  TupleSize;

#if !defined(_LIBCPP_CXX03_LANG)
    template <class _Tp, class...>
    using __enable_if_tuple_size_imp = _Tp;

    template <class _Tp>
    class  TupleSize<__enable_if_tuple_size_imp<
            const _Tp,
            typename LD::EnableIf<!LD::Detail::IsVolatile<_Tp>::value>::type,
            LD::Detail::IntegralConstant<LD::UInteger, sizeof(TupleSize<_Tp>)>>>
            : public LD::Detail::IntegralConstant<LD::UInteger, TupleSize<_Tp>::value> {};

    template <class _Tp>
    class  TupleSize<__enable_if_tuple_size_imp<
            volatile _Tp,
            typename LD::EnableIf<!LD::Detail::IsConst<_Tp>::value>::type,
            LD::Detail::IntegralConstant<LD::UInteger, sizeof(TupleSize<_Tp>)>>>
            : public LD::Detail::IntegralConstant<LD::UInteger, TupleSize<_Tp>::value> {};

    template <class _Tp>
    class  TupleSize<__enable_if_tuple_size_imp<
            const volatile _Tp,
            LD::Detail::IntegralConstant<LD::UInteger, sizeof(TupleSize<_Tp>)>>>
            : public LD::Detail::IntegralConstant<LD::UInteger, TupleSize<_Tp>::value> {};

#else
    template <class _Tp> class  TupleSize<const _Tp> : public TupleSize<_Tp> {};
    template <class _Tp> class  TupleSize<volatile _Tp> : public TupleSize<_Tp> {};
    template <class _Tp> class  TupleSize<const volatile _Tp> : public TupleSize<_Tp> {};
#endif


    template <class ..._Tp>
    class  TupleSize<Tuple<_Tp...> >
            : public LD::Detail::IntegralConstant<LD::UInteger, sizeof...(_Tp)>
    {
    };


    template <>
    class  TupleSize<Tuple<> >
            : public LD::Detail::IntegralConstant<LD::UInteger, 0>
    {
    };


    template<typename ...Args>
    constexpr Tuple<typename LD::Detail::Decay<Args>::type...> MakeTuple(Args && ... arguements) noexcept
    {
        Tuple<typename LD::Detail::Decay<Args>::type...> tuple;
        Detail::AssignTuple(tuple, LD::Forward<Args>(arguements)...);
        return tuple;
    }


    template <class ..._Tp>
    inline constexpr Tuple<_Tp&...> Tie(_Tp&... __t) noexcept
    {
        return Tuple<_Tp&...>(__t...);
    }






    template <typename V, typename T>
    struct IsInTuple;

    template <typename V, typename T0, typename... T>
    struct IsInTuple <V, LD::VariadicPack<T0, T...> >
    {
        static const bool value = IsInTuple<typename LD::Detail::Decay<V>::type, LD::VariadicPack<typename LD::Detail::Decay<T>::type...> >::value;
    };

    template <typename V, typename... T>
    struct IsInTuple <V, LD::VariadicPack<V, T...> >
    {
        static const bool value = true;
    };

    template <typename V>
    struct IsInTuple <V, LD::VariadicPack<> >
    {
        static const bool value = false;
    };
    template <typename P0, typename P1>
    struct Contained;

    template <typename... T0s, typename... T1s>
    struct Contained<LD::VariadicPack<T0s...>, LD::VariadicPack<T1s...>>
            : LD::Detail::IntegralConstant<bool,(IsInTuple<typename LD::Detail::Decay<T0s>::type, LD::VariadicPack<typename LD::Detail::Decay<T1s>::type...>>::value && ...)>
    {

    };






    namespace Detail
    {
        template <LD::UInteger ...I, typename T1, typename T2>
        void CopyTupleImpl(T1 const & from, T2 & to, LD::IndexSequence<I...>)
        {
            int dummy[] = { (Get<I>(to) = Get<I>(from), 0)... };
            static_cast<void>(dummy);
        }

        template <typename T1, typename T2>
        void CopyTupleFromTo(T1 const & from, T2 & to)
        {
            CopyTupleImpl(
                    from, to,
                    typename LD::MakeIndexSequence<TupleSize<T1>::value>::type{});
        }


        template <typename Tuple1, LD::UInteger... Indices1, typename Tuple2, LD::UInteger... Indices2>
        auto tuple_cat1(Tuple1&& tup1, Tuple2&& tup2,
                        LD::IndexSequence<Indices1...>, LD::IndexSequence<Indices2...>)
        {
            return MakeTuple(
                    Get<Indices1>(LD::Forward<Tuple1>(tup1))...,
                    Get<Indices2>(LD::Forward<Tuple2>(tup2))...
            );
        }
        template<typename T, typename U, LD::UInteger ... INDEX>
        constexpr auto TupleAreEqualImpl(const T & a, const U & b, LD::IndexSequence<INDEX...> )-> bool
        {
            return (... && (Get<INDEX>(a) == Get<INDEX>(b)));
        }

        template<typename T, typename U, LD::UInteger ... INDEX>
        constexpr auto TupleAreLessImpl(const T & a, const U & b, LD::IndexSequence<INDEX...> )-> bool
        {
            return (... && (Get<INDEX>(a) < Get<INDEX>(b)));
        }

        template<typename T, typename U, char(*)[TupleSize<T>() * TupleSize<U>()] = nullptr>
        const bool TupleAreEqual (const T & a, const U & b)
        {
            return LD::Detail::TupleAreEqualImpl(a, b, typename LD::MakeIndexSequence<TupleSize<typename LD::Detail::Decay<T>::type>::value>::type{});
        }


        template<typename T, typename U, char(*)[TupleSize<T>() * TupleSize<U>()] = nullptr>
        const bool TupleAreLess (const T & a, const U & b)
        {
            return LD::Detail::TupleAreLessImpl(a, b, typename LD::MakeIndexSequence<TupleSize<typename LD::Detail::Decay<T>::type>::value>::type{});
        }


        template<typename T, typename U, LD::UInteger ... Index>
        void TupleSwapImpl(T & a, U & b, LD::IndexSequence<Index...> )
        {
            (...,LD::Swap(Get<Index>(a),Get<Index>(b)));
        }
    }

    template<typename ... A, typename ... B, char(*)[TupleSize<Tuple<A...>>() * TupleSize<Tuple<B...>>()] = nullptr>
    const bool operator == (const Tuple<A...> & a, const Tuple<B...> & b)
    {
        return Detail::TupleAreEqual(a,b);
    }

    template<typename ... A, typename ... B, char(*)[TupleSize<Tuple<A...>>() * TupleSize<Tuple<B...>>()] = nullptr>
    const bool operator != (const Tuple<A...> & a, const Tuple<B...> & b)
    {
        return !(Detail::TupleAreEqual(a,b));
    }

    template<typename ... A, typename ... B, char(*)[TupleSize<Tuple<A...>>() * TupleSize<Tuple<B...>>()] = nullptr>
    const bool operator <(const Tuple<A...> & a, const Tuple<B...> & b)
    {
        return Detail::TupleAreLess(a,b);
    }

    template<typename ... A, typename ... B, char(*)[TupleSize<Tuple<A...>>() * TupleSize<Tuple<B...>>()] = nullptr>
    const bool operator > (const Tuple<A...> & a, const Tuple<B...> & b)
    {
        return Detail::TupleAreLess(b,a);
    }

    template<typename ... A, typename ... B, char(*)[TupleSize<Tuple<A...>>() * TupleSize<Tuple<B...>>()] = nullptr>
    const bool operator >= (const Tuple<A...> & a, const Tuple<B...> & b)
    {
        return !Detail::TupleAreLess(a,b);
    }

    template<typename ... A, typename ... B, char(*)[TupleSize<Tuple<A...>>() * TupleSize<Tuple<B...>>()] = nullptr>
    const bool operator <= (const Tuple<A...> & a, const Tuple<B...> & b)
    {
        return !Detail::TupleAreLess(b,a);
    }

    template <typename Tuple1, typename Tuple2>
    auto ConcatenateTuple(Tuple1&& tup1, Tuple2&& tup2)
    {
        return Detail::tuple_cat1(
                LD::Forward<Tuple1>(tup1),
                LD::Forward<Tuple2>(tup2),
                typename LD::MakeIndexSequence<TupleSize<typename LD::Detail::Decay<Tuple1>::type>::value>::type{},
                typename LD::MakeIndexSequence<TupleSize<typename LD::Detail::Decay<Tuple2>::type>::value>::type{}
        );
    }

    template<typename ... Args>
    constexpr Tuple<Args...>::Tuple(Args && ... arguements) noexcept :Detail::TupleImpl<Args...>(arguements...)
    {
    }






    template<typename ... Args>
    template<typename ... B>
    constexpr LD::Enable_If_T< LD::ArePacksConvertible<LD::VariadicPack<Args...>, LD::VariadicPack<B...>>::value,Tuple<Args...> &> Tuple<Args...>::Assignment(const Tuple<B...> & tuple) noexcept
    {
        Detail::AssignTupleFromTuple(*this,tuple);
        return (*this);
    }
    template <class... _Tp>
    constexpr Tuple<_Tp&&...>
    ForwardAsTuple(_Tp&&... __t) noexcept
    {
        return Tuple<_Tp&&...>(LD::Forward<_Tp>(__t)...);
    }



/*
    template< class T >
    struct Decay {
        typedef typename LD::Detail::RemoveReference<T>::type U;
        typedef typename PDP::Conditional<
                LD::Detail::IsArray<U>::value,
                typename LD::Detail::RemoveExtent<U>::type*,
                typename PDP::Conditional<
                        LD::Detail::IsFunction<U>::value,
                        typename LD::Detail::add_pointer<U>::type,
                        typename LD::Detail::RemoveCV<U>::type
                >::type
        >::type type;
    };
    */

/*
template <typename T>
struct Decay
{
    template <typename U> static U impl(U);
    using type = decltype(impl(LD::Declval<T>()));
};
 */
}

namespace LD
{
    namespace CT
    {
        template<typename ... A>
        constexpr LD::CT::TypeList<A...> GetTupleTypes(LD::Type<LD::Tuple<A...>> ) noexcept
        {
            return LD::CT::TypeList<A...>{};
        }
    }

}
#endif //LANDESSDEVCORE_TUPLE_HPP