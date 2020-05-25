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
#include "Algorithms/CompileTimeControlFlow.hpp"
#include "Primitives/General/ContextualVariant.h"
#include "DatabaseOperationResult.h"
#include "Functor/fixed_size_function.hpp"
#include "Primitives/General/Pair.h"
#include "DatabaseOpenMode.h"
namespace LD
{



    template<typename T>
    class DebugTemplate;


    class UnQliteBackend
    {
    public:
    private:
        LD::ElementReference<unqlite> mBackend;
    public:
        UnQliteBackend() noexcept :mBackend(nullptr)
        {

        }

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

            //LD::ElementReference<unqlite> db = LD::Match(context,openReadOnly,openInMemory,openReadWrite,openAndCreateIfNotExists);
            //unqlite * ptr = this->mBackend.GetPointer();
            //unqlite_open(&ptr,dbName.data(),UNQLITE_OPEN_CREATE);
        }


        template<typename ... Args,typename Ret = LD::ContextualVariant<LD::Variant<LD::DatabaseError,LD::DatabaseTransactionResult>(Args...)>>
        LD::Variant<LD::DatabaseError,LD::DatabaseInsertResult> Store(const LD::StringView & key, const LD::StringView & data, Args && ... arguements) const noexcept
        {
            unqlite * ptr = this->mBackend.GetPointer();
            LD::UInteger dbResult = unqlite_kv_store(ptr,key.data(),key.size(),data.data(),data.size());
            Ret results[2];
            results[0] = LD::MakeContext(LD::DatabaseError{},LD::Forward<Args>(arguements)...);
            results[1] = LD::MakeContext(LD::DatabaseTransactionResult{},LD::Forward<Args>(arguements)...);
            return results[dbResult == UNQLITE_OK];
        }

        template<typename F, typename ... Args,typename Ret = LD::ContextualVariant<LD::Variant<LD::DatabaseError,LD::DatabaseTransactionResult>(Args...)>>
        Ret Fetch(LD::StringView & key, F && functor, Args && ... arguments) const noexcept
        {
            typedef LD::Pair<LD::fixed_size_function<void(const LD::Context<LD::StringView,LD::StringView,Args...> &)>,LD::Context<Args...>> FetchContext;
            FetchContext pair;
            auto fetchCallback = [](const void * input, unsigned int dataSize, void * inputPointer)->int
            {
                FetchContext * instance = (FetchContext *)inputPointer;
                LD::Get<1>(instance->GetSecond()) = LD::StringView{(char*)input,dataSize};
                instance->GetFirst()(instance->GetSecond());
                return 0;
            };
            pair.GetFirst() = LD::fixed_size_function<void(const LD::StringView&,const LD::StringView &, const LD::Context<Args...> &)>{functor};
            pair.GetSecond() = LD::MakeContext(LD::StringView {key},LD::StringView {},LD::Forward<Args>(arguments)...);
            unqlite * ptr = this->mBackend.GetPointer();
            LD::UInteger dbResult = unqlite_kv_fetch_callback(ptr,key.data(),key.size(),fetchCallback, &pair);

            Ret results[2];
            results[0] = LD::MakeContext(LD::DatabaseError{},LD::Forward<Args>(arguments)...);
            results[1] = LD::MakeContext(LD::DatabaseTransactionResult{},LD::Forward<Args>(arguments)...);
            return results[dbResult == UNQLITE_OK];
        }


        template<typename ... Args, typename Ret = LD::ContextualVariant<LD::Variant<LD::DatabaseError,LD::DatabaseTransactionResult>(Args...)>>
        Ret Begin(Args && ... arguements) const noexcept
        {
            unqlite * ptr = this->mBackend.GetPointer();
            LD::UInteger result = unqlite_begin(ptr);
            LD::ContextualVariant<LD::Variant<LD::DatabaseError,LD::DatabaseTransactionResult>(Args...)> results[2];
            results[0] = LD::MakeContext(LD::DatabaseError{},LD::Forward<Args>(arguements)...);
            results[1] = LD::MakeContext(LD::DatabaseTransactionResult{},LD::Forward<Args>(arguements)...);
            return results[result == UNQLITE_OK];
        }

        template<typename ... Args, typename Ret = LD::ContextualVariant<LD::Variant<LD::DatabaseError,LD::DatabaseTransactionResult>(Args...)>>
        Ret  Commit(Args && ... arguements) const noexcept
        {
            unqlite * ptr = this->mBackend.GetPointer();
            LD::UInteger result = unqlite_commit(ptr);
            LD::ContextualVariant<LD::Variant<LD::DatabaseError,LD::DatabaseTransactionResult>(Args...)> results[2];
            results[0] = LD::MakeContext(LD::DatabaseError{},LD::Forward<Args>(arguements)...);
            results[1] = LD::MakeContext(LD::DatabaseTransactionResult{},LD::Forward<Args>(arguements)...);
            return results[result == UNQLITE_OK];
        }
    };
}
#endif //LANDESSDEVCORE_UNQLITEDATABASEBACKEND_H
