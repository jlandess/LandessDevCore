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




namespace LD
{
    namespace Example
    {

        void ReflectionExample()
        {
            LD::UnQliteBackend<char> currentBackend{LD::StringView {"backend.db"},OpenAndCreateIfNotExists{}};
            BasicDatabase<LD::UnQliteBackend<char>> database{currentBackend};

            Square currentSuqre;
            currentSuqre["Length"_ts] = 7;

            Pyramid currentPyramid;

            currentPyramid["Base"_ts]["Length"_ts] = 7;
            currentPyramid["Side"_ts]["Base"_ts] = 17;
            currentPyramid["Side"_ts]["Height"_ts] = 37;


            database.InsertAndCommit("key"_ts,currentPyramid);


            LD::ContextualVariant<LD::Variant<LD::DatabaseError,LD::DatabaseTransactionResult>(Pyramid)> queryResult = database.Fetch<Pyramid>("key"_ts,LD::CT::TypeList<>{});


            auto onDatabaseError = [](const LD::Context<LD::DatabaseError,Pyramid> & context)
            {

                std::cout << "key not found" << std::endl;
            };

            auto onDatabaseTransaction = [](const LD::Context<LD::DatabaseTransactionResult,Pyramid> & context)
            {

                std::cout << "Querried Pyramid " << std::endl;
                Pyramid & pyramidToUse = LD::Get<1>(context);
                std::cout << pyramidToUse["Base"_ts]["Length"_ts] << std::endl;
                std::cout << pyramidToUse["Side"_ts]["Base"_ts] << std::endl;
                std::cout << pyramidToUse["Side"_ts]["Height"_ts] << std::endl;

            };

            LD::Match(queryResult,onDatabaseError,onDatabaseTransaction);

        }
    }
}