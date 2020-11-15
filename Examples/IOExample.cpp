//
// Created by phoenixflower on 11/14/20.
//
#include "Algorithms/Benchmark.hpp"
#include "Primitives/General/StringView.hpp"
#include "IO/S3DatabaseBackend.h"
#include "IO/EtcdBackingStore.h"
#include "IO/etcd/etcdcpp.h"
#include "IO/RedisBackend.h"
#include "IO/Database.hpp"
#include "Chrono/Timer.h"
#include "IO/UnqliteDatabaseBackend.h"
#include "IO/RocksBackingStore.h"
#include "IO/JsonDatabaseBackend.h"
#include "ReflectionDemoTypes.h"
#include "IO/toml.hpp"
namespace LD
{

    void IOExample()
    {
        Benchmark abc{LD::StringView {"abc"},0,LD::Second<LD::Float>{}};
        LD::Timer currentTimer;
        LD::Milisecond<LD::Float> timeElapsed;
        //------------------------------------------------------------Client Declarations-----------------------------------------------------------------------------
        LD::S3BackingStore client{"192.168.149.170:9010","pictures1234",LD::S3Credentials{"AKIAIOSFODNN7EXAMPLE","wJalrXUtnFEMI/K7MDENG/bPxRfiCYEXAMPLEKEY"}};
        etcdcpp::etcd_host etcdHost{"192.168.149.170",2379};
        LD::RedisBackingStore redisBackingStore{"192.168.149.170",55000};
        etcdcpp::etcd_session session{{etcdHost}};
        LD::UnQliteBackend<char> unqliteBackingStore{"database.db",LD::OpenAndCreateIfNotExists{}};
        LD::RocksBackingStore rocksBackingStore{"/tmp/abcrocks"};
        //-----------------------------------------------------------End of Client Declarations----------------------------------------------------------------------
        //-----------------------------------------------------------Adapter Declarations------------------------------------------------------------------------------
        LD::BasicKeyedDatabase<LD::S3BackingStore> s3BackingKeyedDatabase{client};
        LD::BasicKeyedDatabase<LD::RedisBackingStore> redisKeyedDatabase{redisBackingStore};
        LD::BasicKeyedDatabase<LD::UnQliteBackend<char>> unqliteKeyDatabase{unqliteBackingStore};
        LD::BasicKeyedDatabase<LD::RocksBackingStore> rocksdbKeyedDatabase{rocksBackingStore};
        LD::BasicKeyedDatabase<etcdcpp::etcd_session> etcdKeyedDatabase{session};
        //-----------------------------------------------------------End of Adapter Declarations----------------------------------------------------------------------
        //-----------------------------------------------------------Etcd Serialization Test--------------------------------------------------------------------------
        //session.Store(LD::StringView {"/message/waffles"},LD::StringView{"cde"});

        timeElapsed = LD::TimedExecution(currentTimer,[](LD::BasicKeyedDatabase<etcdcpp::etcd_session> & db) noexcept{
            //db.Insert("abc"_ts,LD::Pyramid{LD::Square{922},LD::Triangle{72.48,92.73}});
            //db.Store(LD::StringView {"waffles.memes"},LD::StringView{"cde"});
            db.Insert("abc"_ts,LD::Pyramid{LD::Square{922},LD::Triangle{72.48,92.73}});
        },etcdKeyedDatabase);

        std::cout << "etcd Insertion execution time : " << timeElapsed.ToImmutableString().Data() << " ms" << std::endl;
        timeElapsed = TimedExecution(currentTimer,[](LD::BasicKeyedDatabase<etcdcpp::etcd_session> & db) noexcept
        {
            std::cout << "Etcd Fetch Qualification " << std::endl;
            auto result =  db.Fetch("abc"_ts,LD::CT::TypeList<LD::Pyramid>{});
            auto onFetch = [](const LD::Context<LD::TransactionResult,LD::Pyramid> & context) noexcept
            {
                LD::Pyramid & pyramid = LD::Get(LD::Get<1>(context));
                std::cout << pyramid.Side().Base() << std::endl;
                std::cout << pyramid.Side().Height() << std::endl;
                std::cout << pyramid.Base().Length() << std::endl;
            };


            auto onFetchError = [](auto) noexcept
            {

            };
            LD::Match(result,onFetch,onFetchError);
        },etcdKeyedDatabase);

        std::cout << "etcd Fetch execution time : " << timeElapsed.ToImmutableString().Data() << " ms" << std::endl;
        //----------------------------------------------------------End Of Etcd Serialization Test---------------------------------------------------------------------
        //---------------------------------------------- Redis Serialization Test -------------------------------------------------------------------
        timeElapsed = LD::TimedExecution(currentTimer,[](LD::BasicKeyedDatabase<LD::RedisBackingStore> & db) noexcept{db.Insert("abc"_ts,LD::Pyramid{LD::Square{922},LD::Triangle{72.48,92.73}});},redisKeyedDatabase);

        std::cout << "Redis Insertion execution time : " << timeElapsed.ToImmutableString().Data() << " ms" << std::endl;

        timeElapsed = TimedExecution(currentTimer,[](LD::BasicKeyedDatabase<LD::RedisBackingStore> & db) noexcept
        {
            std::cout << "Redis Fetch Qualification " << std::endl;
            auto result =  db.Fetch("abc"_ts,LD::CT::TypeList<LD::Pyramid>{});
            auto onFetch = [](const LD::Context<LD::TransactionResult,LD::Pyramid> & context) noexcept
            {
                LD::Pyramid & pyramid = LD::Get(LD::Get<1>(context));
                std::cout << pyramid.Side().Base() << std::endl;
                std::cout << pyramid.Side().Height() << std::endl;
                std::cout << pyramid.Base().Length() << std::endl;
            };


            auto onFetchError = [](auto) noexcept
            {

            };
            LD::Match(result,onFetch,onFetchError);
            std::cout << "End of Redis Fetch Qualification" << std::endl;
        },redisKeyedDatabase);

        std::cout << "Redis fetch execution time : " << timeElapsed.ToImmutableString().Data() << " ms" << std::endl;
        //--------------------------------------------------------End of Redis Serialization Test ---------------------------------------------------------------------------------------------
        //-------------------------------------------------------- S3 Backing Store Test --------------------------------------------------------------------------------
        timeElapsed = TimedExecution(currentTimer,[](LD::BasicKeyedDatabase<LD::S3BackingStore> & db) noexcept
        {
            db.Insert("abc"_ts,LD::Pyramid{LD::Square{922},LD::Triangle{72.48,92.73}});
        },s3BackingKeyedDatabase);
        std::cout << "S3 Insertion  : " << timeElapsed.ToImmutableString().Data() << " ms" << std::endl;

        timeElapsed = TimedExecution(currentTimer,[](LD::BasicKeyedDatabase<LD::S3BackingStore> & db) noexcept
        {
            auto result =  db.Fetch("abc"_ts,LD::CT::TypeList<LD::Pyramid>{});
            auto onFetch = [](const LD::Context<LD::TransactionResult,LD::Pyramid> & context) noexcept
            {
                LD::Pyramid & pyramid = LD::Get(LD::Get<1>(context));
                std::cout << pyramid.Side().Base() << std::endl;
                std::cout << pyramid.Side().Height() << std::endl;
                std::cout << pyramid.Base().Length() << std::endl;
            };

            auto onFetchError = [](auto) noexcept
            {

            };
            LD::Match(result,onFetch,onFetchError);
        },s3BackingKeyedDatabase);
        std::cout << "S3 fetched elapsed time : " << currentTimer.GetElapsedTimeInMilliSec() << " ms" << std::endl;
        //-------------------------------------------------------End of S3 Backing Store Test-----------------------------------------------------------------------
        timeElapsed = TimedExecution(currentTimer,[](LD::BasicKeyedDatabase<LD::UnQliteBackend<char>> & db) noexcept
        {
            db.Insert("abc"_ts,LD::Pyramid{LD::Square{922},LD::Triangle{72.48,92.73}});
        },unqliteKeyDatabase);
        std::cout << "UnQlite Insertion time : " << timeElapsed.ToImmutableString().Data() << " ms" << std::endl;

        timeElapsed = TimedExecution(currentTimer,[](LD::BasicKeyedDatabase<LD::UnQliteBackend<char>> & db) noexcept
        {
            auto result =  db.Fetch("abc"_ts,LD::CT::TypeList<LD::Pyramid>{});
            auto onFetch = [](const LD::Context<LD::TransactionResult,LD::Pyramid> & context) noexcept
            {
                LD::Pyramid & pyramid = LD::Get(LD::Get<1>(context));
                std::cout << pyramid.Side().Base() << std::endl;
                std::cout << pyramid.Side().Height() << std::endl;
                std::cout << pyramid.Base().Length() << std::endl;
            };

            auto onFetchError = [](auto) noexcept
            {

            };
            LD::Match(result,onFetch,onFetchError);
        },unqliteKeyDatabase);
        std::cout << "Unqlite fetched elapsed time : " << currentTimer.GetElapsedTimeInMilliSec() << " ms" << std::endl;
        //---------------------------------------------Beginning of RocksDb Tests------------------------------------------------------------------
        timeElapsed = TimedExecution(currentTimer,[](LD::BasicKeyedDatabase<LD::RocksBackingStore> & db) noexcept
        {
            db.Insert("abc"_ts,LD::Pyramid{LD::Square{922},LD::Triangle{72.48,92.73}});
        },rocksdbKeyedDatabase);
        std::cout << "Rocksdb Insertion time : " << timeElapsed.ToImmutableString().Data() << " ms" << std::endl;

        timeElapsed = TimedExecution(currentTimer,[](LD::BasicKeyedDatabase<LD::RocksBackingStore> & db) noexcept
        {
            auto result =  db.Fetch("abc"_ts,LD::CT::TypeList<LD::Pyramid>{});
            auto onFetch = [](const LD::Context<LD::TransactionResult,LD::Pyramid> & context) noexcept
            {
                LD::Pyramid & pyramid = LD::Get(LD::Get<1>(context));
                std::cout << pyramid.Side().Base() << std::endl;
                std::cout << pyramid.Side().Height() << std::endl;
                std::cout << pyramid.Base().Length() << std::endl;
            };

            auto onFetchError = [](auto) noexcept
            {

            };
            LD::Match(result,onFetch,onFetchError);
        },rocksdbKeyedDatabase);
        std::cout << "Rocksdb fetched elapsed time : " << currentTimer.GetElapsedTimeInMilliSec() << " ms" << std::endl;
        //---------------------------------------------------End of RocksDb Tests------------------------------------------------------------------

        nlohmann::json output;

        nlohmann::json firstBench;
        firstBench["Bench"]["Name"] = "Insertion";
        firstBench["Bench"]["ElapsedTime"] = LD::ToImmutableString(currentTimer.GetElapsedTimeInMilliSec()).Data();
        output["Marks"].push_back(firstBench);
        output["Marks"].push_back(firstBench);
        std::cout << output.dump(1) << std::endl;

        //auto tbl = toml::table{};
        //tbl["a"]["b"] = "abc";
        //std::cout << toml::parse("");
        toml::basic_value<toml::discard_comments> value;
        value["a"]["b"] = "abc";
        //value["MyPyramid"] = "Pyramid";
        value["MyPyramid"]["Base"]["Pyramid.Base"] = "Square";
        value["MyPyramid"]["Base"]["Length"] = "17";
        value["MyPyramid"]["Side"]["Pyramid.Side"] = "Triangle";
        value["MyPyramid"]["Side"]["Width"] = "2";
        value["MyPyramid"]["Side"]["Length"] = "2";
        value["MyPyramid"]["ClassName"] = "Pyramid";
        std::cout << value << std::endl;
    }



    /*
int main()
{
    Benchmark abc{LD::StringView {"abc"},0,LD::Second<LD::Float>{}};
    LD::Timer currentTimer;
    LD::Milisecond<LD::Float> timeElapsed;
    //------------------------------------------------------------Client Declarations-----------------------------------------------------------------------------
    LD::S3BackingStore client{"192.168.149.170:9010","pictures1234",LD::S3Credentials{"AKIAIOSFODNN7EXAMPLE","wJalrXUtnFEMI/K7MDENG/bPxRfiCYEXAMPLEKEY"}};
    etcdcpp::etcd_host etcdHost{"192.168.149.170",2379};
    LD::RedisBackingStore redisBackingStore{"192.168.149.170",55000};
    etcdcpp::etcd_session session{{etcdHost}};
    LD::UnQliteBackend<char> unqliteBackingStore{"database.db",LD::OpenAndCreateIfNotExists{}};
    LD::RocksBackingStore rocksBackingStore{"/tmp/abcrocks"};
    //-----------------------------------------------------------End of Client Declarations----------------------------------------------------------------------
    //-----------------------------------------------------------Adapter Declarations------------------------------------------------------------------------------
    LD::BasicKeyedDatabase<LD::S3BackingStore> s3BackingKeyedDatabase{client};
    LD::BasicKeyedDatabase<LD::RedisBackingStore> redisKeyedDatabase{redisBackingStore};
    LD::BasicKeyedDatabase<LD::UnQliteBackend<char>> unqliteKeyDatabase{unqliteBackingStore};
    LD::BasicKeyedDatabase<LD::RocksBackingStore> rocksdbKeyedDatabase{rocksBackingStore};
    LD::BasicKeyedDatabase<etcdcpp::etcd_session> etcdKeyedDatabase{session};
    //-----------------------------------------------------------End of Adapter Declarations----------------------------------------------------------------------
    //-----------------------------------------------------------Etcd Serialization Test--------------------------------------------------------------------------
    //session.Store(LD::StringView {"/message/waffles"},LD::StringView{"cde"});

    timeElapsed = TimedExecution(currentTimer,[](LD::BasicKeyedDatabase<etcdcpp::etcd_session> & db) noexcept
    {
        //db.Insert("abc"_ts,LD::Pyramid{LD::Square{922},LD::Triangle{72.48,92.73}});
        //db.Store(LD::StringView {"waffles.memes"},LD::StringView{"cde"});
        db.Insert("abc"_ts,LD::Pyramid{LD::Square{922},LD::Triangle{72.48,92.73}});
    },etcdKeyedDatabase);

    std::cout << "etcd Insertion execution time : " << timeElapsed.ToImmutableString().Data() << " ms" << std::endl;
    timeElapsed = TimedExecution(currentTimer,[](LD::BasicKeyedDatabase<etcdcpp::etcd_session> & db) noexcept
    {
        std::cout << "Etcd Fetch Qualification " << std::endl;
        auto result =  db.Fetch("abc"_ts,LD::CT::TypeList<LD::Pyramid>{});
        auto onFetch = [](const LD::Context<LD::TransactionResult,LD::Pyramid> & context) noexcept
        {
            LD::Pyramid & pyramid = LD::Get(LD::Get<1>(context));
            std::cout << pyramid.Side().Base() << std::endl;
            std::cout << pyramid.Side().Height() << std::endl;
            std::cout << pyramid.Base().Length() << std::endl;
        };


        auto onFetchError = [](auto) noexcept
        {

        };
        LD::Match(result,onFetch,onFetchError);
    },etcdKeyedDatabase);

    std::cout << "etcd Fetch execution time : " << timeElapsed.ToImmutableString().Data() << " ms" << std::endl;
    //----------------------------------------------------------End Of Etcd Serialization Test---------------------------------------------------------------------
    //---------------------------------------------- Redis Serialization Test -------------------------------------------------------------------
    timeElapsed = TimedExecution(currentTimer,[](LD::BasicKeyedDatabase<LD::RedisBackingStore> & db) noexcept
    {
        db.Insert("abc"_ts,LD::Pyramid{LD::Square{922},LD::Triangle{72.48,92.73}});
    },redisKeyedDatabase);

    std::cout << "Redis Insertion execution time : " << timeElapsed.ToImmutableString().Data() << " ms" << std::endl;

    timeElapsed = TimedExecution(currentTimer,[](LD::BasicKeyedDatabase<LD::RedisBackingStore> & db) noexcept
    {
        std::cout << "Redis Fetch Qualification " << std::endl;
        auto result =  db.Fetch("abc"_ts,LD::CT::TypeList<LD::Pyramid>{});
        auto onFetch = [](const LD::Context<LD::TransactionResult,LD::Pyramid> & context) noexcept
        {
            LD::Pyramid & pyramid = LD::Get(LD::Get<1>(context));
            std::cout << pyramid.Side().Base() << std::endl;
            std::cout << pyramid.Side().Height() << std::endl;
            std::cout << pyramid.Base().Length() << std::endl;
        };


        auto onFetchError = [](auto) noexcept
        {

        };
        LD::Match(result,onFetch,onFetchError);
        std::cout << "End of Redis Fetch Qualification" << std::endl;
    },redisKeyedDatabase);

    std::cout << "Redis fetch execution time : " << timeElapsed.ToImmutableString().Data() << " ms" << std::endl;
    //--------------------------------------------------------End of Redis Serialization Test ---------------------------------------------------------------------------------------------
    //-------------------------------------------------------- S3 Backing Store Test --------------------------------------------------------------------------------
    timeElapsed = TimedExecution(currentTimer,[](LD::BasicKeyedDatabase<LD::S3BackingStore> & db) noexcept
    {
        db.Insert("abc"_ts,LD::Pyramid{LD::Square{922},LD::Triangle{72.48,92.73}});
    },s3BackingKeyedDatabase);
    std::cout << "S3 Insertion  : " << timeElapsed.ToImmutableString().Data() << " ms" << std::endl;

    timeElapsed = TimedExecution(currentTimer,[](LD::BasicKeyedDatabase<LD::S3BackingStore> & db) noexcept
    {
        auto result =  db.Fetch("abc"_ts,LD::CT::TypeList<LD::Pyramid>{});
        auto onFetch = [](const LD::Context<LD::TransactionResult,LD::Pyramid> & context) noexcept
        {
            LD::Pyramid & pyramid = LD::Get(LD::Get<1>(context));
            std::cout << pyramid.Side().Base() << std::endl;
            std::cout << pyramid.Side().Height() << std::endl;
            std::cout << pyramid.Base().Length() << std::endl;
        };

        auto onFetchError = [](auto) noexcept
        {

        };
        LD::Match(result,onFetch,onFetchError);
    },s3BackingKeyedDatabase);
    std::cout << "S3 fetched elapsed time : " << currentTimer.GetElapsedTimeInMilliSec() << " ms" << std::endl;
    //-------------------------------------------------------End of S3 Backing Store Test-----------------------------------------------------------------------
    timeElapsed = TimedExecution(currentTimer,[](LD::BasicKeyedDatabase<LD::UnQliteBackend<char>> & db) noexcept
    {
        db.Insert("abc"_ts,LD::Pyramid{LD::Square{922},LD::Triangle{72.48,92.73}});
    },unqliteKeyDatabase);
    std::cout << "UnQlite Insertion time : " << timeElapsed.ToImmutableString().Data() << " ms" << std::endl;

    timeElapsed = TimedExecution(currentTimer,[](LD::BasicKeyedDatabase<LD::UnQliteBackend<char>> & db) noexcept
    {
        auto result =  db.Fetch("abc"_ts,LD::CT::TypeList<LD::Pyramid>{});
        auto onFetch = [](const LD::Context<LD::TransactionResult,LD::Pyramid> & context) noexcept
        {
            LD::Pyramid & pyramid = LD::Get(LD::Get<1>(context));
            std::cout << pyramid.Side().Base() << std::endl;
            std::cout << pyramid.Side().Height() << std::endl;
            std::cout << pyramid.Base().Length() << std::endl;
        };

        auto onFetchError = [](auto) noexcept
        {

        };
        LD::Match(result,onFetch,onFetchError);
    },unqliteKeyDatabase);
    std::cout << "Unqlite fetched elapsed time : " << currentTimer.GetElapsedTimeInMilliSec() << " ms" << std::endl;
    //---------------------------------------------Beginning of RocksDb Tests------------------------------------------------------------------
    timeElapsed = TimedExecution(currentTimer,[](LD::BasicKeyedDatabase<LD::RocksBackingStore> & db) noexcept
    {
        db.Insert("abc"_ts,LD::Pyramid{LD::Square{922},LD::Triangle{72.48,92.73}});
    },rocksdbKeyedDatabase);
    std::cout << "Rocksdb Insertion time : " << timeElapsed.ToImmutableString().Data() << " ms" << std::endl;

    timeElapsed = TimedExecution(currentTimer,[](LD::BasicKeyedDatabase<LD::RocksBackingStore> & db) noexcept
    {
        auto result =  db.Fetch("abc"_ts,LD::CT::TypeList<LD::Pyramid>{});
        auto onFetch = [](const LD::Context<LD::TransactionResult,LD::Pyramid> & context) noexcept
        {
            LD::Pyramid & pyramid = LD::Get(LD::Get<1>(context));
            std::cout << pyramid.Side().Base() << std::endl;
            std::cout << pyramid.Side().Height() << std::endl;
            std::cout << pyramid.Base().Length() << std::endl;
        };

        auto onFetchError = [](auto) noexcept
        {

        };
        LD::Match(result,onFetch,onFetchError);
    },rocksdbKeyedDatabase);
    std::cout << "Rocksdb fetched elapsed time : " << currentTimer.GetElapsedTimeInMilliSec() << " ms" << std::endl;
    //---------------------------------------------------End of RocksDb Tests------------------------------------------------------------------

    nlohmann::json output;

    nlohmann::json firstBench;
    firstBench["Bench"]["Name"] = "Insertion";
    firstBench["Bench"]["ElapsedTime"] = LD::ToImmutableString(currentTimer.GetElapsedTimeInMilliSec()).Data();
    output["Marks"].push_back(firstBench);
    output["Marks"].push_back(firstBench);
    std::cout << output.dump(1) << std::endl;

    //auto tbl = toml::table{};
    //tbl["a"]["b"] = "abc";
    //std::cout << toml::parse("");
    toml::basic_value<toml::discard_comments> value;
    value["a"]["b"] = "abc";
    //value["MyPyramid"] = "Pyramid";
    value["MyPyramid"]["Base"]["Pyramid.Base"] = "Square";
    value["MyPyramid"]["Base"]["Length"] = "17";
    value["MyPyramid"]["Side"]["Pyramid.Side"] = "Triangle";
    value["MyPyramid"]["Side"]["Width"] = "2";
    value["MyPyramid"]["Side"]["Length"] = "2";
    value["MyPyramid"]["ClassName"] = "Pyramid";
    std::cout << value << std::endl;
    return 0;
}
 */
}