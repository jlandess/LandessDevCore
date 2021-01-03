//
// Created by phoenixflower on 11/5/20.
//

#ifndef LANDESSDEVCORE_VARIABLE_HPP
#define LANDESSDEVCORE_VARIABLE_HPP
#include "Definitions/Integer.hpp"
#include "Primitives/General/Immutable.hpp"
#include "TypeTraits/Detection.hpp"
#include "Compositor.hpp"
#include "Algorithms/Exponential.hpp"
#include "TypeTraits/CanBeAnImmutableString.hpp"
#include "TypeTraits/IsExternalObjectSymbol.hpp"
namespace LD
{
    namespace SPA
    {
        class UnararyOperator
        {

        };

        class FunctionalOperator
        {

        };

        class NoOpt
        {

        };

        template<typename T,typename StringType>
        class ExternalObjectFunctionalSymbol
        {
        private:
            StringType mName;
        public:
            using NameType = StringType;
            ExternalObjectFunctionalSymbol() = default;
            ExternalObjectFunctionalSymbol(LD::Type<T>,StringType && name) noexcept: mName{LD::Forward<StringType>(name)}
            {

            }
            StringType Symbol() const noexcept
            {
                return this->mName;
            }
        };

        template<typename T, typename StringType>
        class ExternalObjectFunctionalMemberSymbol
        {
        private:
            StringType mName;
        public:
            ExternalObjectFunctionalMemberSymbol() = default;
            ExternalObjectFunctionalMemberSymbol(LD::Type<T>,StringType && name) noexcept: mName{LD::Forward<StringType>(name)}
            {

            }
            StringType Symbol() const noexcept
            {
                return this->mName;
            }
        };

        template <typename T, typename StringType> ExternalObjectFunctionalSymbol(LD::Type<T>, StringType &&) -> ExternalObjectFunctionalSymbol<T,StringType>;



        template<typename T,typename ValueType>
        class Var;
        template<typename T,typename Variables, typename Operator , typename OperationType ,class = void>
        class VarExpression;


        template<typename T>
        constexpr bool IsVarExpression(LD::Type<T> )noexcept
        {
            return false;
        }

        template<typename T, typename ... V, typename O, typename OT>
        constexpr bool IsVarExpression(LD::Type<LD::SPA::VarExpression<T,LD::CT::TypeList<V...>,O,OT>> ) noexcept
        {
            return true;
        }

        template<typename T>
        constexpr bool IsVar(LD::Type<T>) noexcept
        {
            return false;
        }

        template<typename T, typename ValueType>
        constexpr bool IsVar(LD::Type<LD::SPA::Var<T,ValueType>>) noexcept
        {
            return true;
        }


        template<typename T,typename ValueType>
        class Var
        {
        private:
            LD::UInteger mID;
            LD::UInteger mOrder;
            ValueType mValue;
        public:
            using VarType = LD::Type<ValueType>;
            template<typename U, typename ... B, class = LD::Enable_If_T<LD::Require<LD::ConvertiblyCallable<U,LD::UInteger()>::Value()>>>
            Var(ValueType && value,U && randomNumberGenerator) noexcept:mValue{LD::Forward<ValueType>(value)},mOrder{0}
            {
                this->mID = randomNumberGenerator();
                if constexpr (LD::SPA::IsVarExpression(LD::CT::RemoveQualifiers(LD::Type<ValueType>{}))||
                              LD::SPA::IsVar(LD::CT::RemoveQualifiers(LD::Type<ValueType>{})))
                {
                    this->mOrder = value.Order()+1;
                }
            }


            LD::UInteger Identification() const noexcept
            {
                return this->mID;
            }

            const ValueType & DefaultValue() const noexcept
            {
                return this->mValue;
            }

            LD::UInteger Order() const noexcept
            {
                return this->mOrder;
            }

            auto operator + (const LD::SPA::Var<T,ValueType> & variable) const noexcept
            {

                return LD::SPA::VarExpression{LD::Type<ValueType>{},LD::SPA::UnararyOperator{},LD::ImmutableString{"+"},*this,variable};
            }

            auto operator - (const LD::SPA::Var<T,ValueType> & variable) const noexcept
            {

                return LD::SPA::VarExpression{LD::Type<ValueType>{},LD::SPA::UnararyOperator{},LD::ImmutableString{"+"},*this,variable};
            }

            auto operator * (const LD::SPA::Var<T,ValueType> & variable) const noexcept
            {
                return LD::SPA::VarExpression{LD::Type<ValueType>{},LD::SPA::UnararyOperator{},LD::ImmutableString{"*"},*this,variable};
            }
            auto operator / (const LD::SPA::Var<T,ValueType> & variable) const noexcept
            {
                return LD::SPA::VarExpression{LD::Type<ValueType>{},LD::SPA::UnararyOperator{},LD::ImmutableString{"/"},*this,variable};
            }
        };

        template <typename T, typename U, typename ... B> Var(T && value, U &&, B && ...) -> Var<LD::Type<T>,T>;

        template<typename T,typename Operator,typename OperationType,typename U,typename ... Variables> Var(const LD::SPA::VarExpression<T,LD::CT::TypeList<Variables...>,Operator,OperationType> &, U &&) -> Var<LD::Type<T>,const LD::SPA::VarExpression<T,LD::CT::TypeList<Variables...>,Operator,OperationType> &>;



        template<typename ... B>
        constexpr LD::UInteger GetNumberOfVariables(LD::CT::TypeList<B...>) noexcept
        {
            using Auo1 = LD::CT::TypeList<B...>;
            LD::UInteger result = 0;
            LD::For<Auo1::size()>([](auto I,LD::UInteger & result)
            {
                result += LD::SPA::IsVar(LD::CT::RemoveQualifiers(LD::Type<LD::CT::TypeAtIndex<I,Auo1>>{}));
                return true;
            },result);
            return result;
        }
        template<typename ... B>
        constexpr LD::UInteger GetNumberOfExpressions(LD::CT::TypeList<B...>) noexcept
        {
            using Auo1 = LD::CT::TypeList<B...>;
            LD::UInteger result = 0;
            LD::For<Auo1::size()>([](auto I,LD::UInteger & result)
            {

                result += LD::SPA::IsVarExpression(LD::CT::RemoveQualifiers(LD::Type<LD::CT::TypeAtIndex<I,Auo1>>{}));

                return true;

            },result);
            return result;
        }
    }
}

namespace LD
{
    namespace SPA
    {


        /**
         *
         * @tparam T
         * @tparam Operator
         * @tparam OperationType
         * @tparam Variables
         * @brief VarExpressions can only be composed from instances of LD::SPA::Var @see LD::SPA::Var or instances of LD::SPA::VarExpression (sub-expressions)
         */
        template<typename T,typename Operator,typename OperationType,typename ... Variables>
        class VarExpression<T,LD::CT::TypeList<Variables...>,Operator,OperationType,LD::Enable_If_T<
                LD::Require<
                        (LD::SPA::GetNumberOfVariables(LD::CT::TypeList<Variables...>{})+LD::SPA::GetNumberOfExpressions(LD::CT::TypeList<Variables...>{}) == sizeof...(Variables)),
                        LD::IsImmutable<LD::Detail::Decay_T<Operator>>
                >>>
        {
        private:
            LD::Tuple<Variables...> mTuple;
            OperationType mOperator;
            Operator mOperatorSymbol;
            LD::UInteger mOrder;

        public:
            using OpType = OperationType;
            using Op = Operator;
            using TL = LD::CT::TypeList<Variables...>;
            VarExpression(LD::Type<T>, OperationType operatorType,Operator && operatorSymbol,Variables && ... variables) noexcept:mTuple{LD::Forward<Variables>(variables)...},mOperatorSymbol{LD::Forward<Operator>(operatorSymbol)},mOrder{0},mOperator{operatorType}
            {
                auto iterableVariables = LD::MakeContext(LD::Forward<Variables>(variables)...);
                LD::UInteger max = 0;
                LD::For<sizeof...(Variables)>([](
                        auto I,
                        const auto & iterableVariables,
                        LD::UInteger & max)
                {
                    const auto & currentVariable = LD::Get(LD::Get<I>(iterableVariables));
                    max = LD::Max(max, currentVariable.Order());
                    return true;
                },iterableVariables,max);
                this->mOrder = (max+1);
            }
            VarExpression(LD::Type<T>, LD::UInteger order,OperationType,Operator && operatorSymbol,Variables && ... variables) noexcept:mTuple{LD::Forward<Variables>(variables)...},mOperatorSymbol{LD::Forward<Operator>(operatorSymbol)},mOrder{order}
            {

            }

            const OperationType & OperandType() const noexcept
            {
                return this->mOperator;
            }

            LD::UInteger Order() const noexcept
            {
                return this->mOrder;
            }
            const LD::Tuple<Variables...> & Composition() const noexcept
            {
                return this->mTuple;
            }

            //auto operator + (const LD::SPA::Var<T> & variable) const noexcept
            //{
                //return LD::SPA::VarExpression{LD::SPA::UnararyOperator{},LD::ImmutableString{"+"},*this,variable};
            //}

            auto operator + (const LD::SPA::Var<LD::Type<T>,T> & variable) const noexcept
            {
                return LD::SPA::VarExpression{LD::Type<T>{},LD::SPA::UnararyOperator{},LD::ImmutableString{"+"},*this,variable};
            }

            template<typename ... V,typename O1, typename OT1>
            auto operator + (const LD::SPA::VarExpression<T,LD::CT::TypeList<V...>,O1,OT1> & variable) const noexcept
            {
                return LD::SPA::VarExpression{LD::Type<T>{},LD::SPA::UnararyOperator{},LD::ImmutableString{"+"},*this,variable};
            }
            const Operator & Symbol() const noexcept
            {
                return this->mOperatorSymbol;
            }
        };

        template <typename T,typename ... Components, typename OperatorSymbol, typename OperationType> VarExpression(LD::Type<T>,OperationType,OperatorSymbol && symbol,Components && ... components) -> VarExpression<T,LD::CT::TypeList<Components...>,OperatorSymbol,OperationType>;
        //template <typename T,typename ... Components, typename OperatorSymbol, typename OperationType> VarExpression(LD::Type<T>,LD::UInteger ,OperationType,OperatorSymbol && symbol,Components && ... components) -> VarExpression<T,LD::CT::TypeList<Components...>,OperatorSymbol,OperationType>;
    }
}
namespace LD
{
    namespace CT
    {
        template<typename T, typename StringType>
        inline constexpr bool IsExternalObjectFunctionalSymbol(LD::Type<LD::SPA::ExternalObjectFunctionalSymbol<T,StringType>> ) noexcept
        {
            return true;
        }

        template<typename T, typename StringType>
        inline constexpr bool IsExternalObjectMemberSymbol(LD::Type<LD::SPA::ExternalObjectFunctionalMemberSymbol<T,StringType>> ) noexcept
        {
            return true;
        }
    }
}
namespace LD
{
    namespace SPA
    {
        template<typename T>
        struct DetermineNumberOfVariablesInExpression;



        template<typename T,typename Operator,typename OperationType,typename ... Vars>
        struct DetermineNumberOfVariablesInExpression<LD::SPA::VarExpression<T,LD::CT::TypeList<Vars...>,Operator,OperationType>>
        {
            template<typename U>
            using IsExpression = LD::Detail::IntegralConstant<bool,LD::SPA::IsVarExpression(LD::Type<U>{})>;

            template<typename U>
            using IsVariable = LD::Detail::IntegralConstant<bool,LD::SPA::IsVar(LD::Type<U>{})>;

            using SubExpressions =  LD::CT::Filter<LD::CT::TypeList<LD::Detail::Decay_T<Vars>...>,IsExpression>;

            using Variables =  LD::CT::Filter<LD::CT::TypeList<LD::Detail::Decay_T<Vars>...>,IsVariable>;


            using DeCoupledReflectiveList = typename DetermineNumberOfVariablesInExpression<SubExpressions>::type;
            using List = LD::CT::TypeList<Vars...>;

            using ConcatedLists = LD::CT::Concatenate<DeCoupledReflectiveList ,Variables>;
            using type = LD::CT::Flatten<ConcatedLists>;

        };

        template<typename ... SubExpressions>
        struct DetermineNumberOfVariablesInExpression<LD::CT::TypeList<SubExpressions...>>
        {
            using buffer = LD::CT::TypeList<typename DetermineNumberOfVariablesInExpression<SubExpressions>::type...>;
            using type =  buffer ;

        };
    }
}
namespace LD
{
    namespace SPA
    {
        namespace Detail
        {
            template<typename OperationType>
            constexpr LD::UInteger CalculateOperandLength(LD::Type<OperationType> operand, LD::UInteger numberOfArguments, LD::UInteger sizeOfOperand) noexcept
            {

                if constexpr (LD::CT::IsSameWithoutQualifiers(LD::Type<OperationType>{},LD::Type<LD::SPA::UnararyOperator>{}))
                {
                    return sizeOfOperand*(numberOfArguments/2);

                }else if constexpr(LD::CT::IsSameWithoutQualifiers(LD::Type<OperationType>{},LD::Type<LD::SPA::FunctionalOperator>{}))
                {
                    return sizeOfOperand + 2 + (numberOfArguments/2);
                }
                return 0;
            }
            template<typename OperationType>
            constexpr LD::UInteger CalculateOperationTypeLength(LD::Type<OperationType> operationType, LD::UInteger numberOfArguments) noexcept
            {
                if constexpr (LD::CT::IsExternalObjectFunctionalSymbol(LD::Type<OperationType>{}))
                {
                    using Name = typename OperationType::NameType;
                    constexpr LD::UInteger NameSize = Name::StringSize;
                    return NameSize + (numberOfArguments)/2 + 2;
                }
                return 0;
            }
        }
    }
    template<typename T>
    auto ToImmutableString(const LD::SPA::Var<LD::Type<T>,T> & variable) noexcept
    {
        LD::ImmutableString varName{"var"};
        return varName+LD::ToImmutableString(variable.Identification());
    }

    namespace SPA
    {
        namespace Detail
        {
            template<typename T, typename CharacterInserter>
            auto RecurseExpression(const LD::SPA::Var<LD::Type<T>,T> & variable,CharacterInserter characterInserter) noexcept
            {
                auto varToImmutableString = LD::ToImmutableString(variable);

                for(LD::UInteger n = 0;n<varToImmutableString.GetSize();++n)
                {
                    characterInserter = varToImmutableString[n];
                }
            }
            template<typename T,typename Operator, typename OperationType,typename ... Variables, typename CharacterInserter>
            auto RecurseExpression(const LD::SPA::VarExpression<T,LD::CT::TypeList<Variables...>,Operator,OperationType> & expression,CharacterInserter characterInserter) noexcept
            {

                const auto & composition = expression.Composition();
                const auto & symbol = expression.Symbol();
                const auto & operandType = expression.OperandType();
                if constexpr (LD::CT::IsSameWithoutQualifiers(LD::Type<OperationType>{},LD::Type<LD::SPA::UnararyOperator>{}))
                {
                    characterInserter = '(';
                    const auto & firstVariable = LD::Get(LD::Get<0>(composition));
                    LD::SPA::Detail::RecurseExpression(firstVariable,characterInserter);
                    for(LD::UInteger n=0;n<symbol.GetSize();++n)
                    {
                        characterInserter = symbol[n];
                    }

                    LD::For<1,sizeof...(Variables),1>([](
                            auto I,
                            CharacterInserter characterInserter,
                            const auto & composition)
                    {
                        const auto & currentVariable = LD::Get(LD::Get<I>(composition));
                        LD::SPA::Detail::RecurseExpression(currentVariable,characterInserter);
                        return true;
                    },characterInserter,composition);
                    characterInserter = ')';
                }else if constexpr (LD::CT::IsSameWithoutQualifiers(LD::Type<OperationType>{},LD::Type<LD::SPA::NoOpt>{}))
                {
                    //when a variable is expressed as an expression we get an expression with a "no op" or no operation.
                    const auto & firstVariable = LD::Get(LD::Get<0>(composition));
                    LD::SPA::Detail::RecurseExpression(firstVariable,characterInserter);
                }else if constexpr (LD::CT::IsSameWithoutQualifiers(LD::Type<OperationType>{},LD::Type<LD::SPA::FunctionalOperator>{}))
                {
                    for(LD::UInteger n=0;n<symbol.GetSize();++n)
                    {
                        characterInserter = symbol[n];
                    }
                    characterInserter = '(';
                    LD::For<0,sizeof...(Variables),1>([](
                            auto I,
                            CharacterInserter characterInserter,
                            const auto & composition)
                    {
                        const auto & currentVariable = LD::Get(LD::Get<I>(composition));
                        LD::SPA::Detail::RecurseExpression(currentVariable,characterInserter);
                        if constexpr (I < sizeof...(Variables)-1)
                        {
                            characterInserter = ',';
                        }
                        return true;
                    },characterInserter,composition);
                    characterInserter = ')';
                }else if constexpr (LD::CT::IsExternalObjectFunctionalSymbol(LD::Type<OperationType>{}))
                {
                    const auto & operandSymbol = operandType.Symbol();
                    for(LD::UInteger n=0;n<operandSymbol.GetSize();++n)
                    {
                        characterInserter = operandSymbol[n];
                    }
                    characterInserter = '.';
                    for(LD::UInteger n=0;n<symbol.GetSize();++n)
                    {
                        characterInserter = symbol[n];
                    }
                    characterInserter = '(';
                    LD::For<0,sizeof...(Variables),1>([](
                            auto I,
                            CharacterInserter characterInserter,
                            const auto & composition)
                    {
                        const auto & currentVariable = LD::Get(LD::Get<I>(composition));
                        LD::SPA::Detail::RecurseExpression(currentVariable,characterInserter);
                        if constexpr (I < sizeof...(Variables)-1)
                        {
                            characterInserter = ',';
                        }
                        return true;
                    },characterInserter,composition);
                    characterInserter = ')';
                }

            }
        }
    }
    template<typename T,typename Op,typename OpType,typename ... A>
    inline constexpr LD::UInteger GetRequriedSizeForExpression(LD::Type<LD::SPA::VarExpression<T,Op,OpType,A...>> ) noexcept
    {
        using ExpressionType = LD::SPA::VarExpression<T,Op,OpType,A...>;
        using OperationType = typename ExpressionType::OpType;
        using Operator = typename ExpressionType::Op;
        using Determiner = LD::SPA::DetermineNumberOfVariablesInExpression<ExpressionType>;
        using DeterminerVariables = typename Determiner ::type;

        constexpr LD::UInteger AmountDueToVar = 3*DeterminerVariables::size();
        constexpr LD::UInteger AmountDueToVariableNames = 22*DeterminerVariables::size();
        constexpr LD::UInteger AmountToOperator = LD::SPA::Detail::CalculateOperandLength(LD::Type<Operator>{},DeterminerVariables::size(),LD::ImmutableSize(LD::Type<Operator>{}));
        constexpr LD::UInteger AmountToOperatorType = LD::SPA::Detail::CalculateOperationTypeLength(LD::Type<OperationType>{},DeterminerVariables::size());
        constexpr LD::UInteger TotalAmount = AmountDueToVar+AmountDueToVariableNames+AmountToOperator+AmountToOperatorType+1;
        return TotalAmount;
    }
    template<typename ... A>
    constexpr LD::UInteger GetRequiredSizeForExpressions(LD::CT::TypeList<A...>) noexcept
    {
        //LD::Type<LD::SPA::Function<LD::CT::TypeList<A...>>>
        LD::UInteger requiredAmount = 0;
        constexpr LD::UInteger NumberOfExpressions = sizeof...(A);
        LD::For<NumberOfExpressions>([](
                auto I,
                LD::UInteger & amount) noexcept
        {
            using ExpressionType = LD::Detail::Decay_T<LD::CT::TypeAtIndex<I,LD::CT::TypeList<A...>>>;
            //LD::CT::DebugTemplate<ExpressionType>{};
            using OperationType = typename ExpressionType::OpType;
            using Operator = typename ExpressionType::Op;
            using Determiner = LD::SPA::DetermineNumberOfVariablesInExpression<ExpressionType>;
            using DeterminerVariables = typename Determiner ::type;
            //todo determine if current type if of type VAr and look at how much it's assignment will cost as well.
            constexpr LD::UInteger AmountDueToVar = 3*DeterminerVariables::size();
            constexpr LD::UInteger AmountDueToVariableNames = 22*DeterminerVariables::size();
            constexpr LD::UInteger AmountToOperator = LD::SPA::Detail::CalculateOperandLength(LD::Type<OperationType>{},DeterminerVariables::size(),LD::ImmutableSize(LD::Type<Operator>{}));
            constexpr LD::UInteger TotalAmount = AmountDueToVar+AmountDueToVariableNames+AmountToOperator+1;
            amount += TotalAmount;
            return true;
        },requiredAmount);
        return requiredAmount;
    }
    template<typename T,typename Operator,typename OperationType,typename ... A>
    constexpr LD::UInteger GetNumberOfVariablesInExpression(LD::Type<LD::SPA::VarExpression<T,Operator,OperationType,A...>> ) noexcept
    {
        using TL = typename LD::SPA::VarExpression<T,Operator,OperationType,A...>::TL;
        constexpr LD::UInteger NumberOfVariables = TL::size();
        LD::UInteger size = 0;
        LD::For<NumberOfVariables>([](auto I,LD::UInteger & amount) noexcept
        {
            using CurrentVariable = LD::Detail::Decay_T<LD::CT::TypeAtIndex<I,TL>>;
            constexpr auto currentVariableType = LD::CT::RemoveQualifiers(LD::Type<CurrentVariable>{});


            if constexpr (LD::SPA::IsVarExpression(currentVariableType))
            {

                amount += GetRequiredSizeForVariablesInsideExpression(currentVariableType);

            }else if constexpr (LD::SPA::IsVar(currentVariableType))
            {
                amount += 1;
            }
            return true;
        },size);
        return 0;
    }
    template<typename T,typename Operator,typename OperationType,typename ... A>
    constexpr LD::UInteger GetRequiredSizeForVariablesInsideExpression(LD::Type<LD::SPA::VarExpression<T,Operator,OperationType,A...>> ) noexcept
    {
        using TL = typename LD::SPA::VarExpression<T,Operator,OperationType,A...>::TL;
        constexpr LD::UInteger NumberOfVariables = TL::size();

        //LD::CT::DebugTemplate<TL>{};
        LD::UInteger size = 0;
        LD::For<NumberOfVariables>([](auto I,LD::UInteger & amount) noexcept
        {
            using CurrentVariable = LD::Detail::Decay_T<LD::CT::TypeAtIndex<I,TL>>;
            constexpr auto currentVariableType = LD::CT::RemoveQualifiers(LD::Type<CurrentVariable>{});

            if constexpr (LD::SPA::IsVarExpression(currentVariableType))
            {
                amount += GetRequiredSizeForVariablesInsideExpression(currentVariableType);

            }else if constexpr (LD::SPA::IsVar(currentVariableType))
            {
                using VariableType = typename CurrentVariable::VarType;
                using UnderlyingType = LD::GetType<VariableType>;
                using ImmutableStringType = decltype(LD::ToImmutableString(LD::Declval<UnderlyingType>()));
                //LD::CT::DebugTemplate<decltype(currentVariableType)>{};
                //LD::CT::DebugTemplate<ImmutableStringType>{};
                amount += (ImmutableStringType::StringSize + 3)+5;
                if constexpr (LD::CT::IsString(LD::CT::RemoveQualifiers(VariableType{})))
                {
                    amount += 2;
                }
            }
            return true;
        },size);
        return size;
    }
    template<typename T,typename Operator, typename OperationType,typename ... Variables>
    auto ToImmutableString(const LD::SPA::VarExpression<T,LD::CT::TypeList<Variables...>,Operator,OperationType> & expression) noexcept
    {
        using Determiner = LD::SPA::DetermineNumberOfVariablesInExpression<LD::SPA::VarExpression<T,LD::CT::TypeList<Variables...>,Operator,OperationType>>;
        using DeterminerVariables = typename Determiner ::type;
        constexpr LD::UInteger AmountDueToVar = 3*DeterminerVariables::size();
        constexpr LD::UInteger AmountDueToVariableNames = 22*DeterminerVariables::size();
        constexpr LD::UInteger AmountToOperator = LD::SPA::Detail::CalculateOperandLength(LD::Type<OperationType>{},DeterminerVariables::size(),LD::ImmutableSize(LD::Type<Operator>{}));
        constexpr LD::UInteger TotalAmount = AmountDueToVar+AmountDueToVariableNames+AmountToOperator+1;
        LD::StaticArray<char,TotalAmount> buffer;
        LD::BackInserter<LD::StaticArray<char,TotalAmount>> characterInserter{buffer};
        const auto & composition = expression.Composition();
        const auto & symbol = expression.Symbol();
        LD::SPA::Detail::RecurseExpression(expression,characterInserter);
        characterInserter = '\0';
        return LD::ImmutableString{buffer};
    }
}

namespace LD
{
    namespace SPA
    {
        template<typename T,class = void>
        struct GetExpressionComposition;
        template<typename T>
        struct GetExpressionComposition<T,LD::Enable_If_T<LD::Require<LD::SPA::IsVarExpression(LD::CT::RemoveQualifiers(LD::Type<T>{}))>>>
        {
            using List = typename T::TL;
            template<typename V>
            using IsAMember = LD::Detail::IntegralConstant<bool,LD::SPA::IsVar(LD::CT::RemoveQualifiers(LD::Type<V>{}))>;

            template<typename V>
            using IsAnExpression = LD::Detail::IntegralConstant<bool,LD::SPA::IsVarExpression(LD::CT::RemoveQualifiers(LD::Type<V>{}))>;

            using Members  = LD::CT::Filter<List,IsAMember>;
            using Expressions = LD::CT::Filter<List,IsAnExpression>;

            using DeCoupledReflectiveList = typename GetExpressionComposition<Expressions>::type;
            using DeCoupledReflectiveDuplicatedList = typename GetExpressionComposition<Expressions>::duplicatedList;
            using ConcatedLists = LD::CT::Concatenate<Members,DeCoupledReflectiveList>;
            using ConcatedDuplicatedLists = LD::CT::Concatenate<Members,DeCoupledReflectiveDuplicatedList>;
            using type = LD::CT::DeDuplicateTypeList<ConcatedLists>;
            using duplicatedList = ConcatedDuplicatedLists;
        };
        template<typename ...A>
        struct GetExpressionComposition<LD::CT::TypeList<A...>,LD::Enable_If_T<
                LD::Require<(LD::SPA::IsVarExpression(LD::CT::RemoveQualifiers(LD::Type<A>{})) && ...)
                >>>
        {
            using buffer = LD::CT::Concatenate<LD::CT::TypeList<>,LD::CT::TypeList<typename GetExpressionComposition<LD::Detail::Decay_T<A>>::type...>>;
            using buffer1 = LD::CT::Concatenate<LD::CT::TypeList<>,LD::CT::TypeList<typename GetExpressionComposition<LD::Detail::Decay_T<A>>::duplicatedList...>>;
            using type =  LD::CT::Flatten<buffer> ;
            using duplicatedList = LD::CT::Flatten<buffer1>;
        };
    }
}
namespace LD
{
    namespace SPA
    {
        template<typename ... A>
        struct CompositionsOfExpressions
        {
            using type = LD::CT::DeDuplicateTypeList<LD::CT::Flatten<LD::CT::TypeList<LD::CT::Concatenate<LD::CT::TypeList<>,LD::CT::TypeList<typename LD::SPA::GetExpressionComposition<A>::type>>...>>>;
            using alltypes = LD::CT::Flatten<LD::CT::TypeList<LD::CT::Concatenate<LD::CT::TypeList<>,LD::CT::TypeList<typename LD::SPA::GetExpressionComposition<A>::duplicatedList>>...>>;
        };
    }
}
#endif //LANDESSDEVCORE_VARIABLE_HPP
