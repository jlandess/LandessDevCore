//
// Created by phoenixflower on 12/17/20.
//

#ifndef LANDESSDEVCORE_SEARCHING_HPP
#define LANDESSDEVCORE_SEARCHING_HPP
#include "IO/FetchRequest.h"
#include "Primitives/General/Span.hpp"
#include "Sorting.hpp"
namespace LD
{
    /**]
     *
     * @tparam T
     * @tparam U
     * @tparam Comparator
     * @tparam A
     * @param array
     * @param comparator
     * @param key
     * @param args
     * @return
     */
    template<typename T, typename U,typename LessThanComparator, typename EqualityComparator ,typename ... A>
    LD::Enable_If_T<
            LD::Require<
                    LD::CT::Spannable(LD::Type<T>{}),
                    LD::ConvertiblyCallable<LessThanComparator,bool(typename LD::Span<T>::UType,typename LD::Span<T>::UType)>::Value(),
                    LD::CT::IsSameWithoutQualifiers(LD::Type<typename LD::Span<T>::UType>{},LD::Type<U>{})
            >
    ,
    LD::QueryResult<LD::UInteger(A...)>> BinarySearch(T & array, U && key,EqualityComparator && equalityComparator,LessThanComparator && lessThanComparator, A && ... args) noexcept
    {
        LD::Span currentSpan{array};
        LD::UInteger begin = 0;
        LD::UInteger end = currentSpan.Size();
        while (begin <= end) {
            LD::UInteger m = begin + (end - begin) / 2;

            // Check if x is present at mid
            if (equalityComparator(currentSpan[m],key))
                return LD::MakeContext(LD::TransactionResult{},LD::UInteger{m},LD::Forward<A>(args)...);

            // If x greater, ignore left half
            if (lessThanComparator(currentSpan[m],key))
                begin = m + 1;

                // If x is smaller, ignore right half
            else
                end = m - 1;
        }
        return LD::MakeContext(LD::TransactionError{},LD::Forward<A>(args)...);
    }

    template<typename T, typename U,typename LessThanComparator, typename EqualityComparator ,typename ... A>
    LD::Enable_If_T<LD::Require<
            LD::CT::Spannable(LD::Type<T>{}),
            LD::ConvertiblyCallable<LessThanComparator,bool(typename LD::Span<T>::UType,typename LD::Span<T>::UType)>::Value(),
            LD::CT::IsSameWithoutQualifiers(LD::Type<typename LD::Span<T>::UType>{},LD::Type<U>{})
    >
    ,
    LD::QueryResult<LD::UInteger(A...)>> BinarySearch(T & array, U && key, A && ... args) noexcept
    {
        return LD::BinarySearch(array,LD::Forward<U>(key),LD::Descending<U>{},LD::Equality<U>{},LD::Forward<A>(args)...);
    }
}
#endif //LANDESSDEVCORE_SEARCHING_HPP
