//
// Created by phoenixflower on 7/7/20.
//

#ifndef LANDESSDEVCORE_FUNCTIONALREFLECTION_HPP
#define LANDESSDEVCORE_FUNCTIONALREFLECTION_HPP
#include "TypeList.hpp"
#include "Type.h"
#include "Core/Tuple.hpp"
namespace LD
{
    namespace CT
    {
        template<typename Ret>
        struct FunctionSignature
        {
        };

        template<typename T>
        struct FieldSignature
        {

        };

        template <typename Object, typename Field>
        struct FieldSignature<Field Object::*>
        {
            //using ObjectType = Object;
            //using FieldType = Field;
        };

        template<typename Return, typename ... Args>
        struct FunctionSignature<Return(*)(Args...)>
        {
            using ObjectType = void;
            using ReturnType = Return;
            using Arguments = LD::CT::TypeList<Args...>;
            static constexpr bool IsConst = false;
            static constexpr bool IsNoExcept = false;
        };


        template<typename Return, typename ... Args>
        struct FunctionSignature<Return(*)(Args...) noexcept>
        {
            using ObjectType = void;
            using ReturnType = Return;
            using Arguments = LD::CT::TypeList<Args...>;
            static constexpr bool IsConst = false;
            static constexpr bool IsNoExcept = true;
        };



        /*
        template<typename Return, typename Object>
        struct FunctionSignature<Return(Object::*)()>
        {
            using ObjectType = Object;
            using ReturnType = Return;
            using Arguments = LD::CT::TypeList<>;
            static constexpr bool IsConst = false;
            static constexpr bool IsNoExcept = false;
        };
         */

        /*
        template<typename Return, typename Object>
        struct FunctionSignature<Return(Object::*)() noexcept>
        {
            using ObjectType = Object;
            using ReturnType = Return;
            using Arguments = LD::CT::TypeList<>;
            static constexpr bool IsConst = false;
            static constexpr bool IsNoExcept = true;
        };

        template<typename Return, typename Object>
        struct FunctionSignature<Return(Object::*)() const>
        {
            using ObjectType = Object;
            using ReturnType = Return;
            using Arguments = LD::CT::TypeList<>;
            static constexpr bool IsConst = true;
            static constexpr bool IsNoExcept = false;
        };

        template<typename Return, typename Object>
        struct FunctionSignature<Return(Object::*)() const noexcept>
        {
            using ObjectType = Object;
            using ReturnType = Return;
            using Arguments = LD::CT::TypeList<>;
            static constexpr bool IsConst = true;
            static constexpr bool IsNoExcept = true;
        };
         */

        template<typename Return, typename Object, typename ... Args>
        struct FunctionSignature<Return(Object::*)(Args...)>
        {
            using ObjectType = Object;
            using ReturnType = Return;
            using Arguments = LD::CT::TypeList<Args...>;
            static constexpr bool IsConst = false;
            static constexpr bool IsNoExcept = false;
        };

        template<typename Return, typename Object, typename ... Args>
        struct FunctionSignature<Return(Object::*)(Args...) noexcept>
        {
            using ObjectType = Object;
            using ReturnType = Return;
            using Arguments = LD::CT::TypeList<Args...>;
            static constexpr bool IsConst = false;
            static constexpr bool IsNoExcept = true;
        };

        template<typename Return, typename Object, typename ... Args>
        struct FunctionSignature<Return(Object::*)(Args...) const>
        {
            using ObjectType = Object;
            using ReturnType = Return;
            using Arguments = LD::CT::TypeList<Args...>;
            static constexpr bool IsConst = true;
            static constexpr bool IsNoExcept = false;
        };

        template<typename Return, typename Object, typename ... Args>
        struct FunctionSignature<Return(Object::*)(Args...) const noexcept>
        {
            using ObjectType = Object;
            using ReturnType = Return;
            using Arguments = LD::CT::TypeList<Args...>;
            static constexpr bool IsConst = true;
            static constexpr bool IsNoExcept = true;
        };

        struct InvalidFunctionSignature
        {

        };

        struct InvalidFieldSignature
        {

        };

        template<typename T, typename U>
        constexpr bool IsSame(LD::Type<T>,LD::Type<U>) noexcept
        {
            return false;
        }

        template<typename T>
        constexpr bool IsSame(LD::Type<T>,LD::Type<T>) noexcept
        {
            return true;
        }

        template<typename T>
        constexpr bool IsPrimitive(LD::Type<T> ) noexcept
        {
            return LD::Detail::IsPrimitiveType<T>::value;
        }
        template<typename T>
        constexpr bool IsTuple(LD::Type<T> ) noexcept
        {
            return LD::IsTuple<T>;
        }
        template<typename ... B>
        constexpr auto TupleSize( LD::Type<LD::Tuple<B...>>) noexcept
        {
            return LD::TupleSize<LD::Tuple<B...>>::value;
        }


        template<typename T>
        constexpr auto RemoveConst(LD::Type<T> ) noexcept
        {
            return LD::Type<T>{};
        }

        template<typename T>
        constexpr auto RemoveConst(LD::Type<volatile const T> ) noexcept
        {
            return LD::Type<volatile T>{};
        }
        template<typename T>
        constexpr auto RemoveConst(LD::Type<const T> ) noexcept
        {
            return LD::Type<T>{};
        }

        template<typename T>
        constexpr auto RemoveConst(LD::Type<const T &> ) noexcept
        {
            return LD::Type<T&>{};
        }

        template<typename T>
        constexpr auto RemoveConst(LD::Type<volatile const T &> ) noexcept
        {
            return LD::Type<volatile T&>{};
        }

        template<typename T>
        constexpr auto RemoveReference(LD::Type<T> ) noexcept
        {
            return LD::Type<T>{};
        }

        template<typename T>
        constexpr auto RemoveReference(LD::Type<T&> ) noexcept
        {
            return LD::Type<T>{};
        }

        template<typename T>
        constexpr auto RemoveReference(LD::Type<volatile T&> ) noexcept
        {
            return LD::Type<T>{};
        }

        template<typename T>
        constexpr auto RemoveReference(LD::Type<const T&> ) noexcept
        {
            return LD::Type<const T>{};
        }

        template<typename T>
        constexpr auto RemoveReference(LD::Type<volatile const T&> ) noexcept
        {
            return LD::Type<volatile const T>{};
        }

        template<typename T>
        constexpr auto RemoveVolatile(LD::Type<T> ) noexcept
        {
            return LD::Type<T>{};
        }

        template<typename T>
        constexpr auto RemoveVolatile(LD::Type<volatile T> ) noexcept
        {
            return LD::Type<T>{};
        }

        template<typename T>
        constexpr auto RemoveVolatile(LD::Type<volatile T &> ) noexcept
        {
            return LD::Type<T &>{};
        }

        template<typename T>
        constexpr auto RemoveVolatile(LD::Type<volatile const T> ) noexcept
        {
            return LD::Type<const T>{};
        }

        template<typename T>
        constexpr auto RemoveVolatile(LD::Type<volatile const T &> ) noexcept
        {
            return LD::Type<const T &>{};
        }

        template<typename T>
        constexpr auto RemoveConstVolatile(LD::Type<T> ) noexcept
        {
            return LD::CT::RemoveVolatile(LD::CT::RemoveConst(LD::Type<T>{}));
        }

        template<typename T>
        constexpr auto RemoveQualifiers(LD::Type<T> ) noexcept
        {
           // LD::Detail::RemoveCV<T>::type;
            return LD::CT::RemoveVolatile(LD::CT::RemoveConst(LD::CT::RemoveReference(LD::Type<T>{})));
        }



        template<typename T, typename U>
        constexpr bool IsSameWhenDecayed(LD::Type<T>,LD::Type<U>) noexcept
        {
            return LD::Detail::IsSame<LD::Detail::Decay_T<T>,LD::Detail::Decay_T<U>>::value;
        }

        template<typename T, typename U>
        constexpr bool IsSameWithoutQualifiers(LD::Type<T>, LD::Type<U>) noexcept
        {
            return LD::CT::IsSame(LD::CT::RemoveQualifiers(LD::Type<T>{}),LD::CT::RemoveQualifiers(LD::Type<U>{}));
        }


        //template<auto Abc>

        template<typename T>
        constexpr bool IsConst(LD::Type<T>) noexcept
        {
            return LD::Detail::IsConst<T>::value;
        }

        template<typename T>
        constexpr bool IsReference(LD::Type<T> ) noexcept
        {
            return LD::Detail::IsReference<T>::value;
        }

        constexpr const bool Negate(bool value) noexcept
        {
            return !value;
        }
        //------------------------------------------beginning of field reflection----------------------------------
        template<typename T>
        constexpr LD::Type<InvalidFieldSignature> GetFieldType(LD::CT::FieldSignature<T>) noexcept
        {
            return LD::Type<InvalidFieldSignature>{};
        }


        template<typename T>
        constexpr LD::Type<InvalidFieldSignature> GetFieldObject(LD::CT::FieldSignature<T>) noexcept
        {
            return LD::Type<InvalidFieldSignature>{};
        }

        template<typename T>
        constexpr bool IsFieldConst(FieldSignature<T>) noexcept
        {
            return false;
        }

        template<typename T>
        constexpr bool IsFieldAReference(FieldSignature<T>) noexcept
        {
            return false;
        }

        template<typename T>
        constexpr bool IsFieldAPointer(FieldSignature<T>) noexcept
        {
            return false;
        }

        template<typename T>
        constexpr bool IsFieldConstReference(FieldSignature<T>) noexcept
        {
            return false;
        }


        template <typename Object, typename Field>
        constexpr LD::Type<Object> GetFieldObject(FieldSignature<Field Object::*>) noexcept
        {
            return LD::Type<Object>{};
        }

        template <typename Object, typename Field>
        constexpr LD::Type<Field> GetFieldType(FieldSignature<Field Object::*>) noexcept
        {
            return LD::Type<Field>{};
        }

        template <typename Object, typename Field>
        constexpr bool IsFieldConst(FieldSignature<Field Object::*>) noexcept
        {
            return LD::Detail::IsConst<Field>::value;
        }
        template <typename Object, typename Field>
        constexpr bool IsFieldAReference(FieldSignature<Field Object::*>) noexcept
        {
            return LD::Detail::IsReference<Field>::value;
        }

        template <typename Object, typename Field>
        constexpr bool IsFieldAPointer(FieldSignature<Field Object::*>) noexcept
        {
            return LD::Detail::IsPointer<Field>::value;
        }

        template <typename Object, typename Field>
        constexpr bool IsFieldConstReference(FieldSignature<Field Object::*>) noexcept
        {
            return LD::Require<LD::Detail::IsConst<Field>::value,LD::Detail::IsReference<Field>::value>;
        }

        template <typename Object, typename Field>
        constexpr bool IsFieldConstPointer(FieldSignature<Field Object::*>) noexcept
        {
            return LD::Require<LD::Detail::IsConst<Field>::value,LD::Detail::IsPointer<Field>::value>;
        }
        //template <typename Object, typename Field>
        //struct FieldSignature<Field Object::*>


        //---------------------------------------------end of field reflection-------------------------------------

        //----------------------------beginning of function reflection for non functions --------------------------
        template<typename T>
        constexpr bool IsFunctionalSignature(const FunctionSignature<T> &) noexcept
        {
            return false;
        }

        template<typename T>
        constexpr LD::Type<LD::CT::InvalidFunctionSignature> GetSignatureReturn(const FunctionSignature<T> &) noexcept
        {
            return LD::Type<LD::CT::InvalidFunctionSignature>{};
        }

        template<typename T>
        constexpr LD::CT::TypeList<> GetSignatureArguments(const FunctionSignature<T> &) noexcept
        {
            return LD::CT::TypeList<>{};
        }
        template<typename T>
        constexpr bool IsSignatureConst(const FunctionSignature<T> &) noexcept
        {
            return false;
        }
        template<typename T>
        constexpr bool IsSignatureNoExcept(const FunctionSignature<T> &) noexcept
        {
            return false;
        }
        template<typename T>
        constexpr bool IsSignatureMemberFunction(const FunctionSignature<T> &) noexcept
        {
            return false;
        }

        template<typename T>
        constexpr LD::UInteger GetNumberOfArgumentsInFunctionSignature(const FunctionSignature<T> &) noexcept
        {
            return 0;
        }
        //----------------------------------------end of non function reflection -----------------------------------
        //-------------------------------------beginning of function reflection -----------------------------------

        template<typename Return,typename ... Args>
        constexpr bool IsFunctionalSignature(const FunctionSignature<Return(*)(Args...)> &) noexcept
        {
            return true;
        }

        template<typename Return,typename ... Args>
        constexpr bool IsFunctionalSignature(const FunctionSignature<Return(*)(Args...) noexcept> &) noexcept
        {
            return true;
        }
        template<typename Return,typename ... Args>
        constexpr LD::Type<Return> GetSignatureReturn(const FunctionSignature<Return(*)(Args...)> &) noexcept
        {
            return LD::Type<Return>{};
        }

        template<typename Return,typename ... Args>
        constexpr LD::Type<Return> GetSignatureReturn(const FunctionSignature<Return(*)(Args...) noexcept> &) noexcept
        {
            return LD::Type<Return>{};
        }

        template<typename Return,typename ... Args>
        constexpr LD::CT::TypeList<Args...> GetSignatureArguments(const FunctionSignature<Return(*)(Args...)> &) noexcept
        {
            return LD::CT::TypeList<Args...>{};
        }

        template<typename Return,typename ... Args>
        constexpr LD::CT::TypeList<Args...> GetSignatureArguments(const FunctionSignature<Return(*)(Args...) noexcept> &) noexcept
        {
            return LD::CT::TypeList<Args...>{};
        }

        template<typename Return,typename ... Args>
        constexpr LD::UInteger GetNumberOfArgumentsInFunctionSignature(const FunctionSignature<Return(*)(Args...)> &) noexcept
        {
            return LD::CT::TypeList<Args...>::size();
        }

        template<typename Return,typename ... Args>
        constexpr LD::UInteger GetNumberOfArgumentsInFunctionSignature(const FunctionSignature<Return(*)(Args...) noexcept> &) noexcept
        {
            return LD::CT::TypeList<Args...>::size();
        }

        template<typename Return, typename ... Args>
        constexpr bool IsSignatureConst(const FunctionSignature<Return(*)(Args...)> &) noexcept
        {
            return false;
        }

        template<typename Return,typename ... Args>
        constexpr bool IsSignatureConst(const FunctionSignature<Return(*)(Args...) noexcept> &) noexcept
        {
            return false;
        }

        template<typename Return, typename ... Args>
        constexpr bool IsSignatureNoExcept(const FunctionSignature<Return(*)(Args...)> &) noexcept
        {
            return false;
        }

        template<typename Return,typename ... Args>
        constexpr bool IsSignatureNoExcept(const FunctionSignature<Return(*)(Args...) noexcept> &) noexcept
        {
            return true;
        }

        template<typename Return, typename ... Args>
        constexpr bool IsSignatureMemberFunction(const FunctionSignature<Return(*)(Args...)> &) noexcept
        {
            return false;
        }

        template<typename Return,typename ... Args>
        constexpr bool IsSignatureMemberFunction(const FunctionSignature<Return(*)(Args...) noexcept> &) noexcept
        {
            return false;
        }
        //---------------------------------------end of function reflection---------------------------------------

        /////----------------------------------begining of member function reflection---------------------------------
        template<typename Return, typename Object, typename ... Args>
        constexpr bool IsFunctionalSignature(const FunctionSignature<Return(Object::*)(Args...)> &) noexcept
        {
            return true;
        }
        template<typename Return, typename Object, typename ... Args>
        constexpr bool IsFunctionalSignature(const FunctionSignature<Return(Object::*)(Args...) noexcept> &) noexcept
        {
            return true;
        }
        template<typename Return, typename Object, typename ... Args>
        constexpr bool IsFunctionalSignature(const FunctionSignature<Return(Object::*)(Args...) const> &) noexcept
        {
            return true;
        }
        template<typename Return, typename Object, typename ... Args>
        constexpr bool IsFunctionalSignature(const FunctionSignature<Return(Object::*)(Args...) const noexcept> &) noexcept
        {
            return true;
        }
        template<typename Return, typename Object, typename ... Args>
        constexpr bool IsSignatureMemberFunction(const FunctionSignature<Return(Object::*)(Args...)> &) noexcept
        {
            return true;
        }

        template<typename Return, typename Object, typename ... Args>
        constexpr bool IsSignatureMemberFunction(const FunctionSignature<Return(Object::*)(Args...) noexcept> &) noexcept
        {
            return true;
        }

        template<typename Return, typename Object, typename ... Args>
        constexpr bool IsSignatureMemberFunction(const FunctionSignature<Return(Object::*)(Args...) const> &) noexcept
        {
            return true;
        }
        template<typename Return, typename Object, typename ... Args>
        constexpr bool IsSignatureMemberFunction(const FunctionSignature<Return(Object::*)(Args...) const noexcept> &) noexcept
        {
            return true;
        }

        template<typename Return, typename Object, typename ... Args>
        constexpr LD::Type<Return> GetSignatureReturn(const FunctionSignature<Return(Object::*)(Args...)> &) noexcept
        {
            return LD::Type<Return>{};
        }


        template<typename Return, typename Object, typename ... Args>
        constexpr LD::Type<Return> GetSignatureReturn(const FunctionSignature<Return(Object::*)(Args...)  noexcept> &) noexcept
        {
            return LD::Type<Return>{};
        }

        template<typename Return, typename Object, typename ... Args>
        constexpr LD::Type<Return> GetSignatureReturn(const FunctionSignature<Return(Object::*)(Args...) const> &) noexcept
        {
            return LD::Type<Return>{};
        }

        template<typename Return, typename Object, typename ... Args>
        constexpr LD::Type<Return> GetSignatureReturn(const FunctionSignature<Return(Object::*)(Args...) const noexcept> &) noexcept
        {
            return LD::Type<Return>{};
        }

        template<typename Return, typename Object, typename ... Args>
        constexpr bool IsSignatureConst(const FunctionSignature<Return(Object::*)(Args...)> &) noexcept
        {
            return false;
        }

        template<typename Return, typename Object, typename ... Args>
        constexpr bool IsSignatureConst(const FunctionSignature<Return(Object::*)(Args...) noexcept> &) noexcept
        {
            return false;
        }

        template<typename Return, typename Object, typename ... Args>
        constexpr bool IsSignatureConst(const FunctionSignature<Return(Object::*)(Args...) const> &) noexcept
        {
            return true;
        }

        template<typename Return, typename Object, typename ... Args>
        constexpr bool IsSignatureConst(const FunctionSignature<Return(Object::*)(Args...) const noexcept> &) noexcept
        {
            return true;
        }


        template<typename Return, typename Object, typename ... Args>
        constexpr bool IsSignatureNoExcept(const FunctionSignature<Return(Object::*)(Args...)> &) noexcept
        {
            return false;
        }


        template<typename Return, typename Object, typename ... Args>
        constexpr bool IsSignatureNoExcept(const FunctionSignature<Return(Object::*)(Args...) noexcept> &) noexcept
        {
            return true;
        }

        template<typename Return, typename Object, typename ... Args>
        constexpr bool IsSignatureNoExcept(const FunctionSignature<Return(Object::*)(Args...) const > &) noexcept
        {
            return false;
        }

        template<typename Return, typename Object, typename ... Args>
        constexpr bool IsSignatureNoExcept(const FunctionSignature<Return(Object::*)(Args...) const noexcept> &) noexcept
        {
            return true;
        }

        template<typename Return, typename Object, typename ... Args>
        constexpr LD::CT::TypeList<Args...> GetSignatureArguments(const FunctionSignature<Return(Object::*)(Args...)> &) noexcept
        {
            return LD::CT::TypeList<Args...>{};
        }

        template<typename Return, typename Object, typename ... Args>
        constexpr LD::CT::TypeList<Args...> GetSignatureArguments(const FunctionSignature<Return(Object::*)(Args...) noexcept> &) noexcept
        {
            return LD::CT::TypeList<Args...>{};
        }

        template<typename Return, typename Object, typename ... Args>
        constexpr LD::CT::TypeList<Args...> GetSignatureArguments(const FunctionSignature<Return(Object::*)(Args...) const > &) noexcept
        {
            return LD::CT::TypeList<Args...>{};
        }

        template<typename Return, typename Object, typename ... Args>
        constexpr LD::CT::TypeList<Args...> GetSignatureArguments(const FunctionSignature<Return(Object::*)(Args...) const noexcept> &) noexcept
        {
            return LD::CT::TypeList<Args...>{};
        }

        template<typename Return, typename Object, typename ... Args>
        constexpr LD::UInteger GetNumberOfArgumentsInFunctionSignature(const FunctionSignature<Return(Object::*)(Args...)> &) noexcept
        {
            return LD::CT::TypeList<Args...>::size();
        }

        template<typename Return, typename Object, typename ... Args>
        constexpr LD::UInteger GetNumberOfArgumentsInFunctionSignature(const FunctionSignature<Return(Object::*)(Args...) noexcept> &) noexcept
        {
            return LD::CT::TypeList<Args...>::size();
        }

        template<typename Return, typename Object, typename ... Args>
        constexpr LD::UInteger GetNumberOfArgumentsInFunctionSignature(const FunctionSignature<Return(Object::*)(Args...) const > &) noexcept
        {
            return LD::CT::TypeList<Args...>::size();
        }

        template<typename Return, typename Object, typename ... Args>
        constexpr LD::UInteger GetNumberOfArgumentsInFunctionSignature(const FunctionSignature<Return(Object::*)(Args...) const noexcept> &) noexcept
        {
            return LD::CT::TypeList<Args...>::size();
        }

        //-----------------------------------------------end of member function reflection -----------------------------------------------

        //FunctionSignature<Return(*)(Args...)>

    }
}
namespace LD
{
    namespace CT
    {
        template<typename TFunctionSignature, TFunctionSignature tBoundFunction>
        constexpr auto SelectOverload() noexcept
        {
            return tBoundFunction;
        }
    }

}
#endif //LANDESSDEVCORE_FUNCTIONALREFLECTION_HPP
