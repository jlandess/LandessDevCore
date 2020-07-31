//
// Created by phoenixflower on 7/6/20.
//

#ifndef LANDESSDEVCORE_REFLECTION_HPP
#define LANDESSDEVCORE_REFLECTION_HPP
#include "TypeTraits/Detection.hpp"
#include "Memory/ElementReference.h"
#include "TypeTraits/TypeList.hpp"
#include "TypeTraits/FunctionalReflection.hpp"
namespace LD
{
    namespace CT
    {
        template <typename T>
        struct TypeDescriptor
        {
            /** Used for detecting this non-specialized type_info__ instance. */
            struct invalid_marker{};

            //using MemberList = LD::CT::TypeList<>;

            //static constexpr MemberList Members{  };

            /**
             * This is a placeholder definition of which no type instances should be created.
             */
            //template <size_t, typename>
            //struct member;

            /** The number of reflected members of the target type T. */
            //static constexpr size_t member_count{ 0 };

            /** This is a placeholder definition which shold not be referenced by well-formed programs. */
            //static constexpr refl::const_string<0> name{ "" };

            /** This is a placeholder definition which shold not be referenced by well-formed programs. */
            //static constexpr std::tuple<> attributes{ };
        };

        template<const auto & I, typename T>
        struct Member
        {
            //static_assert(LD::IsTypeString<LD::Detail::Decay_T<decltype(I)>>, "Only works with  Type Strings ");
            static constexpr auto Name = I;
            static constexpr bool IsWrittable = LD::Detail::IsConst<T>::value;
            static constexpr bool IsReadable = true;
            using Type = T;
        };



        template<const auto & NameT, auto  Function, class = void>
        struct FunctionDescriptor;

        template<const auto & NameT, auto Function>
        struct FunctionDescriptor<NameT,Function,LD::Enable_If_T<
                LD::Require<
                LD::CT::IsSignatureMemberFunction(LD::CT::FunctionSignature<decltype(Function)>{})
                >>>
        {
            static constexpr auto Name = NameT;
            static constexpr  auto Type = LD::CT::GetSignatureReturn(LD::CT::FunctionSignature<decltype(Function)>{});
            template<typename T, typename ... A>
            constexpr auto operator()(T && object, A && ... arguments) const noexcept(noexcept((LD::Declval<T>().*Function)(LD::Declval<A>()...))) ->decltype((LD::Declval<T>().*Function)(LD::Declval<A>()...))
            {
                return (object.*Function)(LD::Forward<A>()...);
            }
        };

        template<const auto & NameT, auto Function>
        struct FunctionDescriptor<NameT,Function,LD::Enable_If_T<
                LD::Require<
                        LD::CT::Negate(LD::CT::IsSignatureMemberFunction(LD::CT::FunctionSignature<decltype(Function)>{})),
                        LD::CT::IsFunctionalSignature(LD::CT::FunctionSignature<decltype(Function)>{})
                >>>
        {
            static constexpr auto Name = NameT;
            static constexpr  auto Type = LD::CT::GetSignatureReturn(LD::CT::FunctionSignature<decltype(Function)>{});
            template<typename ... A>
            constexpr auto operator()(A && ...arguments) const noexcept(noexcept(Function(LD::Declval<A>()...))) -> decltype(Function(LD::Declval<A>()...))
            {
                return Function(LD::Forward<A>()...);
            }

        };
        /*
        template<const auto & NameT, auto  Function>
        struct FunctionDescriptor<NameT,Function>
        {
            static constexpr auto Name = NameT;


            template<typename ... A>
            constexpr auto operator()(A && ...arguments) const noexcept(noexcept(Function(LD::Declval<A>()...))) -> decltype(Function(LD::Declval<A>()...))
            {
                return Function(LD::Forward<A>()...);
            }

            template<typename T, typename ... A>
            constexpr auto operator()(T && object, A && ... arguments) const noexcept(noexcept((LD::Declval<T>().*Function)(LD::Declval<A>()...))) ->decltype((LD::Declval<T>().*Function)(LD::Declval<A>()...))
            {
                return (object.*Function)(LD::Forward<A>()...);
            }
        };
         */
        template<typename T,const auto & NameT , auto  SetterT, auto  GetterT>
        struct ProxiedEncapsulatedMemberDescriptor
        {
        private:
            using FirstFunctionType = decltype(SetterT);
            using SecondFunctionType = decltype(GetterT);




            using DecayedType = LD::Detail::Decay_T<T>;
            LD::Detail::Conditional_T<LD::Detail::IsLValueReference<T>::value,LD::ElementReference<DecayedType>,DecayedType> mValue;
            //using AssignableType = decltype((LD::Declval<LD::Detail::Decay_T<T>>().*GetterT)());

            template<typename Q, typename V>
            using SettableWithArg = decltype((LD::Declval<LD::Detail::Decay_T<Q>>().*SetterT)(LD::Declval<V>()));

            template<typename Q>
            using SettableWithoutArg = decltype((LD::Declval<LD::Detail::Decay_T<Q>>().*SetterT)());

            template<typename Q, typename V>
            using SettableWithoutArg1 = decltype((LD::Declval<LD::Detail::Decay_T<Q>>().*SetterT)() = LD::Declval<V>());

            using RetType = decltype(LD::CT::GetSignatureReturn(LD::CT::FunctionSignature<decltype(GetterT)>{}));
            using SetType = decltype(LD::CT::GetSignatureReturn(LD::CT::FunctionSignature<decltype(SetterT)>{}));
            using Ret = typename RetType::type;
            using Set = typename SetType::type;
        public:


            static constexpr auto Name = NameT;

            static constexpr  auto Type = LD::CT::GetSignatureReturn(LD::CT::FunctionSignature<decltype(GetterT)>{});

            constexpr ProxiedEncapsulatedMemberDescriptor(T && object) noexcept:mValue{object}
            {

            }
            constexpr operator Ret () const noexcept
            {
                //return LD::Get(this->mValue.*GetterT)();

                ;
                return (LD::Get(this->mValue).*GetterT)();
                //return {};
            }

            template<typename V>
            ProxiedEncapsulatedMemberDescriptor & operator = (V && v) noexcept
            {

                T & object = LD::Get(this->mValue);

                if constexpr(LD::CT::IsSame(SetType{},LD::CT::RemoveConst(RetType{})))
                {
                    (object.*SetterT)() = LD::Forward<V>(v);
                }else if constexpr(LD::CT::IsSame(SetType{},LD::Type<void>{}))
                {
                    (object.*SetterT)(LD::Forward<V>(v));
                }
                return (*this);
            }


            template<typename F>
            constexpr Ret operator()(F && object) noexcept
            {
                return (LD::Get(this->mValue).*GetterT)();
            }



            /*
            //LD::IsSame<decltype((LD::Declval<LD::Detail::Decay_T<T>>().*SetterT)(LD::Declval<AssignableType>())),void>
            template<typename V, typename U = LD::Detail::Decay_T<V>, typename Y = LD::Detail::Decay_T<AssignableType>>
            constexpr LD::Enable_If_T<
                    LD::Require<LD::IsAssignable_V<Y& ,U>,
                            LD::Exists<SettableWithArg,T,V>
                    >
                    ,ProxiedEncapsulatedMemberDescriptor&>  operator = (V && value) noexcept
            {
                //LD::IsAssignable_V
                (LD::Get(this->mValue).*SetterT)(LD::Forward<V>(value));
                return (*this);
            }

            template<typename V, typename U = LD::Detail::Decay_T<V>, typename Y = LD::Detail::Decay_T<AssignableType>>
            constexpr LD::Enable_If_T<
                    LD::Require<LD::IsAssignable_V<Y& ,U>,
                            LD::Exists<SettableWithoutArg1,T,V>
                    >
                    ,ProxiedEncapsulatedMemberDescriptor&>  operator = (V && value) noexcept
            {
                (LD::Get(this->mValue).*SetterT)() = LD::Forward<V>(value);
                return (*this);
            }
             */
        };

        //template<const auto & name, typename Setter, typename Getter>
        template<const auto & NameT,  auto  SetterT,  auto  GetterT, class = void>
        struct EncapsulatedMemberDescriptor;


        namespace Detail
        {
            template<typename T, typename U>
            constexpr bool IsStandardGetterSetterPair(T firstSignature, U secondSignature) noexcept
            {
                constexpr auto lhs = LD::CT::GetType<0>(LD::CT::GetSignatureArguments(firstSignature));
                constexpr auto rhs = LD::CT::GetSignatureReturn(secondSignature);
                return LD::CT::IsSameWhenDecayed(lhs,rhs);;
            }
        }
        template<const auto & NameT,  auto  SetterT,  auto  GetterT>
        struct EncapsulatedMemberDescriptor<NameT,SetterT,GetterT,LD::Enable_If_T<LD::Require<
                LD::CT::IsFunctionalSignature(LD::CT::FunctionSignature<decltype(SetterT)>{}),
                LD::CT::IsFunctionalSignature(LD::CT::FunctionSignature<decltype(GetterT)>{}),
                LD::Negate<LD::CT::IsSignatureConst(LD::CT::FunctionSignature<decltype(SetterT)>{})>,
                LD::CT::IsSignatureConst(LD::CT::FunctionSignature<decltype(GetterT)>{}),
                LD::Either<
                        LD::Require<
                                LD::CT::IsSameWhenDecayed(LD::CT::GetSignatureReturn(LD::CT::FunctionSignature<decltype(SetterT)>{}),LD::CT::GetSignatureReturn(LD::CT::FunctionSignature<decltype(GetterT)>{})),
                                (LD::CT::GetNumberOfArgumentsInFunctionSignature(LD::CT::FunctionSignature<decltype(SetterT)>{}) == 0),
                                (LD::CT::GetNumberOfArgumentsInFunctionSignature(LD::CT::FunctionSignature<decltype(GetterT)>{}) == 0),
                                LD::CT::IsReference(LD::CT::GetSignatureReturn(LD::CT::FunctionSignature<decltype(SetterT)>{})),
                                LD::CT::Negate(LD::CT::IsConst(LD::CT::GetSignatureReturn(LD::CT::FunctionSignature<decltype(SetterT)>{})))
                                >,
                        LD::Require<
                                LD::CT::Detail::IsStandardGetterSetterPair(LD::CT::FunctionSignature<decltype(SetterT)>{},LD::CT::FunctionSignature<decltype(GetterT)>{}),
                                (LD::CT::GetNumberOfArgumentsInFunctionSignature(LD::CT::FunctionSignature<decltype(SetterT)>{}) == 1),
                                (LD::CT::GetNumberOfArgumentsInFunctionSignature(LD::CT::FunctionSignature<decltype(GetterT)>{}) == 0)
                                >
                        >,
                LD::Negate<LD::CT::IsSame(LD::CT::GetSignatureReturn(LD::CT::FunctionSignature<decltype(GetterT)>{}),LD::Type<void>{})>
                >>>
        {
            static constexpr auto Name = NameT;
            static constexpr auto Setter = SetterT;
            static constexpr auto Getter = GetterT;
            static constexpr  auto Type = LD::CT::GetSignatureReturn(LD::CT::FunctionSignature<decltype(GetterT)>{});
            template <typename T>
            constexpr auto operator()(T&& target) const noexcept -> ProxiedEncapsulatedMemberDescriptor<T,NameT,SetterT,GetterT>
            {
                return ProxiedEncapsulatedMemberDescriptor<T,NameT,SetterT,GetterT>{LD::Forward<T>(target)};
            }
        };

        template<const auto & NameT,  auto  SetterT,  auto  GetterT>
        struct EncapsulatedMemberDescriptor<NameT,SetterT,GetterT,LD::Enable_If_T<LD::Require<
                LD::CT::IsFunctionalSignature(LD::CT::FunctionSignature<decltype(SetterT)>{}),
                LD::CT::IsFunctionalSignature(LD::CT::FunctionSignature<decltype(GetterT)>{}),
                LD::Negate<LD::CT::IsSignatureConst(LD::CT::FunctionSignature<decltype(GetterT)>{})>,
                LD::CT::IsSignatureConst(LD::CT::FunctionSignature<decltype(SetterT)>{}),
                LD::Either<
                        LD::Require<
                                LD::CT::IsSameWhenDecayed(LD::CT::GetSignatureReturn(LD::CT::FunctionSignature<decltype(GetterT)>{}),LD::CT::GetSignatureReturn(LD::CT::FunctionSignature<decltype(SetterT)>{})),
                                (LD::CT::GetNumberOfArgumentsInFunctionSignature(LD::CT::FunctionSignature<decltype(GetterT)>{}) == 0),
                                (LD::CT::GetNumberOfArgumentsInFunctionSignature(LD::CT::FunctionSignature<decltype(SetterT)>{}) == 0),
                                LD::CT::IsReference(LD::CT::GetSignatureReturn(LD::CT::FunctionSignature<decltype(GetterT)>{})),
                                LD::CT::Negate(LD::CT::IsConst(LD::CT::GetSignatureReturn(LD::CT::FunctionSignature<decltype(GetterT)>{})))
                        >,
                        LD::Require<
                                LD::CT::Detail::IsStandardGetterSetterPair(LD::CT::FunctionSignature<decltype(GetterT)>{},LD::CT::FunctionSignature<decltype(SetterT)>{}),
                                (LD::CT::GetNumberOfArgumentsInFunctionSignature(LD::CT::FunctionSignature<decltype(GetterT)>{}) == 1),
                                (LD::CT::GetNumberOfArgumentsInFunctionSignature(LD::CT::FunctionSignature<decltype(SetterT)>{}) == 0)
                        >
                >,
                LD::Negate<LD::CT::IsSame(LD::CT::GetSignatureReturn(LD::CT::FunctionSignature<decltype(SetterT)>{}),LD::Type<void>{})>
        >>>
        {
            static constexpr auto Name = NameT;
            static constexpr auto Setter = SetterT;
            static constexpr auto Getter = GetterT;
            static constexpr  auto Type = LD::CT::GetSignatureReturn(LD::CT::FunctionSignature<decltype(GetterT)>{});

            template <typename T>
            constexpr auto operator()(T&& target) const noexcept -> ProxiedEncapsulatedMemberDescriptor<T,NameT,SetterT,GetterT>
            {
                return ProxiedEncapsulatedMemberDescriptor<T,NameT,GetterT,SetterT>{LD::Forward<T>(target)};
            }
        };





        template<const auto & NameT,  auto  Field>
        struct MemberDescriptor
        {
            static constexpr auto Name = NameT;
            static constexpr auto Pointer = Field;
            //static constexpr bool IsStatic = !std::is_member_object_pointer_v<decltype(Pointer)> ;
            template <typename T>
            constexpr auto operator()(T&& target) const noexcept -> decltype(target.*(Pointer))
            {
                return target.*(Pointer);
            }

            template <typename T,typename  DecayedType = LD::Detail::Decay_T<T>>
            constexpr auto operator()(T&& target)  const noexcept -> decltype(DecayedType::Pointer)
            {
                return DecayedType::Pointer;
            }

            static constexpr  auto Type = LD::CT::GetFieldType(LD::CT::FieldSignature<decltype(Field)>{});
        };

        template<const auto & name, auto Description>
        constexpr auto GetMemberDescriptorName(MemberDescriptor<name,Description> ) noexcept
        {
            return MemberDescriptor<name,Description>::Name;
        }

        template<typename T,const auto & NameT,  auto  SetterT,  auto  GetterT>
        constexpr auto GetMemberDescriptorName(ProxiedEncapsulatedMemberDescriptor<T,NameT,SetterT,GetterT> ) noexcept
        {
            return ProxiedEncapsulatedMemberDescriptor<T,NameT,SetterT,GetterT>::Name;
        }

        template<const auto & NameT,  auto  SetterT,  auto  GetterT>
        constexpr auto GetMemberDescriptorName(EncapsulatedMemberDescriptor<NameT,SetterT,GetterT> ) noexcept
        {
            return EncapsulatedMemberDescriptor<NameT,SetterT,GetterT>::Name;
        }
        template<const auto & NameT, auto Function>
        constexpr auto GetMemberDescriptorName(LD::CT::FunctionDescriptor<NameT,Function>) noexcept
        {
            return LD::CT::FunctionDescriptor<NameT,Function>::Name;
        }

        template<typename T>
        constexpr bool IsFunctionDescriptor(T &&) noexcept
        {
            return false;
        }

        template<const auto & Name, auto Function>
        constexpr bool IsFunctionDescriptor(LD::CT::FunctionDescriptor<Name,Function>) noexcept
        {
            return true;
        }

        template<typename T>
        constexpr bool IsMemberDescriptor(T && ) noexcept
        {
            return false;
        }

        template<const auto & Name, auto Field>
        constexpr bool IsMemberDescriptor(LD::CT::MemberDescriptor<Name,Field> ) noexcept
        {
            return true;
        }

        template<const auto & name, auto F1, auto F2>
        constexpr bool IsMemberDescriptor(LD::CT::EncapsulatedMemberDescriptor<name,F1,F2> ) noexcept
        {
            return true;
        }

        template<typename T>
        constexpr bool IsMemberDescriptorWritable(T && ) noexcept
        {
            return false;
        }

        template<const auto & Name, auto Field>
        constexpr bool IsMemberDescriptorWritable(LD::CT::MemberDescriptor<Name,Field> ) noexcept
        {
            return !LD::CT::IsFieldConst(LD::CT::FieldSignature<decltype(Field)>{});
        }

        template<const auto & name, auto F1, auto F2>
        constexpr bool IsMemberDescriptorWritable(LD::CT::EncapsulatedMemberDescriptor<name,F1,F2> ) noexcept
        {
            return true;
        }

        template<const auto & Name, auto Field>
        constexpr bool IsMemberDescriptorReadable(LD::CT::MemberDescriptor<Name,Field> ) noexcept
        {
            return true;
        }

        template<const auto & name, auto F1, auto F2>
        constexpr bool IsMemberDescriptorReadable(LD::CT::EncapsulatedMemberDescriptor<name,F1,F2> ) noexcept
        {
            return true;
        }

        template<typename T>
        constexpr auto GetDescriptorType(T && ) noexcept
        {
            return LD::Type<void>{};
        }

        template<const auto & Name, auto Field>
        constexpr auto GetDescriptorType(LD::CT::MemberDescriptor<Name,Field> ) noexcept
        {
            return LD::CT::MemberDescriptor<Name,Field>::Type;
        }


        template<const auto & name, auto F1, auto F2>
        constexpr auto GetDescriptorType(LD::CT::EncapsulatedMemberDescriptor<name,F1,F2> ) noexcept
        {
            return LD::CT::EncapsulatedMemberDescriptor<name,F1,F2>::Type;
        }


        /*
        template<const auto & Name, auto F>
        constexpr auto GetMemberDescirptorType(LD::CT::FunctionDescriptor<Name,F>) noexcept
        {
            return LD::CT::FunctionDescriptor<Name,F>::Type;
        }
         */

        template<typename T>
        constexpr auto GetClassName(T && ) noexcept
        {
            return LD::CT::TypeDescriptor<LD::Detail::Decay_T<T>>::ClassName;
        }

        template<typename T>
        constexpr auto GetClassName(LD::Type<T>) noexcept
        {
            return LD::CT::TypeDescriptor<LD::Detail::Decay_T<T>>::ClassName;
        }



        template<typename T, typename Result = typename LD::CT::TypeDescriptor<T>::MemberList>
        constexpr auto GetMemberDescriptors(LD::CT::TypeDescriptor<T> ) noexcept //->decltype(LD::Declval<Result>())
        {
            return typename LD::CT::TypeDescriptor<T>::MemberList{};
        }

        namespace Detail
        {
            template<typename Member, typename Object>
            struct GetMemberType;

            template<const auto & NameT,  auto  Description, typename Object>
            struct GetMemberType<MemberDescriptor<NameT,Description>,Object>
            {
                using type = decltype(LD::Declval<MemberDescriptor<NameT,Description>>()(LD::Declval<Object>()));
                //using type = float;
            };

            template<const auto & NameT,  auto  SetterT,  auto  GetterT, typename Object>
            struct GetMemberType<EncapsulatedMemberDescriptor<NameT,SetterT,GetterT>,Object>
            {
                using Member = EncapsulatedMemberDescriptor<NameT,SetterT,GetterT>;
                using Proxy = decltype(LD::Declval<Member>()(LD::Declval<Object>()));
                //using type = decltype(LD::Declval<MemberDescriptor<NameT,Description>>()(LD::Declval<Object>()));
                //using type = float;
                using type = decltype(LD::Declval<Proxy>()(LD::Declval<Object>()));
            };
        }

        template<typename T, typename Object>
        using GetMemberType = typename LD::CT::Detail::GetMemberType<T,Object>::type;

        template<const auto & I>
        class ClassName
        {
            static constexpr auto Name = I;
        };

        template<const auto & Name, auto & FunctionPointer>
        class Function
        {

        };

        namespace Detail
        {
            template<typename T>
            class IsMember: public LD::Detail::IntegralConstant<bool,false>
            {

            };

            template<const auto  & I, typename T>
            class IsMember<LD::CT::Member<I,T>>: public LD::Detail::IntegralConstant<bool,true>
            {

            };

            template<typename T>
            class IsClassName: public LD::Detail::IntegralConstant<bool,false>
            {

            };

            template<const auto  & I>
            class IsClassName<LD::CT::ClassName<I>>: public LD::Detail::IntegralConstant<bool,true>
            {

            };
        }

        template<typename T>
        constexpr bool IsMember = LD::CT::Detail::IsMember<T>::value;

        template<typename T>
        constexpr bool IsClassName = LD::CT::Detail::IsClassName<T>::value;


        /*
        template<typename T, class = void>
        class Reflectable;


        template<typename ClassNameT, typename ... Args>
        class Reflectable<ClassNameT(Args...),LD::Enable_If_T<LD::Require<LD::CT::IsClassName<ClassNameT>,(LD::CT::IsMember<Args> & ...)>>>
        {
        private:

        public:

            using Members = LD::CT::TypeList<Args...>;
            static constexpr auto ClassName = ClassNameT::Name;
        };

         */
        namespace Detail
        {
            template<typename T>
            using HasMemberList = decltype(LD::Declval<T>().Members);

            template<typename T>
            using HasClassName = decltype(LD::Declval<T>().ClassName);


            template<typename T>
            struct HasReflectableTraits
            {
                static constexpr bool value = LD::Require<
                        LD::Exists<LD::CT::Detail::HasMemberList,T>,
                        LD::Exists<LD::CT::Detail::HasClassName,T>
                        >;
            };


            template<typename T, typename = void>
            struct IsReflectable
            {
                static constexpr bool value = false;
            };

            template<typename T>
            struct IsReflectable<T,LD::Enable_If_T<
                    LD::Require<
                    LD::CT::Detail::HasReflectableTraits<LD::CT::TypeDescriptor<T>>::value
                    >>>
            {
                using MemberListType = LD::Detail::Decay_T<decltype(LD::Declval<LD::CT::TypeDescriptor<T>>().Members)>;
                static constexpr bool value = LD::Require<
                        LD::CT::IsTypeList<MemberListType>
                        >;
            };
        }


        template<typename T>
        constexpr bool IsReflectable() noexcept
        {
            return LD::CT::Detail::IsReflectable<LD::Detail::Decay_T<T>>::value;
        }

        template<typename T>
        constexpr auto IsReflectable(LD::Type<T>) noexcept
        {
            return LD::CT::IsReflectable<T>();
        }
        template<typename T>
        constexpr LD::Enable_If_T<
        LD::Require<
        LD::CT::IsReflectable<T>()
                >
        ,LD::UInteger> GetNumberOfMembers(LD::CT::TypeDescriptor<T>) noexcept
        {
            return LD::CT::TypeDescriptor<T>::MemberList::size();
        }

        template<typename T>
        constexpr LD::Enable_If_T<
                LD::Require<
                        LD::Negate<LD::CT::IsReflectable<T>()>
                >
                ,LD::UInteger> GetNumberOfMembers(LD::CT::TypeDescriptor<T>) noexcept
        {
            return 0;
        }

        template<typename T>
        constexpr bool IsReflectable(T &&) noexcept
        {
            return LD::CT::Detail::IsReflectable<LD::Detail::Decay_T<T>>::value;
        }

        template<typename T>
        constexpr LD::Enable_If_T<LD::Require<
        LD::CT::IsReflectable<LD::Detail::Decay_T<T>>()
        >,
        TypeDescriptor<LD::Detail::Decay_T<T>>> Reflect(T && ) noexcept
        {
            return {};
        }

        template<typename T>
        constexpr LD::Enable_If_T<LD::Require<
                LD::CT::IsReflectable<LD::Detail::Decay_T<T>>()
        >,
                TypeDescriptor<LD::Detail::Decay_T<T>>> Reflect(LD::Type<T> ) noexcept
        {
            return {};
        }

        template<typename T>
        constexpr LD::Enable_If_T<LD::Require<
                LD::CT::IsReflectable<LD::Detail::Decay_T<T>>()
        >,
                TypeDescriptor<LD::Detail::Decay_T<T>>> Reflect( ) noexcept
        {
            return {};
        }
    }
}

namespace LD
{
    namespace Detail
    {
        template<typename T, typename F>
        struct MapMembersToTuple
        {

        };

        template<typename ... Members, typename Object>
        struct MapMembersToTuple<LD::CT::TypeList<Members...>,Object>
        {
            //using type = LD::CT::TypeList<LD::Detail::Decay_T<decltype(LD::Declval<Members>()(LD::Declval<Object>()))>...>;
            using type = LD::CT::TypeList<LD::Detail::Decay_T<LD::CT::GetMemberType<Members,Object>>...>;
        };
    }
}
namespace LD
{
    template<typename T, typename V = LD::Detail::Decay_T<T>,
            typename Members = typename LD::Detail::MapMembersToTuple<typename LD::CT::TypeDescriptor<V>::MemberList,T>::type>
    constexpr LD::Enable_If_T<
            LD::Require<
                    LD::CT::Detail::IsReflectable<V>::value
            >,
            LD::CT::RebindList<Members,LD::Tuple>> MapToTuple(T && object) noexcept
    {
        using MemberTuple = LD::CT::RebindList<Members,LD::Tuple>;
        MemberTuple ret;
        constexpr auto traits = LD::CT::Reflect<LD::Detail::Decay_T<T>>().Members;
        LD::For<Members::size()>([](
                auto I,
                T && object,
                const auto  traits,
                auto && tuple)
        {
            constexpr auto memberInfo = LD::Get<I>(traits);
            LD::Get<I>(LD::Forward<MemberTuple>(tuple)) =  memberInfo(LD::Forward<T>(object));
            return true;
        },LD::Forward<T>(object),LD::Forward<decltype(traits)>(traits),LD::Forward<MemberTuple>(ret));
        return ret;
    }
}
#endif //LANDESSDEVCORE_REFLECTION_HPP
