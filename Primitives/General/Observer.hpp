//
// Created by James Landess on 2/3/20.
//

#ifndef LANDESSDEVCORE_OBSERVER_HPP
#define LANDESSDEVCORE_OBSERVER_HPP

//#include "Definitions/Common.hpp"
#include "Memory/ElementReference.h"
namespace LD
{
    template<typename T>
    class ContextObserver
    {
    private:
        LD::ElementReference<T> mReference;
    public:
        inline constexpr ContextObserver() noexcept {}
        inline constexpr ContextObserver(const LD::ElementReference<T> & element) noexcept :mReference(element){}
        inline constexpr ContextObserver &operator = (const LD::ElementReference<T> & element) noexcept{(*this->mReference) = element;return(*this);}

        template<typename U>
        inline constexpr LD::Enable_If_T<
        LD::Require<
                (LD::IsCopyAssignable_V<T,U> || LD::IsAssignable_V<T,U>)
        >,ContextObserver&> operator = (const U & element)
        {
            T * ptr = this->mReference.GetPointer();
            (*ptr) = element;
            return (*this);
        }


        inline constexpr T & Value()
        {
            return *this->mReference;
        }

        inline constexpr const T & Value() const
        {
            return *this->mReference;
        }
        template<typename U,typename V = T>
        inline constexpr LD::Enable_If_T<LD::Require<LD::Concept::Addable<V,U>>,decltype(LD::Declval<V>()+LD::Declval<U>())> operator + (const U & operand) const noexcept(noexcept(LD::Declval<V>()+LD::Declval<U>()))
        {
            V * ptr = this->mReference.GetPointer();
            return (*ptr) + operand;
        }

        template<typename U,typename V = T>
        inline constexpr LD::Enable_If_T<LD::Require<LD::Concept::Subtractable<V,U>>,decltype(LD::Declval<V>()+LD::Declval<U>())> operator - (const U & operand) const noexcept(noexcept(LD::Declval<V>()-LD::Declval<U>()))
        {
            T * ptr = this->mReference.GetPointer();
            return (*ptr) - operand;
        }

        template<typename U,typename V = T>
        inline constexpr LD::Enable_If_T<LD::Require<LD::Concept::Multipliable<V,U>>,decltype(LD::Declval<V>()+LD::Declval<U>())> operator * (const U & operand) const noexcept(noexcept(LD::Declval<V>()*LD::Declval<U>()))
        {
            T * ptr = this->mReference.GetPointer();
            return (*ptr) * operand;
        }

        template<typename U, typename V = T>
        inline constexpr LD::Enable_If_T<LD::Require<LD::Concept::Divisible<V,U>>,decltype(LD::Declval<V>()+LD::Declval<U>())> operator / (const U & operand) const noexcept(noexcept(LD::Declval<V>()/LD::Declval<U>()))
        {
            T * ptr = this->mReference.GetPointer();
            return (*ptr) / operand;
        }

        template<typename U, typename V = T>
        inline constexpr auto operator += (const U & operand) noexcept(noexcept(LD::Declval<V&>()+=LD::Declval<U>())) ->decltype(LD::Declval<V&>()+=LD::Declval<U>())
        {
            T * ptr = this->mReference.GetPointer();
            return ((*ptr) += operand);
        }


        template<typename U, typename V = T>
        inline constexpr auto operator -= (const U & operand) noexcept(noexcept(LD::Declval<V&>()-=LD::Declval<U>())) ->decltype(LD::Declval<V&>()-=LD::Declval<U>())
        {
            T * ptr = this->mReference.GetPointer();
            return ((*ptr) -= operand);
        }

        template<typename U, typename V = T>
        inline constexpr auto operator *= (const U & operand) noexcept(noexcept(LD::Declval<V&>()*=LD::Declval<U>())) ->decltype(LD::Declval<V&>()*=LD::Declval<U>())
        {
            T * ptr = this->mReference.GetPointer();
            return ((*ptr) *= operand);
        }

        template<typename U, typename V = T>
        inline constexpr auto operator /= (const U & operand) noexcept(noexcept(LD::Declval<V&>()/=LD::Declval<U>())) ->decltype(LD::Declval<V&>()/=LD::Declval<U>())
        {
            T * ptr = this->mReference.GetPointer();
            return ((*ptr) /= operand);
        }

        template<typename U, typename V = T>
        inline constexpr auto operator > (const U & operand) noexcept(noexcept(LD::Declval<V>()>LD::Declval<U>())) ->decltype(LD::Declval<V>()>LD::Declval<U>())
        {
            T * ptr = this->mReference.GetPointer();
            return ((*ptr) > operand);
        }

        template<typename U, typename V = T>
        inline constexpr auto operator >= (const U & operand) noexcept(noexcept(LD::Declval<V>()>=LD::Declval<U>())) ->decltype(LD::Declval<V>()>=LD::Declval<U>())
        {
            T * ptr = this->mReference.GetPointer();
            return ((*ptr) >= operand);
        }

        template<typename U, typename V = T>
        inline constexpr auto operator < (const U & operand) noexcept(noexcept(LD::Declval<V>()<LD::Declval<U>())) ->decltype(LD::Declval<V>()<LD::Declval<U>())
        {
            T * ptr = this->mReference.GetPointer();
            return ((*ptr) < operand);
        }

        template<typename U, typename V = T>
        inline constexpr auto operator <= (const U & operand) noexcept(noexcept(LD::Declval<V>()>=LD::Declval<U>())) ->decltype(LD::Declval<V>()<=LD::Declval<U>())
        {
            T * ptr = this->mReference.GetPointer();
            return ((*ptr) <= operand);
        }

        template<typename U, typename V = T>
        inline constexpr auto operator == (const U & operand) noexcept(noexcept(LD::Declval<V>()==LD::Declval<U>())) ->decltype(LD::Declval<V>()==LD::Declval<U>())
        {
            T * ptr = this->mReference.GetPointer();
            return ((*ptr) == operand);
        }

        template<typename U, typename V = T>
        inline constexpr auto operator != (const U & operand) noexcept(noexcept(LD::Declval<V>()!=LD::Declval<U>())) ->decltype(LD::Declval<V>()!=LD::Declval<U>())
        {
            T * ptr = this->mReference.GetPointer();
            return ((*ptr) != operand);
        }

        template<typename U, typename V = T>
        inline constexpr auto operator && (const U & operand) noexcept(noexcept(LD::Declval<V>()&&LD::Declval<U>())) ->decltype(LD::Declval<V>()&&LD::Declval<U>())
        {
            T * ptr = this->mReference.GetPointer();
            return ((*ptr) && operand);
        }

        template<typename U, typename V = T>
        inline constexpr auto operator || (const U & operand) noexcept(noexcept(LD::Declval<V>()||LD::Declval<U>())) ->decltype(LD::Declval<V>()||LD::Declval<U>())
        {
            T * ptr = this->mReference.GetPointer();
            return ((*ptr) || operand);
        }

    };
}
#endif //LANDESSDEVCORE_OBSERVER_HPP
