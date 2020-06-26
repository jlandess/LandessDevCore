//
// Created by phoenixflower on 6/3/20.
//
#ifndef LANDESSDEVCORE_FETCHREQUEST_H
#define LANDESSDEVCORE_FETCHREQUEST_H
#include "Primitives/General/Context.h"
#include "IO/DatabaseOperationResult.h"
#include "Primitives/General/ContextualVariant.h"
namespace LD
{
    namespace Detail
    {
        template<typename T, typename U, typename A>
        struct GenerateFetchRequestContextMap;

        template<typename ... FirstSet, typename Amendable, typename ... Args>
        struct GenerateFetchRequestContextMap<LD::CT::TypeList<FirstSet...>,Amendable,LD::CT::TypeList<Args...>>
        {
            using type = LD::CT::TypeList<LD::Context<FirstSet,Amendable,Args...>...>;
        };

        template<typename ... FirstSet, typename ... SecondSet, typename ... Args>
        struct GenerateFetchRequestContextMap<LD::CT::TypeList<FirstSet...>,LD::CT::TypeList<SecondSet...>,LD::CT::TypeList<Args...>>
        {
            using type = LD::CT::Flatten<LD::CT::TypeList<typename GenerateFetchRequestContextMap<LD::CT::TypeList<FirstSet...>,SecondSet,LD::CT::TypeList<Args...>>::type...>>;
        };


        template<typename T, typename U>
        struct GenerateFetchRequestVariant;


        template<typename ... Pack, typename ... Args>
        struct GenerateFetchRequestVariant<LD::CT::TypeList<Pack...>,LD::CT::TypeList<Args...>>
        {
            using type = LD::CT::RebindList<typename LD::CT::Prepend<LD::Context<LD::DatabaseError,Args...>, typename GenerateFetchRequestContextMap<LD::CT::TypeList<LD::DatabaseTransactionResult>,LD::CT::TypeList<Pack...>,LD::CT::TypeList<Args...>>::type>::type,LD::Variant>;
        };


        template<typename T, typename = void>
        struct OptionalFront;


        template<typename ... Args>
        struct OptionalFront<LD::CT::TypeList<Args...>,LD::Enable_If_T<LD::Require<(sizeof...(Args) > 0)>>>
        {
            using type = typename LD::TypeAtIndex<0,LD::CT::TypeList<Args...>>::type;
        };

        template<typename ... Args>
        struct OptionalFront<LD::CT::TypeList<Args...>,LD::Enable_If_T<LD::Require<(sizeof...(Args) == 0)>>>
        {
            using type = void;
        };




    }

    template<typename T>
    using OptionalFront = typename LD::Detail::OptionalFront<T>::type;

    template<typename T, typename U>
    using GenerateFetchRequestVariant = typename LD::Detail::GenerateFetchRequestVariant<T,U>::type;
}


namespace LD
{
    template<typename T>
    class QueryResult;

    template<typename T, typename ... Args>
    class QueryResult<T(Args...)>: public LD::GenerateFetchRequestVariant<LD::CT::TypeList<T>,LD::CT::TypeList<Args...>>
    {
    private:
        using BaseClass = LD::GenerateFetchRequestVariant<LD::CT::TypeList<T>,LD::CT::TypeList<Args...>>;
    public:
        QueryResult() = default;

        constexpr QueryResult(const BaseClass & base) noexcept :BaseClass{base}
        {

        }


        constexpr QueryResult(const LD::Context<LD::DatabaseTransactionResult,T,Args...> & context) noexcept
        {
            (*this) = context;
        }


        constexpr QueryResult(const LD::Context<LD::DatabaseError,Args...> & context) noexcept
        {
            (*this) = context;
        }


        QueryResult& operator = (const LD::Context<LD::DatabaseTransactionResult,T,Args...> & context) noexcept
        {
            BaseClass::operator=(context);
            return (*this);
        }

        QueryResult & operator = (const LD::Context<LD::DatabaseError,Args...> & context) noexcept
        {
            BaseClass::operator=(context);
            return (*this);
        }
        QueryResult & operator = (const BaseClass & base) noexcept
        {
            BaseClass::operator=(base);
            return (*this);
        }

    };
    template<typename ... Pack, typename ... Args>
    class QueryResult<LD::Variant<Pack...>(Args...)>: public LD::GenerateFetchRequestVariant<LD::CT::TypeList<Pack...>,LD::CT::TypeList<Args...>>
    {
    private:
        using BaseClass = LD::GenerateFetchRequestVariant<LD::CT::TypeList<Pack...>,LD::CT::TypeList<Args...>>;
    public:

        QueryResult() = default;

        constexpr QueryResult(const BaseClass & base) noexcept :BaseClass{base}
        {

        }

        template<typename ... B,
                typename BList = LD::CT::TypeList<B...>,
                typename DBOp = LD::OptionalFront<BList>,
                typename ListWithoutDbOp = LD::CT::PopFront<BList>,
                typename Type = LD::OptionalFront<ListWithoutDbOp>,
                typename = LD::Enable_If_T<
                LD::Require<
                        (sizeof...(B) == sizeof...(Args)+2),
                        LD::IsSame<DBOp,LD::DatabaseTransactionResult>,
                        (LD::GetTypeCountInTypeList<Type,LD::CT::TypeList<Pack...>>::value > 0)
                        >>>
        constexpr QueryResult(const LD::Tuple<B...> & context) noexcept
        {
            //LD::CT::DebugTemplate<LD::CT::TypeList<Pack...>>{};
            //(*this) = context;
            BaseClass::operator=(context);
        }


        template<typename Type, typename = LD::Enable_If_T<
                LD::Require<
                        (LD::GetTypeCountInTypeList<Type,LD::CT::TypeList<Pack...>>::value == 0)
                        >>>
        constexpr QueryResult(const LD::DatabaseTransactionResult & transaction, const Type & object, const LD::Context<Args...> & context) noexcept
        {
            LD::Context<LD::DatabaseTransactionResult,Type,Args...> cntxt;
            LD::Get(LD::Get<0>(cntxt)) = transaction;
            LD::Get(LD::Get<1>(cntxt)) = object;
            if constexpr(sizeof...(Args) > 0)
            {
                LD::For<sizeof...(Args)>([](
                        auto I,
                        auto && inputContext,
                        auto && outputContext)
                {

                    LD::Get<I+2>(outputContext) = LD::Get<I>(inputContext);
                    return true;
                },context,cntxt);
            }

        }

        constexpr QueryResult(const LD::DatabaseError & transaction, const LD::Context<Args...> & context) noexcept
        {
            LD::Context<LD::DatabaseError,Args...> cntxt;
            LD::Get(LD::Get<0>(cntxt)) = transaction;
            //LD::Get(LD::Get<1>(cntxt)) = object;
            if constexpr(sizeof...(Args) > 0)
            {
                LD::For<sizeof...(Args)>([](
                        auto I,
                        auto && inputContext,
                        auto && outputContext)
                {
                    LD::Get<I+1>(outputContext) = LD::Get<I>(inputContext);
                    return true;
                },context,cntxt);
            }

        }



        constexpr QueryResult(const LD::Context<LD::DatabaseError,Args...> & context) noexcept
        {
            (*this) = context;
        }

        /*
        QueryResult & operator = (const BaseClass & base) noexcept
        {
            BaseClass::operator=(base);
            return (*this);
        }
         */


        constexpr QueryResult & operator = (const LD::Context<LD::DatabaseError,Args...> & context)
        {
            BaseClass::operator=(context);
            return (*this);
        }

        //todo fix sfinae with list comparisons instead of implicit capture
        template<typename V>
        LD::Enable_If_T<LD::Require<
                (LD::GetTypeCountInTypeList<V,LD::CT::TypeList<Pack...>>::value > 0)
        >,QueryResult&> operator = (const LD::Context<LD::DatabaseTransactionResult,V,Args...> & context) noexcept
        {
            BaseClass::operator=(context);
            return (*this);
        }
    };


}

#endif //LANDESSDEVCORE_FETCHREQUEST_H
