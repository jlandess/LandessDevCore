//
// Created by phoenixflower on 5/14/20.
//

#ifndef LANDESSDEVCORE_DATABASE_HPP
#define LANDESSDEVCORE_DATABASE_HPP
#include "TypeTraits/EnableIf.hpp"
#include "Definitions/Integer.hpp"
#include "Primitives/General/StringView.hpp"
#include "Reflection/reflectable.hpp"
#include "Reflection/Reflection.hpp"
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
#include "FetchRequest.h"
#include "TypeTraits/Type.h"
#include "Unicode/UTF8.hpp"
namespace LD
{

    /**
     *
     * @tparam Backend The backend responsible for the core operations. The imperative implementation of the class.  Each operation
     * will be considered individually instead of a all or none argument
     * @brief An abstraction layer of NoSQL implementations, SQL can be used if a key-value schema is supported, this requires all objects to be
     * accessed by a key. LD::BasicDatabase can be used for non-keyed databases (which uses this as its core)
     * @see LD::BasicDatabase
     */
    template<typename Backend>
    class BasicKeyedDatabase
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

        BasicKeyedDatabase(const Db & backend):mBackend(backend)
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
         * @brief Inserts the object into the database, if the object already exists, it simply overrides it.  There is no check for consitency,
         * the current predicate to determine which object model wins (if two models of the same class are being used) is the last one serialized wins.
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
                typename VarType = LD::Variant<LD::TransactionError,LD::DatabaseTransactionResult>,
                typename Ret = LD::QueryResult<LD::Ref<LD::Detail::Decay_T<V>>(Args...)>,
                typename CurrentBackend = Backend>
        LD::Enable_If_T<
                LD::Require<
                        LD::IsTypeString<Key>,
                        LD::IsReflectable<LD::Detail::Decay_T<V>>,
                        LD::Exists<InsertTest,BasicKeyedDatabase<CurrentBackend>,Key,V,Args...>,
                        LD::Exists<BeginTest,CurrentBackend>,
                        LD::Exists<CommitTest,CurrentBackend>
                >,Ret> InsertAndCommit(const Key & key,V && object, Args && ... args) noexcept
        {

            this->mBackend->Begin();
            auto resultant = this->Insert(key,LD::Forward<V>(object),LD::Forward<Args>(args)...);
            this->mBackend->Commit();
            return resultant;
        }

        /**
         *
         * @tparam Key
         * @tparam V
         * @tparam Args
         * @tparam VarType
         * @tparam Ret
         * @tparam CurrentBackend
         * @param key
         * @param object
         * @param args
         * @return A query result that either corresponds with a successful insert eg a LD::Context which contains LD::TransactionResult
         * or it will contain a LD::Context with LD::TransactionError
         * @brief This Requires that all classnames, and keys are UT8-Compliant.  No exception will be thrown, the result will contain
         * a LD::TransactionError and the specific error in LD::TransationError will be of type LD::EncodingError
         */
        template<typename Key,typename V, typename ... Args,
                typename VarType = LD::Variant<LD::TransactionError,LD::DatabaseTransactionResult>,
                typename Ret = LD::QueryResult<LD::Ref<LD::Detail::Decay_T<V>>(Args...)>,
                typename CurrentBackend = Backend>
        LD::Enable_If_T<
                LD::Require<
                        LD::IsTypeString<Key>,
                        LD::CT::IsReflectable<LD::Detail::Decay_T<V>>(),
                        LD::Exists<StoreTest,CurrentBackend>
                >,Ret> Insert(const Key & key,V && object, Args && ... args) noexcept
        {
            constexpr auto traits = LD::CT::Reflect<LD::Detail::Decay_T<V>>();
            constexpr auto members = traits.Members;
            constexpr auto NumberOfMembers = LD::CT::GetNumberOfMembers(traits);
            constexpr auto className = LD::CT::GetClassName(traits);
            using ClassNameType = LD::Detail::Decay_T<decltype(className)>;
            using Ctxt = LD::Context<LD::TransactionResult,LD::UInteger,Key&,Db,ClassNameType&,LD::StaticArray<char,1024>&>;
            //use to append strings without allocating
            LD::StaticArray<char,1024> buffer;
            //LD::CT::DebugTemplate<decltype(className)>{};
            //We need to get the actual length of the strings, given that it can be ASCII or Unicode based Strings
            auto classNameLength = LD::UTF8::Distance(
                    (char*)className.content,
                    (char*)className.content+className.size(),
                    key,
                    Db{this->mBackend},
                    LD::Ref<ClassNameType>{className},
                    buffer);
            auto keyLength = LD::UTF8::Distance(
                    key.cbegin(),
                    key.cend(),
                    key,
                    Db{this->mBackend},
                    LD::Ref<ClassNameType>{className},
                    buffer);
            //if one of the strings does not correspond with UT8 encoding then we simply go to the fallthrough [](auto,auto){}
            bool shouldContinue = LD::MultiMatch(LD::Overload
            {
                [](auto,auto ) noexcept -> bool
                {
                    return false;
                },
                [](const Ctxt & cntx1, const Ctxt & cntx2)noexcept -> bool
                {
                    //we now have the calculated lengths, and now we can assume both strings are UTF-8 Compliant
                    LD::UInteger calculatedClassNameLength = LD::Get(LD::Get<1>(cntx1));
                    LD::UInteger calculatedKeyLength = LD::Get(LD::Get<1>(cntx2));
                    auto & buffer = LD::Get(LD::Get<5>(cntx1));
                    const auto & key = LD::Get(LD::Get<2>(cntx1));
                    const auto & className = LD::Get(LD::Get<4>(cntx1));
                    //a way to append to the string without allocating.  A key can have a max size of 1 KB
                    LD::BackInserter<LD::StaticArray<char,1024>> nosqlKey{buffer};
                    auto keyIT = LD::UTF8::Begin(key);
                    auto classNameIT = LD::UTF8::Begin(className);
                    //get the key into an encoded format
                    LD::QueryResult<bool()> unicodeAppendingState = LD::MakeContext(LD::TransactionResult{},true);
                    for(LD::UInteger n = 0;n<calculatedKeyLength && LD::IsTransactionalQuery(unicodeAppendingState);++n)
                    {
                        auto possibleCharacter =  LD::UTF8::Next(keyIT,key.cend(),nosqlKey);
                        auto onKey = [](const LD::Context<LD::TransactionResult,uint32_t,LD::BackInserter<LD::StaticArray<char,1024>>&> & context) noexcept -> LD::QueryResult<bool()>
                        {
                            uint32_t cp = LD::Get(LD::Get<1>(context));
                            LD::BackInserter<LD::StaticArray<char,1024>>& nosqlKeyBackInserter = LD::Get(LD::Get<2>(context));
                            LD::UTF8::Append(cp,nosqlKeyBackInserter);
                            return LD::MakeContext(LD::TransactionResult{},true);
                        };
                        auto onError = [](const LD::Context<LD::TransactionError,LD::BackInserter<LD::StaticArray<char,1024>>&> &) noexcept-> LD::QueryResult<bool()>
                        {
                            return LD::MakeContext(LD::TransactionError{});
                        };
                        unicodeAppendingState = LD::Match(possibleCharacter,LD::Overload{onKey,onError});
                    }
                    auto backend  = LD::Get<3>(cntx1);
                    auto numberOfBytesInClassName = LD::UTF8::NumberOfBytes(className);
                    auto numberOfBytesInKey = LD::UTF8::NumberOfBytes(key);
                    //accounting for the period which has to be put between the key
                    //auto totalNumberOfBytes = numberOfBytesInClassName+numberOfBytesInKey+1;
                    //store the class name, it works for variably encoded (UTF-8) strings
                    auto storeResult =  backend->Store(LD::StringView{key.data(),numberOfBytesInKey},LD::StringView{(char*)LD::UTF8::Begin(className),numberOfBytesInClassName});
                    //we got to store the data in the backing store


                    return LD::MultiMatch(LD::Overload
                    {
                        [](auto,auto){ return false;},
                        [](const LD::Context<LD::TransactionResult,bool> & storeContext, const LD::Context<LD::TransactionResult,bool> & appendResult) noexcept
                        {
                            return true;
                        }

                    },storeResult,unicodeAppendingState);
                }
            },classNameLength,keyLength);

            if (shouldContinue)
            {
                LD::For<NumberOfMembers>([](
                        auto I,
                        auto members,
                        V && object,
                        BasicKeyedDatabase * keyedDatabase,
                        Args && ... arguments) noexcept
                {
                    constexpr auto memberInfo = LD::Get<I>(members);
                    constexpr auto type = LD::CT::GetDescriptorType(memberInfo);
                    auto memberAccessor = memberInfo(LD::Forward<V>(object));
                    //wchar_t buffer[256];
                    //todo - append the member name
                    if constexpr (LD::CT::IsPrimitive(LD::CT::RemoveQualifiers(type)))

                    {
                        auto immutableRepresentation = LD::ToImmutableString(memberAccessor());
                        //LD::ToImmutableString(primitive);

                    }else if constexpr (LD::CT::IsReflectable(type))
                    {
                        keyedDatabase->Insert(""_ts,memberAccessor(),LD::Forward<Args>(arguments)...);
                    }
                    return true;

                    },LD::Forward<decltype(members)>(members),LD::Forward<V>(object),this,LD::Forward<Args>(args)...);
            }

            /*
            using Type = LD::Detail::Decay_T<V>;
            using Var = LD::CT::RebindList<LD::CT::ReflectiveTransformation<Type ,LD::AccessReadOnly>,LD::Variant>;
            auto onClass = [](const LD::Context<LD::StringView,LD::StringView,Db> & context) noexcept
            {
                LD::StringView  key = LD::Get<0>(context);
                LD::StringView className = LD::Get<1>(context);
                Db handle = LD::Get<2>(context);
                LD::QueryResult<bool()> response = handle->Store(
                        LD::StringView{key.data(),key.size()},
                        LD::StringView{className.data(),className.size()});

                auto onDatabaseError = [](const LD::Context<LD::TransactionError> & error) noexcept
                {
                    return false;
                };
                auto onTransaction = [](const LD::Context<LD::TransactionResult,bool> & error) noexcept
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
                        LD::QueryResult<bool()> response = handle->Store(
                                LD::StringView{key.data(),key.size()},
                                LD::StringView{memberAsString.Data(),memberAsString.GetSize()});

                        auto onDatabaseError = [](const LD::Context<LD::TransactionError> & error) noexcept
                        {
                            return false;
                        };
                        auto onTransaction = [](const LD::Context<LD::TransactionResult,bool> & error) noexcept
                        {

                            return true;
                        };
                        queryResult = LD::Match(response,onDatabaseError,onTransaction);
                    }
                    return queryResult;
                };
                return LD::Match(context,onPrimitiveAction);
            };

            const bool result = LD::CT::ReflectiveWalk(key,LD::Forward<V>(object),onClass,onMember,LD::AccessReadOnly{},Db{this->mBackend});
            Ret possibleResults[2];
            possibleResults[0] = LD::MakeContext(LD::TransactionError{},LD::Forward<Args>(args)...);
            possibleResults[1] = LD::MakeContext(LD::TransactionResult{},LD::Ref<LD::Detail::Decay_T<V>>{object},LD::Forward<Args>(args)...);
            return possibleResults[result];
             */
            return {};
        }

        /*
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
         */

        //typename Ret = LD::ContextualVariant<VarType(Boundtype,Args...)>
        /**
         *
         * @tparam Y Is expted to be LD::TypeList<stuff...>, each type specified in the type list indicates what types to search for
         * @tparam Key A typestring that is expted to be the key to look for in the backing store
         * @tparam Args The arguements that should be made available in the calleables for the end LD::FetchRequestResult
         * @tparam Ret The Type of LD::FetchRequestResult which should be generated
         * @param key The instansiation of the specified LD::TypeString
         * @param typelist The instansiation of the TypeList to indicate what types could coorelate to the indicated key
         * @param args The parameters that should be made available in the calleables
         * @see LD::CT::TypeList
         * @see LD::FetchRequestResult
         * @see LD::TypeString
         * @return A generated instansiation of LD::FetchRequestResult which will hold the result of the query performed
         * @brief Fetches an entity with the associated types and given key.  Providing a typelist allows the calling code to help speed up the process of
         * querying the database multiple times for various types.  Instead it just does it in an optimal way.  This feeds into the notion that
         * the type that's in the database does not really matter only that it is typed at all.  Providing various routes of interaction.
         * @code
         * int main()
         * {
         *      LD::UnQliteBackend<char> currentBackend{LD::StringView {"backend.db"},OpenAndCreateIfNotExists{}};
         *      LD::BasicDatabase<LD::UnQliteBackend<char>> database{currentBackend};
         *      //fetch an entity with a key "key" that is exactly of the type LD::Pyramid
         *      LD::FetchRequestResult<LD::Variant<LD::Pyramid>()> fetchResult =  database.Fetch("key"_ts,LD::CT::TypeList<Pyramid>{});
         *      auto onFetchError = [](const LD::Context<LD::DatabaseError> & context )
         *      {
         *      };
         *      auto onFetchTransaction = [](const LD::Context<LD::DatabaseTransactionResult,Pyramid> & context)
         *      {
         *          std::cout << "fetch1 query result " << std::endl;
         *          Pyramid & pyramidToUse = LD::Get<1>(context);
         *          std::cout << pyramidToUse["Base"_ts]["Length"_ts] << std::endl;
         *          std::cout << pyramidToUse["Side"_ts]["Base"_ts] << std::endl;
         *          std::cout << pyramidToUse["Side"_ts]["Height"_ts] << std::endl;
         *      };
         *      LD::Match(fetchResult,onFetchError,onFetchTransaction);
         *      return 0;
         * }
         */
        template<typename Y,typename Key, typename ... Args,
                typename Ret = LD::QueryResult<LD::CT::RebindList<Y,LD::Variant>(Args...)>>
        LD::Enable_If_T<
                LD::Require<
                        LD::IsTypeString<Key>,
                        LD::IsTypeList<Y>::value
                >,Ret> Fetch(const Key & key, const Y & typelist ,Args && ... args) noexcept
        {
            constexpr LD::UInteger CurrentTypeListSize = Y::size();


            auto onClassReanimate = [](const LD::Context<LD::StringView,LD::StringView,Db> & context) noexcept -> bool
            {
                //auto onFetch = [](const LD::Context<LD::StringView,LD::StringView,LD::StringView> & context) noexcept-> LD::UInteger
                //{

                    //LD::UInteger comparisonResult = LD::Get<1>(context) == LD::Get<2>(context);
                    //return LD::UInteger {comparisonResult};

                //};

                auto onFetch = [](const LD::Context<LD::StringView,LD::StringView,LD::StringView> & context)
                {
                    const LD::StringView & data = LD::Get(LD::Get<1>(context));
                    const LD::StringView & className = LD::Get(LD::Get<2>(context));
                    LD::UInteger comparisonResult = (data == className);
                    return comparisonResult;
                };
                Db handle = LD::Get<2>(context);

                //LD::ContextualVariant<LD::Variant<LD::DatabaseError,LD::DatabaseTransactionResult>(LD::StringView ,LD::UInteger,LD::StringView)> fetchContext;

                LD::QueryResult<LD::UInteger(LD::StringView,LD::StringView)> queryResult = handle->Fetch(
                        LD::StringView {LD::Get<0>(context)},
                        onFetch,
                        LD::StringView{LD::Get<1>(context)});

                //fetchContext = handle->Fetch(LD::StringView{LD::Get<0>(context)},onFetch,LD::StringView{LD::Get<1>(context)});

                auto onDatabaseError = [](const LD::Context<LD::TransactionError,LD::StringView,LD::StringView> &) noexcept
                {

                    return false;
                };

                auto onTransaction = [](const LD::Context<LD::TransactionResult,LD::UInteger,LD::StringView ,LD::StringView> & transaction) noexcept
                {

                    return LD::Get<1>(transaction);
                };
                //return LD::Match(fetchContext,onDatabaseError,onTransaction);
                return LD::Match(queryResult,onDatabaseError,onTransaction);
            };

            Ret returnable {};
            returnable = LD::MakeContext(LD::TransactionError{},LD::Forward<Args>(args)...);


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
                       using QueryResultant = LD::Variant<LD::TransactionError,LD::DatabaseTransactionResult>;
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



                                   auto onFetch = [](const LD::Context<LD::StringView,LD::StringView,LD::ElementReference<MemberType>> & context) noexcept -> MemberType
                                   {
                                       auto resultVariant = LD::StringAsNumber<MemberType>(LD::StringView{LD::Get<1>(context).data(),LD::Get<1>(context).size()});
                                       MemberType result = LD::Match(resultVariant,[](const MemberType & obj){ return obj;},[](auto &&){ return MemberType {};});
                                       LD::Get(LD::Get<2>(context)) = result;
                                       return result;
                                   };


                                   LD::QueryResult<MemberType(LD::StringView,LD::Ref<MemberType>)> queryResult = handle->Fetch(
                                           LD::StringView{memberKey},
                                           onFetch,
                                           LD::Ref<MemberType>{memberReference});




                                   auto onDatabaseError = [](const LD::Context<LD::TransactionError,LD::StringView ,LD::ElementReference<MemberType>> &) noexcept
                                   {
                                       return false;
                                   };


                                   auto onTransaction = [](const LD::Context<LD::TransactionResult,MemberType ,LD::StringView,LD::ElementReference<MemberType>> & transaction) noexcept
                                   {


                                       return true;

                                   };

                                   performedQuery =  LD::Match(queryResult,onDatabaseError,onTransaction);
                                   //performedQuery = LD::Match(fetchContext,onDatabaseError,onTransaction);

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
                       LD::IF(result,[](Ret & returnable, Type & objectToReanimate, Args && ... args)
                       {
                           returnable = LD::MakeContext(LD::TransactionResult{},Type {objectToReanimate},LD::Forward<Args>(args)...);
                       }
                       ,returnable,objectToReanimate,LD::Forward<Args>(args)...);
                       /*
                       if (result)
                       {
                           returnable = LD::MakeContext(LD::DatabaseTransactionResult{},Type {objectToReanimate},LD::Forward<Args>(args)...);
                       }
                        */
                       //return false will stop the compile time for loop
                       return !result;
                    },key,LD::Forward<decltype(onClassReanimate)>(onClassReanimate),this->mBackend,returnable,LD::Forward<Args>(args)...);

            return returnable;
        }

        /*
        template<typename Key, typename ... TL ,typename ... Args>
        LD::QueryResult<bool(Args...)> Remove(const Key & key, LD::CT::TypeList<TL...> ,Args && ... arguments) noexcept
        {

            using RefedResult = LD::Ref<LD::QueryResult<bool(Args...)>>;
            using ReferencedInstance = LD::Ref<BasicDatabase>;
            LD::QueryResult<bool(Args...)> result = LD::MakeContext(LD::DatabaseError{},LD::Forward<Args>(arguments)...);
            using Period = LD::TypeString<'.'>;
            LD::For<sizeof...(TL)>([](
                    auto I,
                    LD::Ref<Backend> handle,
                    RefedResult result,
                    ReferencedInstance currentInstance,
                    Args && ... arguments)
            {
                using Type = typename LD::TypeAtIndex<I,LD::CT::TypeList<TL...>>::type;
                using ClassName = decltype(Type::GetClassNameTypeString());
                using KeySet  = typename LD::tlist_erase_at<0,LD::CT::RebindList<typename Type::KeyTypeList,LD::CT::TypeList>>::type;
                using MemberSet  = typename LD::tlist_erase_at<0,LD::CT::RebindList<typename Type::ValueTypeList,LD::CT::TypeList>>::type;

                auto onFetch = [](const LD::Context<LD::StringView,LD::StringView,LD::StringView,LD::Ref<Backend>,RefedResult> & context) noexcept
                {
                    const LD::StringView & data = LD::Get(LD::Get<1>(context));
                    const LD::StringView & className = LD::Get(LD::Get<2>(context));
                    LD::UInteger comparisonResult = (data == className);
                    return comparisonResult;
                };


                LD::QueryResult<LD::UInteger(LD::StringView,LD::StringView,LD::Ref<Backend>,RefedResult)> queryResult = handle->Fetch(
                        LD::StringView {Key::data(),Key::size()},
                        onFetch,
                        LD::StringView{ClassName::data(),ClassName::size()},
                        LD::Ref<Backend>{handle},
                        RefedResult {result});



                auto onDatabaseError = [](const LD::Context<LD::DatabaseError,LD::StringView ,LD::StringView,LD::Ref<Backend>,RefedResult> &) noexcept
                {

                    return true;
                };

                auto onTransaction = [](const LD::Context<LD::DatabaseTransactionResult,LD::UInteger,LD::StringView ,LD::StringView,LD::Ref<Backend>,RefedResult> & transaction) noexcept
                {
                    using ReflectiveTypeStructure =  LD::CT::GenerateNamedReflectiveTypeStructure<Key,Type>;

                    LD::Ref<Backend> handle = LD::Get<4>(transaction);
                    RefedResult  res = LD::Get<5>(transaction);
                    using Keys = typename LD::CT::TypeListChannelView<0,2,ReflectiveTypeStructure>;
                    using Types = typename LD::CT::TypeListChannelView<1,2,ReflectiveTypeStructure>;

                    //iterate through all of the keys which were generate with LD::GenerateNamedReflectiveTypeStructure
                    //delete keys directly which are of type Reflective or Primitive Types
                    //provide a special case for arrays and iterable structures
                    LD::For<0,ReflectiveTypeStructure::size()/2,1>([](
                            auto I,
                            LD::Ref<Backend> handle)
                    {
                        using CurrentKey = LD::CT::TypeAtIndex<I,Keys>;
                        using CurrentType = LD::CT::TypeAtIndex<I,Types>;
                        if constexpr(LD::IsPrimitive<CurrentType> || LD::IsReflectable<CurrentType>)
                        {
                            LD::QueryResult<bool()> currentRemovalRequestRes = handle->Remove(LD::StringView{CurrentKey::data(),CurrentKey::size()});

                            auto onRemovalError = [](const LD::Context<LD::DatabaseError> & context) noexcept
                            {

                            };

                            auto onTransaction = [](const LD::Context<LD::DatabaseTransactionResult,bool> & context) noexcept
                            {

                            };

                            LD::Match(currentRemovalRequestRes,onRemovalError,onTransaction);
                        }

                        return true;
                    },handle);
                    //(*res) = LD::MakeContext(LD::DatabaseTransactionResult{},bool{});
                    return false;
                };
                bool shouldContinueAttemptingToDelete = LD::Match(queryResult,onDatabaseError,onTransaction);

                if (!shouldContinueAttemptingToDelete)//we've found the type that we wanted to delete
                {
                    (*result) = LD::MakeContext(LD::DatabaseTransactionResult{},bool{true},LD::Forward<Args>(arguments)...);
                }
                //we're writing to the data structre eg why it's write only.
                return shouldContinueAttemptingToDelete;
            },LD::Ref<Backend>{this->mBackend},RefedResult{result},ReferencedInstance{this},LD::Forward<Args>(arguments)...);

            //todo at object traversal to remove all keys
            return result;
        }

         */
        template<typename Key, typename ... TL ,typename ... Args>
        LD::Enable_If_T<
                (LD::Require<LD::IsReflectable<TL>> && ...)
        ,
        LD::QueryResult<LD::Variant<LD::Type<TL>...>(Args...)>> Remove(const Key & key, LD::CT::TypeList<TL...> ,Args && ... arguments) noexcept
        {

            using RefedResult = LD::Ref<LD::QueryResult<LD::Variant<LD::Type<TL>...>(Args...)>>;
            using ReferencedInstance = LD::Ref<BasicKeyedDatabase>;
            //set a default value that simply assumes the database had an error
            LD::QueryResult<LD::Variant<LD::Type<TL>...>(Args...)> result = LD::MakeContext(LD::TransactionError{},LD::Forward<Args>(arguments)...);
            //iterate through all the possible types we would like to consider in reference to the given key to remove from the backing data store
            LD::For<sizeof...(TL)>([](
                    auto I,
                    LD::Ref<Backend> handle,
                    RefedResult result,
                    ReferencedInstance currentInstance,
                    Args && ... arguments)
                    {


                        using CurrentType = typename LD::TypeAtIndex<I,LD::CT::TypeList<TL...>>::type;
                        using ClassName = decltype(CurrentType::GetClassNameTypeString());
                        using KeySet  = typename LD::tlist_erase_at<0,LD::CT::RebindList<typename CurrentType::KeyTypeList,LD::CT::TypeList>>::type;
                        using MemberSet  = typename LD::tlist_erase_at<0,LD::CT::RebindList<typename CurrentType::ValueTypeList,LD::CT::TypeList>>::type;

                        //compare the key's classname with the current classname we're looking at.
                        auto onFetch = [](const LD::Context<LD::StringView,LD::StringView,LD::StringView,LD::Ref<Backend>,RefedResult> & context) noexcept
                        {

                            const LD::StringView & data = LD::Get(LD::Get<1>(context));
                            const LD::StringView & className = LD::Get(LD::Get<2>(context));
                            LD::UInteger comparisonResult = (data == className);
                            return comparisonResult;
                        };


                        //determine if the key exists and if it does, does it refer to the class we're attempting to delete
                        LD::QueryResult<LD::UInteger(LD::StringView,LD::StringView,LD::Ref<Backend>,RefedResult)> queryResult = handle->Fetch(
                                LD::StringView {Key::data(),Key::size()},
                                onFetch,
                                LD::StringView{ClassName::data(),ClassName::size()},
                                LD::Ref<Backend>{handle},
                                RefedResult {result});



                        auto onDatabaseError = [](const LD::Context<LD::TransactionError,LD::StringView ,LD::StringView,LD::Ref<Backend>,RefedResult> &) noexcept
                        {
                            //todo - differentiate between an IO error, a not found error, and other errors
                            //continue iterating if we don't find it
                            return true;
                        };

                        auto onTransaction = [](const LD::Context<LD::TransactionResult,LD::UInteger,LD::StringView ,LD::StringView,LD::Ref<Backend>,RefedResult> & transaction) noexcept
                        {
                            using ReflectiveTypeStructure =  LD::CT::GenerateNamedReflectiveTypeStructure<Key,CurrentType>;
                            LD::Ref<Backend> handle = LD::Get<4>(transaction);
                            RefedResult  res = LD::Get<5>(transaction);
                            //we're treating the generate structure in the same way as a packed array
                            //the names of each member (typestrings) are at even indices
                            //the actual types that correspond to those names are at odd indices
                            using Keys = typename LD::CT::TypeListChannelView<0,2,ReflectiveTypeStructure>;
                            using Types = typename LD::CT::TypeListChannelView<1,2,ReflectiveTypeStructure>;
                            //remove the key that refers to the classname
                            handle->Remove(LD::StringView{Key::data(),Key::size()});
                            //iterate through all of the keys which were generate with LD::GenerateNamedReflectiveTypeStructure
                            //delete keys directly which are of type Reflective or Primitive Types
                            //provide a special case for arrays and iterable structures
                            LD::For<0,ReflectiveTypeStructure::size()/2,1>([](
                                    auto I,
                                    LD::Ref<Backend> handle)
                            {
                                using CurrentKey = LD::CT::TypeAtIndex<I,Keys>;
                                using CurrentType = LD::CT::TypeAtIndex<I,Types>;
                                if constexpr(LD::IsPrimitive<CurrentType> || LD::IsReflectable<CurrentType>)
                                {
                                    LD::QueryResult<bool()> currentRemovalRequestRes = handle->Remove(LD::StringView{CurrentKey::data(),CurrentKey::size()});

                                    auto onRemovalError = [](const LD::Context<LD::TransactionError> & context) noexcept
                                    {

                                    };
                                    auto onTransaction = [](const LD::Context<LD::TransactionResult,bool> & context) noexcept
                                    {

                                    };
                                    LD::Match(currentRemovalRequestRes,onRemovalError,onTransaction);
                                }
                                return true;
                                },handle);
                            //stop iterating if we found the classname
                            return false;
                        };
                        bool shouldContinueAttemptingToDelete = LD::Match(queryResult,onDatabaseError,onTransaction);
                        if (!shouldContinueAttemptingToDelete)//we've found the type that we wanted to delete
                        {


                            (*result) = LD::MakeContext(LD::TransactionResult{},LD::Type<CurrentType>{},LD::Forward<Args>(arguments)...);
                        }
                        //we're writing to the data structre eg why it's write only.
                        return shouldContinueAttemptingToDelete;
                    },LD::Ref<Backend>{this->mBackend},RefedResult{result},ReferencedInstance{this},LD::Forward<Args>(arguments)...);

            //todo at object traversal to remove all keys
            return result;
        }

        template<typename TS, typename MemberTS ,typename Comparable ,typename ... Types, typename ... Args,
                typename TList = LD::CT::TypeList<Types...>,
                typename ComparisonType = decltype(LD::Declval<LD::CT::TypeAtIndex<0,TList>>()[MemberTS{}])>
        LD::Enable_If_T<
                LD::Require<
                        LD::ConvertiblyCallable<Comparable,bool(const ComparisonType & a, const ComparisonType & b)>::Value(),
                        (LD::IsReflectable<Types> && ...),
                        (TList::size() > 0)
                        >
        ,
        LD::QueryResult<LD::Variant<Types...>(Args...)>> FetchWithPredicate(
                const TS & key,
                const LD::CT::TypeList<Types...> & ts,
                const MemberTS & tsKey ,
                const ComparisonType & constant ,
                Comparable && comparable,
                Args && ... arguments) noexcept
        {
            return {};
        }
    };
}
#endif //LANDESSDEVCORE_DATABASE_HPP