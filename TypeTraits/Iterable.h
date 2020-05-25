//
// Created by phoenixflower on 5/24/20.
//

#ifndef LANDESSDEVCORE_ITERABLE_H
#define LANDESSDEVCORE_ITERABLE_H
#include "Declval.hpp"
#include "Conditional.hpp"
namespace LD
{
    namespace Detail
    {
        template<typename T>
        using CamelCaseBegin = decltype(LD::Declval<T>().begin());

        template<typename T>
        using CameCaseEnd = decltype(LD::Declval<T>().end());

        template<typename T>
        using ConstCamelCaseBegin = decltype(LD::Declval<T>().cbegin());

        template<typename T>
        using ConstCameCaseEnd = decltype(LD::Declval<T>().cend());

        template<typename T>
        using ReverseCamelCaseBegin = decltype(LD::Declval<T>().Begin());

        template<typename T>
        using ReverseCamelCaseEnd = decltype(LD::Declval<T>().End());

        template<typename T>
        using ConstReverseCamelCaseBegin = decltype(LD::Declval<T>().CBegin());

        template<typename T>
        using ConstReverseCamelCaseEnd = decltype(LD::Declval<T>().CEnd());

        template<typename T, typename = void >
        struct BeginIterator;
        template<typename T, typename = void >
        struct ConstBeginIterator;

        template<typename T>
        struct BeginIterator<T,LD::Enable_If_T<
                LD::Require<
                LD::Negate<LD::Exists<CamelCaseBegin,T>>,
                LD::Negate<LD::Exists<ReverseCamelCaseBegin,T>>
        >>>
        {
            using type = void;
        };

        template<typename T>
        struct ConstBeginIterator<T,LD::Enable_If_T<
                LD::Require<
                        LD::Negate<LD::Exists<ConstCamelCaseBegin,T>>,
                        LD::Negate<LD::Exists<ConstReverseCamelCaseBegin,T>>
                >>>
        {
            using type = void;
        };

        template<typename T>
        struct BeginIterator<T,LD::Enable_If_T<LD::Require<LD::Exists<CamelCaseBegin,T>>>>
        {
            using type = decltype(LD::Declval<T>().begin());
        };

        template<typename T>
        struct ConstBeginIterator<T,LD::Enable_If_T<LD::Require<LD::Exists<CamelCaseBegin,T>>>>
        {
            using type = decltype(LD::Declval<T>().cbegin());
        };

        template<typename T>
        struct BeginIterator<T,LD::Enable_If_T<LD::Require<LD::Exists<ReverseCamelCaseBegin,T>>>>
        {
            using type = decltype(LD::Declval<T>().Begin());
        };

        template<typename T>
        struct ConstBeginIterator<T,LD::Enable_If_T<LD::Require<LD::Exists<ConstReverseCamelCaseBegin,T>>>>
        {
            using type = decltype(LD::Declval<T>().CBegin());
        };

        template<typename T, typename = void>
        struct EndIterator;
        template<typename T, typename = void>
        struct ConstEndIterator;

        template<typename T>
        struct EndIterator<T,LD::Enable_If_T<
                LD::Require<
                        LD::Negate<LD::Exists<CameCaseEnd,T>>,
                        LD::Negate<LD::Exists<ReverseCamelCaseEnd,T>>
        >>>
        {
            using type = void;
        };

        template<typename T>
        struct ConstEndIterator<T,LD::Enable_If_T<
                LD::Require<
                        LD::Negate<LD::Exists<ConstCameCaseEnd,T>>,
                        LD::Negate<LD::Exists<ConstReverseCamelCaseEnd,T>>
                >>>
        {
            using type = void;
        };
        template<typename T>
        struct EndIterator<T,LD::Enable_If_T<LD::Require<LD::Exists<CameCaseEnd,T>>>>
        {
            using type = decltype(LD::Declval<T>().end());
        };

        template<typename T>
        struct ConstEndIterator<T,LD::Enable_If_T<LD::Require<LD::Exists<ConstCameCaseEnd,T>>>>
        {
            using type = decltype(LD::Declval<T>().cend());
        };

        template<typename T>
        struct EndIterator<T,LD::Enable_If_T<LD::Require<LD::Exists<ReverseCamelCaseEnd,T>>>>
        {
            using type = decltype(LD::Declval<T>().End());
        };

        template<typename T>
        struct ConstEndIterator<T,LD::Enable_If_T<LD::Require<LD::Exists<ConstReverseCamelCaseEnd,T>>>>
        {
            using type = decltype(LD::Declval<T>().CEnd());
        };
    }
    template<typename T>
    using BeginIterator = typename LD::Detail::BeginIterator<T>::type;

    template<typename T>
    using EndIterator = typename LD::Detail::EndIterator<T>::type;

    template<typename T>
    using ConstBeginIterator = typename LD::Detail::ConstBeginIterator<T>::type;

    template<typename T>
    using ConstEndIterator = typename LD::Detail::ConstEndIterator<T>::type;



    template<typename T>
    class BackInserter
    {
    private:

    public:
    };

    template<typename T>
    class FrontInserter
    {
    private:

    public:
    };

}
#endif //LANDESSDEVCORE_ITERABLE_H
