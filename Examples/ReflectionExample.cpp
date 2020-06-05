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
            LD::Ref<LD::Pyramid>{};
            LD::Pyramid meep;
            LD::QueryResult<LD::Pyramid &(int)> stuff = LD::MakeContext(LD::DatabaseTransactionResult{},meep,int{});
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

            currentPyramid.Side() = LD::Triangle{37,521};
            currentPyramid.Base() = LD::Square{652};
            //int abc = 93;

            LD::QueryResult<LD::Pyramid &(int)> insertQueryRes = database.InsertAndCommit("key"_ts,currentPyramid,int{232});


            auto onInsertionFailure = [](const LD::Context<LD::DatabaseError,int> & context)
            {

            };

            auto onInsertOfPyramid = [](const LD::Context<LD::DatabaseTransactionResult,Pyramid&,int> & context)
            {

                std::cout << "pyramid succesfully inserted   " << LD::Get<2>(context) << std::endl;
            };
            LD::Match(insertQueryRes,onInsertionFailure,onInsertOfPyramid);

            LD::QueryResult<LD::Variant<LD::Pyramid>()> fetchResult =  database.Fetch("key"_ts,LD::CT::TypeList<Pyramid>{});
            //when a fetch request fails we simply get a context with the database error and the arguements passed in Fetch to be used in the anymous function
            auto onFetchError = [](const LD::Context<LD::DatabaseError> & context )
            {

            };
            //database.InsertGroup(LD::MakeTuple("key1"_ts),LD::MakeContext(LD::Square{}));
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