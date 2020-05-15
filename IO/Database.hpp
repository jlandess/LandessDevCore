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
    class Database
    {
    private:
        unqlite * mBackend;
        LD::UInteger mCode;
        template<typename T, typename Key, typename F, typename ... Args>
        static LD::Enable_If_T <
                LD::Require<
                        LD::IsReflectable<T>,
                        LD::IsTypeString<Key>,
                        LD::Negate<LD::IsPointer<T>
                        >
    >,bool> WalkThroughObject(const Key & key, T && object, F && callback, Args && ... passedArguements) noexcept ;




        template<typename T, typename Key>
        static LD::Enable_If_T <LD::Require<
                LD::IsReflectable<T>,LD::IsTypeString<Key>,LD::Negate<LD::IsPointer<T>>
        >,bool> RetrieveForObject(const Key & key, T & object,unqlite * backend) noexcept
        {
            //get the Type of the passed in object without any references
            using Type = LD::Decay_T<T>;
            //store the classname in a typestring for future usage
            using ClassName = decltype(Type::GetClassNameTypeString());
            //use a compile time cached version of period for concatenations
            using Period = LD::TypeString<'.'>;
            //todo move this out of this function call to a parent function call
            //unqlite_kv_store(database,Key::data(),Key::size(),ClassName::data(),ClassName::size());
            //we need to know the amount of members to iterate through
            constexpr LD::UInteger numberOfMembers = Type::NumberOfMembers;
            LD::For<1,numberOfMembers>([](auto I, auto && object , const Key & key,unqlite * backend)
            {
                using MemberType = LD::Decay_T<decltype(LD::Get<I>(object))>;
                using MemberName = decltype(LD::Declval<T>()[LD::CompileTimeIndex<I>{}]);
                using MemberKey =  LD::tycat<Key ,Period ,ClassName,Period ,MemberName>;
                if constexpr (LD::Exists<CanBeImmutableString,MemberType>)
                {
                    LD::Variant<LD::NullClass,LD::ElementReference<T>> variant;

                    auto fetchCallback = [](const void * input, unsigned int dataSize, void * inputPointer)->int
                    {
                        const char * data = (const char*)input;


                        //convert string to type
                        return 0;
                    };
                    unqlite_kv_fetch_callback(backend,MemberKey::data(),MemberKey::size(),fetchCallback, &object[MemberName{}]);
                    //callback(LD::StringView{MemberKey::data(),MemberKey::size()},
                    //LD::StringView{memberAsString.Data(),memberAsString.Size()},
                    //LD::Forward<Args>(passedArguements)...);
                    //object[MemberName{}];
                }else if constexpr(LD::IsReflectable<MemberType>)
                {
                    return RetrieveForObject(key,object[MemberName{}]);
                }
                return true;
            },LD::Forward<T>(object),backend);
            return true;
        }
    public:
        inline Database() noexcept :mBackend(nullptr){}

        inline Database(const LD::StringView & databaseName) noexcept
        {

            this->mCode = unqlite_open(&this->mBackend,databaseName.data(),UNQLITE_OPEN_CREATE);
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
        >,Database&> Insert(const Key & key, T && object) noexcept
        {
            //get the Type of the passed in object without any references
            using Type = LD::Decay_T<T>;

            //store the classname in a typestring for future usage
            using ClassName = decltype(Type::GetClassNameTypeString());
            unqlite_kv_store(this->mBackend,Key::data(),Key::size(),ClassName::data(),ClassName::size());
            unqlite_begin(this->mBackend);
            auto injectKV = [](const LD::StringView & key, const LD::StringView & value, unqlite * db) noexcept
            {
                unqlite_kv_store(db,key.data(),key.size(),value.data(),value.size());
            };
            Database::WalkThroughObject(key,LD::Forward<T>(object),injectKV,this->mBackend);
            unqlite_commit(this->mBackend);
            return (*this);
        }

        template<typename T, typename Key>
        LD::EnableIf<
        LD::Require<
        LD::IsReflectable<T>,
        LD::IsTypeString<Key>
        >
        ,
        LD::Variant<LD::NullClass,T>> Fetch(const Key & key)
        {
            LD::Variant<LD::NullClass,std::string> keyedObject;
            auto fetchCallback = [](const void * data, unsigned int dataSize, void * inputPointer)->int
            {
                LD::Variant<LD::NullClass,std::string> * keyedObject = (LD::Variant<LD::NullClass,std::string>*)inputPointer;
                printf("found it %s \n",(char*)data);
                (*keyedObject) = (char*)data;
                return 0;
            };
            unqlite_kv_fetch_callback(this->mBackend,Key::data(),Key::size(),fetchCallback, &keyedObject);

            auto onFound = [&](const std::string & className)
            {
                    if(className == Key::data())
                    {
                        //we found the type we were looking for

                        T object;
                        auto fillFromKVStore = [](const LD::StringView & key, const LD::StringView & value, unqlite * db, T & object) noexcept
                        {
                            //unqlite_kv_store(db,key.data(),key.size(),value.data(),value.size());
                        };
                        Database::WalkThroughObject(key,LD::Forward<T>(object),fillFromKVStore,this->mBackend,object);

                    }
            };
            auto onNotFound = [](const LD::NullClass &){};


            LD::Match(keyedObject,onFound,onNotFound);
            return {};
        }

        Database & InsertEntity();

        inline ~Database() noexcept
        {
            unqlite_close(this->mBackend);
        }
    };

    template<typename T, typename Key, typename F, typename ... Args>
    LD::Enable_If_T <LD::Require<
            LD::IsReflectable<T>,LD::IsTypeString<Key>,LD::Negate<LD::IsPointer<T>>
    >,bool> Database::WalkThroughObject(const Key & key, T && object, F && callback,Args && ... passedArguements) noexcept
    {
        //get the Type of the passed in object without any references
        using Type = LD::Decay_T<T>;
        //store the classname in a typestring for future usage
        using ClassName = decltype(Type::GetClassNameTypeString());
        //use a compile time cached version of period for concatenations
        using Period = LD::TypeString<'.'>;
        //todo move this out of this function call to a parent function call
        //unqlite_kv_store(database,Key::data(),Key::size(),ClassName::data(),ClassName::size());
        //we need to know the amount of members to iterate through
        constexpr LD::UInteger numberOfMembers = Type::NumberOfMembers;
        LD::For<1,numberOfMembers>([](auto I, auto && object, F && callback , const Key & key,Args && ... passedArguements)
        {
            using MemberType = LD::Decay_T<decltype(LD::Get<I>(object))>;
            using MemberName = decltype(LD::Declval<T>()[LD::CompileTimeIndex<I>{}]);
            using MemberKey =  LD::tycat<Key ,Period ,ClassName,Period ,MemberName>;
            if constexpr (LD::Exists<CanBeImmutableString,MemberType>)
            {
                auto memberAsString = LD::ToImmutableString( object[MemberName{}]);
                callback(LD::StringView{MemberKey::data(),MemberKey::size()},
                        LD::StringView{memberAsString.Data(),memberAsString.Size()},
                        LD::Forward<Args>(passedArguements)...);
            }else if constexpr(LD::IsReflectable<MemberType>)
            {
                return WalkThroughObject(key,object[MemberName{}],LD::Forward<F>(callback),LD::Forward<Args>(passedArguements)...);
            }
            return true;
        },LD::Forward<T>(object),LD::Forward<F>(callback),key,LD::Forward<Args>(passedArguements)...);
        return true;
    }
}
#endif //LANDESSDEVCORE_DATABASE_HPP
