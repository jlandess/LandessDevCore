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
        LD::Variant<LD::ImmutableString<25>,LD::Tuple<LD::UInteger,char*>> mClassNameBuffer;

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
        >,bool> RetrieveForObject(const Key & key, T * object,unqlite * backend) noexcept
        {
            using Type = LD::Decay_T<T>;
            using ClassName = decltype(Type::GetClassNameTypeString());
            using Period = LD::TypeString<'.'>;
            //todo move this out of this function call to a parent function call
            constexpr LD::UInteger numberOfMembers = Type::NumberOfMembers;

            LD::For<1,numberOfMembers>([](auto I, T * object , const Key & key,unqlite * backend)
            {
                using MemberType = LD::Decay_T<decltype(LD::Get<I>(*object))>;
                using MemberName = decltype(LD::Declval<T>()[LD::CompileTimeIndex<I>{}]);
                //using MemberKey =  LD::tycat<Key ,Period ,ClassName,Period ,MemberName>;
                using MemberKey = LD::tycat<Key,Period,MemberName>;
                if constexpr (LD::Exists<CanBeImmutableString,MemberType>)
                {
                    LD::Variant<LD::NullClass,LD::ElementReference<T>> variant;
                    auto fetchCallback = [](const void * input, unsigned int dataSize, void * inputPointer)->int
                    {
                        StringToPrimitive<MemberType> converter;
                        T * object = (T*)inputPointer;
                        const char * data = (const char*)input;
                        auto resultVariant = converter(LD::StringView{data});
                        MemberType result = LD::Match(resultVariant,[](const MemberType & obj){ return obj;},[](auto &&){ return MemberType{};});
                        (*object)[MemberName{}] = result;
                        return 0;
                    };

                    unqlite_kv_fetch_callback(backend,MemberKey::data(),MemberKey::size(),fetchCallback, object);
                    return true;
                }else if constexpr(LD::IsReflectable<MemberType>)
                {
                    return RetrieveForObject(key,object[MemberName{}]);
                }
                return true;
            },object,key,backend);

            return true;
        }
    public:
        inline Database() noexcept :mBackend(nullptr){}

        inline Database(const LD::StringView & databaseName) noexcept
        {
            this->mCode = unqlite_open(&this->mBackend,databaseName.data(),UNQLITE_OPEN_CREATE);
            constexpr const char * keyName = "master.database.classnamesize";
            auto zero = LD::ToImmutableString(0);


            unqlite_kv_store(this->mBackend,keyName,sizeof(keyName),zero.Data(),zero.GetSize());
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
            using Type = LD::Decay_T<T>;
            using ClassName = decltype(Type::GetClassNameTypeString());

            LD::UInteger maxClassSize = 0;
            auto fetchClassNameSizePredicate = [](const void * data, unsigned int dataSize, void * inputPointer)->int
            {
                LD::UInteger * possibleClassSize = (LD::UInteger*)inputPointer;

                StringToPrimitive<LD::UInteger> converter;
                auto resultVariant = converter(LD::StringView{(const char*)data});
                LD::UInteger result = LD::Match(resultVariant,[](const LD::UInteger & obj){ return obj;},[](auto &&){ return LD::UInteger {};});
                (*possibleClassSize) = result;
                return 0;
            };
            constexpr const char * fetchClassMaxSize = "master.database.classnamesize";
            unqlite_kv_fetch_callback(this->mBackend,fetchClassMaxSize,sizeof(fetchClassMaxSize),fetchClassNameSizePredicate, &maxClassSize);



            if (maxClassSize > 24)
            {

                //LD::Variant<LD::ImmutableString<25>,LD::Tuple<LD::UInteger,char*>>
                auto immutableStringBranch = [&](const LD::ImmutableString<25> &)
                {
                    LD::Tuple<LD::UInteger,char*> buffer;
                    LD::Get<0>(buffer) = maxClassSize;
                    LD::Get<1>(buffer) = new char[maxClassSize];
                    this->mClassNameBuffer = buffer;
                };
                auto charCharBranch = [&](const LD::Tuple<LD::UInteger,char*> & buffer)
                {
                    delete [] LD::Get<1>(buffer);
                    this->mClassNameBuffer = LD::Tuple<LD::UInteger,char*>{};
                };
                LD::Match(this->mClassNameBuffer,immutableStringBranch,charCharBranch);
            }


            unqlite_begin(this->mBackend);
            {
                unqlite_kv_store(this->mBackend,Key::data(),Key::size(),ClassName::data(),ClassName::size());
                auto injectKV = [](const LD::StringView & key, const LD::StringView & value, unqlite * db) noexcept
                {
                    unqlite_kv_store(db,key.data(),key.size(),value.data(),value.size());
                };
                Database::WalkThroughObject(key,LD::Forward<T>(object),injectKV,this->mBackend);
            }
            unqlite_commit(this->mBackend);
            return (*this);
        }

        template<typename T, typename Key>
        LD::Enable_If_T<
        LD::Require<
        LD::IsReflectable<T>,
        LD::IsTypeString<Key>
        >
        ,
        std::optional<T>> Fetch(const Key & key) const noexcept
        {
            using Type = LD::Decay_T<T>;
            using ClassName = decltype(Type::GetClassNameTypeString());
            LD::Variant<LD::NullClass,std::string> keyedObject;

            auto fetchCallback = [](const void * data, unsigned int dataSize, void * inputPointer)->int
            {
                LD::Variant<LD::NullClass,std::string> * keyedObject = (LD::Variant<LD::NullClass,std::string>*)inputPointer;
                (*keyedObject) = (char*)data;

                return 0;
            };
            unqlite_kv_fetch_callback(this->mBackend,Key::data(),Key::size(),fetchCallback, &keyedObject);
            std::optional<T> ret;
            auto foundItClause = [&](const std::string & className)
            {
                if (className == ClassName::data())
                {
                    T object;
                    Database::RetrieveForObject(key,&object,this->mBackend);
                    ret = object;
                }
            };
            auto didNotFinditClause = [](const LD::NullClass &){};
            LD::Match(keyedObject,foundItClause,didNotFinditClause);
            return ret;
        }

        Database & InsertEntity();

        inline ~Database() noexcept
        {
            unqlite_close(this->mBackend);



            auto removeCharBuffer = [](const LD::Tuple<LD::UInteger,char*> & bufferPair)
            {
                delete [] LD::Get<1>(bufferPair);
            };
            auto removeImmutableString = [](auto &&){};

            LD::Match(this->mClassNameBuffer,removeCharBuffer,removeImmutableString);
        }
    };

    template<typename T, typename Key, typename F, typename ... Args>
    LD::Enable_If_T <LD::Require<
            LD::IsReflectable<T>,LD::IsTypeString<Key>,LD::Negate<LD::IsPointer<T>>
    >,bool> Database::WalkThroughObject(const Key & key, T && object, F && onInstanceVariable,Args && ... passedArguements) noexcept
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
        LD::For<1,numberOfMembers>([](auto I, auto && object, F && onInstanceVariable , const Key & key,Args && ... passedArguements)
        {
            using MemberType = LD::Decay_T<decltype(LD::Get<I>(object))>;
            using MemberName = decltype(LD::Declval<T>()[LD::CompileTimeIndex<I>{}]);
            //using MemberKey =  LD::tycat<Key ,Period ,ClassName,Period ,MemberName>;
            using MemberKey = LD::tycat<Key,Period ,MemberName>;
            if constexpr (LD::Exists<CanBeImmutableString,MemberType>)
            {
                auto memberAsString = LD::ToImmutableString( object[MemberName{}]);
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
