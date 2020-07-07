//
// Created by phoenixflower on 7/2/20.
//

#ifndef LANDESSDEVCORE_COMPLIMENTGENERATOR_H
#define LANDESSDEVCORE_COMPLIMENTGENERATOR_H
#include "Definitions/Integer.hpp"
#include "TypeTraits/Detection.hpp"
#include "Primitives/General/Immutable.hpp"
#include "FullAdder.hpp"
#include "Primitives/General/BitSet.hpp"
namespace LD
{
    template<LD::UInteger Compliment, class = void>
    class ComplimentGenerator;



    template<>
    class ComplimentGenerator<1>
    {
    private:
    public:

        template<LD::UInteger Bits>
        constexpr LD::BitSet<Bits>  operator()(const LD::BitSet<Bits> & input) const noexcept
        {



            LD::BitSet<Bits> stuff = input;
            if constexpr(Bits < 1024)
            {
                LD::For<Bits>([](
                        auto I,
                        LD::BitSet<Bits> & bitSet)
                {
                    bitSet[I] = not bitSet[I];
                    return true;
                },stuff);

            }else
            {
                for(LD::UInteger n = 0;n<Bits;++n)
                {
                    stuff[n] = not stuff[n];
                }
            }
            return stuff;
        }

        template<LD::UInteger Bits>
        constexpr LD::ImmutableArray<unsigned char,Bits>  operator()(const LD::ImmutableArray<unsigned char,Bits> & input) const noexcept
        {
            LD::ImmutableArray<unsigned char,Bits> stuff = input;
            if constexpr(Bits < 1024)
            {
                LD::For<Bits>([](
                        auto I,
                        LD::ImmutableArray<unsigned char,Bits> & input)
                {
                    input[I] = (input[I] == '1')*'0' + (input[I] == '0')*'1';
                    return true;
                },stuff);
            }
            else
            {
                for(LD::UInteger n = 0;n<Bits;++n)
                {
                    stuff[n] = (stuff[n] == '1')*'0' + (stuff[n] == '0')*'1';
                }
            }

            return stuff;
        }
    };

    template<>
    class ComplimentGenerator<2>
    {
    public:
        template<LD::UInteger Bits>
        constexpr LD::BitSet<Bits> operator()(const LD::BitSet<Bits> & bitSet) const noexcept
        {
            LD::FullAdder<Bits> fullAdder;
            LD::BitSet<Bits> one = {false};
            one[Bits-1] = true;
            ComplimentGenerator<1> onesGenerator;
            return fullAdder(onesGenerator(bitSet),one);
        }
        template<LD::UInteger Bits>
        constexpr LD::ImmutableArray<unsigned char,Bits>  operator()(const LD::ImmutableArray<unsigned char,Bits> & stuff) const noexcept
        {
            ComplimentGenerator<1> onesGenerator;
            ;
            LD::FullAdder<Bits> adder;
            LD::ImmutableArray<unsigned char,Bits> one = {'0'};
            one[Bits-1] = '1';
            return adder(onesGenerator(stuff),one);
            //return stuff;
        }
    };

    namespace Detail
    {
        template<typename T, LD::UInteger Min, LD::UInteger Max>
        inline LD::BitSet<LD::CT::RangeSpan<LD::CT::Range<Min,Max>>> InternalGenerateBitSet(const T & input, const LD::CT::Range<Min,Max> & range) noexcept
        {
            LD::BitSet<LD::CT::RangeSpan<LD::CT::Range<Min,Max>>> result = {false};
            T n = input;
            int binaryNum[64];
            int i = 0;
            while(n!=0)
            {
                binaryNum[i++] = n%2==1;
                //r=(n%2==0 ?"0":"1")+r;
                n/=2;
            }
            // printing binary array in reverse order
            LD::UInteger k = 0;
            for(LD::UInteger n = 0;n<i;++n)
            {
                result[Max-1-n] = binaryNum[n];
            }
            return result;
        }
    }

    template<typename T, LD::UInteger Min, LD::UInteger Max>
    inline LD::Enable_If_T<LD::Require<LD::IsUnsignedInteger<T>,LD::IsPrimitive<T>>,LD::BitSet<(Max-Min)>> GenerateBitSet(const T & input, const LD::CT::Range<Min,Max> & range) noexcept
    {
        return LD::Detail::InternalGenerateBitSet(input,range);
    }

    template<typename T, LD::UInteger Min, LD::UInteger Max>
    inline LD::Enable_If_T<LD::Require<LD::IsInteger<T>>,LD::BitSet<(Max-Min)>> GenerateBitSet(const T & input, const LD::CT::Range<Min,Max> & range) noexcept
    {
        LD::ComplimentGenerator<2> complimentGenerator;
        //generate both of the signed and unsigned versions
        auto unsignedVersion = LD::Detail::InternalGenerateBitSet(LD::Abs(input),range);
        auto signedVersion = complimentGenerator(unsignedVersion);
        //clear the registers accordingly
        signedVersion.Clear(input >= 0);
        unsignedVersion.Clear(input < 0);
        LD::FullAdder<(Max-Min)> fullAdder;
        //this ends up being the correct one added to 0.
        return fullAdder(signedVersion,unsignedVersion);
    }

    template<typename T>
    inline LD::Enable_If_T<LD::Require<LD::IsSame<T,float>>,LD::BitSet<32>> GenerateBitSet(const T & input) noexcept
    {

        LD::BitSet<1> sign = {};
        LD::BitSet<8> exponent;
        LD::BitSet<23> mantissa;
        return {};
    }

    template<typename T>
    inline LD::Enable_If_T<LD::Require<LD::IsSame<T,double>>,LD::BitSet<64>> GenerateBitSet(const T & input) noexcept
    {
        LD::BitSet<64> floatingPointNumber = {false};

        LD::Float absoluteFP = LD::Abs(input);

        LD::UInteger integerBinaryLength = 0;
        std::cout << "stuff : " << integerBinaryLength << std::endl;
        LD::UInteger scientificNotationInteger = LD::GetInteger(absoluteFP);
        int binaryNum[52];




        while (scientificNotationInteger != 0 && input >= 1)
        {

            // storing remainder in binary array
            binaryNum[integerBinaryLength++] = scientificNotationInteger%2==1;
            //binaryNum[integerBinaryLength] = scientificNotationInteger % 2;
            //scientificNotationInteger = scientificNotationInteger / 2;
            scientificNotationInteger/=2;
            //integerBinaryLength++;
        }




        LD::BitSet<52> mantissa = {false};
        //get the integer in the mantissa if it's above 0

        for (LD::UInteger j = integerBinaryLength - 1,k=0; j != 0 && input >=1; j--,++k)
        {
            mantissa[k] = binaryNum[j];
        }


        double floatingPoint = LD::Abs(input) - LD::Abs((LD::UInteger)input);


        LD::Integer firstOccurenceOfOne = -1;
        //proceed to put the floating point number


        for (LD::UInteger n = 0;  n < (52-integerBinaryLength); ++n)
        {
            floatingPoint = floatingPoint*2;
            ////buffer[n] = (floatingPoint < 1)*'0' + (floatingPoint >= 1)*'1';
            bool index = (floatingPoint >= 1);
            //std::cout << index << std::endl;
            mantissa[integerBinaryLength+n] = (floatingPoint >= 1);
            //mantissa[integerBinaryLength+n] = index;
            floatingPoint = (floatingPoint)-LD::UInteger(floatingPoint);
            firstOccurenceOfOne = (firstOccurenceOfOne == -1 && index)*n + (firstOccurenceOfOne == -1 && not index)*-1 + (firstOccurenceOfOne != -1)*firstOccurenceOfOne;
        }

        LD::BitSet<11> exponentPart = {false};

        LD::Integer exponent = (input >= 1)*(integerBinaryLength-1) + (input > 0 && input < 1)*(-1*firstOccurenceOfOne);
        exponentPart = LD::GenerateBitSet(exponent,LD::CT::Range<0,11>{});

        std::cout << "Exponent : " << exponent << std::endl;
        std::cout << "Exponent part " << "\n";
        for(LD::UInteger n = 0;n<11;++n)
        {
            std::cout << exponentPart[n];
        }
        std::cout << "\n";

        floatingPointNumber[0] = (input < 0);
        for(LD::UInteger n = 0;n<11;++n)
        {
            floatingPointNumber[n+1] = exponentPart[n];
        }
        for(LD::UInteger n = 0;n<52;++n)
        {
            floatingPointNumber[n+12] = mantissa[n];
        }


        return floatingPointNumber;
    }
}
/**
 * void toBinary(const double & n)
{
    double floatingPoint = LD::Abs(n) - LD::Abs((LD::UInteger)n);

    char buffer[23] = {'0'};
    for (LD::UInteger n = 0;  n < 23; ++n)
    {
        floatingPoint = floatingPoint*2;
        buffer[n] = (floatingPoint < 1)*'0' + (floatingPoint >= 1)*'1';
        floatingPoint = (floatingPoint)-LD::UInteger(floatingPoint);
    }
    for (LD::UInteger n= 0;  n<23 ; ++n)
    {
        std::cout << buffer[n];
    }

    std::cout << std::endl;

}
 */
#endif //LANDESSDEVCORE_COMPLIMENTGENERATOR_H
