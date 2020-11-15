//
// Created by phoenixflower on 11/5/20.
//

#ifndef LANDESSDEVCORE_VARIABLE_HPP
#define LANDESSDEVCORE_VARIABLE_HPP
#include "Definitions/Integer.hpp"
#include "Primitives/General/Immutable.hpp"
#include "TypeTraits/Detection.hpp"
#include "Compositor.hpp"
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

            template<typename U, typename ... B>
            Var(ValueType && value,U && randomNumberGenerator, B && ... arguments) noexcept:mValue{LD::Forward<ValueType>(value)},mOrder{0}
            {
                this->mID = randomNumberGenerator(LD::Forward<B>(arguments)...);
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

            const T & DefaultValue() const noexcept
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

        template<typename T,typename Operator,typename OperationType,typename U, typename ... B,typename ... Variables> Var(const LD::SPA::VarExpression<T,LD::CT::TypeList<Variables...>,Operator,OperationType> &, U &&, B && ...) -> Var<LD::Type<T>,const LD::SPA::VarExpression<T,LD::CT::TypeList<Variables...>,Operator,OperationType> &>;



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
            Operator mOperatorSymbol;
            LD::UInteger mOrder;
        public:

            using TL = LD::CT::TypeList<Variables...>;
            VarExpression(LD::Type<T>, OperationType,Operator && operatorSymbol,Variables && ... variables) noexcept:mTuple{LD::Forward<Variables>(variables)...},mOperatorSymbol{LD::Forward<Operator>(operatorSymbol)},mOrder{0}
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
                this->mOrder = max;
            }
            VarExpression(LD::Type<T>, LD::UInteger order,OperationType,Operator && operatorSymbol,Variables && ... variables) noexcept:mTuple{LD::Forward<Variables>(variables)...},mOperatorSymbol{LD::Forward<Operator>(operatorSymbol)},mOrder{order}
            {

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

                }

            }
        }
    }

    template<typename T,typename Operator, typename OperationType,typename ... Variables>
    auto ToImmutableString(const LD::SPA::VarExpression<T,LD::CT::TypeList<Variables...>,Operator,OperationType> & expression) noexcept
    {
        using Determiner = LD::SPA::DetermineNumberOfVariablesInExpression<LD::SPA::VarExpression<T,LD::CT::TypeList<Variables...>,Operator,OperationType>>;
        using DeterminerVariables = typename Determiner ::type;

        constexpr LD::UInteger AmountDueToVar = 3*DeterminerVariables::size();
        constexpr LD::UInteger AmountDueToVariableNames = 22*DeterminerVariables::size();
        constexpr LD::UInteger AmountToOperator = LD::SPA::Detail::CalculateOperandLength(LD::Type<OperationType>{},DeterminerVariables::size(),LD::ImmutableSize(LD::Type<Operator>{}));
        constexpr LD::UInteger TotalAmount = AmountDueToVar+AmountDueToVariableNames+AmountToOperator*50+1;
        LD::StaticArray<char,TotalAmount> buffer;
        LD::BackInserter<LD::StaticArray<char,TotalAmount>> characterInserter{buffer};
        const auto & composition = expression.Composition();
        const auto & symbol = expression.Symbol();
        LD::SPA::Detail::RecurseExpression(expression,characterInserter);
        characterInserter = '\0';
        return LD::ImmutableString{buffer};
    }
}
#endif //LANDESSDEVCORE_VARIABLE_HPP
