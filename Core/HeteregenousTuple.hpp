//
//  HeteregenousTuple.h
//  DataStructures5
//
//  Created by James Landess on 2/22/19.
//  Copyright © 2019 James Landess. All rights reserved.
//

#ifndef HeteregenousTuple_h
#define HeteregenousTuple_h

#include "Definitions/Common.hpp"
#include "Memory/ElementReference.h"
#include "TypeTraits/Decay.hpp"
#include "TypeTraits/TypeList.hpp"
namespace LD
{
    namespace Detail
    {
        template<typename T, typename U>
        using assign_expression = decltype(LD::Declval<T&>() = LD::Declval<U&>());

        template<typename T, typename U>
        constexpr bool are_assignable = LD::Exists<assign_expression, T, U>;
    }



}
namespace LD
{
    template <typename...>
    inline constexpr auto IsUnique = LD::TrueType{};

    template <typename T, typename... Rest>
    inline constexpr auto IsUnique<T, Rest...> = LD::Detail::IntegralConstant<bool,(!LD::Detail::IsSame<T, Rest>::value && ...) && IsUnique<Rest...>
    >{};


    template<typename P0, typename P1, class = void>
    struct Assignable;

    template<typename ... P0s, typename ... P1s>
    struct Assignable<LD::VariadicPack<P0s...>,LD::VariadicPack<P1s...>,
            LD::Enable_If_T<
                    LD::Require<
                            sizeof...(P0s) == sizeof...(P1s),
                            LD::IsUnique<P0s...>,
                            LD::IsUnique<P1s...>
                    >>>: LD::Detail::IntegralConstant<bool, (LD::Detail::are_assignable<P0s, P1s> && ...)>
    {

    };

    template<typename ... P0s, typename ... P1s>
    struct Assignable<LD::VariadicPack<P0s...>,LD::VariadicPack<P1s...>,
            LD::Enable_If_T<
                    LD::Either<sizeof...(P0s) != sizeof...(P1s) , !LD::IsUnique<P0s...> , !LD::IsUnique<P1s...>>
            >>: LD::Detail::IntegralConstant<bool, false>
    {

    };



    namespace Detail
    {

        template<LD::UInteger Index ,typename T, typename U>
        constexpr void HeteregenousAssigneBase(T & a, const U & b) noexcept
        {
            a = LD::Get<T>(b);
        }



        template<typename ... A, typename ... B, LD::UInteger ... Index>
        constexpr void HeteregenousAssignRec(LD::IndexSequence<Index...>,LD::Tuple<A...> & a, const LD::Tuple<B...> & b) noexcept
        {
            (...,HeteregenousAssigneBase<Index>(LD::Get<Index>(a),b));
        }
    }

    /*
    template<typename ... A, typename ... B>
    PDP::Enable_If_T<LD::Assignable<PDP::VariadicPack<A...>, LD::VariadicPack<B...>>::value,PDP::Tuple<A...>&> HetergenousAssign(LD::Tuple<A...> & a, const LD::Tuple<B...> & b)
    {
        Detail::HeteregenousAssignRec(typename LD::MakeIndexSequence<sizeof...(A)>::type{},a,b);
        return a;
    }
    */


    //PDP::Enable_If_T<PDP::Contained<PDP::VariadicPack<A...>, PDP::VariadicPack<B...>>::value,PDP::Tuple<A...>&>
    template<typename ... A, typename ... B>
    constexpr LD::Enable_If_T<LD::Assignable<LD::VariadicPack<A...>, LD::VariadicPack<B...>>::value,LD::Tuple<A...>&> HetergenousAssign(LD::Tuple<A...> & a, const LD::Tuple<B...> & b) noexcept
    {
        Detail::HeteregenousAssignRec(typename LD::MakeIndexSequence<sizeof...(A)>::type{},a,b);
        return a;
    }


    namespace Detail
    {





        template<LD::UInteger Index ,typename T, typename U>
        constexpr void HeteregenousAssigneBase(ElementReference<T> & a, const U & b) noexcept
        {


            a = LD::Get<typename ElementReference<T>::ValueType>(b);
        }


        template<LD::UInteger Index ,typename T, typename U>
        constexpr void HeteregenousAssigneBase1(ElementReference<T> & a, const U & b)  noexcept
        {


            a = LD::Get<ElementReference<T>>(b);
        }



        template<typename ... A, typename ... B, LD::UInteger ... Index>
        constexpr void HeteregenousAssignRec(LD::IndexSequence<Index...>,LD::Tuple<ElementReference<A>...> & a, const LD::Tuple<B...> & b) noexcept

        {

            (...,HeteregenousAssigneBase<Index>(LD::Get<Index>(a),b));

        }
         template<typename ... A, typename ... B, LD::UInteger ... Index>
        constexpr void HeteregenousAssignRec(LD::IndexSequence<Index...>,LD::Tuple<ElementReference<A>...> & a, const LD::Tuple<ElementReference<B>...> & b) noexcept

        {

             (...,HeteregenousAssigneBase1<Index>(LD::Get<Index>(a),b));

        }

}

namespace utilities {
    template <typename T, typename Pack, typename... Ts> struct remove_first_found;

    template <typename T, template <typename...> class P,  typename... Ts>
    struct remove_first_found<T, P<>, Ts...> {
        using type = P<Ts...>;  // T was never found.
    };

    template <typename T, template <typename...> class P, typename... Rest, typename... Ts>
    struct remove_first_found<T, P<T, Rest...>, Ts...> {
        using type = P<Ts..., Rest...>;  // First T found and removed.
    };

    template <typename T, template <typename...> class P, typename First, typename... Rest, typename... Ts>
    struct remove_first_found<T, P<First, Rest...>, Ts...> : remove_first_found<T, P<Rest...>, Ts..., First> { };
}

template <typename T, std::size_t Count> struct num_types;

template <typename T, typename Pack, std::size_t Count, typename... Ts> struct get_num_types;

template <typename T, template <typename...> class P, std::size_t Count, typename... Ts>
struct get_num_types<T, P<>, Count, Ts...> {
    using type = num_types<T, Count>;
    using remaining = P<Ts...>;
};

template <typename T, template <typename...> class P, typename First, typename... Rest, std::size_t Count, typename... Ts>
struct get_num_types<T, P<First, Rest...>, Count, Ts...> : get_num_types<T, P<Rest...>, Count, Ts..., First> { };

template <typename T, template <typename...> class P, typename... Rest, std::size_t Count, typename... Ts>
struct get_num_types<T, P<T, Rest...>, Count, Ts...> : get_num_types<T, P<Rest...>, Count + 1, Ts...> { };

template <typename Pack, typename... NumTypes> struct count_types;

template <template <typename...> class P, typename... NumTypes>
struct count_types<P<>, NumTypes...> {
    using type = P<NumTypes...>;
};

template <template <typename...> class P, typename First, typename... Rest, typename... NumTypes>
struct count_types<P<First, Rest...>, NumTypes...> {
    using meta = get_num_types<First, P<Rest...>, 1>;
    using type = typename count_types<typename meta::remaining, NumTypes..., typename meta::type>::type;
};

template <typename Pack1, typename Pack2>
struct is_permutation_no_repeats : LD::FalseType { };

template <template <typename...> class P, template <typename...> class Q>
struct is_permutation_no_repeats<P<>, Q<>> : LD::TrueType { };

template <template <typename...> class P, typename First, typename... Rest, typename Pack2>
struct is_permutation_no_repeats<P<First, Rest...>, Pack2> {
    using meta = typename utilities::remove_first_found<First, Pack2>::type;
    static constexpr bool value = LD::Detail::IsSame<meta, Pack2>::value ? false : is_permutation_no_repeats<P<Rest...>, meta>::value;
};

template <typename Pack1, typename Pack2>
struct is_permutation : is_permutation_no_repeats<typename count_types<Pack1>::type, typename count_types<Pack2>::type> { };

template <typename Pack1, typename Pack2>
constexpr bool is_permutation_v = is_permutation<Pack1, Pack2>::value;

/*
template<typename ... A, typename ... B>
LD::Enable_If_T<LD::IsUnique<A...> && LD::IsUnique<B...> && LD::Assignable<LD::VariadicPack<ElementReference<A>...>, LD::VariadicPack<B...>>::value,LD::Tuple<LD::ElementReference<A>...>&> HetergenousAssign(LD::Tuple<LD::ElementReference<A>...> & a, const LD::Tuple<B...> & b)
{
    Detail::HeteregenousAssignRec(typename LD::MakeIndexSequence<sizeof...(A)>::type{},a,b);
    return a;
}
 */



/**
 @tparam Args - The types to which the HeteregenousTuple will represent
 @brief LD::HeteregenousTuple represents a set of tuples that can be assigned from or assigned to any instance of LD::HeteregenousTuple to which maintain a unique type list.  LD::HeteregenousTuple instances that have repeated types will not have any valid assignment operators or construction methods.
 */

template<typename T, typename U>
using AssignableExpression = decltype(LD::Declval<T>() = LD::Declval<U>());

template<typename T, typename U>
constexpr bool AreAssignable = LD::Exists<AssignableExpression, T, U>;

template<typename T, typename U, class = void>
struct IsUniquelyContained : public LD::FalseType {};

template<typename ... A, typename ... B>
struct IsUniquelyContained<LD::VariadicPack<A...>, LD::VariadicPack<B...>,LD::Enable_If_T<
        LD::Either<
                sizeof...(A) != sizeof...(B),
                !LD::IsUnique<A...>,
                !LD::IsUnique<B...>
        >>>
{
    static const bool value = false;
};

template<typename ... A, typename ... B>
struct IsUniquelyContained<LD::VariadicPack<A...>, LD::VariadicPack<B...>,LD::Enable_If_T<
        LD::Require<
                sizeof...(A) == sizeof...(B),
                LD::IsUnique<A...>,
                LD::IsUnique<B...>
        >>>
{
    static const bool value = LD::Detail::IntegralConstant<bool, (LD::IsInTuple<A, LD::VariadicPack<B...>>::value && ...)>::value;
};




namespace Detail
{
    template<LD::UInteger Index ,typename T, typename U>
    constexpr void HAssignBase(T & a, const U & b) noexcept
    {
        a = LD::Get<T>(b);
    }


    template<typename ... A, typename ... B, LD::UInteger ... Index>
    constexpr void HAssignRec(LD::IndexSequence<Index...>,LD::Tuple<A...> & a, const LD::Tuple<B...> & b) noexcept
    {
        (...,HAssignBase<Index>(LD::Get<Index>(a),b));
    }


    template<LD::UInteger Index ,typename T, typename U>
    constexpr void HReferenceAssignBase(ElementReference<T> & a, U && b) noexcept
    {
        a = LD::ElementReference<typename ElementReference<T>::ValueType>( LD::Get<typename ElementReference<T>::ValueType>(LD::Forward<U>(b)));
    }






    template<typename ... A, typename ... B, LD::UInteger ... Index>
    constexpr void HReferenceAssignRec(LD::IndexSequence<Index...>,LD::Tuple<ElementReference<A>...> & a, const LD::Tuple<B...> & b) noexcept

    {

         (...,HReferenceAssignBase<Index>(LD::Get<Index>(a),b));

    }
}



template<typename ... A, typename ... B>
constexpr LD::Enable_If_T<LD::Require<
        LD::is_permutation_v<LD::VariadicPack<typename LD::Decay<A>::type...>,LD::VariadicPack<typename LD::Decay<B>::type...>>
>,LD::Tuple<A...>> & InternalHAssign(LD::Tuple<A...> & a, const LD::Tuple<B...> & b) noexcept
{
    Detail::HeteregenousAssignRec(typename LD::MakeIndexSequence<sizeof...(A)>::type{},a,b);
    return a;
}



template<typename ... A, typename ... B>
constexpr LD::Enable_If_T<LD::Require<
        LD::is_permutation_v<LD::VariadicPack<A...>,LD::VariadicPack<typename LD::Decay<B>::type...>>
>,LD::Tuple<LD::ElementReference<A>...>> & InternalHReferenceAssign(LD::Tuple<LD::ElementReference<A>...> & a, const LD::Tuple<B...> & b) noexcept
{
    Detail::HReferenceAssignRec(typename LD::MakeIndexSequence<sizeof...(A)>::type{},a,b);
    return a;
}


template<typename ...A>
constexpr LD::Tuple<LD::ElementReference<typename LD::Decay<A>::type>...> MakeReferencedTuple(A & ... arguements) noexcept
{
    LD::Tuple<LD::ElementReference<typename LD::Decay<A>::type>...> referencedTuple;
    auto tiedTuple = LD::Tie(arguements...);

    InternalHReferenceAssign(referencedTuple,tiedTuple);

    //LD::HetergenousAssign(referencedTuple,tiedTuple);
    return referencedTuple;
}


template<typename ... Args>
class HeteregenousTuple
{
private:
    LD::Tuple<Args...> TupleHandle;
public:

    inline constexpr HeteregenousTuple() noexcept {}

    //Assignable<LD::VariadicPack<Args...>,LD::VariadicPack<A...>>::value
    template<typename ... A, typename = typename LD::Enable_If_T<
            LD::Require<
                    LD::Assignable<LD::VariadicPack<Args...>,LD::VariadicPack<A...>>::value
            >>>
    inline constexpr HeteregenousTuple(A && ... arguements) noexcept
    {


        auto tiedTuple = LD::Tie(arguements...);

        LD::HetergenousAssign(this->TupleHandle,tiedTuple);
    }


    //Assignable<LD::VariadicPack<Args...>,LD::VariadicPack<A...>>::value
    template<typename ... A, typename = typename LD::Enable_If_T<
            LD::Require<
                    LD::Assignable<LD::VariadicPack<Args...>,LD::VariadicPack<A...>>::value
            >>>
    inline constexpr HeteregenousTuple(const LD::Tuple<A...> & tuple) noexcept
    {
        LD::HetergenousAssign(this->TupleHandle,tuple);
        //(*this) = tuple;
    }


    //Assignable<LD::VariadicPack<Args...>,LD::VariadicPack<A...>>::value
    template<typename ... A, typename = LD::Enable_If_T<
            LD::Require<
                    LD::is_permutation_v<LD::VariadicPack<Args...>,LD::VariadicPack<A...>>
            >>>
    inline constexpr HeteregenousTuple(const LD::HeteregenousTuple<A...> & tuple) noexcept
    {
        //LD::HetergenousAssign(this->TupleHandle,tuple.GetTuple());
        InternalHAssign(this->TupleHandle,tuple.GetTuple());
        // (*this) = tuple;
    }



    template<typename ... A>
    inline constexpr LD::Enable_If_T<
            LD::Require<
                    LD::is_permutation_v<LD::VariadicPack<Args...>,LD::VariadicPack<A...>>
            >,HeteregenousTuple<Args...>> & operator = (const HeteregenousTuple<A...> & tuple) noexcept
    {
        LD::HetergenousAssign(this->TupleHandle,tuple());
        return (*this);
    }


    template<typename ... A>
    inline constexpr LD::Enable_If_T<
            LD::Require<
                    LD::is_permutation_v<LD::VariadicPack<Args...>,LD::VariadicPack<A...>>
            >,HeteregenousTuple<Args...>> & operator = (const LD::Tuple<A...> & tuple) noexcept
    {
        LD::HetergenousAssign(this->TupleHandle,tuple);
        return (*this);
    }



    constexpr LD::Tuple<Args...> & operator()() noexcept
    {
        return this->TupleHandle;
    }


    constexpr const LD::Tuple<Args...> & GetTuple() const noexcept
    {
        return this->TupleHandle;
    }
    constexpr const LD::Tuple<Args...> & operator()() const noexcept
    {
        return this->TupleHandle;
    }
};


    template<typename T, typename ... Args, typename CurrentTypeList = LD::CT::TypeList<Args...>>
    constexpr LD::Enable_If_T<LD::Require<
            (LD::GetTypeCountInTypeList<T,CurrentTypeList>::value == 1)
            >
    , T&>  Get(LD::HeteregenousTuple<Args...> & tuple) noexcept
    {
        return LD::Get<T>(tuple());
    }


    template<typename T, typename ... Args, typename CurrentTypeList = LD::CT::TypeList<Args...>>
    constexpr LD::Enable_If_T<LD::Require<
            (LD::GetTypeCountInTypeList<T,CurrentTypeList>::value == 1)
    >
            , const T&> Get(const LD::HeteregenousTuple<Args...> & tuple) noexcept
    {
        return LD::Get<T>(tuple());
    }


    template<LD::UInteger Index, typename ... Args, typename CurrentTypeList = LD::CT::TypeList<Args...>>
    constexpr LD::Enable_If_T<LD::Require<(Index < sizeof...(Args))>, typename LD::TypeAtIndex<Index,CurrentTypeList>::type> & Get(LD::HeteregenousTuple<Args...> & tuple) noexcept
    {
        return LD::Get<Index>(tuple());
    }

    template<LD::UInteger Index, typename ... Args, typename CurrentTypeList = LD::CT::TypeList<Args...>>
    const LD::Enable_If_T<LD::Require<(Index < sizeof...(Args))>,typename LD::TypeAtIndex<Index,CurrentTypeList>::type> & Get(const LD::HeteregenousTuple<Args...> & tuple) noexcept
    {
        return LD::Get<Index>(tuple());
    }

}
#endif /* HeteregenousTuple_h */
