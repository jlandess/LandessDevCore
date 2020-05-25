//
// Created by phoenixflower on 5/22/20.
//

#ifndef LANDESSDEVCORE_SPAN_HPP
#define LANDESSDEVCORE_SPAN_HPP
#include "Definitions/Integer.hpp"
#include "Definitions/Common.hpp"
#include "TypeTraits/Iterable.h"
namespace LD
{

    namespace Detail
    {
        template<typename T, class = void>
        struct Spannable;
        template<typename T>
        struct Spannable<T,LD::EnableIf<
                LD::Require<
                LD::Negate<LD::Concept::ContinuousIterable<T>>
                >>>
        {
            constexpr static const bool value = false;
        };

        template<typename T>
        struct Spannable<T,LD::EnableIf<
                LD::Require<
                        LD::Concept::ContinuousIterable<T>
                >>>
        {
            constexpr static const bool value = true;
        };


        template<typename T, class = void>
        class SpanAdapter;




        template<typename T>
        class SpanAdapter<T,LD::Enable_If_T<
                LD::Require<
                LD::Detail::Spannable<T>::value,
                LD::Exists<LD::Detail::CamelCaseBegin,T>,
                LD::Exists<LD::Detail::CameCaseEnd,T>,
                LD::Negate<LD::Exists<LD::Detail::ReverseCamelCaseBegin,T>>,
                LD::Negate<LD::Exists<LD::Detail::ReverseCamelCaseEnd,T>>
        >>>
        {
        public:
            using IteratorType = decltype(LD::Declval<T>().begin());
            using Type = decltype(*LD::Declval<IteratorType>());
        private:
            Type * mBegin;
            LD::UInteger mSize;
        public:
            constexpr SpanAdapter() noexcept:mBegin(nullptr),mSize(0)
            {

            }
            constexpr SpanAdapter(const T & spannableObject) noexcept:mBegin(&spannableObject[0]),mSize(spannableObject.end() - spannableObject.begin())
            {
            }

            constexpr Type * Begin() noexcept
            {
                return this->mBegin;
            }

            constexpr LD::UInteger & Size() const noexcept
            {
                return this->mSize;
            }
        };

        template<typename T>
        class SpanAdapter<T,LD::Enable_If_T<
                LD::Require<
                        LD::Detail::Spannable<T>::value,
                        LD::Either<
                        LD::Require<LD::Exists<LD::Detail::ReverseCamelCaseBegin,T>,LD::Exists<LD::Detail::ReverseCamelCaseEnd,T>>,
                        LD::Require<LD::Exists<LD::Detail::CamelCaseBegin,T>,LD::Exists<LD::Detail::CameCaseEnd,T>>
                        >
                >>>
        {
        public:
            using IteratorType = decltype(LD::Declval<T>().Begin());
            using Type = decltype(*LD::Declval<IteratorType>());
        private:
            Type * mBegin;
            LD::UInteger mSize;
        public:
            constexpr SpanAdapter() noexcept:mBegin(nullptr),mSize(0)
            {

            }
            constexpr SpanAdapter(const T & spannableObject) noexcept:mBegin(&spannableObject[0]),mSize(spannableObject.End() - spannableObject.Begin())
            {
            }

            constexpr Type * Begin() noexcept
            {
                return this->mBegin;
            }

            constexpr LD::UInteger & Size() const noexcept
            {
                return this->mSize;
            }

        };

    }

    template<typename T>
    constexpr const bool Spannable = LD::Detail::Spannable<T>::value;

    template<typename T>
    class Span
    {
    private:
        T * mBuffer;
        LD::UInteger mSize;
    public:
        inline Span() noexcept:mBuffer(nullptr),mSize(0){}

        inline Span(const Span & span) = default;

        inline Span(T * buffer, const LD::UInteger & size) noexcept:mBuffer(buffer),mSize(size){}

        inline Span(T * firstElement, T * lastelement) noexcept:mBuffer(firstElement),mSize(lastelement-firstElement){}


        template<typename V, typename = LD::EnableIf<LD::Require<LD::Spannable<V>>>>
        inline Span(const V & contiguousContainer) noexcept
        {
            LD::Detail::SpanAdapter<V> spanAdapter{contiguousContainer};
            this->mBuffer = spanAdapter.Begin();
            this->mSize = spanAdapter.Size();
        }

        template<typename V>
        LD::Enable_If_T<LD::Require<LD::Spannable<V>>> operator = (const V & contiguousContainer) noexcept
        {
            LD::Detail::SpanAdapter<V> spanAdapter{contiguousContainer};
            this->mBuffer = spanAdapter.Begin();
            this->mSize = spanAdapter.Size();
            return (*this);
        }

        constexpr LD::UInteger Size() const noexcept{return this->mSize;}


        inline constexpr const T & Front() const noexcept{return *(this->mBuffer);}

        inline constexpr const T & Back() const noexcept{return *( this->mBuffer + this->mSize - 1 );}

        template<typename V>
        inline constexpr LD::UInteger ToSize( const V & size)
        {
            return static_cast<LD::UInteger>( size );
        }

        LD::UInteger SizeInBytes() const noexcept{return  this->Size() * ToSize( sizeof( T ) );}

         inline constexpr bool Empty() const noexcept{return this->Size() == 0;}

        inline constexpr const T & operator[]( const LD::UInteger & idx ) const noexcept{return *(mBuffer+idx);}

        inline constexpr  T & operator[]( const LD::UInteger & idx )  noexcept{return *(mBuffer+idx);}

        inline constexpr T * Data() const noexcept{return this->mBuffer;}
    };
}
#endif //LANDESSDEVCORE_SPAN_HPP
