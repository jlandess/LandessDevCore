//
// Created by phoenixflower on 7/7/20.
//

#ifndef LANDESSDEVCORE_FUNCTIONALREFLECTION_HPP
#define LANDESSDEVCORE_FUNCTIONALREFLECTION_HPP
#include "TypeList.hpp"
#include "Type.h"
namespace LD
{
    namespace CT
    {
        template<typename Ret>
        struct FunctionSignature
        {
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

        //-------------------------------------beginning of function reflection -----------------------------------
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

        //-----------------------------------------------end of member function reflection -----------------------------------------------

        //FunctionSignature<Return(*)(Args...)>

    }
}
#endif //LANDESSDEVCORE_FUNCTIONALREFLECTION_HPP
