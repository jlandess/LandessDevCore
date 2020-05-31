// TypeList.hpp - definitions for typelists
// Project: TypeList
#ifndef LANDESSDEVCORE_VARIADICPACKLIST_H
#define LANDESSDEVCORE_VARIADICPACKLIST_H



//#include <type_traits>
#include "TypeTraits/Conditional.hpp"
#include "TypeTraits/IntegerSequence.hpp"
#include "TypeTraits/IntegralConstant.hpp"
#include "TypeTraits/IsSame.hpp"
#include "Definitions/Integer.hpp"
//#include <tuple>

#pragma once

namespace LD
{
    namespace CT
    {
        template<typename T> class DebugTemplate;

        namespace Detail
        {
            template<class A, template<class...> class B>
            struct RebindList;

            template<template<class...> class A, class... T, template<class...> class B>
            struct RebindList<A<T...>, B> {
                using type = B<T...>;
            };
        }

        template<typename A, template<typename ...> class B>
        using RebindList = typename LD::CT::Detail::RebindList<A,B>::type;

    }

}
namespace LD
{
//-----------------------------------------------------------------------------
/// Helper to use with is_tlist, is_iseq (borrowed)

    template <typename T>
    struct type_to_type
    {
        using orig = T;
        type_to_type() {}
    };

/////////////////////////////////////////////////////////////////////////////
/// Helpers to work with integer_seqyense

//-----------------------------------------------------------------------------
/// Is the class integer-sequence

    template <typename T>
    struct is_iseq
    {
    private:
        using yes = char(&)[1];
        using no  = char(&)[2];

        template <typename U, U... Is>
        static yes Test(type_to_type<LD::IntegerSequence<U, Is...>>) {}

        static no Test(...) {}

    public:
        using type = LD::Detail::IntegralConstant<bool, sizeof(Test(type_to_type<T>())) == 1>;
        using value_type = typename type::value_type;
        static constexpr value_type value = type::value;
    };

//-----------------------------------------------------------------------------
/// Get a number at the index

    template <LD::UInteger idx, class ISeq>
    struct iseq_nmb_at_impl;

    template <typename U, U head, U... Is>  /// End of search, number is found
    struct iseq_nmb_at_impl < 0, LD::IntegerSequence<U, head, Is...> >
    {

        using type = LD::Detail::IntegralConstant<U, head>;
    };

/// Recursion
    template <LD::UInteger idx, typename U, U head, U...Is>
    struct iseq_nmb_at_impl<idx, LD::IntegerSequence<U, head, Is...>>
    {
        using type = typename iseq_nmb_at_impl<idx - 1, LD::IntegerSequence<U, Is...>>::type;
    };

/// Wrapper; tests the index, has added 'value_type' and 'value' members
    template <LD::UInteger idx, class ISeg>
    struct iseq_nmb_at;

    template <LD::UInteger idx, typename U, U... Is>
    struct iseq_nmb_at<idx, LD::IntegerSequence<U, Is...>>
    {
    private:
        static_assert(LD::IntegerSequence<U, Is...>::size() > idx,
                      "iseq_nmb_at: Index is out of sequence bounds");
    public:
        using type = typename iseq_nmb_at_impl<idx, LD::IntegerSequence<U, Is...>>::type;
        using value_type = typename type::value_type;
        static constexpr value_type value = type::value;
    };

//-----------------------------------------------------------------------------
/// Get the index of a number

    template <LD::UInteger idx, typename U,U nmb, class ISeq>
    struct iseq_index_of_impl;

    template <LD::UInteger idx, typename U, U nmb>                  /// Number not the in sequence
    struct iseq_index_of_impl<idx, U, nmb, LD::IntegerSequence<U>>
    {
        using type = LD::Detail::IntegralConstant<int, -1>;        /// Stop recursion
    };

    template <LD::UInteger idx, typename U, U nmb, U... Ns>        /// Number found
    struct iseq_index_of_impl<idx, U, nmb, LD::IntegerSequence<U, nmb, Ns...>>
    {
        using type = LD::Detail::IntegralConstant<int, idx>;
    };

    template <LD::UInteger idx, typename U, U nmb, U head, U... Ns>
    struct iseq_index_of_impl<idx, U, nmb, LD::IntegerSequence<U, head, Ns...>>
    {
        using type = typename iseq_index_of_impl<idx + 1, U, nmb, LD::IntegerSequence<U, Ns...>>::type;
    };

/// Wrapper to test arguments, supply starting index 0, and add members 'value_type' and 'value'

    template <typename U, U nmb, class ISeq>
    struct iseq_index_of
    {
    private:
        static_assert(is_iseq<ISeq>::value,
                      "iseq_index_of: The class supplied is not std::integer_sequence");
        using data_type = typename ISeq::value_type;
        static_assert(LD::Detail::IsSame<U, data_type>::value,
                      "iseq_index_of: Number must be the same type as the sequence");
    public:
        using type = typename iseq_index_of_impl<0, data_type, nmb, ISeq>::type;
        using value_type = typename type::value_type;
        static constexpr value_type value = type::value;
    };

//-----------------------------------------------------------------------------
/// Remove some number from a sequence

    template <typename U, U nmb, class IntSeq1, class IntSeq2>
    struct iseq_remove_nmb_impl;

    template <typename U, U nmb, U... Ns>      /// Called on empty sequence or the number not found
    struct iseq_remove_nmb_impl<U, nmb, LD::IntegerSequence<U, Ns...>, LD::IntegerSequence<U>>
    {
        using type = LD::IntegerSequence<U, Ns...>;
    };

    template <typename U, U nmb, U... N1s, U... N2s>  // Number found and removed
    struct iseq_remove_nmb_impl<U, nmb, LD::IntegerSequence<U, N1s...>, LD::IntegerSequence<U, nmb, N2s...>>
    {
        using type = LD::IntegerSequence<U, N1s..., N2s...>;
    };

    template <typename U, U nmb, U head, U... N1s, U... N2s>
    struct iseq_remove_nmb_impl <U, nmb,LD::IntegerSequence<U, N1s...>, LD::IntegerSequence<U, head, N2s...>>
    {
        using type = typename iseq_remove_nmb_impl<U, nmb, LD::IntegerSequence<U, N1s..., head>,
                LD::IntegerSequence<U, N2s...>>::type;
    };

/// Wrapper

    template <typename U, U nmb, class ISeq>
    struct iseq_remove_nmb
    {
    private:
        static_assert ( is_iseq<ISeq>::value,
                        "iseq_remove_nmb: Supplied class is not integral_sequence");
        using data_type = typename ISeq::value_type;
        static_assert(LD::Detail::IsSame<U, data_type>::value,
                      "iseq_remove_nmb: Number must be of the type of sequence numbers");
    public:
        using type =
        typename iseq_remove_nmb_impl<data_type, nmb, LD::IntegerSequence<data_type>, ISeq>::type;
    };

//-----------------------------------------------------------------------------
/// Push a number back:
    template <typename T, T nmb, typename U, class ISeg>
    struct iseq_push_back_impl;

    template <typename T, T nmb, typename U, U... Ns>
    struct iseq_push_back_impl<T, nmb, U, LD::IntegerSequence<U, Ns...>>
    {
        using type =LD::IntegerSequence<U, Ns..., nmb>;
    };

/// Wrapper to check template arguments
    template <typename T, T nmb, class ISeq>
    struct iseq_push_back
    {
    private:
        static_assert(is_iseq<ISeq>::value,
                      "iseq_push_back: Supplied class is not std::integer_sequence");
        using data_type = typename ISeq::value_type;
        static_assert(LD::Detail::IsSame<T, data_type>::value,
                      "iseq_push_back: Number must be of the type of sequence numbers");
    public:
        using type = typename iseq_push_back_impl<T, nmb, data_type, ISeq>::type;
    };

//----------------------------------------------------------------------------
/// Get the sum of std::integer_sequence members

    template <typename U, U sum, class IntSeq>
    struct iseq_sum_impl;

    template <typename U, U sum>                               /// End of recursion
    struct iseq_sum_impl <U, sum, LD::IntegerSequence< U>>
    {
        using type = typename LD::Detail::IntegralConstant<U, sum>;
    };

    template <typename U, U sum, U head, U... Ns>             /// Recursion
    struct iseq_sum_impl < U, sum, LD::IntegerSequence<U, head, Ns...>>
    {
        using type = typename iseq_sum_impl<U, sum + head, LD::IntegerSequence<U, Ns...>>::type;
    };

    /// Wrapper to check template arguments and supply initial sum = 0
    template <class ISeq>
    struct iseq_sum
    {
    private:
        static_assert(is_iseq<ISeq>::value, "iseq_sum: Supplied class is not inteter_sequence");
        using data_type = typename ISeq::value_type;
    public:
        using type = typename iseq_sum_impl<data_type, 0, ISeq>::type;
        using value_type = typename type::value_type;
        static constexpr value_type value = type::value;
    };

///////////////////////////////////////////////////////////////////////////////
/// NullType and Sizeof to assign size zero to NullType

/// NullType definition
    class NullType
    {};

/// Helper: assigns size 0 to NullType, calculates sizeof(T) for others types

    template <typename T>
    struct Sizeof
    {
        static constexpr LD::UInteger value = LD::Detail::IsSame<T, NullType>::value ? 0 : sizeof(T);
    };

//-----------------------------------------------------------------------------
/// TypeList definition; defines size similar to  C++ STL containers size

    namespace CT
    {
        template <typename... Ts>
        struct TypeList
        {
            using type = TypeList;
            static constexpr LD::UInteger size() noexcept { return sizeof...(Ts); }
        };

        using tlist_emptylist = TypeList<>;
    }


//-----------------------------------------------------------------------------
/// Is the list empty? Has 'type', 'value_type' and 'value' members

    template <class List>
    struct IsTypeListEmpty
    {
        using type = LD::FalseType ; // integral_constant<bool, false>;
        using value_type = typename type::value_type;
        static constexpr value_type value = type::value;
    };

/// Specialization for an empty list

    template <>
    struct IsTypeListEmpty<CT::TypeList<>>
    {
        using type = LD::TrueType ; // integral_constant<bool, true>;
        using value_type = type::value_type;
        static constexpr value_type value = type::value;
    };

//-----------------------------------------------------------------------------
/// Is this type a tlist?

    template <typename T>
    struct IsTypeList
    {
    private:
        using yes = char(&)[1];
        using no  = char(&)[2];

        template <typename... Ts>
        static yes Test(type_to_type<CT::TypeList<Ts...>>) {}

        static no Test(...) {}

    public:
        using type = LD::Detail::IntegralConstant<bool, sizeof(Test(type_to_type<T>())) == 1>;
        using value_type = typename type::value_type;
        static constexpr value_type value = type::value;
    };

    //---------------------------------------------------------------------------
    ///Test whether all list's elements are TypeLists: used as a switch

    template <typename List>    // Is called if List is not tlist
    struct is_listoflists       // or on empty list
    {
        using type = LD::Detail::IntegralConstant<bool, false>;
        using value_type = typename type::value_type;
        static constexpr value_type value = type::value;
    };

    template <typename H, typename... ListEls>
    struct is_listoflists<LD::CT::TypeList<H, ListEls...>>
    {
    private:
        static constexpr bool bLst = IsTypeList<H>::value;
    public:
        using type = typename LD::Detail::Conditonal < bLst,
                typename is_listoflists<LD::CT::TypeList<ListEls...>>::type, LD::Detail::IntegralConstant<bool, false >> ::type;
        using value_type = typename type::value_type;            // Goes there only not tlist,stops
        static constexpr value_type value = type::value;
    };

    template <typename ListEl>                                 // Got to the end of original list
    struct is_listoflists<LD::CT::TypeList<ListEl>>
    {
    private:
        static constexpr bool bLst = IsTypeList<ListEl>::value;
    public:
        using type = LD::Detail::IntegralConstant<bool, bLst>;
        using value_type = typename type::value_type;
        static constexpr value_type value = type::value;
    };

//-----------------------------------------------------------------------------
/// Access an element by an index

    template <LD::UInteger idx, class List>
    struct tlist_type_at_impl;

    template <typename T, typename... Ts>  ///> End of search, type is found
    struct tlist_type_at_impl < 0, LD::CT::TypeList<T, Ts...> >
    {
        using type = T;
    };

/// Recursion
    template <LD::UInteger idx, typename T, typename...Ts>
    struct tlist_type_at_impl <idx, LD::CT::TypeList<T, Ts...>>
    {
        using type = typename tlist_type_at_impl<idx - 1, LD::CT::TypeList<Ts...>>::type;
    };

/// Wrapper
    template <LD::UInteger idx, class List>
    struct TypeAtIndex;

    template <LD::UInteger idx, typename... Ts>
    struct TypeAtIndex<idx, LD::CT::TypeList<Ts...>>
    {
    private:
        static_assert(sizeof...(Ts) > idx,
                      "tlist_type_at: Index out of bounds or called on empty list");
    public:
        using type = typename tlist_type_at_impl<idx, LD::CT::TypeList<Ts...>>::type;

    };

//-----------------------------------------------------------------------------
/// Search a typelist for a first occurrence of the type T

/// Implementation: has index as a template parameter
    template <LD::UInteger idx, typename T, class List>
    struct tlist_index_of_impl;

    template <LD::UInteger idx, typename T> /// The type T is not in the list
    struct tlist_index_of_impl <idx, T, LD::CT::TypeList<>>
    {
        using type = LD::Detail::IntegralConstant<int, -1>;
    };

    template <LD::UInteger idx, typename T, typename... Ts>    ///> The type is found
    struct tlist_index_of_impl <idx, T, LD::CT::TypeList<T, Ts...>>
    {
        using type = LD::Detail::IntegralConstant<int, idx>;
    };

    template <LD::UInteger idx, typename T, typename H, typename... Ts>  ///> Recursion
    struct tlist_index_of_impl <idx, T, LD::CT::TypeList<H, Ts...>>
    {
        using type = typename tlist_index_of_impl<idx + 1, T, LD::CT::TypeList<Ts...>>::type;
    };

/// Wrapping to supply initial index 0
    template <typename T, class List>
    struct tlist_index_of;

/// Specializing for idx >= 0
    template <typename T, typename... Ts>
    struct tlist_index_of<T, LD::CT::TypeList<Ts...>>
    {
        using type = typename tlist_index_of_impl<0, T, LD::CT::TypeList<Ts...>>::type;
        using value_type = typename type::value_type;
        static constexpr value_type value = type::value;
    };

//-----------------------------------------------------------------------------
/// Find the first element not equal to argument T

    template <LD::UInteger idx, typename T, class List>
    struct tlist_index_of_not_impl;

    template <LD::UInteger idx, typename T>             // All list's elements are T
    struct tlist_index_of_not_impl<idx, T, LD::CT::TypeList<>>
    {
        using type = LD::Detail::IntegralConstant<int, -1>;
    };

    template <LD::UInteger idx, typename T, typename H, typename... Ts>
    struct tlist_index_of_not_impl <idx, T, LD::CT::TypeList<H, Ts...>>
    {
        using type = typename LD::Detail::Conditonal < LD::Detail::IsSame<T, H>::value,
                typename tlist_index_of_not_impl<idx + 1, T, LD::CT::TypeList<Ts...>>::type,
                LD::Detail::IntegralConstant<int, idx >> ::type;
    };

/// Wrapper
    template <typename T, class List>
    struct tlist_index_of_not;

    template <typename T, typename... Ts>
    struct tlist_index_of_not<T, LD::CT::TypeList<Ts...>>
    {
        using type = typename tlist_index_of_not_impl<0, T, LD::CT::TypeList<Ts...>>::type;
        using value_type = typename type::value_type;
        static constexpr value_type value = type::value;
    };

//-----------------------------------------------------------------------------
/// Access to the front and the back of the list

    template <class Lists>
    struct tlist_front;

    template <typename... Ts>
    struct tlist_front<LD::CT::TypeList<Ts...>>
    {
    private:
        static_assert(sizeof...(Ts) > 0, "tlist_front: Called on empty list");
    public:
        using type = typename TypeAtIndex<0, LD::CT::TypeList<Ts...>>::type;
    };

    template <class List>
    struct tlist_back;

    template <typename... Ts>
    struct tlist_back<LD::CT::TypeList<Ts...>>
    {
    private:
        static_assert(sizeof...(Ts) > 0, "tlist_back: Called on empty list");
    public:
        using type = typename TypeAtIndex< (sizeof...(Ts)) - 1, LD::CT::TypeList<Ts...>>::type;
    };

//-----------------------------------------------------------------------------
/// Push front and push back


    namespace CT
    {
        namespace Detail
        {
            template <typename H, class List>
            struct Prepend; ;

            template <typename H, typename... Ts>
            struct Prepend<H, LD::CT::TypeList<Ts...>>
            {
                using type = LD::CT::TypeList<H, Ts...>;
            };
        }
    }


    namespace CT
    {
        template<typename H, typename ... Ts>
        using Prepend = typename LD::CT::Detail::Prepend<H,Ts...>::type;
    }



    template <typename T, class List>
    struct tlist_push_back;

    template <typename T, typename... Ts>
    struct tlist_push_back<T, LD::CT::TypeList<Ts...>>
    {
        using type = LD::CT::TypeList<Ts..., T>;
    };

//-----------------------------------------------------------------------------
/// Erase an element from the list

    template <typename T, class List>
    struct tlist_erase_type;

    template <typename T>
    struct tlist_erase_type<T, LD::CT::TypeList<>>
    {
        using type = LD::CT::TypeList<>;
    };

    template <typename T, typename... Ts>
    struct tlist_erase_type <T, LD::CT::TypeList<T, Ts...>>
    {
        using type = LD::CT::TypeList<Ts...>;
    };

    template <typename T, typename H, typename... Ts>
    struct tlist_erase_type<T, LD::CT::TypeList<H, Ts...>>
    {
        using type =
        typename LD::CT::Prepend<H, typename tlist_erase_type<T, LD::CT::TypeList<Ts...>>::type>::type;
    };

//-----------------------------------------------------------------------------
/// Erase an element from a typelist by its index

    template <LD::UInteger idx, class List>
    struct tlist_erase_at_impl;

    template <typename T, typename... Ts>
    struct tlist_erase_at_impl<0, LD::CT::TypeList<T, Ts...>>
    {
        using type = LD::CT::TypeList<Ts...>;
    };

    template <LD::UInteger idx, typename H, typename... Ts>
    struct tlist_erase_at_impl<idx, LD::CT::TypeList<H, Ts...>>
    {
        using type =
        typename LD::CT::Prepend < H, typename tlist_erase_at_impl<idx - 1, LD::CT::TypeList<Ts...>>::type>::type;
    };

/// Wrapper to assert index is in range
    template <LD::UInteger idx, class List>
    struct tlist_erase_at;

    template <LD::UInteger idx, typename... Ts>
    struct tlist_erase_at<idx, LD::CT::TypeList<Ts...>>
    {
    private:
        static_assert(sizeof...(Ts) > idx, "tlist_erase_at: Index out of bounds");
    public:
        using type = typename tlist_erase_at_impl<idx, LD::CT::TypeList<Ts...>>::type;
    };

//-----------------------------------------------------------------------------
/// Pop front, this can be called on empty list

    namespace CT
    {
        namespace Detail
        {
            template <class List>
            struct tlist_pop_front;

            template <typename... Ts>
            struct tlist_pop_front<LD::CT::TypeList<Ts...>>
            {
                using type = typename LD::Detail::Conditonal < sizeof...(Ts) == 0, LD::CT::TypeList<>,
                        typename  tlist_erase_at<0, LD::CT::TypeList<Ts...>>::type> ::type;
            };
        }

        template<typename T>
        using PopFront = typename LD::CT::Detail::tlist_pop_front<T>::type;
    }


//-----------------------------------------------------------------------------
/// Pop back, allows an empty list

    template <class List>
    struct tlist_pop_back;

    template <typename... Ts>
    struct tlist_pop_back<LD::CT::TypeList<Ts...>>
    {
        using type = typename LD::Detail::Conditonal<sizeof...(Ts) == 0, LD::CT::TypeList<>,
                typename tlist_erase_at<sizeof...(Ts)-1, LD::CT::TypeList<Ts...>>::type>::type;
    };

//-----------------------------------------------------------------------------
/// Erase all occurrence of type

    template <typename T, class List>
    struct tlist_erase_all;

    template <typename T>
    struct tlist_erase_all<T, LD::CT::TypeList<>>
    {
        using type = LD::CT::TypeList<>;
    };

    template <typename T, typename... Ts>
    struct tlist_erase_all<T, LD::CT::TypeList<T, Ts...>>
    {
        using type = typename tlist_erase_all<T, LD::CT::TypeList<Ts...>>::type;
    };

    template <typename T, typename H, typename... Ts>
    struct tlist_erase_all<T, LD::CT::TypeList<H, Ts...>>
    {
        using type = typename LD::CT::Prepend < H, typename tlist_erase_all<T, LD::CT::TypeList<Ts...>>::type>::type;
    };

//-----------------------------------------------------------------------------
/// Erase all elements of given type from a list of lists
/// Never use it as a standalone meta function, use tlist_deeperase_all instead

    template <typename T, class List>
    struct tlist_erase_all_ll
    {
        using type = LD::CT::TypeList<>;
    };

    template <typename T, typename... Hs, typename... Ts>
    struct tlist_erase_all_ll<T, LD::CT::TypeList<LD::CT::TypeList<Hs...>, Ts...>>
    {
    private:
        using el_cleaned = typename tlist_erase_all<T, LD::CT::TypeList<Hs...>>::type;
    public:
        using type =
        typename LD::CT::Prepend<el_cleaned, typename tlist_erase_all_ll<T, LD::CT::TypeList<Ts...>>::type>::type;
    };

//-----------------------------------------------------------------------------
/// Wrapper for deep erasure (inside each of list's elements)

    template <typename T, class List>
    struct tlist_deeperase_all;

    template <typename T, typename... ListEls>
    struct tlist_deeperase_all<T, LD::CT::TypeList<ListEls...>>
    {
    private:
        static constexpr bool bLL = is_listoflists <LD::CT::TypeList<ListEls...>>::value;
    public:
        using type = typename LD::Detail::Conditonal < bLL,
                typename tlist_erase_all_ll<T, LD::CT::TypeList<ListEls...>>::type,
                typename tlist_erase_all<T, LD::CT::TypeList<ListEls...>>::type>::type;
    };

//-----------------------------------------------------------------------------
/// Erase all duplicates from the list

    namespace CT
    {
        namespace Detail
        {
            template <class List>
            struct DeDuplicateTypeList;

            template <>
            struct DeDuplicateTypeList<LD::CT::TypeList<>>
            {
                using type = LD::CT::TypeList<>;
            };

            template <typename H, typename... Ts>
            struct DeDuplicateTypeList<LD::CT::TypeList<H, Ts...>>
            {
            private:
                using unique_t = typename DeDuplicateTypeList<LD::CT::TypeList<Ts...>>::type;
                using new_t = typename tlist_erase_type<H, unique_t>::type;
            public:
                using type = typename LD::CT::Prepend<H, new_t>::type;
            };
        }
        template<typename T>
        using DeDuplicateTypeList = typename LD::CT::Detail::DeDuplicateTypeList<T>::type;
    }




//---------------------------------------------------------------------------
/// Erase duplicates in all elements of list of lists
/// Not use it as a standalone meta function; use tlist_deeperase_dupl instead

    template <class List>
    struct tlist_erase_dupl_ll
    {
        using type = LD::CT::TypeList<>;
    };

    template <typename... Hs, typename... ListEls>
    struct tlist_erase_dupl_ll<LD::CT::TypeList<LD::CT::TypeList<Hs...>, ListEls...>>
    {
    private:
        using lst_clean = typename LD::CT::Detail::DeDuplicateTypeList < LD::CT::TypeList<Hs...>>::type;
    public:
        using type =
        typename LD::CT::Prepend<lst_clean,
                typename tlist_erase_dupl_ll<LD::CT::TypeList<ListEls...>>::type>::type;;
    };

//---------------------------------------------------------------------------
/// Wrapper for a deep erasure of duplicates

    template <class List>
    struct tlist_deeperase_dupl;

    template <typename... ListEls>
    struct tlist_deeperase_dupl<LD::CT::TypeList<ListEls...>>
    {
    private:
        static constexpr bool bLL = is_listoflists<LD::CT::TypeList<ListEls...>>::value;
    public:
        using type = typename LD::Detail::Conditonal < bLL,
                typename tlist_erase_dupl_ll<LD::CT::TypeList<ListEls...>>::type,
                typename LD::CT::Detail::DeDuplicateTypeList<LD::CT::TypeList<ListEls...>>::type>::type;
    };

//---------------------------------------------------------------------------
/// Erase first k elements from the list

    template <LD::UInteger k, class List>
    struct tlist_erase_firsts; //{};

    template <LD::UInteger k>
    struct tlist_erase_firsts<k, LD::CT::TypeList<>>
    {
        using type = LD::CT::TypeList<>;
    };

    template <typename... Ts>
    struct tlist_erase_firsts < 1, LD::CT::TypeList<Ts...>>
    {
        using type =  LD::CT::PopFront < LD::CT::TypeList<Ts...>>;
    };

    template <LD::UInteger k, typename... Ts>
    struct tlist_erase_firsts<k, LD::CT::TypeList<Ts...>>
    {
    private:
        static_assert(k > 0, "Wrong number of items to erase (k must be > 0)");
    public:
        using type = typename tlist_erase_firsts<k - 1, typename LD::CT::Detail::tlist_pop_front<LD::CT::TypeList<Ts...>>::type>::type;
    };

//-----------------------------------------------------------------------------
// Erase last k elements

    template <LD::UInteger k, class List>
    struct tlist_erase_lasts;

    template <LD::UInteger k>
    struct tlist_erase_lasts<k, LD::CT::TypeList<>>
    {
        using type = LD::CT::TypeList<>;
    };

    template <typename... Ts>
    struct tlist_erase_lasts<1, LD::CT::TypeList<Ts...>>
    {
        using type = typename tlist_pop_back<LD::CT::TypeList<Ts...>>::type;
    };

    template <LD::UInteger k, typename... Ts>
    struct tlist_erase_lasts<k, LD::CT::TypeList<Ts...>>
    {
    private:
        static_assert(k > 0, "Wrong number of items to erase (k must be > 0)");
    public:
        using type =
        typename tlist_erase_lasts<k - 1, typename tlist_pop_back<LD::CT::TypeList<Ts...>>::type>::type;
    };

//-----------------------------------------------------------------------------
/// Replace the first occurrence of the type in the typelist
/// R - replacement for the type T

    template <typename R, typename T, class List>
    struct tlist_replace_first; ;

    template <typename R, typename T>
    struct tlist_replace_first<R, T, LD::CT::TypeList<>>
    {
        using type = LD::CT::TypeList<>;
    };

    template <typename R, typename T, typename H, typename... Ts>
    struct tlist_replace_first<R, T, LD::CT::TypeList<H, Ts...>>
    {
        using type = typename LD::Detail::Conditonal<LD::Detail::IsSame<T, H>::value,
                LD::CT::TypeList<R, Ts...>,
                typename LD::CT::Prepend<H, typename tlist_replace_first<R, T, LD::CT::TypeList<Ts...>>::type>::type>::type;
    };

//-----------------------------------------------------------------------------
/// Replace a type at index; R is a replacement

    template <LD::UInteger idx, typename R, class List>
    struct tlist_replace_at_impl;

    template <typename R, typename H, typename... Ts>   // Index reached, type replaced
    struct tlist_replace_at_impl<0, R, LD::CT::TypeList<H, Ts...>>
    {
        using type = LD::CT::TypeList<R, Ts...>;
    };

    template <LD::UInteger idx, typename R, typename H, typename... Ts>
    struct tlist_replace_at_impl<idx, R, LD::CT::TypeList<H, Ts...>>  // Recursion
    {
        using type =
        typename LD::CT::Prepend< H, typename tlist_replace_at_impl<idx - 1, R, LD::CT::TypeList<Ts...>>::type>::type;
    };

/// Wrapper to check the index
    template <LD::UInteger idx, typename R, class List>
    struct tlist_replace_at;

    template <LD::UInteger idx, typename R, typename... Ts>
    struct tlist_replace_at<idx, R, LD::CT::TypeList<Ts...>>
    {
    private:
        static_assert(sizeof...(Ts) > idx, "tlist_replace_at: Index is out of bounds");
    public:
        using type = typename tlist_replace_at_impl<idx, R, LD::CT::TypeList<Ts...>>::type;
    };

//-----------------------------------------------------------------------------
/// Replace all occurrences of a given type in a tlist; R is a replacement for type T

    template <typename R, typename T, class List>
    struct tlist_replace_all;

    template <typename R, typename T>
    struct tlist_replace_all<R, T, LD::CT::TypeList<>>
    {
        using type = LD::CT::TypeList<>;
    };

    template <typename R, typename T, typename... Ts>
    struct tlist_replace_all<R, T, LD::CT::TypeList<T, Ts...>>
    {
        using type = typename LD::CT::Prepend<R, typename tlist_replace_all<R, T, LD::CT::TypeList<Ts...>>::type>::type;
    };

    template <typename R, typename T, typename H, typename... Ts>
    struct tlist_replace_all<R, T, LD::CT::TypeList<H, Ts...>>
    {
        using type = typename LD::CT::Prepend<H, typename tlist_replace_all<R, T, LD::CT::TypeList<Ts...>>::type>::type;
    };

//-----------------------------------------------------------------------------
/// Replace all occurrences of a type in a list of lists elements
/// Do not use it as a standalone meta function; use tlist_deepreplace_all instead

    template <typename R, typename T, class List>
    struct tlist_replace_all_ll
    {
        using type = LD::CT::TypeList<>;
    };

    template <typename R, typename T, typename... Hs, typename... ListEls>
    struct tlist_replace_all_ll<R, T, LD::CT::TypeList<LD::CT::TypeList<Hs...>, ListEls...>>
    {
    private:
        using lst_replaced = typename tlist_replace_all<R, T, LD::CT::TypeList<Hs...>>::type;
    public:
        using type =
        typename LD::CT::Prepend < lst_replaced,
                typename tlist_replace_all_ll<R, T, LD::CT::TypeList<ListEls...>>::type>::type;
    };

//-----------------------------------------------------------------------------
/// Wrapper to deep replacement

    template <typename R, typename T, class List>
    struct tlist_deepreplace_all;

    template <typename R, typename T, typename... ListEls>
    struct tlist_deepreplace_all<R, T, LD::CT::TypeList<ListEls...>>
    {
    private:
        static constexpr bool bLL = is_listoflists <LD::CT::TypeList<ListEls...>>::value;
    public:
        using type = typename LD::Detail::Conditonal<bLL,
                typename tlist_replace_all_ll<R, T, LD::CT::TypeList<ListEls...>>::type,
                typename tlist_replace_all<R, T, LD::CT::TypeList<ListEls...>>::type>::type;
    };

//-----------------------------------------------------------------------------
/// Count a type in the list

    template <LD::UInteger k, typename T, class List>
    struct tlist_count_type_impl;

    template <LD::UInteger k, typename T >
    struct tlist_count_type_impl<k, T, LD::CT::TypeList<>>
    {
        using type = LD::Detail::IntegralConstant<LD::UInteger , k>;
    };

    template <LD::UInteger k, typename T, typename H, typename... Ts>
    struct tlist_count_type_impl<k, T, LD::CT::TypeList<H, Ts...>>
    {
    private:
        static constexpr LD::UInteger count = LD::Detail::IsSame<T, H>::value ? k + 1 : k;
    public:
        using type = typename tlist_count_type_impl<count, T, LD::CT::TypeList<Ts...>>::type;
    };

/// Wrapper
    template <typename T, class Loist>
    struct GetTypeCountInTypeList;

    template <typename T, typename... Ts>
    struct GetTypeCountInTypeList<T, LD::CT::TypeList<Ts...>>
    {
        using type = typename tlist_count_type_impl<0, T, LD::CT::TypeList<Ts...>>::type;
        using value_type = typename type::value_type;
        static constexpr value_type value = type::value;
    };

//---------------------------------------------------------------------------
/// Count types in the list of lists
/// Do not use it as a standalone meta function; use tlist_deepcount_type instead

    template <typename T, class ISeq, class List>
    struct tlist_count_type_ll
    {
        using type = LD::IntegerSequence<LD::UInteger>;
    };

    template <typename T, LD::UInteger ... Rs, typename... Hs >
    struct tlist_count_type_ll<T, LD::IntegerSequence<LD::UInteger , Rs...>, LD::CT::TypeList<LD::CT::TypeList<Hs...>>>
    {
    private:
        static constexpr LD::UInteger res_cnt = GetTypeCountInTypeList<T, LD::CT::TypeList<Hs...>>::value;
    public:
        using type = LD::IntegerSequence<LD::UInteger , Rs..., res_cnt>;
    };

    template <typename T, LD::UInteger ... Rs, typename... Hs, typename... Ls >
    struct tlist_count_type_ll<T, LD::IntegerSequence<LD::UInteger , Rs...>, LD::CT::TypeList<LD::CT::TypeList<Hs...>, Ls...>>
    {
    private:
        static constexpr LD::UInteger res_cnt = GetTypeCountInTypeList<T, LD::CT::TypeList<Hs...>>::value;
    public:
        using type = typename tlist_count_type_ll<T, LD::IntegerSequence<LD::UInteger , Rs..., res_cnt>, LD::CT::TypeList<Ls...>>::type;
    };

//-----------------------------------------------------------------------------
/// Wrap it; result is std::integral_constant<size_t, count> for plain list
/// For the list of lists it is std::integer_sequence<size_t, counts...>

    template <typename T, class List>
    struct tlist_deepcount_type;

    template <typename T, typename... Ls>
    struct tlist_deepcount_type<T, LD::CT::TypeList<Ls...>>
    {
/// Condition to select path
    private:
        static constexpr bool bLL = is_listoflists<LD::CT::TypeList<Ls...>>::value;
    public:
        using type = typename LD::Detail::Conditonal<bLL,
                typename tlist_count_type_ll<T, LD::IntegerSequence<LD::UInteger>, LD::CT::TypeList<Ls...>>::type,
                typename GetTypeCountInTypeList<T, LD::CT::TypeList<Ls...>>::type>::type;
    };

//-----------------------------------------------------------------------------
/// Get all indexes of a type in a typelist; the result is a std::integer_sequence
/// Implementation

    template <LD::UInteger k, typename T, class List, class IntSeq>
    struct tlist_all_idx_impl;

    template <LD::UInteger k, typename T, LD::UInteger ... Idx>                    // End of search
    struct tlist_all_idx_impl<k, T, LD::CT::TypeList<>, LD::IntegerSequence<LD::UInteger , Idx...>>
    {
        using type = LD::IntegerSequence<LD::UInteger , Idx...>;
    };

    template <LD::UInteger k, typename T, typename... Ts, LD::UInteger ... Idx>    // Type found at k
    struct tlist_all_idx_impl<k, T, LD::CT::TypeList<T, Ts...>, LD::IntegerSequence<LD::UInteger , Idx...>>
    {
        using type =
        typename tlist_all_idx_impl<k + 1, T, LD::CT::TypeList<Ts...>, LD::IntegerSequence<LD::UInteger , Idx..., k>>::type;
    };

    template <LD::UInteger k, typename T, typename H, typename... Ts, LD::UInteger ... Idx> // Different type, continue
    struct tlist_all_idx_impl<k, T, LD::CT::TypeList<H, Ts...>, LD::IntegerSequence<LD::UInteger , Idx...>>
    {
        using type =
        typename tlist_all_idx_impl<k + 1, T, LD::CT::TypeList<Ts...>, LD::IntegerSequence<LD::UInteger , Idx...>>::type;
    };

/// Wrapper;; Supply the start index zero
    template <typename T, class List>
    struct tlist_all_idx;

    template <typename T, typename... Ts>
    struct tlist_all_idx<T, LD::CT::TypeList<Ts...>>
    {
        using type = typename tlist_all_idx_impl<0, T, LD::CT::TypeList<Ts...>, LD::IntegerSequence<LD::UInteger>>::type;
    };

//-----------------------------------------------------------------------------
/// Get all indexes of a type in each element of a list of lists
/// Do not use it as a standalone meta function; use tlist_all_deepidx instead

    template <typename T, class Res, class List>
    struct tlist_all_idx_ll
    {
        using type = LD::CT::TypeList<LD::IntegerSequence<LD::UInteger>>;          // Will never called
    };

    template <typename T, typename... Rs, typename... Hs>         // End of search
    struct tlist_all_idx_ll<T, LD::CT::TypeList<Rs...>, LD::CT::TypeList<LD::CT::TypeList<Hs...>>>
    {
    private:
        using iseq = typename tlist_all_idx_impl<0, T, LD::CT::TypeList<Hs...>, LD::IntegerSequence<LD::UInteger>>::type;
    public:
        using type = LD::CT::TypeList<Rs..., iseq>;
    };

    template <typename T, typename... Rs, typename... Hs, typename... ListEls>
    struct tlist_all_idx_ll<T, LD::CT::TypeList<Rs...>, LD::CT::TypeList<LD::CT::TypeList<Hs...>, ListEls...>>
    {
    private:
        using iseq = typename tlist_all_idx_impl<0, T, LD::CT::TypeList<Hs...>, LD::IntegerSequence<LD::UInteger>>::type;
    public:
        using type =
        typename tlist_all_idx_ll<T, LD::CT::TypeList<Rs..., iseq>, LD::CT::TypeList<ListEls...>>::type;
    };

//-----------------------------------------------------------------------------
/// Wrapper: find all indexes of the given type in a list or in elements of list of lists

    template <typename T, class List>
    struct tlist_all_deepidx;

    template <typename T, typename... ListEls>
    struct tlist_all_deepidx<T, LD::CT::TypeList<ListEls...>>
    {
    private:
        static constexpr bool bLL = is_listoflists < LD::CT::TypeList<ListEls...>>::value;
    public:
        using type = typename LD::Detail::Conditonal < bLL,
                typename tlist_all_idx_ll<T, LD::CT::TypeList<>, LD::CT::TypeList<ListEls...>>::type,
                typename tlist_all_idx_impl<0, T, LD::CT::TypeList<ListEls...>, LD::IntegerSequence<LD::UInteger>>::type>::type;
    };

//-----------------------------------------------------------------------------
/// Helper: Check whether all elements of a list are std::integer_sequences

    template <class List>
    struct is_listofiseqs                     // Called if the class is not a tlist
    {
        using type = LD::Detail::IntegralConstant<bool, false>;
        using value_type = typename type::value_type;
        static constexpr value_type value = type::value;
    };

    template <>
    struct is_listofiseqs<LD::CT::TypeList<>>
    {
        using type = LD::Detail::IntegralConstant<bool, false>;
        using value_type = type::value_type;
        static constexpr value_type value = type::value;
    };

    template <typename H>
    struct is_listofiseqs<LD::CT::TypeList<H>>
    {
    private:
        static constexpr bool bISeq = is_iseq<H>::value;
    public:
        using type = LD::Detail::IntegralConstant<bool, bISeq>;
        using value_type = typename type::value_type;
        static constexpr value_type value = type::value;
    };

    template <typename H, typename... ISeqs>
    struct is_listofiseqs<LD::CT::TypeList<H, ISeqs...>>
    {
    private:
        static constexpr bool bISeq = is_iseq<H>::value;
    public:
        using type = typename LD::Detail::Conditonal < bISeq,
                typename is_listofiseqs<LD::CT::TypeList<ISeqs...>>::type, LD::Detail::IntegralConstant<bool, false>>::type;
        using value_type = typename type::value_type;
        static constexpr value_type value = type::value;
    };

//-----------------------------------------------------------------------------
/// Reverse order of members of the list implementation: needs empty list as destiny

    template <class Src, class Rsvr>
    struct tlist_reverse_impl;

    template <typename... Rs>
    struct tlist_reverse_impl<LD::CT::TypeList<>, LD::CT::TypeList<Rs...>>
    {
        using type = LD::CT::TypeList<Rs...>;
    };

    template <class H, class... Rs >
    struct tlist_reverse_impl<LD::CT::TypeList<H>, LD::CT::TypeList<Rs...>>
    {
        using type = LD::CT::TypeList <H, Rs... >;
    };

    template <class H, class... Ts, class... Rs>
    struct tlist_reverse_impl<LD::CT::TypeList<H, Ts...>, LD::CT::TypeList<Rs...>>
    {
        using type = typename tlist_reverse_impl <  LD::CT::TypeList<Ts...>, LD::CT::TypeList<H, Rs...>>::type;
    };

/// Wrapper to hide starting empty resulting list inside the wrapper

    template <class List>
    struct tlist_reverse;

    template <typename... Ts>
    struct tlist_reverse<LD::CT::TypeList<Ts...>>
    {
        using type = typename tlist_reverse_impl<LD::CT::TypeList<Ts...>, LD::CT::TypeList<>>::type;
    };


//---------------------------------------------------------------------------
/// Concatenate typelists

/// Implementation
    template <class ResList, class OrigLists>
    struct tlist_concat_lists_impl;

    template <typename... Rs, typename... Ts>                     // Last OrigList
    struct tlist_concat_lists_impl<LD::CT::TypeList<Rs...>, LD::CT::TypeList<Ts...>>
    {
        using type = LD::CT::TypeList<Rs..., Ts...>;
    };

    template <typename... Rs, typename... Ts, typename... Rests>  // Recursion
    struct tlist_concat_lists_impl<LD::CT::TypeList<Rs...>, LD::CT::TypeList<LD::CT::TypeList<Ts...>, Rests...>>
    {
        using type = typename tlist_concat_lists_impl<LD::CT::TypeList<Rs..., Ts...>, LD::CT::TypeList<Rests...>>::type;
    };

/// Flatten a list of lists (convert it to a plain list) - is used as standalone and in a wrapper
    template <class ListOfLists>
    struct tlist_flatten_list;

    template <typename... Lists>
    struct tlist_flatten_list<LD::CT::TypeList<Lists...>>
    {
    private:
        static_assert(is_listoflists<LD::CT::TypeList<Lists...>>::value,
                      "tlist_flatten: Argument is not a tlist of lists");
    public:
        using type = typename tlist_concat_lists_impl<LD::CT::TypeList<>, LD::CT::TypeList<Lists...>>::type;
    };

    template <typename... Lists>
    struct tlist_concat_lists
    {
    private:
        using temp_list = LD::CT::TypeList<Lists...>;
        static_assert(is_listoflists<temp_list>::value, "Not all arguments are lists");
    public:
        using type = typename tlist_flatten_list<temp_list>::type;
    };

//-----------------------------------------------------------------------------
/// Helper: Move first k elements from list L2 to the tail of the list T1
/// Type is the list of two lists, one with added elements, other with a rest

    template <LD::UInteger k, class Lst1, class Lst2>
    struct tlist_move_firsts;

    template <typename H, typename... T1s, typename... T2s>
    struct tlist_move_firsts<1, LD::CT::TypeList<T1s...>, LD::CT::TypeList<H, T2s...>>
    {
        using type = LD::CT::TypeList < LD::CT::TypeList<T1s..., H>, LD::CT::TypeList<T2s... >>;
    };

    template <LD::UInteger k, typename H, typename... T1s, typename... T2s>
    struct tlist_move_firsts<k, LD::CT::TypeList<T1s...>, LD::CT::TypeList<H, T2s...>>
    {
        static_assert (k > 0, "Number of elements to transfer must be > 0");
        using type = typename tlist_move_firsts<k - 1, LD::CT::TypeList<T1s..., H>, LD::CT::TypeList<T2s...>>::type;
    };

//---------------------------------------------------------------------------
/// Split a TypeList into multiple lists; in tlist_split_impl numbers of elements
//  in lists are in integer sequence

    template <class Idx, class List1, class List2>
    struct tlist_split_impl;

    template <LD::UInteger l, typename... Lists, typename... Tails>
    struct tlist_split_impl<LD::IntegerSequence<LD::UInteger , l>, LD::CT::TypeList<Lists...>, LD::CT::TypeList<Tails...>>
    {
    private:
        using lst_split_2 = typename tlist_move_firsts<l, LD::CT::TypeList<>, LD::CT::TypeList<Tails...>>::type;
        using first = typename TypeAtIndex<0, lst_split_2>::type;
        using second = typename TypeAtIndex<1, lst_split_2>::type;
    public:
        using type = LD::CT::TypeList<Lists..., first, second>;
    };

    template <LD::UInteger l, LD::UInteger ... Ls, typename ... Lists, typename... Tails>
    struct tlist_split_impl < LD::IntegerSequence<LD::UInteger , l, Ls...>, LD::CT::TypeList<Lists...>, LD::CT::TypeList<Tails...>>
    {
    private:
        using lst_split_2 = typename tlist_move_firsts<l, LD::CT::TypeList<>, LD::CT::TypeList<Tails...>>::type;
        using first = typename TypeAtIndex<0, lst_split_2>::type;
        using second = typename TypeAtIndex<1, lst_split_2>::type;
        using lst_res = LD::CT::TypeList<Lists..., first>;
    public:
        using type = typename tlist_split_impl<LD::IntegerSequence<LD::UInteger , Ls...>, lst_res, second>::type;
    };

/// Wrapper to save the user from generating std::integer_sequence; will do it inside the structure

    template < class List, LD::UInteger ... Ls>
    struct tlist_split;

    template <typename... Ts, LD::UInteger ... Ls>
    struct tlist_split<LD::CT::TypeList<Ts...>, Ls...>
    {
    private:
// Check the sum of component lengths; must be less than size of original list
        using lengths = LD::IntegerSequence<LD::UInteger , Ls...>;
        using lengths_sum = typename iseq_sum<lengths>::type;
        static_assert(lengths_sum::value < LD::CT::TypeList<Ts...>::size(),
                      "tlist_split: Length of a last component must be greater than zero; check the sum of component sizes");
// No zero lengths are allowed
        static_assert(iseq_index_of<LD::UInteger , 0, lengths>::value == -1,
                      "Multi_Split: Remove components with zero size from the sequence of sizes");
    public:   // Go
        using type = typename tlist_split_impl<LD::IntegerSequence<LD::UInteger , Ls...>, LD::CT::TypeList<>, LD::CT::TypeList<Ts...>>::type;
    };

//-----------------------------------------------------------------------------
/// tlist_max_type_impl gets first element of max size in a plain list

    template <typename T, class List>
    struct tlist_max_type_impl;

    template <typename T>                   // End of recursion
    struct tlist_max_type_impl<T, LD::CT::TypeList<>>
    {
        using max_type = T;
    };

    template <typename T, typename H, typename... Ts>
    struct tlist_max_type_impl<T, LD::CT::TypeList<H, Ts...>>
    {
    private:
        static constexpr bool  bRes = Sizeof<T>::value >= Sizeof<H>::value ? true : false;
        using selected = typename LD::Detail::Conditonal<bRes, T, H>::type;
    public:
        using max_type = typename tlist_max_type_impl<selected, LD::CT::TypeList<Ts...>>::max_type;
    };

/// Wrapper to apply to a plain list (Starts from a NullType, size = 0
    template <class List>
    struct tlist_max_type_size;

    template <typename... Ts>
    struct tlist_max_type_size<LD::CT::TypeList<Ts...>>
    {
    private:
        using max_type = typename tlist_max_type_impl < NullType, LD::CT::TypeList<Ts... >> ::max_type;
    public:
        using type = LD::Detail::IntegralConstant<LD::UInteger , Sizeof<max_type>::value>;
        using value_type = typename type::value_type;
        static constexpr value_type value = type::value;
    };

//-----------------------------------------------------------------------------
/// Find max size of elements in each list of a list of lists

    template <class ISeq, class List>
    struct tlist_max_type_size_ll    // Never executed, but needed for std::std::conditional
    {
        using type = LD::IntegerSequence<LD::UInteger>;
    };

    template <LD::UInteger ... Ids, typename... Hs>             // End of recursion
    struct tlist_max_type_size_ll<LD::IntegerSequence<LD::UInteger , Ids...>, LD::CT::TypeList<LD::CT::TypeList<Hs...>>>
    {
    private:
        static constexpr LD::UInteger loc_max = tlist_max_type_size<LD::CT::TypeList<Hs...>>::value;
    public:
        using type = LD::IntegerSequence<LD::UInteger , Ids..., loc_max>;
    };

    template <LD::UInteger ... Ids, typename... Hs, typename... ListEls>
    struct tlist_max_type_size_ll<LD::IntegerSequence<LD::UInteger , Ids...>, LD::CT::TypeList<LD::CT::TypeList<Hs...>, ListEls...>>
    {
    private:
        static constexpr LD::UInteger loc_max = tlist_max_type_size<LD::CT::TypeList<Hs...>>::value;
    public:
        using type =
        typename tlist_max_type_size_ll<LD::IntegerSequence<LD::UInteger , Ids..., loc_max>, LD::CT::TypeList<ListEls...>>::type;
    };

//-----------------------------------------------------------------------------
/// Wrapper: find max size of elements in a plain list or max elements in
///a list of lists

    template <class List>
    struct tlist_max_type_deepsize;

    template <typename... ListEls>
    struct tlist_max_type_deepsize<LD::CT::TypeList<ListEls...>>
    {
    private:
        static constexpr bool bLL = is_listoflists<LD::CT::TypeList<ListEls...>>::value;
    public:
        using type = typename LD::Detail::Conditonal < bLL,
                typename tlist_max_type_size_ll < LD::IntegerSequence<LD::UInteger>, LD::CT::TypeList<ListEls...>>::type,
                typename tlist_max_type_size<LD::CT::TypeList<ListEls...>>::type > ::type;
    };

//-----------------------------------------------------------------------------
/// Find the first element of given size in the list of mixed element types

    template <LD::UInteger k, class List>
    struct tlist_firsttype_of_size;

    template <LD::UInteger k>
    struct tlist_firsttype_of_size<k, LD::CT::TypeList<>>
    {
        using type = LD::FalseType ;
    };

    template <LD::UInteger k, typename H, typename... Ts>
    struct tlist_firsttype_of_size< k, LD::CT::TypeList<H, Ts...>>
    {
        using type =
        typename LD::Detail::Conditonal < Sizeof<H>::value == k, H,
                typename tlist_firsttype_of_size<k, LD::CT::TypeList<Ts...>>::type>::type;
    };

//------------------------------------------------------------------------------
/// Find the first element of given size in each list of a list of lists; result is a tlist

    template <LD::UInteger k, class ResLst, class List> /// It is only to compile;
    struct tlist_firstltype_of_size                /// a branch in the wrapper
    {                                             /// is never called
        using type = LD::CT::TypeList<>;
    };


    template <LD::UInteger k, typename... Rs, typename... Hs>           // Stop recursion
    struct tlist_firstltype_of_size<k, LD::CT::TypeList<Rs...>, LD::CT::TypeList<LD::CT::TypeList<Hs...>>>
    {
    private:
        using type_found = typename tlist_firsttype_of_size<k, LD::CT::TypeList<Hs...>>::type;
    public:
        using type = LD::CT::TypeList<Rs..., type_found>;
    };

    template <LD::UInteger k, typename... Rs, typename... Hs, typename... Ts>
    struct tlist_firstltype_of_size < k, LD::CT::TypeList <Rs...>, LD::CT::TypeList<LD::CT::TypeList<Hs...>, Ts...>>
    {
    private:
        using type_found = typename tlist_firsttype_of_size < k, LD::CT::TypeList<Hs...>>::type;
    public:
        using type = typename tlist_firstltype_of_size<k, LD::CT::TypeList<Rs..., type_found>, LD::CT::TypeList<Ts...>>::type;
    };

//---------------------------------------------------------------------------
/// Wrapper

    template <LD::UInteger k, class List>
    struct tlist_first_typedeep_of_size;

    template <LD::UInteger k, typename... ListEls>
    struct tlist_first_typedeep_of_size<k, LD::CT::TypeList<ListEls...>>
    {
    private:
        static constexpr bool bLs = is_listoflists<LD::CT::TypeList<ListEls...>>::value;
    public:
        using type = typename LD::Detail::Conditonal < bLs,
                typename tlist_firstltype_of_size < k, LD::CT::TypeList<>, LD::CT::TypeList<ListEls...>>::type,
                typename tlist_firsttype_of_size<k, LD::CT::TypeList<ListEls...>>::type>::type;
    };

//-----------------------------------------------------------------------------
/// Get all types of given size from the plain list of mixed elements, no duplicates

    template <LD::UInteger k, class ListRes, class ListSoource>
    struct tlist_all_ttypes_of_size;

    template <LD::UInteger k, typename... Rs>
    struct tlist_all_ttypes_of_size<k, LD::CT::TypeList<Rs...>, LD::CT::TypeList<>>
    {
        using type = typename LD::CT::Detail::DeDuplicateTypeList<LD::CT::TypeList<Rs...>>::type;
    };

    template <LD::UInteger k, typename H, typename... Rs, typename... Ts>
    struct tlist_all_ttypes_of_size<k, LD::CT::TypeList<Rs...>, LD::CT::TypeList<H, Ts...>>
    {
    private:
        using res_lst = typename LD::Detail::Conditonal < Sizeof<H>::value == k, LD::CT::TypeList<Rs..., H>, LD::CT::TypeList < Rs...>>::type;
    public:
        using type =
        typename tlist_all_ttypes_of_size<k, res_lst, LD::CT::TypeList<Ts...>>::type;
    };

//---------------------------------------------------------------------------
/// Wrapper to use without looking inside the list's elements of tlist type

    template <LD::UInteger k, class List>
    struct tlist_all_types_of_size;

    template <LD::UInteger k, typename... Ts>
    struct tlist_all_types_of_size<k, LD::CT::TypeList<Ts...>>
    {
        using type =
        typename tlist_all_ttypes_of_size<k, LD::CT::TypeList<>, LD::CT::TypeList<Ts...>>::type;
    };
//---------------------------------------------------------------------------
/// Get all types of size k from a list of lists

    template <LD::UInteger k, class ListRes, class ListSource>
    struct tlist_all_ltypes_of_size
    {
        using type = LD::CT::TypeList<>;
    };

    template <LD::UInteger k, typename... RLs, typename... Hs>
    struct tlist_all_ltypes_of_size<k, LD::CT::TypeList<RLs...>, LD::CT::TypeList<LD::CT::TypeList<Hs...>>>
    {
    private:
        using types_found = typename tlist_all_types_of_size < k, LD::CT::TypeList<Hs...>>::type;
    public:
        using type = LD::CT::TypeList<RLs..., types_found>;
    };

    template <LD::UInteger k, typename... RLs, typename... Hs, typename... Ts>
    struct tlist_all_ltypes_of_size<k, LD::CT::TypeList<RLs...>, LD::CT::TypeList<LD::CT::TypeList<Hs...>, Ts...>>
    {
    private:
        using types_found = typename tlist_all_types_of_size<k, LD::CT::TypeList<Hs...>>::type;
    public:
        using type =
        typename tlist_all_ltypes_of_size < k, LD::CT::TypeList<RLs..., types_found>, LD::CT::TypeList<Ts...>>::type;
    };

//---------------------------------------------------------------------------
/// Wrapper

    template <LD::UInteger k, class List>
    struct tlist_all_typesdeep_of_size;

    template <LD::UInteger k, typename... ListEls>
    struct tlist_all_typesdeep_of_size<k, LD::CT::TypeList<ListEls...>>
    {
    private:
        static constexpr bool bLs = is_listoflists<LD::CT::TypeList<ListEls...>>::value;
    public:
        using type = typename LD::Detail::Conditonal < bLs,
                typename tlist_all_ltypes_of_size < k, LD::CT::TypeList<>, LD::CT::TypeList<ListEls...>>::type,
                typename tlist_all_ttypes_of_size<k, LD::CT::TypeList<>, LD::CT::TypeList<ListEls...>>::type>::type;
    };

//-----------------------------------------------------------------------------
/// Get all types of greater or greater and equal (bEqual ==true) of given size
///  from the list of mixed elements, no duplicates

    template <bool bEqual, LD::UInteger k, class ListRes, class ListSoource>
    struct tlist_all_ttypes_greater;

    template <bool bEqual, LD::UInteger k, typename... Rs>
    struct tlist_all_ttypes_greater<bEqual, k, LD::CT::TypeList<Rs...>, LD::CT::TypeList<>>
    {
        using type = typename LD::CT::Detail::DeDuplicateTypeList<LD::CT::TypeList<Rs...>>::type;
    };

    template <bool bEqual, LD::UInteger k, typename H, typename... Rs, typename... Ts>
    struct tlist_all_ttypes_greater<bEqual, k, LD::CT::TypeList<Rs...>, LD::CT::TypeList<H, Ts...>>
    {
    private:
        static constexpr bool bComp = bEqual ? (Sizeof<H>::value) >= k : (Sizeof<H>::value) > k;
        using res_lst =
        typename LD::Detail::Conditonal < bComp, LD::CT::TypeList<Rs..., H>, LD::CT::TypeList < Rs...>>::type;
    public:
        using type =
        typename tlist_all_ttypes_greater<bEqual, k, res_lst, LD::CT::TypeList<Ts...>>::type;
    };

/// Wrapper to provide initial empty list for result
    template <bool bEqual, LD::UInteger k, class List>
    struct tlist_all_types_greater;

    template <bool bEqual, LD::UInteger k, typename... Ts>
    struct tlist_all_types_greater<bEqual, k, LD::CT::TypeList<Ts...>>
    {
        using type = typename tlist_all_ttypes_greater<bEqual, k, LD::CT::TypeList<>, LD::CT::TypeList<Ts...>>::type;
    };

//-----------------------------------------------------------------------------
/// All types greater or equal of size k in a list of lists

    template <bool bEqual, LD::UInteger k, class ListRes, class ListSource>
    struct tlist_all_ltypes_greater
    {
        using type = LD::CT::TypeList<>;
    };

    template <bool bEqual, LD::UInteger k, typename... RLs, typename... Hs>
    struct tlist_all_ltypes_greater<bEqual, k, LD::CT::TypeList<RLs...>, LD::CT::TypeList<LD::CT::TypeList<Hs...>>>
    {
    private:
        using types_found = typename tlist_all_types_greater<bEqual, k, LD::CT::TypeList<Hs...>>::type;
    public:
        using type = LD::CT::TypeList<RLs..., types_found>; ;
    };

    template <bool bEqual, LD::UInteger k, typename... RLs, typename... Hs, typename... Ts>
    struct tlist_all_ltypes_greater<bEqual, k, LD::CT::TypeList<RLs...>, LD::CT::TypeList<LD::CT::TypeList<Hs...>, Ts...>>
    {
    private:
        using types_found = typename tlist_all_types_greater<bEqual, k, LD::CT::TypeList<Hs...>>::type;
    public:
        using type =
        typename tlist_all_ltypes_greater <bEqual, k, LD::CT::TypeList<RLs..., types_found>, LD::CT::TypeList<Ts...>>::type;
    };

//-----------------------------------------------------------------------------
/// Wrapper for types greater than size k

    template <bool bEqual, LD::UInteger k, class ListSources>
    struct tlist_all_typesdeep_greater;

    template <bool bEqual, LD::UInteger k, typename... ListEls>
    struct tlist_all_typesdeep_greater<bEqual, k, LD::CT::TypeList<ListEls...>>
    {
    private:
        static constexpr bool bLs = is_listoflists<LD::CT::TypeList<ListEls...>>::value;
    public:
        using type = typename LD::Detail::Conditonal < bLs,
                typename tlist_all_ltypes_greater <bEqual, k, LD::CT::TypeList<>, LD::CT::TypeList<ListEls...>>::type,
                typename tlist_all_ttypes_greater<bEqual, k, LD::CT::TypeList<>, LD::CT::TypeList<ListEls...>>::type>::type;
    };

//-----------------------------------------------------------------------------
/// All types less or equal the given size (supply true to include equal)
/// for a list of mixed elements

    template <bool bEqual, LD::UInteger k, class ListRes, class ListSoource>
    struct tlist_all_ttypes_less;

    template <bool bEqual, LD::UInteger k, typename... Rs>
    struct tlist_all_ttypes_less<bEqual, k, LD::CT::TypeList<Rs...>, LD::CT::TypeList<>>
    {
        using type = typename LD::CT::Detail::DeDuplicateTypeList<LD::CT::TypeList<Rs...>>::type;
    };

    template <bool bEqual, LD::UInteger k, typename H, typename... Rs, typename... Ts>
    struct tlist_all_ttypes_less<bEqual, k, LD::CT::TypeList<Rs...>, LD::CT::TypeList<H, Ts...>>
    {
    private:
        static constexpr bool bComp = bEqual ? (Sizeof<H>::value) <= k : (Sizeof<H>::value) < k;
        using res_lst =
        typename LD::Detail::Conditonal < bComp, LD::CT::TypeList<Rs..., H>, LD::CT::TypeList < Rs...>>::type;
    public:
        using type =
        typename tlist_all_ttypes_less<bEqual, k, res_lst, LD::CT::TypeList<Ts...>>::type;
    };

// Initial result list wrapped
    template <bool bEqual, LD::UInteger k, class List>
    struct tlist_all_types_less;

    template <bool bEqual, LD::UInteger k, typename... Ts>
    struct tlist_all_types_less<bEqual, k, LD::CT::TypeList<Ts...>>
    {
        using type = typename tlist_all_ttypes_less<bEqual, k, LD::CT::TypeList<>, LD::CT::TypeList<Ts...>>::type;
    };

//-----------------------------------------------------------------------------
/// All types less or equal size k in a list of lists

    template <bool bEqual, LD::UInteger k, class ListRes, class ListSource>
    struct tlist_all_ltypes_less
    {
        using type = LD::CT::TypeList<>;
    };

    template <bool bEqual, LD::UInteger k, typename... RLs, typename... Hs>
    struct tlist_all_ltypes_less<bEqual, k, LD::CT::TypeList<RLs...>, LD::CT::TypeList<LD::CT::TypeList<Hs...>>>
    {
    private:
        using types_found = typename tlist_all_types_less<bEqual, k, LD::CT::TypeList<Hs...>>::type;
    public:
        using type = LD::CT::TypeList<RLs..., types_found>; ;
    };

    template <bool bEqual, LD::UInteger k, typename... RLs, typename... Ts, typename... Hs>
    struct tlist_all_ltypes_less<bEqual, k, LD::CT::TypeList<RLs...>, LD::CT::TypeList<LD::CT::TypeList<Hs...>, Ts...>>
    {
    private:
        using types_found = typename tlist_all_ttypes_less<bEqual, k, LD::CT::TypeList<>, LD::CT::TypeList<Hs...>>::type;
    public:
        using type =
        typename tlist_all_ltypes_less <bEqual, k, LD::CT::TypeList<RLs..., types_found>, LD::CT::TypeList<Ts...>>::type;
    };

//-----------------------------------------------------------------------------
/// Wrapper for types lesser or equal than size k

    template <bool bEqual, LD::UInteger k, class ListSources>
    struct tlist_all_typesdeep_less;

    template <bool bEqual, LD::UInteger k, typename... ListEls>
    struct tlist_all_typesdeep_less<bEqual, k, LD::CT::TypeList<ListEls...>>
    {
    private:
        static constexpr bool bLs = is_listoflists<LD::CT::TypeList<ListEls...>>::value;
    public:
        using type = typename LD::Detail::Conditonal < bLs,
                typename tlist_all_ltypes_less <bEqual, k, LD::CT::TypeList<>, LD::CT::TypeList<ListEls...>>::type,
                typename tlist_all_ttypes_less<bEqual, k, LD::CT::TypeList<>, LD::CT::TypeList<ListEls...>>::type>::type;
    };


//-------------------------------------------------------------------------------
/// Convert tlist to tuple and back (From Peter Dimov)

}// End of namespace TypeList

namespace LD
{
    namespace CT
    {
        namespace Detail
        {

            template <class T, class U>
            struct flatten_helper;

            // first case - the head of the List is List too
            // expand this List and continue
            template <class... Ts, class... Heads, class... Tail>
            struct flatten_helper<LD::CT::TypeList<Ts...>, LD::CT::TypeList<LD::CT::TypeList<Heads...>, Tail...>> {
                using type = typename flatten_helper<LD::CT::TypeList<Ts...>, LD::CT::TypeList<Heads..., Tail...>>::type;
            };

            // second case - the head of the List is not a List
            // append it to our new, flattened list
            template <class... Ts, class Head, class... Tail>
            struct flatten_helper<LD::CT::TypeList<Ts...>, LD::CT::TypeList<Head, Tail...>> {
                using type = typename flatten_helper<LD::CT::TypeList<Ts..., Head>, LD::CT::TypeList<Tail...>>::type;
            };

            // base case - input List is empty
            // return our flattened list
            template <class... Ts>
            struct flatten_helper<LD::CT::TypeList<Ts...>, LD::CT::TypeList<>> {
                using type = LD::CT::TypeList<Ts...>;
            };

            // wrapper around flatten_helper
            template <class T> struct flatten;

            // start with an empty accumulator
            template <class... Ts>
            struct flatten<LD::CT::TypeList<Ts...>> {
                using type = typename flatten_helper<LD::CT::TypeList<>, LD::CT::TypeList<Ts...>>::type;
            };

            template<typename TypeList, template<typename> class Transformation>
            class Transform;

            template<template<typename> class Transformation,typename ... Args>
            struct Transform<LD::CT::TypeList<Args...>,Transformation>
            {
                using type = LD::CT::TypeList<Transformation<Args>...>;
            };
        }

        template<typename T, template<typename> class Transformation>
        using Tranform = typename LD::CT::Detail::Transform<T,Transformation>::type;

        template<typename T>
        using Flatten = typename LD::CT::Detail::flatten<T>::type;

        template <typename... T1s, typename... T2s>
        constexpr auto concatenate(LD::CT::TypeList<T1s...>, LD::CT::TypeList<T2s...>) {
            return LD::CT::TypeList<T1s..., T2s...>{};
        }

        template <template <typename> typename Condition, typename Result>
        constexpr auto filter_types(Result result, LD::CT::TypeList<>)
        {
            return result;
        }

        template <template <typename> class Condition, typename Result, typename T, typename... Ts>
        constexpr auto filter_types(Result result, LD::CT::TypeList<T, Ts...>)
        {

            if constexpr (Condition<T>{})
                return filter_types<Condition>(concatenate(result, LD::CT::TypeList<T>{}), LD::CT::TypeList<Ts...>{});
            else
                return filter_types<Condition>(result, LD::CT::TypeList<Ts...>{});
        }



        template <template <typename> typename Condition, typename... Types>
        using FilterPack = LD::Detail::Decay_T<decltype(filter_types<Condition>(LD::CT::TypeList<>{}, LD::CT::TypeList<Types...>{}))>;

        template <typename TL,template <typename> typename Condition>
        using Filter = LD::Detail::Decay_T<decltype(filter_types<Condition>(LD::CT::TypeList<>{}, TL{}))>;



        template<typename TL1, typename TL2>
        using Concatenate = LD::Detail::Decay_T<decltype(concatenate(LD::Declval<TL1>(),LD::Declval<TL2>()))>;
    }
}
#endif
