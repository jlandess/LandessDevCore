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
#include "IO/DatabaseOperationResult.h"
#include "Primitives/General/Span.hpp"
namespace LD
{


    namespace CT
    {


        namespace Detail
        {
            template <typename... T1s, typename... T2s>
            constexpr auto concatenate(LD::CT::TypeList<T1s...>, LD::CT::TypeList<T2s...>) {
                return LD::CT::TypeList<T1s..., T2s...>{};
            }

            template <template <typename> typename Condition, typename Result>
            constexpr auto filter_types(Result result, LD::CT::TypeList<>) {
                return result;
            }

            template <template <typename> typename Condition, typename Result, typename T, typename... Ts>
            constexpr auto filter_types(Result result, LD::CT::TypeList<T, Ts...>) {
                if constexpr (Condition<T>{})
                    return filter_types<Condition>(concatenate(result, LD::CT::TypeList<T>{}), LD::CT::TypeList<Ts...>{});
                else
                    return filter_types<Condition>(result, LD::CT::TypeList<Ts...>{});
            }
        }

        template <template <typename> typename Condition, typename... Types>
        using FilterPack = LD::Decay_T<decltype(LD::CT::Detail::filter_types<Condition>(LD::CT::TypeList<>{}, LD::CT::TypeList<Types...>{}))>;

        template <typename TL,template <typename> typename Condition>
        using Filter = LD::Decay_T<decltype(LD::CT::Detail::filter_types<Condition>(LD::CT::TypeList<>{}, TL{}))>;

        namespace Detail
        {

            template<typename TypeList, template<typename> class Transformation>
            class Transform;
            template<template<typename> class Transformation,typename ... Args>
            struct Transform<LD::CT::TypeList<Args...>,Transformation>
            {
                using type = LD::CT::TypeList<Transformation<Args>...>;
            };
        }

        template<typename T>
        using ReadOnlyTransform =  LD::Conditonal<LD::IsPrimitive<T>,LD::ElementReference<T>,
                                                  LD::Conditonal<LD::Concept::ContinuousIterable<T>,LD::Span<T>,
                                                  LD::Conditonal<LD::Concept::Iterable<T>,LD::Pair<LD::ConstBeginIterator<T>,LD::ConstEndIterator<T>>,T>>>;


        template<typename T>
        using WriteOnlyTransform = LD::Conditonal<LD::IsPrimitive<T>,LD::ElementReference<T>,
                                                  LD::Conditonal<LD::Concept::Iterable<T>,LD::BackInserter<T>,T>>;


        template<typename T>
        using ReadWriteTransform = void;



        template<typename T, template<typename> class Transformation>
        using Tranform = typename LD::CT::Detail::Transform<T,Transformation>::type;

    }
    namespace Example
    {
        void ReflectionExample()
        {


            using list = LD::tlist_erase_at<0,LD::CT::RebindList<ArrayTest::ValueTypeList,LD::CT::TypeList>::type>::type;

            using transformedList = LD::CT::Tranform<list,LD::CT::ReadOnlyTransform>;
            //LD::CT::DebugTemplate<transformedList>{};
            LD::ContextualVariant<LD::DatabaseOpenMode()> context = LD::ContextualVariant<LD::DatabaseOpenMode()>{LD::MakeContext(LD::OpenReadAndWrite{})};
            LD::Variant<LD::Context<int,float,double>,LD::Context<float,LD::StringView>> var;
            var = LD::Context<int,float,double>{};
            //LD::Variant<LD::Context<LD::Variant<int,float>,Cntx...>...>
            LD::ContextualVariant<LD::Variant<int,float>(LD::StringView,LD::StringView)> contextualVariant;
            contextualVariant = LD::Context<int,LD::StringView,LD::StringView>{};


            contextualVariant = LD::MakeContext(int{},LD::StringView{},LD::StringView{});
            LD::Match(contextualVariant,
                    [](const LD::Context<int,LD::StringView,LD::StringView> &){ printf("match int made! \n");},
                    [](const LD::Context<float,LD::StringView,LD::StringView> &){ printf("match float made! \n");}
            );

            LD::Timer timer;
            timer.Start();
            Database currentData{{"database"}};
            Square currentSquare;

            //LD::DebugTemplate<Square::ValueTypeList>{};
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
                std::cout << "Square's Length   " << LD::Get<Square>(square)["Length"_ts] << std::endl;
                std::cout << "context number : " << LD::Get<1>(square) << std::endl;
            };
            LD::Match(result,notFound,found);

        }
    }
}