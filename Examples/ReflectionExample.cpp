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
#include "Algorithms/Calculus.h"
#include "IO/FetchRequest.h"


namespace LD
{
    namespace Example
    {

        void ReflectionExample()
        {
            //using AList = AugmentedList<LD::CT::TypeList<int,double,char>,float,LD::CT::TypeList<>>::UsableContext;
            //LD::DebugTemplate<CList>{};

            //LD::ContextualVariant<LD::Pair<LD::Variant<int>,LD::Variant<int>>()> stuff;
            std::cout << LD::RectangularIntegrate([](const LD::Float & x){return x*x;},1.0,5.0,LD::MidPointRule{},0.001) << std::endl;

            std::cout << LD::TrapezoidIntegrate([](const LD::Float & x){return x*x;},1.0,5.0,0.0001) << std::endl;

            std::cout << LD::SimpsonIntegrate([](const LD::Float & x){return x*x;},1.0,5.0,0.0001) << std::endl;

            std::cout << LD::QuadratureIntegrate([](const LD::Float & x){return x*x;},1.0,5.0,0.0001) << std::endl;

            std::cout << LD::RungaKutta4Integrate([](const LD::Float & x, const LD::Float & y){return x*x;},1.0,5.0,0.0001) << std::endl;



            LD::UnQliteBackend<char> currentBackend{LD::StringView {"backend.db"},OpenAndCreateIfNotExists{}};
            LD::BasicDatabase<LD::UnQliteBackend<char>> database{currentBackend};

            Square currentSuqre;
            currentSuqre["Length"_ts] = 7;

            LD::Get<0>(currentSuqre);

            Pyramid currentPyramid;

            //it does
            //currentPyramid["Base"_ts]["Length"_ts] = 7;
            //currentPyramid["Side"_ts]["Base"_ts] = 17;
            //currentPyramid["Side"_ts]["Height"_ts] = 37;


            currentPyramid.Side() = LD::Triangle{37,521};
            currentPyramid.Base() = LD::Square{622};



            LD::Ref<int>{};

            int abc = 93;
            LD::ContextualVariant<LD::Variant<LD::DatabaseError,LD::DatabaseTransactionResult>(LD::Ref<int>)> insertResult;
            insertResult  = database.InsertAndCommit("key"_ts,currentPyramid,abc);




            auto onSuccessFullInsert = [](const LD::Context<LD::DatabaseTransactionResult,LD::Ref<int>> & cntxt)
            {
                std::cout << "successful transaction " << LD::Get(LD::Get<1>(cntxt)) << std::endl;

                return LD::UInteger {};
            };


            auto onDbInsertError = [](const LD::Context<LD::DatabaseError,LD::Ref<int>> &)
            {
                return LD::UInteger {};

            };

            LD::UInteger workDone = LD::Match(insertResult,onSuccessFullInsert,onDbInsertError);


            //using auto is encouraged here, the type is fully typed out for clarity
            //LD::ContextualVariant<LD::Variant<LD::DatabaseError,LD::DatabaseTransactionResult>(LD::Variant<Pyramid>)> queryResult = database.Fetch("key"_ts,LD::CT::TypeList<Pyramid>{});

            LD::FetchRequestResult<LD::Variant<LD::Pyramid>()> fetchResult =  database.Fetch("key"_ts,LD::CT::TypeList<Pyramid>{});
            //when a fetch request fails we simply get a context with the database error and the arguements passed in Fetch to be used in the anymous function
            auto onFetchError = [](const LD::Context<LD::DatabaseError> & context )
            {

            };

            //the second object specified in a given LD::Context after a fetch request will always be the object found
            //the first object specified in a given LD::Context will either be LD::DatabaseTransactionResult or LD::DatabaseError
            //If a database error has occured, as show above, there will be no object available for usage and so it simply provides the error in question
            auto onPyramidTransaction = [](const LD::Context<LD::DatabaseTransactionResult,Pyramid> & context)
            {

                std::cout << "fetch1 query result " << std::endl;
                Pyramid & pyramidToUse = LD::Get<1>(context);

                std::cout << pyramidToUse.Base().Length()<< std::endl;
                std::cout << pyramidToUse.Side().Base() << std::endl;
                std::cout << pyramidToUse.Side().Height() << std::endl;

            };
            LD::Match(fetchResult,onFetchError,onPyramidTransaction);
        }
    }
}