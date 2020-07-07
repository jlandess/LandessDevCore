//
// Created by phoenixflower on 7/6/20.
//

#ifndef LANDESSDEVCORE_REFLECTION_HPP
#define LANDESSDEVCORE_REFLECTION_HPP
#include "TypeTraits/Detection.hpp"
#include "Memory/ElementReference.h"
#include "TypeTraits/TypeList.hpp"
namespace LD
{
    namespace CT
    {
        template <typename T>
        struct TypeDescriptor
        {
            /** Used for detecting this non-specialized type_info__ instance. */
            struct invalid_marker{};

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



        template<auto  S>
        struct Setter
        {
            static constexpr auto Pointer = S;
        };

        template< auto  G>
        struct Getter
        {
            static constexpr auto Pointer = G;
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



        template<const auto & NameT, const auto & Function>
        struct FunctionDescriptor
        {
            static constexpr auto Name = NameT;


            template<typename ... A>
            constexpr auto operator()(A && ...arguments) const noexcept -> decltype(Function(LD::Declval<A>()...))
            {
                return Function(LD::Forward<A>()...);
            }

            template<typename T, typename ... A>
            constexpr auto operator()(T && object, A && ... arguments) const noexcept ->decltype((LD::Declval<T>().*Function)(LD::Declval<A>()...))
            {
                return (object.*Function)(LD::Forward<A>()...);
            }
        };
        template<typename T,const auto & NameT , auto  SetterT, auto  GetterT>
        struct ProxiedEncapsulatedMemberDescriptor
        {
        private:

            using DecayedType = LD::Detail::Decay_T<T>;
            LD::Detail::Conditional_T<LD::Detail::IsLValueReference<T>::value,LD::ElementReference<DecayedType>,DecayedType> mValue;
            using AssignableType = decltype((LD::Declval<LD::Detail::Decay_T<T>>().*GetterT)());

            template<typename Q, typename V>
            using SettableWithArg = decltype((LD::Declval<LD::Detail::Decay_T<Q>>().*SetterT)(LD::Declval<V>()));

            template<typename Q>
            using SettableWithoutArg = decltype((LD::Declval<LD::Detail::Decay_T<Q>>().*SetterT)());

            template<typename Q, typename V>
            using SettableWithoutArg1 = decltype((LD::Declval<LD::Detail::Decay_T<Q>>().*SetterT)() = LD::Declval<V>());

        public:


            static constexpr auto Name = NameT;
            constexpr ProxiedEncapsulatedMemberDescriptor(T && object) noexcept:mValue{object}
            {

            }
            constexpr operator AssignableType () const noexcept
            {
                //return LD::Get(this->mValue.*GetterT)();

                return (LD::Get(this->mValue).*GetterT)();
                return {};
            }

            template<typename F>
            constexpr AssignableType operator()(F && object) noexcept
            {
                return (LD::Get(this->mValue).*GetterT)();
            }


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
        };

        //template<const auto & name, typename Setter, typename Getter>
        template<const auto & NameT,  auto  SetterT,  auto  GetterT>
        struct EncapsulatedMemberDescriptor;


        template<const auto & NameT,  auto  SetterT,  auto  GetterT>
        struct EncapsulatedMemberDescriptor
        {
            static constexpr auto Name = NameT;
            static constexpr auto Setter = SetterT;
            static constexpr auto Getter = GetterT;

            template <typename T>
            constexpr auto operator()(T&& target) const noexcept -> ProxiedEncapsulatedMemberDescriptor<T,NameT,SetterT,GetterT>
            {
                return ProxiedEncapsulatedMemberDescriptor<T,NameT,SetterT,GetterT>{LD::Forward<T>(target)};
            }
        };



        template<const auto & NameT,  auto  Description>
        struct MemberDescriptor
        {
            static constexpr auto Name = NameT;
            static constexpr auto Pointer = Description;
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
        };

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
            return LD::CT::Detail::IsReflectable<T>::value;
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
    }
}
#endif //LANDESSDEVCORE_REFLECTION_HPP
