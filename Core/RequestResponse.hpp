//
// Created by phoenixflower on 2/11/21.
//

#ifndef LANDESSDEVCORE_REQUESTRESPONSE_HPP
#define LANDESSDEVCORE_REQUESTRESPONSE_HPP
//#include "Definitions/Common.hpp"
#include "SumTypes/MPark/variant.hpp"
#include "IO/DatabaseOperationResult.h"
#include "Algorithms/Invoke.hpp"
#include "TypeTraits/TypeList.hpp"
namespace LD
{

    template<typename T>
    class RequestResponse;

    template<typename T, typename ... A>
    class RequestResponse<T(A...)>: public LD::Tuple<mpark::variant<LD::TransactionError,T>,mpark::variant<LD::Detail::Conditional_T<(LD::Detail::IsLValueReference<A>::value),LD::ElementReference <LD::Conditonal<LD::Detail::IsConst<A>::value,typename LD::Detail::AddConst<LD::Detail::Decay_T<A>>::type,LD::Detail::Decay_T<A>>>,LD::Detail::Decay_T<A>>>...>
    {
    private:

        using BaseClass = LD::Tuple<mpark::variant<LD::TransactionError,T>,mpark::variant<LD::Detail::Conditional_T<(LD::Detail::IsLValueReference<A>::value),LD::ElementReference <LD::Conditonal<LD::Detail::IsConst<A>::value,typename LD::Detail::AddConst<LD::Detail::Decay_T<A>>::type,LD::Detail::Decay_T<A>>>,LD::Detail::Decay_T<A>>>...>;
    public:

        RequestResponse() noexcept{}


        RequestResponse(const BaseClass & context) noexcept
        {
            (*this) = context;
        }

        RequestResponse& operator = (const BaseClass & context) noexcept
        {
            BaseClass::operator=(context);
            return (*this);
        }
    };


    template<typename ... T, typename ... A>

    class RequestResponse<LD::CT::TypeList<T...>(A...)>: public LD::Tuple<mpark::variant<LD::TransactionError,T...>,mpark::variant<LD::Detail::Conditional_T<(LD::Detail::IsLValueReference<A>::value),LD::ElementReference <LD::Conditonal<LD::Detail::IsConst<A>::value,typename LD::Detail::AddConst<LD::Detail::Decay_T<A>>::type,LD::Detail::Decay_T<A>>>,LD::Detail::Decay_T<A>>>...>
    {

    private:
        using BaseClass = LD::Tuple<mpark::variant<LD::TransactionError,T...>,mpark::variant<LD::Detail::Conditional_T<(LD::Detail::IsLValueReference<A>::value),LD::ElementReference <LD::Conditonal<LD::Detail::IsConst<A>::value,typename LD::Detail::AddConst<LD::Detail::Decay_T<A>>::type,LD::Detail::Decay_T<A>>>,LD::Detail::Decay_T<A>>>...>;
    public:

        RequestResponse() noexcept{}

        RequestResponse(const BaseClass & context) noexcept
        {
            (*this) = context;
        }
        RequestResponse& operator = (const BaseClass & context) noexcept
        {
            BaseClass::operator=(context);
            return (*this);
        }

    };

    template<typename T, typename H,typename ... A,
        typename R = LD::Tuple<mpark::variant<LD::TransactionError,T>,mpark::variant<LD::Detail::Conditional_T<(LD::Detail::IsLValueReference<A>::value),LD::ElementReference <LD::Conditonal<LD::Detail::IsConst<A>::value,typename LD::Detail::AddConst<LD::Detail::Decay_T<A>>::type,LD::Detail::Decay_T<A>>>,LD::Detail::Decay_T<A>>>...>>
    inline RequestResponse<T(A...)> CreateResponse(LD::Type<T>,H && a ,A && ... args) noexcept
    {
        return R{LD::Forward<H>(a),LD::Forward<A>(args)...};
    }

    template<typename ... T, typename H,typename ... A,
        typename R = LD::Tuple<mpark::variant<LD::TransactionError,T...>,mpark::variant<LD::Detail::Conditional_T<(LD::Detail::IsLValueReference<A>::value),LD::ElementReference <LD::Conditonal<LD::Detail::IsConst<A>::value,typename LD::Detail::AddConst<LD::Detail::Decay_T<A>>::type,LD::Detail::Decay_T<A>>>,LD::Detail::Decay_T<A>>>...>>
    inline RequestResponse<LD::CT::TypeList<T...>(A...)> CreateResponse(LD::CT::TypeList<T...>, H && a,A && ... args) noexcept
    {
        return R{LD::Forward<H>(a),LD::Forward<A>(args)...};
    }



    template<typename T, typename ... Args>
    constexpr bool IsTransactionalResponse(const LD::RequestResponse<T(Args...)> & queryResult) noexcept
    {
        auto transactionLambda = [](auto && ... stuff) noexcept
        {
            auto PickOffFirst = [](auto && first, auto && ... stuffings) noexcept -> bool
            {
                using FirstType = LD::Detail::Decay_T <decltype(first)>;
                return not LD::CT::IsSameWithoutQualifiers(LD::Type<FirstType>{},LD::Type<LD::TransactionError>{});
            };


            return  PickOffFirst(LD::Forward<decltype(stuff)>(stuff)...);
        };

        return LD::InvokeVisitation(LD::Overload{transactionLambda},queryResult);
    }

    template<typename T, typename ... Args>
    constexpr bool IsErrorResponse(const LD::RequestResponse<T(Args...)> & queryResult) noexcept
    {
        auto transactionLambda = [](auto && ... stuff) noexcept
        {
            auto PickOffFirst = [](auto && first, auto && ... stuffings) noexcept -> bool
            {
                using FirstType = LD::Detail::Decay_T <decltype(first)>;
                //std::cout << typeid(FirstType{}).name() << std::endl;
                return  LD::CT::IsSameWithoutQualifiers(LD::Type<FirstType>{},LD::Type<LD::TransactionError>{});
            };


            return  PickOffFirst(LD::Forward<decltype(stuff)>(stuff)...);
        };

        return LD::InvokeVisitation(LD::Overload{transactionLambda},queryResult);
    }


}
#endif //LANDESSDEVCORE_REQUESTRESPONSE_HPP
