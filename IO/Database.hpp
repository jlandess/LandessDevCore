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
#include "Memory/UniquePointer.h"
#include "Functor/fixed_size_function.hpp"
#include "DatabaseOperationResult.h"
#include "DatabaseOpenMode.h"
#include "Primitives/General/Span.hpp"
namespace LD
{

    template<typename Backend>
    class BasicDatabase
    {
    private:

        using Db = LD::ElementReference<Backend>;
        Db mBackend;
    public:

        BasicDatabase(const Db & backend):mBackend(backend)
        {

        }

        template<typename Key,typename V, typename ... Args,
                typename VarType = LD::Variant<LD::DatabaseError,LD::DatabaseTransactionResult>,
                typename Ret = LD::ContextualVariant<VarType(Args...)>,
                typename CurrentBackend = Backend>
        LD::Enable_If_T<
                LD::Require<
                        LD::IsTypeString<Key>
                >,Ret> InsertAndCommit(const Key & key,V && object, Args && ... args) noexcept
        {
            this->mBackend->Begin();
            auto resultant = this->Insert(key,LD::Forward<V>(object),LD::Forward<Args>(args)...);
            this->mBackend->Commit();
            return resultant;
        }
        template<typename Key,typename V, typename ... Args,
                typename VarType = LD::Variant<LD::DatabaseError,LD::DatabaseTransactionResult>,
                typename Ret = LD::ContextualVariant<VarType(Args...)>>
        LD::Enable_If_T<
                LD::Require<
                        LD::IsTypeString<Key>
                >,Ret> Insert(const Key & key,V && object, Args && ... args) noexcept
        {

            using Type = LD::Detail::Decay_T<V>;
            using Var = LD::CT::RebindList<LD::CT::ReflectiveTransformation<Type ,LD::AccessReadOnly>,LD::Variant>;
            using QueryResultant = LD::Variant<LD::DatabaseError,LD::DatabaseTransactionResult>;
            using QueryResponse = LD::ContextualVariant<QueryResultant()>;
            auto onClass = [](const LD::Context<LD::StringView,LD::StringView,Db> & context)
            {
                LD::StringView  key = LD::Get<0>(context);
                LD::StringView className = LD::Get<1>(context);
                Db handle = LD::Get<2>(context);
                QueryResponse response = handle->Store(
                        LD::StringView{key.data(),key.size()},
                        LD::StringView{className.data(),className.size()});

                auto onDatabaseError = [](const LD::Context<LD::DatabaseError> & error)
                {
                    return false;
                };
                auto onTransaction = [](const LD::Context<LD::DatabaseTransactionResult> & error)
                {

                    return true;
                };
                return LD::Match(response,onDatabaseError,onTransaction);
            };

            auto onMember = [](const LD::ContextualVariant<Var(LD::StringView,Db)> & context)
            {

                auto onPrimitiveAction = [](auto && context)
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

                        auto onDatabaseError = [](const LD::Context<LD::DatabaseError> & error)
                        {
                            return false;
                        };
                        auto onTransaction = [](const LD::Context<LD::DatabaseTransactionResult> & error)
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

        template<typename V,typename Y,typename Key, typename ... Args,
                typename VarType = LD::Variant<LD::DatabaseError,LD::DatabaseTransactionResult>,
                typename Ret = LD::ContextualVariant<VarType(V,Args...)>>
        LD::Enable_If_T<
                LD::Require<
                        LD::IsTypeString<Key>,
                        LD::IsDefaultConstructible<V>,
                        LD::IsTypeList<Y>::value
                >,Ret> Fetch(const Key & key, const Y & typelist ,Args && ... args) noexcept
        {

            using Var = LD::CT::RebindList<LD::CT::ReflectiveTransformation<V ,LD::AccessReadOnly>,LD::Variant>;
            using QueryResultant = LD::Variant<LD::DatabaseError,LD::DatabaseTransactionResult>;
            using QueryResponse = LD::ContextualVariant<QueryResultant()>;

            auto onClassReanimate = [](const LD::Context<LD::StringView,LD::StringView,Db> & context)
            {
                auto onFetch = [](const LD::Context<LD::StringView,LD::StringView,LD::StringView> & context) -> LD::UInteger
                {

                    LD::UInteger comparisonResult = LD::Get<1>(context) == LD::Get<2>(context);
                    return LD::UInteger {comparisonResult};

                };

                Db handle = LD::Get<2>(context);

                LD::ContextualVariant<LD::Variant<LD::DatabaseError,LD::DatabaseTransactionResult>(LD::StringView ,LD::UInteger,LD::StringView)> fetchContext;
                fetchContext = handle->Fetch(LD::StringView{LD::Get<0>(context)},onFetch,LD::StringView{LD::Get<1>(context)});

                auto onDatabaseError = [](const LD::Context<LD::DatabaseError,LD::StringView ,LD::UInteger,LD::StringView> &)
                {

                    return false;
                };

                auto onTransaction = [](const LD::Context<LD::DatabaseTransactionResult,LD::StringView ,LD::UInteger,LD::StringView> & transaction)
                {

                    return LD::Get<2>(transaction);
                };
                return LD::Match(fetchContext,onDatabaseError,onTransaction);
            };

            auto onMemberReanimate = [](const LD::ContextualVariant<Var(LD::StringView,Db)> & context)
            {
                auto onPrimitiveAction = [](auto && context)
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

            //todo iterate through all of the types in the typelist
            V objectToReanimate;
            const bool result = LD::CT::ReflectiveWalk(key,objectToReanimate,onClassReanimate,onMemberReanimate,LD::AccessWriteOnly{},Db{this->mBackend});
            Ret possibleResults[2];
            possibleResults[0] = LD::MakeContext(LD::DatabaseError{},V{objectToReanimate},LD::Forward<Args>(args)...);
            possibleResults[1] = LD::MakeContext(LD::DatabaseTransactionResult{},V{objectToReanimate},LD::Forward<Args>(args)...);
            return possibleResults[result];
        }

    };
}
#endif //LANDESSDEVCORE_DATABASE_HPP