//
// Created by phoenixflower on 9/27/21.
//

#ifndef LANDESSDEVCORE_DEPENDENCYINJECTION_HPP
#define LANDESSDEVCORE_DEPENDENCYINJECTION_HPP
#include "Primitives/General/Tuple.hpp"
#include "Memory/MemoryResource.hpp"
#include "Memory/PolyMorphicAllocator.hpp"
#include "Memory/UniquePointer.h"
#include "ServiceLocator.hpp"
#include "Primitives/General/Unit.hpp"
#include "Memory/shared_ptr.hpp"
namespace LD
{

    template<typename T>
    class DependencyInjection;
    template<typename T,typename ... A>
    class DependencyInjection<T(A...)>
    {
    public:
        static T  Create(A & ... injections) noexcept
        {
            return T{injections...};
        }
    };




    template<typename ... A>
    class BasicServiceExecutor
    {
    private:
        BasicServiceRepository<A...> mServices;
        //we need a factory pattern member here, as we're assuming all types asked for have a "Create" function, this assumption should be made to be more generic
        template<typename T,typename ... C, typename ... B>
        static T AssignObject(LD::CT::TypeList<C...> types, LD::Tuple<B...> & services)
        {
            auto tiedTuple  = LD::Tie(LD::Get<C>(services)...);

            //return LD::InvokeConstruction<T>(tiedTuple);
            auto functor = [](auto && ...objects) noexcept
            {
                return T::Create(LD::Forward<decltype(objects)>(objects)...);
            };
            return LD::Invoke(functor,tiedTuple);
        }
    public:

        BasicServiceExecutor() noexcept
        {

        }
        BasicServiceExecutor(A && ... args) noexcept:mServices{LD::Forward<A>(args)...}
        {

        }
        LD::Second<LD::Float> operator()() noexcept
        {
            return LD::Second<LD::Float>{};
        }
        template<typename T>
        T Make() noexcept
        {
            auto signatureArguments = LD::CT::GetSignatureArguments(LD::CT::FunctionSignature<decltype(&T::Create)>{});
            using DecayedSignatureArguments = LD::CT::Tranform<decltype(signatureArguments),LD::Detail::Decay_T>;
            return AssignObject<T>(DecayedSignatureArguments {},this->mServices.Services());
        }
        template<typename T>
        LD::SharedPointer<T> MakeShared(LD::Mem::MemoryResource * resource = LD::Mem::GetNewDeleteResource()) noexcept
        {
            auto signatureArguments = LD::CT::GetSignatureArguments(LD::CT::FunctionSignature<decltype(&T::Create)>{});
            using DecayedSignatureArguments = LD::CT::Tranform<decltype(signatureArguments),LD::Detail::Decay_T>;
            return LD::AllocateShared(resource,AssignObject<T>(DecayedSignatureArguments {},this->mServices.Services()));
        }
        template<typename T>
        LD::UniquePointer<T,LD::MemoryResourceDeleter> MakeUnique(LD::Mem::MemoryResource * resource = LD::Mem::GetNewDeleteResource()) noexcept
        {
            auto signatureArguments = LD::CT::GetSignatureArguments(LD::CT::FunctionSignature<decltype(&T::Create)>{});
            using DecayedSignatureArguments = LD::CT::Tranform<decltype(signatureArguments),LD::Detail::Decay_T>;
            return LD::AllocateUnique<T>(resource,AssignObject<T>(DecayedSignatureArguments {},this->mServices.Services()));
        }
    };
}
#endif //LANDESSDEVCORE_DEPENDENCYINJECTION_HPP
