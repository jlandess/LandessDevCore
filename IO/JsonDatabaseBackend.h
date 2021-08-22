//
// Created by phoenixflower on 6/9/20.
//

#ifndef LANDESSDEVCORE_JSONDATABASEBACKEND_H
#define LANDESSDEVCORE_JSONDATABASEBACKEND_H
#include "json.hpp"
#include "Memory/ElementReference.h"
#include "Primitives/General/StringView.hpp"
#include "FetchRequest.h"
#include "DatabaseOperationResult.h"
#include "Primitives/General/Context.h"
#include "TypeTraits/Detection.hpp"
#include "Functor/fixed_size_function.hpp"
#include "Core/RequestResponse.hpp"
namespace LD
{
    class JsonBackend
    {
    private:
        LD::Ref<nlohmann::json> mBackend;
    public:
        inline  JsonBackend(const LD::ElementReference<nlohmann::json> & backend) noexcept :mBackend(backend)
        {

        }
        template<typename ... Args,typename Ret = LD::QueryResult<bool(Args...)>>
        Ret Store(const LD::StringView & key, const LD::StringView & data, Args && ... arguements) const noexcept
        {
            nlohmann::json * ptr = this->mBackend.GetPointer();
            (*ptr)[key.data()] = data.data();
            return LD::MakeContext(LD::TransactionResult{},bool{true},LD::Forward<Args>(arguements)...);
        }

        template<typename ... Args>
        LD::RequestResponse<bool(Args...)> Insert(const LD::StringView & key, const LD::StringView & data, Args && ... arguements) const noexcept
        {
            nlohmann::json * ptr = this->mBackend.GetPointer();
            (*ptr)[key.data()] = data.data();
            return LD::CreateResponse(LD::Type<bool>{},bool{true},LD::Forward<Args>(arguements)...);
        }

        template<typename F, typename ... Args,
                typename FunctorRet = decltype(LD::Declval<F>()(LD::Declval<LD::Context<LD::StringView,LD::StringView,Args...>>())),
                typename Ret = LD::QueryResult<FunctorRet(LD::StringView,Args...)>>
        LD::Enable_If_T<
                LD::Require<
                        LD::Negate<LD::Detail::IsSame<void,FunctorRet>::value>
                >
                ,
                Ret> Fetch(const LD::StringView & key, F && functor, Args && ... arguments) const noexcept
        {
            nlohmann::json * ptr = this->mBackend.GetPointer();
            auto it = (*ptr).find(key.data());
            if (it != (*ptr).end())
            {
                LD::StringView resultantValue;
                (*it).get_to(resultantValue);
                Ret returnable = functor(LD::MakeContext(LD::StringView{key},LD::StringView{resultantValue}),LD::Forward<Args>(arguments)...);
                return LD::MakeContext(LD::DatabaseTransactionResult{},Ret{returnable},LD::Forward<Args>(arguments)...);

            }
            return LD::MakeContext(LD::TransactionError{},LD::Forward<Args>(arguments)...);
        }


        template<typename ... Args, typename Ret = LD::ContextualVariant<LD::Variant<LD::TransactionError,LD::DatabaseTransactionResult>(Args...)>>
        Ret Begin(Args && ... arguements) const noexcept
        {
            return LD::MakeContext(LD::DatabaseTransactionResult{},LD::Forward<Args>(arguements)...);
        }
        template<typename ... Args, typename Ret = LD::ContextualVariant<LD::Variant<LD::TransactionError,LD::DatabaseTransactionResult>(Args...)>>
        Ret  Commit(Args && ... arguements) const noexcept
        {
            return LD::MakeContext(LD::DatabaseTransactionResult{},LD::Forward<Args>(arguements)...);
        }


    };
}
#endif //LANDESSDEVCORE_JSONDATABASEBACKEND_H
