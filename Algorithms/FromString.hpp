//
// Created by phoenixflower on 11/21/20.
//

#ifndef LANDESSDEVCORE_FROMSTRING_HPP
#define LANDESSDEVCORE_FROMSTRING_HPP
#include "IO/FetchRequest.h"
#include "Memory/Optional.h"
#include "Primitives/General/StringView.hpp"
#include "Primitives/General/ctre.hpp"
#include "Exponential.hpp"
#include "StringAsNumber.h"
#include "Core/RequestResponse.hpp"
namespace LD
{
    template<typename T, typename ... A>
    LD::Enable_If_T<
            LD::Require<
                    LD::IsInteger<T>
    >,
    LD::QueryResult<T(A...)>> StringToNumber(LD::Type<T>,LD::StringView view, A && ... args) noexcept
    {
        if (view.size() > 0)
        {
            bool isNegative = (view[0] == '-');
            LD::UInteger index = view.size()-isNegative;
            T intermediateSum = 0;
            for (LD::UInteger i = isNegative; i < view.size(); ++i,--index)
            {
                short number = view[i]-'0';
                intermediateSum += LD::Pow(10,index-1)*number;
            }
            T result = (isNegative)*(-1*intermediateSum) + (!isNegative)*intermediateSum;
            return LD::MakeContext(LD::TransactionResult{},T(result),LD::Forward<A>(args)...);
        }
        return LD::MakeContext(LD::TransactionError{},LD::Forward<A>(args)...);
    }
    template<typename T, typename ... A>
    LD::Enable_If_T<
            LD::Require<
                    LD::IsInteger<T>
            >,
            LD::RequestResponse<T(A...)>> StringToNumberCore(LD::Type<T>,LD::StringView view, A && ... args) noexcept
    {
        if (view.size() > 0)
        {
            bool isNegative = (view[0] == '-');
            LD::UInteger index = view.size()-isNegative;
            T intermediateSum = 0;
            for (LD::UInteger i = isNegative; i < view.size(); ++i,--index)
            {
                short number = view[i]-'0';
                intermediateSum += LD::Pow(10,index-1)*number;
            }
            T result = (isNegative)*(-1*intermediateSum) + (!isNegative)*intermediateSum;
            return LD::CreateResponse(LD::Type<T>{},T(result),LD::Forward<A>(args)...);
        }
        return LD::CreateResponse(LD::Type<T>{},LD::TransactionError{},LD::Forward<A>(args)...);
    }
    template<typename T, typename ... A>
    LD::Enable_If_T<
            LD::Require<
                    LD::IsUnsignedInteger<T>
            >,
            LD::QueryResult<T(A...)>> StringToNumber(LD::Type<T>,LD::StringView view, A && ... args) noexcept
    {
        if (view.size() > 0)
        {
            LD::UInteger index = view.size();
            T intermediateSum = 0;
            for (LD::UInteger i = 0; i < view.size(); ++i,--index)
            {
                short number = view[i]-'0';
                intermediateSum += LD::Pow(10,index-1)*number;
            }
            return LD::MakeContext(LD::TransactionResult{},T(intermediateSum),LD::Forward<A>(args)...);
        }
        return LD::MakeContext(LD::TransactionError{},LD::Forward<A>(args)...);
    }
    template<typename T, typename ... A>
    LD::Enable_If_T<
            LD::Require<
                    LD::IsUnsignedInteger<T>
            >,
            LD::RequestResponse<T(A...)>> StringToNumberCore(LD::Type<T>,LD::StringView view, A && ... args) noexcept
    {
        if (view.size() > 0)
        {
            LD::UInteger index = view.size();
            T intermediateSum = 0;
            for (LD::UInteger i = 0; i < view.size(); ++i,--index)
            {
                short number = view[i]-'0';
                intermediateSum += LD::Pow(10,index-1)*number;
            }
            return LD::CreateResponse(LD::Type<T>{},T(intermediateSum),LD::Forward<A>(args)...);
        }
        return LD::CreateResponse(LD::Type<T>{},LD::TransactionError{},LD::Forward<A>(args)...);
    }
    namespace Detail
    {
        //[+-]?[0-9][0-9]*
        static constexpr auto  SignedIntegerRegularExpression = ctll::basic_fixed_string{"[\\+\\-]?([0-9]+)"};
        static constexpr auto  BooleanRegularExpressionTrue0 = ctll::basic_fixed_string{"([t,T])rue"};
        static constexpr auto  BooleanRegularExpressionFalse0 = ctll::basic_fixed_string{"([f,F])alse"};
        static constexpr auto  BooleanRegularExpressionYes0 = ctll::basic_fixed_string{"([y,Y])es"};
        static constexpr auto  BooleanRegularExpressionNo0 = ctll::basic_fixed_string{"([n,N])o"};
        static constexpr auto  UnsignedIntegerRegularExpression = ctll::basic_fixed_string{"[\\+]?([0-9]+)"};
        static constexpr auto  D = ctll::basic_fixed_string{"(\\d{4})-(\\d{1,2})-(\\d{1,2})"};
    }
    template<typename ... A>
    LD::QueryResult<bool(A...)> FromStringView(LD::Type<bool>, LD::StringView view,A && ... args) noexcept
    {
        if (auto m = ctre::match<LD::Detail::BooleanRegularExpressionTrue0>(view))
        {
            return LD::MakeContext(LD::TransactionResult{},true,LD::Forward<A>(args)...);
        }else if(auto m = ctre::match<LD::Detail::BooleanRegularExpressionFalse0>(view))
        {
            return LD::MakeContext(LD::TransactionResult{},false,LD::Forward<A>(args)...);
        }else if(auto m = ctre::match<LD::Detail::BooleanRegularExpressionYes0>(view))
        {
            return LD::MakeContext(LD::TransactionResult{},true,LD::Forward<A>(args)...);
        }
        else if(auto m = ctre::match<LD::Detail::BooleanRegularExpressionNo0>(view))
        {
            return LD::MakeContext(LD::TransactionResult{},false,LD::Forward<A>(args)...);
        }
        return LD::MakeContext(LD::TransactionError{},LD::Forward<A>(args)...);
    }
    template<typename ... A>
    LD::RequestResponse<bool(A...)> FromString(LD::Type<bool>, LD::StringView view, A && ... args) noexcept
    {
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
        return LD::CreateResponse(LD::Type<bool>{},LD::TransactionError{},LD::Forward<A>(args)...);
    }
    template<typename ... A>
    LD::QueryResult<char(A...)> FromStringView(LD::Type<char>, LD::StringView view,A && ... args) noexcept
    {
        if (view.size() > 0)
        {
            return LD::MakeContext(LD::TransactionResult{},char(view[0]),LD::Forward<A>(args)...);
        }
        return LD::MakeContext(LD::TransactionError{},LD::Forward<A>(args)...);
    }
    template<typename ... A>
    LD::RequestResponse<char(A...)> FromString(LD::Type<char>, LD::StringView view, A && ... args) noexcept
    {
        if (view.size() > 0)
        {
            return LD::CreateResponse(LD::Type<char>{},char(view[0]),LD::Forward<A>(args)...);
        }
        return LD::CreateResponse(LD::Type<char>{},LD::TransactionError{},LD::Forward<A>(args)...);
    }
    template<typename ... A>
    LD::QueryResult<short(A...)> FromStringView(LD::Type<short>, LD::StringView view,A && ... args) noexcept
    {
        if (auto m = ctre::match<LD::Detail::SignedIntegerRegularExpression>(view))
        {
            LD::Integer result = atoi(m.view().data());
            if (result > LD::Limit<short>::GetMax() || result < LD::Limit<short>::GetMin())
            {
                return LD::MakeContext(LD::TransactionError{},LD::Forward<A>(args)...);
            }
            return LD::MakeContext(LD::TransactionResult{},short(result),LD::Forward<A>(args)...);
        }
        return LD::MakeContext(LD::TransactionError{},LD::Forward<A>(args)...);
    }
    template<typename ... A>
    LD::RequestResponse<short(A...)> Fromstring(LD::Type<short>, LD::StringView view, A && ... args) noexcept
    {
        if (auto m = ctre::match<LD::Detail::SignedIntegerRegularExpression>(view))
        {
            LD::Integer result = atoi(m.view().data());
            if (result > LD::Limit<short>::GetMax() || result < LD::Limit<short>::GetMin())
            {
                return LD::CreateResponse(LD::Type<short>{},LD::TransactionError{},LD::Forward<A>(args)...);
            }
            return LD::CreateResponse(LD::Type<short>{},short(result),LD::Forward<A>(args)...);
        }
        return LD::CreateResponse(LD::Type<short>{},LD::TransactionError{},LD::Forward<A>(args)...);
    }
    template<typename ... A>
    LD::QueryResult<int(A...)> FromStringView(LD::Type<int>, LD::StringView view,A && ... args) noexcept
    {
        if (auto m = ctre::match<LD::Detail::SignedIntegerRegularExpression>(view))
        {
            return LD::StringToNumber(LD::Type<int>{},view,LD::Forward<A>(args)...);
        }
        return LD::MakeContext(LD::TransactionError{},LD::Forward<A>(args)...);
    }

    template<typename ... A>
    LD::RequestResponse<int(A...)> FromString(LD::Type<int>, LD::StringView view,A && ... args) noexcept
    {
        if (auto m = ctre::match<LD::Detail::SignedIntegerRegularExpression>(view))
        {
            return LD::StringToNumberCore(LD::Type<int>{},view,LD::Forward<A>(args)...);
        }
        return LD::CreateResponse(LD::Type<int>{},LD::TransactionError{},LD::Forward<A>(args)...);
    }


    template<typename ... A>
    LD::QueryResult<long(A...)> FromStringView(LD::Type<long>, LD::StringView view,A && ... args) noexcept
    {
        if (auto m = ctre::match<LD::Detail::SignedIntegerRegularExpression>(view))
        {
            return LD::StringToNumber(LD::Type<long>{},view,LD::Forward<A>(args)...);
        }
        return LD::MakeContext(LD::TransactionError{},LD::Forward<A>(args)...);
    }

    template<typename ... A>
    LD::RequestResponse<long(A...)> FromString(LD::Type<long>, LD::StringView view,A && ... args) noexcept
    {
        if (auto m = ctre::match<LD::Detail::SignedIntegerRegularExpression>(view))
        {
            return LD::StringToNumberCore(LD::Type<long>{},view,LD::Forward<A>(args)...);
        }
        return LD::CreateResponse(LD::Type<long>{},LD::TransactionError{},LD::Forward<A>(args)...);
    }

    template<typename ... A>
    LD::QueryResult<unsigned short(A...)> FromStringView(LD::Type<unsigned short>, LD::StringView view,A && ... args) noexcept
    {
        using Ret = unsigned short;
        if (auto m = ctre::match<LD::Detail::UnsignedIntegerRegularExpression>(view))
        {
            return LD::StringToNumber(LD::Type<unsigned short>{},view,LD::Forward<A>(args)...);
        }
        return LD::MakeContext(LD::TransactionError{},LD::Forward<A>(args)...);
    }

    template<typename ... A>
    LD::RequestResponse<unsigned short(A...)> FromString(LD::Type<unsigned short>, LD::StringView view,A && ... args) noexcept
    {
        if (auto m = ctre::match<LD::Detail::SignedIntegerRegularExpression>(view))
        {
            return LD::StringToNumberCore(LD::Type<unsigned short>{},view,LD::Forward<A>(args)...);
        }
        return LD::CreateResponse(LD::Type<unsigned short>{},LD::TransactionError{},LD::Forward<A>(args)...);
    }
    template<typename ... A>
    LD::QueryResult<unsigned int(A...)> FromStringView(LD::Type<unsigned int>, LD::StringView view,A && ... args) noexcept
    {
        using Ret = unsigned int;
        if (auto m = ctre::match<LD::Detail::UnsignedIntegerRegularExpression>(view))
        {
            return LD::StringToNumber(LD::Type<unsigned int>{},view,LD::Forward<A>(args)...);
        }
        return LD::MakeContext(LD::TransactionError{},LD::Forward<A>(args)...);
    }

    template<typename ... A>
    LD::RequestResponse<unsigned int(A...)> FromString(LD::Type<unsigned int>, LD::StringView view,A && ... args) noexcept
    {
        if (auto m = ctre::match<LD::Detail::SignedIntegerRegularExpression>(view))
        {
            return LD::StringToNumberCore(LD::Type<unsigned int>{},view,LD::Forward<A>(args)...);
        }
        return LD::CreateResponse(LD::Type<unsigned int>{},LD::TransactionError{},LD::Forward<A>(args)...);
    }
    template<typename ... A>
    LD::QueryResult<unsigned long(A...)> FromStringView(LD::Type<unsigned long>, LD::StringView view,A && ... args) noexcept
    {
        using Ret = unsigned long;
        if (auto m = ctre::match<LD::Detail::UnsignedIntegerRegularExpression>(view))
        {
            return LD::StringToNumber(LD::Type<unsigned long>{},view,LD::Forward<A>(args)...);
        }
        return LD::MakeContext(LD::TransactionError{},LD::Forward<A>(args)...);
    }
    template<typename ... A>
    LD::RequestResponse<unsigned long(A...)> FromString(LD::Type<unsigned long>, LD::StringView view,A && ... args) noexcept
    {
        if (auto m = ctre::match<LD::Detail::SignedIntegerRegularExpression>(view))
        {
            return LD::StringToNumberCore(LD::Type<unsigned long>{},view,LD::Forward<A>(args)...);
        }
        return LD::CreateResponse(LD::Type<unsigned long>{},LD::TransactionError{},LD::Forward<A>(args)...);
    }
    template<typename ... A>
    LD::RequestResponse<unsigned long long int(A...)> FromString(LD::Type<unsigned long long int>, LD::StringView view,A && ... args) noexcept
    {
        if (auto m = ctre::match<LD::Detail::SignedIntegerRegularExpression>(view))
        {
            return LD::StringToNumberCore(LD::Type<unsigned long long int>{},view,LD::Forward<A>(args)...);
        }
        return LD::CreateResponse(LD::Type<unsigned long long int>{},LD::TransactionError{},LD::Forward<A>(args)...);
    }
    template<typename ... A>
    LD::RequestResponse<double(A...)> FromString(LD::Type<double>, LD::StringView view,A && ... args) noexcept
    {
        if (LD::Detail::IsSignedIntegerNumber(view))
        {
            const LD::UInteger stringSize = view.size();
            if (view.data() == nullptr || stringSize == 0)
            {
                return LD::CreateResponse(LD::Type<double>{},LD::TransactionError{},LD::Forward<A>(args)...);
            }
            const bool isNegative = (view[0] == '-');
            double sign = 1*(!isNegative) + -1*(isNegative);
            double sum = {0};
            //sum = view[0]-'0';
            for (LD::UInteger i = isNegative; i < view.size(); ++i)
            {
                sum += LD::Power(10,view.size()-i-1)*(view[i]-'0');
            }
            return LD::CreateResponse(LD::Type<double>{},double{sum*sign},LD::Forward<A>(args)...);
        }else if(LD::Detail::IsFloatingPointNumber(view))
        {
            auto match = ctre::range<LD::Detail::NotCommaPattern>(view);
            auto it = match.begin();
            LD::StringView firstSegment = (*it).get<0>();
            ++it;
            LD::StringView secondSegment = (*it).get<0>();
            double sum = {0};
            const bool isNegative = (firstSegment[0] == '-');
            double sign = 1*(!isNegative) + -1*(isNegative);
            for (LD::UInteger i = isNegative; i < firstSegment.size(); ++i)
            {
                sum += LD::Power(10,firstSegment.size()-i-1)*(firstSegment[i]-'0');
            }
            double secondarySum = {0};
            for (LD::UInteger i = 0; i < secondSegment.size(); ++i)
            {
                secondarySum += LD::Power(10,secondSegment.size()-i-1)*(secondSegment[i]-'0');
            }
            sum+=(secondarySum/LD::Power(10,secondSegment.size()));
            return LD::CreateResponse(LD::Type<double>{},double{sum*sign},LD::Forward<A>(args)...);
        }
        return LD::CreateResponse(LD::Type<double>{},LD::TransactionError{},LD::Forward<A>(args)...);
    }
    template<typename T,typename ... A>
    LD::Enable_If_T<LD::Require<LD::IsFloatingPoint<T>>,LD::QueryResult<double(A...)>> FromStringView(LD::Type<T>, LD::StringView view,A && ... args) noexcept
    {
        if (LD::Detail::IsSignedIntegerNumber(view))
        {
            const LD::UInteger stringSize = view.size();
            if (view.data() == nullptr || stringSize == 0)
            {
                return LD::MakeContext(LD::TransactionError{},LD::Forward<A>(args)...);
            }
            const bool isNegative = (view[0] == '-');
            T sign = 1*(!isNegative) + -1*(isNegative);
            T sum = {0};
            //sum = view[0]-'0';
            for (LD::UInteger i = isNegative; i < view.size(); ++i)
            {
                sum += LD::Power(10,view.size()-i-1)*(view[i]-'0');
            }
            return LD::MakeContext(LD::TransactionResult{},T{sum*sign},LD::Forward<A>(args)...);
        }else if(LD::Detail::IsFloatingPointNumber(view))
        {
            auto match = ctre::range<LD::Detail::NotCommaPattern>(view);
            auto it = match.begin();
            LD::StringView firstSegment = (*it).get<0>();
            ++it;
            LD::StringView secondSegment = (*it).get<0>();
            T sum = {0};
            const bool isNegative = (firstSegment[0] == '-');
            T sign = 1*(!isNegative) + -1*(isNegative);
            for (LD::UInteger i = isNegative; i < firstSegment.size(); ++i)
            {
                sum += LD::Power(10,firstSegment.size()-i-1)*(firstSegment[i]-'0');
            }
            T secondarySum = {0};
            for (LD::UInteger i = 0; i < secondSegment.size(); ++i)
            {
                secondarySum += LD::Power(10,secondSegment.size()-i-1)*(secondSegment[i]-'0');
            }
            sum+=(secondarySum/LD::Power(10,secondSegment.size()));
            return LD::MakeContext(LD::TransactionResult{},T{sum*sign},LD::Forward<A>(args)...);
        }
        return LD::MakeContext(LD::TransactionError{},LD::Forward<A>(args)...);
    }

    template<typename T>
    LD::Optional<short> OptionallyFromStringView(LD::Type<short>, LD::StringView view) noexcept
    {
        return LD::Optional<short>{};
    }


}
#endif //LANDESSDEVCORE_FROMSTRING_HPP
