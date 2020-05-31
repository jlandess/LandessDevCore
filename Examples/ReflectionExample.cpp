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
            LD::BasicDatabase<LD::UnQliteBackend<char>> database{currentBackend};

            Square currentSuqre;
            currentSuqre["Length"_ts] = 7;

            Pyramid currentPyramid;

            //it does
            currentPyramid["Base"_ts]["Length"_ts] = 7;
            currentPyramid["Side"_ts]["Base"_ts] = 17;
            currentPyramid["Side"_ts]["Height"_ts] = 37;

            currentPyramid.Side() = LD::Triangle{37,521};
            currentPyramid.Base() = LD::Square{92};



            LD::ContextualVariant<LD::Variant<LD::DatabaseError,LD::DatabaseTransactionResult>()> insertResult =  database.InsertAndCommit("key"_ts,currentPyramid);


            auto onSuccessFullInsert = [](const LD::Context<LD::DatabaseTransactionResult> &)
            {
                std::cout << "successful transaction " << std::endl;

            };

            auto onDbInsertError = [](const LD::Context<LD::DatabaseError> &)
            {

            };

            LD::Match(insertResult,onSuccessFullInsert,onDbInsertError);

            LD::ContextualVariant<LD::Variant<LD::DatabaseError,LD::DatabaseTransactionResult>(LD::Variant<Pyramid>)> queryResult = database.Fetch("key"_ts,LD::CT::TypeList<Pyramid>{});


            auto onDatabaseError = [](const LD::Context<LD::DatabaseError,LD::Variant<Pyramid>> & context)
            {

                std::cout << "key not found" << std::endl;
            };

            auto onDatabaseTransaction = [](const LD::Context<LD::DatabaseTransactionResult,LD::Variant<Pyramid>> & context)
            {

                auto printVariant = [](const LD::Pyramid & pyramidToUse)
                {
                    std::cout << "Querried Pyramid " << std::endl;
                    //Pyramid & pyramidToUse = LD::Get<1>(context);
                    std::cout << pyramidToUse["Base"_ts]["Length"_ts] << std::endl;
                    std::cout << pyramidToUse["Side"_ts]["Base"_ts] << std::endl;
                    std::cout << pyramidToUse["Side"_ts]["Height"_ts] << std::endl;
                };

                LD::Match(LD::Get<1>(context),printVariant);
            };

            LD::Match(queryResult,onDatabaseError,onDatabaseTransaction);

        }
    }
}