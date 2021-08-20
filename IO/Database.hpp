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
#include "Unicode/IsUTF8Compliant.hpp"
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
        template<typename Bk>
        using StoreTest = decltype(LD::Declval<Bk>().Store(LD::Declval<LD::StringView>(),LD::Declval<LD::StringView>()));
        template<typename Bk,typename K, typename O, typename ... Args>
        using InsertTest = decltype(LD::Declval<Bk>().Insert(LD::Declval<K>(),LD::Declval<O>(),LD::Declval<Args>()...));
        template<typename Bk, typename ... Args>
        using BeginTest = decltype(LD::Declval<Bk>().Begin(LD::Declval<Args>()...));
        template<typename Bk, typename ... Args>
        using CommitTest = decltype(LD::Declval<Bk>().Commit(LD::Declval<Args>()...));

        template<typename T>
        using UTFBeginTest = decltype(LD::UTF8::Begin(LD::Declval<T>()));
        template<typename T>
        using UTFBEndTest = decltype(LD::UTF8::End(LD::Declval<T>()));

        using Db = LD::ElementReference<Backend>;
    private:
        LD::StaticArray<char,1024> mKeyBuffer;
        Db mBackend;

        template<typename Key>
        auto IncorporateKey(Key && key) noexcept -> LD::QueryResult<bool()>
        {
            auto keyLength = LD::UTF8::Distance(
                    LD::UTF8::Begin(LD::Forward<Key>(key)),
                    LD::UTF8::End(LD::Forward<Key>(key)),
                    LD::ElementReference<BasicKeyedDatabase>{this},
                    LD::ElementReference<Key>{LD::Forward<Key>(key)});

            auto onError = [](const LD::Context<LD::TransactionError,LD::ElementReference<BasicKeyedDatabase>,LD::ElementReference<Key>> & context) noexcept-> LD::QueryResult<bool()>
            {
                return LD::MakeContext(LD::TransactionError{});
            };

            auto onLength = [](const LD::Context<LD::TransactionResult,LD::UInteger,LD::ElementReference<BasicKeyedDatabase>,LD::ElementReference<Key>> & context) noexcept -> LD::QueryResult<bool()>
            {
                auto length = LD::Get(LD::Get<1>(context));
                auto keyedDatabase = LD::Get<2>(context);
                const auto & key = LD::Get(LD::Get<3>(context));
                LD::BackInserter<LD::StaticArray<char,1024>> backInserter{keyedDatabase->mKeyBuffer};
                using IT = LD::Detail::Decay_T<decltype(backInserter)>;
                LD::QueryResult<IT()> unicodeAppendingState = LD::MakeContext(LD::TransactionResult{},IT{backInserter});
                for(auto i = 0;i<length && LD::IsTransactionalQuery(unicodeAppendingState);++i)
                {
                    //add the characters to the buffer, in the case of Unicode characters it will add the multi-byte representations
                    unicodeAppendingState = LD::UTF8::Append(key[i],backInserter);
                }

                auto onTransaction = [](const LD::Context<LD::TransactionResult,IT> & ) noexcept -> LD::QueryResult<bool()>
                {
                    return LD::MakeContext(LD::TransactionResult{},true);
                };

                auto onError = [](const LD::Context<LD::TransactionError> &) noexcept -> LD::QueryResult<bool()>
                {
                    return LD::MakeContext(LD::TransactionError{});
                };
                return LD::Match(unicodeAppendingState,onTransaction,onError);
            };

            return LD::Match(keyLength,onError,onLength);
        }

        template<typename Key>
        static auto IncorporateKey(Key && key, LD::StaticArray<char,1024> & keyedBuffer) noexcept -> LD::QueryResult<bool()>
        {

            auto keyLength = LD::UTF8::Distance(
                    LD::UTF8::Begin(LD::Forward<Key>(key)),
                    LD::UTF8::End(LD::Forward<Key>(key)),
                    LD::ElementReference<LD::StaticArray<char,1024>>{keyedBuffer},
                    LD::ElementReference<Key>{LD::Forward<Key>(key)});

            auto onError = [](const LD::Context<LD::TransactionError,LD::StaticArray<char,1024> &,LD::ElementReference<Key>> & context) noexcept-> LD::QueryResult<bool()>
            {
                return LD::MakeContext(LD::TransactionError{});
            };

            auto onLength = [](const LD::Context<LD::TransactionResult,LD::UInteger,LD::StaticArray<char,1024> &,LD::ElementReference<Key>> & context) noexcept -> LD::QueryResult<bool()>
            {

                auto length = LD::Get(LD::Get<1>(context));

                LD::StaticArray<char,1024>  & bufferToUse = LD::Get(LD::Get<2>(context));

                const auto & key = LD::Get(LD::Get<3>(context));

                LD::BackInserter<LD::StaticArray<char,1024>> backInserterToUse{bufferToUse};
                //backInserterToUse = 'a';
                using IT = LD::Detail::Decay_T<decltype(backInserterToUse)>;

                LD::QueryResult<IT()> unicodeAppendingState = LD::MakeContext(LD::TransactionResult{},IT{backInserterToUse});

                for(auto i = 0;i<length && LD::IsTransactionalQuery(unicodeAppendingState);++i)
                {
                    unicodeAppendingState = LD::UTF8::Append(key[i],backInserterToUse);
                }


                auto onTransaction = [](const LD::Context<LD::TransactionResult,IT> & ) noexcept -> LD::QueryResult<bool()>
                {
                    return LD::MakeContext(LD::TransactionResult{},true);
                };

                auto onError = [](const LD::Context<LD::TransactionError> &) noexcept -> LD::QueryResult<bool()>
                {
                    return LD::MakeContext(LD::TransactionError{});
                };
                return LD::Match(unicodeAppendingState,onTransaction,onError);
            };

            return LD::Match(keyLength,onError,onLength);
        }
    public:
        constexpr BasicKeyedDatabase(const Db & backend) noexcept :mBackend(backend)
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
                        LD::IsUTF8Compliant<Key>,
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


        template<typename Key,typename V, typename ... Args,
                typename VarType = LD::Variant<LD::TransactionError,LD::DatabaseTransactionResult>,
                typename Ret = LD::QueryResult<bool(Args...)>,
                typename CurrentBackend = Backend>
        constexpr LD::Enable_If_T<
                LD::Require<
                        LD::IsUTF8Compliant<Key>,
                        LD::CT::IsReflectable<LD::Detail::Decay_T<V>>(),
                        LD::Exists<StoreTest,CurrentBackend>
                >,Ret> Insert(Key && key,V && object, Args && ... args) noexcept
        {
           //clear the keybuffer of the previous insertion process
           this->mKeyBuffer.Clear();
           auto keyInjection = IncorporateKey(LD::Forward<Key>(key));
           //Ret state = LD::MakeContext(LD::TransactionResult{},LD::Ref<V>{LD::Forw})
            //if successful start inserting the object
            if(LD::IsTransactionalQuery(keyInjection))
            {

                return this->__Insert(LD::Forward<V>(object),LD::Forward<Args>(args)...);
            }
            return LD::MakeContext(LD::TransactionError{},LD::Forward<Args>(args)...);
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
        template<typename V, typename ... Args,
                typename VarType = LD::Variant<LD::TransactionError,LD::DatabaseTransactionResult>,
                typename Ret = LD::QueryResult<bool(Args...)>,
                typename CurrentBackend = Backend>
        constexpr LD::Enable_If_T<
                LD::Require<
                        LD::CT::IsReflectable<LD::Detail::Decay_T<V>>(),
                        LD::Exists<StoreTest,CurrentBackend>
                >,Ret> __Insert(V && object, Args && ... args) noexcept
        {
            constexpr auto traits = LD::CT::Reflect<LD::Detail::Decay_T<V>>();
            constexpr auto members = traits.Members;
            constexpr auto NumberOfMembers = LD::CT::GetNumberOfMembers(traits);
            constexpr auto className = LD::CT::GetClassName(traits);


            using ClassNameType = LD::Detail::Decay_T<decltype(className)>;
            using Ctxt = LD::Context<LD::TransactionResult,LD::UInteger,Db,ClassNameType&,LD::Ref<BasicKeyedDatabase>>;
            //We need to get the actual length of the strings, given that it can be ASCII or Unicode based Strings
            auto classNameLength = LD::UTF8::Distance(
                    LD::UTF8::Begin(className),
                    LD::UTF8::End(className),
                    Db{this->mBackend},
                    LD::Ref<ClassNameType>{className},
                    LD::Ref<BasicKeyedDatabase>{this});
            bool shouldContinue = LD::MultiMatch(LD::Overload
            {
                //if the classname is not a valid UTF-8 string, then we will indicate that with false
                [](auto ) noexcept -> bool
                {

                    return false;
                },
                //when we have a valid UTF-8 string we will attempt to append to the underlying buffer
                [](const Ctxt & cntx1)noexcept -> bool
                {

                    LD::ElementReference<BasicKeyedDatabase> instance =  LD::Get<4>(cntx1);
                    const auto & className = LD::Get(LD::Get<3>(cntx1));
                    auto backend  = LD::Get<2>(cntx1);
                    LD::StaticArray<char,1024> classNameBuffer;
                    LD::BackInserter<LD::StaticArray<char,1024>> classNameBackInserter{classNameBuffer};
                    using IT = LD::Detail::Decay_T<decltype(classNameBackInserter)>;
                    LD::QueryResult<IT()> result = LD::MakeContext(LD::TransactionResult{},IT{classNameBackInserter});
                    LD::UInteger classNameSize = LD::Get(LD::Get<1>(cntx1));
                    //encode the UTF-8 compliant class name into the buffer
                    for(LD::UInteger n = 0;n<classNameSize && LD::IsTransactionalQuery(result);++n)
                    {
                        result =  LD::UTF8::Append(className[n],classNameBackInserter);
                    }
                    //result =  LD::UTF8::Append('\0',classNameBackInserter);
                    if (LD::IsTransactionalQuery(result))
                    {
                        auto storeResult =  backend->Store(
                                LD::StringView{instance->mKeyBuffer.GetData(),instance->mKeyBuffer.GetSize()},
                                LD::StringView{classNameBuffer.GetData(),classNameBuffer.GetSize()});
                        //if the store was successful then indicate it with a true otherwise false
                        return LD::MultiMatch(LD::Overload
                        {
                            [](auto){ return false;},
                            [](const LD::Context<LD::TransactionResult,bool> & storeContext) noexcept
                            {
                                return true;
                            }
                        },storeResult);
                    }
                    return false;
                }
            },classNameLength);
            //if the previous actions went off without a hitch, then we can continue
            if (shouldContinue)
            {

                //iterate through all of the members with a constexpr compile time for-loop (as we know all the members at compile time)
                LD::QueryResult<bool(Args...)> resultantQuery = LD::MakeContext(LD::TransactionResult{},true,LD::Forward<Args>(args)...);
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
                    constexpr auto memberName = LD::CT::GetMemberDescriptorName(memberInfo);
                    LD::BackInserter<LD::StaticArray<char,1024>> nosqlKey{keyedDatabase->mKeyBuffer};
                    auto memberNameLength = LD::UTF8::Distance(LD::UTF8::Begin(memberName),LD::UTF8::End(memberName),nosqlKey,memberName);
                    auto onMemberLength = [](const LD::Context<LD::TransactionResult,LD::UInteger,LD::BackInserter<LD::StaticArray<char,1024>>&,decltype(memberName)&> & context) noexcept ->LD::UInteger
                    {
                        auto length = LD::Get(LD::Get<1>(context));
                        auto & backInserter = LD::Get(LD::Get<2>(context));
                        using IT = LD::Detail::Decay_T<decltype(backInserter)>;
                        const auto & memberName = LD::Get(LD::Get<3>(context));
                        //keep track of the number of bytes for the member name
                        auto numberOfBytes = LD::UTF8::NumberOfBytes(memberName);
                        LD::QueryResult<IT()> result = LD::MakeContext(LD::TransactionResult{},IT{backInserter});
                        result = LD::UTF8::Append('.',backInserter);
                        for(auto i = 0;i<length && LD::IsTransactionalQuery(result);++i)
                        {
                            result = LD::UTF8::Append(memberName[i],backInserter);
                        }
                        //add one for the period (which accounts for one byte)
                        return LD::IsTransactionalQuery(result)*numberOfBytes + 1;
                    };
                    auto onMemberLengthError = [](const LD::Context<LD::TransactionError,LD::BackInserter<LD::StaticArray<char,1024>>&,decltype(memberName)&> & context) noexcept -> LD::UInteger
                    {
                        return 0;
                    };
                    LD::UInteger numberOfBytes = LD::Match(memberNameLength,LD::Overload{onMemberLength,onMemberLengthError});
                    //If the member name is UTF-8 compliant then we can go through with the serialization

                    if (numberOfBytes > 0)
                    {
                        if constexpr (LD::CT::IsPrimitive(LD::CT::RemoveQualifiers(type)))

                        {
                            auto immutableRepresentation = LD::ToImmutableString(memberAccessor());

                            keyedDatabase->mBackend->Store(
                                    LD::StringView{keyedDatabase->mKeyBuffer.GetData(),keyedDatabase->mKeyBuffer.GetSize()},
                                    LD::StringView{immutableRepresentation.Data(),immutableRepresentation.GetSize()});

                        }else if constexpr (LD::CT::IsReflectable(type))
                        {
                            keyedDatabase->__Insert(memberAccessor(),LD::Forward<Args>(arguments)...);
                        }



                    }
                    //abc
                    //abc.Base
                    //abc.Height
                    //pop back the current classname
                    for(LD::UInteger n = 0;n<numberOfBytes;++n)
                    {
                        keyedDatabase->mKeyBuffer.PopBack();
                    }

                    //determine if the loop should continue
                    return true;

                },LD::Forward<decltype(members)>(members),LD::Forward<V>(object),this,LD::Forward<Args>(args)...);
                return resultantQuery;
            }
            return LD::MakeContext(LD::TransactionError{},LD::Forward<Args>(args)...);
        }
        //typename Ret = LD::ContextualVariant<VarType(Boundtype,Args...)>

        template<typename Y,typename Key, typename ... Args,
                typename Ret = LD::QueryResult<LD::CT::RebindList<Y,LD::Variant>(Args...)>>
        LD::Enable_If_T<
                LD::Require<
                        LD::IsUTF8Compliant<Key>,
                        LD::IsTypeList<Y>::value
                >,Ret> Fetch(Key && key, const Y & typelist ,Args && ... args) noexcept
        {


            LD::StaticArray<char,1024> keyedBuffer;
            //incorporate the key
            LD::QueryResult<bool()> keyIncorporationiQuery = BasicKeyedDatabase<Backend>::IncorporateKey(LD::Forward<Key>(key),keyedBuffer);

            auto onSuccessfulIncorporation = [](const LD::Context<LD::TransactionResult,bool> & ) noexcept
            {
                return true;
            };
            auto onIncorporationFailure = [](const LD::Context<LD::TransactionError> & ) noexcept
            {
                return false;
            };
            bool isIncorporated = LD::Match(keyIncorporationiQuery,onSuccessfulIncorporation,onIncorporationFailure);
            constexpr auto TypeListSize = Y::size();


            //Ret defaultReturn = LD::MakeContext(LD::TransactionError{},LD::Forward<Args>(args)...);
            if(isIncorporated)
            {
                Ret queryResult = LD::MakeContext(LD::TransactionError{},LD::Forward<Args>(args)...);
                LD::For<TypeListSize>([](
                        auto I,
                        Ret & queryResult,
                        LD::Ref<BasicKeyedDatabase> instance,
                        LD::StaticArray<char,1024> & keyedBuffer,
                        Ret & ret,
                        Args && ... arguments)
                {
                    using V = LD::CT::TypeAtIndex<I,Y>;
                    constexpr auto traits = LD::CT::Reflect<LD::Detail::Decay_T<V>>();
                    constexpr auto members = traits.Members;
                    constexpr auto NumberOfMembers = LD::CT::GetNumberOfMembers(traits);
                    constexpr auto className = LD::CT::GetClassName(traits);
                    auto classNameQueryFunctor = [](const auto & context)->bool
                    {
                        auto fetchedClassName = LD::Get(LD::Get<1>(context));
                        auto targetClassName = LD::Get(LD::Get<2>(context));


                        auto fetchClassNameLength = LD::UTF8::Distance(
                                LD::UTF8::Begin(fetchedClassName),
                                LD::UTF8::End(fetchedClassName),
                                fetchedClassName,
                                targetClassName);

                        auto classNameLength = LD::UTF8::Distance(
                                LD::UTF8::Begin(targetClassName),
                                LD::UTF8::End(targetClassName),
                                fetchedClassName,
                                targetClassName);



                        auto onTransaction = [](
                                const LD::Context<LD::TransactionResult,LD::UInteger,decltype(fetchedClassName)&,decltype(targetClassName)&> & cntxt1,
                                const LD::Context<LD::TransactionResult,LD::UInteger,decltype(fetchedClassName)&,decltype(targetClassName)&> & cntxt2) noexcept
                        {
                            LD::StaticArray<char,1024> encodedClassName;
                            LD::BackInserter<LD::StaticArray<char,1024>> encodedBackInserter{encodedClassName};
                            auto length = LD::Get(LD::Get<1>(cntxt1));

                            const auto & targetClassName = LD::Get(LD::Get<3>(cntxt1));
                            for(LD::UInteger n = 0;n<length;++n)
                            {
                                LD::UTF8::Append(targetClassName[n],encodedBackInserter);
                            }
                            if (length == LD::Get(LD::Get<1>(cntxt2)))
                            {
                                const auto & fetchedClassName = LD::Get(LD::Get<2>(cntxt1));

                                auto fetchClassNameBeginning = LD::UTF8::Begin(fetchedClassName);
                                auto targetClassNameBeginning = LD::UTF8::Begin(encodedClassName);
                                auto fetchedClassNameEnd = LD::UTF8::End(fetchedClassName);
                                auto targetClassNameEnd = LD::UTF8::End(encodedClassName);
                                LD::QueryResult<uint32_t()> nextFetchedClassNameCodePoint = LD::MakeContext(LD::TransactionResult{},uint32_t{});
                                LD::QueryResult<uint32_t()> nextTargetClassNameCodePoint = LD::MakeContext(LD::TransactionResult{},uint32_t{});
                                bool doesTargetClassNameMatchFetchClassName = true;
                                for(LD::UInteger n = 0;n<length && doesTargetClassNameMatchFetchClassName && LD::IsTransactionalQuery(nextFetchedClassNameCodePoint) && LD::IsTransactionalQuery(nextTargetClassNameCodePoint);++n)
                                {

                                    nextFetchedClassNameCodePoint = LD::UTF8::Next(fetchClassNameBeginning,fetchedClassNameEnd);
                                    nextTargetClassNameCodePoint = LD::UTF8::Next(targetClassNameBeginning,targetClassNameEnd);

                                    auto onError = [](auto,auto) noexcept
                                    {
                                        return false;
                                    };


                                    auto onTransaction = [](const LD::Context<LD::TransactionResult,uint32_t> & context1,
                                                            const LD::Context<LD::TransactionResult,uint32_t> & context2) noexcept
                                    {
                                        //printf("%c :: %c \n",LD::Get(LD::Get<1>(context1)),LD::Get(LD::Get<1>(context2)));
                                        return true;
                                    };
                                    doesTargetClassNameMatchFetchClassName = LD::MultiMatch(LD::Overload{onError,onTransaction},nextFetchedClassNameCodePoint,nextTargetClassNameCodePoint);
                                }
                                return doesTargetClassNameMatchFetchClassName;
                            }
                            return false;
                        };

                        auto onError = [](
                                auto,
                                auto) noexcept
                        {
                            return false;
                        };

                        return LD::MultiMatch(LD::Overload{onTransaction,onError},fetchClassNameLength,classNameLength);
                    };
                    auto classNameQuery = instance->mBackend->Fetch(LD::StringView{keyedBuffer.GetData(),keyedBuffer.GetSize()},classNameQueryFunctor,className);

                    auto onClassNameMatch = [](const LD::Context<LD::TransactionResult,bool,LD::StringView,decltype(className)&> & context ) noexcept
                    {
                        return LD::Get(LD::Get<1>(context));
                    };
                    auto classNameMisMatch = [](const LD::Context<LD::TransactionError,LD::StringView,decltype(className)&> & ) noexcept
                    {
                        return false;
                    };

                    bool shouldeSerialize = LD::Match(classNameQuery,onClassNameMatch,classNameMisMatch);
                    if (shouldeSerialize)
                    {

                        //printf("should de-serialize \n");
                        V object;
                        instance->__Fetch(Y{},object,keyedBuffer,LD::Forward<Args>(arguments)...);
                        ret = LD::MakeContext(LD::TransactionResult{},V{object},LD::Forward<Args>(arguments)...);
                        //ret = instance->__Fetch(object,keyedBuffer,LD::Forward<Args>(arguments)...);
                        return false;
                    }
                    return true;
                },queryResult,LD::Ref<BasicKeyedDatabase>{this},keyedBuffer,queryResult,LD::Forward<Args>(args)...);



                //return this->__Fetch(typelist,keyedBuffer,LD::Forward<Args>(args)...);
                return queryResult;
            }
            return LD::MakeContext(LD::TransactionError{},LD::Forward<Args>(args)...);
        }
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
        template<typename Y, typename T,typename ... Args,
                typename Ret = LD::QueryResult<LD::CT::RebindList<Y,LD::Variant>(Args...)>>
        LD::Enable_If_T<
                LD::Require<
                        true
                        //LD::IsTypeString<Key>
                >,Ret> __Fetch( Y, T & object, LD::StaticArray<char,1024> & keyedBuffer,Args && ... arguments) noexcept
        {
            constexpr auto traits = LD::CT::Reflect<LD::Detail::Decay_T<T>>();
            constexpr auto members = traits.Members;
            constexpr auto NumberOfMembers = LD::CT::GetNumberOfMembers(traits);
            //Ret returnable = LD::MakeContext(LD::TransactionError{},LD::Forward<Args>(arguments)...);
            LD::For<NumberOfMembers>([](
                    auto I,
                    auto members,
                    T & object,
                    LD::StaticArray<char,1024> & keyedBuffer,
                    LD::Ref<BasicKeyedDatabase> instance,
                    Args && ... args) noexcept
            {

                constexpr auto memberInfo = LD::Get<I>(members);
                constexpr auto memberName = LD::CT::GetMemberDescriptorName(memberInfo);
                constexpr auto type = LD::CT::GetDescriptorType(memberInfo);
                T * castedObject = (T*)&object;
                //LD::CT::DebugTemplate<decltype(*castedObject)>{};
                auto memberAccessor = memberInfo(*castedObject);



                LD::BackInserter<LD::StaticArray<char,1024>> nosqlKey{keyedBuffer};
                auto memberNameLength = LD::UTF8::Distance(LD::UTF8::Begin(memberName),LD::UTF8::End(memberName),nosqlKey,memberName);

                auto onMemberLength = [](const LD::Context<LD::TransactionResult,LD::UInteger,LD::BackInserter<LD::StaticArray<char,1024>>&,decltype(memberName)&> & context) noexcept ->LD::UInteger
                {
                    auto length = LD::Get(LD::Get<1>(context));
                    auto & backInserter = LD::Get(LD::Get<2>(context));
                    using IT = LD::Detail::Decay_T<decltype(backInserter)>;
                    const auto & memberName = LD::Get(LD::Get<3>(context));
                    //keep track of the number of bytes that compose the classname
                    auto numberOfBytes = LD::UTF8::NumberOfBytes(memberName);
                    LD::QueryResult<IT()> result = LD::MakeContext(LD::TransactionResult{},IT{backInserter});
                    result = LD::UTF8::Append('.',backInserter);
                    for(auto i = 0;i<length && LD::IsTransactionalQuery(result);++i)
                    {
                        result = LD::UTF8::Append(memberName[i],backInserter);
                    }
                    //incorporate the idea of appending the period (which accounts for one byte)
                    return LD::IsTransactionalQuery(result)*numberOfBytes + 1;
                };
                auto onMemberLengthError = [](const LD::Context<LD::TransactionError,LD::BackInserter<LD::StaticArray<char,1024>>&,decltype(memberName)&> & context) noexcept -> LD::UInteger
                {
                    return 0;
                };

                LD::UInteger numberOfBytes = LD::Match(memberNameLength,LD::Overload{onMemberLength,onMemberLengthError});

                if (numberOfBytes > 0)
                {



                    if constexpr (LD::CT::IsPrimitive(LD::CT::RemoveQualifiers(type)))
                    {
                        auto primitiveFunctor = [](const auto & context) noexcept -> bool
                        {
                            auto fetchedKey = LD::Get(LD::Get<0>(context));
                            auto fetchedValue = LD::Get(LD::Get<1>(context));
                            auto memberAccessor = LD::Get(LD::Get<2>(context));
                            using AccessedType = LD::Detail::Decay_T<decltype(memberAccessor())>;
                            LD::StringToPrimitive<AccessedType > converter{};
                            AccessedType ret = LD::Match(converter(fetchedValue),[](auto){ return AccessedType {0};},[](const AccessedType & obj){return obj;});
                            memberAccessor = ret;
                            return true;
                        };
                        auto classNameQuery = instance->mBackend->Fetch(LD::StringView{keyedBuffer.GetData(),keyedBuffer.GetSize()},primitiveFunctor,memberAccessor);

                    }else if constexpr (LD::CT::IsReflectable(type))
                    {
                        using SubType = LD::CT::TypeList<LD::Detail::Decay_T<decltype(memberAccessor())>>;
                        auto & member = memberAccessor();
                        //todo - replace evil cast
                        LD::Detail::Decay_T<decltype(member)> * castedMember = (LD::Detail::Decay_T<decltype(member)>*)&member;
                        instance->__Fetch(Y {},*castedMember,keyedBuffer,LD::Forward<Args>(args)...);
                    }
                }
                for(LD::UInteger n = 0;n<numberOfBytes;++n)
                {
                    keyedBuffer.PopBack();
                }

                return true;
            },members,object,keyedBuffer,LD::Ref<BasicKeyedDatabase>{this},LD::Forward<Args>(arguments)...);
            return LD::MakeContext(LD::TransactionError{},LD::Forward<Args>(arguments)...);
        }

        template<typename Key, typename ... TL ,typename ... Args>
        LD::Enable_If_T<
                (LD::Require<LD::IsReflectable<TL>> && ...)
        ,
        LD::QueryResult<LD::Variant<LD::Type<TL>...>(Args...)>> Remove(const Key & key, LD::CT::TypeList<TL...> ,Args && ... arguments) noexcept
        {

            /*
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
             */
            return {};
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