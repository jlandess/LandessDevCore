//
//  CommonAlgorithms.h
//  DataStructures
//
//  Created by James Landess on 10/17/14.
//  Copyright (c) 2014 LandessDev. All rights reserved.
//

#ifndef DataStructures_CommonAlgorithms_h
#define DataStructures_CommonAlgorithms_h

#include <climits>
#include "Float.hpp"
#include "Integer.hpp"
#include "TypeTraits/RemoveReference.hpp"
#include "TypeTraits/RemoveExtent.hpp"
#include "TypeTraits/IntegralConstant.hpp"
#include "TypeTraits/IsIntegralType.hpp"
#include "TypeTraits/IsClassType.hpp"
#include "TypeTraits/IsUnion.hpp"
#include "TypeTraits/IsReference.hpp"
#include "TypeTraits/RemoveCV.hpp"
#include "TypeTraits/IsNullPtr.hpp"
#include "TypeTraits/IsVoid.hpp"
#include "TypeTraits/IsArray.hpp"
#include "TypeTraits/IsSame.hpp"
#include "TypeTraits/AddPointer.hpp"
#include "TypeTraits/Conditional.hpp"
#include "TypeTraits/Decay.hpp"
#include "TypeTraits/IsConst.hpp"
#include "TypeTraits/IntegerSequence.hpp"
#include "TypeTraits/Limit.hpp"
#include "TypeTraits/Declval.hpp"
#include "TypeTraits/RemoveConst.hpp"
#include "TypeTraits/RemoveAllExtents.hpp"
#include "TypeTraits/IsDestructible.hpp"
#include "TypeTraits/IsAssignable.hpp"
#include "TypeTraits/IsVolatile.hpp"
#include "TypeTraits/AddLValueReference.hpp"
#include "TypeTraits/AddRValueReference.hpp"
#include "TypeTraits/IsClassType.hpp"
#include "TypeTraits/IsPointer.hpp"
#include "TypeTraits/EnableIf.hpp"
#include "TypeTraits/IsBaseOf.hpp"
#include "TypeTraits/IsSwappable.hpp"
#include "TypeTraits/IsConstructible.hpp"

#ifndef _LIBCPP_STD_VER
#  if  __cplusplus <= 201103L
#    define _LIBCPP_STD_VER 11
#  elif __cplusplus <= 201402L
#    define _LIBCPP_STD_VER 14
#  elif __cplusplus <= 201703L
#    define _LIBCPP_STD_VER 17
#  else
#    define _LIBCPP_STD_VER 18  // current year, or date of c++2a ratification
#  endif
#endif  // _LIBCPP_STD_VER

#if (__has_feature(cxx_noexcept))
#  define _NOEXCEPT noexcept
#  define _NOEXCEPT_(x) noexcept(x)
#  define _NOEXCEPT_OR_FALSE(x) noexcept(x)
#else
#  define _NOEXCEPT throw()
#  define _NOEXCEPT_(x)
#  define _NOEXCEPT_OR_FALSE(x) false
#endif


#define MAXIMUM_FLOAT_DECIMAL_POINTS 23
#define MAXIMUM_DOUBLE_DECIMAL_POINTS 56
namespace PDP
{






#if !defined(SIMDPP_ARCH_X86_SSE2) && !defined(PDP_SIMD_X86_SSE3) && !defined(PDP_SIMD_X86_SSE4_1) && !defined(PDP_SIMD_X86_AVX) && !defined(PDP_SIMD_X86_AVX2) && !defined(PDP_SIMD_X86_FMA3) && !defined(PDP_SIMD_X86_FMA4) && !defined(PDP_SIMD_X86_XOP) && !defined(PDP_SIMD_X86_ARM_NEON) && !defined(PDP_SIMD_NEON_FLT_SP) && !defined(PDP_SIMD_POWER_ALTIVEC)
#define PDP_SIMD_NONE
#endif


#ifdef PDP_SIMD_NONE

    struct SIMDStatus: public LD::FalseType
    {

    };
#elif defined(PDP_SIMD_X86_SSE2)
    #define SIMDPP_ARCH_X86_SSE2 1
    struct SIMDStatus: public PDP::TrueType
    {

    };

#elif defined(PDP_SIMD_X86_SSE3)
#define SIMDPP_ARCH_X86_SSE3 1
    struct SIMDStatus: public PDP::TrueType
    {

    };

#elif defined(PDP_SIMD_X86_SSE4_1)
#define SIMDPP_ARCH_X86_SSE4_1 1
    struct SIMDStatus: public PDP::TrueType
    {

    };
#elif defined(PDP_SIMD_X86_AVX)
#define SIMDPP_ARCH_X86_AVX
    struct SIMDStatus: public PDP::TrueType
    {

    };

#elif defined(PDP_SIMD_X86_AVX2)
#define SIMDPP_ARCH_X86_AVX2 1
    struct SIMDStatus: public PDP::TrueType
    {

    };

#elif defined(PDP_SIMD_X86_FMA3)
#define SIMD_ARCH_X86_FMA3 1
    struct SIMDStatus: public PDP::TrueType
    {

    };

#elif defined(PDP_SIMD_X86_FMA4)
#define SIMD_ARCH_X86_FMA4 1
    struct SIMDStatus: public PDP::TrueType
    {

    };
#elif defined(PDP_SIMD_X86_XOP)
#define SIMD_ARCH_X86_XOP 1
    struct SIMDStatus: public PDP::TrueType
    {

    };

#elif defined(PDP_SIMD_X86_ARM_NEON)
#define SIMD_ARCH_ARM_NEON 1
    struct SIMDStatus: public PDP::TrueType
    {

    };
#elif defined(PDP_SIMD_NEON_FLT_SP)
#define SIMD_ARCH_ARM_NEON_FLT_SP 1
    struct SIMDStatus: public PDP::TrueType
    {

    };
#elif defined(PDP_SIMD_POWER_ALTIVEC)
#define SIMDPP_ARCH_POWER_ALTIVEC 1
    struct SIMDStatus: public PDP::TrueType
    {

    };
#endif

    template <bool, class _Tp = void> struct  EnableIf {};
    template <class _Tp> struct  EnableIf<true, _Tp> {typedef _Tp type;};

#if _LIBCPP_STD_VER > 11
    template <bool _Bp, class _Tp = void> using Enable_If_T = typename EnableIf<_Bp, _Tp>::type;
#endif



template <class _Tp> struct  IsVolatile               : public LD::FalseType {};
template <class _Tp> struct  IsVolatile<_Tp volatile> : public LD::TrueType {};


template <class _Tp, class _Up> struct  IsSame           : public LD::FalseType {};
template <class _Tp>            struct  IsSame<_Tp, _Tp> : public LD::TrueType {};
template <class _Tp, class _Up> struct  IsA           : public LD::FalseType {};
template <class _Tp>            struct  IsA<_Tp, _Tp> : public LD::TrueType {};


#if _LIBCPP_STD_VER > 11
    template <class First, class Second> using IsSame_V = typename IsSame<First,Second>::value;
#endif


    template<class _Ty>
    struct _Remove_reference
    {   // remove reference
        typedef _Ty _Type;
    };



    template<class _Ty>
    struct _Remove_reference<_Ty&>
    {   // remove reference
        typedef _Ty _Type;
    };

    template<class _Ty>
    struct _Remove_reference<_Ty&&>
    {   // remove rvalue reference
        typedef _Ty _Type;
    };

    //typename _Remove_reference<_Ty>::_Type&&
    template<class _Ty> inline PDP::Enable_If_T<!PDP::IsSame<_Ty, void>::value,typename _Remove_reference<_Ty>::_Type&&> Move(_Ty&& _Arg)
    {   // forward _Arg as movable
        return ((typename _Remove_reference<_Ty>::_Type&&)_Arg);
    }


    template <class _Tp> struct  RemoveReference        {typedef _Tp type;};


#if _LIBCPP_STD_VER > 11
    template <class _Tp> using RemoveReference_T = typename RemoveReference<_Tp>::type;
#endif

    template <class _Tp>
    inline
    _Tp&& Forward(typename RemoveReference<_Tp>::type& __t) _NOEXCEPT
    {
        return static_cast<_Tp&&>(__t);
    }




    namespace Detail
    {
        template <class F, class... Args>
        inline auto INVOKE(F&& f, Args&&... args) ->
        decltype(Forward<F>(f)(Forward<Args>(args)...))
        {
            return Forward<F>(f)(Forward<Args>(args)...);
        }

        template <class Base, class T, class Derived>
        inline auto INVOKE(T Base::*pmd, Derived&& ref) ->
        decltype(Forward<Derived>(ref).*pmd)
        {
            return Forward<Derived>(ref).*pmd;
        }

        template <class PMD, class Pointer>
        inline auto INVOKE(PMD&& pmd, Pointer&& ptr) ->
        decltype((*Forward<Pointer>(ptr)).*Forward<PMD>(pmd))
        {
            return (*Forward<Pointer>(ptr)).*Forward<PMD>(pmd);
        }

        template <class Base, class T, class Derived, class... Args>
        inline auto INVOKE(T Base::*pmf, Derived&& ref, Args&&... args) ->
        decltype((Forward<Derived>(ref).*pmf)(Forward<Args>(args)...))
        {
            return (Forward<Derived>(ref).*pmf)(Forward<Args>(args)...);
        }

        template <class PMF, class Pointer, class... Args>
        inline auto INVOKE(PMF&& pmf, Pointer&& ptr, Args&&... args) ->
        decltype(((*Forward<Pointer>(ptr)).*Forward<PMF>(pmf))(Forward<Args>(args)...))
        {
            return ((*Forward<Pointer>(ptr)).*Forward<PMF>(pmf))(Forward<Args>(args)...);
        }
    } // namespace detail


    template <class _Tp> struct  AddRvalueReference                     {typedef _Tp&& type;};
    template< class T > typename AddRvalueReference<T>::type Declval();

    template <class> struct ResultOf;
    template <class F, class... ArgTypes>
    struct ResultOf<F(ArgTypes...)>
    {
        using type = decltype(Detail::INVOKE(Declval<F>(),Declval<ArgTypes>()...));
    };


#if _LIBCPP_STD_VER > 11
    template <class F, class... ArgTypes> using ResultOf_T = typename ResultOf<F(ArgTypes...)>::type;
#endif

    enum TreeTraversal{ePreOrder,eInOrder,ePostOrder,eReverseOrder,eLevel,eBreadthFirst};
    enum HeapType{eMaxHeap = 0, eMinHeap = 1};
    enum LinearTraveral{eFoward,eBackward};

    inline void DummyFunction()
    {

    }

    template<typename T>
    inline T Abs(const T & integer)
    {
        return (integer<0)?integer*-1:integer;
    }
/*
    inline UInteger Max( UInteger lhs, UInteger rhs )
    {
        return lhs > rhs ? lhs : rhs;
    }
 */
    template<typename T> class MaximumDecimalPlaces
    {
    public:
        static LD::UInteger GetMaximumAmountOfDecimalPlaces()
        {
            return 0;
        }
    };

    template<> class MaximumDecimalPlaces<float>
    {
    public:
        static LD::UInteger GetMaximumAmountOfDecimalPlaces()
        {
            return MAXIMUM_FLOAT_DECIMAL_POINTS;
        }
    };
    template<> class MaximumDecimalPlaces<double>
    {
    public:
        static LD::UInteger GetMaximumAmountOfDecimalPlaces()
        {
            return MAXIMUM_FLOAT_DECIMAL_POINTS;
        }
    };


    template<typename T>
    class Limit;

    template<>
    class Limit<short>
    {
    public:


        static inline short GetMin()
        {
            return SHRT_MIN;
        }


        static inline short GetMax()
        {
            return SHRT_MAX;
        }
    };

    template<>
    class Limit<unsigned short>
    {
    public:


        static inline unsigned short GetMin()
        {
            return 0;
        }


        static inline unsigned short GetMax()
        {
            return USHRT_MAX;
        }
    };



    template<>
    class Limit<int>
    {
    public:


        static inline int GetMin()
        {
            return INT_MIN;
        }


        static inline int GetMax()
        {
            return INT_MAX;
        }
    };


    template<>
    class Limit<unsigned int>
    {
    public:


        static inline unsigned int GetMin()
        {
            return 0;
        }


        static inline unsigned int GetMax()
        {
            return UINT_MAX;
        }
    };


    template<>
    class Limit<long>
    {
    public:


        static inline long GetMin()
        {
            return LONG_MIN;
        }


        static inline long GetMax()
        {
            return LONG_MAX;
        }
    };




    template<>
    class Limit<unsigned long>
    {
    public:


        static inline unsigned long GetMin()
        {
            return 0;
        }


        static inline unsigned long GetMax()
        {
            return ULONG_MAX;
        }
    };


    template<>
    class Limit<long long int>
    {
    public:


        static inline long long int GetMin()
        {
            return LLONG_MIN;
        }


        static inline long long int GetMax()
        {
            return  LLONG_MAX;
        }
    };


    template<>
    class Limit<unsigned long long int>
    {
    public:


        static inline unsigned long long int GetMin()
        {
            return 0;
        }


        static inline unsigned long long int GetMax()
        {
            return  ULLONG_MAX;
        }
    };


    template<>
    class Limit<float>
    {
    public:


        static inline float GetMin()
        {
            return 1.17549e-38;
        }


        static inline float GetMax()
        {
            return 3.40282e+38;
        }
    };


    template<>
    class Limit<double>
    {
    public:


        static inline double GetMin()
        {
            return 2.22507e-308;
        }


        static inline double GetMax()
        {
            return 1.79769e+308;
        }
    };

    template<>
    class Limit<long double>
    {
    public:


        static inline long double GetMin()
        {

            return 4.9406564584124654E-324;
        }


        static inline long double GetMax()
        {
            return 1.7976931348623157E+308;
        }
    };
    inline bool IsNan(const float & f)
    {
        const unsigned int u = *(unsigned int*)&f;
        return (u&0x7F800000) == 0x7F800000 && (u&0x7FFFFF);    // Both NaN and qNan.
    }

    inline bool IsNan(const double & d)
    {
        const unsigned long u = *(unsigned int*)&d;
        return (u&0x7FF0000000000000ULL) == 0x7FF0000000000000ULL && (u&0xFFFFFFFFFFFFFULL);
    }


    inline LD::UInteger FastLog10(LD::UInteger v)
    {
        return (v >= 1000000000u) ? 9 : (v >= 100000000u) ? 8 :
                                        (v >= 10000000u) ? 7 : (v >= 1000000u) ? 6 :
                                                               (v >= 100000u) ? 5 : (v >= 10000u) ? 4 :
                                                                                    (v >= 1000u) ? 3 : (v >= 100u) ? 2 : (v >= 10u) ? 1u : 0u;
    }
    inline LD::UInteger FastAbs(LD::Integer integer)
    {
        LD::Integer value = integer;
        LD::Integer temp = value >> 31;     // make a mask of the sign bit
        value ^= temp;                   // toggle the bits if value is negative
        value += temp & 1;
        return value;
    }
    inline LD::Integer Floor(LD::Float x)
    {
        if(x>0)return (LD::Integer)x;
        return (LD::Integer)(x-((LD::Float)0.9999999999999999));
    }
    inline LD::UInteger Power(const LD::UInteger & x, const LD::UInteger & y)
    {
        LD::UInteger temp;
        if( y == 0)
            return 1;
        temp = PDP::Power(x, y/2);
        if (y%2 == 0)
            return temp*temp;
        else
            return x*temp*temp;
    }
    inline LD::Float Power(const LD::Float & x, const LD::Integer & y)
    {
        LD::Float temp;
        if( y == 0)
            return 1;
        temp = PDP::Power(x, y/2);
        if (y%2 == 0)
            return temp*temp;
        else
        {
            if(y > 0)
                return x*temp*temp;
            else
                return (temp*temp)/x;
        }
    }



    inline LD::UInteger GetDecimalPlaces(LD::Float dbVal)
    {
        static const LD::UInteger MAX_DP = MaximumDecimalPlaces<LD::Float>::GetMaximumAmountOfDecimalPlaces();
        static const LD::Float THRES = Power((LD::Float)0.1, (LD::Integer)MAX_DP);
        if (dbVal == 0.0)
            return 0;
        LD::UInteger nDecimal = 0;
        while (dbVal - PDP::Floor(dbVal) > THRES && nDecimal < MAX_DP)
        {
            dbVal *= 10.0;
            nDecimal++;
        }
        return nDecimal;
    }
    inline LD::UInteger ceil_log2(LD::UInteger x)
    {
        static const unsigned long long t[6] = {
                0xFFFFFFFF00000000ull,
                0x00000000FFFF0000ull,
                0x000000000000FF00ull,
                0x00000000000000F0ull,
                0x000000000000000Cull,
                0x0000000000000002ull
        };

        LD::Integer y = (((x & (x - 1)) == 0) ? 0 : 1);
        LD::Integer j = 32;
        LD::Integer i;

        for (i = 0; i < 6; i++)
        {
            LD::Integer k = (((x & t[i]) == 0) ? 0 : j);
            y += k;
            x >>= k;
            j >>= 1;
        }

        return y;
    }



    template <LD::UInteger N, LD::UInteger base=2>
    struct CompileTimeLog
    {
        enum { value = 1 + CompileTimeLog<N/base, base>::value };
    };

    template <LD::UInteger base>
    struct CompileTimeLog<1, base>
    {
        enum { value = 0 };
    };

    template <LD::UInteger base>
    struct CompileTimeLog<0, base>
    {
        enum { value = 0 };
    };


    template<LD::UInteger Size> using CompileTimeLog_V = typename CompileTimeLog<Size>::value;


    template <LD::UInteger A, LD::UInteger B>
    struct CompileTimeMax
    {
        static const LD::UInteger value = (A > B)?A:B;
    };


    template <LD::UInteger A, LD::UInteger B>
    struct CompileTimePow
    {
        static const LD::UInteger value = A * CompileTimePow<A, B - 1>::value;
    };
    template <LD::UInteger A>
    struct CompileTimePow<A, 0>
    {
        static const LD::UInteger value = 1;
    };

    template<LD::UInteger Size, LD::UInteger Alignment> using CompileTimePow_V = typename CompileTimePow<Size,Alignment>::value;



    template<LD::UInteger Primary, LD::UInteger Secondary>
    struct CompileTimeModulus
    {
        static const bool value = !(Primary%Secondary);
    };

    template<LD::UInteger Size, LD::UInteger Alignment> using CompileTimeModulus_V = typename CompileTimeModulus<Size,Alignment>::value;

    template<LD::UInteger Primary, LD::UInteger Secondary>
    struct CompileTimeDivision
    {
        static const LD::UInteger value = Primary/Secondary;
    };

    template<LD::UInteger Size, LD::UInteger Alignment> using CompileTimeDivision_V = typename CompileTimeDivision<Size,Alignment>::value;

    template<LD::UInteger Primary, LD::UInteger Secondary>
    struct CompileTimeMultiplication
    {
        static const LD::UInteger value = Primary*Secondary;
    };

    template<LD::UInteger Size, LD::UInteger Alignment> using CompileTimeMultiplication_V = typename CompileTimeMultiplication<Size,Alignment>::value;

    template<LD::UInteger Size>
    struct DetermineAlignedSize
    {
        static const LD::UInteger value = (Size<2 || Size&(Size-1))?CompileTimePow<2, CompileTimeLog<Size,2>::value+1>::value:Size;
    };

    template<LD::UInteger Size, LD::UInteger Alignment = 8>
    struct CompileTimeAlignmentSize
    {
        static const LD::UInteger value = PDP::CompileTimeModulus<Size, Alignment>::value?Size:PDP::CompileTimeMultiplication<Alignment, PDP::CompileTimeDivision<Size, Alignment>::value+1>::value;
    };


    template<LD::UInteger Size, LD::UInteger Alignment> using CompileTimeAlignmentSize_V = typename CompileTimeAlignmentSize<Size,Alignment>::value;








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


    template<typename T> using IsCharacter_V = typename IsCharacter<T>::value;


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
        static const bool value = IsCharacter<T>::value || IsUnsignedCharacter<T>::value ||
                                  Is16BitInteger<T>::value || IsUnsigned16BitInteger<T>::value ||
                                  Is16Extended32BitInteger<T>::value ||IsUnsigned16BitExtended32BitInteger<T>::value ||
                                  Is32BitInteger<T>::value || IsUnsigned32BitInteger<T>::value ||
                                  Is64BitInteger<T>::value || IsUnsigned64BitInteger<T>::value ||
                                  Is64Bit32BitExtended<T>::value || IsUnsigned64Bit32BitExtended<T>::value ||
                                  Is64Bit64BitExtended<T>::value || IsUnsigned64Bit64BitExtended<T>::value ||
                                  Is64Bit64Bit32BitExtended<T>::value || IsUnsigned64Bit64Bit32BitExtended<T>::value ||
                                  IsBoolean<T>::value || IsDouble<T>::value || IsFloat<T>::value || IsDoubleExtended64Bit<T>::value;
    };

    template<typename T>
    struct IsInteger
    {
        static const bool value = Is16BitInteger<T>::value||Is16Extended32BitInteger<T>::value||
                                  Is32BitInteger<T>::value||Is64BitInteger<T>::value||
                                  Is64Bit32BitExtended<T>::value||Is64Bit64BitExtended<T>::value||
                                  Is64Bit64Bit32BitExtended<T>::value;
    };

    template<typename T>
    struct Is32BitSignedInteger
    {
        static const bool value = Is32BitInteger<T>::value;
    };

    template<typename T>
    struct Is16BitSignedInteger
    {
        static const bool value = Is16BitInteger<T>::value||Is16Extended32BitInteger<T>::value;
    };

    template<typename T>
    struct Is64BitSignedInteger
    {
        static const bool value = Is64BitInteger<T>::value||
                                  Is64Bit32BitExtended<T>::value||Is64Bit64BitExtended<T>::value||
                                  Is64Bit64Bit32BitExtended<T>::value;
    };


    template<typename T>
    struct IsFloatingPoint
    {
        static const bool value =IsDouble<T>::value || IsFloat<T>::value || IsDoubleExtended64Bit<T>::value;
    };


    template<typename T>
    struct Is32BitFloatingPoint
    {
        static const bool value =IsFloat<T>::value;
    };

    template<typename T>
    struct Is64BitFloatingPoint
    {
        static const bool value =IsDouble<T>::value || IsDoubleExtended64Bit<T>::value;
    };


    template<typename T>
    struct IsUnsignedInteger
    {
        static const bool value =IsUnsigned16BitInteger<T>::value||IsUnsigned16BitExtended32BitInteger<T>::value||
                                 IsUnsigned32BitInteger<T>::value || IsUnsigned64BitInteger<T>::value || IsUnsigned64Bit32BitExtended<T>::value||
                                 IsUnsigned64Bit64BitExtended<T>::value || IsUnsigned64Bit64Bit32BitExtended<T>::value;
    };


    template<typename T>
    struct Is32BitUnsignedInteger
    {
        static const bool value =IsUnsigned16BitInteger<T>::value||IsUnsigned16BitExtended32BitInteger<T>::value||
                                 IsUnsigned32BitInteger<T>::value;
    };

    template<typename T>
    struct Is64BitUnsignedInteger
    {
        static const bool value =IsUnsigned64BitInteger<T>::value || IsUnsigned64Bit32BitExtended<T>::value||
                                 IsUnsigned64Bit64BitExtended<T>::value || IsUnsigned64Bit64Bit32BitExtended<T>::value;
    };

    template<typename T> using IsIntegrelType_T = typename IsIntegrelType<T>::value;


    template<typename T>
    struct IsPrimitiveType
    {
        static const bool value = IsCharacter<T>::value || IsUnsignedCharacter<T>::value ||
                                  Is16BitInteger<T>::value || IsUnsigned16BitInteger<T>::value ||
                                  Is16Extended32BitInteger<T>::value ||IsUnsigned16BitExtended32BitInteger<T>::value ||
                                  Is32BitInteger<T>::value || IsUnsigned32BitInteger<T>::value ||
                                  Is64BitInteger<T>::value || IsUnsigned64BitInteger<T>::value ||
                                  Is64Bit32BitExtended<T>::value || IsUnsigned64Bit32BitExtended<T>::value ||
                                  Is64Bit64BitExtended<T>::value || IsUnsigned64Bit64BitExtended<T>::value ||
                                  Is64Bit64Bit32BitExtended<T>::value || IsUnsigned64Bit64Bit32BitExtended<T>::value ||
                                  IsBoolean<T>::value || IsDouble<T>::value || IsFloat<T>::value || IsDoubleExtended64Bit<T>::value;
    };


    template<typename T> using IsPrimitiveType_V = typename IsPrimitiveType<T>::value;

    template <typename B, typename D>
    struct Host
    {
        operator B*() const;
        operator D*();
    };

    template <typename B, typename D>
    struct IsBaseOf
    {
        typedef char (&yes)[1];
        typedef char (&no)[2];

        template <typename T>
        static yes check(D*, T);
        static no check(B*, int);

        static const bool value = sizeof(check(Host<B,D>(), int())) == sizeof(yes);
    };


    template<typename B, typename D>  using IsBaseOf_V = typename IsBaseOf<B,D>::value;











    //template<typename B, typename D> using IsSame_V = typename IsSame<B,D>::value;




    template<typename B, typename D>
    class IsKindOf
    {
    public:
        static const bool value = PDP::IsBaseOf<B, D>::value || PDP::IsSame<B, D>::value;
    };


    template<typename B, typename D>  using IsKindOf_V = typename IsKindOf<B,D>::value;
    template<typename T>
    struct IsClassType
    {
        static const bool value = !IsIntegrelType<T>::value;
    };

    //template <bool _Bp, class _Tp = void> using Enable_If_T = typename EnableIf<_Bp, _Tp>::type;
    template<typename T> using IsClassType_V = typename IsClassType<T>::value;


    template <typename T>
    struct HasFractionalTypeDefinition
    {
        // Types "yes" and "no" are guaranteed to have different sizes,
        // specifically sizeof(yes) == 1 and sizeof(no) == 2.
        typedef char yes[1];
        typedef char no[2];

        template <typename C>
        static yes& test(typename C::FractionPrincipalType*);

        template <typename>
        static no& test(...);

        // If the "sizeof" of the result of calling test<T>(nullptr) is equal to sizeof(yes),
        // the first overload worked and T has a nested type named foobar.
        static const bool value = (sizeof(test<T>(nullptr)) == sizeof(yes)) || PDP::IsIntegrelType<T>::value;
    };

    template<typename T> using HasFractionalTypeDefinition_V = typename HasFractionalTypeDefinition<T>::value;


    template <typename T>
    struct HasComparisonEqualOperand
    {
        // For the compile time comparison.
        typedef char yes[1];
        typedef yes no[2];

        // This helper struct permits us to check that serialize is truly a method.
        // The second argument must be of the type of the first.
        // For instance reallyHas<int, 10> would be substituted by reallyHas<int, int 10> and works!
        // reallyHas<int, &C::serialize> would be substituted by reallyHas<int, int &C::serialize> and fail!
        // Note: It only works with integral constants and pointers (so function pointers work).
        // In our case we check that &C::serialize has the same signature as the first argument!
        // reallyHas<std::string (C::*)(), &C::serialize> should be substituted by
        // reallyHas<std::string (C::*)(), std::string (C::*)() &C::serialize> and work!
        template <typename U, U u> struct reallyHas;

        // Two overloads for yes: one for the signature of a normal method, one is for the signature of a const method.
        // We accept a pointer to our helper struct, in order to avoid to instantiate a real instance of this type.
        // std::string (C::*)() is function pointer declaration.
        template <typename C> static yes& test(reallyHas<const bool (C::*)(const C & a), (&C::operator==)>* /*unused*/) { }
        template <typename C> static yes& test(reallyHas<const bool (C::*)(const C & a) const, (&C::operator==)>* /*unused*/) { }
        // The famous C++ sink-hole.
        // Note that sink-hole must be templated too as we are testing test<T>(0).
        // If the method serialize isn't available, we will end up in this method.
        template <typename> static no& test(...) { /* dark matter */ }

        // The constant used as a return value for the test.
        // The test is actually done here, thanks to the sizeof compile-time evaluation.
        static const bool value = (sizeof(test<T>(0)) == sizeof(yes) || PDP::IsPrimitiveType<T>::value);
    };

    template <typename T, typename V>
    struct HasComparisonEqualOperandPair
    {
        // For the compile time comparison.
        typedef char yes[1];
        typedef yes no[2];

        // This helper struct permits us to check that serialize is truly a method.
        // The second argument must be of the type of the first.
        // For instance reallyHas<int, 10> would be substituted by reallyHas<int, int 10> and works!
        // reallyHas<int, &C::serialize> would be substituted by reallyHas<int, int &C::serialize> and fail!
        // Note: It only works with integral constants and pointers (so function pointers work).
        // In our case we check that &C::serialize has the same signature as the first argument!
        // reallyHas<std::string (C::*)(), &C::serialize> should be substituted by
        // reallyHas<std::string (C::*)(), std::string (C::*)() &C::serialize> and work!
        template <typename U, U u> struct reallyHas;

        // Two overloads for yes: one for the signature of a normal method, one is for the signature of a const method.
        // We accept a pointer to our helper struct, in order to avoid to instantiate a real instance of this type.
        // std::string (C::*)() is function pointer declaration.
        template <typename C> static yes& test(reallyHas<const bool (C::*)(const V & a), (&C::operator==)>* /*unused*/) { }
        template <typename C> static yes& test(reallyHas<const bool (C::*)(const V & a) const, (&C::operator==)>* /*unused*/) { }
        // The famous C++ sink-hole.
        // Note that sink-hole must be templated too as we are testing test<T>(0).
        // If the method serialize isn't available, we will end up in this method.
        template <typename> static no& test(...) { /* dark matter */ }

        // The constant used as a return value for the test.
        // The test is actually done here, thanks to the sizeof compile-time evaluation.
        static const bool value = (sizeof(test<T>(0)) == sizeof(yes) || PDP::IsPrimitiveType<T>::value);
    };

    template<typename T, typename V>
    struct HasComparisonEqualOperandPair<T, V*>
    {
        // For the compile time comparison.
        typedef char yes[1];
        typedef yes no[2];

        // This helper struct permits us to check that serialize is truly a method.
        // The second argument must be of the type of the first.
        // For instance reallyHas<int, 10> would be substituted by reallyHas<int, int 10> and works!
        // reallyHas<int, &C::serialize> would be substituted by reallyHas<int, int &C::serialize> and fail!
        // Note: It only works with integral constants and pointers (so function pointers work).
        // In our case we check that &C::serialize has the same signature as the first argument!
        // reallyHas<std::string (C::*)(), &C::serialize> should be substituted by
        // reallyHas<std::string (C::*)(), std::string (C::*)() &C::serialize> and work!
        template <typename U, U u> struct reallyHas;

        // Two overloads for yes: one for the signature of a normal method, one is for the signature of a const method.
        // We accept a pointer to our helper struct, in order to avoid to instantiate a real instance of this type.
        // std::string (C::*)() is function pointer declaration.
        template <typename C> static yes& test(reallyHas<const bool (C::*)(V * a), (&C::operator==)>* /*unused*/) { }
        template <typename C> static yes& test(reallyHas<const bool (C::*)(V * a) const, (&C::operator==)>* /*unused*/) { }
        // The famous C++ sink-hole.
        // Note that sink-hole must be templated too as we are testing test<T>(0).
        // If the method serialize isn't available, we will end up in this method.
        template <typename> static no& test(...) { /* dark matter */ }

        // The constant used as a return value for the test.
        // The test is actually done here, thanks to the sizeof compile-time evaluation.
        static const bool value = (sizeof(test<T>(0)) == sizeof(yes) || PDP::IsPrimitiveType<T>::value);
    };

    template<typename T> using HasComparisonEqualOperand_V = typename HasComparisonEqualOperand<T>::value;

    template<typename T>
    struct HasComparisonNotEqualOperand
    {
        // For the compile time comparison.
        typedef char yes[1];
        typedef yes no[2];

        // This helper struct permits us to check that serialize is truly a method.
        // The second argument must be of the type of the first.
        // For instance reallyHas<int, 10> would be substituted by reallyHas<int, int 10> and works!
        // reallyHas<int, &C::serialize> would be substituted by reallyHas<int, int &C::serialize> and fail!
        // Note: It only works with integral constants and pointers (so function pointers work).
        // In our case we check that &C::serialize has the same signature as the first argument!
        // reallyHas<std::string (C::*)(), &C::serialize> should be substituted by
        // reallyHas<std::string (C::*)(), std::string (C::*)() &C::serialize> and work!
        template <typename U, U u> struct reallyHas;

        // Two overloads for yes: one for the signature of a normal method, one is for the signature of a const method.
        // We accept a pointer to our helper struct, in order to avoid to instantiate a real instance of this type.
        // std::string (C::*)() is function pointer declaration.
        template <typename C> static yes& test(reallyHas<const bool (C::*)(const C & a), (&C::operator!=)>* /*unused*/) { }
        template <typename C> static yes& test(reallyHas<const bool (C::*)(const C & a) const, (&C::operator!=)>* /*unused*/) { }
        // The famous C++ sink-hole.
        // Note that sink-hole must be templated too as we are testing test<T>(0).
        // If the method serialize isn't available, we will end up in this method.
        template <typename> static no& test(...) { /* dark matter */ }

        // The constant used as a return value for the test.
        // The test is actually done here, thanks to the sizeof compile-time evaluation.
        static const bool value = (sizeof(test<T>(0)) == sizeof(yes) || PDP::IsPrimitiveType<T>::value);
    };

    template<typename T, typename V>
    struct HasComparisonNotEqualOperandPair
    {
        // For the compile time comparison.
        typedef char yes[1];
        typedef yes no[2];

        // This helper struct permits us to check that serialize is truly a method.
        // The second argument must be of the type of the first.
        // For instance reallyHas<int, 10> would be substituted by reallyHas<int, int 10> and works!
        // reallyHas<int, &C::serialize> would be substituted by reallyHas<int, int &C::serialize> and fail!
        // Note: It only works with integral constants and pointers (so function pointers work).
        // In our case we check that &C::serialize has the same signature as the first argument!
        // reallyHas<std::string (C::*)(), &C::serialize> should be substituted by
        // reallyHas<std::string (C::*)(), std::string (C::*)() &C::serialize> and work!
        template <typename U, U u> struct reallyHas;

        // Two overloads for yes: one for the signature of a normal method, one is for the signature of a const method.
        // We accept a pointer to our helper struct, in order to avoid to instantiate a real instance of this type.
        // std::string (C::*)() is function pointer declaration.
        template <typename C> static yes& test(reallyHas<const bool (C::*)(const V & a), (&C::operator!=)>* /*unused*/) { }
        template <typename C> static yes& test(reallyHas<const bool (C::*)(const V & a) const, (&C::operator!=)>* /*unused*/) { }
        // The famous C++ sink-hole.
        // Note that sink-hole must be templated too as we are testing test<T>(0).
        // If the method serialize isn't available, we will end up in this method.
        template <typename> static no& test(...) { /* dark matter */ }

        // The constant used as a return value for the test.
        // The test is actually done here, thanks to the sizeof compile-time evaluation.
        static const bool value = (sizeof(test<T>(0)) == sizeof(yes) || PDP::IsPrimitiveType<T>::value);
    };

    template<typename T> using HasComparisonNotEqualOperand_V = typename HasComparisonNotEqualOperand<T>::value;



    template<typename T>
    struct HasComparisonGreaterThanOperand
    {
        // For the compile time comparison.
        typedef char yes[1];
        typedef yes no[2];

        // This helper struct permits us to check that serialize is truly a method.
        // The second argument must be of the type of the first.
        // For instance reallyHas<int, 10> would be substituted by reallyHas<int, int 10> and works!
        // reallyHas<int, &C::serialize> would be substituted by reallyHas<int, int &C::serialize> and fail!
        // Note: It only works with integral constants and pointers (so function pointers work).
        // In our case we check that &C::serialize has the same signature as the first argument!
        // reallyHas<std::string (C::*)(), &C::serialize> should be substituted by
        // reallyHas<std::string (C::*)(), std::string (C::*)() &C::serialize> and work!
        template <typename U, U u> struct reallyHas;

        // Two overloads for yes: one for the signature of a normal method, one is for the signature of a const method.
        // We accept a pointer to our helper struct, in order to avoid to instantiate a real instance of this type.
        // std::string (C::*)() is function pointer declaration.
        template <typename C> static yes& test(reallyHas<const bool (C::*)(const C & a), (&C::operator>)>* /*unused*/) { }
        template <typename C> static yes& test(reallyHas<const bool (C::*)(const C & a) const, (&C::operator>)>* /*unused*/) { }
        // The famous C++ sink-hole.
        // Note that sink-hole must be templated too as we are testing test<T>(0).
        // If the method serialize isn't available, we will end up in this method.
        template <typename> static no& test(...) { /* dark matter */ }

        // The constant used as a return value for the test.
        // The test is actually done here, thanks to the sizeof compile-time evaluation.
        static const bool value = (sizeof(test<T>(0)) == sizeof(yes) || PDP::IsPrimitiveType<T>::value);
    };


    template<typename T, typename V>
    struct HasComparisonGreaterThanOperandPair
    {
        // For the compile time comparison.
        typedef char yes[1];
        typedef yes no[2];

        // This helper struct permits us to check that serialize is truly a method.
        // The second argument must be of the type of the first.
        // For instance reallyHas<int, 10> would be substituted by reallyHas<int, int 10> and works!
        // reallyHas<int, &C::serialize> would be substituted by reallyHas<int, int &C::serialize> and fail!
        // Note: It only works with integral constants and pointers (so function pointers work).
        // In our case we check that &C::serialize has the same signature as the first argument!
        // reallyHas<std::string (C::*)(), &C::serialize> should be substituted by
        // reallyHas<std::string (C::*)(), std::string (C::*)() &C::serialize> and work!
        template <typename U, U u> struct reallyHas;

        // Two overloads for yes: one for the signature of a normal method, one is for the signature of a const method.
        // We accept a pointer to our helper struct, in order to avoid to instantiate a real instance of this type.
        // std::string (C::*)() is function pointer declaration.
        template <typename C> static yes& test(reallyHas<const bool (C::*)(const V & a), (&C::operator>)>* /*unused*/) { }
        template <typename C> static yes& test(reallyHas<const bool (C::*)(const V & a) const, (&C::operator>)>* /*unused*/) { }
        // The famous C++ sink-hole.
        // Note that sink-hole must be templated too as we are testing test<T>(0).
        // If the method serialize isn't available, we will end up in this method.
        template <typename> static no& test(...) { /* dark matter */ }

        // The constant used as a return value for the test.
        // The test is actually done here, thanks to the sizeof compile-time evaluation.
        static const bool value = (sizeof(test<T>(0)) == sizeof(yes) || PDP::IsPrimitiveType<T>::value);
    };

    template<typename T> using HasComparisonGreaterThanOperand_V = typename HasComparisonGreaterThanOperand<T>::value;

    template<typename T>
    struct HasComparisonGreaterThanEqualOperand
    {
        // For the compile time comparison.
        typedef char yes[1];
        typedef yes no[2];

        // This helper struct permits us to check that serialize is truly a method.
        // The second argument must be of the type of the first.
        // For instance reallyHas<int, 10> would be substituted by reallyHas<int, int 10> and works!
        // reallyHas<int, &C::serialize> would be substituted by reallyHas<int, int &C::serialize> and fail!
        // Note: It only works with integral constants and pointers (so function pointers work).
        // In our case we check that &C::serialize has the same signature as the first argument!
        // reallyHas<std::string (C::*)(), &C::serialize> should be substituted by
        // reallyHas<std::string (C::*)(), std::string (C::*)() &C::serialize> and work!
        template <typename U, U u> struct reallyHas;

        // Two overloads for yes: one for the signature of a normal method, one is for the signature of a const method.
        // We accept a pointer to our helper struct, in order to avoid to instantiate a real instance of this type.
        // std::string (C::*)() is function pointer declaration.
        template <typename C> static yes& test(reallyHas<const bool (C::*)(const C & a), (&C::operator>=)>* /*unused*/) { }
        template <typename C> static yes& test(reallyHas<const bool (C::*)(const C & a) const, (&C::operator>=)>* /*unused*/) { }
        // The famous C++ sink-hole.
        // Note that sink-hole must be templated too as we are testing test<T>(0).
        // If the method serialize isn't available, we will end up in this method.
        template <typename> static no& test(...) { /* dark matter */ }

        // The constant used as a return value for the test.
        // The test is actually done here, thanks to the sizeof compile-time evaluation.
        static const bool value = (sizeof(test<T>(0)) == sizeof(yes) || PDP::IsPrimitiveType<T>::value);
    };

    template<typename T, typename V>
    struct HasComparisonGreaterThanEqualOperandPair
    {
        // For the compile time comparison.
        typedef char yes[1];
        typedef yes no[2];

        // This helper struct permits us to check that serialize is truly a method.
        // The second argument must be of the type of the first.
        // For instance reallyHas<int, 10> would be substituted by reallyHas<int, int 10> and works!
        // reallyHas<int, &C::serialize> would be substituted by reallyHas<int, int &C::serialize> and fail!
        // Note: It only works with integral constants and pointers (so function pointers work).
        // In our case we check that &C::serialize has the same signature as the first argument!
        // reallyHas<std::string (C::*)(), &C::serialize> should be substituted by
        // reallyHas<std::string (C::*)(), std::string (C::*)() &C::serialize> and work!
        template <typename U, U u> struct reallyHas;

        // Two overloads for yes: one for the signature of a normal method, one is for the signature of a const method.
        // We accept a pointer to our helper struct, in order to avoid to instantiate a real instance of this type.
        // std::string (C::*)() is function pointer declaration.
        template <typename C> static yes& test(reallyHas<const bool (C::*)(const V & a), (&C::operator>=)>* /*unused*/) { }
        template <typename C> static yes& test(reallyHas<const bool (C::*)(const V & a) const, (&C::operator>=)>* /*unused*/) { }
        // The famous C++ sink-hole.
        // Note that sink-hole must be templated too as we are testing test<T>(0).
        // If the method serialize isn't available, we will end up in this method.
        template <typename> static no& test(...) { /* dark matter */ }

        // The constant used as a return value for the test.
        // The test is actually done here, thanks to the sizeof compile-time evaluation.
        static const bool value = (sizeof(test<T>(0)) == sizeof(yes) || PDP::IsPrimitiveType<T>::value);
    };


    template<typename T> using HasComparisonGreaterThanEqualOperand_V = typename HasComparisonGreaterThanEqualOperand<T>::value;

    template<typename T>
    struct HasComparisonLessThanOperand
    {
        // For the compile time comparison.
        typedef char yes[1];
        typedef yes no[2];

        // This helper struct permits us to check that serialize is truly a method.
        // The second argument must be of the type of the first.
        // For instance reallyHas<int, 10> would be substituted by reallyHas<int, int 10> and works!
        // reallyHas<int, &C::serialize> would be substituted by reallyHas<int, int &C::serialize> and fail!
        // Note: It only works with integral constants and pointers (so function pointers work).
        // In our case we check that &C::serialize has the same signature as the first argument!
        // reallyHas<std::string (C::*)(), &C::serialize> should be substituted by
        // reallyHas<std::string (C::*)(), std::string (C::*)() &C::serialize> and work!
        template <typename U, U u> struct reallyHas;

        // Two overloads for yes: one for the signature of a normal method, one is for the signature of a const method.
        // We accept a pointer to our helper struct, in order to avoid to instantiate a real instance of this type.
        // std::string (C::*)() is function pointer declaration.
        template <typename C> static yes& test(reallyHas<const bool (C::*)(const C & a), (&C::operator<)>* /*unused*/) { }
        template <typename C> static yes& test(reallyHas<const bool (C::*)(const C & a) const, (&C::operator<)>* /*unused*/) { }
        // The famous C++ sink-hole.
        // Note that sink-hole must be templated too as we are testing test<T>(0).
        // If the method serialize isn't available, we will end up in this method.
        template <typename> static no& test(...) { /* dark matter */ }

        // The constant used as a return value for the test.
        // The test is actually done here, thanks to the sizeof compile-time evaluation.
        static const bool value = (sizeof(test<T>(0)) == sizeof(yes) || PDP::IsPrimitiveType<T>::value);
    };

    template<typename T, typename V>
    struct HasComparisonLessThanOperandPair
    {
        // For the compile time comparison.
        typedef char yes[1];
        typedef yes no[2];

        // This helper struct permits us to check that serialize is truly a method.
        // The second argument must be of the type of the first.
        // For instance reallyHas<int, 10> would be substituted by reallyHas<int, int 10> and works!
        // reallyHas<int, &C::serialize> would be substituted by reallyHas<int, int &C::serialize> and fail!
        // Note: It only works with integral constants and pointers (so function pointers work).
        // In our case we check that &C::serialize has the same signature as the first argument!
        // reallyHas<std::string (C::*)(), &C::serialize> should be substituted by
        // reallyHas<std::string (C::*)(), std::string (C::*)() &C::serialize> and work!
        template <typename U, U u> struct reallyHas;

        // Two overloads for yes: one for the signature of a normal method, one is for the signature of a const method.
        // We accept a pointer to our helper struct, in order to avoid to instantiate a real instance of this type.
        // std::string (C::*)() is function pointer declaration.
        template <typename C> static yes& test(reallyHas<const bool (C::*)(const V & a), (&C::operator<)>* /*unused*/) { }
        template <typename C> static yes& test(reallyHas<const bool (C::*)(const V & a) const, (&C::operator<)>* /*unused*/) { }
        // The famous C++ sink-hole.
        // Note that sink-hole must be templated too as we are testing test<T>(0).
        // If the method serialize isn't available, we will end up in this method.
        template <typename> static no& test(...) { /* dark matter */ }

        // The constant used as a return value for the test.
        // The test is actually done here, thanks to the sizeof compile-time evaluation.
        static const bool value = (sizeof(test<T>(0)) == sizeof(yes) || PDP::IsPrimitiveType<T>::value);
    };

    template<typename T> using HasComparisonLessThanOperand_V = typename HasComparisonLessThanOperand<T>::value;


    template<typename T>
    struct HasComparisonLessThanEqualOperand
    {
        // For the compile time comparison.
        typedef char yes[1];
        typedef yes no[2];

        // This helper struct permits us to check that serialize is truly a method.
        // The second argument must be of the type of the first.
        // For instance reallyHas<int, 10> would be substituted by reallyHas<int, int 10> and works!
        // reallyHas<int, &C::serialize> would be substituted by reallyHas<int, int &C::serialize> and fail!
        // Note: It only works with integral constants and pointers (so function pointers work).
        // In our case we check that &C::serialize has the same signature as the first argument!
        // reallyHas<std::string (C::*)(), &C::serialize> should be substituted by
        // reallyHas<std::string (C::*)(), std::string (C::*)() &C::serialize> and work!
        template <typename U, U u> struct reallyHas;

        // Two overloads for yes: one for the signature of a normal method, one is for the signature of a const method.
        // We accept a pointer to our helper struct, in order to avoid to instantiate a real instance of this type.
        // std::string (C::*)() is function pointer declaration.
        template <typename C> static yes& test(reallyHas<const bool (C::*)(const C & a), (&C::operator<=)>* /*unused*/) { }
        template <typename C> static yes& test(reallyHas<const bool (C::*)(const C & a) const, (&C::operator<=)>* /*unused*/) { }
        // The famous C++ sink-hole.
        // Note that sink-hole must be templated too as we are testing test<T>(0).
        // If the method serialize isn't available, we will end up in this method.
        template <typename> static no& test(...) { /* dark matter */ }

        // The constant used as a return value for the test.
        // The test is actually done here, thanks to the sizeof compile-time evaluation.
        static const bool value = (sizeof(test<T>(0)) == sizeof(yes) || PDP::IsPrimitiveType<T>::value);
    };

    template<typename T, typename V>
    struct HasComparisonLessThanEqualOperandPair
    {
        // For the compile time comparison.
        typedef char yes[1];
        typedef yes no[2];

        // This helper struct permits us to check that serialize is truly a method.
        // The second argument must be of the type of the first.
        // For instance reallyHas<int, 10> would be substituted by reallyHas<int, int 10> and works!
        // reallyHas<int, &C::serialize> would be substituted by reallyHas<int, int &C::serialize> and fail!
        // Note: It only works with integral constants and pointers (so function pointers work).
        // In our case we check that &C::serialize has the same signature as the first argument!
        // reallyHas<std::string (C::*)(), &C::serialize> should be substituted by
        // reallyHas<std::string (C::*)(), std::string (C::*)() &C::serialize> and work!
        template <typename U, U u> struct reallyHas;

        // Two overloads for yes: one for the signature of a normal method, one is for the signature of a const method.
        // We accept a pointer to our helper struct, in order to avoid to instantiate a real instance of this type.
        // std::string (C::*)() is function pointer declaration.
        template <typename C> static yes& test(reallyHas<const bool (C::*)(const V & a), (&C::operator<=)>* /*unused*/) { }
        template <typename C> static yes& test(reallyHas<const bool (C::*)(const V & a) const, (&C::operator<=)>* /*unused*/) { }
        // The famous C++ sink-hole.
        // Note that sink-hole must be templated too as we are testing test<T>(0).
        // If the method serialize isn't available, we will end up in this method.
        template <typename> static no& test(...) { /* dark matter */ }

        // The constant used as a return value for the test.
        // The test is actually done here, thanks to the sizeof compile-time evaluation.
        static const bool value = (sizeof(test<T>(0)) == sizeof(yes) || PDP::IsPrimitiveType<T>::value);
    };

    template<typename T> using HasComparisonLessThanEqualOperand_V = typename HasComparisonLessThanEqualOperand<T>::value;

    template <typename T>
    struct HasAdditionOperand
    {
        // For the compile time comparison.
        typedef char yes[1];
        typedef yes no[2];

        // This helper struct permits us to check that serialize is truly a method.
        // The second argument must be of the type of the first.
        // For instance reallyHas<int, 10> would be substituted by reallyHas<int, int 10> and works!
        // reallyHas<int, &C::serialize> would be substituted by reallyHas<int, int &C::serialize> and fail!
        // Note: It only works with integral constants and pointers (so function pointers work).
        // In our case we check that &C::serialize has the same signature as the first argument!
        // reallyHas<std::string (C::*)(), &C::serialize> should be substituted by
        // reallyHas<std::string (C::*)(), std::string (C::*)() &C::serialize> and work!
        template <typename U, U u> struct reallyHas;

        // Two overloads for yes: one for the signature of a normal method, one is for the signature of a const method.
        // We accept a pointer to our helper struct, in order to avoid to instantiate a real instance of this type.
        // std::string (C::*)() is function pointer declaration.
        template <typename C> static yes& test(reallyHas<C (C::*)(const C & a), (&C::operator+)>* /*unused*/) { }
        template <typename C> static yes& test(reallyHas<C (C::*)(const C & a) const, (&C::operator+)>* /*unused*/) { }
        // The famous C++ sink-hole.
        // Note that sink-hole must be templated too as we are testing test<T>(0).
        // If the method serialize isn't available, we will end up in this method.
        template <typename> static no& test(...) { /* dark matter */ }

        // The constant used as a return value for the test.
        // The test is actually done here, thanks to the sizeof compile-time evaluation.
        static const bool value = (sizeof(test<T>(0)) == sizeof(yes) || PDP::IsPrimitiveType<T>::value);
    };


    template <typename T, typename V>
    struct HasAdditionOperandPair
    {
        // For the compile time comparison.
        typedef char yes[1];
        typedef yes no[2];

        // This helper struct permits us to check that serialize is truly a method.
        // The second argument must be of the type of the first.
        // For instance reallyHas<int, 10> would be substituted by reallyHas<int, int 10> and works!
        // reallyHas<int, &C::serialize> would be substituted by reallyHas<int, int &C::serialize> and fail!
        // Note: It only works with integral constants and pointers (so function pointers work).
        // In our case we check that &C::serialize has the same signature as the first argument!
        // reallyHas<std::string (C::*)(), &C::serialize> should be substituted by
        // reallyHas<std::string (C::*)(), std::string (C::*)() &C::serialize> and work!
        template <typename U, U u> struct reallyHas;

        // Two overloads for yes: one for the signature of a normal method, one is for the signature of a const method.
        // We accept a pointer to our helper struct, in order to avoid to instantiate a real instance of this type.
        // std::string (C::*)() is function pointer declaration.
        template <typename C> static yes& test(reallyHas<C (C::*)(const V & a), (&C::operator+)>* /*unused*/) { }
        template <typename C> static yes& test(reallyHas<C (C::*)(const V & a) const, (&C::operator+)>* /*unused*/) { }
        // The famous C++ sink-hole.
        // Note that sink-hole must be templated too as we are testing test<T>(0).
        // If the method serialize isn't available, we will end up in this method.
        template <typename> static no& test(...) { /* dark matter */ }

        // The constant used as a return value for the test.
        // The test is actually done here, thanks to the sizeof compile-time evaluation.
        static const bool value = (sizeof(test<T>(0)) == sizeof(yes) || PDP::IsPrimitiveType<T>::value);
    };

    template<typename T> using HasAdditionOperand_V = typename HasAdditionOperand<T>::value;


    template <typename T>
    struct HasCompoundAdditionOperand
    {
        // For the compile time comparison.
        typedef char yes[1];
        typedef yes no[2];

        // This helper struct permits us to check that serialize is truly a method.
        // The second argument must be of the type of the first.
        // For instance reallyHas<int, 10> would be substituted by reallyHas<int, int 10> and works!
        // reallyHas<int, &C::serialize> would be substituted by reallyHas<int, int &C::serialize> and fail!
        // Note: It only works with integral constants and pointers (so function pointers work).
        // In our case we check that &C::serialize has the same signature as the first argument!
        // reallyHas<std::string (C::*)(), &C::serialize> should be substituted by
        // reallyHas<std::string (C::*)(), std::string (C::*)() &C::serialize> and work!
        template <typename U, U u> struct reallyHas;

        // Two overloads for yes: one for the signature of a normal method, one is for the signature of a const method.
        // We accept a pointer to our helper struct, in order to avoid to instantiate a real instance of this type.
        // std::string (C::*)() is function pointer declaration.
        template <typename C> static yes& test(reallyHas<C & (C::*)(const C & a), (&C::operator+=)>* /*unused*/) { }
        template <typename C> static yes& test(reallyHas<C & (C::*)(const C & a) const, (&C::operator+=)>* /*unused*/) { }
        // The famous C++ sink-hole.
        // Note that sink-hole must be templated too as we are testing test<T>(0).
        // If the method serialize isn't available, we will end up in this method.
        template <typename> static no& test(...) { /* dark matter */ }

        // The constant used as a return value for the test.
        // The test is actually done here, thanks to the sizeof compile-time evaluation.
        static const bool value = (sizeof(test<T>(0)) == sizeof(yes) || PDP::IsPrimitiveType<T>::value);
    };

    template <typename T, typename V>
    struct HasCompoundAdditionOperandPair
    {
        // For the compile time comparison.
        typedef char yes[1];
        typedef yes no[2];

        // This helper struct permits us to check that serialize is truly a method.
        // The second argument must be of the type of the first.
        // For instance reallyHas<int, 10> would be substituted by reallyHas<int, int 10> and works!
        // reallyHas<int, &C::serialize> would be substituted by reallyHas<int, int &C::serialize> and fail!
        // Note: It only works with integral constants and pointers (so function pointers work).
        // In our case we check that &C::serialize has the same signature as the first argument!
        // reallyHas<std::string (C::*)(), &C::serialize> should be substituted by
        // reallyHas<std::string (C::*)(), std::string (C::*)() &C::serialize> and work!
        template <typename U, U u> struct reallyHas;

        // Two overloads for yes: one for the signature of a normal method, one is for the signature of a const method.
        // We accept a pointer to our helper struct, in order to avoid to instantiate a real instance of this type.
        // std::string (C::*)() is function pointer declaration.
        template <typename C> static yes& test(reallyHas<C & (C::*)(const V & a), (&C::operator+=)>* /*unused*/) { }
        template <typename C> static yes& test(reallyHas<C & (C::*)(const V & a) const, (&C::operator+=)>* /*unused*/) { }
        // The famous C++ sink-hole.
        // Note that sink-hole must be templated too as we are testing test<T>(0).
        // If the method serialize isn't available, we will end up in this method.
        template <typename> static no& test(...) { /* dark matter */ }

        // The constant used as a return value for the test.
        // The test is actually done here, thanks to the sizeof compile-time evaluation.
        static const bool value = (sizeof(test<T>(0)) == sizeof(yes) || PDP::IsPrimitiveType<T>::value);
    };


    template<typename T> using HasCompoundAdditionOperand_V = typename HasCompoundAdditionOperand<T>::value;

    template <typename T>
    struct HasSubtractionOperand
    {
        // For the compile time comparison.
        typedef char yes[1];
        typedef yes no[2];

        // This helper struct permits us to check that serialize is truly a method.
        // The second argument must be of the type of the first.
        // For instance reallyHas<int, 10> would be substituted by reallyHas<int, int 10> and works!
        // reallyHas<int, &C::serialize> would be substituted by reallyHas<int, int &C::serialize> and fail!
        // Note: It only works with integral constants and pointers (so function pointers work).
        // In our case we check that &C::serialize has the same signature as the first argument!
        // reallyHas<std::string (C::*)(), &C::serialize> should be substituted by
        // reallyHas<std::string (C::*)(), std::string (C::*)() &C::serialize> and work!
        template <typename U, U u> struct reallyHas;

        // Two overloads for yes: one for the signature of a normal method, one is for the signature of a const method.
        // We accept a pointer to our helper struct, in order to avoid to instantiate a real instance of this type.
        // std::string (C::*)() is function pointer declaration.
        template <typename C> static yes& test(reallyHas<C (C::*)(const C & a), (&C::operator-)>* /*unused*/) { }
        template <typename C> static yes& test(reallyHas<C (C::*)(const C & a) const, (&C::operator-)>* /*unused*/) { }
        // The famous C++ sink-hole.
        // Note that sink-hole must be templated too as we are testing test<T>(0).
        // If the method serialize isn't available, we will end up in this method.
        template <typename> static no& test(...) { /* dark matter */ }

        // The constant used as a return value for the test.
        // The test is actually done here, thanks to the sizeof compile-time evaluation.
        static const bool value = (sizeof(test<T>(0)) == sizeof(yes) || PDP::IsPrimitiveType<T>::value);
    };


    template <typename T, typename V>
    struct HasSubtractionOperandPair
    {
        // For the compile time comparison.
        typedef char yes[1];
        typedef yes no[2];

        // This helper struct permits us to check that serialize is truly a method.
        // The second argument must be of the type of the first.
        // For instance reallyHas<int, 10> would be substituted by reallyHas<int, int 10> and works!
        // reallyHas<int, &C::serialize> would be substituted by reallyHas<int, int &C::serialize> and fail!
        // Note: It only works with integral constants and pointers (so function pointers work).
        // In our case we check that &C::serialize has the same signature as the first argument!
        // reallyHas<std::string (C::*)(), &C::serialize> should be substituted by
        // reallyHas<std::string (C::*)(), std::string (C::*)() &C::serialize> and work!
        template <typename U, U u> struct reallyHas;

        // Two overloads for yes: one for the signature of a normal method, one is for the signature of a const method.
        // We accept a pointer to our helper struct, in order to avoid to instantiate a real instance of this type.
        // std::string (C::*)() is function pointer declaration.
        template <typename C> static yes& test(reallyHas<C (C::*)(const V & a), (&C::operator-)>* /*unused*/) { }
        template <typename C> static yes& test(reallyHas<C (C::*)(const V & a) const, (&C::operator-)>* /*unused*/) { }
        // The famous C++ sink-hole.
        // Note that sink-hole must be templated too as we are testing test<T>(0).
        // If the method serialize isn't available, we will end up in this method.
        template <typename> static no& test(...) { /* dark matter */ }

        // The constant used as a return value for the test.
        // The test is actually done here, thanks to the sizeof compile-time evaluation.
        static const bool value = (sizeof(test<T>(0)) == sizeof(yes) || PDP::IsPrimitiveType<T>::value);
    };

    template<typename T> using HasSubtractionOperand_V = typename HasSubtractionOperand<T>::value;


    template <typename T>
    struct HasCompoundSubtractionOperand
    {
        // For the compile time comparison.
        typedef char yes[1];
        typedef yes no[2];

        // This helper struct permits us to check that serialize is truly a method.
        // The second argument must be of the type of the first.
        // For instance reallyHas<int, 10> would be substituted by reallyHas<int, int 10> and works!
        // reallyHas<int, &C::serialize> would be substituted by reallyHas<int, int &C::serialize> and fail!
        // Note: It only works with integral constants and pointers (so function pointers work).
        // In our case we check that &C::serialize has the same signature as the first argument!
        // reallyHas<std::string (C::*)(), &C::serialize> should be substituted by
        // reallyHas<std::string (C::*)(), std::string (C::*)() &C::serialize> and work!
        template <typename U, U u> struct reallyHas;

        // Two overloads for yes: one for the signature of a normal method, one is for the signature of a const method.
        // We accept a pointer to our helper struct, in order to avoid to instantiate a real instance of this type.
        // std::string (C::*)() is function pointer declaration.
        template <typename C> static yes& test(reallyHas<C & (C::*)(const C & a), (&C::operator-=)>* /*unused*/) { }
        template <typename C> static yes& test(reallyHas<C & (C::*)(const C & a) const, (&C::operator-=)>* /*unused*/) { }
        // The famous C++ sink-hole.
        // Note that sink-hole must be templated too as we are testing test<T>(0).
        // If the method serialize isn't available, we will end up in this method.
        template <typename> static no& test(...) { /* dark matter */ }

        // The constant used as a return value for the test.
        // The test is actually done here, thanks to the sizeof compile-time evaluation.
        static const bool value = (sizeof(test<T>(0)) == sizeof(yes) || PDP::IsPrimitiveType<T>::value);
    };


    template <typename T, typename V>
    struct HasCompoundSubtractionOperandPair
    {
        // For the compile time comparison.
        typedef char yes[1];
        typedef yes no[2];

        // This helper struct permits us to check that serialize is truly a method.
        // The second argument must be of the type of the first.
        // For instance reallyHas<int, 10> would be substituted by reallyHas<int, int 10> and works!
        // reallyHas<int, &C::serialize> would be substituted by reallyHas<int, int &C::serialize> and fail!
        // Note: It only works with integral constants and pointers (so function pointers work).
        // In our case we check that &C::serialize has the same signature as the first argument!
        // reallyHas<std::string (C::*)(), &C::serialize> should be substituted by
        // reallyHas<std::string (C::*)(), std::string (C::*)() &C::serialize> and work!
        template <typename U, U u> struct reallyHas;

        // Two overloads for yes: one for the signature of a normal method, one is for the signature of a const method.
        // We accept a pointer to our helper struct, in order to avoid to instantiate a real instance of this type.
        // std::string (C::*)() is function pointer declaration.
        template <typename C> static yes& test(reallyHas<C & (C::*)(const V & a), (&C::operator-=)>* /*unused*/) { }
        template <typename C> static yes& test(reallyHas<C & (C::*)(const V & a) const, (&C::operator-=)>* /*unused*/) { }
        // The famous C++ sink-hole.
        // Note that sink-hole must be templated too as we are testing test<T>(0).
        // If the method serialize isn't available, we will end up in this method.
        template <typename> static no& test(...) { /* dark matter */ }

        // The constant used as a return value for the test.
        // The test is actually done here, thanks to the sizeof compile-time evaluation.
        static const bool value = (sizeof(test<T>(0)) == sizeof(yes) || PDP::IsPrimitiveType<T>::value);
    };

    template<typename T> using HasCompoundSubtractionOperand_V = typename HasCompoundSubtractionOperand<T>::value;


    template <typename T>
    struct HasMultiplicationOperand
    {
        // For the compile time comparison.
        typedef char yes[1];
        typedef yes no[2];

        // This helper struct permits us to check that serialize is truly a method.
        // The second argument must be of the type of the first.
        // For instance reallyHas<int, 10> would be substituted by reallyHas<int, int 10> and works!
        // reallyHas<int, &C::serialize> would be substituted by reallyHas<int, int &C::serialize> and fail!
        // Note: It only works with integral constants and pointers (so function pointers work).
        // In our case we check that &C::serialize has the same signature as the first argument!
        // reallyHas<std::string (C::*)(), &C::serialize> should be substituted by
        // reallyHas<std::string (C::*)(), std::string (C::*)() &C::serialize> and work!
        template <typename U, U u> struct reallyHas;

        // Two overloads for yes: one for the signature of a normal method, one is for the signature of a const method.
        // We accept a pointer to our helper struct, in order to avoid to instantiate a real instance of this type.
        // std::string (C::*)() is function pointer declaration.
        template <typename C> static yes& test(reallyHas<C (C::*)(const C & a), (&C::operator*)>* /*unused*/) { }
        template <typename C> static yes& test(reallyHas<C (C::*)(const C & a) const, (&C::operator*)>* /*unused*/) { }
        // The famous C++ sink-hole.
        // Note that sink-hole must be templated too as we are testing test<T>(0).
        // If the method serialize isn't available, we will end up in this method.
        template <typename> static no& test(...) { /* dark matter */ }

        // The constant used as a return value for the test.
        // The test is actually done here, thanks to the sizeof compile-time evaluation.
        static const bool value = (sizeof(test<T>(0)) == sizeof(yes) || PDP::IsPrimitiveType<T>::value);
    };


    template <typename T, typename V>
    struct HasMultiplicationOperandPair
    {
        // For the compile time comparison.
        typedef char yes[1];
        typedef yes no[2];

        // This helper struct permits us to check that serialize is truly a method.
        // The second argument must be of the type of the first.
        // For instance reallyHas<int, 10> would be substituted by reallyHas<int, int 10> and works!
        // reallyHas<int, &C::serialize> would be substituted by reallyHas<int, int &C::serialize> and fail!
        // Note: It only works with integral constants and pointers (so function pointers work).
        // In our case we check that &C::serialize has the same signature as the first argument!
        // reallyHas<std::string (C::*)(), &C::serialize> should be substituted by
        // reallyHas<std::string (C::*)(), std::string (C::*)() &C::serialize> and work!
        template <typename U, U u> struct reallyHas;

        // Two overloads for yes: one for the signature of a normal method, one is for the signature of a const method.
        // We accept a pointer to our helper struct, in order to avoid to instantiate a real instance of this type.
        // std::string (C::*)() is function pointer declaration.
        template <typename C> static yes& test(reallyHas<C (C::*)(const V & a), (&C::operator*)>* /*unused*/) { }
        template <typename C> static yes& test(reallyHas<C (C::*)(const V & a) const, (&C::operator*)>* /*unused*/) { }
        // The famous C++ sink-hole.
        // Note that sink-hole must be templated too as we are testing test<T>(0).
        // If the method serialize isn't available, we will end up in this method.
        template <typename> static no& test(...) { /* dark matter */ }

        // The constant used as a return value for the test.
        // The test is actually done here, thanks to the sizeof compile-time evaluation.
        static const bool value = (sizeof(test<T>(0)) == sizeof(yes) || PDP::IsPrimitiveType<T>::value);
    };

    template<typename T> using HasMultiplicationOperand_V = typename HasMultiplicationOperand<T>::value;


    template <typename T>
    struct HasCompoundMultiplicationOperand
    {
        // For the compile time comparison.
        typedef char yes[1];
        typedef yes no[2];

        // This helper struct permits us to check that serialize is truly a method.
        // The second argument must be of the type of the first.
        // For instance reallyHas<int, 10> would be substituted by reallyHas<int, int 10> and works!
        // reallyHas<int, &C::serialize> would be substituted by reallyHas<int, int &C::serialize> and fail!
        // Note: It only works with integral constants and pointers (so function pointers work).
        // In our case we check that &C::serialize has the same signature as the first argument!
        // reallyHas<std::string (C::*)(), &C::serialize> should be substituted by
        // reallyHas<std::string (C::*)(), std::string (C::*)() &C::serialize> and work!
        template <typename U, U u> struct reallyHas;

        // Two overloads for yes: one for the signature of a normal method, one is for the signature of a const method.
        // We accept a pointer to our helper struct, in order to avoid to instantiate a real instance of this type.
        // std::string (C::*)() is function pointer declaration.
        template <typename C> static yes& test(reallyHas<C & (C::*)(const C & a), (&C::operator*=)>* /*unused*/) { }
        template <typename C> static yes& test(reallyHas<C & (C::*)(const C & a) const, (&C::operator*=)>* /*unused*/) { }
        // The famous C++ sink-hole.
        // Note that sink-hole must be templated too as we are testing test<T>(0).
        // If the method serialize isn't available, we will end up in this method.
        template <typename> static no& test(...) { /* dark matter */ }

        // The constant used as a return value for the test.
        // The test is actually done here, thanks to the sizeof compile-time evaluation.
        static const bool value = (sizeof(test<T>(0)) == sizeof(yes) || PDP::IsPrimitiveType<T>::value);
    };


    template <typename T, typename V>
    struct HasCompoundMultiplicationOperandPair
    {
        // For the compile time comparison.
        typedef char yes[1];
        typedef yes no[2];

        // This helper struct permits us to check that serialize is truly a method.
        // The second argument must be of the type of the first.
        // For instance reallyHas<int, 10> would be substituted by reallyHas<int, int 10> and works!
        // reallyHas<int, &C::serialize> would be substituted by reallyHas<int, int &C::serialize> and fail!
        // Note: It only works with integral constants and pointers (so function pointers work).
        // In our case we check that &C::serialize has the same signature as the first argument!
        // reallyHas<std::string (C::*)(), &C::serialize> should be substituted by
        // reallyHas<std::string (C::*)(), std::string (C::*)() &C::serialize> and work!
        template <typename U, U u> struct reallyHas;

        // Two overloads for yes: one for the signature of a normal method, one is for the signature of a const method.
        // We accept a pointer to our helper struct, in order to avoid to instantiate a real instance of this type.
        // std::string (C::*)() is function pointer declaration.
        template <typename C> static yes& test(reallyHas<C & (C::*)(const V & a), (&C::operator*=)>* /*unused*/) { }
        template <typename C> static yes& test(reallyHas<C & (C::*)(const V & a) const, (&C::operator*=)>* /*unused*/) { }
        // The famous C++ sink-hole.
        // Note that sink-hole must be templated too as we are testing test<T>(0).
        // If the method serialize isn't available, we will end up in this method.
        template <typename> static no& test(...) { /* dark matter */ }

        // The constant used as a return value for the test.
        // The test is actually done here, thanks to the sizeof compile-time evaluation.
        static const bool value = (sizeof(test<T>(0)) == sizeof(yes) || PDP::IsPrimitiveType<T>::value);
    };

    template<typename T> using HasCompoundMultiplicationOperand_V = typename HasCompoundMultiplicationOperand<T>::value;

    template <typename T>
    struct HasDivisionOperand
    {
        // For the compile time comparison.
        typedef char yes[1];
        typedef yes no[2];

        // This helper struct permits us to check that serialize is truly a method.
        // The second argument must be of the type of the first.
        // For instance reallyHas<int, 10> would be substituted by reallyHas<int, int 10> and works!
        // reallyHas<int, &C::serialize> would be substituted by reallyHas<int, int &C::serialize> and fail!
        // Note: It only works with integral constants and pointers (so function pointers work).
        // In our case we check that &C::serialize has the same signature as the first argument!
        // reallyHas<std::string (C::*)(), &C::serialize> should be substituted by
        // reallyHas<std::string (C::*)(), std::string (C::*)() &C::serialize> and work!
        template <typename U, U u> struct reallyHas;

        // Two overloads for yes: one for the signature of a normal method, one is for the signature of a const method.
        // We accept a pointer to our helper struct, in order to avoid to instantiate a real instance of this type.
        // std::string (C::*)() is function pointer declaration.
        template <typename C> static yes& test(reallyHas<C (C::*)(const C & a), (&C::operator/)>* /*unused*/) { }
        template <typename C> static yes& test(reallyHas<C (C::*)(const C & a) const, (&C::operator/)>* /*unused*/) { }
        // The famous C++ sink-hole.
        // Note that sink-hole must be templated too as we are testing test<T>(0).
        // If the method serialize isn't available, we will end up in this method.
        template <typename> static no& test(...) { /* dark matter */ }

        // The constant used as a return value for the test.
        // The test is actually done here, thanks to the sizeof compile-time evaluation.
        static const bool value = (sizeof(test<T>(0)) == sizeof(yes) || PDP::IsPrimitiveType<T>::value);
    };

    template <typename T, typename V>
    struct HasDivisionOperandPair
    {
        // For the compile time comparison.
        typedef char yes[1];
        typedef yes no[2];

        // This helper struct permits us to check that serialize is truly a method.
        // The second argument must be of the type of the first.
        // For instance reallyHas<int, 10> would be substituted by reallyHas<int, int 10> and works!
        // reallyHas<int, &C::serialize> would be substituted by reallyHas<int, int &C::serialize> and fail!
        // Note: It only works with integral constants and pointers (so function pointers work).
        // In our case we check that &C::serialize has the same signature as the first argument!
        // reallyHas<std::string (C::*)(), &C::serialize> should be substituted by
        // reallyHas<std::string (C::*)(), std::string (C::*)() &C::serialize> and work!
        template <typename U, U u> struct reallyHas;

        // Two overloads for yes: one for the signature of a normal method, one is for the signature of a const method.
        // We accept a pointer to our helper struct, in order to avoid to instantiate a real instance of this type.
        // std::string (C::*)() is function pointer declaration.
        template <typename C> static yes& test(reallyHas<C (C::*)(const V & a), (&C::operator/)>* /*unused*/) { }
        template <typename C> static yes& test(reallyHas<C (C::*)(const V & a) const, (&C::operator/)>* /*unused*/) { }
        // The famous C++ sink-hole.
        // Note that sink-hole must be templated too as we are testing test<T>(0).
        // If the method serialize isn't available, we will end up in this method.
        template <typename> static no& test(...) { /* dark matter */ }

        // The constant used as a return value for the test.
        // The test is actually done here, thanks to the sizeof compile-time evaluation.
        static const bool value = (sizeof(test<T>(0)) == sizeof(yes) || PDP::IsPrimitiveType<T>::value);
    };

    template<typename T> using HasDivisionOperand_V = typename HasDivisionOperand<T>::value;


    template <typename T>
    struct HasCompoundDivisionOperand
    {
        // For the compile time comparison.
        typedef char yes[1];
        typedef yes no[2];

        // This helper struct permits us to check that serialize is truly a method.
        // The second argument must be of the type of the first.
        // For instance reallyHas<int, 10> would be substituted by reallyHas<int, int 10> and works!
        // reallyHas<int, &C::serialize> would be substituted by reallyHas<int, int &C::serialize> and fail!
        // Note: It only works with integral constants and pointers (so function pointers work).
        // In our case we check that &C::serialize has the same signature as the first argument!
        // reallyHas<std::string (C::*)(), &C::serialize> should be substituted by
        // reallyHas<std::string (C::*)(), std::string (C::*)() &C::serialize> and work!
        template <typename U, U u> struct reallyHas;

        // Two overloads for yes: one for the signature of a normal method, one is for the signature of a const method.
        // We accept a pointer to our helper struct, in order to avoid to instantiate a real instance of this type.
        // std::string (C::*)() is function pointer declaration.
        template <typename C> static yes& test(reallyHas<C & (C::*)(const C & a), (&C::operator/=)>* /*unused*/) { }
        template <typename C> static yes& test(reallyHas<C & (C::*)(const C & a) const, (&C::operator/=)>* /*unused*/) { }
        // The famous C++ sink-hole.
        // Note that sink-hole must be templated too as we are testing test<T>(0).
        // If the method serialize isn't available, we will end up in this method.
        template <typename> static no& test(...) { /* dark matter */ }

        // The constant used as a return value for the test.
        // The test is actually done here, thanks to the sizeof compile-time evaluation.
        static const bool value = (sizeof(test<T>(0)) == sizeof(yes) || PDP::IsPrimitiveType<T>::value);
    };


    template <typename T, typename V>
    struct HasCompoundDivisionOperandPair
    {
        // For the compile time comparison.
        typedef char yes[1];
        typedef yes no[2];

        // This helper struct permits us to check that serialize is truly a method.
        // The second argument must be of the type of the first.
        // For instance reallyHas<int, 10> would be substituted by reallyHas<int, int 10> and works!
        // reallyHas<int, &C::serialize> would be substituted by reallyHas<int, int &C::serialize> and fail!
        // Note: It only works with integral constants and pointers (so function pointers work).
        // In our case we check that &C::serialize has the same signature as the first argument!
        // reallyHas<std::string (C::*)(), &C::serialize> should be substituted by
        // reallyHas<std::string (C::*)(), std::string (C::*)() &C::serialize> and work!
        template <typename U, U u> struct reallyHas;

        // Two overloads for yes: one for the signature of a normal method, one is for the signature of a const method.
        // We accept a pointer to our helper struct, in order to avoid to instantiate a real instance of this type.
        // std::string (C::*)() is function pointer declaration.
        template <typename C> static yes& test(reallyHas<C & (C::*)(const V & a), (&C::operator/=)>* /*unused*/) { }
        template <typename C> static yes& test(reallyHas<C & (C::*)(const V & a) const, (&C::operator/=)>* /*unused*/) { }
        // The famous C++ sink-hole.
        // Note that sink-hole must be templated too as we are testing test<T>(0).
        // If the method serialize isn't available, we will end up in this method.
        template <typename> static no& test(...) { /* dark matter */ }

        // The constant used as a return value for the test.
        // The test is actually done here, thanks to the sizeof compile-time evaluation.
        static const bool value = (sizeof(test<T>(0)) == sizeof(yes) || PDP::IsPrimitiveType<T>::value);
    };

    template<typename T> using HasCompoundDivisionOperand_V = typename HasCompoundDivisionOperand<T>::value;

    //|| (PDP::IsIntegrelType<T>::value && !PDP::IsFloat<T>::value && !PDP::IsDoubleExtended64Bit<T>::value && !PDP::IsDouble<T>::value)
    template <typename T>
    struct HasModulusOperand
    {
        // For the compile time comparison.
        typedef char yes[1];
        typedef yes no[2];

        // This helper struct permits us to check that serialize is truly a method.
        // The second argument must be of the type of the first.
        // For instance reallyHas<int, 10> would be substituted by reallyHas<int, int 10> and works!
        // reallyHas<int, &C::serialize> would be substituted by reallyHas<int, int &C::serialize> and fail!
        // Note: It only works with integral constants and pointers (so function pointers work).
        // In our case we check that &C::serialize has the same signature as the first argument!
        // reallyHas<std::string (C::*)(), &C::serialize> should be substituted by
        // reallyHas<std::string (C::*)(), std::string (C::*)() &C::serialize> and work!
        template <typename U, U u> struct reallyHas;

        // Two overloads for yes: one for the signature of a normal method, one is for the signature of a const method.
        // We accept a pointer to our helper struct, in order to avoid to instantiate a real instance of this type.
        // std::string (C::*)() is function pointer declaration.
        template <typename C> static yes& test(reallyHas<C (C::*)(const C & a), (&C::operator%)>* /*unused*/) { }
        template <typename C> static yes& test(reallyHas<C  (C::*)(const C & a) const, (&C::operator%)>* /*unused*/) { }
        // The famous C++ sink-hole.
        // Note that sink-hole must be templated too as we are testing test<T>(0).
        // If the method serialize isn't available, we will end up in this method.
        template <typename> static no& test(...) { /* dark matter */ }

        // The constant used as a return value for the test.
        // The test is actually done here, thanks to the sizeof compile-time evaluation.
        static const bool value = (sizeof(test<T>(0)) == sizeof(yes) ||(PDP::IsPrimitiveType<T>::value && !PDP::IsFloat<T>::value && !PDP::IsDouble<T>::value && ~PDP::IsDoubleExtended64Bit<T>::value));
    };

    template <typename T, typename V>
    struct HasModulusOperandPair
    {
        // For the compile time comparison.
        typedef char yes[1];
        typedef yes no[2];

        // This helper struct permits us to check that serialize is truly a method.
        // The second argument must be of the type of the first.
        // For instance reallyHas<int, 10> would be substituted by reallyHas<int, int 10> and works!
        // reallyHas<int, &C::serialize> would be substituted by reallyHas<int, int &C::serialize> and fail!
        // Note: It only works with integral constants and pointers (so function pointers work).
        // In our case we check that &C::serialize has the same signature as the first argument!
        // reallyHas<std::string (C::*)(), &C::serialize> should be substituted by
        // reallyHas<std::string (C::*)(), std::string (C::*)() &C::serialize> and work!
        template <typename U, U u> struct reallyHas;

        // Two overloads for yes: one for the signature of a normal method, one is for the signature of a const method.
        // We accept a pointer to our helper struct, in order to avoid to instantiate a real instance of this type.
        // std::string (C::*)() is function pointer declaration.
        template <typename C> static yes& test(reallyHas<C (C::*)(const V & a), (&C::operator%)>* /*unused*/) { }
        template <typename C> static yes& test(reallyHas<C  (C::*)(const V & a) const, (&C::operator%)>* /*unused*/) { }
        // The famous C++ sink-hole.
        // Note that sink-hole must be templated too as we are testing test<T>(0).
        // If the method serialize isn't available, we will end up in this method.
        template <typename> static no& test(...) { /* dark matter */ }

        // The constant used as a return value for the test.
        // The test is actually done here, thanks to the sizeof compile-time evaluation.
        static const bool value = (sizeof(test<T>(0)) == sizeof(yes) ||(PDP::IsPrimitiveType<T>::value && !PDP::IsFloat<T>::value && !PDP::IsDouble<T>::value && ~PDP::IsDoubleExtended64Bit<T>::value));
    };

    template<typename T> using HasModulusOperand_V = typename HasModulusOperand<T>::value;


    template <typename T>
    struct HasCompoundModulusOperand
    {
        // For the compile time comparison.
        typedef char yes[1];
        typedef yes no[2];

        // This helper struct permits us to check that serialize is truly a method.
        // The second argument must be of the type of the first.
        // For instance reallyHas<int, 10> would be substituted by reallyHas<int, int 10> and works!
        // reallyHas<int, &C::serialize> would be substituted by reallyHas<int, int &C::serialize> and fail!
        // Note: It only works with integral constants and pointers (so function pointers work).
        // In our case we check that &C::serialize has the same signature as the first argument!
        // reallyHas<std::string (C::*)(), &C::serialize> should be substituted by
        // reallyHas<std::string (C::*)(), std::string (C::*)() &C::serialize> and work!
        template <typename U, U u> struct reallyHas;

        // Two overloads for yes: one for the signature of a normal method, one is for the signature of a const method.
        // We accept a pointer to our helper struct, in order to avoid to instantiate a real instance of this type.
        // std::string (C::*)() is function pointer declaration.
        template <typename C> static yes& test(reallyHas<C & (C::*)(const C & a), (&C::operator%=)>* /*unused*/) { }
        template <typename C> static yes& test(reallyHas<C & (C::*)(const C & a) const, (&C::operator%=)>* /*unused*/) { }
        // The famous C++ sink-hole.
        // Note that sink-hole must be templated too as we are testing test<T>(0).
        // If the method serialize isn't available, we will end up in this method.
        template <typename> static no& test(...) { /* dark matter */ }

        // The constant used as a return value for the test.
        // The test is actually done here, thanks to the sizeof compile-time evaluation.
        static const bool value = (sizeof(test<T>(0)) == sizeof(yes) ||(PDP::IsPrimitiveType<T>::value && !PDP::IsFloat<T>::value && !PDP::IsDouble<T>::value && ~PDP::IsDoubleExtended64Bit<T>::value));
    };


    template <typename T, typename V>
    struct HasCompoundModulusOperandPair
    {
        // For the compile time comparison.
        typedef char yes[1];
        typedef yes no[2];

        // This helper struct permits us to check that serialize is truly a method.
        // The second argument must be of the type of the first.
        // For instance reallyHas<int, 10> would be substituted by reallyHas<int, int 10> and works!
        // reallyHas<int, &C::serialize> would be substituted by reallyHas<int, int &C::serialize> and fail!
        // Note: It only works with integral constants and pointers (so function pointers work).
        // In our case we check that &C::serialize has the same signature as the first argument!
        // reallyHas<std::string (C::*)(), &C::serialize> should be substituted by
        // reallyHas<std::string (C::*)(), std::string (C::*)() &C::serialize> and work!
        template <typename U, U u> struct reallyHas;

        // Two overloads for yes: one for the signature of a normal method, one is for the signature of a const method.
        // We accept a pointer to our helper struct, in order to avoid to instantiate a real instance of this type.
        // std::string (C::*)() is function pointer declaration.
        template <typename C> static yes& test(reallyHas<C & (C::*)(const V & a), (&C::operator%=)>* /*unused*/) { }
        template <typename C> static yes& test(reallyHas<C & (C::*)(const V & a) const, (&C::operator%=)>* /*unused*/) { }
        // The famous C++ sink-hole.
        // Note that sink-hole must be templated too as we are testing test<T>(0).
        // If the method serialize isn't available, we will end up in this method.
        template <typename> static no& test(...) { /* dark matter */ }

        // The constant used as a return value for the test.
        // The test is actually done here, thanks to the sizeof compile-time evaluation.
        static const bool value = (sizeof(test<T>(0)) == sizeof(yes) ||(PDP::IsPrimitiveType<T>::value && !PDP::IsFloat<T>::value && !PDP::IsDouble<T>::value && ~PDP::IsDoubleExtended64Bit<T>::value));
    };

    template<typename T> using HasCompoundModulusOperand_V = typename HasCompoundModulusOperand<T>::value;

    template<typename T>
    struct HasBitWiseNotOperand
    {
        // For the compile time comparison.
        typedef char yes[1];
        typedef yes no[2];

        // This helper struct permits us to check that serialize is truly a method.
        // The second argument must be of the type of the first.
        // For instance reallyHas<int, 10> would be substituted by reallyHas<int, int 10> and works!
        // reallyHas<int, &C::serialize> would be substituted by reallyHas<int, int &C::serialize> and fail!
        // Note: It only works with integral constants and pointers (so function pointers work).
        // In our case we check that &C::serialize has the same signature as the first argument!
        // reallyHas<std::string (C::*)(), &C::serialize> should be substituted by
        // reallyHas<std::string (C::*)(), std::string (C::*)() &C::serialize> and work!
        template <typename U, U u> struct reallyHas;

        // Two overloads for yes: one for the signature of a normal method, one is for the signature of a const method.
        // We accept a pointer to our helper struct, in order to avoid to instantiate a real instance of this type.
        // std::string (C::*)() is function pointer declaration.
        template <typename C> static yes& test(reallyHas<C  (C::*)(const C & a), (&C::operator!)>* /*unused*/) { }
        template <typename C> static yes& test(reallyHas<C  (C::*)(const C & a) const, (&C::operator!)>* /*unused*/) { }
        // The famous C++ sink-hole.
        // Note that sink-hole must be templated too as we are testing test<T>(0).
        // If the method serialize isn't available, we will end up in this method.
        template <typename> static no& test(...) { /* dark matter */ }

        // The constant used as a return value for the test.
        // The test is actually done here, thanks to the sizeof compile-time evaluation.
        static const bool value = (sizeof(test<T>(0)) == sizeof(yes) ||(PDP::IsPrimitiveType<T>::value && !PDP::IsFloat<T>::value && !PDP::IsDouble<T>::value && ~PDP::IsDoubleExtended64Bit<T>::value));
    };

    template<typename T, typename V>
    struct HasBitWiseNotOperandPair
    {
        // For the compile time comparison.
        typedef char yes[1];
        typedef yes no[2];

        // This helper struct permits us to check that serialize is truly a method.
        // The second argument must be of the type of the first.
        // For instance reallyHas<int, 10> would be substituted by reallyHas<int, int 10> and works!
        // reallyHas<int, &C::serialize> would be substituted by reallyHas<int, int &C::serialize> and fail!
        // Note: It only works with integral constants and pointers (so function pointers work).
        // In our case we check that &C::serialize has the same signature as the first argument!
        // reallyHas<std::string (C::*)(), &C::serialize> should be substituted by
        // reallyHas<std::string (C::*)(), std::string (C::*)() &C::serialize> and work!
        template <typename U, U u> struct reallyHas;

        // Two overloads for yes: one for the signature of a normal method, one is for the signature of a const method.
        // We accept a pointer to our helper struct, in order to avoid to instantiate a real instance of this type.
        // std::string (C::*)() is function pointer declaration.
        template <typename C> static yes& test(reallyHas<C  (C::*)(const V & a), (&C::operator!)>* /*unused*/) { }
        template <typename C> static yes& test(reallyHas<C  (C::*)(const V & a) const, (&C::operator!)>* /*unused*/) { }
        // The famous C++ sink-hole.
        // Note that sink-hole must be templated too as we are testing test<T>(0).
        // If the method serialize isn't available, we will end up in this method.
        template <typename> static no& test(...) { /* dark matter */ }

        // The constant used as a return value for the test.
        // The test is actually done here, thanks to the sizeof compile-time evaluation.
        static const bool value = (sizeof(test<T>(0)) == sizeof(yes) ||(PDP::IsPrimitiveType<T>::value && !PDP::IsFloat<T>::value && !PDP::IsDouble<T>::value && ~PDP::IsDoubleExtended64Bit<T>::value));
    };

    template<typename T> using HasBitWiseNotOperand_V = typename HasBitWiseNotOperand<T>::value;

    template<typename T>
    struct HasBitWiseAndOperand
    {
        // For the compile time comparison.
        typedef char yes[1];
        typedef yes no[2];

        // This helper struct permits us to check that serialize is truly a method.
        // The second argument must be of the type of the first.
        // For instance reallyHas<int, 10> would be substituted by reallyHas<int, int 10> and works!
        // reallyHas<int, &C::serialize> would be substituted by reallyHas<int, int &C::serialize> and fail!
        // Note: It only works with integral constants and pointers (so function pointers work).
        // In our case we check that &C::serialize has the same signature as the first argument!
        // reallyHas<std::string (C::*)(), &C::serialize> should be substituted by
        // reallyHas<std::string (C::*)(), std::string (C::*)() &C::serialize> and work!
        template <typename U, U u> struct reallyHas;

        // Two overloads for yes: one for the signature of a normal method, one is for the signature of a const method.
        // We accept a pointer to our helper struct, in order to avoid to instantiate a real instance of this type.
        // std::string (C::*)() is function pointer declaration.
        template <typename C> static yes& test(reallyHas<C  (C::*)(const C & a), (&C::operator&)>* /*unused*/) { }
        template <typename C> static yes& test(reallyHas<C  (C::*)(const C & a) const, (&C::operator&)>* /*unused*/) { }
        // The famous C++ sink-hole.
        // Note that sink-hole must be templated too as we are testing test<T>(0).
        // If the method serialize isn't available, we will end up in this method.
        template <typename> static no& test(...) { /* dark matter */ }

        // The constant used as a return value for the test.
        // The test is actually done here, thanks to the sizeof compile-time evaluation.
        static const bool value = (sizeof(test<T>(0)) == sizeof(yes) || PDP::IsPrimitiveType<T>::value);
    };


    template<typename T, typename V>
    struct HasBitWiseAndOperandPair
    {
        // For the compile time comparison.
        typedef char yes[1];
        typedef yes no[2];

        // This helper struct permits us to check that serialize is truly a method.
        // The second argument must be of the type of the first.
        // For instance reallyHas<int, 10> would be substituted by reallyHas<int, int 10> and works!
        // reallyHas<int, &C::serialize> would be substituted by reallyHas<int, int &C::serialize> and fail!
        // Note: It only works with integral constants and pointers (so function pointers work).
        // In our case we check that &C::serialize has the same signature as the first argument!
        // reallyHas<std::string (C::*)(), &C::serialize> should be substituted by
        // reallyHas<std::string (C::*)(), std::string (C::*)() &C::serialize> and work!
        template <typename U, U u> struct reallyHas;

        // Two overloads for yes: one for the signature of a normal method, one is for the signature of a const method.
        // We accept a pointer to our helper struct, in order to avoid to instantiate a real instance of this type.
        // std::string (C::*)() is function pointer declaration.
        template <typename C> static yes& test(reallyHas<C  (C::*)(const V & a), (&C::operator&)>* /*unused*/) { }
        template <typename C> static yes& test(reallyHas<C  (C::*)(const V & a) const, (&C::operator&)>* /*unused*/) { }
        // The famous C++ sink-hole.
        // Note that sink-hole must be templated too as we are testing test<T>(0).
        // If the method serialize isn't available, we will end up in this method.
        template <typename> static no& test(...) { /* dark matter */ }

        // The constant used as a return value for the test.
        // The test is actually done here, thanks to the sizeof compile-time evaluation.
        static const bool value = (sizeof(test<T>(0)) == sizeof(yes) || PDP::IsPrimitiveType<T>::value);
    };

    template<typename T> using HasBitWiseAndOperand_V = typename HasBitWiseAndOperand<T>::value;

    template<typename T>
    struct HasCompoundBitWiseAndOperand
    {
        // For the compile time comparison.
        typedef char yes[1];
        typedef yes no[2];

        // This helper struct permits us to check that serialize is truly a method.
        // The second argument must be of the type of the first.
        // For instance reallyHas<int, 10> would be substituted by reallyHas<int, int 10> and works!
        // reallyHas<int, &C::serialize> would be substituted by reallyHas<int, int &C::serialize> and fail!
        // Note: It only works with integral constants and pointers (so function pointers work).
        // In our case we check that &C::serialize has the same signature as the first argument!
        // reallyHas<std::string (C::*)(), &C::serialize> should be substituted by
        // reallyHas<std::string (C::*)(), std::string (C::*)() &C::serialize> and work!
        template <typename U, U u> struct reallyHas;

        // Two overloads for yes: one for the signature of a normal method, one is for the signature of a const method.
        // We accept a pointer to our helper struct, in order to avoid to instantiate a real instance of this type.
        // std::string (C::*)() is function pointer declaration.
        template <typename C> static yes& test(reallyHas<C &  (C::*)(const C & a), (&C::operator&=)>* /*unused*/) { }
        template <typename C> static yes& test(reallyHas<C & (C::*)(const C & a) const, (&C::operator&=)>* /*unused*/) { }
        // The famous C++ sink-hole.
        // Note that sink-hole must be templated too as we are testing test<T>(0).
        // If the method serialize isn't available, we will end up in this method.
        template <typename> static no& test(...) { /* dark matter */ }

        // The constant used as a return value for the test.
        // The test is actually done here, thanks to the sizeof compile-time evaluation.
        static const bool value = (sizeof(test<T>(0)) == sizeof(yes) || PDP::IsPrimitiveType<T>::value);
    };


    template<typename T, typename V>
    struct HasCompoundBitWiseAndOperandPair
    {
        // For the compile time comparison.
        typedef char yes[1];
        typedef yes no[2];

        // This helper struct permits us to check that serialize is truly a method.
        // The second argument must be of the type of the first.
        // For instance reallyHas<int, 10> would be substituted by reallyHas<int, int 10> and works!
        // reallyHas<int, &C::serialize> would be substituted by reallyHas<int, int &C::serialize> and fail!
        // Note: It only works with integral constants and pointers (so function pointers work).
        // In our case we check that &C::serialize has the same signature as the first argument!
        // reallyHas<std::string (C::*)(), &C::serialize> should be substituted by
        // reallyHas<std::string (C::*)(), std::string (C::*)() &C::serialize> and work!
        template <typename U, U u> struct reallyHas;

        // Two overloads for yes: one for the signature of a normal method, one is for the signature of a const method.
        // We accept a pointer to our helper struct, in order to avoid to instantiate a real instance of this type.
        // std::string (C::*)() is function pointer declaration.
        template <typename C> static yes& test(reallyHas<C &  (C::*)(const V & a), (&C::operator&=)>* /*unused*/) { }
        template <typename C> static yes& test(reallyHas<C & (C::*)(const V & a) const, (&C::operator&=)>* /*unused*/) { }
        // The famous C++ sink-hole.
        // Note that sink-hole must be templated too as we are testing test<T>(0).
        // If the method serialize isn't available, we will end up in this method.
        template <typename> static no& test(...) { /* dark matter */ }

        // The constant used as a return value for the test.
        // The test is actually done here, thanks to the sizeof compile-time evaluation.
        static const bool value = (sizeof(test<T>(0)) == sizeof(yes) || PDP::IsPrimitiveType<T>::value);
    };
    template<typename T> using HasCompoundBitWiseAndOperand_V = typename HasCompoundBitWiseAndOperand<T>::value;

    template<typename T>
    struct HasBitWiseOrOperand
    {
        // For the compile time comparison.
        typedef char yes[1];
        typedef yes no[2];

        // This helper struct permits us to check that serialize is truly a method.
        // The second argument must be of the type of the first.
        // For instance reallyHas<int, 10> would be substituted by reallyHas<int, int 10> and works!
        // reallyHas<int, &C::serialize> would be substituted by reallyHas<int, int &C::serialize> and fail!
        // Note: It only works with integral constants and pointers (so function pointers work).
        // In our case we check that &C::serialize has the same signature as the first argument!
        // reallyHas<std::string (C::*)(), &C::serialize> should be substituted by
        // reallyHas<std::string (C::*)(), std::string (C::*)() &C::serialize> and work!
        template <typename U, U u> struct reallyHas;

        // Two overloads for yes: one for the signature of a normal method, one is for the signature of a const method.
        // We accept a pointer to our helper struct, in order to avoid to instantiate a real instance of this type.
        // std::string (C::*)() is function pointer declaration.
        template <typename C> static yes& test(reallyHas<C   (C::*)(const C & a), (&C::operator|)>* /*unused*/) { }
        template <typename C> static yes& test(reallyHas<C  (C::*)(const C & a) const, (&C::operator|)>* /*unused*/) { }
        // The famous C++ sink-hole.
        // Note that sink-hole must be templated too as we are testing test<T>(0).
        // If the method serialize isn't available, we will end up in this method.
        template <typename> static no& test(...) { /* dark matter */ }

        // The constant used as a return value for the test.
        // The test is actually done here, thanks to the sizeof compile-time evaluation.
        static const bool value = (sizeof(test<T>(0)) == sizeof(yes) || PDP::IsPrimitiveType<T>::value);
    };

    template<typename T, typename V>
    struct HasBitWiseOrOperandPair
    {
        // For the compile time comparison.
        typedef char yes[1];
        typedef yes no[2];

        // This helper struct permits us to check that serialize is truly a method.
        // The second argument must be of the type of the first.
        // For instance reallyHas<int, 10> would be substituted by reallyHas<int, int 10> and works!
        // reallyHas<int, &C::serialize> would be substituted by reallyHas<int, int &C::serialize> and fail!
        // Note: It only works with integral constants and pointers (so function pointers work).
        // In our case we check that &C::serialize has the same signature as the first argument!
        // reallyHas<std::string (C::*)(), &C::serialize> should be substituted by
        // reallyHas<std::string (C::*)(), std::string (C::*)() &C::serialize> and work!
        template <typename U, U u> struct reallyHas;

        // Two overloads for yes: one for the signature of a normal method, one is for the signature of a const method.
        // We accept a pointer to our helper struct, in order to avoid to instantiate a real instance of this type.
        // std::string (C::*)() is function pointer declaration.
        template <typename C> static yes& test(reallyHas<C   (C::*)(const V & a), (&C::operator|)>* /*unused*/) { }
        template <typename C> static yes& test(reallyHas<C  (C::*)(const V & a) const, (&C::operator|)>* /*unused*/) { }
        // The famous C++ sink-hole.
        // Note that sink-hole must be templated too as we are testing test<T>(0).
        // If the method serialize isn't available, we will end up in this method.
        template <typename> static no& test(...) { /* dark matter */ }

        // The constant used as a return value for the test.
        // The test is actually done here, thanks to the sizeof compile-time evaluation.
        static const bool value = (sizeof(test<T>(0)) == sizeof(yes) || PDP::IsPrimitiveType<T>::value);
    };

    template<typename T> using HasBitWiseOrOperand_V = typename HasBitWiseOrOperand<T>::value;

    template<typename T>
    struct HasCompoundBitWiseOrOperand
    {
        // For the compile time comparison.
        typedef char yes[1];
        typedef yes no[2];

        // This helper struct permits us to check that serialize is truly a method.
        // The second argument must be of the type of the first.
        // For instance reallyHas<int, 10> would be substituted by reallyHas<int, int 10> and works!
        // reallyHas<int, &C::serialize> would be substituted by reallyHas<int, int &C::serialize> and fail!
        // Note: It only works with integral constants and pointers (so function pointers work).
        // In our case we check that &C::serialize has the same signature as the first argument!
        // reallyHas<std::string (C::*)(), &C::serialize> should be substituted by
        // reallyHas<std::string (C::*)(), std::string (C::*)() &C::serialize> and work!
        template <typename U, U u> struct reallyHas;

        // Two overloads for yes: one for the signature of a normal method, one is for the signature of a const method.
        // We accept a pointer to our helper struct, in order to avoid to instantiate a real instance of this type.
        // std::string (C::*)() is function pointer declaration.
        template <typename C> static yes& test(reallyHas<C &   (C::*)(const C & a), (&C::operator|=)>* /*unused*/) { }
        template <typename C> static yes& test(reallyHas<C & (C::*)(const C & a) const, (&C::operator|=)>* /*unused*/) { }
        // The famous C++ sink-hole.
        // Note that sink-hole must be templated too as we are testing test<T>(0).
        // If the method serialize isn't available, we will end up in this method.
        template <typename> static no& test(...) { /* dark matter */ }

        // The constant used as a return value for the test.
        // The test is actually done here, thanks to the sizeof compile-time evaluation.
        static const bool value = (sizeof(test<T>(0)) == sizeof(yes) || PDP::IsPrimitiveType<T>::value);
    };


    template<typename T, typename V>
    struct HasCompoundBitWiseOrOperandPair
    {
        // For the compile time comparison.
        typedef char yes[1];
        typedef yes no[2];

        // This helper struct permits us to check that serialize is truly a method.
        // The second argument must be of the type of the first.
        // For instance reallyHas<int, 10> would be substituted by reallyHas<int, int 10> and works!
        // reallyHas<int, &C::serialize> would be substituted by reallyHas<int, int &C::serialize> and fail!
        // Note: It only works with integral constants and pointers (so function pointers work).
        // In our case we check that &C::serialize has the same signature as the first argument!
        // reallyHas<std::string (C::*)(), &C::serialize> should be substituted by
        // reallyHas<std::string (C::*)(), std::string (C::*)() &C::serialize> and work!
        template <typename U, U u> struct reallyHas;

        // Two overloads for yes: one for the signature of a normal method, one is for the signature of a const method.
        // We accept a pointer to our helper struct, in order to avoid to instantiate a real instance of this type.
        // std::string (C::*)() is function pointer declaration.
        template <typename C> static yes& test(reallyHas<C &   (C::*)(const V & a), (&C::operator|=)>* /*unused*/) { }
        template <typename C> static yes& test(reallyHas<C & (C::*)(const V & a) const, (&C::operator|=)>* /*unused*/) { }
        // The famous C++ sink-hole.
        // Note that sink-hole must be templated too as we are testing test<T>(0).
        // If the method serialize isn't available, we will end up in this method.
        template <typename> static no& test(...) { /* dark matter */ }

        // The constant used as a return value for the test.
        // The test is actually done here, thanks to the sizeof compile-time evaluation.
        static const bool value = (sizeof(test<T>(0)) == sizeof(yes) || PDP::IsPrimitiveType<T>::value);
    };

    template<typename T> using HasCompoundBitWiseOrOperand_V = typename HasCompoundBitWiseOrOperand<T>::value;


    template<typename T>
    struct HasBitWiseXorOperand
    {
        // For the compile time comparison.
        typedef char yes[1];
        typedef yes no[2];

        // This helper struct permits us to check that serialize is truly a method.
        // The second argument must be of the type of the first.
        // For instance reallyHas<int, 10> would be substituted by reallyHas<int, int 10> and works!
        // reallyHas<int, &C::serialize> would be substituted by reallyHas<int, int &C::serialize> and fail!
        // Note: It only works with integral constants and pointers (so function pointers work).
        // In our case we check that &C::serialize has the same signature as the first argument!
        // reallyHas<std::string (C::*)(), &C::serialize> should be substituted by
        // reallyHas<std::string (C::*)(), std::string (C::*)() &C::serialize> and work!
        template <typename U, U u> struct reallyHas;

        // Two overloads for yes: one for the signature of a normal method, one is for the signature of a const method.
        // We accept a pointer to our helper struct, in order to avoid to instantiate a real instance of this type.
        // std::string (C::*)() is function pointer declaration.
        template <typename C> static yes& test(reallyHas<C    (C::*)(const C & a), (&C::operator^)>* /*unused*/) { }
        template <typename C> static yes& test(reallyHas<C  (C::*)(const C & a) const, (&C::operator^)>* /*unused*/) { }
        // The famous C++ sink-hole.
        // Note that sink-hole must be templated too as we are testing test<T>(0).
        // If the method serialize isn't available, we will end up in this method.
        template <typename> static no& test(...) { /* dark matter */ }

        // The constant used as a return value for the test.
        // The test is actually done here, thanks to the sizeof compile-time evaluation.
        static const bool value = (sizeof(test<T>(0)) == sizeof(yes) || PDP::IsPrimitiveType<T>::value);
    };

    template<typename T, typename V>
    struct HasBitWiseXorOperandPair
    {
        // For the compile time comparison.
        typedef char yes[1];
        typedef yes no[2];

        // This helper struct permits us to check that serialize is truly a method.
        // The second argument must be of the type of the first.
        // For instance reallyHas<int, 10> would be substituted by reallyHas<int, int 10> and works!
        // reallyHas<int, &C::serialize> would be substituted by reallyHas<int, int &C::serialize> and fail!
        // Note: It only works with integral constants and pointers (so function pointers work).
        // In our case we check that &C::serialize has the same signature as the first argument!
        // reallyHas<std::string (C::*)(), &C::serialize> should be substituted by
        // reallyHas<std::string (C::*)(), std::string (C::*)() &C::serialize> and work!
        template <typename U, U u> struct reallyHas;

        // Two overloads for yes: one for the signature of a normal method, one is for the signature of a const method.
        // We accept a pointer to our helper struct, in order to avoid to instantiate a real instance of this type.
        // std::string (C::*)() is function pointer declaration.
        template <typename C> static yes& test(reallyHas<C    (C::*)(const V & a), (&C::operator^)>* /*unused*/) { }
        template <typename C> static yes& test(reallyHas<C  (C::*)(const V & a) const, (&C::operator^)>* /*unused*/) { }
        // The famous C++ sink-hole.
        // Note that sink-hole must be templated too as we are testing test<T>(0).
        // If the method serialize isn't available, we will end up in this method.
        template <typename> static no& test(...) { /* dark matter */ }

        // The constant used as a return value for the test.
        // The test is actually done here, thanks to the sizeof compile-time evaluation.
        static const bool value = (sizeof(test<T>(0)) == sizeof(yes) || PDP::IsPrimitiveType<T>::value);
    };


    template<typename T> using HasBitWiseXorOperand_V = typename HasBitWiseXorOperand<T>::value;


    template<typename T>
    struct HasCompoundBitWiseXorOperand
    {
        // For the compile time comparison.
        typedef char yes[1];
        typedef yes no[2];

        // This helper struct permits us to check that serialize is truly a method.
        // The second argument must be of the type of the first.
        // For instance reallyHas<int, 10> would be substituted by reallyHas<int, int 10> and works!
        // reallyHas<int, &C::serialize> would be substituted by reallyHas<int, int &C::serialize> and fail!
        // Note: It only works with integral constants and pointers (so function pointers work).
        // In our case we check that &C::serialize has the same signature as the first argument!
        // reallyHas<std::string (C::*)(), &C::serialize> should be substituted by
        // reallyHas<std::string (C::*)(), std::string (C::*)() &C::serialize> and work!
        template <typename U, U u> struct reallyHas;

        // Two overloads for yes: one for the signature of a normal method, one is for the signature of a const method.
        // We accept a pointer to our helper struct, in order to avoid to instantiate a real instance of this type.
        // std::string (C::*)() is function pointer declaration.
        template <typename C> static yes& test(reallyHas<C &    (C::*)(const C & a), (&C::operator^=)>* /*unused*/) { }
        template <typename C> static yes& test(reallyHas<C &  (C::*)(const C & a) const, (&C::operator^=)>* /*unused*/) { }
        // The famous C++ sink-hole.
        // Note that sink-hole must be templated too as we are testing test<T>(0).
        // If the method serialize isn't available, we will end up in this method.
        template <typename> static no& test(...) { /* dark matter */ }

        // The constant used as a return value for the test.
        // The test is actually done here, thanks to the sizeof compile-time evaluation.
        static const bool value = (sizeof(test<T>(0)) == sizeof(yes) || PDP::IsPrimitiveType<T>::value);
    };


    template<typename T, typename V>
    struct HasCompoundBitWiseXorOperandPair
    {
        // For the compile time comparison.
        typedef char yes[1];
        typedef yes no[2];

        // This helper struct permits us to check that serialize is truly a method.
        // The second argument must be of the type of the first.
        // For instance reallyHas<int, 10> would be substituted by reallyHas<int, int 10> and works!
        // reallyHas<int, &C::serialize> would be substituted by reallyHas<int, int &C::serialize> and fail!
        // Note: It only works with integral constants and pointers (so function pointers work).
        // In our case we check that &C::serialize has the same signature as the first argument!
        // reallyHas<std::string (C::*)(), &C::serialize> should be substituted by
        // reallyHas<std::string (C::*)(), std::string (C::*)() &C::serialize> and work!
        template <typename U, U u> struct reallyHas;

        // Two overloads for yes: one for the signature of a normal method, one is for the signature of a const method.
        // We accept a pointer to our helper struct, in order to avoid to instantiate a real instance of this type.
        // std::string (C::*)() is function pointer declaration.
        template <typename C> static yes& test(reallyHas<C &    (C::*)(const V & a), (&C::operator^=)>* /*unused*/) { }
        template <typename C> static yes& test(reallyHas<C &  (C::*)(const V & a) const, (&C::operator^=)>* /*unused*/) { }
        // The famous C++ sink-hole.
        // Note that sink-hole must be templated too as we are testing test<T>(0).
        // If the method serialize isn't available, we will end up in this method.
        template <typename> static no& test(...) { /* dark matter */ }

        // The constant used as a return value for the test.
        // The test is actually done here, thanks to the sizeof compile-time evaluation.
        static const bool value = (sizeof(test<T>(0)) == sizeof(yes) || PDP::IsPrimitiveType<T>::value);
    };


    template<typename T> using HasCompoundBitWiseXorOperand_V = typename HasCompoundBitWiseXorOperand<T>::value;


    template <class _Tp> struct  RemoveExtent
    {typedef _Tp type;};
    template <class _Tp> struct  RemoveExtent<_Tp[]>
    {typedef _Tp type;};
    template <class _Tp, LD::UInteger _Np> struct  RemoveExtent<_Tp[_Np]>
    {typedef _Tp type;};

#if _LIBCPP_STD_VER > 11
    template <class _Tp> using RemoveExtent_T = typename RemoveExtent<_Tp>::type;
#endif


    template<typename T>
    struct HasLeftBitShiftOperand
    {
        // For the compile time comparison.
        typedef char yes[1];
        typedef yes no[2];

        // This helper struct permits us to check that serialize is truly a method.
        // The second argument must be of the type of the first.
        // For instance reallyHas<int, 10> would be substituted by reallyHas<int, int 10> and works!
        // reallyHas<int, &C::serialize> would be substituted by reallyHas<int, int &C::serialize> and fail!
        // Note: It only works with integral constants and pointers (so function pointers work).
        // In our case we check that &C::serialize has the same signature as the first argument!
        // reallyHas<std::string (C::*)(), &C::serialize> should be substituted by
        // reallyHas<std::string (C::*)(), std::string (C::*)() &C::serialize> and work!
        template <typename U, U u> struct reallyHas;

        // Two overloads for yes: one for the signature of a normal method, one is for the signature of a const method.
        // We accept a pointer to our helper struct, in order to avoid to instantiate a real instance of this type.
        // std::string (C::*)() is function pointer declaration.
        template <typename C> static yes& test(reallyHas<C     (C::*)(const C & a), (&C::operator<<)>* /*unused*/) { }
        template <typename C> static yes& test(reallyHas<C   (C::*)(const C & a) const, (&C::operator<<)>* /*unused*/) { }
        // The famous C++ sink-hole.
        // Note that sink-hole must be templated too as we are testing test<T>(0).
        // If the method serialize isn't available, we will end up in this method.
        template <typename> static no& test(...) { /* dark matter */ }

        // The constant used as a return value for the test.
        // The test is actually done here, thanks to the sizeof compile-time evaluation.
        static const bool value = (sizeof(test<T>(0)) == sizeof(yes) || PDP::IsPrimitiveType<T>::value);
    };

    template<typename T, typename V>
    struct HasLeftBitShiftOperandPair
    {
        // For the compile time comparison.
        typedef char yes[1];
        typedef yes no[2];

        // This helper struct permits us to check that serialize is truly a method.
        // The second argument must be of the type of the first.
        // For instance reallyHas<int, 10> would be substituted by reallyHas<int, int 10> and works!
        // reallyHas<int, &C::serialize> would be substituted by reallyHas<int, int &C::serialize> and fail!
        // Note: It only works with integral constants and pointers (so function pointers work).
        // In our case we check that &C::serialize has the same signature as the first argument!
        // reallyHas<std::string (C::*)(), &C::serialize> should be substituted by
        // reallyHas<std::string (C::*)(), std::string (C::*)() &C::serialize> and work!
        template <typename U, U u> struct reallyHas;

        // Two overloads for yes: one for the signature of a normal method, one is for the signature of a const method.
        // We accept a pointer to our helper struct, in order to avoid to instantiate a real instance of this type.
        // std::string (C::*)() is function pointer declaration.
        template <typename C> static yes& test(reallyHas<C     (C::*)(const V & a), (&C::operator<<)>* /*unused*/) { }
        template <typename C> static yes& test(reallyHas<C   (C::*)(const V & a) const, (&C::operator<<)>* /*unused*/) { }
        // The famous C++ sink-hole.
        // Note that sink-hole must be templated too as we are testing test<T>(0).
        // If the method serialize isn't available, we will end up in this method.
        template <typename> static no& test(...) { /* dark matter */ }

        // The constant used as a return value for the test.
        // The test is actually done here, thanks to the sizeof compile-time evaluation.
        static const bool value = (sizeof(test<T>(0)) == sizeof(yes) || PDP::IsPrimitiveType<T>::value);
    };

    template<typename T> using HasLeftBitShiftOperand_V = typename HasLeftBitShiftOperand<T>::value;

    template<typename T>
    struct HasCompoundLeftBitShiftOperand
    {
        // For the compile time comparison.
        typedef char yes[1];
        typedef yes no[2];

        // This helper struct permits us to check that serialize is truly a method.
        // The second argument must be of the type of the first.
        // For instance reallyHas<int, 10> would be substituted by reallyHas<int, int 10> and works!
        // reallyHas<int, &C::serialize> would be substituted by reallyHas<int, int &C::serialize> and fail!
        // Note: It only works with integral constants and pointers (so function pointers work).
        // In our case we check that &C::serialize has the same signature as the first argument!
        // reallyHas<std::string (C::*)(), &C::serialize> should be substituted by
        // reallyHas<std::string (C::*)(), std::string (C::*)() &C::serialize> and work!
        template <typename U, U u> struct reallyHas;

        // Two overloads for yes: one for the signature of a normal method, one is for the signature of a const method.
        // We accept a pointer to our helper struct, in order to avoid to instantiate a real instance of this type.
        // std::string (C::*)() is function pointer declaration.
        template <typename C> static yes& test(reallyHas<C &    (C::*)(const C & a), (&C::operator<<=)>* /*unused*/) { }
        template <typename C> static yes& test(reallyHas<C &  (C::*)(const C & a) const, (&C::operator<<=)>* /*unused*/) { }
        // The famous C++ sink-hole.
        // Note that sink-hole must be templated too as we are testing test<T>(0).
        // If the method serialize isn't available, we will end up in this method.
        template <typename> static no& test(...) { /* dark matter */ }

        // The constant used as a return value for the test.
        // The test is actually done here, thanks to the sizeof compile-time evaluation.
        static const bool value = (sizeof(test<T>(0)) == sizeof(yes) || PDP::IsPrimitiveType<T>::value);
    };


    template<typename T, typename V>
    struct HasCompoundLeftBitShiftOperandPair
    {
        // For the compile time comparison.
        typedef char yes[1];
        typedef yes no[2];

        // This helper struct permits us to check that serialize is truly a method.
        // The second argument must be of the type of the first.
        // For instance reallyHas<int, 10> would be substituted by reallyHas<int, int 10> and works!
        // reallyHas<int, &C::serialize> would be substituted by reallyHas<int, int &C::serialize> and fail!
        // Note: It only works with integral constants and pointers (so function pointers work).
        // In our case we check that &C::serialize has the same signature as the first argument!
        // reallyHas<std::string (C::*)(), &C::serialize> should be substituted by
        // reallyHas<std::string (C::*)(), std::string (C::*)() &C::serialize> and work!
        template <typename U, U u> struct reallyHas;

        // Two overloads for yes: one for the signature of a normal method, one is for the signature of a const method.
        // We accept a pointer to our helper struct, in order to avoid to instantiate a real instance of this type.
        // std::string (C::*)() is function pointer declaration.
        template <typename C> static yes& test(reallyHas<C &    (C::*)(const V & a), (&C::operator<<=)>* /*unused*/) { }
        template <typename C> static yes& test(reallyHas<C &  (C::*)(const V & a) const, (&C::operator<<=)>* /*unused*/) { }
        // The famous C++ sink-hole.
        // Note that sink-hole must be templated too as we are testing test<T>(0).
        // If the method serialize isn't available, we will end up in this method.
        template <typename> static no& test(...) { /* dark matter */ }

        // The constant used as a return value for the test.
        // The test is actually done here, thanks to the sizeof compile-time evaluation.
        static const bool value = (sizeof(test<T>(0)) == sizeof(yes) || PDP::IsPrimitiveType<T>::value);
    };

    template<typename T> using HasCompoundLeftBitShiftOperand_V = typename HasCompoundLeftBitShiftOperand<T>::value;

    template<typename T>
    struct HasRightBitShiftOperand
    {
        // For the compile time comparison.
        typedef char yes[1];
        typedef yes no[2];

        // This helper struct permits us to check that serialize is truly a method.
        // The second argument must be of the type of the first.
        // For instance reallyHas<int, 10> would be substituted by reallyHas<int, int 10> and works!
        // reallyHas<int, &C::serialize> would be substituted by reallyHas<int, int &C::serialize> and fail!
        // Note: It only works with integral constants and pointers (so function pointers work).
        // In our case we check that &C::serialize has the same signature as the first argument!
        // reallyHas<std::string (C::*)(), &C::serialize> should be substituted by
        // reallyHas<std::string (C::*)(), std::string (C::*)() &C::serialize> and work!
        template <typename U, U u> struct reallyHas;

        // Two overloads for yes: one for the signature of a normal method, one is for the signature of a const method.
        // We accept a pointer to our helper struct, in order to avoid to instantiate a real instance of this type.
        // std::string (C::*)() is function pointer declaration.
        template <typename C> static yes& test(reallyHas<C &    (C::*)(const C & a), (&C::operator>>)>* /*unused*/) { }
        template <typename C> static yes& test(reallyHas<C &  (C::*)(const C & a) const, (&C::operator>>)>* /*unused*/) { }
        // The famous C++ sink-hole.
        // Note that sink-hole must be templated too as we are testing test<T>(0).
        // If the method serialize isn't available, we will end up in this method.
        template <typename> static no& test(...) { /* dark matter */ }

        // The constant used as a return value for the test.
        // The test is actually done here, thanks to the sizeof compile-time evaluation.
        static const bool value = (sizeof(test<T>(0)) == sizeof(yes) || PDP::IsPrimitiveType<T>::value);
    };


    template<typename T, typename V>
    struct HasRightBitShiftOperandPair
    {
        // For the compile time comparison.
        typedef char yes[1];
        typedef yes no[2];

        // This helper struct permits us to check that serialize is truly a method.
        // The second argument must be of the type of the first.
        // For instance reallyHas<int, 10> would be substituted by reallyHas<int, int 10> and works!
        // reallyHas<int, &C::serialize> would be substituted by reallyHas<int, int &C::serialize> and fail!
        // Note: It only works with integral constants and pointers (so function pointers work).
        // In our case we check that &C::serialize has the same signature as the first argument!
        // reallyHas<std::string (C::*)(), &C::serialize> should be substituted by
        // reallyHas<std::string (C::*)(), std::string (C::*)() &C::serialize> and work!
        template <typename U, U u> struct reallyHas;

        // Two overloads for yes: one for the signature of a normal method, one is for the signature of a const method.
        // We accept a pointer to our helper struct, in order to avoid to instantiate a real instance of this type.
        // std::string (C::*)() is function pointer declaration.
        template <typename C> static yes& test(reallyHas<C &    (C::*)(const V & a), (&C::operator>>)>* /*unused*/) { }
        template <typename C> static yes& test(reallyHas<C &  (C::*)(const V & a) const, (&C::operator>>)>* /*unused*/) { }
        // The famous C++ sink-hole.
        // Note that sink-hole must be templated too as we are testing test<T>(0).
        // If the method serialize isn't available, we will end up in this method.
        template <typename> static no& test(...) { /* dark matter */ }

        // The constant used as a return value for the test.
        // The test is actually done here, thanks to the sizeof compile-time evaluation.
        static const bool value = (sizeof(test<T>(0)) == sizeof(yes) || PDP::IsPrimitiveType<T>::value);
    };

    template<typename T> using HasRightBitShiftOperand_V = typename HasRightBitShiftOperand<T>::value;



    template<typename T>
    struct HasCompoundRightBitShiftOperand
    {
        // For the compile time comparison.
        typedef char yes[1];
        typedef yes no[2];

        // This helper struct permits us to check that serialize is truly a method.
        // The second argument must be of the type of the first.
        // For instance reallyHas<int, 10> would be substituted by reallyHas<int, int 10> and works!
        // reallyHas<int, &C::serialize> would be substituted by reallyHas<int, int &C::serialize> and fail!
        // Note: It only works with integral constants and pointers (so function pointers work).
        // In our case we check that &C::serialize has the same signature as the first argument!
        // reallyHas<std::string (C::*)(), &C::serialize> should be substituted by
        // reallyHas<std::string (C::*)(), std::string (C::*)() &C::serialize> and work!
        template <typename U, U u> struct reallyHas;

        // Two overloads for yes: one for the signature of a normal method, one is for the signature of a const method.
        // We accept a pointer to our helper struct, in order to avoid to instantiate a real instance of this type.
        // std::string (C::*)() is function pointer declaration.
        template <typename C> static yes& test(reallyHas<C &    (C::*)(const C & a), (&C::operator>>)>* /*unused*/) { }
        template <typename C> static yes& test(reallyHas<C &  (C::*)(const C & a) const, (&C::operator>>)>* /*unused*/) { }
        // The famous C++ sink-hole.
        // Note that sink-hole must be templated too as we are testing test<T>(0).
        // If the method serialize isn't available, we will end up in this method.
        template <typename> static no& test(...) { /* dark matter */ }

        // The constant used as a return value for the test.
        // The test is actually done here, thanks to the sizeof compile-time evaluation.
        static const bool value = (sizeof(test<T>(0)) == sizeof(yes) || PDP::IsPrimitiveType<T>::value);
    };


    template<typename T, typename V>
    struct HasCompoundRightBitShiftOperandPair
    {
        // For the compile time comparison.
        typedef char yes[1];
        typedef yes no[2];

        // This helper struct permits us to check that serialize is truly a method.
        // The second argument must be of the type of the first.
        // For instance reallyHas<int, 10> would be substituted by reallyHas<int, int 10> and works!
        // reallyHas<int, &C::serialize> would be substituted by reallyHas<int, int &C::serialize> and fail!
        // Note: It only works with integral constants and pointers (so function pointers work).
        // In our case we check that &C::serialize has the same signature as the first argument!
        // reallyHas<std::string (C::*)(), &C::serialize> should be substituted by
        // reallyHas<std::string (C::*)(), std::string (C::*)() &C::serialize> and work!
        template <typename U, U u> struct reallyHas;

        // Two overloads for yes: one for the signature of a normal method, one is for the signature of a const method.
        // We accept a pointer to our helper struct, in order to avoid to instantiate a real instance of this type.
        // std::string (C::*)() is function pointer declaration.
        template <typename C> static yes& test(reallyHas<C &    (C::*)(const V & a), (&C::operator>>)>* /*unused*/) { }
        template <typename C> static yes& test(reallyHas<C &  (C::*)(const V & a) const, (&C::operator>>)>* /*unused*/) { }
        // The famous C++ sink-hole.
        // Note that sink-hole must be templated too as we are testing test<T>(0).
        // If the method serialize isn't available, we will end up in this method.
        template <typename> static no& test(...) { /* dark matter */ }

        // The constant used as a return value for the test.
        // The test is actually done here, thanks to the sizeof compile-time evaluation.
        static const bool value = (sizeof(test<T>(0)) == sizeof(yes) || PDP::IsPrimitiveType<T>::value);
    };
    template<typename T> using HasCompoundRightBitShiftOperand_V = typename HasCompoundRightBitShiftOperand<T>::value;


    template<typename T>
    struct HasLogicalAndOperand
    {
        // For the compile time comparison.
        typedef char yes[1];
        typedef yes no[2];

        // This helper struct permits us to check that serialize is truly a method.
        // The second argument must be of the type of the first.
        // For instance reallyHas<int, 10> would be substituted by reallyHas<int, int 10> and works!
        // reallyHas<int, &C::serialize> would be substituted by reallyHas<int, int &C::serialize> and fail!
        // Note: It only works with integral constants and pointers (so function pointers work).
        // In our case we check that &C::serialize has the same signature as the first argument!
        // reallyHas<std::string (C::*)(), &C::serialize> should be substituted by
        // reallyHas<std::string (C::*)(), std::string (C::*)() &C::serialize> and work!
        template <typename U, U u> struct reallyHas;

        // Two overloads for yes: one for the signature of a normal method, one is for the signature of a const method.
        // We accept a pointer to our helper struct, in order to avoid to instantiate a real instance of this type.
        // std::string (C::*)() is function pointer declaration.
        template <typename C> static yes& test(reallyHas<const bool    (C::*)(const C & a), (&C::operator&&)>* /*unused*/) { }
        template <typename C> static yes& test(reallyHas<const bool  (C::*)(const C & a) const, (&C::operator&&)>* /*unused*/) { }
        // The famous C++ sink-hole.
        // Note that sink-hole must be templated too as we are testing test<T>(0).
        // If the method serialize isn't available, we will end up in this method.
        template <typename> static no& test(...) { /* dark matter */ }

        // The constant used as a return value for the test.
        // The test is actually done here, thanks to the sizeof compile-time evaluation.
        static const bool value = (sizeof(test<T>(0)) == sizeof(yes) || PDP::IsPrimitiveType<T>::value);
    };


    template<typename T, typename V>
    struct HasLogicalAndOperandPair
    {
        // For the compile time comparison.
        typedef char yes[1];
        typedef yes no[2];

        // This helper struct permits us to check that serialize is truly a method.
        // The second argument must be of the type of the first.
        // For instance reallyHas<int, 10> would be substituted by reallyHas<int, int 10> and works!
        // reallyHas<int, &C::serialize> would be substituted by reallyHas<int, int &C::serialize> and fail!
        // Note: It only works with integral constants and pointers (so function pointers work).
        // In our case we check that &C::serialize has the same signature as the first argument!
        // reallyHas<std::string (C::*)(), &C::serialize> should be substituted by
        // reallyHas<std::string (C::*)(), std::string (C::*)() &C::serialize> and work!
        template <typename U, U u> struct reallyHas;

        // Two overloads for yes: one for the signature of a normal method, one is for the signature of a const method.
        // We accept a pointer to our helper struct, in order to avoid to instantiate a real instance of this type.
        // std::string (C::*)() is function pointer declaration.
        template <typename C> static yes& test(reallyHas<const bool    (C::*)(const V & a), (&C::operator&&)>* /*unused*/) { }
        template <typename C> static yes& test(reallyHas<const bool  (C::*)(const V & a) const, (&C::operator&&)>* /*unused*/) { }
        // The famous C++ sink-hole.
        // Note that sink-hole must be templated too as we are testing test<T>(0).
        // If the method serialize isn't available, we will end up in this method.
        template <typename> static no& test(...) { /* dark matter */ }

        // The constant used as a return value for the test.
        // The test is actually done here, thanks to the sizeof compile-time evaluation.
        static const bool value = (sizeof(test<T>(0)) == sizeof(yes) || PDP::IsPrimitiveType<T>::value);
    };

    template<typename T> using HasLogicalAndOperand_V = typename HasLogicalAndOperand<T>::value;



    template<typename T>
    struct HasLogicalOrOperand
    {
        // For the compile time comparison.
        typedef char yes[1];
        typedef yes no[2];

        // This helper struct permits us to check that serialize is truly a method.
        // The second argument must be of the type of the first.
        // For instance reallyHas<int, 10> would be substituted by reallyHas<int, int 10> and works!
        // reallyHas<int, &C::serialize> would be substituted by reallyHas<int, int &C::serialize> and fail!
        // Note: It only works with integral constants and pointers (so function pointers work).
        // In our case we check that &C::serialize has the same signature as the first argument!
        // reallyHas<std::string (C::*)(), &C::serialize> should be substituted by
        // reallyHas<std::string (C::*)(), std::string (C::*)() &C::serialize> and work!
        template <typename U, U u> struct reallyHas;

        // Two overloads for yes: one for the signature of a normal method, one is for the signature of a const method.
        // We accept a pointer to our helper struct, in order to avoid to instantiate a real instance of this type.
        // std::string (C::*)() is function pointer declaration.
        template <typename C> static yes& test(reallyHas<const bool    (C::*)(const C & a), (&C::operator||)>* /*unused*/) { }
        template <typename C> static yes& test(reallyHas<const bool  (C::*)(const C & a) const, (&C::operator||)>* /*unused*/) { }
        // The famous C++ sink-hole.
        // Note that sink-hole must be templated too as we are testing test<T>(0).
        // If the method serialize isn't available, we will end up in this method.
        template <typename> static no& test(...) { /* dark matter */ }

        // The constant used as a return value for the test.
        // The test is actually done here, thanks to the sizeof compile-time evaluation.
        static const bool value = (sizeof(test<T>(0)) == sizeof(yes) || PDP::IsPrimitiveType<T>::value);
    };


    template<typename T, typename V>
    struct HasLogicalOrOperandPair
    {
        // For the compile time comparison.
        typedef char yes[1];
        typedef yes no[2];

        // This helper struct permits us to check that serialize is truly a method.
        // The second argument must be of the type of the first.
        // For instance reallyHas<int, 10> would be substituted by reallyHas<int, int 10> and works!
        // reallyHas<int, &C::serialize> would be substituted by reallyHas<int, int &C::serialize> and fail!
        // Note: It only works with integral constants and pointers (so function pointers work).
        // In our case we check that &C::serialize has the same signature as the first argument!
        // reallyHas<std::string (C::*)(), &C::serialize> should be substituted by
        // reallyHas<std::string (C::*)(), std::string (C::*)() &C::serialize> and work!
        template <typename U, U u> struct reallyHas;

        // Two overloads for yes: one for the signature of a normal method, one is for the signature of a const method.
        // We accept a pointer to our helper struct, in order to avoid to instantiate a real instance of this type.
        // std::string (C::*)() is function pointer declaration.
        template <typename C> static yes& test(reallyHas<const bool    (C::*)(const V & a), (&C::operator||)>* /*unused*/) { }
        template <typename C> static yes& test(reallyHas<const bool  (C::*)(const V & a) const, (&C::operator||)>* /*unused*/) { }
        // The famous C++ sink-hole.
        // Note that sink-hole must be templated too as we are testing test<T>(0).
        // If the method serialize isn't available, we will end up in this method.
        template <typename> static no& test(...) { /* dark matter */ }

        // The constant used as a return value for the test.
        // The test is actually done here, thanks to the sizeof compile-time evaluation.
        static const bool value = (sizeof(test<T>(0)) == sizeof(yes) || PDP::IsPrimitiveType<T>::value);
    };
    template<typename T> using HasLogicalOrOperand_V = typename HasLogicalOrOperand<T>::value;


    template<typename T>
    struct HasLogicalNotOperand
    {
        // For the compile time comparison.
        typedef char yes[1];
        typedef yes no[2];

        // This helper struct permits us to check that serialize is truly a method.
        // The second argument must be of the type of the first.
        // For instance reallyHas<int, 10> would be substituted by reallyHas<int, int 10> and works!
        // reallyHas<int, &C::serialize> would be substituted by reallyHas<int, int &C::serialize> and fail!
        // Note: It only works with integral constants and pointers (so function pointers work).
        // In our case we check that &C::serialize has the same signature as the first argument!
        // reallyHas<std::string (C::*)(), &C::serialize> should be substituted by
        // reallyHas<std::string (C::*)(), std::string (C::*)() &C::serialize> and work!
        template <typename U, U u> struct reallyHas;

        // Two overloads for yes: one for the signature of a normal method, one is for the signature of a const method.
        // We accept a pointer to our helper struct, in order to avoid to instantiate a real instance of this type.
        // std::string (C::*)() is function pointer declaration.
        template <typename C> static yes& test(reallyHas<const bool    (C::*)(const C & a), (&C::operator!)>* /*unused*/) { }
        template <typename C> static yes& test(reallyHas<const bool  (C::*)(const C & a) const, (&C::operator!)>* /*unused*/) { }
        // The famous C++ sink-hole.
        // Note that sink-hole must be templated too as we are testing test<T>(0).
        // If the method serialize isn't available, we will end up in this method.
        template <typename> static no& test(...) { /* dark matter */ }

        // The constant used as a return value for the test.
        // The test is actually done here, thanks to the sizeof compile-time evaluation.
        static const bool value = (sizeof(test<T>(0)) == sizeof(yes) || PDP::IsPrimitiveType<T>::value);
    };


    template<typename T, typename V>
    struct HasLogicalNotOperandPair
    {
        // For the compile time comparison.
        typedef char yes[1];
        typedef yes no[2];

        // This helper struct permits us to check that serialize is truly a method.
        // The second argument must be of the type of the first.
        // For instance reallyHas<int, 10> would be substituted by reallyHas<int, int 10> and works!
        // reallyHas<int, &C::serialize> would be substituted by reallyHas<int, int &C::serialize> and fail!
        // Note: It only works with integral constants and pointers (so function pointers work).
        // In our case we check that &C::serialize has the same signature as the first argument!
        // reallyHas<std::string (C::*)(), &C::serialize> should be substituted by
        // reallyHas<std::string (C::*)(), std::string (C::*)() &C::serialize> and work!
        template <typename U, U u> struct reallyHas;

        // Two overloads for yes: one for the signature of a normal method, one is for the signature of a const method.
        // We accept a pointer to our helper struct, in order to avoid to instantiate a real instance of this type.
        // std::string (C::*)() is function pointer declaration.
        template <typename C> static yes& test(reallyHas<const bool    (C::*)(const V & a), (&C::operator!)>* /*unused*/) { }
        template <typename C> static yes& test(reallyHas<const bool  (C::*)(const V & a) const, (&C::operator!)>* /*unused*/) { }
        // The famous C++ sink-hole.
        // Note that sink-hole must be templated too as we are testing test<T>(0).
        // If the method serialize isn't available, we will end up in this method.
        template <typename> static no& test(...) { /* dark matter */ }

        // The constant used as a return value for the test.
        // The test is actually done here, thanks to the sizeof compile-time evaluation.
        static const bool value = (sizeof(test<T>(0)) == sizeof(yes) || PDP::IsPrimitiveType<T>::value);
    };


    template<typename T> using HasLogicalNotOperand_V = typename HasLogicalNotOperand<T>::value;


    namespace Detail
    {
        struct IsConvertible
        {
            template <typename _Type, typename _OtherType>
            static void convert(_OtherType);
            template <typename _Type, typename _OtherType>
            static auto test(_Type&&, _OtherType&&) -> decltype(convert<_Type, _OtherType>(PDP::Declval<_Type>()), LD::TrueType());
            static auto test(...) -> decltype(LD::FalseType());
        };
    }
    template <typename _Type, typename _OtherType>
    struct IsConvertible : public decltype(Detail::IsConvertible::test(PDP::Declval<_Type>(), PDP::Declval<_OtherType>()))
    {};


    template <class _Tp> struct  remove_volatile               {typedef _Tp type;};

    template <class _Tp> struct  remove_volatile<volatile _Tp> {typedef _Tp type;};

    template <class _Tp> struct  remove_const            {typedef _Tp type;};

    template <class _Tp> struct  remove_const<const _Tp> {typedef _Tp type;};

    template <class _Tp> struct __libcpp_is_pointer       : public LD::FalseType {};

    template <class _Tp> struct __libcpp_is_pointer<_Tp*> : public LD::TrueType {};


    template <class _Tp> struct  RemoveCV

    {typedef typename remove_volatile<typename remove_const<_Tp>::type>::type type;};


    template <class _Tp> struct  IsPointer : public __libcpp_is_pointer<typename RemoveCV<_Tp>::type> {};



    template <bool _Bp, class _If, class _Then>
    struct  Conditonal {typedef _If type;};
    template <class _If, class _Then>
    struct  Conditonal<false, _If, _Then> {typedef _Then type;};

#if _LIBCPP_STD_VER > 11
    template <bool _Bp, class _If, class _Then> using Conditional_T = typename Conditonal<_Bp, _If, _Then>::type;
#endif





    template <class _Tp> struct  IsLValueReference       : public LD::FalseType {};
    template <class _Tp> struct  IsLValueReference<_Tp&> : public LD::TrueType {};

    template <class _Tp> struct  IsRValueReference        : public LD::FalseType {};
#ifndef _LIBCPP_HAS_NO_RVALUE_REFERENCES
    template <class _Tp> struct  IsRValueReference<_Tp&&> : public LD::TrueType {};
#endif

    template <class _Tp> struct  IsReference        : public LD::FalseType {};
    template <class _Tp> struct  IsReference<_Tp&>  : public LD::TrueType {};
#ifndef _LIBCPP_HAS_NO_RVALUE_REFERENCES
    template <class _Tp> struct  IsReference<_Tp&&> : public LD::TrueType {};
#endif



    struct __two {char __lx[2];};


    struct __is_referenceable_impl {
        template <class _Tp> static _Tp& __test(int);
        template <class _Tp> static __two __test(...);
    };

    template <class _Tp>
    struct __is_referenceable : LD::Detail::IntegralConstant<bool,
            !PDP::IsSame<decltype(__is_referenceable_impl::__test<_Tp>(0)), __two>::value> {};



    template <class _Tp, bool = __is_referenceable<_Tp>::value> struct __add_lvalue_reference_impl            { typedef _Tp  type; };
    template <class _Tp                                       > struct __add_lvalue_reference_impl<_Tp, true> { typedef _Tp& type; };

    template <class _Tp> struct  AddLValueReference
    {typedef typename __add_lvalue_reference_impl<_Tp>::type type;};

#if _LIBCPP_STD_VER > 11
    template <class _Tp> using AddLValueReference_T = typename AddLValueReference<_Tp>::type;
#endif


    typedef decltype(nullptr) nullptr_t;



    template <class _Tp> struct __is_nullptr_t_impl       : public LD::FalseType {};
    template <>          struct __is_nullptr_t_impl<nullptr_t> : public LD::TrueType {};

    template <class _Tp> struct  IsNullPtr_T
            : public __is_nullptr_t_impl<typename PDP::RemoveCV<_Tp>::type> {};

#if _LIBCPP_STD_VER > 11
    template <class _Tp> struct  IsNullPointer
            : public __is_nullptr_t_impl<typename PDP::RemoveCV<_Tp>::type> {};

#if _LIBCPP_STD_VER > 14 && !defined(_LIBCPP_HAS_NO_VARIABLE_TEMPLATES)
    template <class _Tp>  bool is_null_pointer_v
            = IsNullPointer<_Tp>::value;
#endif
#endif


#if __has_feature(is_union) || (_GNUC_VER >= 403)

    template <class _Tp> struct  IsUnion
    : public LD::Detail::IntegralConstant<bool, __is_union(_Tp)> {};

#else

template <class _Tp> struct __libcpp_union : public PDP::FalseType {};
    template <class _Tp> struct _LIBCPP_TEMPLATE_VIS IsUnion
    : public __libcpp_union<typename PDP::RemoveCV<_Tp>::type> {};

#endif



// is_void

template <class _Tp> struct __libcpp_is_void       : public LD::FalseType {};
template <>          struct __libcpp_is_void<void> : public LD::TrueType {};

template <class _Tp> struct  IsVoid
: public __libcpp_is_void<typename PDP::RemoveCV<_Tp>::type> {};

#if _LIBCPP_STD_VER > 14 && !defined(_LIBCPP_HAS_NO_VARIABLE_TEMPLATES)
template <class _Tp>  bool IsVoid_V
        = IsVoid<_Tp>::value;
#endif


namespace __libcpp_is_function_imp
{
    struct __dummy_type {};
    template <class _Tp> char  __test(_Tp*);
    template <class _Tp> char __test(__dummy_type);
    template <class _Tp> PDP::__two __test(...);
    template <class _Tp> _Tp&  __source(int);
    template <class _Tp> __dummy_type __source(...);
}

template <class _Tp, bool = PDP::IsClassType<_Tp>::value ||
                            IsUnion<_Tp>::value ||
                            IsVoid<_Tp>::value  ||
                            PDP::IsReference<_Tp>::value ||
                            IsNullPtr_T<_Tp>::value >
struct __libcpp_is_function
        : public LD::Detail::IntegralConstant<bool, sizeof(__libcpp_is_function_imp::__test<_Tp>(__libcpp_is_function_imp::__source<_Tp>(0))) == 1>
{};
template <class _Tp> struct __libcpp_is_function<_Tp, true> : public LD::FalseType {};

template <class _Tp> struct IsFunction
        : public __libcpp_is_function<_Tp> {};

#if _LIBCPP_STD_VER > 14 && !defined(_LIBCPP_HAS_NO_VARIABLE_TEMPLATES)
template <class _Tp>  bool is_function_v
        = IsFunction<_Tp>::value;
#endif

template <class _Tp> struct  IsConst            : public LD::FalseType {};
template <class _Tp> struct  IsConst<_Tp const> : public LD::TrueType {};

#if _LIBCPP_STD_VER > 14 && !defined(_LIBCPP_HAS_NO_VARIABLE_TEMPLATES)
template <class _Tp>  bool IsConst_V
        = IsConst<_Tp>::value;
#endif

template <class _Tp, bool = PDP::IsReference<_Tp>::value ||PDP::IsFunction<_Tp>::value  ||PDP::IsConst<_Tp>::value     >
struct __add_const             {typedef _Tp type;};

template <class _Tp>
struct __add_const<_Tp, false> {typedef const _Tp type;};

template <class _Tp> struct AddConst
{typedef typename __add_const<_Tp>::type type;};


template<typename T>
class IntegerVariantForFloatingPointNumber
{


};



template<>

class IntegerVariantForFloatingPointNumber<float>
{

public:


    typedef int IntegerVariant;


};


template<>
class IntegerVariantForFloatingPointNumber<double>
{
public:


    typedef long IntegerVariant;


};


template<>
class IntegerVariantForFloatingPointNumber<long double>

{

public:


    typedef long IntegerVariant;


};



template<typename T, class = void>
class FloatingPointVariantForNumber
{


};


template<typename T>
class FloatingPointVariantForNumber<T,PDP::Enable_If_T<PDP::Is32BitInteger<T>::value>>
{

public:


    typedef float FloatingPointVariant;

};


template<typename T>
class FloatingPointVariantForNumber<T,PDP::Enable_If_T<PDP::IsSame<T, float>::value>>

{

public:


    typedef float FloatingPointVariant;

};


template<typename T>
class FloatingPointVariantForNumber<T,PDP::Enable_If_T<PDP::IsSame<T, double>::value>>
{

public:


    typedef double FloatingPointVariant;

};

template<typename T>
class FloatingPointVariantForNumber<T,PDP::Enable_If_T<PDP::Is32BitSignedInteger<T>::value || PDP::Is32BitUnsignedInteger<T>::value>>
{
public:

    typedef float FloatingPointVariant;
};

template<typename T>
class FloatingPointVariantForNumber<T,PDP::Enable_If_T<PDP::Is64BitSignedInteger<T>::value || PDP::Is64BitUnsignedInteger<T>::value>>
{
public:

    typedef double FloatingPointVariant;
};

template<typename T, class = void>
class UnsignedIntegerVariantForInteger
{

};

template<typename T>
class UnsignedIntegerVariantForInteger<T,PDP::Enable_If_T<PDP::IsIntegrelType<T>::value>>
{
public:

    typedef  LD::UInteger UnsignedVariant;
};





template <class _Tp> struct  RemoveConst            {typedef _Tp type;};
template <class _Tp> struct  RemoveConst<const _Tp> {typedef _Tp type;};
template<typename T>
struct RemoveConst<volatile T> {
    typedef T type;
};
template<typename T>
struct RemoveConst<const volatile T> {
    typedef T type;
};
#if _LIBCPP_STD_VER > 11
template <class _Tp> using RemoveConst_T = typename RemoveConst<_Tp>::type;
#endif



template < bool... > struct All;
template < > struct All<> : LD::TrueType {};
template < bool B, bool... Rest > struct All<B,Rest...>
{
    constexpr static bool value = B && All<Rest...>::value;
};
/*
     template< bool... Bs >
     using is_all = std::integral_constant< bool, std::is_same< integer_sequence< bool, true, Bs... >, integer_sequence< bool, Bs..., true > >::value >;
     */
#if _LIBCPP_STD_VER > 11
template< bool... Bs > using IsAll_V = typename All<Bs...>::value;
#endif

template <typename T>
struct Decay
{
    template <typename U> static U impl(U);
    using type = decltype(impl(PDP::Declval<T>()));
};

#if _LIBCPP_STD_VER > 11
template <class _Tp> using Decay_T = typename Decay<_Tp>::type;
#endif





/*
     template<typename ... Args>
     Tuple<PDP::Decay_T<Args>...> MakeTuple(const PDP::Decay_T<Args> &... arguements)
     {
     Tuple<PDP::Decay_T<Args>...> returnableTuple(arguements...);

     return returnableTuple;
     }
    */


template <class _Tp> _Tp&& __Declval(int);
template <class _Tp> _Tp   __Declval(long);

template <class _Tp>
decltype(__Declval<_Tp>(0))
Declval() _NOEXCEPT;







template <bool, typename T, typename>
struct Conditional
{
    typedef T type;
};
template <typename T, typename U>
struct Conditional<false, T, U> {
    typedef U type;
};


namespace Detail
{









    template<typename, typename _Tp> struct __select_2nd { typedef _Tp type; };

    template <class _Tp, class _Arg>
    typename __select_2nd<decltype((Declval<_Tp>() = Declval<_Arg>())), LD::TrueType>::type
    __is_assignable_test(int);

    template <class, class>
    LD::FalseType __is_assignable_test(...);


    template <class _Tp, class _Arg, bool = PDP::IsVoid<_Tp>::value || PDP::IsVoid<_Arg>::value>
    struct __is_assignable_imp
            : public decltype((__is_assignable_test<_Tp, _Arg>(0))) {};

    template <class _Tp, class _Arg>
    struct __is_assignable_imp<_Tp, _Arg, true>
            : public LD::FalseType
    {
    };







}



template <class _Tp, class _Arg>
struct IsAssignable
        : public Detail::__is_assignable_imp<_Tp, _Arg> {};

#if _LIBCPP_STD_VER > 14 && !defined(_LIBCPP_HAS_NO_VARIABLE_TEMPLATES)
template <class _Tp, class _Arg>  bool IsAssignable_V
        = IsAssignable<_Tp, _Arg>::value;
#endif

// is_copy_assignable

template <class _Tp> struct  IsCopyAssignable
        : public IsAssignable<typename PDP::AddLValueReference<_Tp>::type,
                typename PDP::AddLValueReference<typename PDP::AddConst<_Tp>::type>::type> {};

#if _LIBCPP_STD_VER > 14 && !defined(_LIBCPP_HAS_NO_VARIABLE_TEMPLATES)
template <class _Tp>  bool IsCopyAssignable_V
        = IsCopyAssignable<_Tp>::value;
#endif



template <class _Tp> struct  IsMoveAssignable
#ifndef _LIBCPP_HAS_NO_RVALUE_REFERENCES
        : public IsAssignable<typename PDP::AddLValueReference<_Tp>::type,
                typename PDP::AddRvalueReference<_Tp>::type> {};
#else
: public IsCopyAssignable<_Tp> {};
#endif












template<class _Tp, _Tp... _Ip>
struct  IntegerSequence
{
    typedef _Tp value_type;
    static_assert( PDP::IsPrimitiveType<_Tp>::value,
                   "PDP::IntegerSequence can only be instantiated with an integral type" );
    static
    constexpr
    LD::UInteger
    size() noexcept { return sizeof...(_Ip); }
};

template<LD::UInteger... _Ip>
using IndexSequence = IntegerSequence<LD::UInteger, _Ip...>;

template<typename S1, typename S2>
struct BuildIndexSequence;

template<typename S1, typename S2>
using BuildIndexSequence_T = typename BuildIndexSequence<S1, S2>::type;

template<
        LD::UInteger... Cs1,
        LD::UInteger... Cs2
>
struct BuildIndexSequence<
        IndexSequence<Cs1...>,
        IndexSequence<Cs2...>
>
{
    using type = IndexSequence<Cs1..., sizeof...(Cs1) + Cs2...>;
};

template<LD::UInteger N>
struct MakeIndexSequence;

template<LD::UInteger N>
using MakeIndexSequence_T =
typename MakeIndexSequence<N>::type;

template<LD::UInteger N>
struct MakeIndexSequence
{
    using type = BuildIndexSequence_T<
            MakeIndexSequence_T<N / 2>,
            MakeIndexSequence_T<N - N / 2>
    >;
};

template<>
struct MakeIndexSequence<1>
{
    using type = IndexSequence<0>;
};

template<>
struct MakeIndexSequence<0>
{
    using type = IndexSequence<>;
};




template <typename... Ts> struct VariadicPack { static const unsigned int size = sizeof...(Ts);};

template <typename P0, typename P1>
struct ArePacksSame;

template <typename... T0s, typename... T1s>
struct ArePacksSame<VariadicPack<T0s...>, VariadicPack<T1s...>>
        : LD::Detail::IntegralConstant<bool,(sizeof...(T0s) == sizeof...(T1s))
                                     && (PDP::IsSame<T0s, T1s>::value && ...)>
{
};

template <typename P0, typename P1>
struct ArePacksConvertible;

template <typename... T0s, typename... T1s>
struct ArePacksConvertible<VariadicPack<T0s...>, VariadicPack<T1s...>>: public LD::Detail::IntegralConstant<bool,(sizeof...(T0s) == sizeof...(T1s))
                                     && (PDP::IsConvertible<T0s, T1s>::value && ...)>
{
};


template<typename P0, typename P1>
struct IsPackConvertible;


template <typename... T0s, typename T1s>
struct IsPackConvertible<VariadicPack<T0s...>, T1s>
        : LD::Detail::IntegralConstant<bool,
                (PDP::IsConvertible<T0s, T1s>::value && ...)>
{
};

template <typename P0, typename P1>
struct ArePacksAssignable;

template <typename... T0s, typename... T1s>
struct ArePacksAssignable<VariadicPack<T0s...>, VariadicPack<T1s...>>
        : LD::Detail::IntegralConstant<bool,(sizeof...(T0s) == sizeof...(T1s))
                                     && (PDP::IsAssignable<T0s, T1s>::value && ...)>
{
};


template <typename P0>
struct ArePacksMoveAssignable;

template <typename... T0s>
struct ArePacksMoveAssignable<VariadicPack<T0s...>>
        : LD::Detail::IntegralConstant<bool,
                (PDP::IsMoveAssignable<T0s>::value && ...)>
{
};


template <typename P0>
struct ArePacksCopyAssignable;

template <typename... T0s>
struct ArePacksCopyAssignable<VariadicPack<T0s...>>
        : LD::Detail::IntegralConstant<bool,
                (PDP::IsCopyAssignable<T0s>::value && ...)>
{
};

namespace Detail
{
    namespace Fibonnaci
    {
        inline void multiply(LD::UInteger F[2][2], LD::UInteger M[2][2])
        {
            LD::UInteger x =  F[0][0] * M[0][0] + F[0][1] * M[1][0];
            LD::UInteger y =  F[0][0] * M[0][1] + F[0][1] * M[1][1];
            LD::UInteger z =  F[1][0] * M[0][0] + F[1][1] * M[1][0];
            LD::UInteger w =  F[1][0] * M[0][1] + F[1][1] * M[1][1];
            F[0][0] = x;
            F[0][1] = y;
            F[1][0] = z;
            F[1][1] = w;
        }

        /*
             * function to calculate power of a matrix
             */
        inline void power(LD::UInteger F[2][2], LD::UInteger n)
        {
            if (n == 0 || n == 1)
                return;
            LD::UInteger M[2][2] = {{1,1},{1,0}};
            Detail::Fibonnaci::power(F, n / 2);
            Detail::Fibonnaci::multiply(F, F);
            if (n % 2 != 0)
                multiply(F, M);
        }

        /*
             * function that returns nth Fibonacci number
             */
    }
}

inline LD::UInteger Fibonnaci(LD::UInteger n)
{
    LD::UInteger F[2][2] = {{1,1},{1,0}};
    if (n == 0)
        return 0;
    Detail::Fibonnaci::power(F, n - 1);
    return F[0][0];
}

template<class ...> using Void_T = void;



namespace Detail
{
    template <class Default, class AlwaysVoid,
            template<class...> class Op, class... Args>
    struct Detector
    {
        using value_t = LD::FalseType;
        using type = Default;
    };

    template <class Default, template<class...> class Op, class... Args>
    struct Detector<Default, Void_T<Op<Args...>>, Op, Args...>
    {
        using value_t = LD::TrueType;
        using type = Op<Args...>;
    };

} // namespace detail

// special type to indicate detection failure
struct nonesuch {
    nonesuch() = delete;
    ~nonesuch() = delete;
    nonesuch(nonesuch const&) = delete;
    void operator=(nonesuch const&) = delete;
};

template <template<class...> class Op, class... Args>
using Is_Detected =
typename Detail::Detector<nonesuch, void, Op, Args...>::value_t;

template <template<class...> class Op, class... Args>
using Is_Detected_T = typename Detail::Detector<nonesuch, void, Op, Args...>::type;

template <class Default, template<class...> class Op, class... Args>
using Detected_Or = Detail::Detector<Default, void, Op, Args...>;

template <typename T, typename Ret, typename Index>
using Subscript_T = LD::Detail::IntegralConstant<Ret (T::*) (Index), & T::operator[]>;

template <typename T, typename Ret, typename Index>
using Has_Subscript = Is_Detected<Subscript_T, T, Ret, Index>;



template<typename ... Args> class Tuple;

namespace Detail
{
    template<class... _Types> class TupleImpl;
    // empty tuple
    template<> class TupleImpl<> {};
    // recursive tuple definition
    template<class _This,
            class... _Rest>
    class TupleImpl<_This, _Rest...>
            : private TupleImpl<_Rest...>
    {
    public:
        _This _Myfirst;


        TupleImpl(){}
        TupleImpl(_This first, _Rest... rest): TupleImpl<_Rest...>(rest...), _Myfirst(first) {}
    };

    // tuple_element
    template<LD::UInteger _Index, class _Tuple> struct TupleElement;

    // select first element
    template<class _This, class... _Rest>
    struct TupleElement<0, TupleImpl<_This, _Rest...>>
    {

        typedef _This& type;
        typedef TupleImpl<_This, _Rest...> _Ttype;

    };

    template<class _This, class... _Rest>
    struct TupleElement<0, Tuple<_This, _Rest...>>
    {
        typedef _This& type;
        typedef Tuple<_This, _Rest...> _Ttype;
    };

    // recursive tuple_element definition
    template <LD::UInteger _Index, class _This, class... _Rest>
    struct TupleElement<_Index, Tuple<_This, _Rest...>>
            : public TupleElement<_Index - 1, Tuple<_Rest...> >
    {

    };

    // recursive tuple_element definition
    template <LD::UInteger _Index, class _This, class... _Rest>
    struct TupleElement<_Index, TupleImpl<_This, _Rest...>>
            : public TupleElement<_Index - 1, TupleImpl<_Rest...> >
    {

    };

    template <class Tuple, class T, LD::UInteger Index = 0>
    struct FindFirstElementIndex;

    template <LD::UInteger Index, bool Valid>
    struct FindFirstElementIndexFinalTest
            : public LD::Detail::IntegralConstant<LD::UInteger, Index>
    {
    };

    template <LD::UInteger Index>
    struct FindFirstElementIndexFinalTest<Index, false>
    {
        static_assert(Index == -1, "Type not found");
    };

    template <class Head, class T, LD::UInteger Index>
    struct FindFirstElementIndex<Tuple<Head>, T, Index>
            : public FindFirstElementIndexFinalTest<Index, PDP::IsSame<typename PDP::Decay<Head>::type, T>::value>
    {
    };

    template <class Head, class ...Rest, class T, LD::UInteger Index>
    struct FindFirstElementIndex<Tuple<Head, Rest...>, T, Index>
            : public PDP::Conditonal<PDP::IsSame<typename PDP::Decay<Head>::type, T>::value,
                    LD::Detail::IntegralConstant<LD::UInteger, Index>,
                    FindFirstElementIndex<Tuple<Rest...>, T, Index+1>>::type
    {

    };

    template<LD::UInteger index, typename ... Args>
    Tuple<Args...> & Assign(Tuple<Args...> & tuple)
    {
        return tuple;
    }
}

template<LD::UInteger _Index, class... _Types> inline
typename Detail::TupleElement<_Index, Tuple<_Types...>>::type
Get(Tuple<_Types...>& _Tuple);

template<LD::UInteger _Index, class... _Types> inline
const typename Detail::TupleElement<_Index, Tuple<_Types...>>::type
Get(const Tuple<_Types...>& _Tuple);

template<typename Type, class... _Types> inline
Type & Get(Tuple<_Types...>& _Tuple);

template<typename Type, class... _Types> inline
const Type & Get(const Tuple<_Types...>& _Tuple);

namespace Detail
{
    template<LD::UInteger index, typename T  ,typename ... Args, typename ... A>
    Tuple<Args...> & Assign(Tuple<Args...> & tuple, T && a,  A && ... arguements)
    {
        Get<index>(tuple) = a;
        return Assign<index+1>(tuple,PDP::Forward<A>(arguements)...);
    }

    template<LD::UInteger index,typename ... Args, typename ... A>
    Tuple<Args...> & Assign(Tuple<Args...> & tuple, A && ... arguements)
    {
        return Detail::Assign<index>(tuple,PDP::Forward<A>(arguements)...);
    }

    template<int ...S, typename ... Arguements, typename ... Args>
    void TupleAssign(Tuple<typename PDP::Decay<Args>::type...> & tuple, Args && ... arguements)
    {

        Detail::Assign<0>(tuple, PDP::Forward<Args>(arguements)...);
    }

    template<typename T, typename U>
    void AssignTupleElementImpl(T & tupleElement,  U && element)
    {
        tupleElement = PDP::Move(element);
    }

    template<typename T, typename U, typename ... Args>
    void AssignTupleElementImpl(T & tupleElement,  U && element, Args && ... arguements)
    {
        AssignTupleElementImpl(tupleElement,element,PDP::Forward<Args>(arguements)...);
    }

    template<LD::UInteger ... Index, typename ... Args>
    void AssignTupleElement(PDP::IndexSequence<Index...>,Tuple<typename PDP::Decay<Args>::type...> & tuple, Args && ... arguements)
    {
        (...,AssignTupleElementImpl(Get<Index>(tuple),PDP::Forward<Args>(arguements)));
    }

    template<typename ... Args>
    void AssignTuple(Tuple<typename PDP::Decay<Args>::type...> & tuple, Args && ... arguements)
    {
        AssignTupleElement(typename PDP::MakeIndexSequence<sizeof...(Args)>::type{},tuple,PDP::Forward<Args>(arguements)...);
    }

    template<typename T, typename U>
    void AssignTupleElementImpl1(T && tupleElement,  U && element)
    {
        tupleElement = element;
    }

    template<typename T, typename U, typename ... Args>
    void AssignTupleElementImpl1(T && tupleElement,  U && element, Args && ... arguements)
    {
        AssignTupleElementImpl(tupleElement,element,PDP::Forward<Args>(arguements)...);
    }

    template<LD::UInteger ... Index, typename ... Args, typename ... B>
    void AssignTupleElement1(PDP::IndexSequence<Index...>,Tuple<Args...> & tuple, B && ... arguements)
    {
        (...,AssignTupleElementImpl1(Get<Index>(tuple),PDP::Forward<Args>(arguements)));
    }

    template<typename ... Args, typename ... B>
    void AssignTuple1(Tuple<Args...> & tuple, B && ... arguements)
    {
        AssignTupleElement1(typename PDP::MakeIndexSequence<sizeof...(Args)>::type{},tuple,PDP::Forward<B>(arguements)...);
    }

    template<LD::UInteger Index, typename T, typename U>
    void AssignTupelFromTupleBase(T && a, const U & b)
    {
        Get<Index>(a) = Get<Index>(b);
    }

    template<typename T, typename U, LD::UInteger ... Index>
    void AssignTupleFromTupleRec(PDP::IndexSequence<Index...>, T && a, const U & b)
    {
        (...,AssignTupelFromTupleBase<Index>(a,b));
    }
    template<typename ... A, typename ... B>
    void AssignTupleFromTuple(Tuple<A...> & a, const Tuple<B...> & b)
    {
        return AssignTupleFromTupleRec(typename PDP::MakeIndexSequence<sizeof...(A)>::type{},a,b);
    }

    template<template<typename> class Functor ,typename ... Args, int ...S>
    void InternalInvoke(const Functor<void(Args...)> & function, const Tuple<typename PDP::Decay<Args>::type...> & tuple,PDP::IndexSequence<S...>)
    {
        function(Get<S>(tuple) ...);
    }
    template<template<typename> class Functor, typename T ,typename ... Args,int ...S>
    T InternalInvoke(const Functor<T(Args...)> & function, const Tuple<typename PDP::Decay<Args>::type...> & tuple,PDP::IndexSequence<S...>)
    {
        return PDP::Move(function(Get<S>(tuple) ...));
    }

    template<typename T, typename F, LD::UInteger... Is>
    void for_each(T&& t, F f, PDP::IndexSequence<Is...>)
    {
        auto l = { (f(Get<Is>(t)), 0)... };
    }

    template<typename T, typename F, typename U, template<typename> class Functor,  LD::UInteger... Is>
    void for_each(T&& t, F f,T & state, const Functor<U(const U & a, const U & b)> & accumulator, PDP::IndexSequence<Is...>)
    {
        T retValue = PDP::Move({ (f(Get<Is>(t)), 0)... });
        state = PDP::Move(accumulator(state,retValue));
    }
}

template<typename ... Args>
class Tuple: public Detail::TupleImpl<Args...>
{
public:
    inline Tuple(){}
    inline Tuple(Args && ... arguements);
    template <typename ... B ,typename = PDP::Enable_If_T< PDP::ArePacksConvertible<PDP::VariadicPack<B...>, PDP::VariadicPack<Args...>>::value>>
    explicit Tuple(const Tuple<B...> & tuple)
    {
        (*this) = tuple;
    }

    template <typename ... B ,typename = PDP::Enable_If_T< PDP::ArePacksConvertible<PDP::VariadicPack<Args...>, PDP::VariadicPack<B...>>::value>>
    Tuple<Args...> & operator = (const Tuple<B...> & tuple)
    {
        return Assignment(tuple);
    }

    template<typename ... B>
    PDP::Enable_If_T< PDP::ArePacksConvertible<PDP::VariadicPack<Args...>, PDP::VariadicPack<B...>>::value,Tuple<Args...> &> Assignment(const Tuple<B...> & tuple);
};

template <> struct Tuple<> {};
// select first element
// get reference to _Index element of tuple
template<LD::UInteger _Index, class... _Types> inline
typename Detail::TupleElement<_Index, Tuple<_Types...>>::type
Get(Tuple<_Types...>& _Tuple)
{
    typedef typename Detail::TupleElement<_Index, Tuple<_Types...>>::_Ttype _Ttype;
    return (((_Ttype&) _Tuple)._Myfirst);
}




template<LD::UInteger _Index> inline
void Get(Tuple<>& _Tuple)
{

}

// get reference to _Index element of tuple
template<LD::UInteger _Index, class... _Types> inline
const typename Detail::TupleElement<_Index, Tuple<_Types...>>::type
Get(const Tuple<_Types...>& _Tuple)
{
    typedef typename Detail::TupleElement<_Index, Tuple<_Types...>>::_Ttype _Ttype;
    return (((_Ttype&) _Tuple)._Myfirst);
}

template<LD::UInteger _Index> inline
void Get(const Tuple<>& _Tuple)
{

}


template<typename Type, class... _Types> inline
Type & Get(Tuple<_Types...>& _Tuple)
{
    return  Get<Detail::FindFirstElementIndex<Tuple<_Types...>, Type>::value>(_Tuple);
}

template<typename Type> inline
void Get(Tuple<>& _Tuple)
{

}

template<typename Type, class... _Types> inline
const Type & Get(const Tuple<_Types...>& _Tuple)
{
    return  Get<Detail::FindFirstElementIndex<Tuple<_Types...>, Type>::value>(_Tuple);
}

template<typename Type> inline
void Get(const Tuple<>& _Tuple)
{

}

template <class _Tp> class  TupleSize;

#if !defined(_LIBCPP_CXX03_LANG)
template <class _Tp, class...>
using __enable_if_tuple_size_imp = _Tp;

template <class _Tp>
class  TupleSize<__enable_if_tuple_size_imp<
        const _Tp,
        typename PDP::EnableIf<!PDP::IsVolatile<_Tp>::value>::type,
        LD::Detail::IntegralConstant<LD::UInteger, sizeof(TupleSize<_Tp>)>>>
        : public LD::Detail::IntegralConstant<LD::UInteger, TupleSize<_Tp>::value> {};

template <class _Tp>
class  TupleSize<__enable_if_tuple_size_imp<
        volatile _Tp,
        typename PDP::EnableIf<!PDP::IsConst<_Tp>::value>::type,
        LD::Detail::IntegralConstant<LD::UInteger, sizeof(TupleSize<_Tp>)>>>
        : public LD::Detail::IntegralConstant<LD::UInteger, TupleSize<_Tp>::value> {};

template <class _Tp>
class  TupleSize<__enable_if_tuple_size_imp<
        const volatile _Tp,
        LD::Detail::IntegralConstant<LD::UInteger, sizeof(TupleSize<_Tp>)>>>
        : public LD::Detail::IntegralConstant<LD::UInteger, TupleSize<_Tp>::value> {};

#else
template <class _Tp> class  TupleSize<const _Tp> : public TupleSize<_Tp> {};
    template <class _Tp> class  TupleSize<volatile _Tp> : public TupleSize<_Tp> {};
    template <class _Tp> class  TupleSize<const volatile _Tp> : public TupleSize<_Tp> {};
#endif


template <class ..._Tp>
class  TupleSize<Tuple<_Tp...> >
        : public LD::Detail::IntegralConstant<LD::UInteger, sizeof...(_Tp)>
{
};


template <>
class  TupleSize<Tuple<> >
        : public LD::Detail::IntegralConstant<LD::UInteger, 0>
{
};


template<typename ...Args>
Tuple<typename PDP::Decay<Args>::type...> MakeTuple(Args && ... arguements)
{
    Tuple<typename PDP::Decay<Args>::type...> tuple;
    Detail::AssignTuple(tuple, PDP::Forward<Args>(arguements)...);
    return tuple;
}




















template<typename ... Args>
Tuple<Args...>::Tuple(Args && ... arguements):Detail::TupleImpl<Args...>(arguements...)
{
}

template<typename ... Args>
template<typename ... B>
PDP::Enable_If_T< PDP::ArePacksConvertible<PDP::VariadicPack<Args...>, PDP::VariadicPack<B...>>::value,Tuple<Args...> &> Tuple<Args...>::Assignment(const Tuple<B...> & tuple)
{
    Detail::AssignTupleFromTuple(*this,tuple);
    return (*this);
}
template <class... _Tp>
Tuple<_Tp&&...>
ForwardAsTuple(_Tp&&... __t) _NOEXCEPT
{
    return Tuple<_Tp&&...>(PDP::Forward<_Tp>(__t)...);
}

template<typename... Ts, typename F>
void Invoke(Tuple<Ts...> const& t, F && f)
{

    Detail::for_each(t, f, typename PDP::MakeIndexSequence<sizeof...(Ts)>::type{});
}

template<typename... Ts, typename T, typename F, template<typename> class Functor>
T Invoke(Tuple<Ts...> const& t, F f,const T & initialState,const Functor<T(const T & a, const T & b)> & accumulator)
{
    T result = initialState;

    Detail::for_each(t, f, result,accumulator,typename PDP::MakeIndexSequence<sizeof...(Ts)>::type{});

    return result;
}

template<template<typename> class Functor ,typename ... Args>
void Invoke(const Functor<void(Args...)> & function, const Tuple<typename PDP::Decay<Args>::type...> & tuple)
{
    Detail::InternalInvoke(function,tuple,typename PDP::MakeIndexSequence<sizeof...(Args)>::type{});
}
template<template<typename> class Functor, typename T ,typename ... Args>
T Invoke(const Functor<T(Args...)> & function, const Tuple<typename PDP::Decay<Args>::type...> & tuple)
{
    return PDP::Move(Detail::InternalInvoke(function,tuple,typename PDP::MakeIndexSequence<sizeof...(Args)>::type{}));;
}

/*
template<typename ... Args, PDP::Enable_If_T<(... && PDP::IsSwappable<Args>::value)> * = nullptr>
void Swap(Tuple<Args...> & x, Tuple<Args...> & y)
{
    return Detail::TupleSwapImpl(x,y,typename PDP::MakeIndexSequence<sizeof...(Args)>::type{});
}
 */

template <class ..._Tp>
inline
Tuple<_Tp&...>
Tie(_Tp&... __t) _NOEXCEPT
{
    return Tuple<_Tp&...>(__t...);
}

template <typename T, typename Tuple>
struct HasType;

template <typename T>
struct HasType<T, PDP::Tuple<>> : LD::FalseType {};

template <typename T, typename U, typename... Ts>
struct HasType<T, PDP::Tuple<U, Ts...>> : HasType<T, PDP::Tuple<Ts...>> {};

template <typename T, typename... Ts>
struct HasType<T, PDP::Tuple<T, Ts...>> : LD::TrueType {};


template <typename P0, typename P1>
struct HasTypes;

template <typename... T0s, typename... T1s>
struct HasTypes<VariadicPack<T0s...>, PDP::Tuple<T1s...>>
        : LD::Detail::IntegralConstant<bool,(PDP::HasType<T0s, PDP::Tuple<T1s...>>::value && ...)>
{
};


template <typename V, typename T>
struct IsInTuple;

template <typename V, typename T0, typename... T>
struct IsInTuple <V, VariadicPack<T0, T...> >
{
    static const bool value = IsInTuple<typename PDP::Decay<V>::type, VariadicPack<typename PDP::Decay<T>::type...> >::value;
};

template <typename V, typename... T>
struct IsInTuple <V, VariadicPack<V, T...> >
{
    static const bool value = true;
};

template <typename V>
struct IsInTuple <V, VariadicPack<> >
{
    static const bool value = false;
};

template <typename P0, typename P1>
struct Contained;

template <typename... T0s, typename... T1s>
struct Contained<VariadicPack<T0s...>, VariadicPack<T1s...>>
        : LD::Detail::IntegralConstant<bool,(IsInTuple<typename PDP::Decay<T0s>::type, VariadicPack<typename PDP::Decay<T1s>::type...>>::value && ...)>
{

};

template <typename T0s, typename... T1s>
struct Contained<T0s, VariadicPack<T1s...>>
        : Contained<VariadicPack<T0s>, VariadicPack<T1s...>>
{

};


template <typename... T0s, typename T1s>
struct Contained<VariadicPack<T0s...>, T1s>
        : Contained<VariadicPack<T1s>, VariadicPack<T0s...>>
{

};

namespace Detail
{
    // Get i-th element of parameter pack
    template<int n, typename F, typename... T>
    struct ElementAt : public ElementAt<n-1, T...>
    {
    };

    template<typename F, typename... T>
    struct ElementAt<0, F, T...>
    {
        typedef F type;
    };

    // Get i-th element of pack
    template<int n, typename P>
    struct Element
    {
    };

    template<int n, typename... T>
    struct Element<n, VariadicPack<T...>>
    {
        typedef typename ElementAt<n, T...>::type type;
    };

    // Concat at left
    template<typename a, typename b, class = void>
    struct TupleConcatLeft
    {
    };

    template<typename a, typename... b>
    struct TupleConcatLeft<a, VariadicPack<b...>,PDP::Enable_If_T<Contained<a, VariadicPack<b...>>::value>>
    {
        typedef VariadicPack<a, b...> type;
    };

    template<typename a, typename... b>
    struct TupleConcatLeft<a, VariadicPack<b...>,PDP::Enable_If_T<!Contained<a, VariadicPack<b...>>::value>>
    {
        typedef VariadicPack<b...> type;
    };
}


// Concat 2 tuples
template<typename a, typename b, int n = 0, bool ok = (n < a::size)>
struct ConcatTuple : public ConcatTuple<a, b, n+1>
{
    typedef typename Detail::TupleConcatLeft<
            typename Detail::Element<n, a>::type,
            typename ConcatTuple<a, b, n+1>::type
    >::type type;
};

template<typename a, typename b, int n>
struct ConcatTuple<a, b, n, false>
{
    typedef b type;
};



template<typename T> class Debug_T;
template<class A, template<class...> class B>
struct Rebind;

template<template<class...> class A, class... T, template<class...> class B>
struct Rebind<A<T...>, B> {
    using type = B<T...>;
};

/*
     template<typename T>
     struct IsPrimitiveType
     {
     static const bool value = IsCharacter<T>::value || IsUnsignedCharacter<T>::value ||
     Is16BitInteger<T>::value || IsUnsigned16BitInteger<T>::value ||
     Is16Extended32BitInteger<T>::value ||IsUnsigned16BitExtended32BitInteger<T>::value ||
     Is32BitInteger<T>::value || IsUnsigned32BitInteger<T>::value ||
     Is64BitInteger<T>::value || IsUnsigned64BitInteger<T>::value ||
     Is64Bit32BitExtended<T>::value || IsUnsigned64Bit32BitExtended<T>::value ||
     Is64Bit64BitExtended<T>::value || IsUnsigned64Bit64BitExtended<T>::value ||
     Is64Bit64Bit32BitExtended<T>::value || IsUnsigned64Bit64Bit32BitExtended<T>::value ||
     IsBoolean<T>::value || IsDouble<T>::value || IsFloat<T>::value || IsDoubleExtended64Bit<T>::value;
     };
     */
//
template<typename T, class = void>
struct NumericSize
{

};

template<typename T>
struct NumericSize<T,PDP::Enable_If_T<PDP::IsPrimitiveType<T>::value>>
{
    static const LD::UInteger value = sizeof(T);
};

template<typename T, typename U, class = void>
struct Cast;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T, typename U>
struct Cast<T, U,PDP::Enable_If_T<(PDP::IsFloatingPoint<T>::value && PDP::IsFloatingPoint<U>::value) && (PDP::NumericSize<T>::value == PDP::NumericSize<U>::value)>>
{
    typedef T type;
};


template<typename T, typename U>
struct Cast<T, U,PDP::Enable_If_T<(PDP::IsFloatingPoint<T>::value && PDP::IsFloatingPoint<U>::value) && (PDP::NumericSize<T>::value > PDP::NumericSize<U>::value)>>
{
    typedef T type;
};


template<typename T, typename U>
struct Cast<T, U,PDP::Enable_If_T<(PDP::IsFloatingPoint<T>::value && PDP::IsFloatingPoint<U>::value) && (PDP::NumericSize<T>::value < PDP::NumericSize<U>::value)>>
{
    typedef T type;
};

template<typename T, typename U>
struct Cast<T, U,PDP::Enable_If_T<(PDP::IsFloatingPoint<T>::value && PDP::IsInteger<U>::value)>>
{
    typedef T type;
};

template<typename T, typename U>
struct Cast<T, U,PDP::Enable_If_T<(PDP::IsFloatingPoint<T>::value && PDP::IsUnsignedInteger<U>::value)>>
{
    typedef T type;
};

template<typename T, typename U>
struct Cast<T, U,PDP::Enable_If_T<(PDP::IsFloatingPoint<T>::value && PDP::IsCharacter<U>::value)>>
{
    typedef T type;
};


template<typename T, typename U>
struct Cast<T, U,PDP::Enable_If_T<(PDP::IsFloatingPoint<T>::value && PDP::IsBoolean<U>::value)>>
{
    typedef T type;
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T, typename U>
struct Cast<T, U,PDP::Enable_If_T<(PDP::IsUnsignedInteger<T>::value && PDP::IsUnsignedInteger<U>::value) && (PDP::NumericSize<T>::value == PDP::NumericSize<U>::value)>>
{
    typedef T type;
};


template<typename T, typename U>
struct Cast<T, U,PDP::Enable_If_T<(PDP::IsUnsignedInteger<T>::value && PDP::IsUnsignedInteger<U>::value) && (PDP::NumericSize<T>::value > PDP::NumericSize<U>::value)>>
{
    typedef T type;
};


template<typename T, typename U>
struct Cast<T, U,PDP::Enable_If_T<(PDP::IsUnsignedInteger<T>::value && PDP::IsUnsignedInteger<U>::value) && (PDP::NumericSize<T>::value < PDP::NumericSize<U>::value)>>
{
    typedef U type;
};

template<typename T, typename U>
struct Cast<T, U,PDP::Enable_If_T<(PDP::IsUnsignedInteger<T>::value && PDP::IsFloatingPoint<U>::value)>>
{
    typedef U type;
};


template<typename T, typename U>
struct Cast<T, U,PDP::Enable_If_T<(PDP::IsUnsignedInteger<T>::value && PDP::IsInteger<U>::value)>>
{
    typedef U type;
};


template<typename T, typename U>
struct Cast<T, U,PDP::Enable_If_T<(PDP::IsUnsignedInteger<T>::value && PDP::IsCharacter<U>::value)>>
{
    typedef T type;
};


template<typename T, typename U>
struct Cast<T, U,PDP::Enable_If_T<(PDP::IsUnsignedInteger<T>::value && PDP::IsBoolean<U>::value)>>
{
    typedef T type;
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename T, typename U>
struct Cast<T, U,PDP::Enable_If_T<((PDP::IsInteger<T>::value && PDP::IsInteger<U>::value) && (PDP::NumericSize<T>::value == PDP::NumericSize<U>::value))>>
{
    typedef T type;
};


template<typename T, typename U>
struct Cast<T, U,PDP::Enable_If_T<((PDP::IsInteger<T>::value && PDP::IsInteger<U>::value) && (PDP::NumericSize<T>::value > PDP::NumericSize<U>::value))>>
{
    typedef T type;
};


template<typename T, typename U>
struct Cast<T, U,PDP::Enable_If_T<((PDP::IsInteger<T>::value && PDP::IsInteger<U>::value) && (PDP::NumericSize<T>::value < PDP::NumericSize<U>::value))>>
{
    typedef U type;
};


template<typename T, typename U>
struct Cast<T, U,PDP::Enable_If_T<(PDP::IsInteger<T>::value && PDP::IsFloatingPoint<U>::value)>>
{
    typedef U type;
};

template<typename T, typename U>
struct Cast<T, U,PDP::Enable_If_T<PDP::IsInteger<T>::value && PDP::IsUnsignedInteger<U>::value>>
{
    typedef T type;
};

template<typename T, typename U>
struct Cast<T, U,PDP::Enable_If_T<PDP::IsInteger<T>::value && PDP::IsCharacter<U>::value>>
{
    typedef T type;
};

template<typename T, typename U>
struct Cast<T, U,PDP::Enable_If_T<PDP::IsInteger<T>::value && PDP::IsBoolean<U>::value>>
{
    typedef T type;
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T, typename U>
struct Cast<T, U,PDP::Enable_If_T<(PDP::IsCharacter<T>::value && PDP::IsCharacter<U>::value) && (PDP::NumericSize<T>::value == PDP::NumericSize<U>::value)>>
{
    typedef T type;
};


template<typename T, typename U>
struct Cast<T, U,PDP::Enable_If_T<(PDP::IsCharacter<T>::value && PDP::IsCharacter<U>::value) && (PDP::NumericSize<T>::value > PDP::NumericSize<U>::value)>>
{
    typedef T type;
};


template<typename T, typename U>
struct Cast<T, U,PDP::Enable_If_T<(PDP::IsCharacter<T>::value && PDP::IsCharacter<U>::value) && (PDP::NumericSize<T>::value < PDP::NumericSize<U>::value)>>
{
    typedef U type;
};


template<typename T, typename U>
struct Cast<T, U,PDP::Enable_If_T<PDP::IsCharacter<T>::value && PDP::IsFloatingPoint<U>::value>>
{
    typedef U type;
};


template<typename T, typename U>
struct Cast<T, U,PDP::Enable_If_T<PDP::IsCharacter<T>::value && PDP::IsUnsignedInteger<U>::value>>
{
    typedef U type;
};

template<typename T, typename U>
struct Cast<T, U,PDP::Enable_If_T<PDP::IsCharacter<T>::value && PDP::IsInteger<U>::value>>
{
    typedef U type;
};


template<typename T, typename U>
struct Cast<T, U,PDP::Enable_If_T<PDP::IsCharacter<T>::value && PDP::IsBoolean<U>::value>>
{
    typedef T type;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename T, typename U>
struct Cast<T, U,PDP::Enable_If_T<(PDP::IsBoolean<T>::value && PDP::IsBoolean<U>::value) && (PDP::NumericSize<T>::value == PDP::NumericSize<U>::value)>>
{
    typedef T type;
};


template<typename T, typename U>
struct Cast<T, U,PDP::Enable_If_T<(PDP::IsBoolean<T>::value && PDP::IsBoolean<U>::value) && (PDP::NumericSize<T>::value > PDP::NumericSize<U>::value)>>
{
    typedef T type;
};


template<typename T, typename U>
struct Cast<T, U,PDP::Enable_If_T<(PDP::IsBoolean<T>::value && PDP::IsBoolean<U>::value) && (PDP::NumericSize<T>::value < PDP::NumericSize<U>::value)>>
{
    typedef U type;
};

template<typename T, typename U>
struct Cast<T, U,PDP::Enable_If_T<PDP::IsBoolean<T>::value && PDP::IsFloatingPoint<U>::value>>
{
    typedef U type;
};


template<typename T, typename U>
struct Cast<T, U,PDP::Enable_If_T<PDP::IsBoolean<T>::value && PDP::IsUnsignedInteger<U>::value>>
{
    typedef U type;
};

template<typename T, typename U>
struct Cast<T, U,PDP::Enable_If_T<PDP::IsBoolean<T>::value && PDP::IsInteger<U>::value>>
{
    typedef U type;
};


template<typename T, typename U>
struct Cast<T, U,PDP::Enable_If_T<PDP::IsBoolean<T>::value && PDP::IsCharacter<U>::value>>
{
    typedef U type;
};

template<typename T, typename U>
struct Cast<T, U,PDP::Enable_If_T<PDP::IsBoolean<T>::value && PDP::IsBoolean<U>::value>>
{
    typedef U type;
};





#define GENERATE_HAS_MEMBER_TYPE(Type)                                            \
\
template < class T >                                                              \
class HasMemberType_##Type                                                        \
{                                                                                 \
private:                                                                          \
using Yes = char[2];                                                          \
using  No = char[1];                                                          \
\
struct Fallback { struct Type { }; };                                         \
struct Derived : T, Fallback { };                                             \
\
template < class U >                                                          \
static No& test ( typename U::Type* );                                        \
template < typename U >                                                       \
static Yes& test ( U* );                                                      \
\
public:                                                                           \
static constexpr bool RESULT = sizeof(test<Derived>(nullptr)) == sizeof(Yes); \
};                                                                                \
\
template < class T >                                                              \
struct has_member_type_##Type                                                     \
: public std::integral_constant<bool, HasMemberType_##Type<T>::RESULT>            \
{ };                                                                              \


#define GENERATE_HAS_MEMBER(member)                                               \
\
template < class T >                                                              \
class HasMember_##member                                                          \
{                                                                                 \
private:                                                                          \
using Yes = char[2];                                                          \
using  No = char[1];                                                          \
\
struct Fallback { int member; };                                              \
struct Derived : T, Fallback { };                                             \
\
template < class U >                                                          \
static No& test ( decltype(U::member)* );                                     \
template < typename U >                                                       \
static Yes& test ( U* );                                                      \
\
public:                                                                           \
static constexpr bool RESULT = sizeof(test<Derived>(nullptr)) == sizeof(Yes); \
};                                                                                \
\
template < class T >                                                              \
struct has_member_##member                                                        \
: public std::integral_constant<bool, HasMember_##member<T>::RESULT>              \
{                                                                                 \
};


#define PDP_STATIC_CHECK(input,message)



#if false
#error Mooo
#endif




#define DEFINE_HAS_SIGNATURE(traitsName, funcName, signature)               \
template <typename U>                                                   \
class traitsName                                                        \
{                                                                       \
private:                                                                \
template<typename T, T> struct helper;                              \
template<typename T>                                                \
static unsigned char check(helper<signature, &funcName>*);           \
template<typename T> static unsigned short check(...);               \
public:                                                                 \
static                                                              \
constexpr bool value = sizeof(check<U>(0)) == sizeof(unsigned char); \
}


DEFINE_HAS_SIGNATURE(has_foo, T::foo, void (*)(void));
}

namespace LD
{

    /**
     @breif An empty class to help differentiate between functioning functors and placeholder functors.  This is a necessary evil due to the type heavyness of C++
     */
    template<typename T>
    class NullFunctor
    {

    };



    /**
     @brief An empty class to help differentiate between empty class situations and needs an actual typename - this helps the compiler better compile without longer work loads
     */
    class NullClass
    {

    };







    template <typename... Ts> struct VariadicPack { static const unsigned int size = sizeof...(Ts);};







    inline bool IsNan(const float & f)
    {
        const unsigned int u = *(unsigned int*)&f;
        return (u&0x7F800000) == 0x7F800000 && (u&0x7FFFFF);    // Both NaN and qNan.
    }

    inline bool IsNan(const double & d)
    {
        const unsigned long u = *(unsigned int*)&d;
        return (u&0x7FF0000000000000ULL) == 0x7FF0000000000000ULL && (u&0xFFFFFFFFFFFFFULL);
    }


    inline LD::UInteger FastLog10(LD::UInteger v)
    {
        return (v >= 1000000000u) ? 9 : (v >= 100000000u) ? 8 :
                                        (v >= 10000000u) ? 7 : (v >= 1000000u) ? 6 :
                                                               (v >= 100000u) ? 5 : (v >= 10000u) ? 4 :
                                                                                    (v >= 1000u) ? 3 : (v >= 100u) ? 2 : (v >= 10u) ? 1u : 0u;
    }
    inline LD::UInteger FastAbs(LD::Integer integer)
    {
        LD::Integer value = integer;
        LD::Integer temp = value >> 31;     // make a mask of the sign bit
        value ^= temp;                   // toggle the bits if value is negative
        value += temp & 1;
        return value;
    }
    inline constexpr LD::Integer Floor(LD::Float x) noexcept
    {
        if(x>0)return (LD::Integer)x;
        return (LD::Integer)(x-((LD::Float)0.9999999999999999));
    }
    inline constexpr LD::UInteger Power(const LD::UInteger & x, const LD::UInteger & y) noexcept
    {
        LD::UInteger temp = 0;
        if( y == 0)
            return 1;
        temp = LD::Power(x, y/2);
        if (y%2 == 0)
            return temp*temp;
        else
            return x*temp*temp;
    }
    inline constexpr LD::Float Power(const LD::Float & x, const LD::Integer & y) noexcept
    {
        LD::Float temp = 0;
        if( y == 0)
            return 1;
        temp = LD::Power(x, y/2);
        if (y%2 == 0)
            return temp*temp;
        else
        {
            if(y > 0)
                return x*temp*temp;
            else
                return (temp*temp)/x;
        }
    }


    template<typename T> class MaximumDecimalPlaces
    {
    public:
        static LD::UInteger GetMaximumAmountOfDecimalPlaces()
        {
            return 0;
        }
    };

    template<> class MaximumDecimalPlaces<float>
    {
    public:
        static constexpr LD::UInteger GetMaximumAmountOfDecimalPlaces() noexcept
        {
            return 7;
        }
    };
    template<> class MaximumDecimalPlaces<double>
    {
    public:
        static constexpr LD::UInteger GetMaximumAmountOfDecimalPlaces() noexcept
        {
            return 15;
        }
    };

    inline LD::UInteger GetDecimalPlaces(LD::Float dbVal)
    {
        static const LD::UInteger MAX_DP = MaximumDecimalPlaces<LD::Float>::GetMaximumAmountOfDecimalPlaces();
        static const LD::Float THRES = LD::Power((LD::Float)0.1, (LD::Integer)MAX_DP);
        if (dbVal == 0.0)
            return 0;
        LD::UInteger nDecimal = 0;
        while (dbVal - LD::Floor(dbVal) > THRES && nDecimal < MAX_DP)
        {
            dbVal *= 10.0;
            nDecimal++;
        }
        return nDecimal;
    }
    inline LD::UInteger ceil_log2(LD::UInteger x)
    {
        static const unsigned long long t[6] = {
                0xFFFFFFFF00000000ull,
                0x00000000FFFF0000ull,
                0x000000000000FF00ull,
                0x00000000000000F0ull,
                0x000000000000000Cull,
                0x0000000000000002ull
        };

        LD::Integer y = (((x & (x - 1)) == 0) ? 0 : 1);
        LD::Integer j = 32;
        LD::Integer i;

        for (i = 0; i < 6; i++)
        {
            LD::Integer k = (((x & t[i]) == 0) ? 0 : j);
            y += k;
            x >>= k;
            j >>= 1;
        }

        return y;
    }

    namespace Detail
    {


        struct IsConvertibleImpl
        {
            template <typename _Type, typename _OtherType>
            static void convert(_OtherType);
            template <typename _Type, typename _OtherType>
            static auto test(_Type&&, _OtherType&&) -> decltype(convert<_Type, _OtherType>(LD::Declval<_Type>()), LD::TrueType());
            static auto test(...) -> decltype(LD::FalseType());


            typedef char small_;

            struct big_ {
                char dummy[2];
            };

            template <class U> static small_ tester(void (U::*)());
            template <class U> static big_ tester(...);
            //static const bool value = sizeof(tester<T>(0)) == sizeof(small_);


            template <typename From, typename To>
            struct ConvertHelper {
                static small_ Test(To);
                static big_ Test(...);
                static From Create();
            };
        };



        template <typename _Type, typename _OtherType>
        struct IsConvertible : public decltype(Detail::IsConvertibleImpl::test(LD::Declval<_Type>(), LD::Declval<_OtherType>()))
        {};



        /*
        template <typename From, typename To>
        struct IsConvertible
                : LD::Detail::IntegralConstant<bool,
                        sizeof(IsConvertibleImpl::ConvertHelper<From, To>::Test(IsConvertibleImpl::ConvertHelper<From, To>::Create()))== sizeof(IsConvertibleImpl::small_)>
        {

        };
         */


        namespace Detail
        {
            typedef char small_;

            struct big_ {
                char dummy[2];
            };

            template <class U> static small_ tester(void (U::*)());
            template <class U> static big_ tester(...);
            //static const bool value = sizeof(tester<T>(0)) == sizeof(small_);


            template <typename From, typename To>
            struct ConvertHelper {
                static small_ Test(To);
                static big_ Test(...);
                static From Create();
            };
        }


        /*
        // Inherits from true_type if From is convertible to To, false_type otherwise.
        template <typename From, typename To>
        struct IsConvertible
        : LD::Detail::IntegralConstant<bool,
        sizeof(Detail::ConvertHelper<From, To>::Test(Detail::ConvertHelper<From, To>::Create()))== sizeof(Detail::small_)>
        {

        };
         */





        //template <class _Tp> struct  IsPointer : public LD::Detail::__libcpp_is_pointer<typename LD::Detail::RemoveCV<_Tp>::type> {};





























    // is_void


































#if _LIBCPP_STD_VER > 14 && !defined(_LIBCPP_HAS_NO_VARIABLE_TEMPLATES)
template <class _Tp>  bool IsConst_V
        = IsConst<_Tp>::value;
#endif






template <class _Tp> struct  RemoveConst            {typedef _Tp type;};
template <class _Tp> struct  RemoveConst<const _Tp> {typedef _Tp type;};
template<typename T>
struct RemoveConst<volatile T> {
    typedef T type;
};
template<typename T>
struct RemoveConst<const volatile T> {
    typedef T type;
};





#if _LIBCPP_STD_VER > 11
template <class _Tp> using RemoveConst_T = typename RemoveConst<_Tp>::type;
#endif


template < bool... > struct All;
template < > struct All<> : LD::TrueType {};
template < bool B, bool... Rest > struct All<B,Rest...>
{
    constexpr static bool value = B && All<Rest...>::value;
};
/*
         template< bool... Bs >
         using is_all = std::integral_constant< bool, std::is_same< integer_sequence< bool, true, Bs... >, integer_sequence< bool, Bs..., true > >::value >;
         */
#if _LIBCPP_STD_VER > 11
template< bool... Bs > using IsAll_V = typename All<Bs...>::value;
#endif



template <LD::UInteger N, LD::UInteger base=2>
struct CompileTimeLog
{
    enum { value = 1 + CompileTimeLog<N/base, base>::value };
};

template <LD::UInteger base>
struct CompileTimeLog<1, base>
{
    enum { value = 0 };
};

template <LD::UInteger base>
struct CompileTimeLog<0, base>
{
    enum { value = 0 };
};


template<LD::UInteger Size> using CompileTimeLog_V = typename CompileTimeLog<Size>::value;


template <LD::UInteger A, LD::UInteger B>
struct CompileTimeMax
{
    static const LD::UInteger value = (A > B)?A:B;
};


template <LD::UInteger A, LD::UInteger B>
struct CompileTimePow
{
    static const LD::UInteger value = A * CompileTimePow<A, B - 1>::value;
};
template <LD::UInteger A>
struct CompileTimePow<A, 0>
{
    static const LD::UInteger value = 1;
};

template<LD::UInteger Size, LD::UInteger Alignment> using CompileTimePow_V = typename CompileTimePow<Size,Alignment>::value;



template<LD::UInteger Primary, LD::UInteger Secondary>
struct CompileTimeModulus
{
    static const bool value = !(Primary%Secondary);
};

template<LD::UInteger Size, LD::UInteger Alignment> using CompileTimeModulus_V = typename CompileTimeModulus<Size,Alignment>::value;

template<LD::UInteger Primary, LD::UInteger Secondary>
struct CompileTimeDivision
{
    static const LD::UInteger value = Primary/Secondary;
};

template<LD::UInteger Size, LD::UInteger Alignment> using CompileTimeDivision_V = typename CompileTimeDivision<Size,Alignment>::value;

template<LD::UInteger Primary, LD::UInteger Secondary>
struct CompileTimeMultiplication
{
    static const LD::UInteger value = Primary*Secondary;
};

template<LD::UInteger Size, LD::UInteger Alignment> using CompileTimeMultiplication_V = typename CompileTimeMultiplication<Size,Alignment>::value;

template<LD::UInteger Size>
struct DetermineAlignedSize
{
    static const LD::UInteger value = (Size<2 || Size&(Size-1))?CompileTimePow<2, CompileTimeLog<Size,2>::value+1>::value:Size;
};

template<LD::UInteger Size, LD::UInteger Alignment = 8>
struct CompileTimeAlignmentSize
{
    static const LD::UInteger value = LD::Detail::CompileTimeModulus<Size, Alignment>::value?Size:LD::Detail::CompileTimeMultiplication<Alignment, LD::Detail::CompileTimeDivision<Size, Alignment>::value+1>::value;
};


template<LD::UInteger Size, LD::UInteger Alignment> using CompileTimeAlignmentSize_V = typename CompileTimeAlignmentSize<Size,Alignment>::value;


















//template<typename B, typename D> using IsSame_V = typename IsSame<B,D>::value;




template<typename B, typename D>
class IsKindOf
{
public:
    static const bool value = LD::Detail::IsBaseOf<B, D>::value || LD::Detail::IsSame<B, D>::value;
};


template<typename B, typename D>  using IsKindOf_V = typename IsKindOf<B,D>::value;



template <typename T>
struct HasFractionalTypeDefinition
{
    // Types "yes" and "no" are guaranteed to have different sizes,
    // specifically sizeof(yes) == 1 and sizeof(no) == 2.
    typedef char yes[1];
    typedef char no[2];

    template <typename C>
    static yes& test(typename C::FractionPrincipalType*);

    template <typename>
    static no& test(...);

    // If the "sizeof" of the result of calling test<T>(nullptr) is equal to sizeof(yes),
    // the first overload worked and T has a nested type named foobar.
    static const bool value = (sizeof(test<T>(nullptr)) == sizeof(yes)) || LD::Detail::IsIntegrelType<T>::value;
};

template<typename T> using HasFractionalTypeDefinition_V = typename HasFractionalTypeDefinition<T>::value;





/*
template <class _Tp, bool =
LD::Detail::IsClassType<_Tp>::value ||
LD::Detail::IsUnion<_Tp>::value ||
LD::Detail::IsVoid<_Tp>::value  ||
LD::Detail::IsReference<_Tp>::value ||
LD::Detail::IsNullPtr<_Tp>::value >
struct __libcpp_is_function
        : public LD::Detail::IntegralConstant<bool, sizeof(__libcpp_is_function_imp::__test<_Tp>(__libcpp_is_function_imp::__source<_Tp>(0))) == 1>
{};
template <class _Tp> struct __libcpp_is_function<_Tp, true> : public LD::FalseType {};

template <class _Tp> struct IsFunction
        : public __libcpp_is_function<_Tp> {};



template <class _Tp, bool = LD::Detail::IsReference<_Tp>::value ||LD::Detail::IsFunction<_Tp>::value  ||LD::Detail::IsConst<_Tp>::value     >
struct __add_const             {typedef _Tp type;};

template <class _Tp>
struct __add_const<_Tp, false> {typedef const _Tp type;};

template <class _Tp> struct AddConst
{typedef typename __add_const<_Tp>::type type;};

*/

template<class _Ty>
struct _Remove_reference
{   // remove reference
    typedef _Ty _Type;
};



template<class _Ty>
struct _Remove_reference<_Ty&>
{   // remove reference
    typedef _Ty _Type;
};

template<class _Ty>
struct _Remove_reference<_Ty&&>
{   // remove rvalue reference
    typedef _Ty _Type;
};

//typename _Remove_reference<_Ty>::_Type&&
template<class _Ty> inline LD::Enable_If_T<!LD::Detail::IsSame<_Ty, void>::value,typename _Remove_reference<_Ty>::_Type&&> Move(_Ty&& _Arg)
{   // forward _Arg as movable
    return ((typename _Remove_reference<_Ty>::_Type&&)_Arg);
}

/*
template <class _Tp> struct  RemoveReference        {typedef _Tp type;};
        template <class _Tp> struct  RemoveReference<_Tp&>        {typedef _Tp type;};
        template <class _Tp> struct  RemoveReference<_Tp&&>        {typedef _Tp type;};
        */





}







//typename _Remove_reference<_Ty>::_Type&&
template<class _Ty> inline LD::Enable_If_T<!LD::Detail::IsSame<_Ty, void>::value,typename LD::Detail::_Remove_reference<_Ty>::_Type&&> Move(_Ty&& _Arg)
{   // forward _Arg as movable
    return ((typename LD::Detail::_Remove_reference<_Ty>::_Type&&)_Arg);
}
template <class _Tp>
inline
_Tp&& Forward(typename LD::Detail::RemoveReference<_Tp>::type& __t) _NOEXCEPT
{
    return static_cast<_Tp&&>(__t);
}



namespace Detail
{
    namespace Fibonnaci
    {
        inline void multiply(LD::UInteger F[2][2], LD::UInteger M[2][2])
        {
            LD::UInteger x =  F[0][0] * M[0][0] + F[0][1] * M[1][0];
            LD::UInteger y =  F[0][0] * M[0][1] + F[0][1] * M[1][1];
            LD::UInteger z =  F[1][0] * M[0][0] + F[1][1] * M[1][0];
            LD::UInteger w =  F[1][0] * M[0][1] + F[1][1] * M[1][1];
            F[0][0] = x;
            F[0][1] = y;
            F[1][0] = z;
            F[1][1] = w;
        }

        /*
             * function to calculate power of a matrix
             */
        inline void power(LD::UInteger F[2][2], LD::UInteger n)
        {
            if (n == 0 || n == 1)
                return;
            LD::UInteger M[2][2] = {{1,1},{1,0}};
            LD::Detail::Fibonnaci::power(F, n / 2);
            LD::Detail::Fibonnaci::multiply(F, F);
            if (n % 2 != 0)
                multiply(F, M);
        }

        /*
             * function that returns nth Fibonacci number
             */
    }
}

inline LD::UInteger Fibonnaci(LD::UInteger n)
{
    LD::UInteger F[2][2] = {{1,1},{1,0}};
    if (n == 0)
        return 0;
    Detail::Fibonnaci::power(F, n - 1);
    return F[0][0];
}







template<typename ... Args> class Tuple;

namespace Detail
{
    template<class... _Types> class TupleImpl;
    // empty tuple
    template<> class TupleImpl<> {};
    // recursive tuple definition
    template<class _This,
            class... _Rest>
    class TupleImpl<_This, _Rest...>
            : private TupleImpl<_Rest...>
    {
    public:
        _This _Myfirst;


        TupleImpl(){}
        TupleImpl(_This first, _Rest... rest): TupleImpl<_Rest...>(rest...), _Myfirst(first) {}
    };

    // tuple_element
    template<LD::UInteger _Index, class _Tuple> struct TupleElement;

    // select first element
    template<class _This, class... _Rest>
    struct TupleElement<0, TupleImpl<_This, _Rest...>>
    {

        typedef _This& type;
        typedef TupleImpl<_This, _Rest...> _Ttype;

    };

    template<class _This, class... _Rest>
    struct TupleElement<0, Tuple<_This, _Rest...>>
    {
        typedef _This& type;
        typedef Tuple<_This, _Rest...> _Ttype;
    };

    // recursive tuple_element definition
    template <LD::UInteger _Index, class _This, class... _Rest>
    struct TupleElement<_Index, Tuple<_This, _Rest...>>
            : public TupleElement<_Index - 1, Tuple<_Rest...> >
    {

    };

    // recursive tuple_element definition
    template <LD::UInteger _Index, class _This, class... _Rest>
    struct TupleElement<_Index, TupleImpl<_This, _Rest...>>
            : public TupleElement<_Index - 1, TupleImpl<_Rest...> >
    {

    };

    template <class Tuple, class T, LD::UInteger Index = 0>
    struct FindFirstElementIndex;

    template <LD::UInteger Index, bool Valid>
    struct FindFirstElementIndexFinalTest
            : public LD::Detail::IntegralConstant<LD::UInteger, Index>
    {
    };

    template <LD::UInteger Index>
    struct FindFirstElementIndexFinalTest<Index, false>
    {
        static_assert(Index == -1, "Type not found");
    };

















    /*
    template <typename T>
    struct Decay
    {
        template <typename U> static U impl(U);
        using type = decltype(impl(LD::Declval<T>()));

        //using type = typename LD::Detail::decay<T>::type;
    };
     */





#if _LIBCPP_STD_VER > 11
    template <class _Tp> using Decay_T = typename Decay<_Tp>::type;
#endif


    template <class Head, class T, LD::UInteger Index>
    struct FindFirstElementIndex<Tuple<Head>, T, Index>
            : public FindFirstElementIndexFinalTest<Index, LD::Detail::IsSame<typename LD::Detail::Decay<Head>::type, T>::value>
    {
    };

    template <class Head, class ...Rest, class T, LD::UInteger Index>
    struct FindFirstElementIndex<Tuple<Head, Rest...>, T, Index>
            : public LD::Detail::Conditonal<LD::Detail::IsSame<typename LD::Detail::Decay<Head>::type, T>::value,
                    LD::Detail::IntegralConstant<LD::UInteger, Index>,
                    FindFirstElementIndex<Tuple<Rest...>, T, Index+1>>::type
    {

    };

    template<LD::UInteger index, typename ... Args>
    Tuple<Args...> & Assign(Tuple<Args...> & tuple)
    {
        return tuple;
    }
}

template<LD::UInteger _Index, class... _Types> inline
constexpr typename Detail::TupleElement<_Index, Tuple<_Types...>>::type
Get(Tuple<_Types...>& _Tuple);

template<LD::UInteger _Index, class... _Types> inline
constexpr const typename Detail::TupleElement<_Index, Tuple<_Types...>>::type
Get(const Tuple<_Types...>& _Tuple);

template<typename Type, class... _Types> inline
constexpr Type & Get(Tuple<_Types...>& _Tuple);

template<typename Type, class... _Types> inline
constexpr const Type & Get(const Tuple<_Types...>& _Tuple);

namespace Detail
{
    template<LD::UInteger index, typename T  ,typename ... Args, typename ... A>
    Tuple<Args...> & Assign(Tuple<Args...> & tuple, T && a,  A && ... arguements)
    {
        Get<index>(tuple) = a;
        return Assign<index+1>(tuple,LD::Forward<A>(arguements)...);
    }

    template<LD::UInteger index,typename ... Args, typename ... A>
    Tuple<Args...> & Assign(Tuple<Args...> & tuple, A && ... arguements)
    {
        return LD::Detail::Assign<index>(tuple,LD::Forward<A>(arguements)...);
    }

    template<int ...S, typename ... Arguements, typename ... Args>
    void TupleAssign(Tuple<typename LD::Detail::Decay<Args>::type...> & tuple, Args && ... arguements)
    {

        LD::Detail::Assign<0>(tuple, LD::Forward<Args>(arguements)...);
    }

    template<typename T, typename U>
    void AssignTupleElementImpl(T & tupleElement,  U && element)
    {
        tupleElement = LD::Move(element);
    }

    template<typename T, typename U, typename ... Args>
    void AssignTupleElementImpl(T & tupleElement,  U && element, Args && ... arguements)
    {
        AssignTupleElementImpl(tupleElement,element,LD::Forward<Args>(arguements)...);
    }

    template<LD::UInteger ... Index, typename ... Args>
    void AssignTupleElement(LD::IndexSequence<Index...>,Tuple<typename LD::Detail::Decay<Args>::type...> & tuple, Args && ... arguements)
    {
        (...,AssignTupleElementImpl(Get<Index>(tuple),LD::Forward<Args>(arguements)));
    }

    template<typename ... Args>
    void AssignTuple(Tuple<typename LD::Detail::Decay<Args>::type...> & tuple, Args && ... arguements)
    {
        AssignTupleElement(typename LD::MakeIndexSequence<sizeof...(Args)>::type{},tuple,LD::Forward<Args>(arguements)...);
    }

    template<typename T, typename U>
    void AssignTupleElementImpl1(T && tupleElement,  U && element)
    {
        tupleElement = element;
    }

    template<typename T, typename U, typename ... Args>
    void AssignTupleElementImpl1(T && tupleElement,  U && element, Args && ... arguements)
    {
        AssignTupleElementImpl(tupleElement,element,LD::Forward<Args>(arguements)...);
    }

    template<LD::UInteger ... Index, typename ... Args, typename ... B>
    void AssignTupleElement1(LD::IndexSequence<Index...>,Tuple<Args...> & tuple, B && ... arguements)
    {
        (...,AssignTupleElementImpl1(Get<Index>(tuple),LD::Forward<Args>(arguements)));
    }

    template<typename ... Args, typename ... B>
    void AssignTuple1(Tuple<Args...> & tuple, B && ... arguements)
    {
        AssignTupleElement1(typename LD::MakeIndexSequence<sizeof...(Args)>::type{},tuple,LD::Forward<B>(arguements)...);
    }

    template<LD::UInteger Index, typename T, typename U>
    void AssignTupelFromTupleBase(T && a, const U & b)
    {
        Get<Index>(a) = Get<Index>(b);
    }

    template<typename T, typename U, LD::UInteger ... Index>
    void AssignTupleFromTupleRec(LD::IndexSequence<Index...>, T && a, const U & b)
    {
        (...,AssignTupelFromTupleBase<Index>(a,b));
    }
    template<typename ... A, typename ... B>
    void AssignTupleFromTuple(Tuple<A...> & a, const Tuple<B...> & b)
    {
        return AssignTupleFromTupleRec(typename LD::MakeIndexSequence<sizeof...(A)>::type{},a,b);
    }

    template<template<typename> class Functor ,typename ... Args, int ...S>
    void InternalInvoke(const Functor<void(Args...)> & function, const Tuple<typename LD::Detail::Decay<Args>::type...> & tuple,LD::IndexSequence<S...>)
    {
        function(Get<S>(tuple) ...);
    }
    template<template<typename> class Functor, typename T ,typename ... Args,int ...S>
    T InternalInvoke(const Functor<T(Args...)> & function, const Tuple<typename LD::Detail::Decay<Args>::type...> & tuple,LD::IndexSequence<S...>)
    {
        return LD::Move(function(Get<S>(tuple) ...));
    }

    template<typename T, typename F, LD::UInteger... Is>
    void for_each(T&& t, F f, LD::IndexSequence<Is...>)
    {
        auto l = { (f(Get<Is>(t)), 0)... };
    }

    template<typename T, typename F, typename U, template<typename> class Functor,  LD::UInteger... Is>
    void for_each(T&& t, F f,T & state, const Functor<U(const U & a, const U & b)> & accumulator, LD::IndexSequence<Is...>)
    {
        T retValue = LD::Move({ (f(Get<Is>(t)), 0)... });
        state = LD::Move(accumulator(state,retValue));
    }
}


template <typename P0, typename P1, typename Predicate = void>
struct ArePacksConvertible;

template <typename... T0s, typename... T1s>
struct ArePacksConvertible<LD::VariadicPack<T0s...>, LD::VariadicPack<T1s...>,LD::Enable_If_T<(true && (sizeof...(T0s) == sizeof...(T1s)))>>
        : LD::Detail::IntegralConstant<bool,(sizeof...(T0s) == sizeof...(T1s))
                                            && (LD::Detail::IsConvertible<T0s, T1s>::value && ...)>
{
};

    template <typename... T0s, typename... T1s>
    struct ArePacksConvertible<LD::VariadicPack<T0s...>, LD::VariadicPack<T1s...>,LD::Enable_If_T<(true && (sizeof...(T0s) != sizeof...(T1s)))>>
            : LD::Detail::IntegralConstant<bool, false>
    {

    };


template<typename ... Args>
class Tuple: public Detail::TupleImpl<Args...>
{
public:
    inline Tuple(){}
    inline Tuple(Args && ... arguements);
    template <typename ... B ,typename = LD::Enable_If_T< LD::ArePacksConvertible<LD::VariadicPack<B...>, LD::VariadicPack<Args...>>::value>>
    explicit Tuple(const Tuple<B...> & tuple)
    {
        (*this) = tuple;

    }

    template <typename ... B ,typename = LD::Enable_If_T< LD::ArePacksConvertible<LD::VariadicPack<Args...>, LD::VariadicPack<B...>>::value>>
    Tuple<Args...> & operator = (const Tuple<B...> & tuple)
    {
        return Assignment(tuple);
    }

    template<typename ... B>
    LD::Enable_If_T< LD::ArePacksConvertible<LD::VariadicPack<Args...>, LD::VariadicPack<B...>>::value,Tuple<Args...> &> Assignment(const Tuple<B...> & tuple);
};

template <> struct Tuple<> {};
// select first element
// get reference to _Index element of tuple
template<LD::UInteger _Index, class... _Types> inline
constexpr typename Detail::TupleElement<_Index, Tuple<_Types...>>::type
Get(Tuple<_Types...>& _Tuple)
{
    typedef typename Detail::TupleElement<_Index, Tuple<_Types...>>::_Ttype _Ttype;
    return (((_Ttype&) _Tuple)._Myfirst);
}




template<LD::UInteger _Index> inline
constexpr void Get(Tuple<>& _Tuple)
{

}

// get reference to _Index element of tuple
template<LD::UInteger _Index, class... _Types> inline
constexpr const typename Detail::TupleElement<_Index, Tuple<_Types...>>::type
Get(const Tuple<_Types...>& _Tuple)
{
    typedef typename Detail::TupleElement<_Index, Tuple<_Types...>>::_Ttype _Ttype;
    return (((_Ttype&) _Tuple)._Myfirst);
}

template<LD::UInteger _Index> inline
constexpr void Get(const Tuple<>& _Tuple)
{

}


template<typename Type, class... _Types> inline
constexpr Type & Get(Tuple<_Types...>& _Tuple)
{
    return  Get<Detail::FindFirstElementIndex<Tuple<_Types...>, Type>::value>(_Tuple);
}

template<typename Type> inline
constexpr void Get(Tuple<>& _Tuple)
{

}

template<typename Type, class... _Types> inline
constexpr const Type & Get(const Tuple<_Types...>& _Tuple)
{
    return  Get<Detail::FindFirstElementIndex<Tuple<_Types...>, Type>::value>(_Tuple);
}

template<typename Type> inline
constexpr void Get(const Tuple<>& _Tuple)
{

}

template <class _Tp> class  TupleSize;

#if !defined(_LIBCPP_CXX03_LANG)
template <class _Tp, class...>
using __enable_if_tuple_size_imp = _Tp;

template <class _Tp>
class  TupleSize<__enable_if_tuple_size_imp<
        const _Tp,
        typename LD::EnableIf<!LD::Detail::IsVolatile<_Tp>::value>::type,
        LD::Detail::IntegralConstant<LD::UInteger, sizeof(TupleSize<_Tp>)>>>
        : public LD::Detail::IntegralConstant<LD::UInteger, TupleSize<_Tp>::value> {};

template <class _Tp>
class  TupleSize<__enable_if_tuple_size_imp<
        volatile _Tp,
        typename LD::EnableIf<!LD::Detail::IsConst<_Tp>::value>::type,
        LD::Detail::IntegralConstant<LD::UInteger, sizeof(TupleSize<_Tp>)>>>
        : public LD::Detail::IntegralConstant<LD::UInteger, TupleSize<_Tp>::value> {};

template <class _Tp>
class  TupleSize<__enable_if_tuple_size_imp<
        const volatile _Tp,
        LD::Detail::IntegralConstant<LD::UInteger, sizeof(TupleSize<_Tp>)>>>
        : public LD::Detail::IntegralConstant<LD::UInteger, TupleSize<_Tp>::value> {};

#else
template <class _Tp> class  TupleSize<const _Tp> : public TupleSize<_Tp> {};
    template <class _Tp> class  TupleSize<volatile _Tp> : public TupleSize<_Tp> {};
    template <class _Tp> class  TupleSize<const volatile _Tp> : public TupleSize<_Tp> {};
#endif


template <class ..._Tp>
class  TupleSize<Tuple<_Tp...> >
        : public LD::Detail::IntegralConstant<LD::UInteger, sizeof...(_Tp)>
{
};


template <>
class  TupleSize<Tuple<> >
        : public LD::Detail::IntegralConstant<LD::UInteger, 0>
{
};


template<typename ...Args>
Tuple<typename LD::Detail::Decay<Args>::type...> MakeTuple(Args && ... arguements)
{
    Tuple<typename LD::Detail::Decay<Args>::type...> tuple;
    Detail::AssignTuple(tuple, LD::Forward<Args>(arguements)...);
    return tuple;
}


template <class ..._Tp>
inline constexpr Tuple<_Tp&...> Tie(_Tp&... __t) _NOEXCEPT
{
    return Tuple<_Tp&...>(__t...);
}






template <typename V, typename T>
struct IsInTuple;

template <typename V, typename T0, typename... T>
struct IsInTuple <V, LD::VariadicPack<T0, T...> >
{
    static const bool value = IsInTuple<typename LD::Detail::Decay<V>::type, LD::VariadicPack<typename LD::Detail::Decay<T>::type...> >::value;
};

template <typename V, typename... T>
struct IsInTuple <V, LD::VariadicPack<V, T...> >
{
    static const bool value = true;
};

template <typename V>
struct IsInTuple <V, LD::VariadicPack<> >
{
    static const bool value = false;
};
template <typename P0, typename P1>
struct Contained;

template <typename... T0s, typename... T1s>
struct Contained<LD::VariadicPack<T0s...>, LD::VariadicPack<T1s...>>
        : LD::Detail::IntegralConstant<bool,(IsInTuple<typename LD::Detail::Decay<T0s>::type, LD::VariadicPack<typename LD::Detail::Decay<T1s>::type...>>::value && ...)>
{

};






namespace Detail
{
    template <LD::UInteger ...I, typename T1, typename T2>
    void CopyTupleImpl(T1 const & from, T2 & to, LD::IndexSequence<I...>)
    {
        int dummy[] = { (Get<I>(to) = Get<I>(from), 0)... };
        static_cast<void>(dummy);
    }

    template <typename T1, typename T2>
    void CopyTupleFromTo(T1 const & from, T2 & to)
    {
        CopyTupleImpl(
                from, to,
                typename LD::MakeIndexSequence<TupleSize<T1>::value>::type{});
    }


    template <typename Tuple1, LD::UInteger... Indices1, typename Tuple2, LD::UInteger... Indices2>
    auto tuple_cat1(Tuple1&& tup1, Tuple2&& tup2,
                    LD::IndexSequence<Indices1...>, LD::IndexSequence<Indices2...>)
    {
        return MakeTuple(
                Get<Indices1>(LD::Forward<Tuple1>(tup1))...,
                Get<Indices2>(LD::Forward<Tuple2>(tup2))...
        );
    }
    template<typename T, typename U, LD::UInteger ... INDEX>
    constexpr auto TupleAreEqualImpl(const T & a, const U & b, LD::IndexSequence<INDEX...> )-> bool
    {
        return (... && (Get<INDEX>(a) == Get<INDEX>(b)));
    }

    template<typename T, typename U, LD::UInteger ... INDEX>
    constexpr auto TupleAreLessImpl(const T & a, const U & b, LD::IndexSequence<INDEX...> )-> bool
    {
        return (... && (Get<INDEX>(a) < Get<INDEX>(b)));
    }

    template<typename T, typename U, char(*)[TupleSize<T>() * TupleSize<U>()] = nullptr>
    const bool TupleAreEqual (const T & a, const U & b)
    {
        return LD::Detail::TupleAreEqualImpl(a, b, typename LD::MakeIndexSequence<TupleSize<typename LD::Detail::Decay<T>::type>::value>::type{});
    }


    template<typename T, typename U, char(*)[TupleSize<T>() * TupleSize<U>()] = nullptr>
    const bool TupleAreLess (const T & a, const U & b)
    {
        return LD::Detail::TupleAreLessImpl(a, b, typename LD::MakeIndexSequence<TupleSize<typename LD::Detail::Decay<T>::type>::value>::type{});
    }


    template<typename T, typename U, LD::UInteger ... Index>
    void TupleSwapImpl(T & a, U & b, LD::IndexSequence<Index...> )
    {
        (...,LD::Swap(Get<Index>(a),Get<Index>(b)));
    }
}

template<typename ... A, typename ... B, char(*)[TupleSize<Tuple<A...>>() * TupleSize<Tuple<B...>>()] = nullptr>
const bool operator == (const Tuple<A...> & a, const Tuple<B...> & b)
{
    return Detail::TupleAreEqual(a,b);
}

template<typename ... A, typename ... B, char(*)[TupleSize<Tuple<A...>>() * TupleSize<Tuple<B...>>()] = nullptr>
const bool operator != (const Tuple<A...> & a, const Tuple<B...> & b)
{
    return !(Detail::TupleAreEqual(a,b));
}

template<typename ... A, typename ... B, char(*)[TupleSize<Tuple<A...>>() * TupleSize<Tuple<B...>>()] = nullptr>
const bool operator <(const Tuple<A...> & a, const Tuple<B...> & b)
{
    return Detail::TupleAreLess(a,b);
}

template<typename ... A, typename ... B, char(*)[TupleSize<Tuple<A...>>() * TupleSize<Tuple<B...>>()] = nullptr>
const bool operator > (const Tuple<A...> & a, const Tuple<B...> & b)
{
    return Detail::TupleAreLess(b,a);
}

template<typename ... A, typename ... B, char(*)[TupleSize<Tuple<A...>>() * TupleSize<Tuple<B...>>()] = nullptr>
const bool operator >= (const Tuple<A...> & a, const Tuple<B...> & b)
{
    return !Detail::TupleAreLess(a,b);
}

template<typename ... A, typename ... B, char(*)[TupleSize<Tuple<A...>>() * TupleSize<Tuple<B...>>()] = nullptr>
const bool operator <= (const Tuple<A...> & a, const Tuple<B...> & b)
{
    return !Detail::TupleAreLess(b,a);
}

template <typename Tuple1, typename Tuple2>
auto ConcatenateTuple(Tuple1&& tup1, Tuple2&& tup2)
{
    return Detail::tuple_cat1(
            LD::Forward<Tuple1>(tup1),
            LD::Forward<Tuple2>(tup2),
            typename LD::MakeIndexSequence<TupleSize<typename LD::Detail::Decay<Tuple1>::type>::value>::type{},
            typename LD::MakeIndexSequence<TupleSize<typename LD::Detail::Decay<Tuple2>::type>::value>::type{}
    );
}

template<typename ... Args>
Tuple<Args...>::Tuple(Args && ... arguements):Detail::TupleImpl<Args...>(arguements...)
{
}






template<typename ... Args>
template<typename ... B>
LD::Enable_If_T< LD::ArePacksConvertible<LD::VariadicPack<Args...>, LD::VariadicPack<B...>>::value,Tuple<Args...> &> Tuple<Args...>::Assignment(const Tuple<B...> & tuple)
{
    Detail::AssignTupleFromTuple(*this,tuple);
    return (*this);
}
template <class... _Tp>
Tuple<_Tp&&...>
ForwardAsTuple(_Tp&&... __t) _NOEXCEPT
{
    return Tuple<_Tp&&...>(LD::Forward<_Tp>(__t)...);
}



/*
    template< class T >
    struct Decay {
        typedef typename LD::Detail::RemoveReference<T>::type U;
        typedef typename PDP::Conditional<
                LD::Detail::IsArray<U>::value,
                typename LD::Detail::RemoveExtent<U>::type*,
                typename PDP::Conditional<
                        LD::Detail::IsFunction<U>::value,
                        typename LD::Detail::add_pointer<U>::type,
                        typename LD::Detail::RemoveCV<U>::type
                >::type
        >::type type;
    };
    */

/*
template <typename T>
struct Decay
{
    template <typename U> static U impl(U);
    using type = decltype(impl(LD::Declval<T>()));
};
 */




namespace Detail
{
    template <class F, class... Args>
    inline auto INVOKE(F&& f, Args&&... args) ->
    decltype(LD::Forward<F>(f)(LD::Forward<Args>(args)...))
    {
        return LD::Forward<F>(f)(LD::Forward<Args>(args)...);
    }

    template <class Base, class T, class Derived>
    inline auto INVOKE(T Base::*pmd, Derived&& ref) ->
    decltype(LD::Forward<Derived>(ref).*pmd)
    {
        return LD::Forward<Derived>(ref).*pmd;
    }

    template <class PMD, class Pointer>
    inline auto INVOKE(PMD&& pmd, Pointer&& ptr) ->
    decltype((*LD::Forward<Pointer>(ptr)).*LD::Forward<PMD>(pmd))
    {
        return (*LD::Forward<Pointer>(ptr)).*LD::Forward<PMD>(pmd);
    }

    template <class Base, class T, class Derived, class... Args>
    inline auto INVOKE(T Base::*pmf, Derived&& ref, Args&&... args) ->
    decltype((LD::Forward<Derived>(ref).*pmf)(LD::Forward<Args>(args)...))
    {
        return (LD::Forward<Derived>(ref).*pmf)(LD::Forward<Args>(args)...);
    }

    template <class PMF, class Pointer, class... Args>
    inline auto INVOKE(PMF&& pmf, Pointer&& ptr, Args&&... args) ->
    decltype(((*LD::Forward<Pointer>(ptr)).*LD::Forward<PMF>(pmf))(LD::Forward<Args>(args)...))
    {
        return ((*LD::Forward<Pointer>(ptr)).*LD::Forward<PMF>(pmf))(LD::Forward<Args>(args)...);
    }







}




template <class> struct ResultOf;
template <class F, class... ArgTypes>
struct ResultOf<F(ArgTypes...)>
{
    using type = decltype(Detail::INVOKE(LD::Declval<F>(),LD::Declval<ArgTypes>()...));
};



template<typename T>
inline T Abs(const T & integer)
{
    //return (integer<0)?integer*-1:integer;

    return (integer < 0)*(integer*-1) + (integer>=0)*integer;
}

}

namespace LD
{

    template<typename T, class = void>
    struct NumericSize
    {

    };

    template<typename T>
    struct NumericSize<T,LD::Enable_If_T<LD::Detail::IsPrimitiveType<T>::value>>
    {
        static const LD::UInteger value = sizeof(T);
    };


    namespace Detail
    {
        template<typename T, typename U, class = void>
        struct Cast;


        //////////////////////////////////////////////////////////////////////////////////////////////////////////////
        template<typename T, typename U>
        struct Cast<T, U,LD::Enable_If_T<(LD::Detail::IsFloatingPoint<T>::value && LD::Detail::IsFloatingPoint<U>::value) && (LD::NumericSize<T>::value == LD::NumericSize<U>::value)>>
        {
            typedef T type;
        };


        template<typename T, typename U>
        struct Cast<T, U,LD::Enable_If_T<(LD::Detail::IsFloatingPoint<T>::value && LD::Detail::IsFloatingPoint<U>::value) && (LD::NumericSize<T>::value > LD::NumericSize<U>::value)>>
        {
            typedef T type;
        };


        template<typename T, typename U>
        struct Cast<T, U,LD::Enable_If_T<(LD::Detail::IsFloatingPoint<T>::value && LD::Detail::IsFloatingPoint<U>::value) && (LD::NumericSize<T>::value < LD::NumericSize<U>::value)>>
        {
            typedef T type;
        };

        template<typename T, typename U>
        struct Cast<T, U,LD::Enable_If_T<LD::Detail::IsFloatingPoint<T>::value && LD::Detail::IsInteger<U>::value>>
        {
            typedef T type;
        };

        template<typename T, typename U>
        struct Cast<T, U,LD::Enable_If_T<LD::Detail::IsFloatingPoint<T>::value && LD::Detail::IsUnsignedInteger<U>::value>>
        {
            typedef T type;
        };

        template<typename T, typename U>
        struct Cast<T, U,LD::Enable_If_T<LD::Detail::IsFloatingPoint<T>::value && LD::Detail::IsCharacter<U>::value>>
        {
            typedef T type;
        };


        template<typename T, typename U>
        struct Cast<T, U,LD::Enable_If_T<LD::Detail::IsFloatingPoint<T>::value && LD::Detail::IsBoolean<U>::value>>
        {
            typedef T type;
        };


        //////////////////////////////////////////////////////////////////////////////////////////////////////////////
        template<typename T, typename U>
        struct Cast<T, U,LD::Enable_If_T<(LD::Detail::IsUnsignedInteger<T>::value && LD::Detail::IsUnsignedInteger<U>::value) && (LD::NumericSize<T>::value == LD::NumericSize<U>::value)>>
        {
            typedef T type;
        };


        template<typename T, typename U>
        struct Cast<T, U,LD::Enable_If_T<(LD::Detail::IsUnsignedInteger<T>::value && LD::Detail::IsUnsignedInteger<U>::value) && (LD::NumericSize<T>::value > LD::NumericSize<U>::value)>>
        {
            typedef T type;
        };


        template<typename T, typename U>
        struct Cast<T, U,LD::Enable_If_T<(LD::Detail::IsUnsignedInteger<T>::value && LD::Detail::IsUnsignedInteger<U>::value) && (LD::NumericSize<T>::value < LD::NumericSize<U>::value)>>
        {
            typedef T type;
        };

        template<typename T, typename U>
        struct Cast<T, U,LD::Enable_If_T<LD::Detail::IsUnsignedInteger<T>::value && LD::Detail::IsFloatingPoint<U>::value>>
        {
            typedef U type;
        };


        template<typename T, typename U>
        struct Cast<T, U,LD::Enable_If_T<LD::Detail::IsUnsignedInteger<T>::value && LD::Detail::IsInteger<U>::value>>
        {
            typedef U type;
        };


        template<typename T, typename U>
        struct Cast<T, U,LD::Enable_If_T<LD::Detail::IsUnsignedInteger<T>::value && LD::Detail::IsCharacter<U>::value>>
        {
            typedef T type;
        };


        template<typename T, typename U>
        struct Cast<T, U,LD::Enable_If_T<LD::Detail::IsUnsignedInteger<T>::value && LD::Detail::IsBoolean<U>::value>>
        {
            typedef T type;
        };
        //////////////////////////////////////////////////////////////////////////////////////////////////////////////

        template<typename T, typename U>
        struct Cast<T, U,LD::Enable_If_T<(LD::Detail::IsInteger<T>::value && LD::Detail::IsInteger<U>::value) && (LD::NumericSize<T>::value == LD::NumericSize<U>::value)>>
        {
            typedef T type;
        };


        template<typename T, typename U>
        struct Cast<T, U,LD::Enable_If_T<(LD::Detail::IsInteger<T>::value && LD::Detail::IsInteger<U>::value) && (LD::NumericSize<T>::value > LD::NumericSize<U>::value)>>
        {
            typedef T type;
        };


        template<typename T, typename U>
        struct Cast<T, U,LD::Enable_If_T<(LD::Detail::IsInteger<T>::value && LD::Detail::IsInteger<U>::value) && (LD::NumericSize<T>::value < LD::NumericSize<U>::value)>>
        {
            typedef T type;
        };


        template<typename T, typename U>
        struct Cast<T,U,LD::Enable_If_T<LD::Detail::IsInteger<T>::value && LD::Detail::IsFloatingPoint<U>::value >>
        {
            typedef U type;
        };
        template<typename T, typename U>
        struct Cast<T, U,LD::Enable_If_T<LD::Detail::IsInteger<T>::value && LD::Detail::IsUnsignedInteger<U>::value>>
        {
            typedef T type;
        };

        template<typename T, typename U>
        struct Cast<T, U,LD::Enable_If_T<LD::Detail::IsInteger<T>::value && LD::Detail::IsCharacter<U>::value>>
        {
            typedef T type;
        };

        template<typename T, typename U>
        struct Cast<T, U,LD::Enable_If_T<LD::Detail::IsInteger<T>::value && LD::Detail::IsBoolean<U>::value>>
        {
            typedef T type;
        };
        //////////////////////////////////////////////////////////////////////////////////////////////////////////////
        template<typename T, typename U>
        struct Cast<T, U,LD::Enable_If_T<(LD::Detail::IsCharacter<T>::value && LD::Detail::IsCharacter<U>::value) && (LD::NumericSize<T>::value == LD::NumericSize<U>::value)>>
        {
            typedef T type;
        };


        template<typename T, typename U>
        struct Cast<T, U,LD::Enable_If_T<(LD::Detail::IsCharacter<T>::value && LD::Detail::IsCharacter<U>::value) && (LD::NumericSize<T>::value > LD::NumericSize<U>::value)>>
        {
            typedef T type;
        };


        template<typename T, typename U>
        struct Cast<T, U,LD::Enable_If_T<(LD::Detail::IsCharacter<T>::value && LD::Detail::IsCharacter<U>::value) && (LD::NumericSize<T>::value < LD::NumericSize<U>::value)>>
        {
            typedef U type;
        };


        template<typename T, typename U>
        struct Cast<T, U,LD::Enable_If_T<LD::Detail::IsCharacter<T>::value && LD::Detail::IsFloatingPoint<U>::value>>
        {
            typedef U type;
        };


        template<typename T, typename U>
        struct Cast<T, U,LD::Enable_If_T<LD::Detail::IsCharacter<T>::value && LD::Detail::IsUnsignedInteger<U>::value>>
        {
            typedef U type;
        };

        template<typename T, typename U>
        struct Cast<T, U,LD::Enable_If_T<LD::Detail::IsCharacter<T>::value && LD::Detail::IsInteger<U>::value>>
        {
            typedef U type;
        };


        template<typename T, typename U>
        struct Cast<T, U,LD::Enable_If_T<LD::Detail::IsCharacter<T>::value && LD::Detail::IsBoolean<U>::value>>
        {
            typedef T type;
        };

        //////////////////////////////////////////////////////////////////////////////////////////////////////////////

        template<typename T, typename U>
        struct Cast<T, U,LD::Enable_If_T<(LD::Detail::IsBoolean<T>::value && LD::Detail::IsBoolean<U>::value) && (LD::NumericSize<T>::value == LD::NumericSize<U>::value)>>
        {
            typedef T type;
        };


        template<typename T, typename U>
        struct Cast<T, U,LD::Enable_If_T<(LD::Detail::IsBoolean<T>::value && LD::Detail::IsBoolean<U>::value) && (LD::NumericSize<T>::value > LD::NumericSize<U>::value)>>
        {
            typedef T type;
        };


        template<typename T, typename U>
        struct Cast<T, U,LD::Enable_If_T<(LD::Detail::IsBoolean<T>::value && LD::Detail::IsBoolean<U>::value) && (LD::NumericSize<T>::value < LD::NumericSize<U>::value)>>
        {
            typedef U type;
        };

        template<typename T, typename U>
        struct Cast<T, U,LD::Enable_If_T<LD::Detail::IsBoolean<T>::value && LD::Detail::IsFloatingPoint<U>::value>>
        {
            typedef U type;
        };


        template<typename T, typename U>
        struct Cast<T, U,LD::Enable_If_T<LD::Detail::IsBoolean<T>::value && LD::Detail::IsUnsignedInteger<U>::value>>
        {
            typedef U type;
        };

        template<typename T, typename U>
        struct Cast<T, U,LD::Enable_If_T<LD::Detail::IsBoolean<T>::value && LD::Detail::IsInteger<U>::value>>
        {
            typedef U type;
        };


        template<typename T, typename U>
        struct Cast<T, U,LD::Enable_If_T<LD::Detail::IsBoolean<T>::value && LD::Detail::IsCharacter<U>::value>>
        {
            typedef U type;
        };

        template<typename T, typename U>
        struct Cast<T, U,LD::Enable_If_T<LD::Detail::IsBoolean<T>::value && LD::Detail::IsBoolean<U>::value>>
        {
            typedef U type;
        };

    }



    template<typename T, typename U>
    using Cast = typename LD::Detail::Cast<T, U>::type;




    template <class _Pred>
    struct LazyNot : LD::Detail::IntegralConstant<bool, !_Pred::type::value> {};


    template <bool _List, class ..._Preds>
    struct __lazy_or_impl;

    template <class ..._Preds>
    struct __lazy_or_impl<true, _Preds...> : LD::TrueType {};

    template <>
    struct __lazy_or_impl<false> : LD::FalseType {};

    template <class _Hp, class ..._Tp>
    struct __lazy_or_impl<false, _Hp, _Tp...>
            : __lazy_or_impl<_Hp::type::value, _Tp...> {};

    template <class _P1, class ..._Pr>
    struct LazyOr : __lazy_or_impl<_P1::type::value, _Pr...> {};
}




#if !defined(_LIBCPP_CXX03_LANG) && (!__has_feature(is_constructible) || \
defined(_LIBCPP_TESTING_FALLBACK_IS_CONSTRUCTIBLE))

namespace LD
{
    template <class _Tp, class... _Args>
    struct __libcpp_is_constructible;

    template <class _To, class _From>
    struct __is_invalid_base_to_derived_cast
    {
        static_assert(LD::Detail::IsReference<_To>::value, "Wrong specialization");
        using _RawFrom = __uncvref_t<_From>;
        using _RawTo = __uncvref_t<_To>;
        static const bool value = __lazy_and<
                LD::LazyNot<LD::Detail::IsSame<_RawFrom, _RawTo>>,
                LD::Detail::IsBaseOf<_RawFrom, _RawTo>,
                LD::LazyNot<__libcpp_is_constructible<_RawTo, _From>>
        >::value;
    };

    template <class _To, class _From>
    struct __is_invalid_lvalue_to_rvalue_cast : false_type {
        static_assert(is_reference<_To>::value, "Wrong specialization");
    };

    template <class _ToRef, class _FromRef>
    struct __is_invalid_lvalue_to_rvalue_cast<_ToRef&&, _FromRef&> {
        using _RawFrom = __uncvref_t<_FromRef>;
        using _RawTo = __uncvref_t<_ToRef>;
        static const bool value = __lazy_and<
                                  LD::LazyNot<is_function<_RawTo>>,
        LD::LazyOr<
                LD::Detail::IsSame<_RawFrom, _RawTo>,
                LD::Detail::IsBaseOf<_RawTo, _RawFrom>>
        >::value;
    };

    struct __is_constructible_helper
    {
        template <class _To>
        static void __eat(_To);

        // This overload is needed to work around a Clang bug that disallows
        // static_cast<T&&>(e) for non-reference-compatible types.
        // Example: static_cast<int&&>(declval<double>());
        // NOTE: The static_cast implementation below is required to support
        //  classes with explicit conversion operators.
        template <class _To, class _From,
                class = decltype(__eat<_To>(LD::Declval<_From>()))>
        static LD::TrueType __test_cast(int);

        template <class _To, class _From,
                class = decltype(static_cast<_To>(LD::Declval<_From>()))>
        static LD::Detail::IntegralConstant<bool,
                !__is_invalid_base_to_derived_cast<_To, _From>::value &&
                !__is_invalid_lvalue_to_rvalue_cast<_To, _From>::value
        > __test_cast(long);

        template <class, class>
        static LD::FalseType __test_cast(...);

        template <class _Tp, class ..._Args,
                class = decltype(_Tp(_VSTD::declval<_Args>()...))>
        static LD::TrueType __test_nary(int);
        template <class _Tp, class...>
        static LD::FalseType __test_nary(...);

        template <class _Tp, class _A0, class = decltype(::new _Tp(LD::Declval<_A0>()))>
        static IsDestructible<_Tp> __test_unary(int);


        template <class, class>
        static LD::FalseType __test_unary(...);
    };

    template <class _Tp, bool = is_void<_Tp>::value>
    struct __is_default_constructible
            : decltype(__is_constructible_helper::__test_nary<_Tp>(0))
    {};

    template <class _Tp>
    struct __is_default_constructible<_Tp, true> : LD::FalseType {};

    template <class _Tp>
    struct __is_default_constructible<_Tp[], false> : LD::FalseType {};

    template <class _Tp, size_t _Nx>
    struct __is_default_constructible<_Tp[_Nx], false>
            : __is_default_constructible<typename LD::RemoveAllExtents<_Tp>::type>  {};

    template <class _Tp, class... _Args>
    struct __libcpp_is_constructible
    {
        static_assert(sizeof...(_Args) > 1, "Wrong specialization");
        typedef decltype(__is_constructible_helper::__test_nary<_Tp, _Args...>(0))
                type;
    };

    template <class _Tp>
    struct __libcpp_is_constructible<_Tp> : __is_default_constructible<_Tp> {};

    template <class _Tp, class _A0>
    struct __libcpp_is_constructible<_Tp, _A0>
            : public decltype(__is_constructible_helper::__test_unary<_Tp, _A0>(0))
    {};

    template <class _Tp, class _A0>
    struct __libcpp_is_constructible<_Tp&, _A0>
            : public decltype(__is_constructible_helper::
            __test_cast<_Tp&, _A0>(0))
    {};


    template <class _Tp, class _A0>
    struct __libcpp_is_constructible<_Tp&&, _A0>
            : public decltype(__is_constructible_helper::
            __test_cast<_Tp&&, _A0>(0))
    {};

    template <class _Tp, class ..._Args>
    struct  IsConstructible
            : public LD::Detail::IntegralConstant<bool, __is_constructible(_Tp, _Args...)>
    {};
}



#endif






namespace LD
{
    template <class _Tp>
    struct  IsCopyConstructible
            : public LD::IsConstructible<_Tp,
                    typename LD::Detail::AddLValueReference<typename LD::Detail::AddConst<_Tp>::type>::type> {};
}



namespace LD
{





    namespace Detail
    {

        struct nonesuch {
            ~nonesuch() = delete;
            nonesuch(nonesuch const&) = delete;
            void operator=(nonesuch const&) = delete;
        };
        template <class Default, class AlwaysVoid,
                template<class...> class Op, class... Args>
        struct Detector {
            using value = LD::FalseType;
            using type = Default;
        };

        template <class Default, template<class...> class Op, class... Args>
        struct Detector<Default, LD::Void_T<Op<Args...>>, Op, Args...> {
            // Note that std::void_t is a C++17 feature
            using value = LD::TrueType;
            using type = Op<Args...>;
        };

    } // namespace detail

    template <template<class...> class Op, class... Args>
    using Is_Detected = typename Detail::Detector<Detail::nonesuch, void, Op, Args...>::value;

    template <template<class...> class Op, class... Args>
    using Detected_T = typename Detail::Detector<Detail::nonesuch, void, Op, Args...>::type;

    template <class Default, template<class...> class Op, class... Args>
    using Detected_Or = Detail::Detector<Default, void, Op, Args...>;


    template <class To, template<class...> class Op, class... Args>
    using Is_Detected_Convertible = LD::Detail::IsConvertible<Detected_T<Op, Args...>, To>;

    template <class Expected, template<class...> class Op, class... Args>
    using Is_Detected_Exact = LD::Detail::IsSame<Expected, Detected_T<Op, Args...>>;


    template <template <class...> class Op, class... Args>
    constexpr bool Exists = LD::Is_Detected<Op, Args...>::value;

    template <template <class...> class Op, class... Args>
    using CanCompile = LD::Is_Detected<Op, Args...>;

    template <class To, template <class...> class Op, class... Args>
    constexpr bool Coverts_To = LD::Is_Detected_Convertible<To, Op, Args...>::value;

    template <class Exact, template <class...> class Op, class... Args>
    constexpr bool Identical_To = LD::Is_Detected_Exact<Exact, Op, Args...>::value;


    namespace adl {
        using LD::Swap;

        template <class T, class U=T>
        using swap_with = decltype(Swap(LD::Declval<T>(), LD::Declval<U>()));
    };




    template<class...> struct Conjunction : LD::TrueType { };
    template<class B1> struct Conjunction<B1> : B1 { };
    template<class B1, class... Bn>
    struct Conjunction<B1, Bn...>
            : LD::Detail::Conditional_T<bool(B1::value), Conjunction<Bn...>, B1> {};



    template<class...> struct Disjunction : LD::FalseType { };
    template<class B1> struct Disjunction<B1> : B1 { };
    template<class B1, class... Bn>
    struct Disjunction<B1, Bn...>
            : LD::Detail::Conditional_T<bool(B1::value), B1, Disjunction<Bn...>>  { };

    template<class B>
    struct Negation : LD::Detail::IntegralConstant<bool, !bool(B::value)> { };


    template <typename ResultType, typename CheckType, template <typename> class ... Values>
    using Requires = LD::Enable_If_T<Conjunction<Values<CheckType>...>::value, ResultType>;

    template <typename ResultType, typename CheckType, template <typename> class ... Values>
    using FallsBack = LD::Enable_If_T<Conjunction<Negation<Values<CheckType>>...>::value, ResultType>;


    template <bool... Bs>
    constexpr bool Require = LD::Conjunction<LD::Detail::IntegralConstant<bool,Bs>...>::value;


    template <bool... Bs>
    constexpr bool FallBack = LD::Conjunction<LD::Negation<LD::Detail::IntegralConstant<bool,Bs>>...>::value;

    template <bool... Bs>
    constexpr bool Negate = not LD::Require<Bs...>;

    template <bool... Bs>
    constexpr bool Either = LD::Disjunction<LD::Detail::IntegralConstant<bool,Bs>...>::value;

    template <bool... Bs>
    constexpr bool Disallow = not LD::Require<Bs...>;


    namespace Ops
    {
        template <class T> using PrefixIncrement = decltype(++LD::Declval<T>());
        template <class T> using PostfixIncrement = decltype(LD::Declval<T>()++);
        template<typename T>
        using PrefixDecrement = decltype(--LD::Declval<T>());
        template<typename T>
        using PostFixDecrement = decltype(LD::Declval<T>()--);
        template <class T> using Dereference = decltype(*LD::Declval<T>());

        template <class T, class Index>
        using Subscript = decltype(LD::Declval<T>()[LD::Declval<Index>()]);


        template <class T, class U>
        using Addition = decltype(LD::Declval<T>()+LD::Declval<U>());

        template <class T, class U>
        using AssignableAddition = decltype((LD::Declval<T&>()+=LD::Declval<U>()));

        template <class T, class U>
        using Subtraction = decltype(LD::Declval<T>()-LD::Declval<U>());

        template <class T, class U>
        using AssignableSubtraction = decltype(LD::Declval<T&>()-=LD::Declval<U>());

        template <class T, class U>
        using Multiplication = decltype(LD::Declval<T>()*LD::Declval<U>());

        template <class T, class U>
        using AssignableMultiplication = decltype(LD::Declval<T&>()*=LD::Declval<U>());

        template <class T, class U>
        using Division = decltype(LD::Declval<T>()/LD::Declval<U>());

        template <class T, class U>
        using AssignableDivision = decltype(LD::Declval<T&>()/=LD::Declval<U>());

        //decltype(std::declval<T &>() << std::declval<U const&>())
        template<typename T, typename U>
        using QuickCheck = decltype(LD::Declval<LD::Decay_T <T> &>() << LD::Declval<LD::Decay_T <U> const&>());

        template <class T, class U>
        using RightShift = decltype(LD::Declval<T>()<<LD::Declval<U>());

        template <class T, class U>
        using LessThan = decltype(LD::Declval<T>() < LD::Declval<U>());

        template <class T, class U = T>
        using LessEqualThan = decltype(LD::Declval<T>() <= LD::Declval<U>());

        template <class T, class U = T>
        using GreaterThan = decltype(LD::Declval<T>() > LD::Declval<U>());

        template <class T, class U = T>
        using GreaterEqualThan = decltype(LD::Declval<T>() >= LD::Declval<U>());

        template <class T, class U = T>
        using Equality = decltype(LD::Declval<T>() == LD::Declval<U>());

        template <class T, class U = T>
        using NotEqual = decltype(LD::Declval<T>() != LD::Declval<U>());

        //template <class T ,typename ... Args>
        //using CalleableOperator = decltype(LD::Declval<T>()(LD::Declval<Args>()...));
        template <class F , typename T ,typename ... Args>
        using CalleableOperator =LD::Detail::IsConvertible<decltype(LD::Declval<F>()(LD::Declval<Args>()...)  ), T>;
        template <class T , typename U>
        using FunctionalAssignmentOperator = decltype(LD::Declval<U>() =  LD::Declval<T>()());


        template<typename F, typename ... Args>
        using CallableFunction = decltype(LD::Declval<F>()(LD::Declval<Args>()...));



    }



    template<typename T, typename U>
    constexpr bool HasLessThan = LD::Exists<LD::Ops::LessThan,T,U>;


    template<typename T, typename U>
    constexpr bool HasGreaterThan = LD::Exists<LD::Ops::GreaterThan,T,U>;

    template<typename T, typename U>
    constexpr bool HasEquality = LD::Exists<LD::Ops::Equality,T,U>;

    template<typename T, typename U>
    constexpr bool HasNotEqual = LD::Exists<LD::Ops::NotEqual,T,U>;

    template<typename F, typename T>
    struct ConvertiblyCallable;




    template<typename F,typename T, typename ... Args>
    struct ConvertiblyCallable<F,T(Args...)>
    {
        static constexpr bool Value()
        {
            return LD::Coverts_To<T,LD::Ops::CallableFunction, F,Args...>;
        }


        constexpr  bool operator()() const { return  LD::Coverts_To<T,LD::Ops::CallableFunction, F,Args...>; }

        constexpr operator const bool () const
        {
            return LD::Coverts_To<T,LD::Ops::CallableFunction, F,Args...>;
        }
    };




    template<typename F, typename ... Args>
    struct ConvertiblyCallable<F,void(Args...)>
    {
        static constexpr bool Value()
        {
            return LD::Exists<LD::Ops::CallableFunction, F,Args...>;
        }


        constexpr  bool operator()() const { return  LD::Exists<LD::Ops::CallableFunction, F,Args...>; }

        constexpr operator const bool () const
        {
            return LD::Exists<LD::Ops::CallableFunction, F,Args...>;
        }
    };



    //template<typename F, typename T>
    //struct AreConvertiblyCallable;

    template<typename F, typename ... Args>
    using CallableType = decltype(LD::Declval<F>()(LD::Declval<Args>()...)  );


    template<typename ... F, typename T, typename ... Args>
    struct ConvertiblyCallable<LD::VariadicPack<F...>, T(Args...)>
    {
        static constexpr bool Value()
        {

            return ((LD::Coverts_To<T,LD::Ops::CallableFunction, F,Args...>) && ... );
        }

        constexpr  bool operator()() const
        {
            return ((LD::Coverts_To<T,LD::Ops::CallableFunction, F,Args...>) && ... );

        }

        constexpr operator const bool () const
        {
            return ((LD::Coverts_To<T,LD::Ops::CallableFunction, F,Args...>) && ... );
        }
    };

    template<typename ... F, typename ... Args>
    struct ConvertiblyCallable<LD::VariadicPack<F...>, void(Args...)>
    {
        static constexpr bool Value()
        {

            return ((LD::Exists<LD::Ops::CallableFunction, F,Args...>) && ... );
        }

        constexpr  bool operator()() const
        {
            return ((LD::Exists<LD::Ops::CallableFunction, F,Args...>) && ... );

        }

        constexpr operator const bool () const
        {
            return ((LD::Exists<LD::Ops::CallableFunction, F,Args...>) && ... );
        }
    };


    template<typename F, typename T>
    struct IdenticallyCallable;


    template<typename F,typename T, typename ... Args>
    struct IdenticallyCallable<F,T(Args...)>
    {
        static constexpr bool Value()
        {
            return LD::Identical_To<T,LD::Ops::CallableFunction, F,Args...>;
        }


        constexpr bool operator()() const { return  LD::Identical_To<T,LD::Ops::CallableFunction, F,Args...>; }

        operator auto () const
        {
            return LD::Identical_To<T,LD::Ops::CallableFunction, F,Args...>;
        }
    };


    namespace Detail
    {
        template <typename P0>
        struct ContainsFloatingPoint;

        template <typename... T0s>
        struct ContainsFloatingPoint<VariadicPack<T0s...>>
                : LD::Detail::IntegralConstant<bool,(LD::Detail::IsFloatingPoint<T0s>::value || ...)>
        {
        };
    }

    template<typename ... Args>
    constexpr bool ContainsFloatingPoint = LD::Detail::ContainsFloatingPoint<LD::VariadicPack<Args...>>::value;

    template<typename F,typename T>
    struct Callable;


    template<typename F,typename T, typename ... Args>
    struct Callable<F,T(Args...)>
    {
        static const bool value = LD::Detail::IsConvertible<decltype(LD::Declval<F>()(LD::Declval<Args>()...)  ), T>::value;
    };


    template<typename F, typename T, typename ... Args>
    struct Calling
    {
        static const bool value = LD::Detail::IsConvertible<decltype(LD::Declval<F>()(LD::Declval<Args>()...)  ), T>::value;
    };


    template<typename U>
    using CanDestruct = decltype(LD::Declval<U>().~U());


    class NL
    {

    };

    class NewLine
    {

    };

    class Tab
    {

    };

    class Space
    {

    };
    namespace Concept
    {
        template <class T>
        constexpr bool CopyConstructible = LD::IsCopyConstructible<T>::value;

        template <class T>
        constexpr bool CopyAssignable = LD::IsCopyAssignable<T>::value;

        template <class T>
        constexpr bool Destructible = LD::IsDestructible<T>::value;

        template <class T>
        constexpr bool Pointer = LD::Detail::IsPointer<T>::value;


        template<typename F , typename T ,typename ... Args>
        constexpr bool Calleable = LD::Ops::CalleableOperator<F, T, Args...>::value;

        template<typename F, typename U>
        constexpr bool FunctionalAssignment = LD::Exists<LD::Ops::FunctionalAssignmentOperator, F,U>;


        template <class T, class U>
        constexpr bool SwappableWith = Exists<adl::swap_with, T, U>;

        template <class T, class U>
        constexpr bool Addable = Exists<LD::Ops::Addition, T, U>;

        template <class T, class U>
        constexpr bool AssignablyAddiditive = Exists<LD::Ops::AssignableAddition, T, U>;

        template <class T, class U>
        constexpr bool Insertable = Exists<LD::Ops::QuickCheck , T, U>;

        template <class T, class U>
        constexpr bool Extractable = Exists<LD::Ops::RightShift, T, U>;

        //subtractable
        template <class T, class U>
        constexpr bool Subtractable = Exists<LD::Ops::Subtraction, T, U>;

        template <class T, class U>
        constexpr bool AssignablySubtractable = Exists<LD::Ops::AssignableSubtraction , T, U>;

        template <class T, class U>
        constexpr bool Multipliable = Exists<LD::Ops::Multiplication, T, U>;

        template <class T, class U>
        constexpr bool AssignablyMultipliable = Exists<LD::Ops::AssignableMultiplication , T, U>;

        template <class T, class U>
        constexpr bool Divisible = Exists<LD::Ops::Division, T, U>;

        template <class T, class U>
        constexpr bool AssignablyDivisible = Exists<LD::Ops::AssignableDivision , T, U>;


        template <class T>
        constexpr bool Swappable = SwappableWith<T&, T&>;


        template<typename T>
        constexpr bool PrefixIncrementable = LD::Exists<LD::Ops::PrefixIncrement,T>;

        template<typename T>
        constexpr bool PrefixDecrementable = LD::Exists<LD::Ops::PrefixDecrement,T>;

        template<typename T>
        constexpr bool PostfixIncrementable = LD::Exists<LD::Ops::PostfixIncrement,T>;

        template<typename T>
        constexpr bool PostfixDecrementable = LD::Exists<LD::Ops::PostFixDecrement ,T>;

        template<typename T>
        constexpr bool Deferenceable = LD::Exists<LD::Ops::Dereference,T>;




        template <class T>
        constexpr bool Iterable = LD::Either<LD::Concept::Pointer<T>, LD::Require<
                LD::Concept::CopyConstructible<T>,
                LD::Concept::CopyAssignable<T>,
                LD::Concept::Destructible<T>,
                LD::Concept::Swappable<T>,
                LD::Concept::PrefixIncrementable<T>,
                LD::Concept::Deferenceable<T>
        >>;




        template<typename T, typename U = T>
        constexpr bool Arithmetical = LD::Require<
                LD::Concept::Addable<T, U>,
                LD::Concept::Subtractable<T, U>,
                LD::Concept::Multipliable<T, U>,
                LD::Concept::Divisible<T, U>
        >;

        template<typename T, typename U>
        constexpr bool Streamable = LD::Require<
                LD::Concept::Insertable<T, U>,
                LD::Concept::Extractable<T, U>
        >;

        template<typename F, typename T, typename ... Args>
        constexpr bool Executable = LD::Require<
                LD::Concept::Calleable<F, Args...>
        >;
    }



    template<typename T>
    constexpr bool IsInteger = LD::Detail::IsInteger<T>::value;

    template<typename T>
    constexpr bool IsUnsignedInteger = LD::Detail::IsUnsignedInteger<T>::value;


    template<typename T>
    constexpr bool IsFloatingPoint = LD::Detail::IsFloatingPoint<T>::value;

    template<typename T>
    constexpr bool IsCharacter = LD::Detail::IsCharacter<T>::value;



   // template<typename T>
    //constexpr bool IsClass = LD::Detail::IsClassType<T>::value;

    //template<typename T>
    //constexpr bool IsPrimitive =
            //LD::Require<
                    //!LD::IsClass<T>,
                    //LD::Either<LD::IsInteger<T>,LD::IsUnsignedInteger<T>,LD::IsFloatingPoint<T>,LD::IsCharacter<T>>
           // >;

    template<typename T>
    constexpr bool IsPrimitive = !LD::IsClass<T>;




    template<typename T, typename U>
    constexpr bool IsSame = LD::Detail::IsSame<T,U>::value;
}


namespace foonathan {
    namespace string_id {
        namespace detail
        {
            using hash_type = LD::UInteger;

            constexpr hash_type fnv_basis = 14695981039346656037ull;
            constexpr hash_type fnv_prime = 109951162821ull;

            // FNV-1a 64 bit hash
            constexpr hash_type sid_hash(const char *str, hash_type hash = fnv_basis) noexcept
            {
                return *str ? sid_hash(str + 1, (hash ^ *str) * fnv_prime) : hash;
            }
        }
    }
} // foonathan::string_id::detail
#endif
