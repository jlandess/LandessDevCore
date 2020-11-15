//
// Created by phoenixflower on 10/17/20.
//

#ifndef LANDESSDEVCORE_ATTRIBUTES_HPP
#define LANDESSDEVCORE_ATTRIBUTES_HPP
#include "TypeTraits/TypeList.hpp"
#include "Primitives/General/Pair.h"
#include "Reflection/Reflection.hpp"
#include "Primitives/General/ctre.hpp"
namespace LD
{
    namespace SPA
    {
        template<typename AttributeType, typename T>
        class Attribute
        {
        private:
            T mValue;
        public:
            static constexpr auto TagName = AttributeType::TagName;
            Attribute(AttributeType,T && object) noexcept:mValue{object}
            {

            }
            T & Value() noexcept
            {
                return this->mValue;
            }

            const T & Value() const noexcept
            {
                return this->mValue;
            }
        };

        namespace Detail
        {
            template<typename T>
            struct IsAttribute
            {
                static constexpr bool value = false;
            };

            template<typename T, typename V>
            struct IsAttribute<LD::SPA::Attribute<T,V>>
            {
                static constexpr bool value = true;
            };
        }

        //template<typename T>
        //constexpr bool IsAttribute = LD::SPA::Detail::IsAttribute<T>::value;

        template <typename AttributeType, typename T> Attribute(AttributeType,T && object) -> Attribute<AttributeType,T>;
    }

    namespace SPA
    {
        template<typename T, typename V>
        constexpr bool IsAttribute(LD::Type<LD::SPA::Attribute<T,V>> ) noexcept
        {
            return true;
        }

        template<typename T>
        constexpr bool IsAttribute(LD::Type<T> ) noexcept
        {
            return false;
        }
    }
    template<typename AttribteType, typename T>
    struct LD::CT::TypeDescriptor<LD::SPA::Attribute<AttribteType,T>>
    {
    private:
        static constexpr auto TagName = AttribteType::TagName;
    public:
        static constexpr auto ClassName = ctll::fixed_string{"attribute"};

        using MemberList = LD::CT::TypeList<
                LD::CT::EncapsulatedMemberDescriptor<TagName,LD::CT::SelectOverload<T & (LD::SPA::Attribute<AttribteType,T>::*)(),&LD::SPA::Attribute<AttribteType,T>::Value>(),LD::CT::SelectOverload<const T & (LD::SPA::Attribute<AttribteType,T>::*)() const,&LD::SPA::Attribute<AttribteType,T>::Value>()>
         >;


        static constexpr MemberList Members{  };

    };
    namespace HTML
    {

        //using FormAttributes = LD::CT::TypeList<Accept,AcceptCharset,Action,AutoComplete,Enctype,NoValidate>;
        //using iFrameAttributes = LD::CT::TypeList<Align,Allow,CSP,Importance,Loading,Name,Sandbox,SrcDoc>;
        //using InputAttributes = LD::CT::TypeList<Accept,Alt,AutoComplete,AutoFocus,Capture,Checked,DirName,Disabled,FormName,FormAction,FormMethod,FormNoValidate,FormTarget,
        //List,Max,MaxLength,MinLength,Min,Multiple,Name,Pattern,PlaceHolder,ReadOnly,Required,Size,Src,Step,Type,UseMap,Value>;

    }
}
#endif //LANDESSDEVCORE_ATTRIBUTES_HPP
