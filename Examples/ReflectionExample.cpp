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
#include "Chrono/Timer.h"
#include "Primitives/General/ContextualVariant.h"
namespace LD
{

    namespace Example
    {
        void ReflectionExample()
        {
            LD::Variant<LD::Context<int,float,double>,LD::Context<float,LD::StringView>> var;
            var = LD::Context<int,float,double>{};
            //LD::Variant<LD::Context<LD::Variant<int,float>,Cntx...>...>
            LD::ContextualVariant<LD::Variant<int,float>(LD::StringView,LD::StringView)> contextualVariant;
            LD::ContextualVariant<LD::TypeList<int,float>(LD::StringView,LD::StringView)> contextualVariant1;
            contextualVariant.Variant() = LD::Context<int,LD::StringView,LD::StringView>{};
            contextualVariant = LD::Context<int,LD::StringView,LD::StringView>{};
            contextualVariant1 = LD::Context<float,LD::StringView,LD::StringView>{};


            LD::Match(contextualVariant1,
                    [](const LD::Context<int,LD::StringView,LD::StringView> &){ printf("match int made! \n");},
                      [](const LD::Context<float,LD::StringView,LD::StringView> &){ printf("match float made! \n");}
            );
            //contextualVariant = LD::Context<int,StringView>{};
            LD::Timer timer;
            timer.Start();
            Database currentData{{"database"}};
            Square currentSquare;
            currentSquare["Length"_ts] = 64.3;

            Pyramid currentPyramid;
            currentPyramid["Base"_ts]["Length"_ts] = 17*7;
            currentSquare["Length"_ts] = 17.96;
            currentData.Insert("key"_ts,currentSquare);
            //currentData.Insert("pyramid"_ts,currentPyramid);
            auto result = currentData.Fetch<Square>("key"_ts,LD::UInteger{32});

            timer.Stop();

            std::cout << "DB execution time: "<< timer.Time()/1.0_us << std::endl;


            auto notFound = [](const LD::Context<LD::DatabaseError,LD::UInteger> &)
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

        }
    }
}