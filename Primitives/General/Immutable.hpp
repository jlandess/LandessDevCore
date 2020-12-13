//
//  Immutable.h
//  DataStructures4
//
//  Created by James Landess on 1/3/19.
//  Copyright © 2019 James Landess. All rights reserved.
//

#ifndef Immutable_h
#define Immutable_h

#include <Definitions/Common.hpp>
#include "Primitives/General/typestring.hpp"
#include "Algorithms/CompileTimeControlFlow.hpp"
#include "Primitives/General/Hash.hpp"
#include "TypeTraits/IsImmutable.h"
#include "TypeTraits/StaticallySized.h"
#include "IO/FetchRequest.h"
#include "StaticArray.hpp"
namespace LD
{
    class ImmutableStringWarrant
    {

    };
    template <class T>
    using ImmutableStringtWarrantCheck = decltype(LD::Declval<T>().GetImmutableStringWarrant());


    //template<typename T>
    //constexpr bool IsImmutableString= LD::Exists<LD::ImmutableStringtWarrantCheck,T>;

    template<UInteger N>
    class ImmutableString
    {
    private:
        char string[N+1];
    public:


        constexpr inline ImmutableStringWarrant GetImmutableStringWarrant() const
        {
            return ImmutableStringWarrant{};
        }

        static constexpr LD::UInteger Length = N;

        constexpr ImmutableString() noexcept
        {
            LD::For<0,N+1>([](auto I, LD::ImmutableString<N> * instance)
            {
                instance->string[I] = '\0';
                return true;
            },this);
        }

        constexpr ImmutableString(const char & value) noexcept: ImmutableString()
        {
            LD::For<0,N>([](auto I, LD::ImmutableString<N> * instance, const char & value)
            {
                instance->string[I] = value;
                return true;
            },this,value);
        }

        template<typename U, typename = typename LD::Enable_If_T<LD::Require<LD::IsInteger<U>,LD::IsUnsignedInteger<U>>>>
        constexpr ImmutableString(const U & value) noexcept
        {
            LD::For<0,N+1>([](auto I, LD::ImmutableString<N> * instance, const U & value)
            {
                instance->string[I] = value;
                return true;
            },this,value);
        }

         const LD::UInteger GetSize() const
        {

            LD::UInteger currentSize = 0;

            LD::For<0,N>([](auto I, LD::UInteger & size, const ImmutableString<N> * instance)
            {
                size += (instance->string[I] != '\0');

                return true;
            },currentSize,this);

            return currentSize;
        }

        template<char ... Characters,typename = typename LD::Enable_If_T<sizeof...(Characters) <= N>>
        constexpr ImmutableString(const LD::TypeString<Characters...> & typeString): ImmutableString()
        {
            (*this) = typeString;

        }
        template<LD::UInteger Size, typename = typename LD::Enable_If_T<Size <= N>>
        constexpr ImmutableString(const LD::ImmutableString<Size> & string) noexcept:ImmutableString()
        {
            LD::For<Size>([](
                    auto I,
                    auto && instance,
                    auto && string)
            {
                instance[I] = string[I];
                return true;
            },this->string,string);
        }
        template<LD::UInteger Size, typename = typename LD::Enable_If_T<Size <= N>>
        constexpr ImmutableString(const LD::StaticArray<char,Size> & string) noexcept
        {
            LD::For<Size>([](
                    auto I,
                    auto && instance,
                    auto && string)
            {

                instance[I] = string[I];

                return true;
            },this->string,string);
        }

        constexpr ImmutableString(const char (&lit)[N + 1]): ImmutableString()
        {
            (*this) = lit;
        }


        constexpr ImmutableString(const char * data, const LD::UInteger & size) noexcept:ImmutableString()
        {
            LD::IF(data != nullptr,[](const char * data, char * string, const LD::UInteger & size)
            {
                const LD::UInteger length = (size>N)*N + (size <= N)*size;
                for (LD::UInteger i = 0; i < length; ++i)
                {
                    string[i] = data[i];
                }

            },data,this->string,size);

        }



        template<char ... Characters,typename = typename LD::Enable_If_T<(sizeof...(Characters) <= N)>>
        constexpr ImmutableString & operator = (const LD::TypeString<Characters...> & typeString)
        {
            LD::For<LD::TypeString<Characters...>::size()>([](auto I,ImmutableString<N> * instance)
            {
              instance->string[I] = LD::TypeString<Characters...>::data()[I];

              return true;

            },this);

            return (*this);
        }

        template<LD::UInteger Size>
        constexpr ImmutableString & operator = (const LD::ImmutableString<Size> & string) noexcept
        {
            LD::For<Size>([](
                    auto I,
                    auto && instance,
                    auto && string)
                    {

                        instance[I] = string[I];
                        return true;
                    },this->string,string);
            return (*this);
        }

        constexpr ImmutableString & operator = (const char (&lit)[N + 1])
        {
            LD::For<0,N>([](auto I, LD::ImmutableString<N> * instance, const char * buffer)
            {
               instance->string[I] = buffer[I];
               return true;
            },this,lit);

            return (*this);
        }

        constexpr const char & operator[](const LD::UInteger & n) const {return string[n];}

        constexpr char & operator[](const LD::UInteger & n) {return string[n];}

        constexpr const char * Data() const
        {
            return string;
        }

        constexpr const UInteger Size()
        const
        {
            return N;
        }

        constexpr const char * Begin() const
        {
            return string;
        }

        constexpr const char * End() const
        {
            return &string[N];

        }

        constexpr const bool operator < (const LD::ImmutableString<N> & string) const
        {
            const bool comparable = true;

            LD::For<0,N>([](auto I, const LD::ImmutableString<N> * instance,const LD::ImmutableString<N> & string, bool & result)
            {
                const bool comparsionResult = instance->string[I] < string.string[I];
                result = (result && (comparsionResult));
                return (comparsionResult);
            },this,string,comparable);
            return comparable;
        }


        template<LD::UInteger M>
        LD::Enable_If_T<
                LD::Require<
                        (N < M)
                >,
                const bool> operator < (const LD::ImmutableString<M> & string) const
        {
            return true;
        }

        template<LD::UInteger M>
        LD::Enable_If_T<
                LD::Require<
                        (N > M)
                >,
                const bool> operator < (const LD::ImmutableString<M> & string) const
        {
            return false;
        }


        constexpr LD::ImmutableString<N> & TrimTrailing(const char & character) noexcept
        {
            bool foundEdgeOfTrim = false;
            LD::For<15,0,1>([](auto Index, bool & foundEdgeOfTrim, char * str, const char & character)
            {
                LD::IF(!(str[Index] == character || str[Index] == '\0'),[](bool & foundEdgeOfTrim)
                {
                    foundEdgeOfTrim=true;

                },foundEdgeOfTrim);

                bool canAssign = ((str[Index] == character || str[Index] == '\0') && !foundEdgeOfTrim);


                str[Index] = (canAssign)*'\0' + (!canAssign)*str[Index];


                return true;
            },foundEdgeOfTrim,this->string,character);
            return (*this);
        }
        const bool operator > (const LD::ImmutableString<N> & string) const
        {
            const bool comparable = true;

            LD::For<N>([](auto I, const LD::ImmutableString<N> * instance,const LD::ImmutableString<N> & string, bool & result)
            {
               const bool comparsionResult = instance->string[I] > string.string[I];
               result = (result && (comparsionResult));
               return (comparsionResult);
            },this,string,comparable);

            return comparable;
        }

        template<LD::UInteger M>
        LD::Enable_If_T<
                LD::Require<
                        (N > M)
                >,
                const bool> operator > (const LD::ImmutableString<M> & string) const
        {
            return true;
        }

        template<LD::UInteger M>
        LD::Enable_If_T<
                LD::Require<
                        (N < M)
                >,
                const bool> operator > (const LD::ImmutableString<M> & string) const
        {
            return false;
        }



        const bool operator <= (const LD::ImmutableString<N> & string) const
        {
            return ((*this) == string) || ((*this) < string);
        }

        template<LD::UInteger M>
        LD::Enable_If_T<
                LD::Require<
                        (N > M)
                >,
                const bool> operator <= (const LD::ImmutableString<M> & string) const
        {
            return false;
        }

        template<LD::UInteger M>
        LD::Enable_If_T<
                LD::Require<
                        (N < M)
                >,
                const bool> operator <= (const LD::ImmutableString<M> & string) const
        {
            return true;
        }


        const bool operator >= (const LD::ImmutableString<N> & string) const
        {
            return ((*this) == string) || ((*this) > string);
        }

        template<LD::UInteger M>
        LD::Enable_If_T<
                LD::Require<
                        (N < M)
                >,
                const bool> operator >= (const LD::ImmutableString<M> & string) const
        {
            return false;
        }

        template<LD::UInteger M>
        LD::Enable_If_T<
                LD::Require<
                        (N > M)
                >,
                const bool> operator >= (const LD::ImmutableString<M> & string) const
        {
            return true;
        }


        template<LD::UInteger M>
        LD::Enable_If_T<
                LD::Require<
                        (N != M)
                >,
                const bool> operator == (const LD::ImmutableString<M> & string) const
        {
            return false;
        }

        constexpr const bool operator == (const LD::ImmutableString<N> & string) const
        {
            bool comparable = true;

            LD::For<0,N>([](auto I, const LD::ImmutableString<N> * instance,const LD::ImmutableString<N> & string, bool & result)
            {
                const bool comparsionResult = instance->string[I] == string.string[I];
                result = (result && (comparsionResult));
                return (comparsionResult);
            },this,string,comparable);

            return comparable;
        }

        constexpr const bool operator != (const LD::ImmutableString<N> & string) const
        {
            const bool comparable = true;

            LD::For<N>([](auto I, const LD::ImmutableString<N> * instance,const LD::ImmutableString<N> & string, bool & result)
            {
               const bool comparsionResult = !(instance->string[I] == string.string[I]);
               result = (result && (comparsionResult));
               return (comparsionResult);
            },this,string,comparable);

            return comparable;
        }

        template<LD::UInteger M>
        LD::Enable_If_T<
                LD::Require<
                        (N != M)
                >,
                const bool> operator != (const LD::ImmutableString<M> & string) const
        {
            return true;
        }
    };

    namespace Detail
    {
        template<typename T>
        struct IsImmutableString
        {
            constexpr static const bool value = false;
        };

        template<LD::UInteger Size>
        struct IsImmutableString<LD::ImmutableString<Size>>
        {
            constexpr static const bool value = true;
        };
    }
    template<typename T>
    constexpr bool IsImmutableString = LD::Detail::IsImmutableString<T>::value;


}

namespace LD
{
    template<LD::UInteger Size>
    constexpr LD::UInteger ImmutableSize(LD::Type<LD::ImmutableString<Size>> ) noexcept
    {
        return Size;
    }
}
template<LD::UInteger N, LD::UInteger M>
constexpr LD::ImmutableString<(N+M)> operator+(const LD::ImmutableString<N> & a, const LD::ImmutableString<M> & b) noexcept
{
    LD::ImmutableString<N+M> ret;

    LD::UInteger firstSize = a.GetSize();
    LD::UInteger secondSize = b.GetSize();

    //LD::UInteger n = 0;+
    for(LD::UInteger n = 0;n<firstSize;++n)
    {
        ret[n] = a[n];
    }
    for(LD::UInteger n = 0;n<secondSize;++n)
    {
        ret[firstSize+n] = b[n];
    }
    /*
    LD::For<N>([](
            auto I,
            LD::UInteger &n,
            const LD::ImmutableString<N> & input,
            const LD::UInteger & size,
            LD::ImmutableString<N+M> & output)
    {
        output[n++] = input[I];
        return (I < size);
    },n,a,firstSize,ret);

    LD::For<M>([](
            auto I,
            LD::UInteger &n,
            const LD::UInteger & size,
            const LD::ImmutableString<M> & input,
            LD::ImmutableString<N+M> & output)
    {
        output[n++] = input[I];
        return (I < size);

    },n,secondSize,b,ret);
     */

    return ret;
}
template<LD::UInteger N, char ... A>
constexpr LD::ImmutableString<N+sizeof...(A)> operator+(const LD::ImmutableString<N> & a, const LD::TypeString<A...> & b) noexcept
{
    return a + LD::ImmutableString<sizeof...(A)>{b};
}
namespace LD
{
    template<typename T,UInteger N>
    class ImmutableArray
    {
    private:
        T Buffer[N];
    public:

        constexpr ImmutableArray() noexcept
        {

        }

        constexpr ImmutableArray(const T & value) noexcept
        {
            LD::For<N>([](
                    auto I,
                    T * buffer,
                    const T & value)
            {
                buffer[I] = value;
                return true;
            },this->Buffer,value);
        }

        constexpr const T & operator[](const UInteger & n) const {return Buffer[n];}

        constexpr  T & operator[](const UInteger & n)  {return Buffer[n];}
        constexpr const T * Data() const
        {
            //return string.data();

            return Buffer;

        }

        template<typename ... Args,typename = typename LD::Enable_If_T<((sizeof...(Args) == N) && LD::IsPackConvertible<LD::VariadicPack<Args...>, T>::value)>>
        constexpr ImmutableArray(Args && ...arguements)
        {
            auto tuple = LD::MakeTuple(LD::Forward<Args>(arguements)...);

            //AssignImmutableArrayFromArguements(PDP::Forward<Args>(arguements)...);

            LD::For<0,N>([](auto I, LD::ImmutableArray<T,N> * instance, auto && tuple)
            {
                instance->Buffer[I] = LD::Get<I>(tuple);

                return true;
            },this,tuple);
        }

        constexpr const T * Begin() const
        {
            return Buffer;

        }

        constexpr const T * End() const
        {
            return &Buffer[N];

        }
    };
}

template<LD::UInteger N, LD::UInteger M, typename T>
constexpr LD::ImmutableArray<T,N+M> operator+(const LD::ImmutableArray<T,N>& a, const LD::ImmutableArray<T,M> & b)
{
    LD::ImmutableArray<T,N+M> ret;

    LD::For<0,N>([](auto I, const LD::ImmutableArray<T,N> & input, LD::ImmutableArray<T,N+M> & output)
    {
        output[I] = input[I];

        return true;
    },a,ret);


    LD::For<N,(N+M)>([](auto I, const LD::ImmutableArray<T,M> & input, LD::ImmutableArray<T,N+M> & output)
    {

        output[I] = input[I-N-1];

        return true;
    },b,ret);

    return ret;
}

namespace LD
{
    template<LD::UInteger Index, LD::UInteger Size>
    const char & Get(const LD::ImmutableString<Size> & immutableString)
    {
        return immutableString[Index];
    }

    template<LD::UInteger Index, LD::UInteger Size>
     char & Get(LD::ImmutableString<Size> & immutableString)
    {
        return immutableString[Index];
    }

    template<LD::UInteger StringSize>
    LD::ImmutableString<StringSize-1> ToImmutableString(const char (&lit)[StringSize])
    {
        LD::ImmutableString<StringSize-1> str;
        LD::For<(StringSize-1)>([](auto Index,const char (&lit)[StringSize],LD::ImmutableString<StringSize-1> & str)
        {
            str[Index] = lit[Index];
            return true;
        },lit,str);
        return str;
    }


    template<char ... Characters>
    constexpr LD::ImmutableString<sizeof...(Characters)> ToImmutableString(const LD::TypeString<Characters...> & typeString)
    {

        char returnable[sizeof...(Characters)+1] = {0};
        LD::For<sizeof...(Characters)>([](auto Index, char ret[sizeof...(Characters)+1])
        {
            ret[Index] = LD::TypeString<Characters...>::data()[Index];
            return true;
        },returnable);

        return LD::ImmutableString<sizeof...(Characters)>{returnable};
    }
    template<typename T>
    constexpr LD::Enable_If_T<LD::Require<LD::Detail::IsSame<T,bool>::value>,LD::ImmutableString<5>> ToImmutableString(const T & booleanValue)
    {
        LD::StaticArray<LD::ImmutableString<5>,2> results;

        LD::ImmutableString<5> trueResult;
        trueResult[0] = 't';
        trueResult[1] = 'r';
        trueResult[2] = 'u';
        trueResult[3] = 'e';
        trueResult[4] = '\0';

        LD::ImmutableString<5> falseResult;
        falseResult[0] = 'f';
        falseResult[1] = 'a';
        falseResult[2] = 'l';
        falseResult[3] = 's';
        falseResult[4] = 'e';

        results[1] = trueResult;
        results[0] = falseResult;
        //true
        //false
        return results[booleanValue];
    }
    template<typename T>
    constexpr LD::Enable_If_T<LD::Require<LD::Detail::IsSame<T,unsigned char>::value>,LD::ImmutableString<1>> ToImmutableString(const T & character)
    {
        LD::ImmutableString<1> mString;
        mString[0] = character;
        return character;
    }
    template<typename T>
     LD::Enable_If_T<LD::Require<
            LD::Detail::IsSame<T,float>::value
    >,
            LD::ImmutableString<19+15+2>> ToImmutableString(const T & number,const LD::Variant<LD::TransactionError,LD::UInteger>  & precision = {},const LD::UInteger & base = 10) noexcept
    {
        LD::ImmutableString<19+15+2> ret = {0};
        LD::UInteger num = LD::UInteger(LD::Abs(number));
        bool isFloatingPoint = ((LD::Abs(number) - num) > 0);
        char integerPart[20] = {0};
        bool isInf = false;
        bool isNan = false;
        const LD::UInteger amountOfDigits = LD::Floor(LD::FastLog10(num))+1;
        const LD::UInteger amountToAllocate = (amountOfDigits   + 1);
        LD::For<19>([](auto Index,
                       LD::UInteger & num,
                       const LD::UInteger & base,
                       char ret[20],
                       const LD::UInteger & isInf,
                       const LD::UInteger & isNan,
                       const LD::UInteger & amountToAllocate,
                       const LD::UInteger & amountOfDigits)
                    {
                        const LD::Integer rem = num % base;
                        ret[(amountToAllocate-1)-Index-1] = (((rem > 9)* ((rem-10) + 'a')) + (!(rem > 9)* (rem + '0'))) * (!isInf && !isNan);
                        num/=base;
                        return (Index < amountOfDigits);
                    },num,base,integerPart,isInf,isNan,amountToAllocate,amountOfDigits);

        ret[0] = '-';
        const bool isNegative = (number < 0);
        for(LD::UInteger n = 0;n<19;++n)
        {
            ret[n+isNegative] = integerPart[n];
        }

        ret[amountOfDigits] = (isFloatingPoint)*'.';
        LD::Float decimalPortion = LD::Abs(number) - num;

        auto onCullingRequest = [](const LD::UInteger & number) noexcept
        {
            return LD::Pair<bool,LD::UInteger>{true,number};
        };

        auto onDefaultRequest = [](const LD::TransactionError & ) noexcept
        {
            return LD::Pair<bool,LD::UInteger>{false,0};
        };

        LD::Pair<bool,LD::UInteger> request = LD::Match(precision,onCullingRequest,onDefaultRequest);
        LD::QueryResult<LD::UInteger(LD::Float &,LD::UInteger&,LD::ImmutableString<19+15+2>&,LD::UInteger &,bool&)> cullingPredicate[2];
        cullingPredicate[0] = LD::MakeContext(LD::TransactionError{},decimalPortion,base,ret,amountOfDigits,isFloatingPoint);
        cullingPredicate[1] = LD::MakeContext(LD::TransactionResult{},LD::UInteger {request.GetSecond()},decimalPortion,base,ret,amountOfDigits,isFloatingPoint);


        auto onCull = [](const LD::Context<LD::TransactionResult,LD::UInteger,LD::Float &,LD::UInteger&,LD::ImmutableString<19+15+2>&,LD::UInteger &,bool&> & context) noexcept
        {

            LD::UInteger cullIndex = LD::Get(LD::Get<1>(context));
            LD::Float & decimalPortion = LD::Get(LD::Get<2>(context));
            const LD::UInteger & base = LD::Get(LD::Get<3>(context));
            LD::ImmutableString<19+15+2>& ret = LD::Get(LD::Get<4>(context));
            const LD::UInteger & amountOfDigits = LD::Get(LD::Get<5>(context));
            const bool & isFloatingPoint = LD::Get(LD::Get<6>(context));

            for(LD::UInteger n = 0;n<15;++n)
            {
                decimalPortion*=base;
                const LD::UInteger digit = (LD::UInteger(decimalPortion))%base;
                ret[amountOfDigits+n+isFloatingPoint] = (isFloatingPoint && n<cullIndex)*('0' + digit);
                decimalPortion = decimalPortion-(LD::UInteger(decimalPortion));
            }
        };

        auto onDefaultBehavior = [](const LD::Context<LD::TransactionError,LD::Float &,LD::UInteger&,LD::ImmutableString<19+15+2>&,LD::UInteger &,bool&> & context) noexcept
        {
            LD::Float & decimalPortion = LD::Get(LD::Get<1>(context));
            const LD::UInteger & base = LD::Get(LD::Get<2>(context));
            LD::ImmutableString<19+15+2>& ret = LD::Get(LD::Get<3>(context));
            const LD::UInteger & amountOfDigits = LD::Get(LD::Get<4>(context));
            const bool & isFloatingPoint = LD::Get(LD::Get<5>(context));


            for(LD::UInteger n = 0;n<15;++n)
            {
                decimalPortion*=base;
                const LD::UInteger digit = (LD::UInteger(decimalPortion))%base;
                ret[amountOfDigits+n+isFloatingPoint] = (isFloatingPoint)*('0' + digit);
                decimalPortion = decimalPortion-(LD::UInteger(decimalPortion));
            }
        };

        LD::Match(cullingPredicate[request.GetFirst()],onCull,onDefaultBehavior);
        return ret;
    }

    template<typename T>
     LD::Enable_If_T<LD::Require<
            LD::Detail::IsSame<T,double>::value
    >,
            LD::ImmutableString<19+15+2>> ToImmutableString(const T & number,const LD::Variant<LD::TransactionError,LD::UInteger>  & precision = {},const LD::UInteger & base = 10) noexcept
    {

        LD::ImmutableString<19+15+2> ret = {0};
        LD::UInteger num = LD::UInteger(LD::Abs(number));
        bool isFloatingPoint = ((LD::Abs(number) - num) > 0);
        char integerPart[20] = {0};
        bool isInf = false;
        bool isNan = false;
        const LD::UInteger amountOfDigits = LD::Floor(LD::FastLog10(num))+1;
        const LD::UInteger amountToAllocate = (amountOfDigits   + 1);
        LD::For<19>([](auto Index,
                       LD::UInteger & num,
                       const LD::UInteger & base,
                       char ret[20],
                       const LD::UInteger & isInf,
                       const LD::UInteger & isNan,
                       const LD::UInteger & amountToAllocate,
                       const LD::UInteger & amountOfDigits)
        {
            const LD::Integer rem = num % base;
            ret[(amountToAllocate-1)-Index-1] = (((rem > 9)* ((rem-10) + 'a')) + (!(rem > 9)* (rem + '0'))) * (!isInf && !isNan);
            num/=base;
            return (Index < amountOfDigits);
        },num,base,integerPart,isInf,isNan,amountToAllocate,amountOfDigits);

        ret[0] = '-';
        const bool isNegative = (number < 0);
        for(LD::UInteger n = 0;n<19;++n)
        {
            ret[n+isNegative] = integerPart[n];
        }

        ret[amountOfDigits] = (isFloatingPoint)*'.';
        LD::Float decimalPortion = LD::Abs(number) - num;
        
        auto onCullingRequest = [](const LD::UInteger & number) noexcept
        {
            return LD::Pair<bool,LD::UInteger>{true,number};
        };

        auto onDefaultRequest = [](const LD::TransactionError & ) noexcept
        {
            return LD::Pair<bool,LD::UInteger>{false,0};
        };

        LD::Pair<bool,LD::UInteger> request = LD::Match(precision,onCullingRequest,onDefaultRequest);
        LD::QueryResult<LD::UInteger(LD::Float &,LD::UInteger&,LD::ImmutableString<19+15+2>&,LD::UInteger &,bool&)> cullingPredicate[2];
        cullingPredicate[0] = LD::MakeContext(LD::TransactionError{},decimalPortion,base,ret,amountOfDigits,isFloatingPoint);
        cullingPredicate[1] = LD::MakeContext(LD::TransactionResult{},LD::UInteger {request.GetSecond()},decimalPortion,base,ret,amountOfDigits,isFloatingPoint);


        auto onCull = [](const LD::Context<LD::TransactionResult,LD::UInteger,LD::Float &,LD::UInteger&,LD::ImmutableString<19+15+2>&,LD::UInteger &,bool&> & context) noexcept
        {

            LD::UInteger cullIndex = LD::Get(LD::Get<1>(context));
            LD::Float & decimalPortion = LD::Get(LD::Get<2>(context));
            const LD::UInteger & base = LD::Get(LD::Get<3>(context));
            LD::ImmutableString<19+15+2>& ret = LD::Get(LD::Get<4>(context));
            const LD::UInteger & amountOfDigits = LD::Get(LD::Get<5>(context));
            const bool & isFloatingPoint = LD::Get(LD::Get<6>(context));

            for(LD::UInteger n = 0;n<15;++n)
            {
                decimalPortion*=base;
                const LD::UInteger digit = (LD::UInteger(decimalPortion))%base;
                ret[amountOfDigits+n+isFloatingPoint] = (isFloatingPoint && n<cullIndex)*('0' + digit);
                decimalPortion = decimalPortion-(LD::UInteger(decimalPortion));
            }
        };

        auto onDefaultBehavior = [](const LD::Context<LD::TransactionError,LD::Float &,LD::UInteger&,LD::ImmutableString<19+15+2>&,LD::UInteger &,bool&> & context) noexcept
        {
            LD::Float & decimalPortion = LD::Get(LD::Get<1>(context));
            const LD::UInteger & base = LD::Get(LD::Get<2>(context));
            LD::ImmutableString<19+15+2>& ret = LD::Get(LD::Get<3>(context));
            const LD::UInteger & amountOfDigits = LD::Get(LD::Get<4>(context));
            const bool & isFloatingPoint = LD::Get(LD::Get<5>(context));


            for(LD::UInteger n = 0;n<15;++n)
            {
                decimalPortion*=base;
                const LD::UInteger digit = (LD::UInteger(decimalPortion))%base;
                ret[amountOfDigits+n+isFloatingPoint] = (isFloatingPoint)*('0' + digit);
                decimalPortion = decimalPortion-(LD::UInteger(decimalPortion));
            }
        };

        LD::Match(cullingPredicate[request.GetFirst()],onCull,onDefaultBehavior);
        return ret;
    }
    template<typename T>
    constexpr LD::Enable_If_T<LD::Require<
            LD::IsUnsignedInteger<T>,
            LD::IsPrimitive<T>
    >,
            LD::ImmutableString<19>>  ToImmutableString(const T & number, const LD::UInteger & base = 10) noexcept
    {
        const LD::UInteger isNan = (number != number);
        const LD::UInteger isInf = (number > LD::Limit<T>::GetMax());
        char returnValue[20] = {0};
        LD::UInteger num = number;
        const LD::UInteger amountOfDigits = LD::Floor(LD::FastLog10(num))+1;
        returnValue[0] = 'n'*(isNan * !isInf);
        returnValue[1] = 'a'*(isNan * !isInf);
        returnValue[2] = 'n'*(isNan * !isInf);
        returnValue[3] = '\0';
        returnValue[0] = '+'*(!isNan * isInf) + (isNan * 'n');
        returnValue[1] = 'i'*(!isNan * isInf) + (isNan * 'a');
        returnValue[2] = 'n'*(!isNan * isInf) + (isNan * 'n');
        returnValue[3] = 'f'*(!isNan * isInf);
        returnValue[4] = '\0';
        const LD::UInteger amountToAllocate = (amountOfDigits   + 1);
        LD::For<19>([](auto Index,
                LD::UInteger & num,
                const LD::UInteger & base,
                char ret[20],
                const LD::UInteger & isInf,
                const LD::UInteger & isNan,
                const LD::UInteger & amountToAllocate,
                const LD::UInteger & amountOfDigits)
        {
            const LD::Integer rem = num % base;
            ret[(amountToAllocate-1)-Index-1] = (((rem > 9)* ((rem-10) + 'a')) + (!(rem > 9)* (rem + '0'))) * (!isInf && !isNan);
            num/=base;
            return (Index < amountOfDigits);
        },num,base,returnValue,isInf,isNan,amountToAllocate,amountOfDigits);
        return LD::ImmutableString<19>{returnValue};
    }

    template<typename T>
    LD::Enable_If_T<LD::Require<
            LD::IsInteger<T>,
            LD::IsPrimitive<T>
    >,
            LD::ImmutableString<20>>  ToImmutableString(const T & number, const LD::UInteger & base = 10) noexcept
    {
        const LD::UInteger isNan = (number != number);
        const LD::UInteger isInf = (number > LD::Limit<T>::GetMax());
        char returnValue[21] = {0};
        LD::UInteger num = LD::Abs(number);
        const LD::UInteger amountOfDigits = LD::Floor(LD::FastLog10(num))+1;
        returnValue[0] = 'n'*(isNan * !isInf);
        returnValue[1] = 'a'*(isNan * !isInf);
        returnValue[2] = 'n'*(isNan * !isInf);
        returnValue[3] = '\0';
        returnValue[0] = '+'*(!isNan * isInf) + (isNan * 'n');
        returnValue[1] = 'i'*(!isNan * isInf) + (isNan * 'a');
        returnValue[2] = 'n'*(!isNan * isInf) + (isNan * 'n');
        returnValue[3] = 'f'*(!isNan * isInf);
        returnValue[4] = '\0';
        const LD::UInteger isNegative = (number < 0);
        const LD::UInteger amountToAllocate = (amountOfDigits   + 1 + isNegative);
        returnValue[0] = '-' * (!isNan) * (!isInf);
        LD::For<20>([](auto Index,
                       LD::UInteger & num,
                       const LD::UInteger & base,
                       char ret[21],
                       const LD::UInteger & isInf,
                       const LD::UInteger & isNan,
                       const LD::UInteger & amountToAllocate,
                       const LD::UInteger & amountOfDigits,
                       const LD::UInteger & isNegative)
                    {
                        const LD::Integer rem = num % base;
                        ret[(amountToAllocate-1)-Index-1] = (((rem > 9)* ((rem-10) + 'a')) + (!(rem > 9)* (rem + '0'))) * (!isInf && !isNan);
                        num/=base;
                        return (Index < (amountOfDigits - isNegative));
                    },num,base,returnValue,isInf,isNan,amountToAllocate,amountOfDigits,isNegative);
        return LD::ImmutableString<20>{returnValue};
    }

    template<typename T>
    LD::Enable_If_T<LD::Require<
            LD::IsCharacter<T>,
            LD::IsPrimitive<T>
    >,
            LD::ImmutableString<1>>  ToImmutableString(const T & character) noexcept
    {
                char buffer[2] = {0};
                buffer[0] = character;
        return LD::ImmutableString<1>(buffer);
    }


    template<typename T>
    using CanBeImmutableString = decltype(LD::ToImmutableString(LD::Declval<T>()));


}

namespace PDP
{
    template<LD::UInteger Size>
    class Hash<LD::ImmutableString<Size>>
    {
    public:
        PDP::UInteger operator()(const LD::ImmutableString<Size> & s) const noexcept
        {
            constexpr static LD::UInteger primaryHash = 54059;
            constexpr static LD::UInteger  secondaryHash = 76963;

            unsigned h = 37;

            const char * buffer = s.Data();
            for (PDP::UInteger n = 0; n<s.GetSize(); ++n )
            {
                h = (h * primaryHash) ^ (*(buffer+n) * secondaryHash);
                //s++;
            }
            return h;
        }
    };




}
template<char ... Characters>
inline constexpr LD::ImmutableString<sizeof...(Characters)> operator "" _is() noexcept
{
    return LD::ImmutableString<sizeof...(Characters)>{LD::TypeString<Characters...>{}};
}

namespace LD
{
    namespace Detail
    {
        template<LD::UInteger Size>
        struct IsImmutable<LD::ImmutableString<Size>>
        {
            constexpr static bool value = true;
        };

        template<typename T,LD::UInteger Size>
        struct IsImmutable<LD::ImmutableArray<T,Size>>
        {
            constexpr static bool value = true;
        };
    }
}

namespace LD
{
    namespace Detail
    {
        template<LD::UInteger Size>
        struct StaticallySized<LD::ImmutableString<Size>>: public LD::Detail::IntegralConstant<bool,true>
        {

        };

        template<typename T,LD::UInteger Size>
        struct StaticallySized<LD::ImmutableArray<T,Size>>: public LD::Detail::IntegralConstant<bool,true>
        {

        };


        template<typename T>
        using ImmtuableConversion = decltype(LD::ToImmutableString(LD::Declval<T>()));
    }

    namespace CT
    {
        /*
        template<typename T>
        constexpr bool CanBeAnImmutableString(LD::Type<T> ) noexcept
        {
            return LD::Exists<LD::Detail::ImmtuableConversion,T>;
        }
         */
    }

    template <typename CharT, LD::UInteger N> ImmutableString(const CharT (&)[N]) -> ImmutableString<N-1>;
    template <LD::UInteger N> ImmutableString(ImmutableString<N>) -> ImmutableString<N>;
    template <LD::UInteger N> ImmutableString(LD::StaticArray<char,N>) -> ImmutableString<N>;
    template<char ... Characters>
    ImmutableString(LD::TypeString<Characters...>) -> ImmutableString<sizeof...(Characters)>;
}

#endif /* Immutable_h */
