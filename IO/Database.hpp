//
// Created by phoenixflower on 5/14/20.
//

#ifndef LANDESSDEVCORE_DATABASE_HPP
#define LANDESSDEVCORE_DATABASE_HPP
#include "TypeTraits/EnableIf.hpp"
#include "Definitions/Integer.hpp"
#include "Primitives/General/StringView.hpp"
#include "Reflection/reflectable.hpp"
#include "Primitives/General/mapboxvariant.hpp"
#include "Primitives/General/mapboxvariantvisitor.h"
#include "Algorithms/StringToPrimitive.hpp"
#include "Algorithms/StringAsNumber.h"
#include "Primitives/General/Context.h"
#include "TypeTraits/TypeList.hpp"
#include "Primitives/General/ContextualVariant.h"
#include "DatabaseOperationResult.h"
#include "Primitives/General/Span.hpp"
#include "Memory/ElementReference.h"
#include "TypeTraits/Declval.hpp"
#include "TypeTraits/Decay.hpp"
namespace LD
{

    /**
     *
     * @tparam Backend
     * @brief An abstraction layer of NoSQL implementations, SQL can be used if a key-value schema is supported
     */
    template<typename Backend>
    class BasicDatabase
    {
    private:

        using Db = LD::ElementReference<Backend>;
        Db mBackend;

        template<typename Bk>
        using StoreTest = decltype(LD::Declval<Bk>().Store(LD::Declval<LD::StringView>(),LD::Declval<LD::StringView>()));

        template<typename Bk,typename K, typename O, typename ... Args>
        using InsertTest = decltype(LD::Declval<Bk>().Insert(LD::Declval<K>(),LD::Declval<O>(),LD::Declval<Args>()...));

        template<typename Bk, typename ... Args>
        using BeginTest = decltype(LD::Declval<Bk>().Begin(LD::Declval<Args>()...));
        template<typename Bk, typename ... Args>
        using CommitTest = decltype(LD::Declval<Bk>().Commit(LD::Declval<Args>()...));
    public:

        BasicDatabase(const Db & backend):mBackend(backend)
        {

        }

        /**
         *
         * @tparam Key A typestring key to represent the key in the database
         * @tparam V The type of object which will be serialized into the backing data store
         * @tparam Args The arguements the be passed back with the Contextual Variant
         * @tparam VarType The set of Database Resultant Types for the contexual variant
         * @tparam Ret A typedef for the currently used Contextual variant
         * @tparam CurrentBackend A sfinae friendly assignment of the type of the currently used backend
         * @param key The key being used to insert the designated object
         * @param object The object to be serialized
         * @param args The arguements that will be passed forward within the Contextual Variant upon return
         * @return
         * @see LD::UnqliteDatabaseBackend
         * @see LD::TypeString
         * @code
         * int main()
         * {
         *      //the backend can be in any type you want, but at some point the elements concerning that type must be usable by
         *      //LD::BasicStringView<T>
         *      LD::UnqliteDatabaseBackend<char> backend{"database.db"};
         *
         *      LD::BasicDatabase<LD::UnQliteBackend<char>> database{currentBackend};
         *
         *      Square currentSuqre;
         *      currentSuqre["Length"_ts] = 7;
         *
         *      Pyramid currentPyramid;
         *      currentPyramid["Base"_ts]["Length"_ts] = 7;
         *      currentPyramid["Side"_ts]["Base"_ts] = 17;
         *      currentPyramid["Side"_ts]["Height"_ts] = 37;
         *      //it doesn't matter how you assign the values, it's all the same to database abstraction
         *      currentPyramid.Side() = LD::Triangle{37,521};
         *      currentPyramid.Base() = LD::Square{92};
         *
         *      //you can of course use auto here, but the full classname is presented for documentation purposes
         *      LD::ContextualVariant<LD::Variant<LD::DatabaseError,LD::DatabaseTransactionResult>()> insertResult =  database.InsertAndCommit("key"_ts,currentPyramid);
         *      auto onSuccessFullInsert = [](const LD::Context<LD::DatabaseTransactionResult> &)
         *      {
         *      }
         *      auto onDbInsertError = [](const LD::Context<LD::DatabaseError> &)
         *      {
         *      }
         *
         *      //the visitor pattern is a much cleaner way to deal with errors then exceptions or shoving std::optional everywhere
         *      //a more delcarative implementation is a less confusing implementation
         *      LD::Match(insertResult,onSuccessFullInsert,onDbInsertError);
         *
         *      //again auto is encouraged when using these statement as here.
         *      //If you're not sure what type is associated with the key, then you can simply provide a typelist of all expected types you want that key to be
         *      //It will simply go through each type in the order presented in the typelist from left to right and s top when it finds something
         *      //If no key is found to have those types then it will return a database error
         *      //the key is a type corresponding to that of LD::TypeString
         *      LD::ContextualVariant<LD::Variant<LD::DatabaseError,LD::DatabaseTransactionResult>(LD::Variant<Pyramid>)> queryResult = database.Fetch("key"_ts,LD::CT::TypeList<Pyramid>{});
         * }
         */
        template<typename Key,typename V, typename ... Args,
                typename VarType = LD::Variant<LD::DatabaseError,LD::DatabaseTransactionResult>,
                typename Ret = LD::ContextualVariant<VarType(Args...)>,
                typename CurrentBackend = Backend>
        LD::Enable_If_T<
                LD::Require<
                        LD::IsTypeString<Key>,
                        LD::IsReflectable<LD::Detail::Decay_T<V>>,
                        LD::Exists<InsertTest,BasicDatabase<CurrentBackend>,Key,V,Args...>,
                        LD::Exists<BeginTest,CurrentBackend>,
                        LD::Exists<CommitTest,CurrentBackend>
                >,Ret> InsertAndCommit(const Key & key,V && object, Args && ... args) noexcept
        {

            this->mBackend->Begin();
            auto resultant = this->Insert(key,LD::Forward<V>(object),LD::Forward<Args>(args)...);
            this->mBackend->Commit();
            return resultant;
        }
        template<typename Key,typename V, typename ... Args,
                typename VarType = LD::Variant<LD::DatabaseError,LD::DatabaseTransactionResult>,
                typename Ret = LD::ContextualVariant<VarType(Args...)>,
                typename CurrentBackend = Backend>
        LD::Enable_If_T<
                LD::Require<
                        LD::IsTypeString<Key>,
                        LD::IsReflectable<LD::Detail::Decay_T<V>>,
                        LD::Exists<StoreTest,CurrentBackend>
                >,Ret> Insert(const Key & key,V && object, Args && ... args) noexcept
        {

            using Type = LD::Detail::Decay_T<V>;
            using Var = LD::CT::RebindList<LD::CT::ReflectiveTransformation<Type ,LD::AccessReadOnly>,LD::Variant>;
            using QueryResultant = LD::Variant<LD::DatabaseError,LD::DatabaseTransactionResult>;
            using QueryResponse = LD::ContextualVariant<QueryResultant()>;
            auto onClass = [](const LD::Context<LD::StringView,LD::StringView,Db> & context) noexcept
            {
                LD::StringView  key = LD::Get<0>(context);
                LD::StringView className = LD::Get<1>(context);
                Db handle = LD::Get<2>(context);
                QueryResponse response = handle->Store(
                        LD::StringView{key.data(),key.size()},
                        LD::StringView{className.data(),className.size()});

                auto onDatabaseError = [](const LD::Context<LD::DatabaseError> & error) noexcept
                {
                    return false;
                };
                auto onTransaction = [](const LD::Context<LD::DatabaseTransactionResult> & error) noexcept
                {

                    return true;
                };
                return LD::Match(response,onDatabaseError,onTransaction);
            };

            auto onMember = [](const LD::ContextualVariant<Var(LD::StringView,Db)> & context) noexcept
            {

                auto onPrimitiveAction = [](auto && context) noexcept
                {
                    Db handle = LD::Get<2>(context);
                    using MemberType = LD::Detail::Decay_T<decltype(LD::Get(LD::Get<0>(context)))>;
                    bool queryResult = false;
                    if constexpr(LD::Require<LD::IsPrimitive<LD::Detail::Decay_T<decltype(LD::Get(LD::Get<0>(context)))>>>)
                    {
                        LD::StringView key = LD::Get<1>(context);
                        auto memberAsString = LD::ToImmutableString(LD::Get(LD::Get<0>(context)));
                        QueryResponse response = handle->Store(
                                LD::StringView{key.data(),key.size()},
                                LD::StringView{memberAsString.Data(),memberAsString.GetSize()});

                        auto onDatabaseError = [](const LD::Context<LD::DatabaseError> & error) noexcept
                        {
                            return false;
                        };
                        auto onTransaction = [](const LD::Context<LD::DatabaseTransactionResult> & error) noexcept
                        {

                            return true;
                        };
                        queryResult = LD::Match(response,onDatabaseError,onTransaction);
                        //std::cout << LD::Get<1>(context) << " : " << LD::Get(LD::Get<0>(context)) <<std::endl;
                    }
                    return queryResult;
                };
                return LD::Match(context,onPrimitiveAction);;
            };

            const bool result = LD::CT::ReflectiveWalk(key,LD::Forward<V>(object),onClass,onMember,LD::AccessReadOnly{},Db{this->mBackend});
            Ret possibleResults[2];
            possibleResults[0] = LD::MakeContext(LD::DatabaseError{},LD::Forward<Args>(args)...);
            possibleResults[1] = LD::MakeContext(LD::DatabaseTransactionResult{},LD::Forward<Args>(args)...);
            return possibleResults[result];
        }

        template<typename Y,typename Key, typename ... Args,
                typename VarType = LD::Variant<LD::DatabaseError,LD::DatabaseTransactionResult>,
                typename Boundtype = LD::CT::RebindList<Y,LD::Variant>,
                typename Ret = LD::ContextualVariant<VarType(Boundtype,Args...)>>
        LD::Enable_If_T<
                LD::Require<
                        LD::IsTypeString<Key>,
                        LD::IsTypeList<Y>::value
                >,Ret> Fetch(const Key & key, const Y & typelist ,Args && ... args) noexcept
        {
            constexpr LD::UInteger CurrentTypeListSize = Y::size();


            auto onClassReanimate = [](const LD::Context<LD::StringView,LD::StringView,Db> & context) noexcept -> bool
            {
                auto onFetch = [](const LD::Context<LD::StringView,LD::StringView,LD::StringView> & context) noexcept-> LD::UInteger
                {

                    LD::UInteger comparisonResult = LD::Get<1>(context) == LD::Get<2>(context);
                    return LD::UInteger {comparisonResult};

                };

                Db handle = LD::Get<2>(context);

                LD::ContextualVariant<LD::Variant<LD::DatabaseError,LD::DatabaseTransactionResult>(LD::StringView ,LD::UInteger,LD::StringView)> fetchContext;
                fetchContext = handle->Fetch(LD::StringView{LD::Get<0>(context)},onFetch,LD::StringView{LD::Get<1>(context)});

                auto onDatabaseError = [](const LD::Context<LD::DatabaseError,LD::StringView ,LD::UInteger,LD::StringView> &) noexcept
                {

                    return false;
                };

                auto onTransaction = [](const LD::Context<LD::DatabaseTransactionResult,LD::StringView ,LD::UInteger,LD::StringView> & transaction) noexcept
                {

                    return LD::Get<2>(transaction);
                };
                return LD::Match(fetchContext,onDatabaseError,onTransaction);
            };

            Ret returnable {};
            returnable = LD::MakeContext(LD::DatabaseError{},Boundtype{},LD::Forward<Args>(args)...);

            //iterate through all of the types in the typelist
            LD::For<CurrentTypeListSize>([](
                    auto I,
                    const Key & key,
                    auto && onClassReanimate,
                    const Backend & currentBackend,
                    Ret & returnable,
                    Args && ... args) noexcept
            {
                //get the transform for the current type at index I in the typelist
                using Type = typename LD::TypeAtIndex<I,Y>::type;
                //we're writing to the data structre eg why it's write only.
                using Var = LD::CT::RebindList<LD::CT::ReflectiveTransformation<Type ,LD::AccessWriteOnly>,LD::Variant>;
                using QueryResultant = LD::Variant<LD::DatabaseError,LD::DatabaseTransactionResult>;
                using QueryResponse = LD::ContextualVariant<QueryResultant()>;


                //actions to get stuff back into the object from the data store
                auto onMemberReanimate = [](const LD::ContextualVariant<Var(LD::StringView,Db)> & context) noexcept
                {
                    auto onPrimitiveAction = [](auto && context) noexcept
                    {
                        Db handle = LD::Get<2>(context);
                        using MemberType = LD::Detail::Decay_T<decltype(LD::Get(LD::Get<0>(context)))>;
                        bool performedQuery = false;
                        if constexpr(LD::Require<LD::IsPrimitive<LD::Detail::Decay_T<decltype(LD::Get(LD::Get<0>(context)))>>>)
                        {
                            LD::StringView memberKey = LD::Get<1>(context);
                            LD::ElementReference<MemberType> memberReference = LD::Get<0>(context);
                            auto onFetch = [](const LD::Context<LD::StringView,LD::StringView,LD::StringView,LD::ElementReference<MemberType>> & context) noexcept -> MemberType
                            {

                                auto resultVariant = LD::StringAsNumber<MemberType>(LD::StringView{LD::Get<1>(context).data(),LD::Get<1>(context).size()});
                                MemberType result = LD::Match(resultVariant,[](const MemberType & obj){ return obj;},[](auto &&){ return LD::UInteger {};});
                                LD::Get(LD::Get<3>(context)) = result;
                                return result;
                            };

                            LD::ContextualVariant<LD::Variant<LD::DatabaseError,LD::DatabaseTransactionResult>(LD::StringView ,MemberType,LD::StringView,LD::ElementReference<MemberType>)> fetchContext;

                            fetchContext = handle->Fetch(LD::StringView{memberKey},onFetch,LD::StringView{memberKey},LD::ElementReference<MemberType>{memberReference});


                            auto onDatabaseError = [](const LD::Context<LD::DatabaseError,LD::StringView ,MemberType ,LD::StringView,LD::ElementReference<MemberType>> &) noexcept
                            {

                                return false;
                            };

                            auto onTransaction = [](const LD::Context<LD::DatabaseTransactionResult,LD::StringView ,MemberType ,LD::StringView,LD::ElementReference<MemberType>> & transaction) noexcept
                            {

                                return true;
                            };
                            performedQuery = LD::Match(fetchContext,onDatabaseError,onTransaction);
                        }
                        return performedQuery;
                    };
                    return LD::Match(context,onPrimitiveAction);
                };
                //default construction of type being look at
                Type objectToReanimate;
                const bool result = LD::CT::ReflectiveWalk(
                        key,
                        objectToReanimate,
                        onClassReanimate,
                        onMemberReanimate,
                        LD::AccessWriteOnly{},
                        Db{currentBackend});

                //if we have a successful query with that classname then set the returnable and stop looping
                if (result)
                {
                    returnable = LD::MakeContext(LD::DatabaseTransactionResult{},Boundtype{objectToReanimate},LD::Forward<Args>(args)...);
                }

                //return false will stop the compile time for loop
                return !result;
            },key,LD::Forward<decltype(onClassReanimate)>(onClassReanimate),this->mBackend,returnable,LD::Forward<Args>(args)...);
            return returnable;
        }
    };
}
#endif //LANDESSDEVCORE_DATABASE_HPP