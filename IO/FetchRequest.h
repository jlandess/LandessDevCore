//
// Created by phoenixflower on 6/3/20.
//
#ifndef LANDESSDEVCORE_FETCHREQUEST_H
#define LANDESSDEVCORE_FETCHREQUEST_H
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
    }

    template<typename T, typename U>
    using GenerateFetchRequestVariant = typename LD::Detail::GenerateFetchRequestVariant<T,U>::type;
}


namespace LD
{
    template<typename T>
    class FetchRequestResult;

    template<typename ... Pack, typename ... Args>
    class FetchRequestResult<LD::Variant<Pack...>(Args...)>: public LD::GenerateFetchRequestVariant<LD::CT::TypeList<Pack...>,LD::CT::TypeList<Args...>>
    {
    private:
        using BaseClass = LD::GenerateFetchRequestVariant<LD::CT::TypeList<Pack...>,LD::CT::TypeList<Args...>>;
    public:

        FetchRequestResult() = default;

        constexpr FetchRequestResult(const BaseClass & base) noexcept :BaseClass{base}
        {

        }
        template<typename V, typename = LD::Enable_If_T<
                LD::Require<
                        (LD::GetTypeCountInTypeList<V,LD::CT::TypeList<Pack...>>::value > 0)
                        >>>
        constexpr FetchRequestResult(const LD::Context<LD::DatabaseTransactionResult,V,Args...> & context) noexcept
        {
            (*this) = context;
        }

        constexpr FetchRequestResult(const LD::Context<LD::DatabaseError,Args...> & context) noexcept
        {
            (*this) = context;
        }
        FetchRequestResult & operator = (const BaseClass & base) noexcept
        {
            BaseClass::operator=(base);
            return (*this);
        }

        constexpr FetchRequestResult & operator = (const LD::Context<LD::DatabaseError,Args...> & context)
        {
            BaseClass::operator=(context);
            return (*this);
        }

        template<typename V>
        LD::Enable_If_T<LD::Require<
                (LD::GetTypeCountInTypeList<V,LD::CT::TypeList<Pack...>>::value > 0)
        >,FetchRequestResult&> operator = (const LD::Context<LD::DatabaseTransactionResult,V,Args...> & context) noexcept
        {
            BaseClass::operator=(context);
            return (*this);
        }
    };
}

#endif //LANDESSDEVCORE_FETCHREQUEST_H
