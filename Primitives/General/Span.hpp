//
// Created by phoenixflower on 5/22/20.
//

#ifndef LANDESSDEVCORE_SPAN_HPP
#define LANDESSDEVCORE_SPAN_HPP
#include "Definitions/Integer.hpp"
#include "Definitions/Common.hpp"
#include "TypeTraits/Iterable.h"
#include "TypeTraits/Detection.hpp"
#include "TypeTraits/Type.h"
#include "TypeTraits/FunctionalReflection.hpp"
namespace LD
{

    template<typename T>
    class Span;
    namespace Detail
    {

        template<typename T, class = void>
        struct Spannable
        {
            constexpr static const bool value = false;
        };
        template<typename T>
        struct Spannable<T,LD::Enable_If_T<
                LD::Require<
                        LD::Exists<LD::Detail::CamelCaseBegin,T>,
                        LD::Exists<LD::Detail::CameCaseEnd,T>,
                        LD::Negate<LD::Exists<LD::Detail::ReverseCamelCaseBegin,T>>,
                        LD::Negate<LD::Exists<LD::Detail::ReverseCamelCaseEnd,T>>
                >>>
        {
            constexpr static const bool value = LD::Concept::ContinuousIterable<decltype(LD::Declval<T>().begin())>;
        };

        template<typename T>
        struct Spannable<T,LD::Enable_If_T<
                LD::Require<
                        LD::Negate<LD::Exists<LD::Detail::CamelCaseBegin,T>>,
                        LD::Negate<LD::Exists<LD::Detail::CameCaseEnd,T>>,
                        LD::Exists<LD::Detail::ReverseCamelCaseBegin,T>,
                        LD::Exists<LD::Detail::ReverseCamelCaseEnd,T>
                >>>
        {
            constexpr static const bool value = LD::Concept::ContinuousIterable<decltype(LD::Declval<T>().Begin())>;
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
            using Type = LD::Detail::Decay_T<decltype(*LD::Declval<IteratorType>())>;
        private:
            Type * mBegin;
            LD::UInteger mSize;
        public:
            constexpr SpanAdapter() noexcept:mBegin(nullptr),mSize(0)
            {

            }
            constexpr SpanAdapter(T & spannableObject) noexcept:mBegin{(Type*)&spannableObject.begin()[0]},mSize(spannableObject.end()-spannableObject.begin())
            {
            }

            constexpr Type * Begin() noexcept
            {
                return this->mBegin;
            }

            constexpr const LD::UInteger & Size() const noexcept
            {
                return this->mSize;
            }


        };

        template<typename T>
        class SpanAdapter<T,LD::Enable_If_T<
                LD::Require<
                        LD::Detail::Spannable<T>::value,
                        LD::Negate<LD::Exists<LD::Detail::CamelCaseBegin,T>>,
                        LD::Negate<LD::Exists<LD::Detail::CameCaseEnd,T>>,
                        LD::Exists<LD::Detail::ReverseCamelCaseBegin,T>,
                        LD::Exists<LD::Detail::ReverseCamelCaseEnd,T>
                >>>
        {
        public:
            using IteratorType = decltype(LD::Declval<T>().Begin());
            using Type = LD::Detail::Decay_T<decltype(*LD::Declval<IteratorType>())>;
        private:
            Type * mBegin;
            LD::UInteger mSize;
        public:
            constexpr SpanAdapter() noexcept:mBegin(nullptr),mSize(0)
            {

            }
            constexpr SpanAdapter(T & spannableObject) noexcept:mBegin{(Type*)&spannableObject.Begin()[0]},mSize(spannableObject.End()-spannableObject.Begin())
            {
            }

            constexpr Type * Begin() noexcept
            {
                return this->mBegin;
            }

            constexpr const LD::UInteger & Size() const noexcept
            {
                return this->mSize;
            }
        };
        /*
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
         */

        template<typename T>
        struct IsSpan: public LD::Detail::IntegralConstant<bool,false>
        {

        };

        template<typename T>
        struct IsSpan<LD::Span<T>>: public LD::Detail::IntegralConstant<bool,true>
        {

        };



    }

    template<typename T>
    constexpr const bool IsSpan = LD::Detail::IsSpan<T>::value;
    template<typename T>
    constexpr const bool Spannable = LD::Detail::Spannable<T>::value;

    namespace CT
    {
        template<typename T>
        constexpr bool Spannable(LD::Type<T> )
        {
            return LD::Spannable<T>;
        }


    }

    template<typename T>
    class Span
    {
    public:
        using UType = typename LD::Detail::SpanAdapter<T>::Type;
    private:

        UType * mBuffer;
        LD::UInteger mSize;
    public:
        inline Span() noexcept:mBuffer(nullptr),mSize(0){}

        inline Span(const Span & span) = default;

        inline Span(T * buffer, const LD::UInteger & size) noexcept:mBuffer(buffer),mSize(size){}

        inline Span(T * firstElement, T * lastelement) noexcept:mBuffer(firstElement),mSize(lastelement-firstElement){}


        template<typename V, typename = LD::EnableIf<LD::Require<LD::Spannable<V>>>>
        inline Span(V & contiguousContainer) noexcept
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


        inline constexpr const UType & Front() const noexcept{return *(this->mBuffer);}

        inline constexpr const UType & Back() const noexcept{return *( this->mBuffer + this->mSize - 1 );}

        template<typename V>
        inline constexpr LD::UInteger ToSize( const V & size)
        {
            return static_cast<LD::UInteger>( size );
        }

        LD::UInteger SizeInBytes() const noexcept{return  this->Size() * ToSize( sizeof( T ) );}

         inline constexpr bool Empty() const noexcept{return this->Size() == 0;}

        inline constexpr const UType & operator[]( const LD::UInteger & idx ) const noexcept{return *(mBuffer+idx);}

        inline constexpr  UType & operator[]( const LD::UInteger & idx )  noexcept{return *(mBuffer+idx);}

        inline constexpr UType * Data() const noexcept{return this->mBuffer;}
    };

    template <typename T> Span(T & span) -> Span<T>;

    namespace CT
    {
        template<typename T>
        constexpr auto SpanType(LD::Type<LD::Span<T>>) noexcept
        {
            using SpanObjectType = LD::Span<T>;
            using SpanInnerType = typename SpanObjectType::UType ;
            return LD::Type<SpanInnerType>{};
        }

        template<typename T, typename U>
        constexpr bool SpannableWithConstraint(LD::Type<T> ,LD::Type<U>) noexcept
        {
            if constexpr (LD::Spannable<T>)
            {
                constexpr auto SpanInnerType = SpanType(LD::Type<LD::Span<T>>{});
                return LD::CT::IsSame(SpanInnerType,LD::Type<U>{});
            }
            return false;
        }
    }
}
#endif //LANDESSDEVCORE_SPAN_HPP
