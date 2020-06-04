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
namespace LD
{
    class ImmutableStringWarrant
    {

    };
    template <class T>
    using ImmutableStringtWarrantCheck = decltype(LD::Declval<T>().GetImmutableStringWarrant());


    template<typename T>
    constexpr bool IsImmutableString= LD::Exists<LD::ImmutableStringtWarrantCheck,T>;

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

        constexpr ImmutableString(const char & value) noexcept
        {
            LD::For<0,N+1>([](auto I, LD::ImmutableString<N> * instance, const char & value)
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
}


template<LD::UInteger N, LD::UInteger M>
constexpr LD::ImmutableString<N+M> operator+(const LD::ImmutableString<N> & a, const LD::ImmutableString<M> & b)
{

    LD::ImmutableString<N+M> ret;

    //LD::UInteger count = 0;
    LD::UInteger firstSize = a.GetSize();
    LD::UInteger secondSize = b.GetSize();


    LD::For<0,N>([](auto I, const LD::ImmutableString<N> & input, LD::ImmutableString<N+M> & output, const LD::UInteger & size)
    {
         output[I] = input[I];

         return (I+1) < size;
    },a,ret,firstSize);

    LD::For<N,(N+M)>([](auto I, const LD::ImmutableString<M> & input, LD::ImmutableString<N+M> & output, const LD::UInteger & size)
    {
         output[I] = input[I-N-1];

         return ((I-N-1)+1) < size;
    },b,ret,secondSize);

    return ret;
}

namespace LD
{
    template<typename T,UInteger N>
    class ImmutableArray
    {
    private:
        T Buffer[N];
    public:

        constexpr ImmutableArray()
        {

        }

        constexpr const T & operator[](const UInteger & n) const {return Buffer[n];}

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
    constexpr LD::Enable_If_T<LD::Require<
            LD::Detail::IsSame<T,float>::value
    >,
            LD::ImmutableString<20+7+2>> ToImmutableString(const T & number,const LD::UInteger & precision = 0,const LD::UInteger base = 10) noexcept
    {
        const LD::UInteger isNan = (number != number);
        const LD::UInteger isInf = (number > LD::Detail::Limit<T>::GetMax());
        char returnValue[20+7+2+1] = {0};
        LD::Float unsignedNumber = LD::Abs(number);
        LD::UInteger num = LD::UInteger (unsignedNumber);
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
        const LD::UInteger hasDecimal = ((unsignedNumber - num) > 0);
        returnValue[0] = '-' * (!isNan) * (!isInf);
        LD::For<20>([](auto Index,
                       LD::UInteger & num,
                       const LD::UInteger & base,
                       char ret[20+7+2+1],
                       const LD::UInteger & isInf,
                       const LD::UInteger & isNan,
                       const LD::UInteger & amountToAllocate,
                       const LD::UInteger & amountOfDigits,
                       const LD::UInteger & isNegative)
        {
            const LD::Integer rem = num % base;
            ret[(amountToAllocate-1)-Index-1] = (((rem > 9)* ((rem-10) + 'a')) + (!(rem > 9)* (rem + '0'))) * (!isInf && !isNan);
            num/=base;
            return (Index < (amountOfDigits - isNegative)) && (num != 0);
        },num,base,returnValue,isInf,isNan,amountToAllocate,amountOfDigits,isNegative);

        returnValue[amountToAllocate-1] = '.' * (hasDecimal);

        const LD::Float decimalPortion = (unsignedNumber - num);
        LD::UInteger decimalInIntegerPortion = LD::Detail::CompileTimePow<10,7>::value*decimalPortion;
        const LD::UInteger amountOfDecimalDigits = LD::Floor(LD::FastLog10(decimalInIntegerPortion))+1;
        LD::For<7>([](auto Index,
                char ret[20+7+2+1],
                 LD::UInteger & num,
                const LD::UInteger & base,
                const LD::UInteger & isInf,
                const LD::UInteger & isNan,
                const LD::UInteger & amountToAllocate)
        {

            const LD::Integer rem = num % base;
            ret[(amountToAllocate-1)+(Index+1)]= (((rem > 9)* ((rem-10) + 'a')) + (!(rem > 9)* (rem + '0'))) * (!isInf && !isNan);;
            num/=base;
            return true;

        },returnValue,decimalInIntegerPortion,base,isInf,isNan,amountToAllocate);
        char * returnStringBuffer = (char*)returnValue;
        char * str = (returnStringBuffer+(amountToAllocate));
        LD::For<7/2>([](auto Index, char * string)
        {
            LD::Swap(string[Index],string[7-Index-1]);
            return true;
        },str);



        LD::For<7>([](auto Index, char * string, const LD::UInteger & precision)
        {
            string[Index] = (string[Index]*(precision == 0)) + ((precision > 0 && Index < precision)*(string[Index]));
            return true;
        },str,precision);

        return LD::ImmutableString<29>{returnValue};
    }

    template<typename T>
    constexpr LD::Enable_If_T<LD::Require<
            LD::Detail::IsSame<T,double>::value
    >,
            LD::ImmutableString<20+15+2>> ToImmutableString(const T & number,const LD::UInteger  & precision = 0,const LD::UInteger & base = 10) noexcept
    {

        const LD::UInteger isNan = (number != number);
        const LD::UInteger isInf = (number > LD::Detail::Limit<T>::GetMax());
        char returnValue[20+15+2+1] = {0};
        LD::Float unsignedNumber = LD::Abs(number);
        LD::UInteger num = LD::UInteger (unsignedNumber);
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
        const LD::UInteger hasDecimal = ((unsignedNumber - num) > 0);
        returnValue[0] = '-' * (!isNan) * (!isInf);
        LD::For<20>([](auto Index,
                       LD::UInteger & num,
                       const LD::UInteger & base,
                       char ret[20+7+2+1],
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


        returnValue[amountToAllocate-1] = '.' * (hasDecimal);


        const LD::Float decimalPortion = (unsignedNumber - num);
        LD::UInteger decimalInIntegerPortion = LD::Detail::CompileTimePow<10,15>::value*decimalPortion;
        const LD::UInteger amountOfDecimalDigits = LD::Floor(LD::FastLog10(decimalInIntegerPortion))+1;
        LD::For<15>([](auto Index,
                      char ret[20+15+2+1],
                      LD::UInteger & num,
                      const LD::UInteger & base,
                      const LD::UInteger & isInf,
                      const LD::UInteger & isNan,
                      const LD::UInteger & amountToAllocate)
        {

            const LD::Integer rem = num % base;
            char currentDigit = (((rem > 9)* ((rem-10) + 'a')) + (!(rem > 9)* (rem + '0'))) * (!isInf && !isNan);;
            ret[(amountToAllocate-1)+(Index+1)]= (currentDigit);
            num/=base;

            return   true;
        },returnValue,decimalInIntegerPortion,base,isInf,isNan,amountToAllocate);

        char * returnStringBuffer = (char*)returnValue;
        char * str = (returnStringBuffer+(amountToAllocate));


        LD::For<15/2>([](auto Index, char * string)
        {
            LD::Swap(string[Index],string[15-Index-1]);
            return true;
         },str);

        bool foundEdgeOfTrim = false;


        LD::For<15,0,1>([](auto Index, bool & foundEdgeOfTrim, char * str)
        {
            LD::IF(!(str[Index] == '0' || str[Index] == '\0'),[](bool & foundEdgeOfTrim)
            {
                foundEdgeOfTrim=true;

            },foundEdgeOfTrim);

            bool canAssign = ((str[Index] == '0' || str[Index] == '\0') && !foundEdgeOfTrim);


            str[Index] = (canAssign)*'\0' + (!canAssign)*str[Index];


            return true;
        },foundEdgeOfTrim,str);


        LD::For<15>([](auto Index, char * string, const LD::UInteger & precision)
        {
            string[Index] = (precision != 0)*((Index <= (precision-1))*string[Index] + (Index>(precision-1))*'\0') + (precision==0)*string[Index];
            return true;
         },str,precision);


        return LD::ImmutableString<20+15+2>{returnValue};
    }
    template<typename T>
    constexpr LD::Enable_If_T<LD::Require<
            LD::IsUnsignedInteger<T>,
            LD::IsPrimitive<T>
    >,
            LD::ImmutableString<19>>  ToImmutableString(const T & number, const LD::UInteger & base = 10) noexcept
    {
        const LD::UInteger isNan = (number != number);
        const LD::UInteger isInf = (number > LD::Detail::Limit<T>::GetMax());
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
        const LD::UInteger isInf = (number > LD::Detail::Limit<T>::GetMax());
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
    }
}

#endif /* Immutable_h */