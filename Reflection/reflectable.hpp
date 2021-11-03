#ifndef REFLECTABLE_REFLECTABLE_H_INCLUDED
#define REFLECTABLE_REFLECTABLE_H_INCLUDED

#include "tagged_tuple.hpp"
#include "TypeTraits/CompileTimeControlFlow.hpp"
#include "Primitives/General/typestring.hpp"
#include "TypeTraits/SearchableVariadicPack.hpp"
#include "ConstexprFor.hpp"
#include "TypeTraits/IsMemberPointer.hpp"
#include "Primitives/General/mapboxvariant.hpp"
#include "TypeTraits/Iterable.h"
#include "Primitives/General/Pair.h"
#include "Primitives/General/Span.hpp"
#include "TypeTraits/TypeList.hpp"
#include "TypeTraits/IsImmutable.h"
#include "Primitives/General/Context.h"
#include "Primitives/General/ContextualVariant.h"
#include "Primitives/General/StringView.hpp"
#include "Primitives/General/Immutable.hpp"

namespace LD
{
    class AccessReadOnly
    {

    };

    class AccessWriteOnly
    {

    };


    class AccessReadWrite
    {

    };

}
namespace LD
{
#define RecflectableStringification( x ) #x
    struct ReflectableWarrant
    {

        constexpr  static bool Value = {true};

        constexpr const bool GetWarrant() const
        {
            return Value;
        }
    };


    namespace Detail
    {
        template <class T>
        using ReflectableWarrantCheck = decltype(LD::Declval<T>().GetReflectionWarrant());



        template <class T>
        class HasReflectableWarrant
        {
            template<class U, class = typename std::enable_if<!LD::IsMemberPointer<decltype(&U::ReflectionWarrant)>>::type>
            static constexpr LD::TrueType check(int);
            template <class>
            static constexpr LD::FalseType check(...);
        public:
            static constexpr bool value = decltype(check<T>(0))::value;
        };
    }







    template<typename T>
    constexpr const bool IsReflectable = LD::Exists<LD::Detail::ReflectableWarrantCheck,T>;

    namespace Detail
    {
        template<typename T>
        using IsReflectable = LD::Detail::IntegralConstant<bool,LD::IsReflectable<T>>;
        template<typename T>
        using IsNotReflectable = LD::Detail::IntegralConstant<bool,LD::Negate<LD::IsReflectable<T>>>;
    }

    namespace Detail
    {
        template<typename ... A>
        struct IsNamedPack
        {

            using SearchablePack = LD::SearchableVariadicPack<A...>;



            constexpr static const bool IsNamed()
            {
                if constexpr (sizeof...(A) > 0 && sizeof...(A)%2 == 0)//make sure the number of arguements are even and a positive number
                {
                    bool isReady = true;

                    for_constexpr<for_bounds<0, sizeof...(A)>>([&](auto I)
                    {
                        if constexpr (I %2 == 0)
                        {

                            //make sure all of the even indices are typestrings

                           isReady = isReady && LD::IsTypeString< LD::Extract<I,SearchablePack> >;
                        }
                    });

                    return isReady;
                }


                return false;
            }

        };
    }


    template<typename ... A>
    constexpr bool IsNamedPack = LD::Detail::IsNamedPack<A...>::IsNamed();


    template<typename T, class = void>
    class Reflectable;

    template <typename T,typename ...Args>
    class Reflectable<T(Args...),LD::Enable_If_T<
            LD::Require<
                    LD::IsTypeString<T>
                    >>> : public LD::tagged_tuple<decltype("ClassName"_ts),const char *,Args...>
    {
    public:
        using ValueTypeList = typename LD::tagged_tuple<decltype("ClassName"_ts),const char *,Args...>::ValueTypeList;
        using KeyTypeList = typename LD::tagged_tuple<decltype("ClassName"_ts),const char *,Args...>::KeyTypeList;
        typedef  LD::SearchableVariadicPack<decltype("ClassName"_ts),const char *,Args...> SearchableReflectionList;

        constexpr static LD::UInteger NumberOfMembers = {(sizeof...(Args)/2)+1};

        constexpr static ReflectableWarrant ReflectionWarrant = {};


        constexpr static T GetClassNameTypeString()
        {
            return T{};
        }

        constexpr static const char * GetClassName()
        {
            return T::data();
        }
        inline constexpr Reflectable() noexcept
        {
            (*this)["ClassName"_ts] = T::data();
        }
        /*
        template <typename FuncT>
        void iterate(const FuncT& f)
        {
            for_each(*this, f);
        }
         */
        
        
        template<char ... Characters>
        constexpr auto & operator[](const LD::TypeString<Characters...> & characters)
        {
            return (*this).template Get<LD::TypeString<Characters...>>();
        }
        template<char ... Characters>
        constexpr const auto & operator[](const LD::TypeString<Characters...> & characters) const
        {
            return (*this).template Get<LD::TypeString<Characters...>>();
        }

        template<LD::UInteger Index>
        constexpr auto  operator[](const LD::CompileTimeIndex<Index> & compiletimeIndex) const
        {
            return (*this).template GetMemberName<Index>();
        }



        template<LD::UInteger Index>
        constexpr auto GetMemberName() const
        {

            using extracted =  LD::Extract<(2*Index), SearchableReflectionList>;

            //return extracted::data();

            return extracted{};
        }


        constexpr ReflectableWarrant GetReflectionWarrant() const
        {
            return ReflectionWarrant;
        }



    };




    namespace Detail
    {
        template<typename T, class = void>
        struct GetTypeString;


        template<typename T>
        struct GetTypeString<T,LD::Enable_If_T<LD::Require<LD::IsReflectable<T>>>>{

            static decltype(T::GetClassNameTypeString()) Value()
            {
                return T::GetClassNameTypeString();
            }


        };

        template<typename T>
        struct GetPrimitiveTypeString;


        template<>
        struct GetPrimitiveTypeString<bool>
        {
            static auto Value()
            {
                return "bool"_ts;
            }
        };


        template<>
        struct GetPrimitiveTypeString<char>{

            static auto Value()
            {
                return "char"_ts;
            }


        };

        template<>
        struct GetPrimitiveTypeString<short>{

            static auto Value()
            {
                return "short"_ts;
            }


        };
        template<>
        struct GetPrimitiveTypeString<int>{

            static auto Value()
            {
                return "int"_ts;
            }


        };

        template<>
        struct GetPrimitiveTypeString<long>{

            static auto Value()
            {
                return "long"_ts;
            }


        };

        template<>
        struct GetPrimitiveTypeString<float>{

            static auto Value()
            {
                return "float"_ts;
            }


        };

        template<>
        struct GetPrimitiveTypeString<double>{

            static auto Value()
            {
                return "double"_ts;
            }


        };

        template<>
        struct GetPrimitiveTypeString<unsigned  char>{

            static auto Value()
            {
                return "uchar"_ts;
            }


        };

        template<>
        struct GetPrimitiveTypeString<unsigned short>{

            static auto Value()
            {
                return "ushort"_ts;
            }


        };

        template<>
        struct GetPrimitiveTypeString<unsigned int>{

            static auto Value()
            {
                return "uint"_ts;
            }


        };

        template<>
        struct GetPrimitiveTypeString<unsigned long>{

            static auto Value()
            {
                return "ulong"_ts;
            }


        };

        template<typename T>
        struct GetTypeString<T,LD::Enable_If_T<LD::Require<LD::IsPrimitive<T>>>>{

            static auto Value()
            {
                return  LD::Detail::GetPrimitiveTypeString<T>::Value();
            }


        };

        template<typename T>
        struct GetTypeString<T,LD::Enable_If_T <LD::FallBack<LD::IsPrimitive<T>,LD::IsReflectable<T>>>>{


            static_assert(LD::Negate<LD::IsPrimitive<T>>,"You can only get a typestring for a primitive type or a reflective type");
            static_assert(LD::Negate<LD::IsReflectable<T>>,"You can only get a typestring for a primitive type or a reflective type");
        };




    }
}

namespace LD
{
    namespace CT
    {
        namespace Detail
        {
            template<typename T,typename = void>
            struct ReadOnlyTransform
            {
                using type = T;
            };

            template<typename T>
            struct ReadOnlyTransform<T,LD::Enable_If_T<LD::IsPrimitive<T>>>
            {
                using type = LD::ElementReference<T>;
            };

            template<typename T>
            struct ReadOnlyTransform<T,LD::Enable_If_T<
                    LD::Require<
                            LD::Concept::ContinuousIterable<LD::BeginIterator<T>>
                    >>>

            {
                using ListType = LD::Detail::Decay_T<T>;
                using IteratorType = LD::BeginIterator<ListType>;
                //using ValueType = LD::Detail::LDValueType<staticIterator>;
                using ValueType = LD::ValueType<IteratorType>;
                using type = LD::Span<ValueType> ;
            };


            template<typename T>
            struct ReadOnlyTransform<T,LD::Enable_If_T<
                    LD::Require<
                            LD::Concept::Iterable<LD::BeginIterator<T>>,
                            LD::Negate<LD::Concept::ContinuousIterable<LD::BeginIterator<T>>>
                    >>>
            {
                using ListType = LD::Detail::Decay_T<T>;
                using BeginIterator = LD::BeginIterator<ListType>;
                using EndIterator = LD::EndIterator<ListType>;
                using type = LD::Pair<BeginIterator ,EndIterator >;
            };

            ///----------------------------------- Replace Mutable Contiguous Arrays with Contiguous with back_inserters --------------
            //------------------------------------ Immutable Contiguous Arrays can still use LD::Span -------------------------------
            template<typename T,typename = void>
            struct WriteOnlyTransform
            {
                using type = T;
            };

            template<typename T>
            struct WriteOnlyTransform<T,LD::Enable_If_T<LD::IsPrimitive<T>>>
            {
                using type = LD::ElementReference<T>;
            };

            template<typename T>
            struct WriteOnlyTransform<T,LD::Enable_If_T<
                    LD::Require<
                            LD::Concept::ContinuousIterable<LD::BeginIterator<T>>,
                            LD::IsImmutable<T>
                    >>>
            {
                using ListType = LD::Detail::Decay_T<T>;
                using IteratorType = LD::BeginIterator<ListType>;
                //using ValueType = LD::Detail::LDValueType<staticIterator>;
                using ValueType = LD::ValueType<IteratorType>;
                using type = LD::Span<ValueType> ;
            };


            template<typename T, typename = void>
            struct ReflectiveTypeList;
            template<typename T>
            struct ReflectiveTypeList<T,LD::Enable_If_T<
                    LD::Require<LD::IsReflectable<T>
                    >>>
            {
                using AllMembers  = typename LD::tlist_erase_at<0,LD::CT::RebindList<typename T::ValueTypeList,LD::CT::TypeList>>::type;

                template<typename V>
                using IsAMember = LD::Detail::IntegralConstant<bool,LD::Negate<LD::IsReflectable<V>>>;

                template<typename V>
                using IsAReflector = LD::Detail::IntegralConstant<bool,LD::IsReflectable<V>>;


                using Members  = LD::CT::Filter<AllMembers,IsAMember>;


                using ReflectiveMembers = LD::CT::Filter<AllMembers,IsAReflector>;


                using DeCoupledReflectiveList = typename LD::CT::Detail::ReflectiveTypeList<ReflectiveMembers>::type;

                using ConcatedLists = LD::CT::Concatenate<Members,DeCoupledReflectiveList>;

                using type = LD::CT::DeDuplicateTypeList<ConcatedLists>;
            };
            template<typename ...A>
            struct ReflectiveTypeList<LD::CT::TypeList<A...>,LD::Enable_If_T<
                    LD::Require<(LD::IsReflectable<A> && ...)
                    >>>
            {
                using buffer = LD::CT::Concatenate<LD::CT::TypeList<>,LD::CT::TypeList<typename LD::CT::Detail::ReflectiveTypeList<A>::type...>>;
                using type =  LD::CT::Flatten<buffer> ;
            };
        }

        template<typename T>
        using ReflectiveTypeList = typename LD::CT::Detail::ReflectiveTypeList<T>::type;
        template<typename T>
        using ReadOnlyTransform = typename LD::CT::Detail::ReadOnlyTransform<T>::type;

        template<typename T>
        using WriteOnlyTransform = typename LD::CT::Detail::WriteOnlyTransform<T>::type;


        template<typename T>
        using ReadWriteTransform = void;



        namespace Detail
        {






            template<typename ReflectiveObject, typename AccessMode, typename = void>
            struct ReflectiveTransformation;




            template<typename ReflectiveObject>
            struct ReflectiveTransformation<ReflectiveObject,LD::AccessReadOnly,
                    LD::Enable_If_T<LD::Require<
                            LD::IsReflectable<ReflectiveObject>
                    >>>
            {
                using Members = LD::CT::ReflectiveTypeList<ReflectiveObject>;

                using list = LD::CT::Tranform<Members,LD::CT::ReadOnlyTransform>;

                using type =  LD::CT::RebindList<list ,LD::Variant> ;

            };

            template<typename ReflectiveObject>
            struct ReflectiveTransformation<ReflectiveObject,LD::AccessWriteOnly,
                    LD::Enable_If_T<LD::Require<
                            LD::IsReflectable<ReflectiveObject>
                    >>>
            {

                using Members = LD::CT::ReflectiveTypeList<ReflectiveObject>;

                using list = LD::CT::Tranform<Members,LD::CT::ReadOnlyTransform>;

                using type =  LD::CT::RebindList<list ,LD::Variant> ;
            };

            template<typename ReflectiveObject>
            struct ReflectiveTransformation<ReflectiveObject,LD::AccessReadWrite,
                    LD::Enable_If_T<LD::Require<
                            LD::IsReflectable<ReflectiveObject>
                    >>>
            {
                using Members = LD::CT::ReflectiveTypeList<ReflectiveObject>;

                using list = LD::CT::Tranform<Members,LD::CT::ReadOnlyTransform>;

                using type =  LD::CT::RebindList<list ,LD::Variant> ;
            };
        }

        template<typename T, typename AccessMode>
        using ReflectiveTransformation = typename LD::CT::Detail::ReflectiveTransformation<T,AccessMode>::type;
    }
}

/*
template<typename T, typename Key ,typename F, typename H, typename Q ,typename ... Args>
LD::Enable_If_T<
        LD::Require<
                LD::IsReflectable<T>,
                LD::IsTypeString<Key>,
                LD::Either<LD::IsSame<Q,LD::AccessReadOnly>,LD::IsSame<Q,LD::AccessWriteOnly>,LD::IsSame<Q,LD::AccessReadWrite>>,
                LD::ConvertiblyCallable<H,void(const LD::ContextualVariant<PrimitiveVariant(LD::StringView,Args...)> &)>::Value()
        >
        ,void> NoSQLTraversal(const Key & key,T && object, F && onClassName, H && onMember , const Q & accessMode ,Args && ... arguements)
{


    using Type = LD::Detail::Decay_T<T>;
    using ClassName = decltype(Type::GetClassNameTypeString());
    using Period = LD::TypeString<'.'>;
    constexpr LD::UInteger numberOfMembers = Type::NumberOfMembers;

    onClassName(LD::StringView {Key::data(),Key::size()},LD::StringView{ClassName::data(),ClassName::size()});
    //todo optimize with access mode for different access patterns
    LD::ContextualVariant<LD::Variant<LD::AccessReadWrite,LD::AccessWriteOnly,LD::AccessReadOnly>()> context;

    LD::For<1,numberOfMembers>([](auto I, T && object,F && onClass ,H && onMember,Args && ... arguements)
                               {
                                   using MemberType = LD::Detail::Decay_T<decltype(LD::Get<I>(*object))>;
                                   using MemberName = decltype(LD::Declval<T>()[LD::CompileTimeIndex<I>{}]);
                                   using MemberKey = LD::tycat<Key,Period ,MemberName>;
                                   auto & reference = object[MemberName{}];
                                   if constexpr (LD::IsPrimitive<MemberType> && LD::Negate<LD::IsPointer<MemberType>>)
                                   {

                                       LD::ElementReference<MemberType> memberReference = LD::ElementReference<MemberType>{reference};
                                       onMember(LD::MakeContext(LD::ElementReference<MemberType>{memberReference},
                                                                LD::StringView{MemberKey::data(),MemberKey::size()},
                                                                LD::Forward<Args>(arguements)...));

                                   }else if constexpr (LD::Detail::Spannable<T>::value)
                                   {
                                       LD::Span<MemberType> currentSpan{reference};
                                       //todo maybe use iterators for ranging instead? offer optional iterators for insertions
                                       onMember(LD::MakeContext(LD::Span<MemberType>{currentSpan},
                                                                LD::StringView{MemberKey::data(),MemberKey::size()},
                                                                LD::Forward<Args>(arguements)...));


                                   }else if constexpr(LD::IsReflectable<MemberType>)
                                   {

                                       return NoSQLTraversal(MemberKey{},reference,LD::Forward<T>(onClass),LD::Forward<H>(onMember),LD::Forward<Args>(arguements)...);
                                   }
                                   return true;
                               },LD::Forward<T>(object),LD::Forward<F>(onClassName),LD::Forward<H>(onMember),LD::Forward<Args>(arguements)...);
}
 */

namespace LD
{
    namespace CT
    {

        namespace Detail
        {

            template<typename ReflectiveMemberTypes,typename K, typename T, typename F, typename H, typename Q, typename ... Args>
            LD::Enable_If_T<
                    LD::Require<
                            LD::Either<LD::IsTypeString<K>,LD::IsImmutableString<K>>
            >
            ,bool> ReflectiveWalk(
                    const K & key,
                    T && object,
                    F && onClass,
                    H && onMember,
                    const Q & accessMode,
                    const LD::UInteger & level,
                    Args && ... arguements) noexcept
            {
                using Type = LD::Detail::Decay_T<T>;
                using ClassName = decltype(Type::GetClassNameTypeString());
                using Period = LD::TypeString<'.'>;
                constexpr LD::UInteger numberOfMembers = Type::NumberOfMembers;

                auto onClassContext = LD::MakeContext(
                        LD::StringView{K::data(),K::size()},
                        LD::StringView{ClassName::data(),ClassName::size()},
                        LD::Forward<Args>(arguements)...);
                const bool shouldContinue = onClass(onClassContext);

                //it's okay for members to not be missing, but if the core class related to the key is missing, then it's pointless
                //the core class will always be at level == 0
                if (!shouldContinue && level == 0)
                {
                    return false;
                }
                if (shouldContinue)
                {
                    LD::For<1,numberOfMembers>([](
                    auto I,
                    T && object,
                    F && onClass,
                    H && onMember,
                    const Q & mode,
                    const LD::UInteger & level,
                    const K & key,
                    Args && ... arguments)
                    {


                        using MemberType = LD::Detail::Decay_T<decltype(LD::Get<I>(object))>;

                        using MemberName = decltype(LD::Declval<T>()[LD::CompileTimeIndex<I>{}]);

                        //using MemberKey = LD::tycat<K,Period,MemberName>;

                        auto memberKey = key+Period{}+MemberName{};
                        if constexpr(LD::IsReflectable<MemberType>)
                        {
                            return ReflectiveWalk<ReflectiveMemberTypes>(
                            memberKey,
                            object[MemberName{}],
                            LD::Forward<F>(onClass),
                            LD::Forward<H>(onMember),
                            mode,
                            level+1,
                            LD::Forward<Args>(arguments)...);
                        }

                        if constexpr(LD::IsSame<Q,LD::AccessReadOnly>)
                        {

                            if constexpr(LD::Negate<LD::IsPointer<MemberType>>)
                            {

                                if constexpr(LD::IsPrimitive<MemberType>)

                                {

                                    LD::ElementReference<MemberType> reference = {};

                                    reference = LD::ElementReference<MemberType>{object[MemberName{}]};

                                    LD::ContextualVariant<ReflectiveMemberTypes(LD::StringView ,Args...)> memberContext;

                                    memberContext = LD::MakeContext(
                                            LD::ElementReference<MemberType>{reference},
                                            LD::StringView {memberKey.Data(),memberKey.Size()},
                                            LD::Forward<Args>(arguments)...);


                                    onMember(memberContext);
                                }

                            }

                        }else if constexpr(LD::IsSame<Q,LD::AccessWriteOnly>)
                        {

                            if constexpr(LD::Negate<LD::IsPointer<MemberType>>)

                            {

                                if constexpr(LD::IsPrimitive<MemberType>)

                                {

                                    LD::ElementReference<MemberType> reference = {};

                                    reference = LD::ElementReference<MemberType>{object[MemberName{}]};

                                    LD::ContextualVariant<ReflectiveMemberTypes(LD::StringView ,Args...)> memberContext;

                                    //LD::DebugTemplate<ReflectiveMemberTypes>{};

                                    memberContext = LD::MakeContext(
                                            LD::ElementReference<MemberType>{reference},
                                            LD::StringView {memberKey.Data(),memberKey.Size()},
                                            LD::Forward<Args>(arguments)...);


                                    onMember(memberContext);

                                }

                            }


                        }

                        return true;//control flow for compile time for loop

                        },LD::Forward<T>(object),LD::Forward<F>(onClass),LD::Forward<H>(onMember),accessMode,level,key,LD::Forward<Args>(arguements)...);
                }
                return true;
            }

        }
        //LD::ConvertiblyCallable<H,bool(const LD::ContextualVariant<LD::CT::RebindList<Types,LD::Variant>(LD::StringView,Args...)> &)>::Value()
        template<typename K,typename T, typename F, typename H, typename Q, typename ... Args,
                typename Types = LD::CT::ReflectiveTransformation<LD::Detail::Decay_T<T>,Q>
        >
        LD::Enable_If_T<LD::Require<

                LD::IsReflectable<T>,
                LD::Either<LD::IsTypeString<K>,LD::IsImmutableString<K>>,
                LD::ConvertiblyCallable<H,bool(const LD::ContextualVariant<LD::CT::RebindList<Types,LD::Variant>(LD::StringView,Args...)> &)>::Value(),
                LD::ConvertiblyCallable<F,bool(const LD::Context<LD::StringView,LD::StringView,Args...> &)>::Value(),
                LD::Either<LD::IsSame<Q,LD::AccessReadOnly>,LD::IsSame<Q,LD::AccessWriteOnly>,LD::IsSame<Q,LD::AccessReadWrite>>
        >
                ,bool> ReflectiveWalk(
                const K & key,
                T && object,
                F && onClass,
                H && onMember,
                const Q & accessMode,
                Args && ... arguements) noexcept
        {
            return LD::CT::Detail::ReflectiveWalk<Types>(key,
                                                         LD::Forward<T>(object),
                                                         LD::Forward<F>(onClass),
                                                         LD::Forward<H>(onMember),
                                                         accessMode,
                                                         LD::UInteger{0},
                                                         LD::Forward<Args>(arguements)...);

        }
    }
}

namespace LD
{
    namespace Detail
    {
        template<typename T, typename Constant>
        struct AppendConstantToTypeString;

        template<char ... A, char ... ContantCharacters>
        struct AppendConstantToTypeString<LD::TypeString<A...>,LD::TypeString<ContantCharacters...>>
        {
            using type = LD::tycat<LD::TypeString<ContantCharacters...>,LD::TypeString<'.'>,LD::TypeString<A...>>;
        };
    }
    namespace CT
    {
        template<typename T, typename H>
        using AppendConstantToTypeString = typename LD::Detail::AppendConstantToTypeString<T,H>::type;
    }
}


namespace LD
{
    namespace Detail
    {

        template<typename Key,typename T, typename = void>
        struct GenerateNamedReflectiveTypeStructure;
        template<typename Key,typename T>
        struct GenerateNamedReflectiveTypeStructure<Key,T,LD::Enable_If_T<
                LD::Require<LD::IsReflectable<T>
                >>>
        {
            using AllKeys  = typename LD::tlist_erase_at<0,LD::CT::RebindList<typename T::KeyTypeList,LD::CT::TypeList>>::type;

            using AllMembers  = typename LD::tlist_erase_at<0,LD::CT::RebindList<typename T::ValueTypeList,LD::CT::TypeList>>::type;

            using ConcatedKeys = LD::CT::TransformWithArgs<AllKeys,LD::CT::AppendConstantToTypeString,Key>;

            using InterwovenMembers = LD::CT::InterleaveTypeLists<ConcatedKeys ,AllMembers >;

            template<typename V>
            using IsAReflectorFuzed = LD::Detail::IntegralConstant<bool,LD::IsReflectable<LD::CT::TypeAtIndex<1,V>>>;

            using ReflectiveFuzedMembers = LD::CT::Filter<InterwovenMembers ,IsAReflectorFuzed>;

            using DeCoupledReflectiveList = typename GenerateNamedReflectiveTypeStructure<Key,ReflectiveFuzedMembers>::type;

            using ConcatedLists = LD::CT::Concatenate<InterwovenMembers ,DeCoupledReflectiveList>;
            using type = LD::CT::Flatten<ConcatedLists>;
        };

        template<typename Key,typename ...A, typename ... B>
        struct GenerateNamedReflectiveTypeStructure<Key,LD::CT::TypeList<LD::CT::TypeList<A,B>...>,LD::Enable_If_T<
                LD::Require<(LD::IsReflectable<B> && ...)
                >>>
        {
            using Period = LD::TypeString<'.'>;
            //using buffer = LD::CT::Concatenate<LD::CT::TypeList<>,LD::CT::TypeList<LD::CT::TypeList<LD::CT::TypeList<A,typename ReflectiveKeyList<A,B>::type>...>>   >;
            using buffer = LD::CT::TypeList<typename GenerateNamedReflectiveTypeStructure<A,B>::type...>;
            using type =  buffer ;
        };
    }

    namespace CT
    {

        template<typename TS, typename ReflectiveObject>
        using GenerateNamedReflectiveTypeStructure = typename LD::Detail::GenerateNamedReflectiveTypeStructure<TS,ReflectiveObject>::type;
    }
}
#endif // REFLECTABLE_REFLECTABLE_H_INCLUDED
