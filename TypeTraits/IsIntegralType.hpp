//
// Created by James Landess on 2/4/20.
//

#ifndef LANDESSDEVCORE_ISINTEGRALTYPE_HPP
#define LANDESSDEVCORE_ISINTEGRALTYPE_HPP
namespace LD
{
    namespace Detail
    {
        template<typename T>
        struct IsCharacter
        {
            static const bool value = false;
        };



        template<>
        struct IsCharacter<char>
        {
            static const bool value = true;
        };


        template<typename T> using IsCharacter_V = typename LD::Detail::IsCharacter<T>::value;


        template<typename T>
        struct IsUnsignedCharacter
        {
            static const bool value = false;
        };


        template<>
        struct IsUnsignedCharacter<unsigned char>
        {
            static const bool value = true;
        };


        template<typename T> using IsUnsignedCharacter_V = typename IsUnsignedCharacter<T>::value;


        template<typename T>
        struct Is16BitInteger
        {
            static const bool value = false;
        };

        template<>
        struct Is16BitInteger<short>
        {
            static const bool value = true;
        };



        template<typename T> using Is16BitInteger_V = typename Is16BitInteger<T>::value;


        template<typename T>
        struct Is16Extended32BitInteger
        {
            static const bool value = false;
        };

        template<>
        struct Is16Extended32BitInteger<short int>
        {
            static const bool value = false;
        };

        template<typename T> using Is16Extended32BitInteger_V = typename Is16Extended32BitInteger<T>::value;

        template<typename T>
        struct IsUnsigned16BitInteger
        {
            static const bool value = false;
        };

        template<>
        struct IsUnsigned16BitInteger<unsigned short>
        {
            static const bool value = true;
        };


        template<typename T> using IsUnsigned16BitInteger_V = typename IsUnsigned16BitInteger<T>::value;

        template<typename T>
        struct IsUnsigned16BitExtended32BitInteger
        {
            static const bool value = false;
        };

        template<>
        struct IsUnsigned16BitExtended32BitInteger<unsigned short int>
        {
            static const bool value = true;
        };

        template<typename T> using IsUnsigned16BitExtended32BitInteger_V = typename IsUnsigned16BitExtended32BitInteger<T>::value;

        template<typename T>
        struct Is32BitInteger
        {
            static const bool value = false;
        };

        template<>
        struct Is32BitInteger<int>
        {
            static const bool value = true;
        };

        template<typename T> using Is32BitInteger_V = typename Is32BitInteger<T>::value;


        template<typename T>
        struct IsUnsigned32BitInteger
        {
            static const bool value =false;
        };


        template<>
        struct IsUnsigned32BitInteger<unsigned int>
        {
            static const bool value = true;
        };


        template<typename T> using IsUnsigned32BitInteger_V = typename IsUnsigned32BitInteger<T>::value;


        template<typename T>
        struct Is64BitInteger
        {
            static const bool value = false;
        };

        template<>
        struct Is64BitInteger<long>
        {
            static const bool value = true;
        };

        template<typename T> using Is64BitInteger_V = typename Is64BitInteger<T>::value;

        template<typename T>
        struct IsUnsigned64BitInteger
        {
            static const bool value = false;
        };

        template<>
        struct IsUnsigned64BitInteger<unsigned long>
        {
            static const bool value = true;
        };

        template<typename T> using IsUnsigned64BitInteger_V = typename IsUnsigned64BitInteger<T>::value;


        template<typename T>
        struct Is64Bit32BitExtended
        {
            static const bool value = false;
        };

        template<>
        struct Is64Bit32BitExtended<long int>
        {
            static const bool value = true;
        };


        template<typename T> using Is64Bit32BitExtended_V = typename Is64Bit32BitExtended<T>::value;



        template<typename T>
        struct IsUnsigned64Bit32BitExtended
        {
            static const bool value = false;
        };

        template<>
        struct IsUnsigned64Bit32BitExtended<unsigned long int>
        {
            static const bool value = true;
        };


        template<typename T> using IsUnsigned64Bit32BitExtended_V = typename IsUnsigned64Bit32BitExtended<T>::value;

        template<typename T>
        struct Is64Bit64BitExtended
        {
            static const bool value = false;
        };

        template<>
        struct Is64Bit64BitExtended<long long>
        {
            static const bool value = true;
        };

        template<typename T> using Is64Bit64BitExtended_V = typename Is64Bit64BitExtended<T>::value;

        template<typename T>
        struct IsUnsigned64Bit64BitExtended
        {
            static const bool value = false;
        };

        template<>
        struct IsUnsigned64Bit64BitExtended<unsigned long long>
        {
            static const bool value = true;
        };

        template<typename T> using IsUnsigned64Bit64BitExtended_V = typename IsUnsigned64Bit64BitExtended<T>::value;


        template<typename T>
        struct Is64Bit64Bit32BitExtended
        {
            static const bool value = false;
        };
        template<>
        struct Is64Bit64Bit32BitExtended<long long int>
        {
            static const bool value = true;
        };

        template<typename T> using Is64Bit64Bit32BitExtended_V = typename Is64Bit64Bit32BitExtended<T>::value;

        template<typename T>
        struct IsUnsigned64Bit64Bit32BitExtended
        {
            static const bool value = false;
        };
        template<>
        struct IsUnsigned64Bit64Bit32BitExtended<unsigned long long int>
        {
            static const bool value = true;
        };

        template<typename T> using IsUnsigned64Bit64Bit32BitExtended_V = typename IsUnsigned64Bit64Bit32BitExtended<T>::value;

        template<typename T>
        struct IsBoolean
        {
            static const bool value = false;
        };

        template<>
        struct IsBoolean<bool>
        {
            static const bool value = true;
        };


        template<typename T> using IsBoolean_V = typename IsBoolean<T>::value;


        template<typename T>
        struct IsDouble
        {
            static const bool value = false;
        };


        template<>
        struct IsDouble<double>
        {
            static const bool value = true;
        };

        template<typename T> using IsDouble_V = typename IsDouble<T>::value;


        template<typename T>
        struct IsFloat
        {
            static const bool value = false;
        };





        template<>
        struct IsFloat<float>
        {
            static const bool value = true;
        };

        template<typename T> using IsFloat_V = typename IsFloat<T>::value;


        template<typename T>
        struct IsDoubleExtended64Bit
        {
            static const bool value = false;
        };

        template<>
        struct IsDoubleExtended64Bit<long double>
        {
            static const bool value = true;
        };

        template<typename T> using IsDoubleExtended64Bit_V = typename IsDoubleExtended64Bit<T>::value;


        template<typename T>
        struct IsIntegrelType
        {
            static const bool value =
                    LD::Detail::IsCharacter<T>::value ||
                    LD::Detail::IsUnsignedCharacter<T>::value ||
                    LD::Detail::Is16BitInteger<T>::value ||
                    LD::Detail::IsUnsigned16BitInteger<T>::value ||
                    LD::Detail::Is16Extended32BitInteger<T>::value ||
                    LD::Detail::IsUnsigned16BitExtended32BitInteger<T>::value ||
                    LD::Detail::Is32BitInteger<T>::value ||
                    LD::Detail::IsUnsigned32BitInteger<T>::value ||
                    LD::Detail::Is64BitInteger<T>::value ||
                    LD::Detail::IsUnsigned64BitInteger<T>::value ||
                    LD::Detail::Is64Bit32BitExtended<T>::value ||
                    LD::Detail::IsUnsigned64Bit32BitExtended<T>::value ||
                    LD::Detail::Is64Bit64BitExtended<T>::value ||
                    LD::Detail::IsUnsigned64Bit64BitExtended<T>::value ||
                    LD::Detail::Is64Bit64Bit32BitExtended<T>::value ||
                    LD::Detail::IsUnsigned64Bit64Bit32BitExtended<T>::value ||
                    LD::Detail::IsBoolean<T>::value ||
                    LD::Detail::IsDouble<T>::value ||
                    LD::Detail::IsFloat<T>::value ||
                    LD::Detail::IsDoubleExtended64Bit<T>::value;
        };

        template<typename T>
        struct IsInteger
        {
            static const bool value =
                    LD::Detail::Is16BitInteger<T>::value||
                    LD::Detail::Is16Extended32BitInteger<T>::value||
                    LD::Detail::Is32BitInteger<T>::value||
                    LD::Detail::Is64BitInteger<T>::value||
                    LD::Detail::Is64Bit32BitExtended<T>::value||
                    LD::Detail::Is64Bit64BitExtended<T>::value||
                    LD::Detail::Is64Bit64Bit32BitExtended<T>::value;
        };

        template<typename T>
        struct Is32BitSignedInteger
        {
            static const bool value = Is32BitInteger<T>::value;
        };

        template<typename T>
        struct Is16BitSignedInteger
        {
            static const bool value =
                    LD::Detail::Is16BitInteger<T>::value||
                    LD::Detail::Is16Extended32BitInteger<T>::value;
        };

        template<typename T>
        struct Is64BitSignedInteger
        {
            static const bool value =
                    LD::Detail::Is64BitInteger<T>::value||
                    LD::Detail::Is64Bit32BitExtended<T>::value||
                    LD::Detail::Is64Bit64BitExtended<T>::value||
                    LD::Detail::Is64Bit64Bit32BitExtended<T>::value;
        };


        template<typename T>
        struct IsFloatingPoint
        {
            static const bool value =
                    LD::Detail::IsDouble<T>::value ||
                    LD::Detail::IsFloat<T>::value ||
                    LD::Detail::IsDoubleExtended64Bit<T>::value;
        };


        template<typename T>
        struct Is32BitFloatingPoint
        {
            static const bool value =IsFloat<T>::value;
        };

        template<typename T>
        struct Is64BitFloatingPoint
        {
            static const bool value =
                    LD::Detail::IsDouble<T>::value ||
                    LD::Detail::IsDoubleExtended64Bit<T>::value;
        };


        template<typename T>
        struct IsUnsignedInteger
        {
            static const bool value =
                    LD::Detail::IsUnsigned16BitInteger<T>::value||
                    LD::Detail::IsUnsigned16BitExtended32BitInteger<T>::value||
                    LD::Detail::IsUnsigned32BitInteger<T>::value ||
                    LD::Detail::IsUnsigned64BitInteger<T>::value ||
                    LD::Detail::IsUnsigned64Bit32BitExtended<T>::value||
                    LD::Detail::IsUnsigned64Bit64BitExtended<T>::value ||
                    LD::Detail::IsUnsigned64Bit64Bit32BitExtended<T>::value;
        };


        template<typename T>
        struct Is32BitUnsignedInteger
        {
            static const bool value =
                    LD::Detail::IsUnsigned16BitInteger<T>::value||
                    LD::Detail::IsUnsigned16BitExtended32BitInteger<T>::value||
                    LD::Detail::IsUnsigned32BitInteger<T>::value;
        };

        template<typename T>
        struct Is64BitUnsignedInteger
        {
            static const bool value =
                    LD::Detail::IsUnsigned64BitInteger<T>::value ||
                    LD::Detail::IsUnsigned64Bit32BitExtended<T>::value||
                    LD::Detail::IsUnsigned64Bit64BitExtended<T>::value ||
                    LD::Detail::IsUnsigned64Bit64Bit32BitExtended<T>::value;
        };

        template<typename T> using IsIntegrelType_T = typename IsIntegrelType<T>::value;


        template<typename T>
        struct IsPrimitiveType
        {
            static const bool value =
                    LD::Detail::IsCharacter<T>::value ||
                    LD::Detail::IsUnsignedCharacter<T>::value ||
                    LD::Detail::Is16BitInteger<T>::value ||
                    LD::Detail::IsUnsigned16BitInteger<T>::value ||
                    LD::Detail::Is16Extended32BitInteger<T>::value ||
                    LD::Detail::IsUnsigned16BitExtended32BitInteger<T>::value ||
                    LD::Detail::Is32BitInteger<T>::value ||
                    LD::Detail::IsUnsigned32BitInteger<T>::value ||
                    LD::Detail::Is64BitInteger<T>::value ||
                    LD::Detail::IsUnsigned64BitInteger<T>::value ||
                    LD::Detail::Is64Bit32BitExtended<T>::value ||
                    LD::Detail::IsUnsigned64Bit32BitExtended<T>::value ||
                    LD::Detail::Is64Bit64BitExtended<T>::value ||
                    LD::Detail::IsUnsigned64Bit64BitExtended<T>::value ||
                    LD::Detail::Is64Bit64Bit32BitExtended<T>::value ||
                    LD::Detail::IsUnsigned64Bit64Bit32BitExtended<T>::value ||
                    LD::Detail::IsBoolean<T>::value ||
                    LD::Detail::IsDouble<T>::value ||
                    LD::Detail::IsFloat<T>::value ||
                    LD::Detail::IsDoubleExtended64Bit<T>::value;
        };


        template<typename T> using IsPrimitiveType_V = typename IsPrimitiveType<T>::value;
    }
}
#endif //LANDESSDEVCORE_ISINTEGRALTYPE_HPP
