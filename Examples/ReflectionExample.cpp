//
// Created by phoenixflower on 5/13/20.
//
#include "ReflectionExample.hpp"
#include "Primitives/General/Immutable.hpp"
#include "IO/Database.hpp"
#include "ReflectionDemoTypes.h"
#include <iostream>
#include "Primitives/General/ContextualVariant.h"
#include "IO/DatabaseOperationResult.h"
#include "Primitives/General/Span.hpp"
#include "IO/UnqliteDatabaseBackend.h"
#include "IO/FetchRequest.h"
#include "IO/DatabaseEntity.h"
#include "IO/JsonDatabaseBackend.h"
#include "Chrono/Timer.h"
namespace LD
{
    namespace Example
    {

        void ReflectionExample()
        {

            LD::Timer currentTimer;
            currentTimer.Start();

            using ReflectiveTypeStructure =  LD::CT::GenerateNamedReflectiveTypeStructure<decltype("meep"_ts),LD::Pyramid>;

            using Keys = typename LD::CT::TypeListChannelView<0,2,ReflectiveTypeStructure>;
            using Types = typename LD::CT::TypeListChannelView<1,2,ReflectiveTypeStructure>;

            LD::For<0,ReflectiveTypeStructure::size()/2,1>([](auto I)
            {
                using Key = LD::CT::TypeAtIndex<I,Keys>;
                using Type = LD::CT::TypeAtIndex<I,Types>;
                if constexpr(LD::IsPrimitive<Type> || LD::IsReflectable<Type>)
                {
                    std::cout << LD::CT::TypeAtIndex<I,Keys>::data() << "  : " << typeid(Type).name() <<  std::endl;
                }
                return true;
            });

            LD::Entity<decltype("key"_ts),LD::Square,LD::BasicKeyedDatabase<LD::UnQliteBackend<char>>> entity;



            LD::UnQliteBackend<char> currentBackend{LD::StringView {"backend.db"},OpenAndCreateIfNotExists{}};


            LD::BasicKeyedDatabase<LD::UnQliteBackend<char>> database{currentBackend};

            nlohmann::json currentJsonBackingStore;
            LD::JsonBackend jsonBackend{currentJsonBackingStore};

            LD::BasicKeyedDatabase<LD::JsonBackend> jsonDatabase{jsonBackend};

            Square currentSuqre;
            currentSuqre["Length"_ts] = 7;

            auto jsonDBInsertBegin = currentTimer.Time();
            jsonDatabase.Insert("key"_ts,currentSuqre);
            auto jsonDBInsertEnd = currentTimer.Time();
            std::cout << "Json Insert of a Square took : " << ((jsonDBInsertEnd-jsonDBInsertBegin)/1.0_us) << " micrseconds " << std::endl;

            Pyramid currentPyramid;

            jsonDBInsertBegin = currentTimer.Time();
            jsonDatabase.Insert("pyrakey"_ts,currentPyramid);
            jsonDBInsertEnd = currentTimer.Time();
            std::cout << "Json Insert of a Pyramid took : " << ((jsonDBInsertEnd-jsonDBInsertBegin)/1.0_us) << " micrseconds " << std::endl;
            std::cout << currentJsonBackingStore.dump(2) << std::endl;

            currentPyramid.Side() = LD::Triangle{37,521};
            currentPyramid.Base() = LD::Square{9723};

            auto unqliteInsertAndCommitBegin = currentTimer.Time();
            LD::QueryResult<LD::Pyramid &(int)> insertQueryRes = database.InsertAndCommit("key"_ts,currentPyramid,int{232});
            auto unqliteInsertAndCommitEnd = currentTimer.Time();
            std::cout << "Unqlite Insert of a Pyramid took : " << ((unqliteInsertAndCommitEnd-unqliteInsertAndCommitBegin)/1.0_us) << " micrseconds " << std::endl;

            auto onInsertionFailure = [](const LD::Context<LD::TransactionError,int> & context)
            {

            };

            auto onInsertOfPyramid = [](const LD::Context<LD::TransactionResult,Pyramid&,int> & context)
            {

                std::cout << "pyramid succesfully inserted   " << LD::Get<2>(context) << std::endl;
            };
            LD::Match(insertQueryRes,onInsertionFailure,onInsertOfPyramid);

            auto unqliteFetchBegin = currentTimer.Time();
            LD::QueryResult<LD::Variant<LD::Pyramid>()> fetchResult =  database.Fetch("key"_ts,LD::CT::TypeList<Pyramid>{});
            auto unqliteFetchEnd = currentTimer.Time();
            std::cout << "Unqlite Fetch of a Pyramid took : " << ((unqliteFetchEnd-unqliteFetchBegin)/1.0_us) << " micrseconds " << std::endl;
            //when a fetch request fails we simply get a context with the database error and the arguements passed in Fetch to be used in the anymous function
            auto onFetchError = [](const LD::Context<LD::TransactionError> & context )
            {

            };
            //database.InsertGroup(LD::MakeTuple("key1"_ts),LD::MakeContext(LD::Square{}));
            //the second object specified in a given LD::Context after a fetch request will always be the object found
            //the first object specified in a given LD::Context will either be LD::DatabaseTransactionResult or LD::DatabaseError
            //If a database error has occured, as show above, there will be no object available for usage and so it simply provides the error in question
            auto onPyramidTransaction = [](const LD::Context<LD::TransactionResult,Pyramid> & context)
            {

                std::cout << "fetch1 query result " << std::endl;
                Pyramid & pyramidToUse = LD::Get<1>(context);

                std::cout << pyramidToUse.Base().Length()<< std::endl;
                std::cout << pyramidToUse.Side().Base() << std::endl;
                std::cout << pyramidToUse.Side().Height() << std::endl;

            };
            LD::Match(fetchResult,onFetchError,onPyramidTransaction);

            //LD::QueryResult<bool()> deletionQueryResult = database.Remove("key"_ts,LD::CT::TypeList<LD::Pyramid>{});

            auto unqliteRemoveStart = currentTimer.Time();
            LD::QueryResult<LD::Type<LD::Pyramid>()> deletionQueryResult = database.Remove("key"_ts,LD::CT::TypeList<LD::Pyramid>{});
            auto unqliteRemoveEnd = currentTimer.Time();
            std::cout << "Unqlite Removal of a Pyramid took : " << ((unqliteRemoveEnd-unqliteRemoveStart)/1.0_us) << " micrseconds " << std::endl;


            auto onDeletionError = [](const LD::Context<LD::TransactionError> & context)
            {

            };

            auto onDeletionSuccess = [](const LD::Context<LD::TransactionResult,LD::Type<Pyramid>> & context)
            {
                std::cout << "successfully deleted pyramid" << std::endl;

            };

            LD::Match(deletionQueryResult,onDeletionSuccess,onDeletionError);
            currentTimer.Stop();
        }
    }
}