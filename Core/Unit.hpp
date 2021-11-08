//
// Created by James Landess on 2/23/20.
//

#ifndef LANDESSDEVCORE_UNIT_H
#define LANDESSDEVCORE_UNIT_H

#include "Primitives/General/Ratio.hpp"
#include "TypeTraits/ExclusiveOr.hpp"
#include "Primitives/General/Immutable.hpp"
#include "Core/RequestResponse.hpp"
#include "Primitives/General/StringView.hpp"
#include "Algorithms/FromString.hpp"
#include "Algorithms/Exponential.hpp"
#include "Algorithms/Periodic.hpp"
namespace LD
{
    template<typename Exponent>
    class UnitExponent;
    template<LD::UInteger Numerator, LD::UInteger Denominator>
    class UnitExponent<LD::CT::Ratio<Numerator,Denominator>>
    {};
    template<typename BaseTag, typename Tag>
    class UnitRelationship;
    /**
     *
     * @tparam Precison
     * @tparam BaseTag
     * @tparam CurrentTag
     * @tparam PrefixRatio
     * @tparam FractionalRatio
     * @tparam UnitRatio
     * @tparam sfinae
     */
    template<typename Precison,template<typename> class  BaseTag, template<typename> class  CurrentTag, class PrefixRatio , class UnitExponent ,class sfinae = void>
    class Unit;

    namespace CT
    {
        template<typename T>
        constexpr bool IsUnit(LD::Type<T> ) noexcept
        {
            return false;
        }

        template<typename Precison,template<typename> class  BaseTag, template<typename> class  CurrentTag, class PrefixRatio , class UnitExponent>
        constexpr bool IsUnit(LD::Type<Unit<Precison,BaseTag,CurrentTag,PrefixRatio,UnitExponent>> )
        {
            return true;
        }
    }
    /**
     *
     * @tparam Precision
     * @tparam BaseTag
     * @tparam CurrentTag
     * @tparam PrefixRatioNumerator
     * @tparam PrefixRatioDenominator
     * @tparam ExponentNumerator
     * @tparam ExponentDenominator
     * @tparam UnitNumerator
     * @tparam UnitDenominator
     * @brief Represents the base construction of all Units, provides the default operations of addition and subtraction for all units, as well as automatic prefix conversion
     * If you want units to convert from one type to another automatically such as Celcius to Fehrenheit or Celcius to Kelvin etc @seeLD::UnitRelationship
     */
    template<typename T,
            template<typename> class BaseTag,
            template<typename> class CurrentTag,
            LD::UInteger PrefixRatioNumerator, LD::UInteger PrefixRatioDenom,
            LD::Integer UnitExponentNumerator, LD::UInteger UnitExponentDenominator
    >
    class Unit<T,BaseTag,CurrentTag,LD::CT::Ratio<PrefixRatioNumerator,PrefixRatioDenom>,LD::UnitExponent<LD::CT::Ratio<UnitExponentNumerator,UnitExponentDenominator>>,
            LD::Enable_If_T<
                    LD::Require<
                            LD::ConvertiblyCallable<UnitRelationship<BaseTag<T>,CurrentTag<T>>,BaseTag<T>(const CurrentTag<T> &)>::Value(),
                            LD::ConvertiblyCallable<UnitRelationship<BaseTag<T>,CurrentTag<T>>,CurrentTag<T>(const BaseTag<T> &)>::Value()
                    >>>
    {
    private:
        CurrentTag<T> mUnit;
    public:
        template<typename Q, template<typename> class B, template<typename> class C, class P, typename Z , class s>
        friend class Unit;
        inline constexpr explicit Unit() noexcept :mUnit(){}
        inline constexpr explicit Unit(const CurrentTag<T> & tag) noexcept :mUnit{tag}{}
        template<typename U, typename V = T ,class = LD::Enable_If_T<LD::Require<LD::Detail::IsConvertible<V,U>::value,LD::Concept::Arithmetical<U>>>>
        inline constexpr explicit Unit(const U & object) noexcept : mUnit{CurrentTag{object}}{}

        template<typename U,template<typename> class Tag,
                LD::UInteger CurrentPrefixRatioNumerator, LD::UInteger CurrentPrefixRatioDenom,
                template<typename> class UnitBaseTag,
                template<typename> class CurrentBaseTag = BaseTag,
                template<typename> class CurrentTagType = CurrentTag,
                typename V = T, typename = LD::Enable_If_T<
                        LD::Require<
                                LD::Detail::IsConvertible<V,U>::value
                        >>>
        inline constexpr  Unit(const Unit<U,UnitBaseTag,Tag,LD::CT::Ratio<CurrentPrefixRatioNumerator,CurrentPrefixRatioDenom>,LD::UnitExponent<LD::CT::Ratio<UnitExponentNumerator,UnitExponentDenominator>>> & unit) noexcept
        {
            (*this) = unit;
        }

        CurrentTag<T> NativeRepresentation() const noexcept
        {
            return this->mUnit;
        }

        template<typename U,template<typename> class Tag,
                LD::UInteger CurrentPrefixRatioNumerator, LD::UInteger CurrentPrefixRatioDenom,
                template<typename> class CurrentBaseTag = BaseTag,
                template<typename> class CurrentTagType = CurrentTag,
                typename V = T>
        bool operator == (const Unit<U,CurrentBaseTag,Tag,LD::CT::Ratio<CurrentPrefixRatioNumerator,CurrentPrefixRatioDenom>,LD::UnitExponent<LD::CT::Ratio<UnitExponentNumerator,UnitExponentDenominator>>> & instance) const noexcept
        {
            auto basePrefixRepresentation = (instance.mUnit.Value()*CurrentPrefixRatioNumerator)/CurrentPrefixRatioDenom;
            auto currentPrefixRepresentation = (basePrefixRepresentation*PrefixRatioDenom)/PrefixRatioNumerator;

            return this->mUnit.Value() > currentPrefixRepresentation;
        }
        template<typename U,template<typename> class Tag,
                LD::UInteger CurrentPrefixRatioNumerator, LD::UInteger CurrentPrefixRatioDenom,
                template<typename> class CurrentBaseTag = BaseTag,
                template<typename> class CurrentTagType = CurrentTag,
                typename V = T>
        bool operator != (const Unit<U,CurrentBaseTag,Tag,LD::CT::Ratio<CurrentPrefixRatioNumerator,CurrentPrefixRatioDenom>,LD::UnitExponent<LD::CT::Ratio<UnitExponentNumerator,UnitExponentDenominator>>> & instance) const noexcept
        {
            auto basePrefixRepresentation = (instance.mUnit.Value()*CurrentPrefixRatioNumerator)/CurrentPrefixRatioDenom;
            auto currentPrefixRepresentation = (basePrefixRepresentation*PrefixRatioDenom)/PrefixRatioNumerator;

            return this->mUnit.Value() > currentPrefixRepresentation;
        }
        template<typename U,template<typename> class Tag,
                LD::UInteger CurrentPrefixRatioNumerator, LD::UInteger CurrentPrefixRatioDenom,
                template<typename> class CurrentBaseTag = BaseTag,
                template<typename> class CurrentTagType = CurrentTag,
                typename V = T>
        bool operator < (const Unit<U,CurrentBaseTag,Tag,LD::CT::Ratio<CurrentPrefixRatioNumerator,CurrentPrefixRatioDenom>,LD::UnitExponent<LD::CT::Ratio<UnitExponentNumerator,UnitExponentDenominator>>> & instance) const noexcept
        {
            auto basePrefixRepresentation = (instance.mUnit.Value()*CurrentPrefixRatioNumerator)/CurrentPrefixRatioDenom;
            auto currentPrefixRepresentation = (basePrefixRepresentation*PrefixRatioDenom)/PrefixRatioNumerator;

            return this->mUnit.Value() < currentPrefixRepresentation;
        }
        template<typename U,template<typename> class Tag,
                LD::UInteger CurrentPrefixRatioNumerator, LD::UInteger CurrentPrefixRatioDenom,
                template<typename> class CurrentBaseTag = BaseTag,
                template<typename> class CurrentTagType = CurrentTag,
                typename V = T>
        bool operator > (const Unit<U,CurrentBaseTag,Tag,LD::CT::Ratio<CurrentPrefixRatioNumerator,CurrentPrefixRatioDenom>,LD::UnitExponent<LD::CT::Ratio<UnitExponentNumerator,UnitExponentDenominator>>> & instance) const noexcept
        {
            auto basePrefixRepresentation = (instance.mUnit.Value()*CurrentPrefixRatioNumerator)/CurrentPrefixRatioDenom;
            auto currentPrefixRepresentation = (basePrefixRepresentation*PrefixRatioDenom)/PrefixRatioNumerator;

            return this->mUnit.Value() > currentPrefixRepresentation;
        }
        template<typename U,template<typename> class Tag,
                LD::UInteger CurrentPrefixRatioNumerator, LD::UInteger CurrentPrefixRatioDenom,
                template<typename> class CurrentBaseTag = BaseTag,
                template<typename> class CurrentTagType = CurrentTag,
                typename V = T>
        bool operator >= (const Unit<U,CurrentBaseTag,Tag,LD::CT::Ratio<CurrentPrefixRatioNumerator,CurrentPrefixRatioDenom>,LD::UnitExponent<LD::CT::Ratio<UnitExponentNumerator,UnitExponentDenominator>>> & instance) const noexcept
        {
            auto basePrefixRepresentation = (instance.mUnit.Value()*CurrentPrefixRatioNumerator)/CurrentPrefixRatioDenom;
            auto currentPrefixRepresentation = (basePrefixRepresentation*PrefixRatioDenom)/PrefixRatioNumerator;

            return this->mUnit.Value() >= currentPrefixRepresentation;
        }
        template<typename U,template<typename> class Tag,
                LD::UInteger CurrentPrefixRatioNumerator, LD::UInteger CurrentPrefixRatioDenom,
                template<typename> class CurrentBaseTag = BaseTag,
                template<typename> class CurrentTagType = CurrentTag,
                typename V = T>
        bool operator <= (const Unit<U,CurrentBaseTag,Tag,LD::CT::Ratio<CurrentPrefixRatioNumerator,CurrentPrefixRatioDenom>,LD::UnitExponent<LD::CT::Ratio<UnitExponentNumerator,UnitExponentDenominator>>> & instance) const noexcept
        {
            auto basePrefixRepresentation = (instance.mUnit.Value()*CurrentPrefixRatioNumerator)/CurrentPrefixRatioDenom;
            auto currentPrefixRepresentation = (basePrefixRepresentation*PrefixRatioDenom)/PrefixRatioNumerator;

            return this->mUnit.Value() <= currentPrefixRepresentation;
        }
        template<typename U,template<typename> class Tag,
                LD::UInteger CurrentPrefixRatioNumerator, LD::UInteger CurrentPrefixRatioDenom,
                template<typename> class UnitBaseTag,
                template<typename> class CurrentBaseTag = BaseTag,
                template<typename> class CurrentTagType = CurrentTag,
                typename V = T>
        LD::Enable_If_T<LD::Require<
                LD::Detail::IsConvertible<V,U>::value,
                LD::CT::IsSame(LD::Type<CurrentBaseTag<U>>{},LD::Type<UnitBaseTag<U>>{}),
                LD::CT::IsSame(LD::Type<CurrentTagType<U>>{},LD::Type<Tag<U>>{})
        >,Unit> & operator = (const Unit<U,UnitBaseTag,Tag,LD::CT::Ratio<CurrentPrefixRatioNumerator,CurrentPrefixRatioDenom>,LD::UnitExponent<LD::CT::Ratio<UnitExponentNumerator,UnitExponentDenominator>>> & unit) noexcept
        {
            auto basePrefixRepresentation = (unit.mUnit.Value()*CurrentPrefixRatioNumerator)/CurrentPrefixRatioDenom;
            auto currentPrefixRepresentation = (basePrefixRepresentation*PrefixRatioDenom)/PrefixRatioNumerator;
            this->mUnit = CurrentTag<T>{currentPrefixRepresentation};
            return (*this);
        }
        template<typename U,template<typename> class Tag,
                LD::UInteger CurrentPrefixRatioNumerator, LD::UInteger CurrentPrefixRatioDenom,
                template<typename> class UnitBaseTag,
                template<typename> class CurrentBaseTag = BaseTag,
                template<typename> class CurrentTagType = CurrentTag,
                typename V = T>
        LD::Enable_If_T<LD::Require<
                LD::Detail::IsConvertible<V,U>::value,
                LD::CT::IsSame(LD::Type<CurrentBaseTag<U>>{},LD::Type<UnitBaseTag<U>>{}),
                LD::CT::Negate(LD::CT::IsSame(LD::Type<CurrentTagType<U>>{},LD::Type<Tag<U>>{}))
        >,Unit> & operator = (const Unit<U,UnitBaseTag,Tag,LD::CT::Ratio<CurrentPrefixRatioNumerator,CurrentPrefixRatioDenom>,LD::UnitExponent<LD::CT::Ratio<UnitExponentNumerator,UnitExponentDenominator>>> & unit) noexcept
        {
            auto basePrefixRepresentation = (unit.mUnit.Value()*CurrentPrefixRatioNumerator)/CurrentPrefixRatioDenom;
            decltype(LD::Declval<T>()*LD::Declval<U>()) currentPrefixRepresentation = 0;
            if constexpr (LD::CT::IsSame(LD::Type<UnitBaseTag<U>>{},LD::Type<Tag<U>>{}))
            {
                CurrentTagType<V> inputTagRepresentation;
                CurrentTagType<V> currentTagRepresentation= inputTagRepresentation(UnitBaseTag<V>{basePrefixRepresentation});
                currentPrefixRepresentation = (currentTagRepresentation.Value()*PrefixRatioDenom)/PrefixRatioNumerator;;
            }
            else if constexpr (LD::ConvertiblyCallable<Tag<V>,CurrentTag<V>(Tag<V>)>::Value())
            {
                Tag<V> inputTagRepresentation;
                CurrentTag<V> currentTagRepresentation =  inputTagRepresentation(Tag<V>{basePrefixRepresentation});
                currentPrefixRepresentation = (currentTagRepresentation.Value()*PrefixRatioDenom)/PrefixRatioNumerator;
            }else
            {
                currentPrefixRepresentation = unit.mUnit.Value();
            }
            this->mUnit = CurrentTag<T>{currentPrefixRepresentation};
            return (*this);
        }

        template<typename U,template<typename> class Tag,
                LD::UInteger CurrentPrefixRatioNumerator, LD::UInteger CurrentPrefixRatioDenom,
                template<typename> class UnitBaseTag,
                template<typename> class CurrentBaseTag,
                typename V = T>
        LD::Enable_If_T<LD::Require<
                LD::Detail::IsConvertible<V,U>::value,
                LD::CT::Negate(LD::CT::IsSame(LD::Type<CurrentBaseTag<U>>{},LD::Type<UnitBaseTag<U>>{}))
        >,Unit> & operator = (const Unit<U,UnitBaseTag,Tag,LD::CT::Ratio<CurrentPrefixRatioNumerator,CurrentPrefixRatioDenom>,LD::UnitExponent<LD::CT::Ratio<UnitExponentNumerator,UnitExponentDenominator>>> & unit) noexcept
        {

            //first convert into base unit prefix
            auto basePrefixRepresentation = (unit.mUnit.Value()*CurrentPrefixRatioNumerator)/CurrentPrefixRatioDenom;
            UnitRelationship<BaseTag<T>,CurrentBaseTag<T>> converter;
            auto currentPrefixRepresentation = (basePrefixRepresentation*PrefixRatioDenom)/PrefixRatioNumerator;
            this->mUnit = CurrentTag<T>{currentPrefixRepresentation};
            return (*this);
        }

        constexpr auto ToImmutableString() const noexcept
        {
            return LD::ToImmutableString(this->mUnit.Value());
        }

        template<typename U, typename V = T,typename R = decltype(LD::Declval<U>() * LD::Declval<V>())>
        constexpr LD::Enable_If_T<LD::Require<
                LD::Concept::Multipliable<U,V>,
                LD::CT::Negate(LD::CT::IsUnit(LD::Type<U>{}))
        >,
                Unit<R,BaseTag,CurrentTag,LD::CT::Ratio<PrefixRatioNumerator,PrefixRatioDenom>,LD::UnitExponent<LD::CT::Ratio<UnitExponentNumerator,UnitExponentDenominator>>>>
        operator * (const U & constant) noexcept(noexcept(LD::Declval<V>() * LD::Declval<U>()))
        {
            mUnit.Value()*=constant;
            return (*this);
        }

        template<typename U, typename V = T ,typename R = decltype(LD::Declval<U>() * LD::Declval<V>())>
        constexpr LD::Enable_If_T<LD::Require<
                LD::Concept::Multipliable<U,V>,
                LD::CT::Negate(LD::CT::IsUnit(LD::Type<U>{}))
        >,
                Unit<R,BaseTag,CurrentTag,LD::CT::Ratio<PrefixRatioNumerator,PrefixRatioDenom>,LD::UnitExponent<LD::CT::Ratio<UnitExponentNumerator,UnitExponentDenominator>>>&>
        operator *= (const U & constant) noexcept(noexcept(LD::Declval<V&>() *= LD::Declval<U>()))
        {
            mUnit.Value()*=constant;
            return (*this);
        }

        template<typename U, typename V = T ,typename R = decltype(LD::Declval<U>() * LD::Declval<V>())>
        constexpr LD::Enable_If_T<LD::Require<
                LD::Concept::Divisible<V,U>
        >,
                Unit<R,BaseTag,CurrentTag,LD::CT::Ratio<PrefixRatioNumerator,PrefixRatioDenom>,LD::UnitExponent<LD::CT::Ratio<UnitExponentNumerator,UnitExponentDenominator>>>>
        operator / (const U & constant) const noexcept(noexcept(LD::Declval<V>() / LD::Declval<U>()))
        {
            return Unit<R,BaseTag,CurrentTag,LD::CT::Ratio<PrefixRatioNumerator,PrefixRatioDenom>,LD::UnitExponent<LD::CT::Ratio<UnitExponentNumerator,UnitExponentDenominator>>>{mUnit.Value()/constant};
        }

        template<typename U, typename V = T ,typename R = decltype(LD::Declval<U>() * LD::Declval<V>())>
        constexpr LD::Enable_If_T<LD::Require<
                LD::Concept::AssignablyDivisible<V,U>
        >,
                Unit<R,BaseTag,CurrentTag,LD::CT::Ratio<PrefixRatioNumerator,PrefixRatioDenom>,LD::UnitExponent<LD::CT::Ratio<UnitExponentNumerator,UnitExponentDenominator>>>&>
        operator /= (const U & constant) noexcept(noexcept(LD::Declval<V&>() /= LD::Declval<U>()))
        {
            mUnit.Value()/=constant;
            return (*this);
        }

        Unit<T,BaseTag,CurrentTag,LD::CT::Ratio<PrefixRatioNumerator,PrefixRatioDenom>,LD::UnitExponent<LD::CT::Ratio<UnitExponentNumerator,UnitExponentDenominator>>> &
        operator++() noexcept
        {
            mUnit.Value()++;
            return (*this);
        };

        Unit<T,BaseTag,CurrentTag,LD::CT::Ratio<PrefixRatioNumerator,PrefixRatioDenom>,LD::UnitExponent<LD::CT::Ratio<UnitExponentNumerator,UnitExponentDenominator>>> &
        operator++(int) noexcept
        {
            mUnit.Value()++;
            return (*this);
        };

        Unit<T,BaseTag,CurrentTag,LD::CT::Ratio<PrefixRatioNumerator,PrefixRatioDenom>,LD::UnitExponent<LD::CT::Ratio<UnitExponentNumerator,UnitExponentDenominator>>> &
        operator--() noexcept
        {
            mUnit.Value()--;
            return (*this);
        };

        Unit<T,BaseTag,CurrentTag,LD::CT::Ratio<PrefixRatioNumerator,PrefixRatioDenom>,LD::UnitExponent<LD::CT::Ratio<UnitExponentNumerator,UnitExponentDenominator>>> &
        operator--(int) noexcept
        {
            mUnit.Value()--;
            return (*this);
        };

        template<typename U,
                template<typename> class Tag,
                LD::UInteger ParamPrefixRatioNumerator,
                LD::UInteger ParamPrefixRatioDenominator,
                typename V = T ,
                typename ERatio1 = LD::CT::Ratio<PrefixRatioNumerator,PrefixRatioDenom>,
                typename ERatio2 = LD::CT::Ratio<ParamPrefixRatioNumerator,ParamPrefixRatioDenominator>,
                typename U1 = LD::CT::Ratio<UnitExponentNumerator,UnitExponentDenominator>,
                typename U2 = LD::CT::Ratio<UnitExponentNumerator,UnitExponentDenominator>,
                typename R = decltype(LD::Declval<U>() + LD::Declval<V>())>
        constexpr
        LD::Enable_If_T<LD::Require<
                LD::IsSame<Tag<T>,CurrentTag<T>>,
                LD::CT::IsRatioEqual<ERatio1,ERatio2>
        >,
                Unit<R,BaseTag,CurrentTag,ERatio1,LD::UnitExponent<U1>>> operator +
                (const Unit<U,BaseTag,Tag,LD::CT::Ratio<ParamPrefixRatioNumerator,ParamPrefixRatioDenominator>,LD::UnitExponent<LD::CT::Ratio<UnitExponentNumerator,UnitExponentDenominator>>> & instance)const noexcept(noexcept(LD::Declval<V>() + LD::Declval<U>()))
        {
            auto basePrefixRepresentation = (instance.mUnit.Value()*ParamPrefixRatioNumerator)/ParamPrefixRatioDenominator;
            auto currentPrefixRepresentation = (basePrefixRepresentation*PrefixRatioDenom)/PrefixRatioNumerator;
            return Unit<R,BaseTag,Tag,ERatio1,LD::UnitExponent<U1>>{mUnit.Value() + currentPrefixRepresentation};
        }

        template<typename U,
                template<typename> class Tag,
                LD::UInteger ParamPrefixRatioNumerator,
                LD::UInteger ParamPrefixRatioDenominator>
        LD::Enable_If_T <
                LD::Require<
                        LD::IsSame<Tag<T>,CurrentTag<T>>,
                        LD::CT::IsRatioEqual<LD::CT::Ratio<PrefixRatioNumerator,PrefixRatioDenom>,LD::CT::Ratio<ParamPrefixRatioNumerator,ParamPrefixRatioDenominator>>,
                        LD::Detail::IsConvertible<decltype(LD::Declval<T>() + LD::Declval<U>()),T>::value
                >,
                Unit<T,BaseTag,CurrentTag,LD::CT::Ratio<PrefixRatioNumerator,PrefixRatioDenom>,LD::UnitExponent<LD::CT::Ratio<UnitExponentNumerator,UnitExponentDenominator>>> &
        >
        operator += (const Unit<U,BaseTag,Tag,LD::CT::Ratio<ParamPrefixRatioNumerator,ParamPrefixRatioDenominator>,LD::UnitExponent<LD::CT::Ratio<UnitExponentNumerator,UnitExponentDenominator>>> & unit) noexcept(noexcept(LD::Declval<T>() + LD::Declval<U>()))
        {
            auto basePrefixRepresentation = (unit.mUnit.Value()*ParamPrefixRatioNumerator)/ParamPrefixRatioDenominator;
            auto currentPrefixRepresentation = (basePrefixRepresentation*PrefixRatioDenom)/PrefixRatioNumerator;
            mUnit.Value() = mUnit.Value()+currentPrefixRepresentation;
            return (*this);
        }

        template<typename U,
                template<typename> class Tag,
                LD::UInteger ParamPrefixRatioNumerator,
                LD::UInteger ParamPrefixRatioDenominator,
                typename V = T ,
                typename A = decltype(LD::Declval<V>() * LD::Declval<LD::UInteger>()),
                typename B = decltype(LD::Declval<A>() / LD::Declval<LD::UInteger>()),
                typename C = decltype(LD::Declval<U>() * LD::Declval<LD::UInteger>()),
                typename D = decltype(LD::Declval<C>() / LD::Declval<LD::UInteger>()),
                typename E = decltype(LD::Declval<B>() + LD::Declval<D>()),
                typename F = decltype(LD::Declval<E>() * LD::Declval<LD::UInteger>()),
                typename R = decltype(LD::Declval<F>() / LD::Declval<LD::UInteger>())
        >
        constexpr
        LD::Enable_If_T<LD::Require<
                LD::IsSame<Tag<T>,CurrentTag<T>>,
                !LD::IsSame<LD::CT::Ratio<PrefixRatioNumerator,PrefixRatioDenom>,LD::CT::Ratio<ParamPrefixRatioNumerator,ParamPrefixRatioDenominator>>
        >,
                Unit<R,BaseTag,CurrentTag,LD::CT::Ratio<PrefixRatioNumerator,PrefixRatioDenom>,LD::UnitExponent<LD::CT::Ratio<UnitExponentNumerator,UnitExponentDenominator>>>> operator +
                (const Unit<U,BaseTag,Tag,LD::CT::Ratio<ParamPrefixRatioNumerator,ParamPrefixRatioDenominator>,LD::UnitExponent<LD::CT::Ratio<UnitExponentNumerator,UnitExponentDenominator>>> & instance) const noexcept
        {

            A instanceRatioMultiplication = this->mUnit.Value()*PrefixRatioDenom;
            B instanceUnitAsBaseRatio = instanceRatioMultiplication/PrefixRatioNumerator;
            C paramRationMultiplication = instance.mUnit.Value()*ParamPrefixRatioDenominator;
            D paramUnitAsBaseRatio = paramRationMultiplication/ParamPrefixRatioNumerator;


            E resultantAddition = instanceUnitAsBaseRatio + paramUnitAsBaseRatio;


            F resultantWithRatioNumerator = resultantAddition*PrefixRatioNumerator;
            R resultantWithCurrentRatio = resultantWithRatioNumerator/PrefixRatioDenom;

            return Unit<R,BaseTag,Tag,LD::CT::Ratio<PrefixRatioNumerator,PrefixRatioDenom>,LD::UnitExponent<LD::CT::Ratio<UnitExponentNumerator,UnitExponentDenominator>>>{resultantWithCurrentRatio};
        }

        template<typename U,
                template<typename> class Tag,
                LD::UInteger ParamPrefixRatioNumerator,
                LD::UInteger ParamPrefixRatioDenominator,
                typename V = T,
                typename A = decltype(LD::Declval<V>() * LD::Declval<LD::UInteger>()),
                typename B = decltype(LD::Declval<A>() / LD::Declval<LD::UInteger>()),
                typename C = decltype(LD::Declval<U>() * LD::Declval<LD::UInteger>()),
                typename D = decltype(LD::Declval<C>() / LD::Declval<LD::UInteger>()),
                typename E = decltype(LD::Declval<B>() + LD::Declval<D>()),
                typename F = decltype(LD::Declval<E>() * LD::Declval<LD::UInteger>()),
                typename R = decltype(LD::Declval<F>() / LD::Declval<LD::UInteger>())>
        LD::Enable_If_T <
                LD::Require<
                        LD::IsSame<Tag<T>,CurrentTag<T>>,
                        !LD::CT::IsRatioEqual<LD::CT::Ratio<PrefixRatioNumerator,PrefixRatioDenom>,LD::CT::Ratio<ParamPrefixRatioNumerator,ParamPrefixRatioDenominator>>,
                        LD::Detail::IsConvertible<decltype(LD::Declval<T>() + LD::Declval<U>()),T>::value
                >,
                Unit<T,BaseTag,CurrentTag,LD::CT::Ratio<PrefixRatioNumerator,PrefixRatioDenom>,LD::UnitExponent<LD::CT::Ratio<UnitExponentNumerator,UnitExponentDenominator>>> &
        >
        operator += (const Unit<U,BaseTag,Tag,LD::CT::Ratio<ParamPrefixRatioNumerator,ParamPrefixRatioDenominator>,LD::UnitExponent<LD::CT::Ratio<UnitExponentNumerator,UnitExponentDenominator>>> & instance) noexcept(noexcept(LD::Declval<T>() + LD::Declval<U>()))
        {

            A instanceRatioMultiplication = this->mUnit.Value()*PrefixRatioDenom;
            B instanceUnitAsBaseRatio = instanceRatioMultiplication/PrefixRatioNumerator;
            C paramRationMultiplication = instance.mUnit.Value()*ParamPrefixRatioDenominator;
            D paramUnitAsBaseRatio = paramRationMultiplication/ParamPrefixRatioNumerator;


            E resultantAddition = instanceUnitAsBaseRatio + paramUnitAsBaseRatio;


            F resultantWithRatioNumerator = resultantAddition*PrefixRatioNumerator;
            R resultantWithCurrentRatio = resultantWithRatioNumerator/PrefixRatioDenom;
            mUnit.Value() = resultantWithCurrentRatio;
            return (*this);
        }

        template<typename U,
                template<typename> class Tag,
                LD::UInteger ParamPrefixRatioNumerator,
                LD::UInteger ParamPrefixRatioDenominator,
                typename V = T ,
                typename R = decltype(LD::Declval<U>() + LD::Declval<V>())>
        constexpr
        LD::Enable_If_T<LD::Require<
                !LD::IsSame<Tag<T>,CurrentTag<T>>,
                LD::CT::IsRatioEqual<LD::CT::Ratio<PrefixRatioNumerator,PrefixRatioDenom>,LD::CT::Ratio<ParamPrefixRatioNumerator,ParamPrefixRatioDenominator>>
        >,
                Unit<R,BaseTag,CurrentTag,LD::CT::Ratio<PrefixRatioNumerator,PrefixRatioDenom>,LD::UnitExponent<LD::CT::Ratio<UnitExponentNumerator,UnitExponentDenominator>>>> operator +
                (const Unit<U,BaseTag,Tag,LD::CT::Ratio<ParamPrefixRatioNumerator,ParamPrefixRatioDenominator>,LD::UnitExponent<LD::CT::Ratio<UnitExponentNumerator,UnitExponentDenominator>>> & instance) const noexcept
        {

            UnitRelationship<BaseTag<U>,Tag<U>> paramToBaseTagConverter;
            UnitRelationship<BaseTag<T>,CurrentTag<T>> instanceTagConverter;
            UnitRelationship<BaseTag<R>,CurrentTag<R>> resultantTagConverter;

            BaseTag<T> covertedInstanceTagToBaseTag = instanceTagConverter(this->mUnit);
            BaseTag<U> convertedParameterTagToBaseTag = paramToBaseTagConverter(instance.mUnit);

            R additionResult = covertedInstanceTagToBaseTag.Value() + convertedParameterTagToBaseTag.Value();

            CurrentTag<R> resultantTag = resultantTagConverter(BaseTag<R>{additionResult});


            return Unit<R,BaseTag,Tag,LD::CT::Ratio<PrefixRatioNumerator,PrefixRatioDenom>,LD::UnitExponent<LD::CT::Ratio<UnitExponentNumerator,UnitExponentDenominator>>>{resultantTag.Value()};
        }

        template<typename U,
                template<typename> class Tag,
                LD::UInteger ParamPrefixRatioNumerator,
                LD::UInteger ParamPrefixRatioDenominator,
                typename V = T,
                typename R = decltype(LD::Declval<U>() + LD::Declval<V>())>
        LD::Enable_If_T <
                LD::Require<
                        !LD::IsSame<Tag<T>,CurrentTag<T>>,
                        LD::CT::IsRatioEqual<LD::CT::Ratio<PrefixRatioNumerator,PrefixRatioDenom>,LD::CT::Ratio<ParamPrefixRatioNumerator,ParamPrefixRatioDenominator>>,
                        LD::Detail::IsConvertible<decltype(LD::Declval<T>() + LD::Declval<U>()),T>::value
                >,
                Unit<T,BaseTag,CurrentTag,LD::CT::Ratio<PrefixRatioNumerator,PrefixRatioDenom>,LD::UnitExponent<LD::CT::Ratio<UnitExponentNumerator,UnitExponentDenominator>>> &
        >
        operator += (const Unit<U,BaseTag,Tag,LD::CT::Ratio<ParamPrefixRatioNumerator,ParamPrefixRatioDenominator>,LD::UnitExponent<LD::CT::Ratio<UnitExponentNumerator,UnitExponentDenominator>>> & instance) noexcept(noexcept(LD::Declval<T>() + LD::Declval<U>()))
        {
            UnitRelationship<BaseTag<U>,Tag<U>> paramToBaseTagConverter;
            UnitRelationship<BaseTag<T>,CurrentTag<T>> instanceTagConverter;
            UnitRelationship<BaseTag<R>,CurrentTag<R>> resultantTagConverter;

            BaseTag<T> covertedInstanceTagToBaseTag = instanceTagConverter(this->mUnit);
            BaseTag<U> convertedParameterTagToBaseTag = paramToBaseTagConverter(instance.mUnit);

            R additionResult = covertedInstanceTagToBaseTag.Value() + convertedParameterTagToBaseTag.Value();
            mUnit.Value() = additionResult;
            return (*this);
        }

        template<typename U,
                template<typename> class Tag,
                LD::UInteger ParamPrefixRatioNumerator,
                LD::UInteger ParamPrefixRatioDenominator,
                typename V = T ,
                typename A = decltype(LD::Declval<V>() * LD::Declval<LD::UInteger>()),
                typename B = decltype(LD::Declval<A>() / LD::Declval<LD::UInteger>()),
                typename C = decltype(LD::Declval<U>() * LD::Declval<LD::UInteger>()),
                typename D = decltype(LD::Declval<C>() / LD::Declval<LD::UInteger>()),
                typename E = decltype(LD::Declval<B>() + LD::Declval<D>()),
                typename F = decltype(LD::Declval<E>() * LD::Declval<LD::UInteger>()),
                typename R = decltype(LD::Declval<F>() / LD::Declval<LD::UInteger>())
        >
        constexpr
        LD::Enable_If_T<LD::Require<
                !LD::IsSame<Tag<T>,CurrentTag<T>>,
                !LD::CT::IsRatioEqual<LD::CT::Ratio<PrefixRatioNumerator,PrefixRatioDenom>,LD::CT::Ratio<ParamPrefixRatioNumerator,ParamPrefixRatioDenominator>>
        >,
                Unit<R,BaseTag,CurrentTag,LD::CT::Ratio<PrefixRatioNumerator,PrefixRatioDenom>,LD::UnitExponent<LD::CT::Ratio<UnitExponentNumerator,UnitExponentDenominator>>>> operator +
                (const Unit<U,BaseTag,Tag,LD::CT::Ratio<ParamPrefixRatioNumerator,ParamPrefixRatioDenominator>,LD::UnitExponent<LD::CT::Ratio<UnitExponentNumerator,UnitExponentDenominator>>> & instance) const noexcept
        {
            UnitRelationship<BaseTag<T>,CurrentTag<T>> instanceTagConverter;
            UnitRelationship<BaseTag<U>,Tag<U>> paramToBaseTagConverter;
            UnitRelationship<BaseTag<R>,Tag<R>> resultantTagConverter;

            BaseTag<T> instanceTagToBaseTag = instanceTagConverter(this->mUnit);
            BaseTag<T> paramTagToBaseTag = paramToBaseTagConverter(instance);


            A instanceRatioMultiplication = instanceTagToBaseTag.Value()*PrefixRatioDenom;
            B instanceUnitAsBaseRatio = instanceRatioMultiplication/PrefixRatioNumerator;
            C paramRationMultiplication = paramTagToBaseTag.Value()*ParamPrefixRatioDenominator;
            D paramUnitAsBaseRatio = paramRationMultiplication/ParamPrefixRatioNumerator;



            E resultantAddition = instanceUnitAsBaseRatio + paramUnitAsBaseRatio;



            F resultantWithRatioNumerator = resultantAddition*PrefixRatioNumerator;
            R resultantWithCurrentRatioAsBaseTag = resultantWithRatioNumerator/PrefixRatioDenom;

            CurrentTag<R> resultantTag = resultantTagConverter(BaseTag<R>{resultantWithCurrentRatioAsBaseTag});
            return Unit<R,BaseTag,Tag,LD::CT::Ratio<PrefixRatioNumerator,PrefixRatioDenom>,LD::UnitExponent<LD::CT::Ratio<UnitExponentNumerator,UnitExponentDenominator>>>{resultantTag.Value()};
        }

        template<typename U,
                template<typename> class Tag,
                LD::UInteger ParamPrefixRatioNumerator,
                LD::UInteger ParamPrefixRatioDenominator,
                typename V = T ,
                typename A = decltype(LD::Declval<V>() * LD::Declval<LD::UInteger>()),
                typename B = decltype(LD::Declval<A>() / LD::Declval<LD::UInteger>()),
                typename C = decltype(LD::Declval<U>() * LD::Declval<LD::UInteger>()),
                typename D = decltype(LD::Declval<C>() / LD::Declval<LD::UInteger>()),
                typename E = decltype(LD::Declval<B>() + LD::Declval<D>()),
                typename F = decltype(LD::Declval<E>() * LD::Declval<LD::UInteger>()),
                typename R = decltype(LD::Declval<F>() / LD::Declval<LD::UInteger>())>
        LD::Enable_If_T <
                LD::Require<
                        !LD::IsSame<Tag<T>,CurrentTag<T>>,
                        !LD::CT::IsRatioEqual<LD::CT::Ratio<PrefixRatioNumerator,PrefixRatioDenom>,LD::CT::Ratio<ParamPrefixRatioNumerator,ParamPrefixRatioDenominator>>,
                        LD::Detail::IsConvertible<decltype(LD::Declval<T>() + LD::Declval<U>()),T>::value
                >,
                Unit<T,BaseTag,CurrentTag,LD::CT::Ratio<PrefixRatioNumerator,PrefixRatioDenom>,LD::UnitExponent<LD::CT::Ratio<UnitExponentNumerator,UnitExponentDenominator>>> &
        >
        operator += (const Unit<U,BaseTag,Tag,LD::CT::Ratio<ParamPrefixRatioNumerator,ParamPrefixRatioDenominator>,LD::UnitExponent<LD::CT::Ratio<UnitExponentNumerator,UnitExponentDenominator>>> & instance) noexcept(noexcept(LD::Declval<T>() + LD::Declval<U>()))
        {
            UnitRelationship<BaseTag<T>,CurrentTag<T>> instanceTagConverter;
            UnitRelationship<BaseTag<U>,Tag<U>> paramToBaseTagConverter;
            UnitRelationship<BaseTag<R>,Tag<R>> resultantTagConverter;

            BaseTag<T> instanceTagToBaseTag = instanceTagConverter(this->mUnit);
            BaseTag<T> paramTagToBaseTag = paramToBaseTagConverter(instance);

            A instanceRatioMultiplication = instanceTagToBaseTag.Value()*PrefixRatioDenom;
            B instanceUnitAsBaseRatio = instanceRatioMultiplication/PrefixRatioNumerator;
            C paramRationMultiplication = paramTagToBaseTag.Value()*ParamPrefixRatioDenominator;
            D paramUnitAsBaseRatio = paramRationMultiplication/ParamPrefixRatioNumerator;

            E resultantAddition = instanceUnitAsBaseRatio + paramUnitAsBaseRatio;



            F resultantWithRatioNumerator = resultantAddition*PrefixRatioNumerator;
            R resultantWithCurrentRatioAsBaseTag = resultantWithRatioNumerator/PrefixRatioDenom;
            mUnit.Value() = resultantWithCurrentRatioAsBaseTag;
            return (*this);
        }

        template<typename U,
                template<typename> class Tag,
                LD::UInteger ParamPrefixRatioNumerator,
                LD::UInteger ParamPrefixRatioDenominator,
                typename V = T ,
                typename R = decltype(LD::Declval<U>() + LD::Declval<V>())>
        constexpr
        LD::Enable_If_T<LD::Require<
                LD::IsSame<Tag<T>,CurrentTag<T>>,
                LD::CT::IsRatioEqual<LD::CT::Ratio<PrefixRatioNumerator,PrefixRatioDenom>,LD::CT::Ratio<ParamPrefixRatioNumerator,ParamPrefixRatioDenominator>>
        >,
                Unit<R,BaseTag,CurrentTag,LD::CT::Ratio<PrefixRatioNumerator,PrefixRatioDenom>,LD::UnitExponent<LD::CT::Ratio<UnitExponentNumerator,UnitExponentDenominator>>>> operator -
                (const Unit<U,BaseTag,Tag,LD::CT::Ratio<ParamPrefixRatioNumerator,ParamPrefixRatioDenominator>,LD::UnitExponent<LD::CT::Ratio<UnitExponentNumerator,UnitExponentDenominator>>> & instance) const noexcept
        {
            return Unit<R,BaseTag,Tag,LD::CT::Ratio<PrefixRatioNumerator,PrefixRatioDenom>,LD::UnitExponent<LD::CT::Ratio<UnitExponentNumerator,UnitExponentDenominator>>>{mUnit.Value() - instance.mUnit.Value()};
        }
        template<typename U,
                template<typename> class Tag,
                LD::UInteger ParamPrefixRatioNumerator,
                LD::UInteger ParamPrefixRatioDenominator,
                typename V = T ,
                typename A = decltype(LD::Declval<V>() * LD::Declval<LD::UInteger>()),
                typename B = decltype(LD::Declval<A>() / LD::Declval<LD::UInteger>()),
                typename C = decltype(LD::Declval<U>() * LD::Declval<LD::UInteger>()),
                typename D = decltype(LD::Declval<C>() / LD::Declval<LD::UInteger>()),
                typename E = decltype(LD::Declval<B>() + LD::Declval<D>()),
                typename F = decltype(LD::Declval<E>() * LD::Declval<LD::UInteger>()),
                typename R = decltype(LD::Declval<F>() / LD::Declval<LD::UInteger>())
        >
        constexpr
        LD::Enable_If_T<LD::Require<
                LD::IsSame<Tag<T>,CurrentTag<T>>,
                !LD::CT::IsRatioEqual<LD::CT::Ratio<PrefixRatioNumerator,PrefixRatioDenom>,LD::CT::Ratio<ParamPrefixRatioNumerator,ParamPrefixRatioDenominator>>
        >,
                Unit<R,BaseTag,CurrentTag,LD::CT::Ratio<PrefixRatioNumerator,PrefixRatioDenom>,LD::UnitExponent<LD::CT::Ratio<UnitExponentNumerator,UnitExponentDenominator>>>> operator -
                (const Unit<U,BaseTag,Tag,LD::CT::Ratio<ParamPrefixRatioNumerator,ParamPrefixRatioDenominator>,LD::UnitExponent<LD::CT::Ratio<UnitExponentNumerator,UnitExponentDenominator>>> & instance) const noexcept
        {

            A instanceRatioMultiplication = this->mUnit.Value()*PrefixRatioDenom;
            B instanceUnitAsBaseRatio = instanceRatioMultiplication/PrefixRatioNumerator;
            C paramRationMultiplication = instance.mUnit.Value()*ParamPrefixRatioDenominator;
            D paramUnitAsBaseRatio = paramRationMultiplication/ParamPrefixRatioNumerator;


            E resultantAddition = instanceUnitAsBaseRatio - paramUnitAsBaseRatio;


            F resultantWithRatioNumerator = resultantAddition*PrefixRatioNumerator;
            R resultantWithCurrentRatio = resultantWithRatioNumerator/PrefixRatioDenom;

            return Unit<R,BaseTag,Tag,LD::CT::Ratio<PrefixRatioNumerator,PrefixRatioDenom>,LD::UnitExponent<LD::CT::Ratio<UnitExponentNumerator,UnitExponentDenominator>>>{resultantWithCurrentRatio};
        }

        template<typename U,
                template<typename> class Tag,
                LD::UInteger ParamPrefixRatioNumerator,
                LD::UInteger ParamPrefixRatioDenominator,
                typename V = T ,
                typename R = decltype(LD::Declval<U>() + LD::Declval<V>())>
        constexpr
        LD::Enable_If_T<LD::Require<
                !LD::IsSame<Tag<T>,CurrentTag<T>>,
                LD::CT::IsRatioEqual<LD::CT::Ratio<PrefixRatioNumerator,PrefixRatioDenom>,LD::CT::Ratio<ParamPrefixRatioNumerator,ParamPrefixRatioDenominator>>
        >,
                Unit<R,BaseTag,CurrentTag,LD::CT::Ratio<PrefixRatioNumerator,PrefixRatioDenom>,LD::UnitExponent<LD::CT::Ratio<UnitExponentNumerator,UnitExponentDenominator>>>> operator -
                (const Unit<U,BaseTag,Tag,LD::CT::Ratio<ParamPrefixRatioNumerator,ParamPrefixRatioDenominator>,LD::UnitExponent<LD::CT::Ratio<UnitExponentNumerator,UnitExponentDenominator>>> & instance) const noexcept
        {

            UnitRelationship<BaseTag<U>,Tag<U>> paramToBaseTagConverter;
            UnitRelationship<BaseTag<T>,CurrentTag<T>> instanceTagConverter;
            UnitRelationship<BaseTag<R>,CurrentTag<R>> resultantTagConverter;

            BaseTag<T> covertedInstanceTagToBaseTag = instanceTagConverter(this->mUnit);
            BaseTag<U> convertedParameterTagToBaseTag = paramToBaseTagConverter(instance.mUnit);

            R additionResult = covertedInstanceTagToBaseTag.Value() - convertedParameterTagToBaseTag.Value();

            CurrentTag<R> resultantTag = resultantTagConverter(BaseTag<R>{additionResult});


            return Unit<R,BaseTag,Tag,LD::CT::Ratio<PrefixRatioNumerator,PrefixRatioDenom>,LD::UnitExponent<LD::CT::Ratio<UnitExponentNumerator,UnitExponentDenominator>>>{resultantTag.Value()};
        }

        template<typename U,
                template<typename> class Tag,
                LD::UInteger ParamPrefixRatioNumerator,
                LD::UInteger ParamPrefixRatioDenominator,
                typename V = T ,
                typename A = decltype(LD::Declval<V>() * LD::Declval<LD::UInteger>()),
                typename B = decltype(LD::Declval<A>() / LD::Declval<LD::UInteger>()),
                typename C = decltype(LD::Declval<U>() * LD::Declval<LD::UInteger>()),
                typename D = decltype(LD::Declval<C>() / LD::Declval<LD::UInteger>()),
                typename E = decltype(LD::Declval<B>() + LD::Declval<D>()),
                typename F = decltype(LD::Declval<E>() * LD::Declval<LD::UInteger>()),
                typename R = decltype(LD::Declval<F>() / LD::Declval<LD::UInteger>())
        >
        constexpr
        LD::Enable_If_T<LD::Require<
                !LD::IsSame<Tag<T>,CurrentTag<T>>,
                !LD::CT::IsRatioEqual<LD::CT::Ratio<PrefixRatioNumerator,PrefixRatioDenom>,LD::CT::Ratio<ParamPrefixRatioNumerator,ParamPrefixRatioDenominator>>
        >,
                Unit<R,BaseTag,CurrentTag,LD::CT::Ratio<PrefixRatioNumerator,PrefixRatioDenom>,LD::UnitExponent<LD::CT::Ratio<UnitExponentNumerator,UnitExponentDenominator>>>> operator -
                (const Unit<U,BaseTag,Tag,LD::CT::Ratio<ParamPrefixRatioNumerator,ParamPrefixRatioDenominator>,LD::UnitExponent<LD::CT::Ratio<UnitExponentNumerator,UnitExponentDenominator>>> & instance) const noexcept
        {
            UnitRelationship<BaseTag<T>,CurrentTag<T>> instanceTagConverter;
            UnitRelationship<BaseTag<U>,Tag<U>> paramToBaseTagConverter;
            UnitRelationship<BaseTag<R>,Tag<R>> resultantTagConverter;

            BaseTag<T> instanceTagToBaseTag = instanceTagConverter(this->mUnit);
            BaseTag<T> paramTagToBaseTag = paramToBaseTagConverter(instance);


            A instanceRatioMultiplication = instanceTagToBaseTag.Value()*PrefixRatioDenom;
            B instanceUnitAsBaseRatio = instanceRatioMultiplication/PrefixRatioNumerator;
            C paramRationMultiplication = paramTagToBaseTag.Value()*ParamPrefixRatioDenominator;
            D paramUnitAsBaseRatio = paramRationMultiplication/ParamPrefixRatioNumerator;



            E resultantAddition = instanceUnitAsBaseRatio - paramUnitAsBaseRatio;



            F resultantWithRatioNumerator = resultantAddition*PrefixRatioNumerator;
            R resultantWithCurrentRatioAsBaseTag = resultantWithRatioNumerator/PrefixRatioDenom;

            CurrentTag<R> resultantTag = resultantTagConverter(BaseTag<R>{resultantWithCurrentRatioAsBaseTag});
            return Unit<R,BaseTag,Tag,LD::CT::Ratio<PrefixRatioNumerator,PrefixRatioDenom>,LD::UnitExponent<LD::CT::Ratio<UnitExponentNumerator,UnitExponentDenominator>>>{resultantTag.Value()};
        }

        template<typename U,LD::UInteger ParamRatioNumerator, LD::UInteger ParamRatioDenominator,LD::UInteger ParamUnitNumerator, LD::UInteger ParamUnitDenominator,template<typename> class Tag ,typename V = T, typename R = decltype(LD::Declval<V>() * LD::Declval<T>())>
        constexpr LD::Enable_If_T<LD::Require<
                LD::CT::IsRatioIdentity<LD::CT::Ratio<PrefixRatioNumerator,PrefixRatioDenom>>,
                LD::CT::IsRatioIdentity<LD::CT::Ratio<ParamRatioNumerator,ParamRatioDenominator>>,
                LD::IsSame<CurrentTag<T>,Tag<T>>,
                LD::CT::IsRatioEqual<LD::CT::Ratio<PrefixRatioNumerator,PrefixRatioDenom>,LD::CT::Ratio<ParamRatioNumerator,ParamRatioDenominator>>
        >,
                Unit<R,BaseTag,CurrentTag,LD::CT::Ratio<PrefixRatioNumerator,PrefixRatioDenom>,LD::UnitExponent<LD::CT::Ratio<2,1>>>>
        operator * (const Unit<U,BaseTag,Tag,LD::CT::Ratio<ParamRatioNumerator,ParamRatioDenominator>,LD::UnitExponent<LD::CT::Ratio<ParamUnitNumerator,ParamUnitDenominator>>> & unit) const noexcept(noexcept(LD::Declval<V>()*LD::Declval<U>()))
        {
            return Unit<R,BaseTag,CurrentTag,LD::CT::Ratio<PrefixRatioNumerator,PrefixRatioDenom>,LD::UnitExponent<LD::CT::Ratio<2,1>>>{mUnit.Value()*unit.mUnit.Value()};
        };

        template<typename U,LD::UInteger ParamRatioNumerator,
                LD::UInteger ParamRatioDenominator,
                LD::UInteger ParamUnitNumerator,
                LD::UInteger ParamUnitDenominator,
                template<typename> class Tag ,
                typename V = T,
                typename A = decltype(LD::Declval<T>() * LD::Declval<LD::UInteger>()),
                typename B = decltype(LD::Declval<A>() / LD::Declval<LD::UInteger>()),
                typename C = decltype(LD::Declval<U>() * LD::Declval<LD::UInteger>()),
                typename D = decltype(LD::Declval<C>() / LD::Declval<LD::UInteger>()),
                typename E = decltype(LD::Declval<B>() * LD::Declval<D>()),
                typename F = decltype(LD::Declval<E>() * LD::Declval<LD::UInteger>()),
                typename R = decltype(LD::Declval<F>() / LD::Declval<LD::UInteger>())
        >
        constexpr LD::Enable_If_T<LD::Require<
                LD::CT::IsRatioIdentity<LD::CT::Ratio<PrefixRatioNumerator,PrefixRatioDenom>>,
                LD::CT::IsRatioIdentity<LD::CT::Ratio<ParamRatioNumerator,ParamRatioDenominator>>,
                LD::IsSame<CurrentTag<T>,Tag<T>>,
                !LD::CT::IsRatioEqual<LD::CT::Ratio<PrefixRatioNumerator,PrefixRatioDenom>,LD::CT::Ratio<ParamRatioNumerator,ParamRatioDenominator>>
        >,
                Unit<R,BaseTag,CurrentTag,LD::CT::Ratio<PrefixRatioNumerator,PrefixRatioDenom>,LD::UnitExponent<LD::CT::Ratio<2,1>>>>
        operator * (const Unit<U,BaseTag,Tag,LD::CT::Ratio<ParamRatioNumerator,ParamRatioDenominator>,LD::UnitExponent<LD::CT::Ratio<ParamUnitNumerator,ParamUnitDenominator>>> & unit) const noexcept(noexcept(LD::Declval<V>()*LD::Declval<U>()))
        {
            A multiplicand = mUnit.Value()*PrefixRatioDenom;
            B instanceAsBaseRatio = multiplicand/PrefixRatioNumerator;
            C paramMultiplicand = unit.mUnit.Value()*ParamRatioDenominator;
            D paramAsBaseRatio = instanceAsBaseRatio/ParamRatioNumerator;
            E resultantAsBaseRatio = instanceAsBaseRatio*paramAsBaseRatio;
            F resultantmultiplicand = resultantAsBaseRatio*PrefixRatioNumerator;
            R resultantValue = resultantmultiplicand/PrefixRatioDenom;
            return Unit<R,BaseTag,CurrentTag,LD::CT::Ratio<PrefixRatioNumerator,PrefixRatioDenom>,LD::UnitExponent<LD::CT::Ratio<UnitExponentNumerator,UnitExponentDenominator>>>{resultantValue};
        };

        template<typename U,LD::UInteger ParamRatioNumerator,
                LD::UInteger ParamRatioDenominator,
                LD::UInteger ParamUnitNumerator,
                LD::UInteger ParamUnitDenominator,
                template<typename> class Tag ,
                typename V = T,
                typename R = decltype(LD::Declval<V>() * LD::Declval<T>())>
        constexpr LD::Enable_If_T<LD::Require<
                LD::CT::IsRatioIdentity<LD::CT::Ratio<PrefixRatioNumerator,PrefixRatioDenom>>,
                LD::CT::IsRatioIdentity<LD::CT::Ratio<ParamRatioNumerator,ParamRatioDenominator>>,
                !LD::IsSame<CurrentTag<T>,Tag<T>>,
                LD::CT::IsRatioEqual<LD::CT::Ratio<PrefixRatioNumerator,PrefixRatioDenom>,LD::CT::Ratio<ParamRatioNumerator,ParamRatioDenominator>>
        >,
                Unit<R,BaseTag,CurrentTag,LD::CT::Ratio<PrefixRatioNumerator,PrefixRatioDenom>,LD::UnitExponent<LD::CT::Ratio<2,1>>>>
        operator * (const Unit<U,BaseTag,Tag,LD::CT::Ratio<ParamRatioNumerator,ParamRatioDenominator>,LD::UnitExponent<LD::CT::Ratio<ParamUnitNumerator,ParamUnitDenominator>>> & unit) const noexcept(noexcept(LD::Declval<V>()*LD::Declval<U>()))
        {
            UnitRelationship<BaseTag<T>,CurrentTag<T>> instanceTagConverter;
            UnitRelationship<BaseTag<U>,Tag<U>> paramTagConverter;
            UnitRelationship<BaseTag<R>,Tag<R>> resultantTagConverter;
            BaseTag<T> instanceTagAsBaseTag = instanceTagConverter(this->mUnit);
            BaseTag<U> paramTagAsBaseTag = paramTagConverter(unit.mUnit);
            R result = instanceTagAsBaseTag.Value()*paramTagAsBaseTag.Value();
            CurrentTag<R> resultantTag = resultantTagConverter(BaseTag<R>(result));
            return Unit<R,BaseTag,CurrentTag,LD::CT::Ratio<PrefixRatioNumerator,PrefixRatioDenom>,LD::UnitExponent<LD::CT::Ratio<UnitExponentNumerator,UnitExponentDenominator>>>{resultantTag};
        };

        template<typename U,LD::UInteger ParamRatioNumerator,
                LD::UInteger ParamRatioDenominator,
                LD::UInteger ParamUnitNumerator,
                LD::UInteger ParamUnitDenominator,
                template<typename> class Tag ,
                typename V = T,
                typename A = decltype(LD::Declval<T>() * LD::Declval<LD::UInteger>()),
                typename B = decltype(LD::Declval<A>() / LD::Declval<LD::UInteger>()),
                typename C = decltype(LD::Declval<U>() * LD::Declval<LD::UInteger>()),
                typename D = decltype(LD::Declval<C>() / LD::Declval<LD::UInteger>()),
                typename E = decltype(LD::Declval<B>() * LD::Declval<D>()),
                typename F = decltype(LD::Declval<E>() * LD::Declval<LD::UInteger>()),
                typename R = decltype(LD::Declval<F>() / LD::Declval<LD::UInteger>())
        >
        constexpr LD::Enable_If_T<LD::Require<
                LD::CT::IsRatioIdentity<LD::CT::Ratio<PrefixRatioNumerator,PrefixRatioDenom>>,
                LD::CT::IsRatioIdentity<LD::CT::Ratio<ParamRatioNumerator,ParamRatioDenominator>>,
                !LD::IsSame<CurrentTag<T>,Tag<T>>,
                !LD::CT::IsRatioEqual<LD::CT::Ratio<PrefixRatioNumerator,PrefixRatioDenom>,LD::CT::Ratio<ParamRatioNumerator,ParamRatioDenominator>>
        >,
                Unit<R,BaseTag,CurrentTag,LD::CT::Ratio<PrefixRatioNumerator,PrefixRatioDenom>,LD::UnitExponent<LD::CT::Ratio<2,1>>>>
        operator * (const Unit<U,BaseTag,Tag,LD::CT::Ratio<ParamRatioNumerator,ParamRatioDenominator>,LD::UnitExponent<LD::CT::Ratio<ParamUnitNumerator,ParamUnitDenominator>>> & unit) const noexcept(noexcept(LD::Declval<V>()*LD::Declval<U>()))
        {
            UnitRelationship<BaseTag<T>,CurrentTag<T>> instanceTagConverter;
            UnitRelationship<BaseTag<U>,Tag<U>> paramTagConverter;
            UnitRelationship<BaseTag<R>,Tag<R>> resultantTagConverter;
            BaseTag<T> instanceTagAsBaseTag = instanceTagConverter(this->mUnit);
            BaseTag<U> paramTagAsBaseTag = paramTagConverter(unit.mUnit);
            A multiplicand = instanceTagAsBaseTag.Value()*PrefixRatioDenom;
            B instanceAsBaseRatio = multiplicand/PrefixRatioNumerator;
            C paramMultiplicand = paramTagAsBaseTag.Value()*ParamRatioDenominator;
            D paramAsBaseRatio = paramMultiplicand/ParamRatioNumerator;
            E resultantAsBaseRatio = instanceAsBaseRatio*paramAsBaseRatio;
            F resultantMultiplicand = resultantAsBaseRatio*PrefixRatioNumerator;
            R resultantAsBaseRatioAndBaseTag = resultantMultiplicand/PrefixRatioDenom;
            CurrentTag<R> resultantTag = resultantTagConverter(BaseTag<R>{resultantAsBaseRatioAndBaseTag});
            return Unit<R,BaseTag,CurrentTag,LD::CT::Ratio<PrefixRatioNumerator,PrefixRatioDenom>,LD::UnitExponent<LD::CT::Ratio<UnitExponentNumerator,UnitExponentDenominator>>>{resultantTag};
        };

        template<typename U,LD::UInteger ParamRatioNumerator,
                LD::UInteger ParamRatioDenominator,
                LD::UInteger ParamUnitNumerator,
                LD::UInteger ParamUnitDenominator,
                template<typename> class Tag ,
                typename V = T,
                typename R = decltype(LD::Declval<V>() * LD::Declval<T>())>
        constexpr LD::Enable_If_T<LD::Require<
                !LD::CT::IsRatioIdentity<LD::CT::Ratio<UnitExponentNumerator,UnitExponentDenominator>>,
                !LD::CT::IsRatioIdentity<LD::CT::Ratio<UnitExponentNumerator,UnitExponentDenominator>>,
                LD::CT::IsRatioRecipricol<LD::CT::Ratio<UnitExponentNumerator,UnitExponentDenominator>,LD::CT::Ratio<UnitExponentNumerator,UnitExponentDenominator>>,
                LD::IsSame<CurrentTag<T>,Tag<T>>,
                LD::CT::IsRatioEqual<LD::CT::Ratio<PrefixRatioNumerator,PrefixRatioDenom>,LD::CT::Ratio<ParamRatioNumerator,ParamRatioDenominator>>
        >,
                R
        >
        operator * (const Unit<U,BaseTag,Tag,LD::CT::Ratio<ParamRatioNumerator,ParamRatioDenominator>,LD::UnitExponent<LD::CT::Ratio<ParamUnitNumerator,ParamUnitDenominator>>> & unit) const noexcept(noexcept(LD::Declval<V>()*LD::Declval<U>()))
        {
            return R{mUnit.Value() * unit.mUnit.Value()};
        };

        template<typename U,LD::UInteger ParamRatioNumerator,
                LD::UInteger ParamRatioDenominator,
                LD::UInteger ParamUnitNumerator,
                LD::UInteger ParamUnitDenominator,
                template<typename> class Tag ,
                typename V = T,
                typename A = decltype(LD::Declval<T>() * LD::Declval<LD::UInteger>()),
                typename B = decltype(LD::Declval<A>() / LD::Declval<LD::UInteger>()),
                typename C = decltype(LD::Declval<U>() * LD::Declval<LD::UInteger>()),
                typename D = decltype(LD::Declval<C>() / LD::Declval<LD::UInteger>()),
                typename E = decltype(LD::Declval<B>() * LD::Declval<D>()),
                typename F = decltype(LD::Declval<E>() * LD::Declval<LD::UInteger>()),
                typename R = decltype(LD::Declval<F>() / LD::Declval<LD::UInteger>())
        >
        constexpr LD::Enable_If_T<LD::Require<
                !LD::CT::IsRatioIdentity<LD::CT::Ratio<UnitExponentNumerator,UnitExponentDenominator>>,
                !LD::CT::IsRatioIdentity<LD::CT::Ratio<UnitExponentNumerator,UnitExponentDenominator>>,
                LD::CT::IsRatioRecipricol<LD::CT::Ratio<UnitExponentNumerator,UnitExponentDenominator>,LD::CT::Ratio<UnitExponentNumerator,UnitExponentDenominator>>,
                LD::IsSame<CurrentTag<T>,Tag<T>>,
                !LD::CT::IsRatioEqual<LD::CT::Ratio<PrefixRatioNumerator,PrefixRatioDenom>,LD::CT::Ratio<ParamRatioNumerator,ParamRatioDenominator>>
        >,
                R
        >
        operator * (const Unit<U,BaseTag,Tag,LD::CT::Ratio<ParamRatioNumerator,ParamRatioDenominator>,LD::UnitExponent<LD::CT::Ratio<ParamUnitNumerator,ParamUnitDenominator>>> & unit) const noexcept(noexcept(LD::Declval<V>()*LD::Declval<U>()))
        {
            A multiplicand = mUnit.Value()*PrefixRatioDenom;
            B instanceAsBaseRatio = multiplicand/PrefixRatioNumerator;
            C paramMultiplicand = unit.mUnit.Value()*ParamRatioDenominator;
            D paramAsBaseRatio = instanceAsBaseRatio/ParamRatioNumerator;
            E resultantAsBaseRatio = instanceAsBaseRatio*paramAsBaseRatio;
            F resultantmultiplicand = resultantAsBaseRatio*PrefixRatioNumerator;
            R resultantValue = resultantmultiplicand/PrefixRatioDenom;
            return resultantValue;
        };

        template<typename U,LD::UInteger ParamRatioNumerator,
                LD::UInteger ParamRatioDenominator,
                LD::UInteger ParamUnitNumerator,
                LD::UInteger ParamUnitDenominator,
                template<typename> class Tag ,
                typename V = T,
                typename R = decltype(LD::Declval<V>() * LD::Declval<T>())>
        constexpr LD::Enable_If_T<LD::Require<
                !LD::CT::IsRatioIdentity<LD::CT::Ratio<UnitExponentNumerator,UnitExponentDenominator>>,
                !LD::CT::IsRatioIdentity<LD::CT::Ratio<UnitExponentNumerator,UnitExponentDenominator>>,
                LD::CT::IsRatioRecipricol<LD::CT::Ratio<UnitExponentNumerator,UnitExponentDenominator>,LD::CT::Ratio<UnitExponentNumerator,UnitExponentDenominator>>,
                !LD::IsSame<CurrentTag<T>,Tag<T>>,
                LD::CT::IsRatioEqual<LD::CT::Ratio<PrefixRatioNumerator,PrefixRatioDenom>,LD::CT::Ratio<ParamRatioNumerator,ParamRatioDenominator>>
        >,
                R
        >
        operator * (const Unit<U,BaseTag,Tag,LD::CT::Ratio<ParamRatioNumerator,ParamRatioDenominator>,LD::UnitExponent<LD::CT::Ratio<ParamUnitNumerator,ParamUnitDenominator>>> & unit) const noexcept(noexcept(LD::Declval<V>()*LD::Declval<U>()))
        {
            UnitRelationship<BaseTag<T>,CurrentTag<T>> instanceTagConverter;
            UnitRelationship<BaseTag<U>,Tag<U>> paramTagConverter;
            UnitRelationship<BaseTag<R>,Tag<R>> resultantTagConverter;
            BaseTag<T> instanceTagAsBaseTag = instanceTagConverter(this->mUnit);
            BaseTag<U> paramTagAsBaseTag = paramTagConverter(unit.mUnit);
            R result = instanceTagAsBaseTag.Value()*paramTagAsBaseTag.Value();
            CurrentTag<R> resultantTag = resultantTagConverter(BaseTag<R>(result));
            return resultantTag.Value();
        };

        template<typename U,LD::UInteger ParamPrefixRatioNumerator,
                LD::UInteger ParamPrefixRatioDenominator,
                LD::UInteger ParamUnitNumerator,
                LD::UInteger ParamUnitDenominator,
                template<typename> class Tag ,
                typename V = T,
                typename A = decltype(LD::Declval<T>() * LD::Declval<LD::UInteger>()),
                typename B = decltype(LD::Declval<A>() / LD::Declval<LD::UInteger>()),
                typename C = decltype(LD::Declval<U>() * LD::Declval<LD::UInteger>()),
                typename D = decltype(LD::Declval<C>() / LD::Declval<LD::UInteger>()),
                typename E = decltype(LD::Declval<B>() * LD::Declval<D>()),
                typename F = decltype(LD::Declval<E>() * LD::Declval<LD::UInteger>()),
                typename R = decltype(LD::Declval<F>() / LD::Declval<LD::UInteger>())
        >
        constexpr LD::Enable_If_T<LD::Require<
                !LD::CT::IsRatioIdentity<LD::CT::Ratio<UnitExponentNumerator,UnitExponentDenominator>>,
                !LD::CT::IsRatioIdentity<LD::CT::Ratio<UnitExponentNumerator,UnitExponentDenominator>>,
                LD::CT::IsRatioRecipricol<LD::CT::Ratio<UnitExponentNumerator,UnitExponentDenominator>,LD::CT::Ratio<UnitExponentNumerator,UnitExponentDenominator>>,
                !LD::IsSame<CurrentTag<T>,Tag<T>>,
                !LD::CT::IsRatioEqual<LD::CT::Ratio<PrefixRatioNumerator,PrefixRatioDenom>,LD::CT::Ratio<ParamPrefixRatioNumerator,ParamPrefixRatioDenominator>>
        >,
                R
        >
        operator * (const Unit<U,BaseTag,Tag,LD::CT::Ratio<ParamPrefixRatioNumerator,ParamPrefixRatioDenominator>,LD::UnitExponent<LD::CT::Ratio<ParamUnitNumerator,ParamUnitDenominator>>> & instance) const noexcept(noexcept(LD::Declval<V>()*LD::Declval<U>()))
        {
            UnitRelationship<BaseTag<T>,CurrentTag<T>> instanceTagConverter;
            UnitRelationship<BaseTag<U>,Tag<U>> paramToBaseTagConverter;
            UnitRelationship<BaseTag<R>,Tag<R>> resultantTagConverter;

            BaseTag<T> instanceTagToBaseTag = instanceTagConverter(this->mUnit);
            BaseTag<T> paramTagToBaseTag = paramToBaseTagConverter(instance);

            A instanceRatioMultiplication = instanceTagToBaseTag.Value()*PrefixRatioDenom;
            B instanceUnitAsBaseRatio = instanceRatioMultiplication/PrefixRatioNumerator;
            C paramRationMultiplication = paramTagToBaseTag.Value()*ParamPrefixRatioDenominator;
            D paramUnitAsBaseRatio = paramRationMultiplication/ParamPrefixRatioNumerator;



            E resultantAddition = instanceUnitAsBaseRatio - paramUnitAsBaseRatio;



            F resultantWithRatioNumerator = resultantAddition*PrefixRatioNumerator;
            R resultantWithCurrentRatioAsBaseTag = resultantWithRatioNumerator/PrefixRatioDenom;

            CurrentTag<R> resultantTag = resultantTagConverter(BaseTag<R>{resultantWithCurrentRatioAsBaseTag});


            return resultantTag.Value();
        };

        template<typename U,LD::UInteger ParamRatioNumerator,
                LD::UInteger ParamRatioDenominator,
                LD::UInteger ParamUnitNumerator,
                LD::UInteger ParamUnitDenominator,
                template<typename> class Tag ,
                typename V = T,
                typename U1 = LD::CT::Ratio<UnitExponentNumerator,UnitExponentDenominator>,
                typename U2 = LD::CT::Ratio<ParamUnitNumerator,ParamUnitDenominator>,
                typename ERatio1 = LD::CT::Ratio<PrefixRatioNumerator,PrefixRatioDenom>,
                typename ERatio2 = LD::CT::Ratio<ParamRatioNumerator,ParamRatioDenominator>,
                typename R = decltype(LD::Declval<V>() * LD::Declval<T>())
        >
        constexpr LD::Enable_If_T<LD::Require<
                !LD::CT::IsRatioIdentity<U1>,
                !LD::CT::IsRatioIdentity<U2>,
                !LD::CT::IsRatioRecipricol<U1,U2>,
                LD::IsSame<CurrentTag<T>,Tag<T>>,
                LD::CT::IsRatioEqual<ERatio1,ERatio2>
        >,
                Unit<R,BaseTag,CurrentTag,ERatio1,LD::UnitExponent<LD::CT::AddRatio<U1,U2>>>
        >
        operator * (const Unit<U,BaseTag,Tag,LD::CT::Ratio<ParamRatioNumerator,ParamRatioDenominator>,LD::UnitExponent<LD::CT::Ratio<ParamUnitNumerator,ParamUnitDenominator>>> & unit) const noexcept(noexcept(LD::Declval<V>()*LD::Declval<U>()))
        {
            return Unit<R,BaseTag,CurrentTag,ERatio1,LD::UnitExponent<LD::CT::AddRatio<U1,U2>>>{mUnit.Value() * unit.mUnit.Value()};
        };


        template<typename U,LD::UInteger ParamRatioNumerator,
                LD::UInteger ParamRatioDenominator,
                LD::UInteger ParamUnitNumerator,
                LD::UInteger ParamUnitDenominator,
                template<typename> class Tag ,
                typename V = T,
                typename U1 = LD::CT::Ratio<UnitExponentNumerator,UnitExponentDenominator>,
                typename U2 = LD::CT::Ratio<ParamUnitNumerator,ParamUnitDenominator>,
                typename ERatio1 = LD::CT::Ratio<PrefixRatioNumerator,PrefixRatioDenom>,
                typename ERatio2 = LD::CT::Ratio<ParamRatioNumerator,ParamRatioDenominator>,
                typename R = decltype(LD::Declval<V>() * LD::Declval<T>())
        >
        constexpr LD::Enable_If_T<LD::Require<
                !LD::CT::IsRatioIdentity<U1>,
                !LD::CT::IsRatioIdentity<U2>,
                !LD::CT::IsRatioRecipricol<U1,U2>,
                !LD::IsSame<CurrentTag<T>,Tag<T>>,
                LD::CT::IsRatioEqual<ERatio1,ERatio2>
        >,
                Unit<R,BaseTag,CurrentTag,ERatio1,LD::UnitExponent<LD::CT::AddRatio<U1,U2>>>
        >
        operator * (const Unit<U,BaseTag,Tag,LD::CT::Ratio<ParamRatioNumerator,ParamRatioDenominator>,LD::UnitExponent<LD::CT::Ratio<ParamUnitNumerator,ParamUnitDenominator>>> & instance) const noexcept(noexcept(LD::Declval<V>()*LD::Declval<U>()))
        {
            UnitRelationship<BaseTag<T>,CurrentTag<T>> instanceTagConverter;
            UnitRelationship<BaseTag<U>,Tag<U>> paramTagConverter;
            UnitRelationship<BaseTag<R>,Tag<R>> resultantTagConverter;
            BaseTag<T> instanceTagAsBaseTag = instanceTagConverter(this->mUnit);
            BaseTag<U> paramTagAsBaseTag = paramTagConverter(instance.mUnit);
            R result = instanceTagAsBaseTag.Value()*paramTagAsBaseTag.Value();
            CurrentTag<R> resultantTag = resultantTagConverter(BaseTag<R>(result));

            return Unit<R,BaseTag,CurrentTag,ERatio1,LD::UnitExponent<LD::CT::AddRatio<U1,U2>>>{resultantTag.Value()};
        };

        template<typename U,LD::UInteger ParamRatioNumerator,
                LD::UInteger ParamRatioDenominator,
                LD::UInteger ParamUnitNumerator,
                LD::UInteger ParamUnitDenominator,
                template<typename> class Tag ,
                typename V = T,
                typename U1 = LD::CT::Ratio<UnitExponentNumerator,UnitExponentDenominator>,
                typename U2 = LD::CT::Ratio<ParamUnitNumerator,ParamUnitDenominator>,
                typename ERatio1 = LD::CT::Ratio<PrefixRatioNumerator,PrefixRatioDenom>,
                typename ERatio2 = LD::CT::Ratio<ParamRatioNumerator,ParamRatioDenominator>,
                typename A = decltype(LD::Declval<T>() * LD::Declval<LD::UInteger>()),
                typename B = decltype(LD::Declval<A>() / LD::Declval<LD::UInteger>()),
                typename C = decltype(LD::Declval<U>() * LD::Declval<LD::UInteger>()),
                typename D = decltype(LD::Declval<C>() / LD::Declval<LD::UInteger>()),
                typename E = decltype(LD::Declval<B>() * LD::Declval<D>()),
                typename F = decltype(LD::Declval<E>() * LD::Declval<LD::UInteger>()),
                typename R = decltype(LD::Declval<F>() / LD::Declval<LD::UInteger>())
        >
        constexpr LD::Enable_If_T<LD::Require<
                !LD::CT::IsRatioIdentity<U1>,
                !LD::CT::IsRatioIdentity<U2>,
                !LD::CT::IsRatioRecipricol<U1,U2>,
                LD::IsSame<CurrentTag<T>,Tag<T>>,
                !LD::CT::IsRatioEqual<ERatio1,ERatio2>
        >,
                Unit<R,BaseTag,CurrentTag,ERatio1,LD::UnitExponent<LD::CT::AddRatio<U1,U2>>>
        >
        operator * (const Unit<U,BaseTag,Tag,LD::CT::Ratio<ParamRatioNumerator,ParamRatioDenominator>,LD::UnitExponent<LD::CT::Ratio<ParamUnitNumerator,ParamUnitDenominator>>> & instance) const noexcept(noexcept(LD::Declval<V>()*LD::Declval<U>()))
        {
            A multiplicand = mUnit.Value()*PrefixRatioDenom;
            B instanceAsBaseRatio = multiplicand/PrefixRatioNumerator;
            C paramMultiplicand = instance.mUnit.Value()*ParamRatioDenominator;
            D paramAsBaseRatio = instanceAsBaseRatio/ParamRatioNumerator;
            E resultantAsBaseRatio = instanceAsBaseRatio*paramAsBaseRatio;
            F resultantmultiplicand = resultantAsBaseRatio*PrefixRatioNumerator;
            R resultantValue = resultantmultiplicand/PrefixRatioDenom;

            return Unit<R,BaseTag,CurrentTag,ERatio1,LD::UnitExponent<LD::CT::AddRatio<U1,U2>>>{resultantValue};
        };

        template<typename U,LD::UInteger ParamRatioNumerator,
                LD::UInteger ParamRatioDenominator,
                LD::UInteger ParamUnitNumerator,
                LD::UInteger ParamUnitDenominator,
                template<typename> class Tag ,
                typename V = T,
                typename U1 = LD::CT::Ratio<UnitExponentNumerator,UnitExponentDenominator>,
                typename U2 = LD::CT::Ratio<ParamUnitNumerator,ParamUnitDenominator>,
                typename ERatio1 = LD::CT::Ratio<PrefixRatioNumerator,PrefixRatioDenom>,
                typename ERatio2 = LD::CT::Ratio<ParamRatioNumerator,ParamRatioDenominator>,
                typename A = decltype(LD::Declval<T>() * LD::Declval<LD::UInteger>()),
                typename B = decltype(LD::Declval<A>() / LD::Declval<LD::UInteger>()),
                typename C = decltype(LD::Declval<U>() * LD::Declval<LD::UInteger>()),
                typename D = decltype(LD::Declval<C>() / LD::Declval<LD::UInteger>()),
                typename E = decltype(LD::Declval<B>() * LD::Declval<D>()),
                typename F = decltype(LD::Declval<E>() * LD::Declval<LD::UInteger>()),
                typename R = decltype(LD::Declval<F>() / LD::Declval<LD::UInteger>())
        >
        constexpr LD::Enable_If_T<LD::Require<
                !LD::CT::IsRatioIdentity<U1>,
                !LD::CT::IsRatioIdentity<U2>,
                !LD::CT::IsRatioRecipricol<U1,U2>,
                !LD::IsSame<CurrentTag<T>,Tag<T>>,
                !LD::CT::IsRatioEqual<ERatio1,ERatio2>
        >,
                Unit<R,BaseTag,CurrentTag,ERatio1,LD::UnitExponent<LD::CT::AddRatio<U1,U2>>>
        >
        operator * (const Unit<U,BaseTag,Tag,LD::CT::Ratio<ParamRatioNumerator,ParamRatioDenominator>,LD::UnitExponent<LD::CT::Ratio<ParamUnitNumerator,ParamUnitDenominator>>> & instance) const noexcept(noexcept(LD::Declval<V>()*LD::Declval<U>()))
        {
            UnitRelationship<BaseTag<T>,CurrentTag<T>> instanceTagConverter;
            UnitRelationship<BaseTag<U>,Tag<U>> paramToBaseTagConverter;
            UnitRelationship<BaseTag<R>,Tag<R>> resultantTagConverter;

            BaseTag<T> instanceTagToBaseTag = instanceTagConverter(this->mUnit);
            BaseTag<T> paramTagToBaseTag = paramToBaseTagConverter(instance);

            A instanceRatioMultiplication = instanceTagToBaseTag.Value()*PrefixRatioDenom;
            B instanceUnitAsBaseRatio = instanceRatioMultiplication/PrefixRatioNumerator;
            C paramRationMultiplication = paramTagToBaseTag.Value()*ParamRatioNumerator;
            D paramUnitAsBaseRatio = paramRationMultiplication/ParamRatioDenominator;



            E resultantAddition = instanceUnitAsBaseRatio - paramUnitAsBaseRatio;



            F resultantWithRatioNumerator = resultantAddition*PrefixRatioNumerator;
            R resultantWithCurrentRatioAsBaseTag = resultantWithRatioNumerator/PrefixRatioDenom;

            CurrentTag<R> resultantTag = resultantTagConverter(BaseTag<R>{resultantWithCurrentRatioAsBaseTag});

            return Unit<R,BaseTag,CurrentTag,ERatio1,LD::UnitExponent<LD::CT::AddRatio<U1,U2>>>{resultantTag.Value()};
        };

        template<typename U,LD::UInteger ParamRatioNumerator,
                LD::UInteger ParamRatioDenominator,
                LD::UInteger ParamUnitNumerator,
                LD::UInteger ParamUnitDenominator,
                template<typename> class Tag ,
                typename V = T,
                typename U1 = LD::CT::Ratio<UnitExponentNumerator,UnitExponentDenominator>,
                typename U2 = LD::CT::Ratio<ParamUnitNumerator,ParamUnitDenominator>,
                typename ERatio1 = LD::CT::Ratio<PrefixRatioNumerator,PrefixRatioDenom>,
                typename ERatio2 = LD::CT::Ratio<ParamRatioNumerator,ParamRatioDenominator>,
                typename R = decltype(LD::Declval<V>() * LD::Declval<T>())
        >
        constexpr LD::Enable_If_T<LD::Require<
                LD::CT::IsRatioEqual<U1,U2>,
                LD::IsSame<CurrentTag<T>,Tag<T>>,
                LD::CT::IsRatioEqual<ERatio1,ERatio2>
        >,
                R
        >
        operator / (const Unit<U,BaseTag,Tag,LD::CT::Ratio<ParamRatioNumerator,ParamRatioDenominator>,LD::UnitExponent<LD::CT::Ratio<ParamUnitNumerator,ParamUnitDenominator>>> & unit) const noexcept(noexcept(LD::Declval<V>()*LD::Declval<U>()))
        {
            return mUnit.Value() / unit.mUnit.Value();
        };

        template<typename U,LD::UInteger ParamRatioNumerator,
                LD::UInteger ParamRatioDenominator,
                LD::UInteger ParamUnitNumerator,
                LD::UInteger ParamUnitDenominator,
                template<typename> class Tag ,
                typename V = T,
                typename U1 = LD::CT::Ratio<UnitExponentNumerator,UnitExponentDenominator>,
                typename U2 = LD::CT::Ratio<ParamUnitNumerator,ParamUnitDenominator>,
                typename ERatio1 = LD::CT::Ratio<PrefixRatioNumerator,PrefixRatioDenom>,
                typename ERatio2 = LD::CT::Ratio<ParamRatioNumerator,ParamRatioDenominator>,
                typename A = decltype(LD::Declval<T>() * LD::Declval<LD::UInteger>()),
                typename B = decltype(LD::Declval<A>() / LD::Declval<LD::UInteger>()),
                typename C = decltype(LD::Declval<U>() * LD::Declval<LD::UInteger>()),
                typename D = decltype(LD::Declval<C>() / LD::Declval<LD::UInteger>()),
                typename E = decltype(LD::Declval<B>() * LD::Declval<D>()),
                typename F = decltype(LD::Declval<E>() * LD::Declval<LD::UInteger>()),
                typename R = decltype(LD::Declval<F>() / LD::Declval<LD::UInteger>())
        >
        constexpr LD::Enable_If_T<LD::Require<
                LD::CT::IsRatioEqual<U1,U2>,
                LD::IsSame<CurrentTag<T>,Tag<T>>,
                !LD::CT::IsRatioEqual<ERatio1,ERatio2>
        >,
                R
        >
        operator / (const Unit<U,BaseTag,Tag,LD::CT::Ratio<ParamRatioNumerator,ParamRatioDenominator>,LD::UnitExponent<LD::CT::Ratio<ParamUnitNumerator,ParamUnitDenominator>>> & instance) const noexcept(noexcept(LD::Declval<V>()*LD::Declval<U>()))
        {
            A multiplicand = mUnit.Value()*PrefixRatioDenom;
            B instanceAsBaseRatio = multiplicand/PrefixRatioNumerator;
            C paramMultiplicand = instance.mUnit.Value()*ParamRatioDenominator;
            D paramAsBaseRatio = instanceAsBaseRatio/ParamRatioNumerator;
            E resultantAsBaseRatio = instanceAsBaseRatio/paramAsBaseRatio;
            F resultantmultiplicand = resultantAsBaseRatio*PrefixRatioNumerator;
            R resultantValue = resultantmultiplicand/PrefixRatioDenom;
            return resultantValue;
        }

        template<typename U,LD::UInteger ParamRatioNumerator,
                LD::UInteger ParamRatioDenominator,
                LD::UInteger ParamUnitNumerator,
                LD::UInteger ParamUnitDenominator,
                template<typename> class Tag ,
                typename V = T,
                typename U1 = LD::CT::Ratio<UnitExponentNumerator,UnitExponentDenominator>,
                typename U2 = LD::CT::Ratio<ParamUnitNumerator,ParamUnitDenominator>,
                typename ERatio1 = LD::CT::Ratio<PrefixRatioNumerator,PrefixRatioDenom>,
                typename ERatio2 = LD::CT::Ratio<ParamRatioNumerator,ParamRatioDenominator>,
                typename R = decltype(LD::Declval<V>() * LD::Declval<T>())
        >
        constexpr LD::Enable_If_T<LD::Require<
                LD::CT::IsRatioEqual<U1,U2>,
                !LD::IsSame<CurrentTag<T>,Tag<T>>,
                LD::CT::IsRatioEqual<ERatio1,ERatio2>
        >,
                R
        >
        operator / (const Unit<U,BaseTag,Tag,LD::CT::Ratio<ParamRatioNumerator,ParamRatioDenominator>,LD::UnitExponent<LD::CT::Ratio<ParamUnitNumerator,ParamUnitDenominator>>> & instance) const noexcept(noexcept(LD::Declval<V>()*LD::Declval<U>()))
        {
            UnitRelationship<BaseTag<T>,CurrentTag<T>> instanceTagConverter;
            UnitRelationship<BaseTag<U>,Tag<U>> paramTagConverter;
            UnitRelationship<BaseTag<R>,Tag<R>> resultantTagConverter;
            BaseTag<T> instanceTagAsBaseTag = instanceTagConverter(this->mUnit);
            BaseTag<U> paramTagAsBaseTag = paramTagConverter(instance.mUnit);
            R result = instanceTagAsBaseTag.Value()/paramTagAsBaseTag.Value();
            CurrentTag<R> resultantTag = resultantTagConverter(BaseTag<R>(result));

            return resultantTag.Value();
        }

    };

    namespace CT
    {
        template<typename T,
                template<typename> class BaseTag,
                template<typename> class CurrentTag,
                LD::UInteger PrefixRatioNumerator, LD::UInteger PrefixRatioDenom,
                LD::UInteger UnitExponentNumerator, LD::UInteger UnitExponentDenominator
        >
        LD::Type<T> RetrieveUnitUnderlyingType(LD::Type<LD::Unit<T,BaseTag,CurrentTag,LD::CT::Ratio<PrefixRatioNumerator,PrefixRatioDenom> ,LD::UnitExponent<LD::CT::Ratio<UnitExponentNumerator,UnitExponentDenominator>>>>) noexcept
        {
            return LD::Type<T>{};
        }
    }

    template<typename T, class sfinae = void>
    class UnitSkills;

    template<typename T>
    class UnitSkills<T,LD::Enable_If_T<LD::Require<LD::Concept::Arithmetical<T>>>>
    {
    private:
        T mValue;
    public:


        constexpr explicit UnitSkills(const T & value) noexcept :mValue(value){}
        const T & Value() const noexcept
        {
            return this->mValue;
        }


        T & Value() noexcept
        {
            return this->mValue;
        }
    };




    template<typename T>
    class SecondTag: public UnitSkills<T>
    {
    private:

    public:
        constexpr explicit  SecondTag() noexcept : UnitSkills<T>(0) {}
        constexpr explicit SecondTag(const T & value) noexcept : UnitSkills<T>(value) {}


        constexpr SecondTag<T> operator()(const SecondTag<T> & tag) const noexcept
        {
            return SecondTag<T>{tag.Value()};
        }



    };

    template<typename T>
    class MinuteTag: public UnitSkills<T>
    {
    private:

    public:

        constexpr explicit  MinuteTag() noexcept : UnitSkills<T>(0) {}
        constexpr explicit MinuteTag(const T & value) noexcept : UnitSkills<T>(value) {}
        constexpr SecondTag<T> operator()(const MinuteTag<T> & minute) const noexcept
        {
            return SecondTag<T>{minute.Value()*60};
        }

        constexpr MinuteTag<T> operator()(const SecondTag<T> & minute) const noexcept
        {
            return MinuteTag<T>{minute.Value()/T(60)};
        }
    };

    template<typename T>
    class ByteTag: public UnitSkills<T>
    {
    private:

    public:

        constexpr explicit  ByteTag() noexcept : UnitSkills<T>(0) {}
        constexpr explicit ByteTag(const T & value) noexcept : UnitSkills<T>(value) {}
        constexpr ByteTag<T> operator()(const ByteTag<T> & minute) const noexcept
        {
            return SecondTag<T>{minute.Value()};
        }

    };

    template<typename T>
    class HourTag: public UnitSkills<T>
    {
    private:

    public:
        constexpr explicit  HourTag() noexcept : UnitSkills<T>(0) {}
        constexpr explicit HourTag(const T & value) noexcept : UnitSkills<T>(value) {}


        constexpr SecondTag<T> operator()(const HourTag<T> & tag) const noexcept
        {
            return SecondTag<T>{tag.Value()*3600};
        }

        constexpr HourTag<T> operator()(const SecondTag<T> & tag) const noexcept
        {
            return HourTag<T>{tag.Value()/3600};
        }

    };

    template<typename T>
    class DayTag: public UnitSkills<T>
    {
    private:

    public:
        constexpr explicit  DayTag() noexcept : UnitSkills<T>(0) {}
        constexpr explicit DayTag(const T & value) noexcept : UnitSkills<T>(value) {}


        constexpr SecondTag<T> operator()(const DayTag<T> & tag) const noexcept
        {
            return SecondTag<T>{tag.Value()*3600*24};
        }

        constexpr DayTag<T> operator()(const SecondTag<T> & tag) const noexcept
        {
            return DayTag<T>{tag.Value()/3600*24};
        }
    };

    template<typename T>
    class MonthTag: public UnitSkills<T>
    {
    private:

    public:
        constexpr explicit  MonthTag() noexcept : UnitSkills<T>(0) {}
        constexpr explicit MonthTag(const T & value) noexcept : UnitSkills<T>(value) {}


        constexpr SecondTag<T> operator()(const MonthTag<T> & tag) const noexcept
        {
            return SecondTag<T>{tag.Value()*2629800};
        }

        constexpr MonthTag<T> operator()(const SecondTag<T> & tag) const noexcept
        {
            return DayTag<T>{tag.Value()/2629800};
        }
    };

    template<typename T>
    class YearTag: public UnitSkills<T>
    {
    private:

    public:
        constexpr explicit  YearTag() noexcept : UnitSkills<T>(0) {}
        constexpr explicit YearTag(const T & value) noexcept : UnitSkills<T>(value) {}


        constexpr SecondTag<T> operator()(const YearTag<T> & tag) const noexcept
        {
            return SecondTag<T>{tag.Value()*31557600};
        }

        constexpr YearTag<T> operator()(const SecondTag<T> & tag) const noexcept
        {
            //printf("being called \n");
            return YearTag<T>{tag.Value()/31557600};
        }
    };

    template<typename T>
    class MeterTag: public UnitSkills<T>
    {
    private:
    public:
        constexpr explicit  MeterTag() noexcept : UnitSkills<T>(0) {}
        constexpr explicit MeterTag(const T & value) noexcept : UnitSkills<T>(value) {}
    };

    template<typename T>
    class FootTag: public UnitSkills<T>
    {
    private:
    public:
        constexpr explicit  FootTag() noexcept : UnitSkills<T>(0) {}
        constexpr explicit FootTag(const T & value) noexcept : UnitSkills<T>(value) {}
    };

    template<typename T, template<typename> class BaseTag>
    class UnitRelationship<BaseTag<T>,BaseTag<T>>
    {
    public:
        constexpr BaseTag<T> operator()(const BaseTag<T> & unit) const noexcept
        {
            return BaseTag<T>{unit.Value()};
        }
    };
    template<typename T>
    class UnitRelationship<LD::SecondTag<T>,LD::MinuteTag<T>>
    {
    private:

    public:

        constexpr SecondTag<T> operator()(const MinuteTag<T> & minute) const noexcept
        {
            return SecondTag<T>{minute.Value()*60};
        }

        constexpr MinuteTag<T> operator()(const SecondTag<T> & minute) const noexcept
        {
            return MinuteTag<T>{minute.Value()/T(60)};
        }
    };

    template<typename T>
    class UnitRelationship<LD::SecondTag<T>,LD::HourTag<T>>
    {
    private:

    public:

        constexpr SecondTag<T> operator()(const LD::HourTag<T> & minute) const noexcept
        {
            return SecondTag<T>{minute.Value()*3600};
        }

        constexpr LD::HourTag<T> operator()(const SecondTag<T> & minute) const noexcept
        {
            return LD::HourTag<T>{minute.Value()/T(3600)};
        }
    };

    template<typename T>
    class UnitRelationship<LD::SecondTag<T>,LD::DayTag<T>>
    {
    private:

    public:

        constexpr SecondTag<T> operator()(const DayTag<T> & minute) const noexcept
        {
            return SecondTag<T>{minute.Value()*2629800};
        }

        constexpr DayTag<T> operator()(const SecondTag<T> & minute) const noexcept
        {
            return DayTag<T>{minute.Value()/T(2629800)};
        }
    };

    template<typename T>
    class UnitRelationship<LD::SecondTag<T>,LD::MonthTag<T>>
    {
    private:

    public:

        constexpr SecondTag<T> operator()(const MonthTag<T> & minute) const noexcept
        {
            return SecondTag<T>{minute.Value()*2629800};
        }

        constexpr MonthTag<T> operator()(const SecondTag<T> & minute) const noexcept
        {
            return MonthTag<T>{minute.Value()/T(2629800)};
        }
    };

    template<typename T>
    class UnitRelationship<LD::SecondTag<T>,LD::YearTag<T>>
    {
    private:

    public:

        constexpr SecondTag<T> operator()(const YearTag<T> & minute) const noexcept
        {
            return SecondTag<T>{minute.Value()*31557600};
        }

        constexpr YearTag<T> operator()(const SecondTag<T> & minute) const noexcept
        {
            return MinuteTag<T>{minute.Value()/T(31557600)};
        }
    };

    template<typename T>
    class UnitRelationship<LD::MeterTag<T>,LD::FootTag<T>>
    {
    private:
    public:
        constexpr MeterTag<T> operator()(const FootTag<T> & foot) const noexcept
        {
            return MeterTag<T>{foot.Value()*0.3048};
        }
    };

    template<typename T>
    class UnitRelationship<LD::FootTag<T>,LD::MeterTag<T>>
    {
    private:
    public:
        constexpr FootTag<T> operator()(const MeterTag<T> & meter) const noexcept
        {
            return FootTag<T>{meter.Value()*3.28084};
        }
    };
    //-----------------------------------------------------Beginning of Byte Declaration-----------------------------------------------------------------------------
    template<typename T>
    using YoctoByte = LD::Unit<T,LD::ByteTag,LD::ByteTag,LD::Yocto ,LD::UnitExponent<LD::CT::Ratio<1,1>>>;
    template<typename T>
    using ZeptoByte = LD::Unit<T,LD::ByteTag,LD::ByteTag,LD::Zepto ,LD::UnitExponent<LD::CT::Ratio<1,1>>>;
    template<typename T>
    using AttoByte = LD::Unit<T,LD::ByteTag,LD::ByteTag,LD::Atto,LD::UnitExponent<LD::CT::Ratio<1,1>>>;
    template<typename T>
    using FemtoByte = LD::Unit<T,LD::ByteTag,LD::ByteTag,LD::Femto ,LD::UnitExponent<LD::CT::Ratio<1,1>>>;
    template<typename T>
    using PicoByte = LD::Unit<T,LD::ByteTag,LD::ByteTag,LD::Pico ,LD::UnitExponent<LD::CT::Ratio<1,1>>>;
    template<typename T>
    using NanoByte = LD::Unit<T,LD::ByteTag,LD::ByteTag,LD::Nano ,LD::UnitExponent<LD::CT::Ratio<1,1>>>;
    template<typename T>
    using MicroByte = LD::Unit<T,LD::ByteTag,LD::ByteTag,LD::Micro ,LD::UnitExponent<LD::CT::Ratio<1,1>>>;
    template<typename T>
    using MiliByte = LD::Unit<T,LD::ByteTag,LD::ByteTag,LD::Milli,LD::UnitExponent<LD::CT::Ratio<1,1>>>;
    template<typename T>
    using CentiByte = LD::Unit<T,LD::ByteTag,LD::ByteTag,LD::Centi ,LD::UnitExponent<LD::CT::Ratio<1,1>>>;
    template<typename T>
    using DeciByte = LD::Unit<T,LD::ByteTag,LD::ByteTag,LD::Deci,LD::UnitExponent<LD::CT::Ratio<1,1>>>;
    template<typename T>
    using Byte = LD::Unit<T,LD::ByteTag,LD::ByteTag,LD::CT::Ratio<1,1>,LD::UnitExponent<LD::CT::Ratio<1,1>>>;
    template<typename T>
    using DecaByte = LD::Unit<T,LD::ByteTag,LD::ByteTag,LD::CT::Ratio<1024,1> ,LD::UnitExponent<LD::CT::Ratio<1,1>>>;
    template<typename T>
    using HectoByte = LD::Unit<T,LD::ByteTag,LD::ByteTag,LD::CT::Ratio<1024*1024,1> ,LD::UnitExponent<LD::CT::Ratio<1,1>>>;
    template<typename T>
    using KiloByte = LD::Unit<T,LD::ByteTag,LD::ByteTag,LD::CT::Ratio<1024*1024*1024,1> ,LD::UnitExponent<LD::CT::Ratio<1,1>>>;
    template<typename T>
    using MegaByte = LD::Unit<T,LD::ByteTag,LD::ByteTag,LD::CT::Ratio<1024ul*1024ul*1024ul*1024ul,1>  ,LD::UnitExponent<LD::CT::Ratio<1,1>>>;
    template<typename T>
    using GigaByte = LD::Unit<T,LD::ByteTag,LD::ByteTag,LD::CT::Ratio<1024ul*1024ul*1024ul*1024ul*1024ul,1> ,LD::UnitExponent<LD::CT::Ratio<1,1>>>;
    template<typename T>
    using TeraByte = LD::Unit<T,LD::ByteTag,LD::ByteTag,LD::CT::Ratio<1024ul*1024ul*1024ul*1024ul*1024ul*1024ul,1> ,LD::UnitExponent<LD::CT::Ratio<1,1>>>;
    template<typename T>
    using PetaByte = LD::Unit<T,LD::ByteTag,LD::ByteTag,LD::CT::Ratio<1024ul*1024ul*1024ul*1024ul*1024ul*1024ul*1024ul,1> ,LD::UnitExponent<LD::CT::Ratio<1,1>>>;
    template<typename T>
    using ExaByte = LD::Unit<T,LD::ByteTag,LD::ByteTag,LD::CT::Ratio<1024ul*1024ul*1024ul*1024ul*1024ul*1024ul*1024ul,1> ,LD::UnitExponent<LD::CT::Ratio<1,1>>>;
    template<typename T>
    using ZettaByte = LD::Unit<T,LD::ByteTag,LD::ByteTag,LD::CT::Ratio<1024ul*1024ul*1024ul*1024ul*1024ul*1024ul*1024ul,1> ,LD::UnitExponent<LD::CT::Ratio<1,1>>>;
    template<typename T>
    using YottaByte = LD::Unit<T,LD::ByteTag,LD::ByteTag,LD::CT::Ratio<1024ul*1024ul*1024ul*1024ul*1024ul*1024ul*1024ul,1> ,LD::UnitExponent<LD::CT::Ratio<1,1>>>;
    //-----------------------------------------------------End of Byte Declaration------------------------------------------------------------------------------------
    //-----------------------------------------------------Beginning of Second Declaration----------------------------------------------------------------------------
    template<typename T>
    using YoctoSecond = LD::Unit<T,LD::SecondTag,LD::SecondTag,LD::Yocto ,LD::UnitExponent<LD::CT::Ratio<1,1>>>;



    template<typename T>
    using ZeptoSecond = LD::Unit<T,LD::SecondTag,LD::SecondTag,LD::Zepto ,LD::UnitExponent<LD::CT::Ratio<1,1>>>;



    template<typename T>
    using AttoSecond = LD::Unit<T,LD::SecondTag,LD::SecondTag,LD::Atto,LD::UnitExponent<LD::CT::Ratio<1,1>>>;



    template<typename T>
    using FemtoSecond = LD::Unit<T,LD::SecondTag,LD::SecondTag,LD::Femto ,LD::UnitExponent<LD::CT::Ratio<1,1>>>;




    template<typename T>
    using PicoSecond = LD::Unit<T,LD::SecondTag,LD::SecondTag,LD::Pico ,LD::UnitExponent<LD::CT::Ratio<1,1>>>;




    template<typename T>
    using NanoSecond = LD::Unit<T,LD::SecondTag,LD::SecondTag,LD::Nano ,LD::UnitExponent<LD::CT::Ratio<1,1>>>;




    template<typename T>
    using MicroSecond = LD::Unit<T,LD::SecondTag,LD::SecondTag,LD::Micro ,LD::UnitExponent<LD::CT::Ratio<1,1>>>;




    template<typename T>
    using Milisecond = LD::Unit<T,LD::SecondTag,LD::SecondTag,LD::Milli,LD::UnitExponent<LD::CT::Ratio<1,1>>>;




    template<typename T>
    using CentiSecond = LD::Unit<T,LD::SecondTag,LD::SecondTag,LD::Centi ,LD::UnitExponent<LD::CT::Ratio<1,1>>>;




    template<typename T>
    using DeciSecond = LD::Unit<T,LD::SecondTag,LD::SecondTag,LD::Deci,LD::UnitExponent<LD::CT::Ratio<1,1>>>;



    template<typename T>
    using Second = LD::Unit<T,LD::SecondTag,LD::SecondTag,LD::CT::Ratio<1,1>,LD::UnitExponent<LD::CT::Ratio<1,1>>>;




    template<typename T>
    using DecaSecond = LD::Unit<T,LD::SecondTag,LD::SecondTag,LD::Deca ,LD::UnitExponent<LD::CT::Ratio<1,1>>>;




    template<typename T>
    using HectoSecond = LD::Unit<T,LD::SecondTag,LD::SecondTag,LD::Hecto ,LD::UnitExponent<LD::CT::Ratio<1,1>>>;




    template<typename T>
    using KiloSecond = LD::Unit<T,LD::SecondTag,LD::SecondTag,LD::Kilo ,LD::UnitExponent<LD::CT::Ratio<1,1>>>;





    template<typename T>
    using MegaSecond = LD::Unit<T,LD::SecondTag,LD::SecondTag,LD::Mega ,LD::UnitExponent<LD::CT::Ratio<1,1>>>;




    template<typename T>
    using GigaSecond = LD::Unit<T,LD::SecondTag,LD::SecondTag,LD::Giga ,LD::UnitExponent<LD::CT::Ratio<1,1>>>;




    template<typename T>
    using TeraSecond = LD::Unit<T,LD::SecondTag,LD::SecondTag,LD::Tera ,LD::UnitExponent<LD::CT::Ratio<1,1>>>;




    template<typename T>
    using PetaSecond = LD::Unit<T,LD::SecondTag,LD::SecondTag,LD::Peta ,LD::UnitExponent<LD::CT::Ratio<1,1>>>;




    template<typename T>
    using ExaSecond = LD::Unit<T,LD::SecondTag,LD::SecondTag,LD::Exa ,LD::UnitExponent<LD::CT::Ratio<1,1>>>;


    template<typename T>
    using ZettaSecond = LD::Unit<T,LD::SecondTag,LD::SecondTag,LD::Zetta ,LD::UnitExponent<LD::CT::Ratio<1,1>>>;


    template<typename T>
    using YottaSecond = LD::Unit<T,LD::SecondTag,LD::SecondTag,LD::Yotta ,LD::UnitExponent<LD::CT::Ratio<1,1>>>;
//-------------------------------------------------------End of Second Declaration-------------------------------------------------------------------------------------

//-------------------------------------------------------Beginning of Minute Declaration-------------------------------------------------------------------------------

    template<typename T>
    using YoctoMinute = LD::Unit<T,SecondTag,LD::MinuteTag,LD::Yocto ,LD::UnitExponent<LD::CT::Ratio<1,1>>>;

    template<typename T>
    using ZeptoMinute = LD::Unit<T,SecondTag,LD::MinuteTag,LD::Zepto ,LD::UnitExponent<LD::CT::Ratio<1,1>>>;

    template<typename T>
    using AttoMinute = LD::Unit<T,SecondTag,LD::MinuteTag,LD::Atto,LD::UnitExponent<LD::CT::Ratio<1,1>>>;

    template<typename T>
    using FemtoMinute = LD::Unit<T,SecondTag,LD::MinuteTag,LD::Femto ,LD::UnitExponent<LD::CT::Ratio<1,1>>>;

    template<typename T>
    using PicoMinute = LD::Unit<T,SecondTag,LD::MinuteTag,LD::Pico ,LD::UnitExponent<LD::CT::Ratio<1,1>>>;

    template<typename T>
    using NanoMinute = LD::Unit<T,SecondTag,LD::MinuteTag,LD::Nano ,LD::UnitExponent<LD::CT::Ratio<1,1>>>;

    template<typename T>
    using MicroMinute = LD::Unit<T,SecondTag,LD::MinuteTag,LD::Micro ,LD::UnitExponent<LD::CT::Ratio<1,1>>>;

    template<typename T>
    using MiliMinute = LD::Unit<T,SecondTag,LD::MinuteTag,LD::Milli,LD::UnitExponent<LD::CT::Ratio<1,1>>>;

    template<typename T>
    using CentiMinute = LD::Unit<T,SecondTag,LD::MinuteTag,LD::Centi ,LD::UnitExponent<LD::CT::Ratio<1,1>>>;

    template<typename T>
    using DeciMinute = LD::Unit<T,SecondTag,LD::MinuteTag,LD::Deci,LD::UnitExponent<LD::CT::Ratio<1,1>>>;

    template<typename T>
    using Minute = LD::Unit<T,SecondTag,LD::MinuteTag,LD::CT::Ratio<1,1>,LD::UnitExponent<LD::CT::Ratio<1,1>>>;

    template<typename T>
    using DecaMinute = LD::Unit<T,SecondTag,LD::MinuteTag,LD::Deca ,LD::UnitExponent<LD::CT::Ratio<1,1>>>;

    template<typename T>
    using HectoMinute = LD::Unit<T,SecondTag,LD::MinuteTag,LD::Hecto ,LD::UnitExponent<LD::CT::Ratio<1,1>>>;

    template<typename T>
    using KiloMinute = LD::Unit<T,SecondTag,LD::MinuteTag,LD::Kilo ,LD::UnitExponent<LD::CT::Ratio<1,1>>>;

    template<typename T>
    using MegaMinute = LD::Unit<T,SecondTag,LD::MinuteTag,LD::Mega ,LD::UnitExponent<LD::CT::Ratio<1,1>>>;

    template<typename T>
    using GigaMinute = LD::Unit<T,SecondTag,LD::MinuteTag,LD::Giga ,LD::UnitExponent<LD::CT::Ratio<1,1>>>;

    template<typename T>
    using TeraMinute = LD::Unit<T,SecondTag,LD::MinuteTag,LD::Tera ,LD::UnitExponent<LD::CT::Ratio<1,1>>>;

    template<typename T>
    using PetaMinute = LD::Unit<T,SecondTag,LD::MinuteTag,LD::Peta ,LD::UnitExponent<LD::CT::Ratio<1,1>>>;

    template<typename T>
    using ExaMinute = LD::Unit<T,SecondTag,LD::MinuteTag,LD::Exa ,LD::UnitExponent<LD::CT::Ratio<1,1>>>;

    template<typename T>
    using ZettaMinute = LD::Unit<T,LD::SecondTag,MinuteTag,LD::Zetta ,LD::UnitExponent<LD::CT::Ratio<1,1>>>;

    template<typename T>
    using YottaMinute = LD::Unit<T,SecondTag,LD::MinuteTag,LD::Yotta ,LD::UnitExponent<LD::CT::Ratio<1,1>>>;
    //---------------------------------------------------End of Minute Declaration-------------------------------------------------------------------------------------
    //---------------------------------------------------Beginning of Hour Declaration---------------------------------------------------------------------------------
    template<typename T>
    using YoctoHour = LD::Unit<T,SecondTag,LD::HourTag,LD::Yocto ,LD::UnitExponent<LD::CT::Ratio<1,1>>>;

    template<typename T>
    using ZeptoHour = LD::Unit<T,SecondTag,LD::HourTag,LD::Zepto ,LD::UnitExponent<LD::CT::Ratio<1,1>>>;

    template<typename T>
    using AttoHour = LD::Unit<T,SecondTag,LD::HourTag,LD::Atto,LD::UnitExponent<LD::CT::Ratio<1,1>>>;

    template<typename T>
    using FemtoHour = LD::Unit<T,SecondTag,LD::HourTag,LD::Femto ,LD::UnitExponent<LD::CT::Ratio<1,1>>>;

    template<typename T>
    using PicoHour = LD::Unit<T,SecondTag,LD::HourTag,LD::Pico ,LD::UnitExponent<LD::CT::Ratio<1,1>>>;

    template<typename T>
    using NanoHour = LD::Unit<T,SecondTag,LD::HourTag,LD::Nano ,LD::UnitExponent<LD::CT::Ratio<1,1>>>;

    template<typename T>
    using MicroHour = LD::Unit<T,SecondTag,LD::HourTag,LD::Micro ,LD::UnitExponent<LD::CT::Ratio<1,1>>>;

    template<typename T>
    using MiliHour = LD::Unit<T,SecondTag,LD::HourTag,LD::Milli,LD::UnitExponent<LD::CT::Ratio<1,1>>>;

    template<typename T>
    using CentiHour = LD::Unit<T,SecondTag,LD::HourTag,LD::Centi ,LD::UnitExponent<LD::CT::Ratio<1,1>>>;

    template<typename T>
    using DeciHour = LD::Unit<T,SecondTag,LD::HourTag,LD::Deci,LD::UnitExponent<LD::CT::Ratio<1,1>>>;

    template<typename T>
    using Hour = LD::Unit<T,SecondTag,LD::HourTag,LD::CT::Ratio<1,1>,LD::UnitExponent<LD::CT::Ratio<1,1>>>;

    template<typename T>
    using DecaHour = LD::Unit<T,SecondTag,LD::HourTag,LD::Deca ,LD::UnitExponent<LD::CT::Ratio<1,1>>>;

    template<typename T>
    using HectoHour = LD::Unit<T,SecondTag,LD::HourTag,LD::Hecto ,LD::UnitExponent<LD::CT::Ratio<1,1>>>;

    template<typename T>
    using KiloHour = LD::Unit<T,SecondTag,LD::HourTag,LD::Kilo ,LD::UnitExponent<LD::CT::Ratio<1,1>>>;

    template<typename T>
    using MegaHour = LD::Unit<T,SecondTag,LD::HourTag,LD::Mega ,LD::UnitExponent<LD::CT::Ratio<1,1>>>;

    template<typename T>
    using GigaHour = LD::Unit<T,SecondTag,LD::HourTag,LD::Giga ,LD::UnitExponent<LD::CT::Ratio<1,1>>>;

    template<typename T>
    using TeraHour = LD::Unit<T,SecondTag,LD::HourTag,LD::Tera ,LD::UnitExponent<LD::CT::Ratio<1,1>>>;

    template<typename T>
    using PetaHour = LD::Unit<T,SecondTag,LD::HourTag,LD::Peta ,LD::UnitExponent<LD::CT::Ratio<1,1>>>;

    template<typename T>
    using ExaHour = LD::Unit<T,SecondTag,LD::HourTag,LD::Exa ,LD::UnitExponent<LD::CT::Ratio<1,1>>>;

    template<typename T>
    using ZettaHour = LD::Unit<T,LD::SecondTag,LD::HourTag,LD::Zetta ,LD::UnitExponent<LD::CT::Ratio<1,1>>>;

    template<typename T>
    using YottaHour = LD::Unit<T,SecondTag,LD::HourTag,LD::Yotta ,LD::UnitExponent<LD::CT::Ratio<1,1>>>;
    //---------------------------------------------------End of Hour Declaration---------------------------------------------------------------------------------------

//-------------------------------------------------------Beginning of Day Declaration----------------------------------------------------------------------------------
    template<typename T>
    using YoctoDay = LD::Unit<T,SecondTag,LD::DayTag,LD::Yocto ,LD::UnitExponent<LD::CT::Ratio<1,1>>>;

    template<typename T>
    using ZeptoDay = LD::Unit<T,SecondTag,LD::DayTag,LD::Zepto ,LD::UnitExponent<LD::CT::Ratio<1,1>>>;

    template<typename T>
    using AttoDay = LD::Unit<T,SecondTag,LD::DayTag,LD::Atto,LD::UnitExponent<LD::CT::Ratio<1,1>>>;

    template<typename T>
    using FemtoDay = LD::Unit<T,SecondTag,LD::DayTag,LD::Femto ,LD::UnitExponent<LD::CT::Ratio<1,1>>>;

    template<typename T>
    using PicoDay = LD::Unit<T,SecondTag,LD::DayTag,LD::Pico ,LD::UnitExponent<LD::CT::Ratio<1,1>>>;

    template<typename T>
    using NanoDay = LD::Unit<T,SecondTag,LD::DayTag,LD::Nano ,LD::UnitExponent<LD::CT::Ratio<1,1>>>;

    template<typename T>
    using MicroDay = LD::Unit<T,SecondTag,LD::DayTag,LD::Micro ,LD::UnitExponent<LD::CT::Ratio<1,1>>>;

    template<typename T>
    using MiliDay = LD::Unit<T,SecondTag,LD::DayTag,LD::Milli,LD::UnitExponent<LD::CT::Ratio<1,1>>>;

    template<typename T>
    using CentiDay = LD::Unit<T,SecondTag,LD::DayTag,LD::Centi ,LD::UnitExponent<LD::CT::Ratio<1,1>>>;

    template<typename T>
    using DeciDay = LD::Unit<T,SecondTag,LD::DayTag,LD::Deci,LD::UnitExponent<LD::CT::Ratio<1,1>>>;

    template<typename T>
    using Day = LD::Unit<T,SecondTag,LD::DayTag,LD::CT::Ratio<1,1>,LD::UnitExponent<LD::CT::Ratio<1,1>>>;

    template<typename T>
    using DecaDay = LD::Unit<T,SecondTag,LD::DayTag,LD::Deca ,LD::UnitExponent<LD::CT::Ratio<1,1>>>;

    template<typename T>
    using HectoDay = LD::Unit<T,SecondTag,LD::DayTag,LD::Hecto ,LD::UnitExponent<LD::CT::Ratio<1,1>>>;

    template<typename T>
    using KiloDay = LD::Unit<T,SecondTag,LD::DayTag,LD::Kilo ,LD::UnitExponent<LD::CT::Ratio<1,1>>>;

    template<typename T>
    using MegaDay = LD::Unit<T,SecondTag,LD::DayTag,LD::Mega ,LD::UnitExponent<LD::CT::Ratio<1,1>>>;

    template<typename T>
    using GigaDay = LD::Unit<T,SecondTag,LD::DayTag,LD::Giga ,LD::UnitExponent<LD::CT::Ratio<1,1>>>;

    template<typename T>
    using TeraDay = LD::Unit<T,SecondTag,LD::DayTag,LD::Tera ,LD::UnitExponent<LD::CT::Ratio<1,1>>>;

    template<typename T>
    using PetaDay = LD::Unit<T,SecondTag,LD::DayTag,LD::Peta ,LD::UnitExponent<LD::CT::Ratio<1,1>>>;

    template<typename T>
    using ExaDay = LD::Unit<T,SecondTag,LD::DayTag,LD::Exa ,LD::UnitExponent<LD::CT::Ratio<1,1>>>;

    template<typename T>
    using ZettaDay = LD::Unit<T,LD::SecondTag,LD::DayTag,LD::Zetta ,LD::UnitExponent<LD::CT::Ratio<1,1>>>;

    template<typename T>
    using YottaDay = LD::Unit<T,SecondTag,LD::DayTag,LD::Yotta ,LD::UnitExponent<LD::CT::Ratio<1,1>>>;
    //---------------------------------------------------End of Day Declaration----------------------------------------------------------------------------------------
    //---------------------------------------------------Beginning of Month Declaration--------------------------------------------------------------------------------
    template<typename T>
    using YoctoMonth = LD::Unit<T,SecondTag,LD::MonthTag,LD::Yocto ,LD::UnitExponent<LD::CT::Ratio<1,1>>>;

    template<typename T>
    using ZeptoMonth = LD::Unit<T,SecondTag,LD::MonthTag,LD::Zepto ,LD::UnitExponent<LD::CT::Ratio<1,1>>>;

    template<typename T>
    using AttoMonth = LD::Unit<T,SecondTag,LD::MonthTag,LD::Atto,LD::UnitExponent<LD::CT::Ratio<1,1>>>;

    template<typename T>
    using FemtoMonth = LD::Unit<T,SecondTag,LD::MonthTag,LD::Femto ,LD::UnitExponent<LD::CT::Ratio<1,1>>>;

    template<typename T>
    using PicoMonth = LD::Unit<T,SecondTag,LD::MonthTag,LD::Pico ,LD::UnitExponent<LD::CT::Ratio<1,1>>>;

    template<typename T>
    using NanoMonth = LD::Unit<T,SecondTag,LD::MonthTag,LD::Nano ,LD::UnitExponent<LD::CT::Ratio<1,1>>>;

    template<typename T>
    using MicroMonth = LD::Unit<T,SecondTag,LD::MonthTag,LD::Micro ,LD::UnitExponent<LD::CT::Ratio<1,1>>>;

    template<typename T>
    using MiliMonth = LD::Unit<T,SecondTag,LD::MonthTag,LD::Milli,LD::UnitExponent<LD::CT::Ratio<1,1>>>;

    template<typename T>
    using CentiMonth = LD::Unit<T,SecondTag,LD::MonthTag,LD::Centi ,LD::UnitExponent<LD::CT::Ratio<1,1>>>;

    template<typename T>
    using DeciMonth = LD::Unit<T,SecondTag,LD::MonthTag,LD::Deci,LD::UnitExponent<LD::CT::Ratio<1,1>>>;

    template<typename T>
    using Month = LD::Unit<T,SecondTag,LD::MonthTag,LD::CT::Ratio<1,1>,LD::UnitExponent<LD::CT::Ratio<1,1>>>;

    template<typename T>
    using DecaMonth = LD::Unit<T,SecondTag,LD::MonthTag,LD::Deca ,LD::UnitExponent<LD::CT::Ratio<1,1>>>;

    template<typename T>
    using HectoMonth = LD::Unit<T,SecondTag,LD::MonthTag,LD::Hecto ,LD::UnitExponent<LD::CT::Ratio<1,1>>>;

    template<typename T>
    using KiloMonth = LD::Unit<T,SecondTag,LD::MonthTag,LD::Kilo ,LD::UnitExponent<LD::CT::Ratio<1,1>>>;

    template<typename T>
    using MegaMonth = LD::Unit<T,SecondTag,LD::MonthTag,LD::Mega ,LD::UnitExponent<LD::CT::Ratio<1,1>>>;

    template<typename T>
    using GigaMonth = LD::Unit<T,SecondTag,LD::MonthTag,LD::Giga ,LD::UnitExponent<LD::CT::Ratio<1,1>>>;

    template<typename T>
    using TeraMonth = LD::Unit<T,SecondTag,LD::MonthTag,LD::Tera ,LD::UnitExponent<LD::CT::Ratio<1,1>>>;

    template<typename T>
    using PetaMonth = LD::Unit<T,SecondTag,LD::MonthTag,LD::Peta ,LD::UnitExponent<LD::CT::Ratio<1,1>>>;

    template<typename T>
    using ExaMonth = LD::Unit<T,SecondTag,LD::MonthTag,LD::Exa ,LD::UnitExponent<LD::CT::Ratio<1,1>>>;

    template<typename T>
    using ZettaMonth = LD::Unit<T,LD::SecondTag,LD::MonthTag,LD::Zetta ,LD::UnitExponent<LD::CT::Ratio<1,1>>>;

    template<typename T>
    using YottaMonth = LD::Unit<T,SecondTag,LD::MonthTag,LD::Yotta ,LD::UnitExponent<LD::CT::Ratio<1,1>>>;
    //--------------------------------------------------End of Month Declaration---------------------------------------------------------------------------------------
    //--------------------------------------------------Beginning of Year Declaration----------------------------------------------------------------------------------
    template<typename T>
    using YoctoYear = LD::Unit<T,SecondTag,LD::YearTag,LD::Yocto ,LD::UnitExponent<LD::CT::Ratio<1,1>>>;

    template<typename T>
    using ZeptoYear = LD::Unit<T,SecondTag,LD::YearTag,LD::Zepto ,LD::UnitExponent<LD::CT::Ratio<1,1>>>;

    template<typename T>
    using AttoYear = LD::Unit<T,SecondTag,LD::YearTag,LD::Atto,LD::UnitExponent<LD::CT::Ratio<1,1>>>;

    template<typename T>
    using FemtoYear = LD::Unit<T,SecondTag,LD::YearTag,LD::Femto ,LD::UnitExponent<LD::CT::Ratio<1,1>>>;

    template<typename T>
    using PicoYear = LD::Unit<T,SecondTag,LD::YearTag,LD::Pico ,LD::UnitExponent<LD::CT::Ratio<1,1>>>;

    template<typename T>
    using NanoYear = LD::Unit<T,SecondTag,LD::YearTag,LD::Nano ,LD::UnitExponent<LD::CT::Ratio<1,1>>>;

    template<typename T>
    using MicroYear = LD::Unit<T,SecondTag,LD::YearTag,LD::Micro ,LD::UnitExponent<LD::CT::Ratio<1,1>>>;

    template<typename T>
    using MiliYear = LD::Unit<T,SecondTag,LD::YearTag,LD::Milli,LD::UnitExponent<LD::CT::Ratio<1,1>>>;

    template<typename T>
    using CentiYear = LD::Unit<T,SecondTag,LD::YearTag,LD::Centi ,LD::UnitExponent<LD::CT::Ratio<1,1>>>;

    template<typename T>
    using DeciYear = LD::Unit<T,SecondTag,LD::YearTag,LD::Deci,LD::UnitExponent<LD::CT::Ratio<1,1>>>;

    template<typename T>
    using Year = LD::Unit<T,SecondTag,LD::YearTag,LD::CT::Ratio<1,1>,LD::UnitExponent<LD::CT::Ratio<1,1>>>;

    template<typename T>
    using DecaYear = LD::Unit<T,SecondTag,LD::YearTag,LD::Deca ,LD::UnitExponent<LD::CT::Ratio<1,1>>>;

    template<typename T>
    using HectoYear = LD::Unit<T,SecondTag,LD::YearTag,LD::Hecto ,LD::UnitExponent<LD::CT::Ratio<1,1>>>;

    template<typename T>
    using KiloYear = LD::Unit<T,SecondTag,LD::YearTag,LD::Kilo ,LD::UnitExponent<LD::CT::Ratio<1,1>>>;

    template<typename T>
    using MegaYear = LD::Unit<T,SecondTag,LD::YearTag,LD::Mega ,LD::UnitExponent<LD::CT::Ratio<1,1>>>;

    template<typename T>
    using GigaYear = LD::Unit<T,SecondTag,LD::YearTag,LD::Giga ,LD::UnitExponent<LD::CT::Ratio<1,1>>>;

    template<typename T>
    using TeraYear = LD::Unit<T,SecondTag,LD::YearTag,LD::Tera ,LD::UnitExponent<LD::CT::Ratio<1,1>>>;

    template<typename T>
    using PetaYear = LD::Unit<T,SecondTag,LD::YearTag,LD::Peta ,LD::UnitExponent<LD::CT::Ratio<1,1>>>;

    template<typename T>
    using ExaYear = LD::Unit<T,SecondTag,LD::YearTag,LD::Exa ,LD::UnitExponent<LD::CT::Ratio<1,1>>>;

    template<typename T>
    using ZettaYear = LD::Unit<T,LD::SecondTag,LD::YearTag,LD::Zetta ,LD::UnitExponent<LD::CT::Ratio<1,1>>>;

    template<typename T>
    using YottaYear = LD::Unit<T,SecondTag,LD::YearTag,LD::Yotta ,LD::UnitExponent<LD::CT::Ratio<1,1>>>;
    //---------------------------------------------------End of Year Declaration--------------------------------------------------------------------------------------


//-------------------------------------------------------Beginning of Meter Declaration--------------------------------------------------------------------------------
    template<typename T>
    using YoctoMeter = LD::Unit<T,MeterTag,MeterTag,LD::Yocto ,LD::UnitExponent<LD::CT::Ratio<1,1>>>;


    template<typename T>
    using ZeptoMeter = LD::Unit<T,MeterTag,MeterTag,LD::Zepto ,LD::UnitExponent<LD::CT::Ratio<1,1>>>;

    template<typename T>
    using AttoMeter = LD::Unit<T,MeterTag,MeterTag,LD::Atto,LD::UnitExponent<LD::CT::Ratio<1,1>>>;

    template<typename T>
    using FemtoMeter = LD::Unit<T,MeterTag,MeterTag,LD::Femto ,LD::UnitExponent<LD::CT::Ratio<1,1>>>;

    template<typename T>
    using PicoMeter = LD::Unit<T,MeterTag,MeterTag,LD::Pico ,LD::UnitExponent<LD::CT::Ratio<1,1>>>;

    template<typename T>
    using NanoMeter = LD::Unit<T,MeterTag,MeterTag,LD::Nano ,LD::UnitExponent<LD::CT::Ratio<1,1>>>;

    template<typename T>
    using MicroMeter = LD::Unit<T,MeterTag,MeterTag,LD::Micro ,LD::UnitExponent<LD::CT::Ratio<1,1>>>;

    template<typename T>
    using MiliMeter = LD::Unit<T,MeterTag,MeterTag,LD::Milli,LD::UnitExponent<LD::CT::Ratio<1,1>>>;

    template<typename T>
    using CentiMeter = LD::Unit<T,MeterTag,MeterTag,LD::Centi ,LD::UnitExponent<LD::CT::Ratio<1,1>>>;

    template<typename T>
    using DeciMeter = LD::Unit<T,MeterTag,MeterTag,LD::Deci,LD::UnitExponent<LD::CT::Ratio<1,1>>>;

    template<typename T>
    using Meter = LD::Unit<T,MeterTag,MeterTag,LD::CT::Ratio<1,1>,LD::UnitExponent<LD::CT::Ratio<1,1>>>;

    template<typename T>
    using DecaMeter= LD::Unit<T,MeterTag,MeterTag,LD::Deca ,LD::UnitExponent<LD::CT::Ratio<1,1>>>;

    template<typename T>
    using HectoMeter = LD::Unit<T,MeterTag,MeterTag,LD::Hecto ,LD::UnitExponent<LD::CT::Ratio<1,1>>>;

    template<typename T>
    using KiloMeter = LD::Unit<T,MeterTag,MeterTag,LD::Kilo ,LD::UnitExponent<LD::CT::Ratio<1,1>>>;

    template<typename T>
    using MegaMeter = LD::Unit<T,MeterTag,MeterTag,LD::Mega ,LD::UnitExponent<LD::CT::Ratio<1,1>>>;

    template<typename T>
    using GigaMeter = LD::Unit<T,MeterTag,MeterTag,LD::Giga ,LD::UnitExponent<LD::CT::Ratio<1,1>>>;

    template<typename T>
    using TeraMeter = LD::Unit<T,MeterTag,MeterTag,LD::Tera ,LD::UnitExponent<LD::CT::Ratio<1,1>>>;

    template<typename T>
    using PetaMeter = LD::Unit<T,MeterTag,MeterTag,LD::Peta ,LD::UnitExponent<LD::CT::Ratio<1,1>>>;

    template<typename T>
    using ExaMeter = LD::Unit<T,MeterTag,MeterTag,LD::Exa ,LD::UnitExponent<LD::CT::Ratio<1,1>>>;

    template<typename T>
    using ZettaMeter = LD::Unit<T,MeterTag,MeterTag,LD::Zetta ,LD::UnitExponent<LD::CT::Ratio<1,1>>>;

    template<typename T>
    using YottaMeter = LD::Unit<T,MeterTag,MeterTag,LD::Yotta ,LD::UnitExponent<LD::CT::Ratio<1,1>>>;
    //----------------------------------------------------------End of Meter Declaration--------------------------------------------------------------------------------
    //----------------------------------------------------------beginning of Feet Declaration---------------------------------------------------------------------------
    template<typename T>
    using Foot = LD::Unit<T,LD::FootTag,LD::FootTag,LD::CT::Ratio<1,1>,LD::UnitExponent<LD::CT::Ratio<1,1>>>;
}
//-------------------------------------- Beginning of Time Suffixes -----------------------------------------------------------

inline LD::YoctoSecond<LD::Float> operator""_ys(long double second) noexcept
{
    return LD::YoctoSecond<LD::Float>{LD::Float {static_cast<LD::Float>(second)}};

}

inline LD::YoctoSecond<unsigned long long int > operator""_ys(unsigned long long int second) noexcept
{
    using CurrentType = unsigned long long int;
    return LD::YoctoSecond<unsigned long long int >{CurrentType {static_cast<CurrentType>(second)}};

}

inline LD::ZeptoSecond<LD::Float> operator""_zs(long double second) noexcept
{
    return LD::ZeptoSecond<LD::Float>{LD::Float {static_cast<LD::Float>(second)}};

}

inline LD::ZeptoSecond<unsigned long long int > operator""_zs(unsigned long long int second) noexcept
{
    using CurrentType = unsigned long long int;
    return LD::ZeptoSecond<unsigned long long int >{CurrentType {static_cast<CurrentType>(second)}};

}

inline LD::AttoSecond<LD::Float> operator""_as(long double second) noexcept
{
    return LD::AttoSecond<LD::Float>{LD::Float {static_cast<LD::Float>(second)}};

}

inline LD::AttoSecond<unsigned long long int > operator""_as(unsigned long long int second) noexcept
{
    using CurrentType = unsigned long long int;
    return LD::AttoSecond<unsigned long long int >{CurrentType {static_cast<CurrentType>(second)}};

}

inline LD::FemtoSecond<LD::Float> operator""_fs(long double second) noexcept
{
    return LD::FemtoSecond<LD::Float>{LD::Float {static_cast<LD::Float>(second)}};

}

inline LD::FemtoSecond<unsigned long long int > operator""_fs(unsigned long long int second) noexcept
{
    using CurrentType = unsigned long long int;
    return LD::FemtoSecond<unsigned long long int >{CurrentType {static_cast<CurrentType>(second)}};

}

inline LD::PicoSecond<LD::Float> operator""_ps(long double second) noexcept
{
    return LD::PicoSecond<LD::Float>{LD::Float {static_cast<LD::Float>(second)}};

}

inline LD::PicoSecond<unsigned long long int > operator""_ps(unsigned long long int second) noexcept
{
    using CurrentType = unsigned long long int;
    return LD::PicoSecond<unsigned long long int >{CurrentType {static_cast<CurrentType>(second)}};

}

inline LD::NanoSecond<LD::Float> operator""_ns(long double second) noexcept
{
    return LD::NanoSecond<LD::Float>{LD::Float {static_cast<LD::Float>(second)}};

}

inline LD::NanoSecond<unsigned long long int > operator""_ns(unsigned long long int second) noexcept
{
    using CurrentType = unsigned long long int;
    return LD::NanoSecond<unsigned long long int >{CurrentType {static_cast<CurrentType>(second)}};

}
inline LD::MicroSecond<LD::Float> operator""_us(long double second) noexcept
{
    return LD::MicroSecond<LD::Float>{LD::Float {static_cast<LD::Float>(second)}};

}
inline LD::MicroSecond<unsigned long long int > operator""_us(unsigned long long int second) noexcept
{
    using CurrentType = unsigned long long int;
    return LD::MicroSecond<unsigned long long int >{CurrentType {static_cast<CurrentType>(second)}};

}


inline LD::Milisecond<LD::Float> operator""_ms(long double second) noexcept
{
    return LD::Milisecond<LD::Float>{LD::Float {static_cast<LD::Float>(second)}};

}

inline LD::Milisecond<unsigned long long int > operator""_ms(unsigned long long int second) noexcept
{
    using CurrentType = unsigned long long int;
    return LD::Milisecond<unsigned long long int >{CurrentType {static_cast<CurrentType>(second)}};

}

inline LD::CentiSecond<LD::Float> operator""_cs(long double second) noexcept
{
    return LD::CentiSecond<LD::Float>{LD::Float {static_cast<LD::Float>(second)}};

}
inline LD::CentiSecond<unsigned long long int > operator""_cs(unsigned long long int second) noexcept
{
    using CurrentType = unsigned long long int;
    return LD::CentiSecond<unsigned long long int >{CurrentType {static_cast<CurrentType>(second)}};

}
inline LD::DeciSecond<LD::Float> operator""_ds(long double second) noexcept
{
    return LD::DeciSecond<LD::Float>{LD::Float {static_cast<LD::Float>(second)}};

}
inline LD::DeciSecond<unsigned long long int > operator""_ds(unsigned long long int second) noexcept
{
    using CurrentType = unsigned long long int;
    return LD::DeciSecond<unsigned long long int >{CurrentType {static_cast<CurrentType>(second)}};

}
inline LD::Second<unsigned long long int> operator "" _s(unsigned long long int a)
{
    return LD::Second<unsigned long long int>(LD::SecondTag<unsigned long long int>{a});
}
inline LD::Second<LD::Float> operator""_s(long double second) noexcept
{
    return LD::Second<LD::Float>{LD::Float {static_cast<LD::Float>(second)}};

}

inline LD::DecaSecond<unsigned long long int> operator "" _das(unsigned long long int a)
{
    return LD::DecaSecond<unsigned long long int>(LD::SecondTag<unsigned long long int>{a});
}

inline LD::DecaSecond<LD::Float> operator""_das(long double second) noexcept
{
    return LD::DecaSecond<LD::Float>{LD::Float {static_cast<LD::Float>(second)}};

}

inline LD::HectoSecond<unsigned long long int> operator "" _hs(unsigned long long int a)
{
    return LD::HectoSecond<unsigned long long int>(LD::SecondTag<unsigned long long int>{a});
}

inline LD::HectoSecond<LD::Float> operator""_hs(long double second) noexcept
{
    return LD::HectoSecond<LD::Float>{LD::Float {static_cast<LD::Float>(second)}};

}

inline LD::KiloSecond<unsigned long long int> operator "" _ks(unsigned long long int a)
{
    return LD::KiloSecond<unsigned long long int>(LD::SecondTag<unsigned long long int>{a});
}

inline LD::KiloSecond<LD::Float> operator""_ks(long double second) noexcept
{
    return LD::KiloSecond<LD::Float>{LD::Float {static_cast<LD::Float>(second)}};

}

inline LD::MegaSecond<unsigned long long int> operator "" _Ms(unsigned long long int a)
{
    return LD::MegaSecond<unsigned long long int>(LD::SecondTag<unsigned long long int>{a});
}

inline LD::MegaSecond<LD::Float> operator""_Ms(long double second) noexcept
{
    return LD::MegaSecond<LD::Float>{LD::Float {static_cast<LD::Float>(second)}};

}

inline LD::GigaSecond<unsigned long long int> operator "" _Gs(unsigned long long int a)
{
    return LD::GigaSecond<unsigned long long int>(LD::SecondTag<unsigned long long int>{a});
}

inline LD::GigaSecond<LD::Float> operator""_Gs(long double second) noexcept
{
    return LD::GigaSecond<LD::Float>{LD::Float {static_cast<LD::Float>(second)}};

}

inline LD::TeraSecond<unsigned long long int> operator "" _Ts(unsigned long long int a)
{
    return LD::TeraSecond<unsigned long long int>(LD::SecondTag<unsigned long long int>{a});
}
inline LD::TeraSecond<LD::Float> operator""_Ts(long double second) noexcept
{
    return LD::TeraSecond<LD::Float>{LD::Float {static_cast<LD::Float>(second)}};

}

inline LD::PetaSecond<unsigned long long int> operator "" _Ps(unsigned long long int a)
{
    return LD::PetaSecond<unsigned long long int>(LD::SecondTag<unsigned long long int>{a});
}

inline LD::PetaSecond<LD::Float> operator""_Ps(long double second) noexcept
{
    return LD::PetaSecond<LD::Float>{LD::Float {static_cast<LD::Float>(second)}};

}

inline LD::ExaSecond<unsigned long long int> operator "" _Es(unsigned long long int a)
{
    return LD::ExaSecond<unsigned long long int>(LD::SecondTag<unsigned long long int>{a});
}

inline LD::ExaSecond<LD::Float> operator""_Es(long double second) noexcept
{
    return LD::ExaSecond<LD::Float>{LD::Float {static_cast<LD::Float>(second)}};

}

inline LD::ZettaSecond<unsigned long long int> operator "" _Zs(unsigned long long int a)
{
    return LD::ZettaSecond<unsigned long long int>(LD::SecondTag<unsigned long long int>{a});
}

inline LD::ZettaSecond<LD::Float> operator""_Zs(long double second) noexcept
{
    return LD::ZettaSecond<LD::Float>{LD::Float {static_cast<LD::Float>(second)}};

}

inline LD::YottaSecond<unsigned long long int> operator "" _Ys(unsigned long long int a)
{
    return LD::YottaSecond<unsigned long long int>(LD::SecondTag<unsigned long long int>{a});
}
inline LD::YottaSecond<LD::Float> operator""_Ys(long double second) noexcept
{
    return LD::YottaSecond<LD::Float>{LD::Float {static_cast<LD::Float>(second)}};

}
//-------------------------------------- End of Time Suffixes -----------------------------------------------------------




inline LD::GigaByte<LD::UInteger> operator""_GB(unsigned long long int amount) noexcept
{
    return LD::GigaByte<LD::UInteger>{LD::UInteger{amount}};
}

inline LD::MegaByte<LD::UInteger> operator""_MB(unsigned long long int amount) noexcept
{
    return LD::MegaByte<LD::UInteger>{LD::UInteger{amount}};
}

namespace LD
{
    template<typename T, template<typename> class BaseTag, template<typename> class CurrentTag,
            LD::UInteger PrefixRatioNumerator, LD::UInteger PrefixRatioDenom,
            LD::UInteger UnitExponentNumerator, LD::UInteger UnitExponentDenominator>
    auto ToImmutableString(const LD::Unit<T,BaseTag,CurrentTag,LD::CT::Ratio<PrefixRatioNumerator,PrefixRatioDenom>,LD::UnitExponent<LD::CT::Ratio<UnitExponentNumerator,UnitExponentDenominator>>> & unit) noexcept
    {
        return unit.ToImmutableString();
    }

    template<typename T, template<typename> class BaseTag, template<typename> class CurrentTag,
            LD::UInteger PrefixRatioNumerator, LD::UInteger PrefixRatioDenom,
            LD::UInteger UnitExponentNumerator, LD::UInteger UnitExponentDenominator, typename ... A>
    LD::RequestResponse<LD::Unit<T,BaseTag,CurrentTag,LD::CT::Ratio<PrefixRatioNumerator,PrefixRatioDenom>,LD::UnitExponent<LD::CT::Ratio<UnitExponentNumerator,UnitExponentDenominator>>>(A...)> FromString(
            LD::Type<LD::Unit<T,BaseTag,CurrentTag,LD::CT::Ratio<PrefixRatioNumerator,PrefixRatioDenom>,LD::UnitExponent<LD::CT::Ratio<UnitExponentNumerator,UnitExponentDenominator>>>>,
            LD::StringView view,
            A && ... args) noexcept
    {
        /*
        if (auto m = ctre::match<LD::Detail::BooleanRegularExpressionTrue0>(view))
        {
            return LD::CreateResponse(LD::Type<bool>{},true,LD::Forward<A>(args)...);
        }else if(auto m = ctre::match<LD::Detail::BooleanRegularExpressionFalse0>(view))
        {
            return LD::CreateResponse(LD::Type<bool>{},false,LD::Forward<A>(args)...);
        }else if(auto m = ctre::match<LD::Detail::BooleanRegularExpressionYes0>(view))
        {
            return LD::CreateResponse(LD::Type<bool>{},true,LD::Forward<A>(args)...);
        }
        else if(auto m = ctre::match<LD::Detail::BooleanRegularExpressionNo0>(view))
        {
            return LD::CreateResponse(LD::Type<bool>{},false,LD::Forward<A>(args)...);
        }
         */

        return LD::FromString(LD::Type<double>{},view);
        //return LD::CreateResponse(LD::Type<bool>{},LD::TransactionError{},LD::Forward<A>(args)...);
    }
}
namespace LD
{
    template<typename T,
            template<typename> class BaseTag,
            template<typename> class CurrentTag,
            LD::UInteger PrefixRatioNumerator, LD::UInteger PrefixRatioDenom,
            LD::UInteger UnitExponentNumerator, LD::UInteger UnitExponentDenominator
    >
    inline Unit<T,BaseTag,CurrentTag,LD::CT::Ratio<PrefixRatioNumerator,PrefixRatioDenom>,LD::UnitExponent<LD::CT::Ratio<UnitExponentNumerator,UnitExponentDenominator>>> Exp(Unit<T,BaseTag,CurrentTag,LD::CT::Ratio<PrefixRatioNumerator,PrefixRatioDenom>,LD::UnitExponent<LD::CT::Ratio<UnitExponentNumerator,UnitExponentDenominator>>> unit)
    {
        return Unit<T,BaseTag,CurrentTag,LD::CT::Ratio<PrefixRatioNumerator,PrefixRatioDenom>,LD::UnitExponent<LD::CT::Ratio<UnitExponentNumerator,UnitExponentDenominator>>>{CurrentTag<T>{LD::Exp(unit.NativeRepresentation().Value())}};
    }
    template<typename T,
            template<typename> class BaseTag,
            template<typename> class CurrentTag,
            LD::UInteger PrefixRatioNumerator, LD::UInteger PrefixRatioDenom,
            LD::UInteger UnitExponentNumerator, LD::UInteger UnitExponentDenominator
    >
    inline Unit<T,BaseTag,CurrentTag,LD::CT::Ratio<PrefixRatioNumerator,PrefixRatioDenom>,LD::UnitExponent<LD::CT::Ratio<UnitExponentNumerator,UnitExponentDenominator>>>  Ln(Unit<T,BaseTag,CurrentTag,LD::CT::Ratio<PrefixRatioNumerator,PrefixRatioDenom>,LD::UnitExponent<LD::CT::Ratio<UnitExponentNumerator,UnitExponentDenominator>>> unit)
    {
        return Unit<T,BaseTag,CurrentTag,LD::CT::Ratio<PrefixRatioNumerator,PrefixRatioDenom>,LD::UnitExponent<LD::CT::Ratio<UnitExponentNumerator,UnitExponentDenominator>>>{CurrentTag<T>{LD::Ln(unit.NativeRepresentation().Value())}};
    }

    template<typename T,
            template<typename> class BaseTag,
            template<typename> class CurrentTag,
            LD::UInteger PrefixRatioNumerator, LD::UInteger PrefixRatioDenom,
            LD::UInteger UnitExponentNumerator, LD::UInteger UnitExponentDenominator
    >
    inline Unit<T,BaseTag,CurrentTag,LD::CT::Ratio<PrefixRatioNumerator,PrefixRatioDenom>,LD::UnitExponent<LD::CT::Ratio<UnitExponentNumerator,UnitExponentDenominator>>>  Log(Unit<T,BaseTag,CurrentTag,LD::CT::Ratio<PrefixRatioNumerator,PrefixRatioDenom>,LD::UnitExponent<LD::CT::Ratio<UnitExponentNumerator,UnitExponentDenominator>>> unit, T base = 10)
    {
        return Unit<T,BaseTag,CurrentTag,LD::CT::Ratio<PrefixRatioNumerator,PrefixRatioDenom>,LD::UnitExponent<LD::CT::Ratio<UnitExponentNumerator,UnitExponentDenominator>>>{CurrentTag<T>{LD::Log(unit.NativeRepresentation().Value(),base)}};
    }
    template<typename T,
            template<typename> class BaseTag,
            template<typename> class CurrentTag,
            LD::UInteger PrefixRatioNumerator, LD::UInteger PrefixRatioDenom,
            LD::UInteger UnitExponentNumerator, LD::UInteger UnitExponentDenominator
    >
    inline Unit<T,BaseTag,CurrentTag,LD::CT::Ratio<PrefixRatioNumerator,PrefixRatioDenom>,LD::UnitExponent<LD::CT::Ratio<UnitExponentNumerator,UnitExponentDenominator>>>  Log10(Unit<T,BaseTag,CurrentTag,LD::CT::Ratio<PrefixRatioNumerator,PrefixRatioDenom>,LD::UnitExponent<LD::CT::Ratio<UnitExponentNumerator,UnitExponentDenominator>>> unit)
    {
        return Unit<T,BaseTag,CurrentTag,LD::CT::Ratio<PrefixRatioNumerator,PrefixRatioDenom>,LD::UnitExponent<LD::CT::Ratio<UnitExponentNumerator,UnitExponentDenominator>>>{CurrentTag<T>{LD::Log10(unit.NativeRepresentation().Value())}};
    }

    template<typename T,
            template<typename> class BaseTag,
            template<typename> class CurrentTag,
            LD::UInteger PrefixRatioNumerator, LD::UInteger PrefixRatioDenom,
            LD::UInteger UnitExponentNumerator, LD::UInteger UnitExponentDenominator
    >
    inline Unit<T,BaseTag,CurrentTag,LD::CT::Ratio<PrefixRatioNumerator,PrefixRatioDenom>,LD::UnitExponent<LD::CT::Ratio<UnitExponentNumerator,UnitExponentDenominator>>>  Log2(Unit<T,BaseTag,CurrentTag,LD::CT::Ratio<PrefixRatioNumerator,PrefixRatioDenom>,LD::UnitExponent<LD::CT::Ratio<UnitExponentNumerator,UnitExponentDenominator>>> unit)
    {
        return Unit<T,BaseTag,CurrentTag,LD::CT::Ratio<PrefixRatioNumerator,PrefixRatioDenom>,LD::UnitExponent<LD::CT::Ratio<UnitExponentNumerator,UnitExponentDenominator>>>{CurrentTag<T>{LD::Log2(unit.NativeRepresentation().Value())}};
    }

    template<typename T,
            template<typename> class BaseTag,
            template<typename> class CurrentTag,
            LD::UInteger PrefixRatioNumerator, LD::UInteger PrefixRatioDenom,
            LD::UInteger UnitExponentNumerator, LD::UInteger UnitExponentDenominator
    >
    inline Unit<T,BaseTag,CurrentTag,LD::CT::Ratio<PrefixRatioNumerator,PrefixRatioDenom>,LD::UnitExponent<LD::CT::Ratio<UnitExponentNumerator,UnitExponentDenominator>>>  GCD(
            Unit<T,BaseTag,CurrentTag,LD::CT::Ratio<PrefixRatioNumerator,PrefixRatioDenom>,LD::UnitExponent<LD::CT::Ratio<UnitExponentNumerator,UnitExponentDenominator>>> a,
            Unit<T,BaseTag,CurrentTag,LD::CT::Ratio<PrefixRatioNumerator,PrefixRatioDenom>,LD::UnitExponent<LD::CT::Ratio<UnitExponentNumerator,UnitExponentDenominator>>> b)
    {
        return Unit<T,BaseTag,CurrentTag,LD::CT::Ratio<PrefixRatioNumerator,PrefixRatioDenom>,LD::UnitExponent<LD::CT::Ratio<UnitExponentNumerator,UnitExponentDenominator>>>{CurrentTag<T>{LD::GCD(a.NativeRepresentation().Value(),b.NativeRepresentation().Value())}};
    }

    template<typename T,
            template<typename> class BaseTag,
            template<typename> class CurrentTag,
            LD::UInteger PrefixRatioNumerator, LD::UInteger PrefixRatioDenom,
            LD::UInteger UnitExponentNumerator, LD::UInteger UnitExponentDenominator
    >
    inline Unit<T,BaseTag,CurrentTag,LD::CT::Ratio<PrefixRatioNumerator,PrefixRatioDenom>,LD::UnitExponent<LD::CT::Ratio<UnitExponentNumerator,UnitExponentDenominator>>>  LCM(
            Unit<T,BaseTag,CurrentTag,LD::CT::Ratio<PrefixRatioNumerator,PrefixRatioDenom>,LD::UnitExponent<LD::CT::Ratio<UnitExponentNumerator,UnitExponentDenominator>>> a,
            Unit<T,BaseTag,CurrentTag,LD::CT::Ratio<PrefixRatioNumerator,PrefixRatioDenom>,LD::UnitExponent<LD::CT::Ratio<UnitExponentNumerator,UnitExponentDenominator>>> b)
    {
        return Unit<T,BaseTag,CurrentTag,LD::CT::Ratio<PrefixRatioNumerator,PrefixRatioDenom>,LD::UnitExponent<LD::CT::Ratio<UnitExponentNumerator,UnitExponentDenominator>>>{CurrentTag<T>{LD::LCM(a.NativeRepresentation().Value(),b.NativeRepresentation().Value())}};
    }

    template<typename T,
            template<typename> class BaseTag,
            template<typename> class CurrentTag,
            LD::UInteger PrefixRatioNumerator, LD::UInteger PrefixRatioDenom,
            LD::UInteger UnitExponentNumerator, LD::UInteger UnitExponentDenominator
    >
    inline Unit<T,BaseTag,CurrentTag,LD::CT::Ratio<PrefixRatioNumerator,PrefixRatioDenom>,LD::UnitExponent<LD::CT::Ratio<UnitExponentNumerator,UnitExponentDenominator>>>  Ceil(Unit<T,BaseTag,CurrentTag,LD::CT::Ratio<PrefixRatioNumerator,PrefixRatioDenom>,LD::UnitExponent<LD::CT::Ratio<UnitExponentNumerator,UnitExponentDenominator>>> unit)
    {
        return Unit<T,BaseTag,CurrentTag,LD::CT::Ratio<PrefixRatioNumerator,PrefixRatioDenom>,LD::UnitExponent<LD::CT::Ratio<UnitExponentNumerator,UnitExponentDenominator>>>{CurrentTag<T>{LD::Ceil(unit.NativeRepresentation().Value())}};
    }

    template<typename T,
            template<typename> class BaseTag,
            template<typename> class CurrentTag,
            LD::UInteger PrefixRatioNumerator, LD::UInteger PrefixRatioDenom,
            LD::UInteger UnitExponentNumerator, LD::UInteger UnitExponentDenominator
    >
    inline Unit<T,BaseTag,CurrentTag,LD::CT::Ratio<PrefixRatioNumerator,PrefixRatioDenom>,LD::UnitExponent<LD::CT::Ratio<UnitExponentNumerator,UnitExponentDenominator>>>  Fmod(
            Unit<T,BaseTag,CurrentTag,LD::CT::Ratio<PrefixRatioNumerator,PrefixRatioDenom>,LD::UnitExponent<LD::CT::Ratio<UnitExponentNumerator,UnitExponentDenominator>>> a,
            Unit<T,BaseTag,CurrentTag,LD::CT::Ratio<PrefixRatioNumerator,PrefixRatioDenom>,LD::UnitExponent<LD::CT::Ratio<UnitExponentNumerator,UnitExponentDenominator>>> b)
    {
        return Unit<T,BaseTag,CurrentTag,LD::CT::Ratio<PrefixRatioNumerator,PrefixRatioDenom>,LD::UnitExponent<LD::CT::Ratio<UnitExponentNumerator,UnitExponentDenominator>>>{CurrentTag<T>{LD::Fmod(a.NativeRepresentation().Value(),b.NativeRepresentation().Value())}};
    }

    template<typename T,
            template<typename> class BaseTag,
            template<typename> class CurrentTag,
            LD::UInteger PrefixRatioNumerator, LD::UInteger PrefixRatioDenom,
            LD::UInteger UnitExponentNumerator, LD::UInteger UnitExponentDenominator
    >
    inline Unit<T,BaseTag,CurrentTag,LD::CT::Ratio<PrefixRatioNumerator,PrefixRatioDenom>,LD::UnitExponent<LD::CT::Ratio<UnitExponentNumerator,UnitExponentDenominator>>>  Fraction(Unit<T,BaseTag,CurrentTag,LD::CT::Ratio<PrefixRatioNumerator,PrefixRatioDenom>,LD::UnitExponent<LD::CT::Ratio<UnitExponentNumerator,UnitExponentDenominator>>> unit)
    {
        return Unit<T,BaseTag,CurrentTag,LD::CT::Ratio<PrefixRatioNumerator,PrefixRatioDenom>,LD::UnitExponent<LD::CT::Ratio<UnitExponentNumerator,UnitExponentDenominator>>>{CurrentTag<T>{LD::Fraction(unit.NativeRepresentation().Value())}};
    }

    template<typename T,
            template<typename> class BaseTag,
            template<typename> class CurrentTag,
            LD::UInteger PrefixRatioNumerator, LD::UInteger PrefixRatioDenom,
            LD::UInteger UnitExponentNumerator, LD::UInteger UnitExponentDenominator
    >
    inline Unit<T,BaseTag,CurrentTag,LD::CT::Ratio<PrefixRatioNumerator,PrefixRatioDenom>,LD::UnitExponent<LD::CT::Ratio<UnitExponentNumerator,UnitExponentDenominator>>>  Max(
            Unit<T,BaseTag,CurrentTag,LD::CT::Ratio<PrefixRatioNumerator,PrefixRatioDenom>,LD::UnitExponent<LD::CT::Ratio<UnitExponentNumerator,UnitExponentDenominator>>> a,
            Unit<T,BaseTag,CurrentTag,LD::CT::Ratio<PrefixRatioNumerator,PrefixRatioDenom>,LD::UnitExponent<LD::CT::Ratio<UnitExponentNumerator,UnitExponentDenominator>>> b)
    {
        return Unit<T,BaseTag,CurrentTag,LD::CT::Ratio<PrefixRatioNumerator,PrefixRatioDenom>,LD::UnitExponent<LD::CT::Ratio<UnitExponentNumerator,UnitExponentDenominator>>>{CurrentTag<T>{LD::Max(a.NativeRepresentation().Value(),b.NativeRepresentation().Value())}};
    }

    template<typename T,
            template<typename> class BaseTag,
            template<typename> class CurrentTag,
            LD::UInteger PrefixRatioNumerator, LD::UInteger PrefixRatioDenom,
            LD::UInteger UnitExponentNumerator, LD::UInteger UnitExponentDenominator
    >
    inline Unit<T,BaseTag,CurrentTag,LD::CT::Ratio<PrefixRatioNumerator,PrefixRatioDenom>,LD::UnitExponent<LD::CT::Ratio<UnitExponentNumerator,UnitExponentDenominator>>>  Min(
            Unit<T,BaseTag,CurrentTag,LD::CT::Ratio<PrefixRatioNumerator,PrefixRatioDenom>,LD::UnitExponent<LD::CT::Ratio<UnitExponentNumerator,UnitExponentDenominator>>> a,
            Unit<T,BaseTag,CurrentTag,LD::CT::Ratio<PrefixRatioNumerator,PrefixRatioDenom>,LD::UnitExponent<LD::CT::Ratio<UnitExponentNumerator,UnitExponentDenominator>>> b)
    {
        return Unit<T,BaseTag,CurrentTag,LD::CT::Ratio<PrefixRatioNumerator,PrefixRatioDenom>,LD::UnitExponent<LD::CT::Ratio<UnitExponentNumerator,UnitExponentDenominator>>>{CurrentTag<T>{LD::Min(a.NativeRepresentation().Value(),b.NativeRepresentation().Value())}};
    }

    template<typename T,
            template<typename> class BaseTag,
            template<typename> class CurrentTag,
            LD::UInteger PrefixRatioNumerator, LD::UInteger PrefixRatioDenom,
            LD::UInteger UnitExponentNumerator, LD::UInteger UnitExponentDenominator
    >
    inline Unit<T,BaseTag,CurrentTag,LD::CT::Ratio<PrefixRatioNumerator,PrefixRatioDenom>,LD::UnitExponent<LD::CT::Ratio<UnitExponentNumerator,UnitExponentDenominator>>>  Clamp(
            Unit<T,BaseTag,CurrentTag,LD::CT::Ratio<PrefixRatioNumerator,PrefixRatioDenom>,LD::UnitExponent<LD::CT::Ratio<UnitExponentNumerator,UnitExponentDenominator>>> a,
            Unit<T,BaseTag,CurrentTag,LD::CT::Ratio<PrefixRatioNumerator,PrefixRatioDenom>,LD::UnitExponent<LD::CT::Ratio<UnitExponentNumerator,UnitExponentDenominator>>> b,
            Unit<T,BaseTag,CurrentTag,LD::CT::Ratio<PrefixRatioNumerator,PrefixRatioDenom>,LD::UnitExponent<LD::CT::Ratio<UnitExponentNumerator,UnitExponentDenominator>>> c)
    {
        return Unit<T,BaseTag,CurrentTag,LD::CT::Ratio<PrefixRatioNumerator,PrefixRatioDenom>,LD::UnitExponent<LD::CT::Ratio<UnitExponentNumerator,UnitExponentDenominator>>>{CurrentTag<T>{LD::Clamp(a.NativeRepresentation().Value(),b.NativeRepresentation().Value(),c.NativeRepresentation().Value())}};
    }

    template<typename T,
            template<typename> class BaseTag,
            template<typename> class CurrentTag,
            LD::UInteger PrefixRatioNumerator, LD::UInteger PrefixRatioDenom,
            LD::UInteger UnitExponentNumerator, LD::UInteger UnitExponentDenominator
    >
    inline Unit<T,BaseTag,CurrentTag,LD::CT::Ratio<PrefixRatioNumerator,PrefixRatioDenom>,LD::UnitExponent<LD::CT::Ratio<UnitExponentNumerator,UnitExponentDenominator>>>  Round(Unit<T,BaseTag,CurrentTag,LD::CT::Ratio<PrefixRatioNumerator,PrefixRatioDenom>,LD::UnitExponent<LD::CT::Ratio<UnitExponentNumerator,UnitExponentDenominator>>> unit)
    {
        return Unit<T,BaseTag,CurrentTag,LD::CT::Ratio<PrefixRatioNumerator,PrefixRatioDenom>,LD::UnitExponent<LD::CT::Ratio<UnitExponentNumerator,UnitExponentDenominator>>>{CurrentTag<T>{LD::Round(unit.NativeRepresentation().Value())}};
    }

    template<typename T,
            template<typename> class BaseTag,
            template<typename> class CurrentTag,
            LD::UInteger PrefixRatioNumerator, LD::UInteger PrefixRatioDenom,
            LD::UInteger UnitExponentNumerator, LD::UInteger UnitExponentDenominator
    >
    inline bool  IsNegative(Unit<T,BaseTag,CurrentTag,LD::CT::Ratio<PrefixRatioNumerator,PrefixRatioDenom>,LD::UnitExponent<LD::CT::Ratio<UnitExponentNumerator,UnitExponentDenominator>>> unit)
    {
        return LD::IsNegative(unit.NativeRepresentation().Value());
    }

    template<typename T,
            template<typename> class BaseTag,
            template<typename> class CurrentTag,
            LD::UInteger PrefixRatioNumerator, LD::UInteger PrefixRatioDenom,
            LD::UInteger UnitExponentNumerator, LD::UInteger UnitExponentDenominator
    >
    inline Unit<T,BaseTag,CurrentTag,LD::CT::Ratio<PrefixRatioNumerator,PrefixRatioDenom>,LD::UnitExponent<LD::CT::Ratio<UnitExponentNumerator,UnitExponentDenominator>>>  Sin(Unit<T,BaseTag,CurrentTag,LD::CT::Ratio<PrefixRatioNumerator,PrefixRatioDenom>,LD::UnitExponent<LD::CT::Ratio<UnitExponentNumerator,UnitExponentDenominator>>> unit)
    {
        return Unit<T,BaseTag,CurrentTag,LD::CT::Ratio<PrefixRatioNumerator,PrefixRatioDenom>,LD::UnitExponent<LD::CT::Ratio<UnitExponentNumerator,UnitExponentDenominator>>>{CurrentTag<T>{LD::Sin(unit.NativeRepresentation().Value())}};
    }

    template<typename T,
            template<typename> class BaseTag,
            template<typename> class CurrentTag,
            LD::UInteger PrefixRatioNumerator, LD::UInteger PrefixRatioDenom,
            LD::UInteger UnitExponentNumerator, LD::UInteger UnitExponentDenominator
    >
    inline Unit<T,BaseTag,CurrentTag,LD::CT::Ratio<PrefixRatioNumerator,PrefixRatioDenom>,LD::UnitExponent<LD::CT::Ratio<UnitExponentNumerator,UnitExponentDenominator>>>  Cos(Unit<T,BaseTag,CurrentTag,LD::CT::Ratio<PrefixRatioNumerator,PrefixRatioDenom>,LD::UnitExponent<LD::CT::Ratio<UnitExponentNumerator,UnitExponentDenominator>>> unit)
    {
        return Unit<T,BaseTag,CurrentTag,LD::CT::Ratio<PrefixRatioNumerator,PrefixRatioDenom>,LD::UnitExponent<LD::CT::Ratio<UnitExponentNumerator,UnitExponentDenominator>>>{CurrentTag<T>{LD::Cos(unit.NativeRepresentation().Value())}};
    }

    template<typename T,
            template<typename> class BaseTag,
            template<typename> class CurrentTag,
            LD::UInteger PrefixRatioNumerator, LD::UInteger PrefixRatioDenom,
            LD::UInteger UnitExponentNumerator, LD::UInteger UnitExponentDenominator
    >
    inline Unit<T,BaseTag,CurrentTag,LD::CT::Ratio<PrefixRatioNumerator,PrefixRatioDenom>,LD::UnitExponent<LD::CT::Ratio<UnitExponentNumerator,UnitExponentDenominator>>>  Tan(Unit<T,BaseTag,CurrentTag,LD::CT::Ratio<PrefixRatioNumerator,PrefixRatioDenom>,LD::UnitExponent<LD::CT::Ratio<UnitExponentNumerator,UnitExponentDenominator>>> unit)
    {
        return Unit<T,BaseTag,CurrentTag,LD::CT::Ratio<PrefixRatioNumerator,PrefixRatioDenom>,LD::UnitExponent<LD::CT::Ratio<UnitExponentNumerator,UnitExponentDenominator>>>{CurrentTag<T>{LD::Tan(unit.NativeRepresentation().Value())}};
    }
    template<typename T,
            template<typename> class BaseTag,
            template<typename> class CurrentTag,
            LD::UInteger PrefixRatioNumerator, LD::UInteger PrefixRatioDenom,
            LD::UInteger UnitExponentNumerator, LD::UInteger UnitExponentDenominator
    >
    inline Unit<T,BaseTag,CurrentTag,LD::CT::Ratio<PrefixRatioNumerator,PrefixRatioDenom>,LD::UnitExponent<LD::CT::Ratio<UnitExponentNumerator,UnitExponentDenominator>>>  Csc(Unit<T,BaseTag,CurrentTag,LD::CT::Ratio<PrefixRatioNumerator,PrefixRatioDenom>,LD::UnitExponent<LD::CT::Ratio<UnitExponentNumerator,UnitExponentDenominator>>> unit)
    {
        return Unit<T,BaseTag,CurrentTag,LD::CT::Ratio<PrefixRatioNumerator,PrefixRatioDenom>,LD::UnitExponent<LD::CT::Ratio<UnitExponentNumerator,UnitExponentDenominator>>>{CurrentTag<T>{LD::Csc(unit.NativeRepresentation().Value())}};
    }
    template<typename T,
            template<typename> class BaseTag,
            template<typename> class CurrentTag,
            LD::UInteger PrefixRatioNumerator, LD::UInteger PrefixRatioDenom,
            LD::UInteger UnitExponentNumerator, LD::UInteger UnitExponentDenominator
    >
    inline Unit<T,BaseTag,CurrentTag,LD::CT::Ratio<PrefixRatioNumerator,PrefixRatioDenom>,LD::UnitExponent<LD::CT::Ratio<UnitExponentNumerator,UnitExponentDenominator>>>  Sec(Unit<T,BaseTag,CurrentTag,LD::CT::Ratio<PrefixRatioNumerator,PrefixRatioDenom>,LD::UnitExponent<LD::CT::Ratio<UnitExponentNumerator,UnitExponentDenominator>>> unit)
    {
        return Unit<T,BaseTag,CurrentTag,LD::CT::Ratio<PrefixRatioNumerator,PrefixRatioDenom>,LD::UnitExponent<LD::CT::Ratio<UnitExponentNumerator,UnitExponentDenominator>>>{CurrentTag<T>{LD::Sec(unit.NativeRepresentation().Value())}};
    }
    template<typename T,
            template<typename> class BaseTag,
            template<typename> class CurrentTag,
            LD::UInteger PrefixRatioNumerator, LD::UInteger PrefixRatioDenom,
            LD::UInteger UnitExponentNumerator, LD::UInteger UnitExponentDenominator
    >
    inline Unit<T,BaseTag,CurrentTag,LD::CT::Ratio<PrefixRatioNumerator,PrefixRatioDenom>,LD::UnitExponent<LD::CT::Ratio<UnitExponentNumerator,UnitExponentDenominator>>>  CoTan(Unit<T,BaseTag,CurrentTag,LD::CT::Ratio<PrefixRatioNumerator,PrefixRatioDenom>,LD::UnitExponent<LD::CT::Ratio<UnitExponentNumerator,UnitExponentDenominator>>> unit)
    {
        return Unit<T,BaseTag,CurrentTag,LD::CT::Ratio<PrefixRatioNumerator,PrefixRatioDenom>,LD::UnitExponent<LD::CT::Ratio<UnitExponentNumerator,UnitExponentDenominator>>>{CurrentTag<T>{LD::CoTan(unit.NativeRepresentation().Value())}};
    }
    template<typename T,
            template<typename> class BaseTag,
            template<typename> class CurrentTag,
            LD::UInteger PrefixRatioNumerator, LD::UInteger PrefixRatioDenom,
            LD::UInteger UnitExponentNumerator, LD::UInteger UnitExponentDenominator
    >
    inline Unit<T,BaseTag,CurrentTag,LD::CT::Ratio<PrefixRatioNumerator,PrefixRatioDenom>,LD::UnitExponent<LD::CT::Ratio<UnitExponentNumerator,UnitExponentDenominator>>>  Sinh(Unit<T,BaseTag,CurrentTag,LD::CT::Ratio<PrefixRatioNumerator,PrefixRatioDenom>,LD::UnitExponent<LD::CT::Ratio<UnitExponentNumerator,UnitExponentDenominator>>> unit)
    {
        return Unit<T,BaseTag,CurrentTag,LD::CT::Ratio<PrefixRatioNumerator,PrefixRatioDenom>,LD::UnitExponent<LD::CT::Ratio<UnitExponentNumerator,UnitExponentDenominator>>>{CurrentTag<T>{LD::Sinh(unit.NativeRepresentation().Value())}};
    }
    template<typename T,
            template<typename> class BaseTag,
            template<typename> class CurrentTag,
            LD::UInteger PrefixRatioNumerator, LD::UInteger PrefixRatioDenom,
            LD::UInteger UnitExponentNumerator, LD::UInteger UnitExponentDenominator
    >
    inline Unit<T,BaseTag,CurrentTag,LD::CT::Ratio<PrefixRatioNumerator,PrefixRatioDenom>,LD::UnitExponent<LD::CT::Ratio<UnitExponentNumerator,UnitExponentDenominator>>>  Cosh(Unit<T,BaseTag,CurrentTag,LD::CT::Ratio<PrefixRatioNumerator,PrefixRatioDenom>,LD::UnitExponent<LD::CT::Ratio<UnitExponentNumerator,UnitExponentDenominator>>> unit)
    {
        return Unit<T,BaseTag,CurrentTag,LD::CT::Ratio<PrefixRatioNumerator,PrefixRatioDenom>,LD::UnitExponent<LD::CT::Ratio<UnitExponentNumerator,UnitExponentDenominator>>>{CurrentTag<T>{LD::Cosh(unit.NativeRepresentation().Value())}};
    }
}
#endif //LANDESSDEVCORE_UNIT_H