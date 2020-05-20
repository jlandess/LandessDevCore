//
// Created by phoenixflower on 5/14/20.
//

#ifndef LANDESSDEVCORE_DATABASE_HPP
#define LANDESSDEVCORE_DATABASE_HPP
#include "TypeTraits/EnableIf.hpp"
#include "Definitions/Integer.hpp"
#include "Primitives/General/StringView.hpp"
#include "Primitives/General/Immutable.hpp"
#include "unqlite.h"
#include "Reflection/reflectable.hpp"
#include "Primitives/General/mapboxvariant.hpp"
#include "Primitives/General/mapboxvariantvisitor.h"
#include "Algorithms/StringToPrimitive.hpp"
#include "Primitives/General/StaticArray.hpp"
#include "Algorithms/StringAsNumber.h"
#include "Async/Thread.h"
#include "Primitives/General/Context.h"
#include "TypeTraits/TypeList.hpp"
#include "Primitives/General/ContextualVariant.h"
namespace LD
{
    template<typename T>
    using CanBeImmutableString = decltype(LD::ToImmutableString(LD::Declval<T>()));


    template<typename T>
    class DatabaseArrayElem
    {
    private:
        LD::UInteger mIndex;
        LD::StringView mName;
    public:

        DatabaseArrayElem & operator = (const T &) noexcept;

    };
    template<typename T>
    class DatabaseArray
    {
    private:
        unqlite * mBackend;
        LD::UInteger mSize;
    public:
        inline DatabaseArray() noexcept {}
        inline DatabaseArray(unqlite * database) noexcept{}

        DatabaseArrayElem<T> operator[](const LD::UInteger & index) noexcept;
    };

    class DatabaseFoundResult
    {

    };

    class DatabaseInsertResult
    {

    };

    class DatabaseError
    {

    };

    template<typename ... Args>
    using DatabaseSupportedVariant = LD::Variant<LD::ElementReference<Args>...>;


    using DefaultDatabaseSupportedVariant = DatabaseSupportedVariant<float,double,long double,
                                                                        bool,
                                                                        char,unsigned char,
                                                                        short,int,long,long long,long long int,
                                                                        unsigned short, unsigned int, unsigned long, unsigned long long, unsigned long long int
                                                                        >;
    template<typename ... Args>
    using DatabaseReAnimationContext =
            LD::Variant<LD::Context<LD::ElementReference<float>,Args...>,
                        LD::Context<LD::ElementReference<double>,Args...>,
                        LD::Context<LD::ElementReference<long double>,Args...>,
                        LD::Context<LD::ElementReference<bool>,Args...>,
                        LD::Context<LD::ElementReference<char>,Args...>,
                        LD::Context<LD::ElementReference<unsigned char>,Args...>,
                        LD::Context<LD::ElementReference<short>,Args...>,
                        LD::Context<LD::ElementReference<int>,Args...>,
                        LD::Context<LD::ElementReference<long>,Args...>,
                        LD::Context<LD::ElementReference<long long>,Args...>,
                        LD::Context<LD::ElementReference<long long int>,Args...>,
                        LD::Context<LD::ElementReference<unsigned short>,Args...>,
                        LD::Context<LD::ElementReference<unsigned int>,Args...>,
                        LD::Context<LD::ElementReference<unsigned long>,Args...>,
                        LD::Context<LD::ElementReference<unsigned long long>,Args...>,
                        LD::Context<LD::ElementReference<unsigned long long int>,Args...>>;



    //using DatabaseReAnimationContext = LD::Context<DefaultDatabaseSupportedVariant,Args...>;

    template<typename T>
    class DebugTemplate;

    class Database
    {
    private:


        unqlite * mBackend;
        LD::UInteger mStatus;
        template<typename T, typename Key, typename F, typename H,typename ... Args>
        static LD::Enable_If_T <
        LD::Require<
        LD::IsReflectable<T>,
        LD::IsTypeString<Key>,
        LD::Negate<LD::IsPointer<T>>,
        LD::ConvertiblyCallable<H,void(const LD::StringView& key, const LD::StringView & className, Args && ...)>::Value(),
        LD::ConvertiblyCallable<F,void(const LD::StringView& memberKey, const LD::StringView & memberValue, Args && ...)>::Value()
    >,bool> WalkThroughObject(const Key & key, T && object, H && onFoundClass,F && onInstanceVariable, Args && ... passedArguements) noexcept ;

        template<typename T, typename Key, typename F, typename ... Args>
        static LD::Enable_If_T <
        LD::Require<
        LD::IsReflectable<T>,
        LD::IsTypeString<Key>,
        LD::Negate<LD::IsPointer<T>>
        >,bool> RetrieveForObject(const Key & key, T * object,F && onMember ,unqlite * backend,LD::UInteger & status,Args && ... arguements) noexcept
        {
            if (!(status == UNQLITE_OK || status == UNQLITE_NOTFOUND))
            {
                return status;
            }
            using Type = LD::Decay_T<T>;
            using ClassName = decltype(Type::GetClassNameTypeString());
            using Period = LD::TypeString<'.'>;
            constexpr LD::UInteger numberOfMembers = Type::NumberOfMembers;
            LD::For<1,numberOfMembers>([](auto I, T * object , const Key & key,unqlite * backend, LD::UInteger & status, F && onMember,Args && ... arguements)
            {
                using MemberType = LD::Decay_T<decltype(LD::Get<I>(*object))>;
                using MemberName = decltype(LD::Declval<T>()[LD::CompileTimeIndex<I>{}]);
                using MemberKey = LD::tycat<Key,Period,MemberName>;
                if constexpr (LD::Exists<CanBeImmutableString,MemberType>)
                {
                    LD::Variant<LD::NullClass,LD::ElementReference<T>> variant;
                    auto fetchCallback = [](const void * input, unsigned int dataSize, void * inputPointer)->int
                    {
                        MemberType * member = (MemberType*)inputPointer;
                        const char * data = (const char*)input;
                        auto resultVariant = LD::StringAsNumber<MemberType>(LD::StringView{data});
                        MemberType result = LD::Match(resultVariant,[](const MemberType & obj){ return obj;},[](auto &&){ return MemberType{};});
                        (*member) = result;
                        return 0;
                    };
                    //todo move function call to the parent function
                    //OnMember(const LD::StringView & memberKey,const LD::Variant<LD::ElementReference<T>...> & context, Args && ... context) noexcept;
                    LD::ElementReference<LD::Decay_T<MemberType>> referenceableMember(&(*object)[MemberName{}]);
                    //LD::Variant<LD::Context<LD::DatabaseError,Args...>,LD::Context<DefaultDatabaseSupportedVariant ,Args...>> mooo;
                    //LD::Context<DefaultDatabaseSupportedVariant ,Args...> context = LD::MakeContext(DefaultDatabaseSupportedVariant{LD::ElementReference<LD::Decay_T<MemberType>>{}},LD::Forward<Args>(arguements)...);

                    LD::ContextualVariant<DefaultDatabaseSupportedVariant(LD::StringView ,Args...)> managedContext = LD::ContextualVariant<DefaultDatabaseSupportedVariant(LD::StringView,Args...)>{LD::MakeContext(LD::ElementReference<MemberType>{referenceableMember},LD::StringView {MemberKey::data(),MemberKey::size()},LD::Forward<Args>(arguements)...)};
                    //LD::MakeContext(int{},LD::StringView{},LD::StringView{});
                    //managedContext = LD::MakeContext(LD::ElementReference<MemberType>{},LD::Forward<Args>(arguements)...);
                    onMember(managedContext);



                    //status = unqlite_kv_fetch_callback(backend,MemberKey::data(),MemberKey::size(),fetchCallback, referenceableMember.GetPointer());
                    return (status == UNQLITE_NOTFOUND || status == UNQLITE_OK);
                }else if constexpr(LD::IsReflectable<MemberType>)
                {
                    return RetrieveForObject(key,object[MemberName{}],status);
                }
                return (status == UNQLITE_NOTFOUND || status == UNQLITE_OK);
            },object,key,backend,status,LD::Forward<F>(onMember),LD::Forward<Args>(arguements)...);
            return true;
        }
    public:
        inline Database() noexcept :mBackend(nullptr){}

        inline Database(const LD::StringView & databaseName) noexcept
        {
            this->mStatus = unqlite_open(&this->mBackend,databaseName.data(),UNQLITE_OPEN_CREATE);
        }

        template<typename T,typename Key>
        LD::Enable_If_T<LD::Require<LD::IsTypeString<Key>>,DatabaseArray<T>> Array(const Key & key) noexcept
        {
            using Period = LD::TypeString<'.'>;
            using SizeAsTypeString = decltype("Size"_ts);
            using MemberKey =  LD::tycat<Key ,Period ,SizeAsTypeString>;
            auto zero = LD::ToImmutableString(0);
            unqlite_kv_store(this->mBackend,Key::data(),Key::size(),"array",sizeof("array"));
            unqlite_kv_store(this->mBackend,MemberKey::data,MemberKey::size,zero.Data(),zero.Size());
            return {};
        }
        /**
         *@brief Inserts any object into the backend database
         * @return
         */
        template<typename T, typename Key>
        LD::Enable_If_T <LD::Require<
                LD::IsReflectable<T>,LD::IsTypeString<Key>,LD::Negate<LD::IsPointer<T>>
        >,LD::Variant<LD::DatabaseError,DatabaseInsertResult>> Insert(const Key & key, T && object) noexcept
        {
            using Type = LD::Decay_T<T>;
            using ClassName = decltype(Type::GetClassNameTypeString());
            ////UNQLITE_OK
            LD::UInteger statusCode = {0};
            statusCode = unqlite_begin(this->mBackend);
            if (statusCode == UNQLITE_OK)
            {
                //statusCode = unqlite_kv_store(this->mBackend,Key::data(),Key::size(),ClassName::data(),ClassName::size());
                auto injectKV = [](const LD::StringView & key, const LD::StringView & value, unqlite * db) noexcept
                {
                    unqlite_kv_store(db,key.data(),key.size(),value.data(),value.size());
                };
                auto addClassKV = [](const LD::StringView  & key,const LD::StringView & className, unqlite * db) noexcept
                {
                    unqlite_kv_store(db,key.data(),key.size(),className.data(),className.size());
                };
                Database::WalkThroughObject(key,LD::Forward<T>(object),addClassKV,injectKV,this->mBackend);


                statusCode = unqlite_commit(this->mBackend);
                if (statusCode != UNQLITE_OK)
                {
                    return {};
                }


                return {LD::DatabaseInsertResult{}};

            }
            if (statusCode != UNQLITE_OK)
            {
                unqlite_rollback(this->mBackend);
            }
            return {};
        }



        template<typename T, typename Key, typename ... Context>
        LD::Enable_If_T<
                LD::Require<
                        LD::IsReflectable<T>,
                        LD::IsTypeString<Key>
                >
                ,
                LD::Variant<LD::Context<LD::DatabaseError,Context...>,LD::Context<T,Context...>>> Fetch(const Key & key,Context && ... context) const noexcept
        {
            using Type = LD::Decay_T<T>;
            using ClassName = decltype(Type::GetClassNameTypeString());

            //------------------------------------beginning of fetching class name assoiated with key ----------------------------------------
            LD::Variant<LD::NullClass,LD::ImmutableString<1024>> keyedObject;
            auto fetchCallback = [](const void * data, unsigned int dataSize, void * inputPointer)->int
            {
                //LD::Variant<LD::NullClass,std::string> * keyedObject = (LD::Variant<LD::NullClass,std::string>*)inputPointer;
                LD::Variant<LD::NullClass,LD::ImmutableString<1024>> * keyedObject = (LD::Variant<LD::NullClass,LD::ImmutableString<1024>>*)inputPointer;
                (*keyedObject) = LD::ImmutableString<1024>{(const char*)data,dataSize};
                return 0;
            };
            LD::UInteger status = unqlite_kv_fetch_callback(this->mBackend,Key::data(),Key::size(),fetchCallback, &keyedObject);
            if (status != UNQLITE_OK)
            {
                return {};
            }
            //-----------------------------------end of fetching class name associated with key ----------------------------------------------
            auto foundItClause = [&](const LD::ImmutableString<1024> & className)->LD::Variant<LD::Context<LD::DatabaseError,Context...>,LD::Context<T,Context...>>
            {
                LD::ImmutableString<1024> targetClassName{ClassName{}};
                if (className == targetClassName)
                {
                    T object;
                    LD::UInteger status = UNQLITE_OK;

                    auto onMember = [](const LD::ContextualVariant<DefaultDatabaseSupportedVariant(LD::StringView,LD::ElementReference<unqlite>)> & context)
                    {
                        auto PeformMemberQuery = [](const auto & object)
                        {
                            using MemberType = LD::Decay_T<decltype(LD::Get(LD::Get<0>(object)))>;
                            auto fetchCallback = [](const void * input, unsigned int dataSize, void * inputPointer)->int
                            {
                                MemberType * member = (MemberType*)inputPointer;
                                const char * data = (const char*)input;
                                auto resultVariant = LD::StringAsNumber<MemberType>(LD::StringView{data});
                                MemberType result = LD::Match(resultVariant,[](const MemberType & obj){ return obj;},[](auto &&){ return MemberType{};});
                                (*member) = result;
                                return 0;
                            };
                            LD::ElementReference<MemberType> instanceVariable = LD::Get<LD::ElementReference<MemberType>>(object);
                            LD::ElementReference<unqlite> backend = LD::Get<LD::ElementReference<unqlite>>(object);
                            LD::StringView memberKey = LD::Get<LD::StringView>(object);
                            LD::UInteger result = unqlite_kv_fetch_callback(backend.GetPointer(),memberKey.data(),memberKey.size(),fetchCallback, instanceVariable.GetPointer());
                            if((result == UNQLITE_NOTFOUND || result == UNQLITE_OK))
                            {
                                //query happened without error
                            }

                            //return database error
                        };
                        LD::Match(context,PeformMemberQuery);
                        //using MemberType = LD::Decay_T<decltype(LD::Get(LD::Get<0>(context)))>;
                        auto deserialize = [](auto && passedInObject)
                        {
                            /**
                            using MemberType = LD::Decay_T<decltype(LD::Get(passedInObject))>;
                            auto fetchCallback = [](const void * input, unsigned int dataSize, void * inputPointer)->int
                            {
                                MemberType * member = (MemberType*)inputPointer;
                                const char * data = (const char*)input;
                                auto resultVariant = LD::StringAsNumber<MemberType>(LD::StringView{data});
                                MemberType result = LD::Match(resultVariant,[](const MemberType & obj){ return obj;},[](auto &&){ return MemberType{};});
                                (*member) = result;
                                return 0;
                            };
                             */
                            //unqlite_kv_fetch_callback(db,MemberKey::data(),MemberKey::size(),fetchCallback, LD::Get(passedInObject));
                        auto OnCompleteQuery = []()
                        {

                            };
                        };


                        auto OnQueryError = []()
                        {

                        };

                        //LD::Match(memberReference,deserialize);
                        /*
                        auto fetchCallback = [](const void * input, unsigned int dataSize, void * inputPointer)->int
                        {
                            MemberType * member = (MemberType*)inputPointer;
                            const char * data = (const char*)input;
                            auto resultVariant = LD::StringAsNumber<MemberType>(LD::StringView{data});
                            MemberType result = LD::Match(resultVariant,[](const MemberType & obj){ return obj;},[](auto &&){ return MemberType{};});
                            (*member) = result;
                            return 0;
                        };
                         */
                    };
                    bool wasSuccessful = Database::RetrieveForObject(key,&object,onMember,this->mBackend,status,LD::ElementReference<unqlite>{this->mBackend});
                    if (wasSuccessful)
                    {
                        return LD::MakeContext(T{object},LD::Forward<Context>(context)...);
                    }
                    return {};
                }
                return {};
            };
            auto didNotFinditClause = [&](const LD::NullClass &)->LD::Variant<LD::Context<LD::DatabaseError,Context...>,LD::Context<T,Context...>>
            {

                return LD::MakeContext(LD::DatabaseError{},LD::Forward<Context>(context)...);
            };
            return LD::Match(keyedObject,foundItClause,didNotFinditClause);
        }

        Database & InsertEntity();

        inline ~Database() noexcept
        {
            unqlite_close(this->mBackend);
        }
    };

    template<typename T, typename Key, typename F, typename H,typename ... Args>
    LD::Enable_If_T <
    LD::Require<
    LD::IsReflectable<T>,
    LD::IsTypeString<Key>,
    LD::Negate<LD::IsPointer<T>>,
    LD::ConvertiblyCallable<H,void(const LD::StringView& key, const LD::StringView & className, Args && ...)>::Value(),
    LD::ConvertiblyCallable<F,void(const LD::StringView& memberKey, const LD::StringView & memberValue, Args && ...)>::Value()
    >,bool> Database::WalkThroughObject(const Key & key, T && object,H && onFoundClass ,F && onInstanceVariable,Args && ... passedArguements) noexcept
    {
        using Type = LD::Decay_T<T>;
        using ClassName = decltype(Type::GetClassNameTypeString());
        using Period = LD::TypeString<'.'>;
        //todo add error handling
        onFoundClass(LD::StringView{Key::data(),Key::size()},LD::StringView{ClassName::data(),ClassName::size()},LD::Forward<Args>(passedArguements)...);
        constexpr LD::UInteger numberOfMembers = Type::NumberOfMembers;
        LD::For<1,numberOfMembers>([](auto I, auto && object, F && onInstanceVariable , const Key & key,Args && ... passedArguements)
        {
            using MemberType = LD::Decay_T<decltype(LD::Get<I>(object))>;
            using MemberName = decltype(LD::Declval<T>()[LD::CompileTimeIndex<I>{}]);
            using MemberKey = LD::tycat<Key,Period ,MemberName>;
            if constexpr (LD::Exists<CanBeImmutableString,MemberType>)
            {
                auto memberAsString = LD::ToImmutableString( object[MemberName{}]);
                //todo add error handling
                onInstanceVariable(LD::StringView{MemberKey::data(),MemberKey::size()},
                        LD::StringView{memberAsString.Data(),memberAsString.GetSize()},
                        LD::Forward<Args>(passedArguements)...);
            }else if constexpr(LD::IsReflectable<MemberType>)
            {
                return WalkThroughObject(key,object[MemberName{}],LD::Forward<F>(onInstanceVariable),LD::Forward<Args>(passedArguements)...);
            }
            return true;
        },LD::Forward<T>(object),LD::Forward<F>(onInstanceVariable),key,LD::Forward<Args>(passedArguements)...);
        return true;
    }
}
#endif //LANDESSDEVCORE_DATABASE_HPP
