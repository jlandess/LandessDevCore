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
#include "Core/Unit.hpp"
#include "Memory/shared_ptr.hpp"
#include "Memory/Optional.h"
#include "Configuration.hpp"
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



    template<typename ConfigurationManager>
    class BasicServiceExecutor;

    template<typename ConfigurationManager,typename ... A>
    class BasicServiceExecutor<ConfigurationManager(A...)>
    {
    private:
        BasicServiceRepository<A...> mServices;
        LD::Configuration & mConfiguration;
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

        template<typename T,typename ... C, typename ... B>
        static T AssignObject(LD::Configuration & configuration,LD::CT::TypeList<C...> types, LD::Tuple<B...> & services)
        {
            auto totalServices = LD::Tie(configuration,LD::Get<LD::Detail::Decay_T<B>>(services)...);
            auto tiedTuple  = LD::Tie(LD::Get<C>(totalServices)...);

            auto functor = [](auto && ...objects) noexcept
            {
                return T::Create(LD::Forward<decltype(objects)>(objects)...);
            };
            return LD::Invoke(functor,tiedTuple);
        }
    public:

        //BasicServiceExecutor() noexcept
        //{

        //}
        BasicServiceExecutor(LD::Configuration & config,A && ... args) noexcept:mServices{LD::Forward<A>(args)...},mConfiguration{config}
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
        T Make(LD::Type<T>) noexcept
        {
            auto signatureArguments = LD::CT::GetSignatureArguments(LD::CT::FunctionSignature<decltype(&T::Create)>{});
            using DecayedSignatureArguments = LD::CT::Tranform<decltype(signatureArguments),LD::Detail::Decay_T>;
            return AssignObject<T>(DecayedSignatureArguments {},this->mServices.Services());
        }

        template<typename T>
        LD::Optional<T> Make(LD::StringView componentName) noexcept
        {
            LD::Optional<LD::UniquePointer<Configuration,LD::MemoryResourceDeleter>> possibleComponent =  this->mConfiguration(componentName,LD::Type<LD::Configuration>{});
            if (possibleComponent)
            {
                LD::Configuration & foundSubConfiguration = (*(*possibleComponent));
                auto signatureArguments = LD::CT::GetSignatureArguments(LD::CT::FunctionSignature<decltype(&T::Create)>{});
                using DecayedSignatureArguments = LD::CT::Tranform<decltype(signatureArguments),LD::Detail::Decay_T>;
                //LD::CT::DebugTemplate<DecayedSignatureArguments>{};
                return LD::Optional<T>{AssignObject<T>(foundSubConfiguration,DecayedSignatureArguments {},this->mServices.Services())};
            }
            return LD::nullopt;
        }
        template<typename T>
        LD::Optional<T> Make(LD::Type<T>,LD::StringView componentName) noexcept
        {
            LD::Optional<LD::UniquePointer<Configuration,LD::MemoryResourceDeleter>> possibleComponent =  this->mConfiguration(componentName,LD::Type<LD::Configuration>{});
            if (possibleComponent)
            {
                LD::Configuration & foundSubConfiguration = (*(*possibleComponent));
                auto signatureArguments = LD::CT::GetSignatureArguments(LD::CT::FunctionSignature<decltype(&T::Create)>{});

                using DecayedSignatureArguments = LD::CT::Tranform<decltype(signatureArguments),LD::Detail::Decay_T>;
                return LD::Optional<T>{AssignObject<T>(foundSubConfiguration,DecayedSignatureArguments {},this->mServices.Services())};
            }
            return LD::nullopt;
        }
        template<typename T>
        LD::SharedPointer<T> MakeShared(LD::Mem::MemoryResource * resource = LD::Mem::GetNewDeleteResource()) noexcept
        {
            auto signatureArguments = LD::CT::GetSignatureArguments(LD::CT::FunctionSignature<decltype(&T::Create)>{});
            using DecayedSignatureArguments = LD::CT::Tranform<decltype(signatureArguments),LD::Detail::Decay_T>;
            return LD::AllocateShared(resource,AssignObject<T>(DecayedSignatureArguments {},this->mServices.Services()));
        }
        template<typename T>
        LD::SharedPointer<T> MakeShared(LD::Type<T>,LD::Mem::MemoryResource * resource = LD::Mem::GetNewDeleteResource()) noexcept
        {
            auto signatureArguments = LD::CT::GetSignatureArguments(LD::CT::FunctionSignature<decltype(&T::Create)>{});
            using DecayedSignatureArguments = LD::CT::Tranform<decltype(signatureArguments),LD::Detail::Decay_T>;
            return LD::AllocateShared(resource,AssignObject<T>(DecayedSignatureArguments {},this->mServices.Services()));
        }
        template<typename T>
        LD::Optional<LD::SharedPointer<T>> MakeShared(LD::StringView componentName,LD::Mem::MemoryResource * resource = LD::Mem::GetNewDeleteResource()) noexcept
        {
            LD::Optional<LD::UniquePointer<Configuration,LD::MemoryResourceDeleter>> possibleComponent =  this->mConfiguration(componentName,LD::Type<LD::Configuration>{});
            if (possibleComponent)
            {
                LD::Configuration & foundSubConfiguration = (*(*possibleComponent));
                auto signatureArguments = LD::CT::GetSignatureArguments(LD::CT::FunctionSignature<decltype(&T::Create)>{});

                using DecayedSignatureArguments = LD::CT::Tranform<decltype(signatureArguments),LD::Detail::Decay_T>;
                auto createdObject = LD::Optional<T>{AssignObject<T>(foundSubConfiguration,DecayedSignatureArguments {},this->mServices.Services())};
                return LD::Optional<LD::SharedPointer<T>>{LD::AllocateShared(resource,createdObject)};
            }
            return LD::nullopt;
        }
        template<typename T>
        LD::Optional<LD::SharedPointer<T>> MakeShared(LD::Type<T>,LD::StringView componentName,LD::Mem::MemoryResource * resource = LD::Mem::GetNewDeleteResource()) noexcept
        {
            LD::Optional<LD::UniquePointer<Configuration,LD::MemoryResourceDeleter>> possibleComponent =  this->mConfiguration(componentName,LD::Type<LD::Configuration>{});
            if (possibleComponent)
            {
                LD::Configuration & foundSubConfiguration = (*(*possibleComponent));
                auto signatureArguments = LD::CT::GetSignatureArguments(LD::CT::FunctionSignature<decltype(&T::Create)>{});

                using DecayedSignatureArguments = LD::CT::Tranform<decltype(signatureArguments),LD::Detail::Decay_T>;
                auto createdObject = LD::Optional<T>{AssignObject<T>(foundSubConfiguration,DecayedSignatureArguments {},this->mServices.Services())};
                return LD::Optional<LD::SharedPointer<T>>{LD::AllocateShared(resource,createdObject)};
            }
            return LD::nullopt;
        }
        template<typename T>
        LD::UniquePointer<T,LD::MemoryResourceDeleter> MakeUnique(LD::Mem::MemoryResource * resource = LD::Mem::GetNewDeleteResource()) noexcept
        {
            auto signatureArguments = LD::CT::GetSignatureArguments(LD::CT::FunctionSignature<decltype(&T::Create)>{});
            using DecayedSignatureArguments = LD::CT::Tranform<decltype(signatureArguments),LD::Detail::Decay_T>;
            return LD::AllocateUnique<T>(resource,AssignObject<T>(DecayedSignatureArguments {},this->mServices.Services()));
        }
        template<typename T>
        LD::UniquePointer<T,LD::MemoryResourceDeleter> MakeUnique(LD::Type<T>,LD::Mem::MemoryResource * resource = LD::Mem::GetNewDeleteResource()) noexcept
        {
            auto signatureArguments = LD::CT::GetSignatureArguments(LD::CT::FunctionSignature<decltype(&T::Create)>{});
            using DecayedSignatureArguments = LD::CT::Tranform<decltype(signatureArguments),LD::Detail::Decay_T>;
            return LD::AllocateUnique<T>(resource,AssignObject<T>(DecayedSignatureArguments {},this->mServices.Services()));
        }
        template<typename T>
        LD::Optional<LD::UniquePointer<T,LD::MemoryResourceDeleter>> MakeUnique(LD::StringView componentName,LD::Mem::MemoryResource * resource = LD::Mem::GetNewDeleteResource()) noexcept
        {
            LD::Optional<LD::UniquePointer<Configuration,LD::MemoryResourceDeleter>> possibleComponent =  this->mConfiguration(componentName,LD::Type<LD::Configuration>{});
            if(possibleComponent)
            {
                LD::Configuration & foundSubConfiguration = (*(*possibleComponent));
                auto signatureArguments = LD::CT::GetSignatureArguments(LD::CT::FunctionSignature<decltype(&T::Create)>{});

                using DecayedSignatureArguments = LD::CT::Tranform<decltype(signatureArguments),LD::Detail::Decay_T>;
                auto createdObject = LD::Optional<T>{AssignObject<T>(foundSubConfiguration,DecayedSignatureArguments {},this->mServices.Services())};
                return LD::Optional<LD::UniquePointer<T,LD::MemoryResourceDeleter>>{LD::AllocateUnique<T>(resource,createdObject)};
            }
            return LD::nullopt;
        }
        template<typename T>
        LD::Optional<LD::UniquePointer<T,LD::MemoryResourceDeleter>> MakeUnique(LD::Type<T>,LD::StringView  componentName,LD::Mem::MemoryResource * resource = LD::Mem::GetNewDeleteResource()) noexcept
        {
            LD::Optional<LD::UniquePointer<Configuration,LD::MemoryResourceDeleter>> possibleComponent =  this->mConfiguration(componentName,LD::Type<LD::Configuration>{});
            if(possibleComponent)
            {
                LD::Configuration & foundSubConfiguration = (*(*possibleComponent));
                auto signatureArguments = LD::CT::GetSignatureArguments(LD::CT::FunctionSignature<decltype(&T::Create)>{});

                using DecayedSignatureArguments = LD::CT::Tranform<decltype(signatureArguments),LD::Detail::Decay_T>;
                auto createdObject = LD::Optional<T>{AssignObject<T>(foundSubConfiguration,DecayedSignatureArguments {},this->mServices.Services())};
                return LD::Optional<LD::UniquePointer<T,LD::MemoryResourceDeleter>>{LD::AllocateUnique<T>(resource,createdObject)};
            }
            return LD::nullopt;
        }
    };
}
#endif //LANDESSDEVCORE_DEPENDENCYINJECTION_HPP
