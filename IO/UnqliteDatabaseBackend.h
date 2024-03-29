//
// Created by phoenixflower on 5/21/20.
//

#ifndef LANDESSDEVCORE_UNQLITEDATABASEBACKEND_H
#define LANDESSDEVCORE_UNQLITEDATABASEBACKEND_H
#include "Primitives/General/StringView.hpp"
#include "Memory/ElementReference.h"
#include "Primitives/General/mapboxvariant.hpp"
#include "Primitives/General/mapboxvariantvisitor.h"
#include "unqlite.h"
#include "TypeTraits/CompileTimeControlFlow.hpp"
#include "Primitives/General/ContextualVariant.h"
#include "DatabaseOperationResult.h"
#include "Functor/fixed_size_function.hpp"
#include "Primitives/General/Pair.h"
#include "DatabaseOpenMode.h"
#include "TypeTraits/IsConstructible.hpp"
#include "FetchRequest.h"
#include "TypeTraits/Conditional.hpp"
#include "Core/RequestResponse.hpp"
namespace LD
{
    template<typename T>
    class DebugTemplate;


    template<typename BufferType>
    class UnQliteBackend
    {
    public:
    private:
        LD::ElementReference<unqlite> mBackend;
    public:
        ~UnQliteBackend()
        {
            LD::IF(mBackend.GetPointer() != nullptr,[](LD::ElementReference<unqlite> backend)
            {
                unqlite * ptr = backend.GetPointer();
                unqlite_close(ptr);

            },this->mBackend);
        }

        template<typename OpenMode>
        inline UnQliteBackend(const LD::StringView & dbName, const OpenMode & mode) noexcept
        {

            LD::ContextualVariant<LD::DatabaseOpenMode(LD::ElementReference<unqlite>,LD::StringView)> context = LD::ContextualVariant<DatabaseOpenMode(LD::ElementReference<unqlite>,LD::StringView)>{LD::MakeContext(OpenMode{mode},LD::ElementReference<unqlite>{this->mBackend},LD::StringView{dbName})};

            auto openReadOnly = [](const LD::Context<OpenReadOnly,LD::ElementReference<unqlite>,LD::StringView> & context)
            {
                unqlite * ptr = LD::Get<1>(context).GetPointer();
                LD::StringView dbName = LD::Get<2>(context);
                unqlite_open(&ptr,dbName.data(),UNQLITE_OPEN_READONLY);
                return LD::ElementReference<unqlite>{ptr};
            };

            auto openReadWrite = [](const LD::Context<OpenReadAndWrite,LD::ElementReference<unqlite>,LD::StringView> & context)
            {
                unqlite * ptr = LD::Get<1>(context).GetPointer();
                LD::StringView dbName = LD::Get<2>(context);
                unqlite_open(&ptr,dbName.data(),UNQLITE_OPEN_READWRITE);
                return LD::ElementReference<unqlite>{ptr};
            };

            auto openInMemory = [](const LD::Context<OpenInMemory,LD::ElementReference<unqlite>,LD::StringView> & context)
            {
                unqlite * ptr = LD::Get<1>(context).GetPointer();
                LD::StringView dbName = LD::Get<2>(context);
                unqlite_open(&ptr,dbName.data(),UNQLITE_OPEN_IN_MEMORY);
                return LD::ElementReference<unqlite>{ptr};
            };

            auto openAndCreateIfNotExists = [](const LD::Context<OpenAndCreateIfNotExists,LD::ElementReference<unqlite>,LD::StringView> & context)
            {
                unqlite * ptr = LD::Get<1>(context).GetPointer();
                LD::StringView dbName = LD::Get<2>(context);
                unqlite_open(&ptr,dbName.data(),UNQLITE_OPEN_CREATE);
                return LD::ElementReference<unqlite>{ptr};
            };

            this->mBackend = LD::Match(context,openReadOnly,openInMemory,openReadWrite,openAndCreateIfNotExists);
        }


        template<typename ... Args>
        LD::RequestResponse<bool(Args...)> Insert(const LD::BasicStringView<BufferType> key, const LD::BasicStringView<BufferType> data, Args && ... arguments) const noexcept
        {
            unqlite * ptr = this->mBackend.GetPointer();
            LD::UInteger dbResult = unqlite_kv_store(ptr,key.data(),key.size(),data.data(),data.size());
            if (dbResult == UNQLITE_OK)
            {
                return LD::CreateResponse(LD::Type<bool>{},true,LD::Forward<Args>(arguments)...);
            }
            return LD::CreateResponse(LD::Type<bool>{},LD::TransactionError{},LD::Forward<Args>(arguments)...);
        }

        template<typename ... Args, typename F,
                typename Ret = decltype(LD::Declval<F>()(LD::Declval<LD::StringView>(),LD::Declval<StringView>(),LD::Declval<Args>()...))>
        auto Query(LD::BasicStringView<BufferType> key, F && functor, Args && ... arguments) const noexcept -> LD::RequestResponse<Ret(Args&&...)>
        {
            //using ReturnType = decltype(LD::Declval<F>()(LD::Declval<LD::StringView>(),LD::Declval<LD::StringView>()));
            using FunctorDef = LD::fixed_size_function<Ret (LD::StringView ,LD::StringView ,Args&&...)>;
            auto context = LD::MakeContext(LD::StringView {key},LD::StringView{},LD::Forward<Args>(arguments)...);

            using StorageClass = LD::Conditonal<LD::CT::IsSameWithoutQualifiers(LD::Type<void>{},LD::Type<Ret>{}),int,Ret>;
            using FetchContext = LD::Tuple<LD::Ref<FunctorDef>,LD::Ref<decltype(context)>,StorageClass>;
            auto fetchCallback = [](const void * input, unsigned int dataSize, void * inputPointer)->int
            {
                FetchContext * instance = (FetchContext*)inputPointer;
                FunctorDef  & function = LD::Get(LD::Get<0>(*instance));
                auto & context = LD::Get(LD::Get<1>(*instance));
                LD::Get(LD::Get<1>(context)) = LD::StringView{(const char*)input,dataSize};
                if constexpr (LD::CT::IsSameWithoutQualifiers(LD::Type<void>{},LD::Type<Ret>{}))
                {
                    LD::Invoke(function,context);
                }else
                {
                    LD::Get(LD::Get<2>(*instance)) = LD::Invoke(function,context);
                }

                return 0;
            };
            FetchContext currentFetchContext;
            FunctorDef function = FunctorDef{LD::Forward<F>(functor)};
            LD::Get<0>(currentFetchContext) = LD::Ref<FunctorDef>{function};
            LD::Get<1>(currentFetchContext) = LD::Ref<decltype(context)>{context};
            unqlite * ptr = this->mBackend.GetPointer();
            LD::UInteger dbResult = unqlite_kv_fetch_callback(ptr,key.data(),key.size(),fetchCallback, &currentFetchContext);
            if (dbResult == UNQLITE_OK)
            {
                return LD::CreateResponse(LD::Type<Ret>{},LD::Get(LD::Get<2>(currentFetchContext)),LD::Forward<Args>(arguments)...);
            }
            return LD::CreateResponse(LD::Type<Ret>{},LD::TransactionError{},LD::Forward<Args>(arguments)...);
        }

        template<typename ... Args,typename Ret = LD::QueryResult<bool(Args...)>>
        Ret Store(const LD::BasicStringView<BufferType> & key, const LD::BasicStringView<BufferType> & data, Args && ... arguements) const noexcept
        {
            unqlite * ptr = this->mBackend.GetPointer();
            LD::UInteger dbResult = unqlite_kv_store(ptr,key.data(),key.size(),data.data(),data.size());
            Ret results[2];
            results[0] = LD::MakeContext(LD::TransactionError{},LD::Forward<Args>(arguements)...);
            results[1] = LD::MakeContext(LD::TransactionResult{},bool{true},LD::Forward<Args>(arguements)...);
            return results[dbResult == UNQLITE_OK];
        }
        template<typename F, typename ... Args,
                typename FunctorRet = decltype(LD::Declval<F>()(LD::Declval<LD::Context<LD::StringView,LD::StringView,Args...>>())),
                typename Ret = LD::QueryResult<FunctorRet(LD::StringView,Args...)>>
        LD::Enable_If_T<
        LD::Require<
                LD::Negate<LD::Detail::IsSame<void,FunctorRet>::value>
        >
        ,
        Ret> Fetch(const LD::BasicStringView<BufferType> & key, F && functor, Args && ... arguments) const noexcept
        {
            using UsableContext = LD::Context<LD::StringView,LD::StringView ,Args...>;
            using FunctorDef = LD::fixed_size_function<FunctorRet(const UsableContext &)>;
            using DecayedRet = LD::Detail::Decay_T<FunctorRet>;
            using FetchContext = LD::Tuple<LD::Ref<FunctorDef>,LD::Ref<UsableContext>,DecayedRet>;
            UsableContext context = LD::MakeContext(LD::StringView{key},LD::StringView{},LD::Forward<Args>(arguments)...);
            FunctorDef function = FunctorDef{LD::Forward<F>(functor)};
            FetchContext currentFetchContext;
            LD::Get<0>(currentFetchContext) = LD::Ref<FunctorDef>{function};
            LD::Get<1>(currentFetchContext) = LD::Ref<UsableContext>{context};
            unqlite * ptr = this->mBackend.GetPointer();
            auto fetchCallback = [](const void * input, unsigned int dataSize, void * inputPointer)->int
            {
                FetchContext * instance = (FetchContext *)inputPointer;
                FunctorDef  & function = LD::Get(LD::Get<0>(*instance));
                UsableContext & context = LD::Get(LD::Get<1>(*instance));
                LD::Get<1>(context) = LD::StringView {(char*)input,dataSize};
                FunctorRet & functorResult = LD::Get(LD::Get<2>(*instance));

                functorResult = function(context);
                return 0;
            };
            LD::UInteger dbResult = unqlite_kv_fetch_callback(ptr,key.data(),key.size(),fetchCallback, &currentFetchContext);
            Ret results[2];
            results[0] = LD::MakeContext(LD::TransactionError{},LD::StringView{key},LD::Forward<Args>(arguments)...);

            FunctorRet & result = LD::Get<2>(currentFetchContext);
            results[1] = LD::MakeContext(LD::TransactionResult{},FunctorRet{result},LD::StringView{key},LD::Forward<Args>(arguments)...);
            return results[dbResult == UNQLITE_OK];
        }


        template<typename ... Args, typename Ret = LD::ContextualVariant<LD::Variant<LD::TransactionError,LD::DatabaseTransactionResult>(Args...)>>
        Ret Begin(Args && ... arguements) const noexcept
        {
            unqlite * ptr = this->mBackend.GetPointer();
            LD::UInteger result = unqlite_begin(ptr);
            LD::ContextualVariant<LD::Variant<LD::TransactionError,LD::DatabaseTransactionResult>(Args...)> results[2];
            results[0] = LD::MakeContext(LD::TransactionError{},LD::Forward<Args>(arguements)...);
            results[1] = LD::MakeContext(LD::DatabaseTransactionResult{},LD::Forward<Args>(arguements)...);
            return results[result == UNQLITE_OK];
        }

        template<typename ... Args, typename Ret = LD::ContextualVariant<LD::Variant<LD::TransactionError,LD::DatabaseTransactionResult>(Args...)>>
        Ret  Commit(Args && ... arguements) const noexcept
        {
            unqlite * ptr = this->mBackend.GetPointer();
            LD::UInteger result = unqlite_commit(ptr);
            LD::ContextualVariant<LD::Variant<LD::TransactionError,LD::DatabaseTransactionResult>(Args...)> results[2];
            results[0] = LD::MakeContext(LD::TransactionError{},LD::Forward<Args>(arguements)...);
            results[1] = LD::MakeContext(LD::DatabaseTransactionResult{},LD::Forward<Args>(arguements)...);
            return results[result == UNQLITE_OK];
        }

        template<typename ... Args, typename Ret = LD::QueryResult<bool(Args...)>>
        Ret Remove(const LD::BasicStringView<BufferType> & key, Args && ... arguments)
        {
            LD::UInteger result = unqlite_kv_delete(this->mBackend.GetPointer(),key.data(),key.size());
            Ret queryResult[2];
            queryResult[0] = LD::MakeContext(LD::TransactionError{},LD::Forward<Args>(arguments)...);
            queryResult[1] = LD::MakeContext(LD::TransactionResult{},bool{(result == UNQLITE_OK)},LD::Forward<Args>(arguments)...);
            return queryResult[result == UNQLITE_OK];
        }


    };
}
#endif //LANDESSDEVCORE_UNQLITEDATABASEBACKEND_H
