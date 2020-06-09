//
// Created by phoenixflower on 6/5/20.
//

#ifndef LANDESSDEVCORE_DATABASEENTITY_H
#define LANDESSDEVCORE_DATABASEENTITY_H
#include "TypeTraits/Detection.hpp"
#include "Primitives/General/typestring.hpp"
#include "Primitives/General/Immutable.hpp"
#include "Reflection/reflectable.hpp"
#include "Memory/ElementReference.h"
#include "FetchRequest.h"
namespace LD
{

    template<typename TypeString, typename T, typename Backend, class = void>
    class Entity;
    template<typename Key, typename T, typename Backend>
    class Entity<Key,T,Backend,LD::Enable_If_T<
            LD::Require<
            LD::IsTypeString<Key>,
            LD::IsReflectable<T>,
            LD::IsDefaultConstructible<T>
            >>>
    {
    private:
        LD::Ref<Backend> mBackend;
        LD::QueryResult<LD::Variant<T>()> mFetchQuery;
        LD::QueryResult<T &()> mInsertionQuery;
        Key mKey;
    public:
        constexpr Entity() noexcept:mKey(Key{}),mBackend(LD::Ref<Backend>{nullptr})
        {

        }

        constexpr Entity(const Key & key, const LD::Ref<Backend> & backend) noexcept :mKey(key),mBackend(backend)
        {
            //todo - perform the actual query at this point
        }



        explicit operator LD::QueryResult<T &()>() const noexcept
        {
            return {};
        }

        template<typename ... Args>
        LD::QueryResult<T&()> operator()(Args && ... arguments) const noexcept
        {
            return {};
        }


        ~Entity()
        {

            //todo serialize to the data store
        }

    };

    template<typename Key, typename T, typename Backend>
    class Entity<Key,T,Backend,LD::Enable_If_T<
            LD::Require<
            LD::IsImmutableString<Key>,
            LD::IsReflectable<T>,
            LD::IsDefaultConstructible<T>
            >>>
    {
    private:
        LD::Ref<Backend> mBackend;
        T mObject;
        Key mKey;
    public:

    };
    template<typename Key, typename T, typename Backend>
    class Entity<Key,T,Backend,LD::Enable_If_T<
            LD::Require<
                    LD::Negate<LD::IsImmutableString<Key>>,
                    LD::Negate<LD::IsTypeString<Key>>,
                    LD::IsReflectable<T>,
                    LD::IsDefaultConstructible<T>
            >>>
    {
    private:
        LD::Ref<Backend> mBackend;
        T mObject;
    public:

    };
}
#endif //LANDESSDEVCORE_DATABASEENTITY_H
