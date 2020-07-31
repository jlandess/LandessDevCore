//
// Created by James Landess on 2/6/20.
//

#ifndef LANDESSDEVCORE_DETECTION_HPP
#define LANDESSDEVCORE_DETECTION_HPP

#include "IsSame.hpp"
#include "IsVoid.hpp"
#include "IsClassType.hpp"
#include "IsConvertible.hpp"
#include "Declval.hpp"
#include "Conditional.hpp"
#include "EnableIf.hpp"
#include "Decay.hpp"
#include "IsSwappable.hpp"
#include "VariadicPack.hpp"
#include "IsPointer.hpp"
#include "IsDestructible.hpp"
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


    //template <typename ResultType, typename CheckType, template <typename> class ... Values>
    //using Requires = LD::Enable_If_T<Conjunction<Values<CheckType>...>::value, ResultType>;

    template <typename ResultType, typename CheckType, template <typename> class ... Values>
    using FallsBack = LD::Enable_If_T<Conjunction<Negation<Values<CheckType>>...>::value, ResultType>;


    template <bool... Bs>
    constexpr bool Require = LD::Conjunction<LD::Detail::IntegralConstant<bool,Bs>...>::value;




    template<bool ... Bs>
    using Requires = LD::Enable_If_T<LD::Require<Bs...>>;

    template<typename T, bool ... Bs>
    using TypedRequires = LD::Enable_If_T<LD::Require<Bs...>,T>;

    //template<bool ... Bs>
    //using Requires = LD::Enable_If_T<LD::Require<Bs...>,void>;
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
        using QuickCheck = decltype(LD::Declval<LD::Detail::Decay_T <T> &>() << LD::Declval<LD::Detail::Decay_T <U> const&>());

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
        static constexpr bool Value() noexcept
        {
            return LD::Coverts_To<T,LD::Ops::CallableFunction, F,Args...>;
        }


        constexpr  bool operator()() const noexcept { return  LD::Coverts_To<T,LD::Ops::CallableFunction, F,Args...>; }

        constexpr operator const bool () const noexcept
        {
            return LD::Coverts_To<T,LD::Ops::CallableFunction, F,Args...>;
        }
    };




    template<typename F, typename ... Args>
    struct ConvertiblyCallable<F,void(Args...)>
    {
        static constexpr bool Value() noexcept
        {
            return LD::Exists<LD::Ops::CallableFunction, F,Args...>;
        }


        constexpr  bool operator()() const noexcept { return  LD::Exists<LD::Ops::CallableFunction, F,Args...>; }

        constexpr operator const bool () const noexcept
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

    template<typename T>
    constexpr bool Indexable = LD::Exists<LD::Ops::Subscript,T,LD::UInteger>;


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

    template<typename T>
    constexpr bool ContinuousIterable = LD::Either<
            LD::Concept::Pointer<T>,
            LD::Require<
                    LD::Concept::CopyConstructible<T>,
                    LD::Concept::CopyAssignable<T>,
                    LD::Concept::Destructible<T>,
                    LD::Concept::Swappable<T>,
                    LD::Concept::PrefixIncrementable<T>,
                    LD::Concept::Deferenceable<T>,
                    LD::Concept::Indexable<T>
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

    namespace Detail
    {
        template<typename T>
        using InternalDefaultConstructibleTest = decltype(LD::Declval<T>());

        template<typename T>
        struct IsDefaultConstructible: public LD::Detail::IntegralConstant<bool,LD::Exists<InternalDefaultConstructibleTest,T>>
        {



        };
    }

    template<typename T>
    constexpr const bool IsDefaultConstructible = LD::Detail::IsDefaultConstructible<T>::value;
}


#endif //LANDESSDEVCORE_DETECTION_HPP
