//
// Created by James Landess on 2/23/20.
//

#ifndef LANDESSDEVCORE_UNIT_H
#define LANDESSDEVCORE_UNIT_H

#include "Definitions/Common.hpp"
#include "Ratio.hpp"
#include "TypeTraits/ExclusiveOr.hpp"
namespace LD
{

    template<typename Exponent>
    class UnitExponent;


    template<LD::UInteger Numerator, LD::UInteger Denominator>
    class UnitExponent<LD::CT::Ratio<Numerator,Denominator>>
    {

    };



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
        inline constexpr explicit Unit() noexcept :mUnit(){}
        inline constexpr explicit Unit(const CurrentTag<T> & tag) noexcept :mUnit{tag}{}
        template<typename U, typename V = T ,class = LD::Enable_If_T<LD::Require<LD::Detail::IsConvertible<V,U>::value,LD::Concept::Arithmetical<U>>>>
        inline constexpr explicit Unit(const U & object) noexcept : mUnit{CurrentTag{object}}{}




        template<typename U, typename V = T ,typename R = decltype(LD::Declval<U>() * LD::Declval<V>())>
        constexpr LD::Enable_If_T<LD::Require<
        LD::Concept::Multipliable<U,V>
        >,
        Unit<R,BaseTag,CurrentTag,LD::CT::Ratio<PrefixRatioNumerator,PrefixRatioDenom>,LD::UnitExponent<LD::CT::Ratio<UnitExponentNumerator,UnitExponentDenominator>>>>
        operator * (const U & constant) const noexcept(noexcept(LD::Declval<V>() * LD::Declval<U>()))
        {
            return Unit<R,BaseTag,CurrentTag,LD::CT::Ratio<PrefixRatioNumerator,PrefixRatioDenom>,LD::UnitExponent<LD::CT::Ratio<UnitExponentNumerator,UnitExponentDenominator>>>{mUnit.Value()*constant};
        }


        template<typename U, typename V = T ,typename R = decltype(LD::Declval<U>() * LD::Declval<V>())>
        constexpr LD::Enable_If_T<LD::Require<
                LD::Concept::Multipliable<U,V>
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
            return Unit<R,BaseTag,Tag,ERatio1,LD::UnitExponent<U1>>{mUnit.Value() + instance.mUnit.Value()};
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
            mUnit.Value() = mUnit.Value()+unit.mUnit.Value();
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
            return Unit<R,BaseTag,CurrentTag,LD::CT::Ratio<PrefixRatioNumerator,PrefixRatioDenom>,LD::UnitExponent<LD::CT::Ratio<UnitExponentNumerator,UnitExponentDenominator>>>{mUnit.Value()*unit.mUnit.Value()};
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
    class UnitRelationship<LD::SecondTag<T>,LD::SecondTag<T>>
    {
    private:

    public:

        constexpr SecondTag<T> operator()(const SecondTag<T> & minute) const noexcept
        {
            return SecondTag<T>{minute.Value()};
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
    class MeterTag: UnitSkills<T>
    {
    private:
    public:
        constexpr explicit  MeterTag() noexcept : UnitSkills<T>(0) {}
        constexpr explicit MeterTag(const T & value) noexcept : UnitSkills<T>(value) {}
        constexpr MeterTag<T> operator()(const MeterTag<T> & m) const noexcept
        {
            return MeterTag<T>{this->mValue};
        }
    };



    template<typename T>
    using YoctoSecond = LD::Unit<T,SecondTag,SecondTag,LD::Yocto ,LD::UnitExponent<LD::CT::Ratio<1,1>>>;



    template<typename T>
    using ZeptoSecond = LD::Unit<T,SecondTag,SecondTag,LD::Zepto ,LD::UnitExponent<LD::CT::Ratio<1,1>>>;



    template<typename T>
    using AttoSecond = LD::Unit<T,SecondTag,SecondTag,LD::Atto,LD::UnitExponent<LD::CT::Ratio<1,1>>>;



    template<typename T>
    using FemtoSecond = LD::Unit<T,SecondTag,SecondTag,LD::Femto ,LD::UnitExponent<LD::CT::Ratio<1,1>>>;




    template<typename T>
    using PicoSecond = LD::Unit<T,SecondTag,SecondTag,LD::Pico ,LD::UnitExponent<LD::CT::Ratio<1,1>>>;




    template<typename T>
    using NanoSecond = LD::Unit<T,SecondTag,SecondTag,LD::Nano ,LD::UnitExponent<LD::CT::Ratio<1,1>>>;




    template<typename T>
    using MicroSecond = LD::Unit<T,SecondTag,SecondTag,LD::Micro ,LD::UnitExponent<LD::CT::Ratio<1,1>>>;




    template<typename T>
    using Milisecond = LD::Unit<T,SecondTag,SecondTag,LD::Milli,LD::UnitExponent<LD::CT::Ratio<1,1>>>;




    template<typename T>
    using CentiSecond = LD::Unit<T,SecondTag,SecondTag,LD::Centi ,LD::UnitExponent<LD::CT::Ratio<1,1>>>;




    template<typename T>
    using DeciSecond = LD::Unit<T,SecondTag,SecondTag,LD::Deci,LD::UnitExponent<LD::CT::Ratio<1,1>>>;



    template<typename T>
    using Second = LD::Unit<T,SecondTag,SecondTag,LD::CT::Ratio<1,1>,LD::UnitExponent<LD::CT::Ratio<1,1>>>;




    template<typename T>
    using DecaSecond = LD::Unit<T,SecondTag,SecondTag,LD::Deca ,LD::UnitExponent<LD::CT::Ratio<1,1>>>;




    template<typename T>
    using HectoSecond = LD::Unit<T,SecondTag,SecondTag,LD::Hecto ,LD::UnitExponent<LD::CT::Ratio<1,1>>>;




    template<typename T>
    using KiloSecond = LD::Unit<T,SecondTag,SecondTag,LD::Kilo ,LD::UnitExponent<LD::CT::Ratio<1,1>>>;





    template<typename T>
    using MegaSecond = LD::Unit<T,SecondTag,SecondTag,LD::Mega ,LD::UnitExponent<LD::CT::Ratio<1,1>>>;




    template<typename T>
    using GigaSecond = LD::Unit<T,SecondTag,SecondTag,LD::Giga ,LD::UnitExponent<LD::CT::Ratio<1,1>>>;




    template<typename T>
    using TeraSecond = LD::Unit<T,SecondTag,SecondTag,LD::Tera ,LD::UnitExponent<LD::CT::Ratio<1,1>>>;




    template<typename T>
    using PetaSecond = LD::Unit<T,SecondTag,SecondTag,LD::Peta ,LD::UnitExponent<LD::CT::Ratio<1,1>>>;




    template<typename T>
    using ExaSecond = LD::Unit<T,SecondTag,SecondTag,LD::Exa ,LD::UnitExponent<LD::CT::Ratio<1,1>>>;


    template<typename T>
    using ZettaSecond = LD::Unit<T,SecondTag,SecondTag,LD::Zetta ,LD::UnitExponent<LD::CT::Ratio<1,1>>>;


    template<typename T>
    using YottaSecond = LD::Unit<T,SecondTag,SecondTag,LD::Yotta ,LD::UnitExponent<LD::CT::Ratio<1,1>>>;


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
}



/*
template<typename T,
        typename U,
        typename DistanceTag,
        typename TimeTag,
        typename DistancePrefixRatio,
        typename TimePrefixRatio,
        typename DistanceFractionRatio,
        typename TimeFractionRatio
        >
constexpr LD::Ratio<int,int> operator / (const LD::Unit<T,LD::MeterTag<T>,DistanceTag,DistancePrefixRatio,DistanceFractionRatio,LD::CT::Ratio<1,1>> & distance,
                               const LD::Unit<T,LD::SecondTag<U>,TimeTag,TimePrefixRatio,TimeFractionRatio,LD::CT::Ratio<1,1>> & time) noexcept (noexcept(LD::Declval<T>()/LD::Declval<U>()))
{
    return LD::Ratio<int,int>{};
}
 */
#endif //LANDESSDEVCORE_UNIT_H
