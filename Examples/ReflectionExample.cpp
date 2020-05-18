//
// Created by phoenixflower on 5/13/20.
//
#include "ReflectionExample.hpp"
#include "Reflection/reflectable.hpp"
#include "Functor/FunctionView.h"
#include "Primitives/General/Immutable.hpp"
#include "IO/Database.hpp"
#include "ReflectionDemoTypes.h"
#include <iostream>
namespace LD
{

    namespace Example
    {
        void ReflectionExample()
        {
            printf("%lu \n",std::atoi("77"));
            Database currentData{{"database"}};
            Square currentSquare;
            currentSquare["Length"_ts] = 64.3;

            Pyramid currentPyramid;
            currentPyramid["Base"_ts]["Length"_ts] = 17*7;
            currentSquare["Length"_ts] = 17.96;
            currentData.Insert("key"_ts,currentSquare);
            //currentData.Insert("pyramid"_ts,currentPyramid);
            auto result = currentData.Fetch<Square>("key"_ts,LD::UInteger{32});



            auto notFound = [](const LD::Context<LD::NullClass,LD::UInteger> &)
            {
                printf("not found\n");
            };
            auto found= []( const LD::Context<Square,LD::UInteger> & square)
            {

                //printf("%lu \n",square["Length"_ts]);
                std::cout << "Square's Length  1 " << LD::Get<Square>(square)["Length"_ts] << std::endl;
                std::cout << "context number : " << LD::Get<1>(square) << std::endl;
            };

            

            LD::Match(result,notFound,found);


            /*
            printf("%s\n","beginning of reflection example");
            unqlite *pDb;
            int rc;
            rc = unqlite_open(&pDb,"test.db",UNQLITE_OPEN_CREATE);
            Square currentSquare;
            Pyramid currentPyramid;
            currentPyramid["Base"_ts]["Length"_ts] = 17*7;
            currentSquare["Length"_ts] = 17.96;

            using ClassName = decltype(Square::GetClassNameTypeString());
            SerializeObject(pDb,"key"_ts,currentSquare);
            SerializeObject(pDb,"pyramid"_ts,currentPyramid);

            using KeyTest = decltype("key.Square.Length"_ts);
            using PyramidTest = decltype("pyramid.Pyramid.Base.Square.Length"_ts);
            if( rc != UNQLITE_OK ){ return; }

            auto fetchCallback = [](const void * data, unsigned int dataSize, void * inputPointer)->int
            {
                printf("found it %s\n",(char*)data);
                //std::cout << "fetch being called correctly " << (const char*)data << "  size : " << dataSize << "   " << strlen((const char*)data) <<  std::endl;
                return 0;
            };
            unqlite_kv_fetch_callback(pDb,PyramidTest::data(),PyramidTest::size(),fetchCallback, nullptr);

            //unqlite_kv_store(pDb,key.c_str(),key.size(),cachedDocument.c_str(),cachedDocument.size());
            unqlite_commit(pDb);
            unqlite_close(pDb);
             */
        }
    }
}