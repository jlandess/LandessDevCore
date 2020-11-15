//
// Created by phoenixflower on 10/7/20.
//

#ifndef LANDESSDEVCORE_ROCKSBACKINGSTORE_H
#define LANDESSDEVCORE_ROCKSBACKINGSTORE_H
#include "Primitives/General/StringView.hpp"
#include <rocksdb/db.h>
#include "TypeTraits/Detection.hpp"
#include "FetchRequest.h"
namespace LD
{
    class RocksBackingStore
    {
    private:
        rocksdb::DB* db;
    public:
        RocksBackingStore(LD::StringView location) noexcept
        {
            rocksdb::Options options;
            options.create_if_missing = true;
            options.OptimizeLevelStyleCompaction();
            std::string kDBPath = "/tmp/rocksdb_simple_example";
            rocksdb::Status status = rocksdb::DB::Open(options, location.data(),&db);
        }

        template<typename ... Args,typename Ret = LD::QueryResult<bool(Args...)>>
        Ret Store(const LD::StringView & key, const LD::StringView & data, Args && ... arguements) const noexcept
        {
            rocksdb::Status status = db->Put(rocksdb::WriteOptions(), key.data(), data.data());
            if (status.ok())
            {
                return LD::MakeContext(LD::TransactionResult{},true,LD::Forward<Args>(arguements)...);
            }
            return LD::MakeContext(LD::TransactionError{},LD::Forward<Args>(arguements)...);
        }
        template<typename F, typename ... Args,
                typename FunctorRet = decltype(LD::Declval<F>()(LD::Declval<LD::Context<LD::StringView,LD::StringView,Args...>>())),
                typename Ret = LD::QueryResult<FunctorRet(LD::StringView,Args...)>>
        LD::Enable_If_T<
                LD::Require<
                        LD::Negate<LD::Detail::IsSame<void,FunctorRet>::value>
                >
                ,
                Ret> Fetch(const LD::StringView & key, F && functor, Args && ... arguments) const noexcept
        {
            std::string value;
            rocksdb::Status s = db->Get(rocksdb::ReadOptions(), key.data(), &value);
            if (s.ok())
            {
                auto result = functor(LD::MakeContext(LD::StringView{key},LD::StringView{value.c_str(),value.size()},LD::Forward<Args>(arguments)...));
                return LD::MakeContext(LD::TransactionResult{},FunctorRet{result},LD::StringView{key},LD::Forward<Args>(arguments)...);

            }
            return LD::MakeContext(LD::TransactionError{},LD::StringView{key},LD::Forward<Args>(arguments)...);
        }
        ~RocksBackingStore()
        {
            delete db;
        }
    };
}
#endif //LANDESSDEVCORE_ROCKSBACKINGSTORE_H
