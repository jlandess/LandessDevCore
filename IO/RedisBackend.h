//
// Created by phoenixflower on 9/19/20.
//

#ifndef LANDESSDEVCORE_REDISBACKEND_H
#define LANDESSDEVCORE_REDISBACKEND_H
#include "IO/Redis/hiredis.h"
#include "Primitives/General/StringView.hpp"
//#include "Definitions/Common.hpp"
#include "Unicode/UTF8.hpp"
#include "Primitives/General/Immutable.hpp"

#include <memory>
namespace LD
{
    class RedisReplyStatus
    {
    private:
        LD::StringView mView;
        mutable std::unique_ptr<redisReply ,void(*)(void*)> mContext;
    public:
        RedisReplyStatus(LD::StringView value,std::unique_ptr<redisReply ,void(*)(void*)>  reply) noexcept :mContext{LD::Move(reply)},mView{value}
        {


        }
        RedisReplyStatus(const RedisReplyStatus & status) noexcept:mContext{LD::Move(status.mContext)},mView{status.mView}
        {

        }
        const LD::StringView & View() const noexcept
        {
            return this->mView;
        }
    };

    class RedisReplyError
    {
    private:
        LD::StringView mView;
        mutable std::unique_ptr<redisReply ,void(*)(void*)> mContext;
    public:
        RedisReplyError(LD::StringView value,std::unique_ptr<redisReply ,void(*)(void*)>  reply) noexcept :mContext{LD::Move(reply)},mView{value}
        {


        }
        RedisReplyError(const RedisReplyError & status) noexcept:mContext{LD::Move(status.mContext)},mView{status.mView}
        {

        }
        const LD::StringView & View() const noexcept
        {
            return this->mView;
        }
    };
    class RedisNill
    {

    };

    class RedisReplyString
    {
    private:
        mutable std::unique_ptr<redisReply ,void(*)(void*)> mContext;
        LD::StringView mView;
    public:
        RedisReplyString(LD::StringView value,std::unique_ptr<redisReply ,void(*)(void*)>  reply) noexcept :mContext{LD::Move(reply)},mView{value}
        {


        }
        RedisReplyString(const RedisReplyString & status) noexcept:mContext{LD::Move(status.mContext)},mView{status.mView}
        {

        }
        LD::StringView View() noexcept
        {
            return this->mView;
        }
    };



    class RedisBackingStore
    {
    private:
        std::unique_ptr<redisContext,void(*)(redisContext*)> mContext;
    public:
        RedisBackingStore( LD::StringView  url, LD::UInteger  port) noexcept:mContext{redisConnect(url.data(),port),redisFree}
        {

        }

        static char GetKeySeperator() noexcept
        {
            return '.';
        }
        template<typename ... Args,typename Ret = LD::QueryResult<bool(Args...)>>
        Ret Store(const LD::StringView & key, const LD::StringView & data, Args && ... arguements)  noexcept
        {
            using TransactionalContext = LD::Context<LD::TransactionResult,LD::UInteger,LD::StringView &,LD::StringView &,LD::BackInserter<LD::StaticArray<char,1024>>&>;
            redisContext * ptr = this->mContext.get();
            LD::StaticArray<char,1024> buffer;
            LD::BackInserter<LD::StaticArray<char,1024>> inserter{buffer};
            auto keyLength = LD::UTF8::Distance(LD::UTF8::Begin(key),LD::UTF8::End(key),key,data,inserter);
            auto dataLength = LD::UTF8::Distance(LD::UTF8::Begin(data),LD::UTF8::End(data),key,data,inserter);
            inserter = 's';
            inserter = 'e';
            inserter = 't';
            inserter = ' ';
            using IT = LD::Detail::Decay_T<decltype(inserter)>;
            auto res = LD::MultiMatch(
                    LD::Overload{
                            [](auto,auto) noexcept-> LD::QueryResult<bool()>
                            {
                                return LD::MakeContext(LD::TransactionResult{},true);
                            },
                            [](const TransactionalContext & keyTransaction, const TransactionalContext & dataTransaction) noexcept-> LD::QueryResult<bool()>
                            {
                                //get the length of key
                                const auto & keyLength = LD::Get(LD::Get<1>(keyTransaction));
                                //get the length of the data
                                const auto & dataLength = LD::Get(LD::Get<1>(dataTransaction));
                                const auto  key = LD::Get(LD::Get<2>(keyTransaction));
                                const auto  data = LD::Get(LD::Get<3>(keyTransaction));
                                LD::BackInserter<LD::StaticArray<char,1024>> & inserter =  LD::Get(LD::Get<4>(keyTransaction));
                                LD::QueryResult<IT()> result = LD::MakeContext(LD::TransactionResult{},IT{inserter});

                                //insert the key into the buffer
                                for(auto i = 0;i<keyLength && LD::IsTransactionalQuery(result);++i)
                                {
                                    result = LD::UTF8::Append(key[i],inserter);
                                }
                                result = LD::UTF8::Append(' ',inserter);
                                for(auto i = 0;i<dataLength && LD::IsTransactionalQuery(result);++i)
                                {
                                    result = LD::UTF8::Append(data[i],inserter);
                                }

                                if (LD::IsTransactionalQuery(result))
                                {
                                    return LD::MakeContext(LD::TransactionResult{},true);
                                }
                                return LD::MakeContext(LD::TransactionError{});
                            }
                    },keyLength,dataLength);

            for(LD::UInteger n = 0;n<buffer.GetSize();++n)
            {
                //printf("%c",buffer[n]);
            }
            //printf("\n");

            inserter = '\0';
            if (LD::IsTransactionalQuery(res))
            {
                this->Execute(LD::StringView {buffer.GetData(),buffer.GetSize()});
                return LD::MakeContext(LD::TransactionResult{},true,LD::Forward<Args>(arguements)...);
            }
            return  LD::MakeContext(LD::TransactionError{},LD::Forward<Args>(arguements)...);
        }

        template<typename F, typename ... Args,
                typename FunctorRet = decltype(LD::Declval<F>()(LD::Declval<LD::Context<LD::StringView,LD::StringView,Args...>>())),
                typename Ret = LD::QueryResult<FunctorRet(LD::StringView,Args...)>>
        LD::Enable_If_T<
                LD::Require<
                        LD::Negate<LD::Detail::IsSame<void,FunctorRet>::value>
                >
                ,
                Ret> Fetch( LD::StringView  key, F && functor, Args && ... arguments)  noexcept
        {
            using TransactionalContext = LD::Context<LD::TransactionResult,LD::UInteger,LD::StringView &,LD::BackInserter<LD::StaticArray<char,1024>>&>;
            LD::StaticArray<char,1024> buffer;
            LD::BackInserter<LD::StaticArray<char,1024>> inserter{buffer};
            inserter = 'g';
            inserter = 'e';
            inserter = 't';
            inserter = ' ';
            auto keyLength = LD::UTF8::Distance(LD::UTF8::Begin(key),LD::UTF8::End(key),key,inserter);
            using IT = LD::Detail::Decay_T<decltype(inserter)>;
            auto res = LD::Match(keyLength,LD::Overload{
                                         [](auto) noexcept -> LD::QueryResult<bool()>
                                         {
                                             return LD::MakeContext(LD::TransactionError{});
                                         }},
                                 [](const TransactionalContext & keyedTransaction) noexcept -> LD::QueryResult<bool()>
                                 {
                                     const auto keyLength = LD::Get(LD::Get<1>(keyedTransaction));
                                     const auto key = LD::Get(LD::Get<2>(keyedTransaction));
                                     LD::BackInserter<LD::StaticArray<char,1024>> & inserter =  LD::Get(LD::Get<3>(keyedTransaction));
                                     LD::QueryResult<IT()> result = LD::MakeContext(LD::TransactionResult{},IT{inserter});
                                     for(auto i = 0;i<keyLength && LD::IsTransactionalQuery(result);++i)
                                     {
                                         result = LD::UTF8::Append(key[i],inserter);
                                     }
                                     //the redis API requires null terminated strings
                                     inserter = '\0';
                                     return LD::MakeContext(LD::TransactionResult{},true);
                                 });

            for(LD::UInteger n = 0;n<buffer.GetSize();++n)
            {
                //printf("%c",buffer[n]);
            }
            //printf("\n");
            //we have a valid query
            if (LD::IsTransactionalQuery(res))
            {
                LD::ImmutableString<64> primitiveBuffer = {'\0'};

                auto executionResult = this->Execute(LD::StringView{buffer.GetData(),buffer.GetSize()},primitiveBuffer);

                auto onExecutionBool = [](const LD::Context<LD::TransactionResult,bool,LD::ImmutableString<64>&> & context) noexcept -> LD::QueryResult<LD::StringView()>
                {
                    //printf("1\n");
                    auto & immutableString = LD::Get(LD::Get<2>(context));
                    immutableString = LD::ToImmutableString(LD::UInteger(LD::Get(LD::Get<1>(context))));
                    return LD::MakeContext(LD::TransactionResult{},LD::StringView{immutableString.Data(),immutableString.GetSize()});

                };
                auto onExecutionInteger = [](const LD::Context<LD::TransactionResult,long long,LD::ImmutableString<64>&> & context) noexcept  -> LD::QueryResult<LD::StringView ()>
                {
                    //printf("2\n");
                    auto & immutableString = LD::Get(LD::Get<2>(context));
                    immutableString = LD::ToImmutableString(LD::Get(LD::Get<1>(context)));
                    return LD::MakeContext(LD::TransactionResult{},LD::StringView{immutableString.Data(),immutableString.GetSize()});

                };
                auto onExecutionDouble = [](const LD::Context<LD::TransactionResult,double,LD::ImmutableString<64>&> & context) noexcept  -> LD::QueryResult<LD::StringView ()>
                {
                    //printf("3\n");
                    auto & immutableString = LD::Get(LD::Get<2>(context));
                    immutableString = LD::ToImmutableString(LD::Get(LD::Get<1>(context)));
                    return LD::MakeContext(LD::TransactionResult{},LD::StringView{immutableString.Data(),immutableString.GetSize()});
                };
                auto onExecutionNill = [](const LD::Context<LD::TransactionResult,RedisNill,LD::ImmutableString<64>&> & context) noexcept  -> LD::QueryResult<LD::StringView ()>
                {
                    //printf("4\n");
                    return LD::MakeContext(LD::TransactionError{});
                };
                auto onExecutionReplyString = [](const LD::Context<LD::TransactionResult,RedisReplyString,LD::ImmutableString<64>&> & context) noexcept  -> LD::QueryResult<LD::StringView ()>
                {
                    //printf("5\n");
                    //LD::Get(LD::Get<1>(context)).View();
                    return LD::MakeContext(LD::TransactionResult{},LD::StringView{LD::Get(LD::Get<1>(context)).View()});
                };
                auto onExecutionReplyStatus = [](const LD::Context<LD::TransactionResult,RedisReplyStatus,LD::ImmutableString<64>&> & context) noexcept  -> LD::QueryResult<LD::StringView ()>
                {
                    //printf("6\n");
                    return LD::MakeContext(LD::TransactionError{});
                };
                auto onExecutionReplyError = [](const LD::Context<LD::TransactionResult,RedisReplyError,LD::ImmutableString<64>&> & context) noexcept  -> LD::QueryResult<LD::StringView ()>
                {
                    //printf("7\n");
                    return LD::MakeContext(LD::TransactionError{});
                };
                auto onExecutionError = [](const LD::Context<LD::TransactionError,LD::ImmutableString<64>&> & context) noexcept  -> LD::QueryResult<LD::StringView ()>
                {
                    //printf("8\n");
                    return LD::MakeContext(LD::TransactionError{});
                };

                auto valueQuery = LD::Match(
                        executionResult,
                        onExecutionBool,
                        onExecutionDouble,
                        onExecutionError,
                        onExecutionInteger,
                        onExecutionNill,
                        onExecutionReplyError,
                        onExecutionReplyString,
                        onExecutionReplyStatus);


                if (LD::IsTransactionalQuery(valueQuery))
                {
                    //printf("fetch being called\n");
                    LD::StringView value = LD::MultiMatch(
                            LD::Overload{
                                    [](auto) -> LD::StringView { return LD::StringView {};},
                                    [](const LD::Context<LD::TransactionResult,LD::StringView> & context ) -> LD::StringView { return LD::Get(LD::Get<1>(context));}
                            }
                            ,valueQuery);
                    auto resultFromFunctor = functor(LD::MakeContext(LD::StringView{key},LD::StringView{value},LD::Forward<Args>(arguments)...));
                    return LD::MakeContext(LD::TransactionResult{},FunctorRet{resultFromFunctor},LD::StringView {key},LD::Forward<Args>(arguments)...);

                }
                return LD::MakeContext(LD::TransactionError{},LD::StringView{key},LD::Forward<Args>(arguments)...);
            }

            return LD::MakeContext(LD::TransactionError{},LD::StringView{key},LD::Forward<Args>(arguments)...);
        }

        template<typename ... Args>
        LD::QueryResult<LD::Variant<long long,double,bool,RedisNill,RedisReplyString,RedisReplyStatus,RedisReplyError>(Args...)> Execute(LD::StringView command, Args && ... args) noexcept
        {
            std::unique_ptr<redisReply ,void(*)(void*)> reply{(redisReply *)redisCommand(this->mContext.get(), command.data()),freeReplyObject};

            if (this->mContext->err != 0)
            {

                switch (this->mContext->err)
                {
                    case REDIS_ERR_IO:
                    {
                        return LD::MakeContext(LD::TransactionError{LD::IOError{}},LD::Forward<Args>(args)...);
                    }
                    case REDIS_ERR_EOF:
                    {
                        return LD::MakeContext(LD::TransactionError{LD::IOError{}},LD::Forward<Args>(args)...);
                    }
                    case REDIS_ERR_PROTOCOL:
                    {
                        return LD::MakeContext(LD::TransactionError{LD::SemanticError{}},LD::Forward<Args>(args)...);
                    }
                    case REDIS_ERR_OOM:
                    {
                        return LD::MakeContext(LD::TransactionError{LD::MemoryError{}},LD::Forward<Args>(args)...);
                    }
                    case REDIS_ERR_TIMEOUT:
                    {
                        return LD::MakeContext(LD::TransactionError{LD::TimeoutError{}},LD::Forward<Args>(args)...);
                    }
                    case REDIS_ERR_OTHER:
                    {
                        return LD::MakeContext(LD::TransactionError{LD::RandomError{}},LD::Forward<Args>(args)...);
                    }
                }
            }
            switch (reply->type)
            {
                case REDIS_REPLY_STATUS:
                {
                    ;
                    return LD::MakeContext(LD::TransactionResult{},RedisReplyStatus {LD::StringView{reply->str,reply->len},LD::Move(reply)},LD::Forward<Args>(args)...);
                }
                case REDIS_REPLY_ERROR:
                {
                    return LD::MakeContext(LD::TransactionResult{},RedisReplyError{LD::StringView{reply->str,reply->len},LD::Move(reply)},LD::Forward<Args>(args)...);
                }
                case REDIS_REPLY_INTEGER:
                {
                    using Type = long long;
                    return LD::MakeContext(LD::TransactionResult{},Type{reply->integer},LD::Forward<Args>(args)...);
                }
                case REDIS_REPLY_DOUBLE:
                {
                    return LD::MakeContext(LD::TransactionResult{},double{reply->dval},LD::Forward<Args>(args)...);
                }
                case REDIS_REPLY_BOOL:
                {
                    return LD::MakeContext(LD::TransactionResult{},bool{bool(reply->integer)},LD::Forward<Args>(args)...);
                }
                case REDIS_REPLY_NIL:
                {
                    return LD::MakeContext(LD::TransactionResult{},RedisNill{},LD::Forward<Args>(args)...);
                }
                case REDIS_REPLY_STRING:
                {
                    return LD::MakeContext(LD::TransactionResult{},RedisReplyString{LD::StringView{reply->str,reply->len},LD::Move(reply)},LD::Forward<Args>(args)...);
                }
                case REDIS_REPLY_ARRAY:
                {
                    return  LD::MakeContext(LD::TransactionError{LD::SemanticError{}},LD::Forward<Args>(args)...);
                }
                case REDIS_REPLY_MAP:
                {
                    return  LD::MakeContext(LD::TransactionError{LD::SemanticError{}},LD::Forward<Args>(args)...);
                }

                case REDIS_REPLY_SET:
                {
                    return  LD::MakeContext(LD::TransactionError{LD::SemanticError{}},LD::Forward<Args>(args)...);
                }
                case REDIS_REPLY_PUSH:
                {
                    return  LD::MakeContext(LD::TransactionError{LD::SemanticError{}},LD::Forward<Args>(args)...);
                }
                case REDIS_REPLY_ATTR:
                {
                    return  LD::MakeContext(LD::TransactionError{LD::SemanticError{}},LD::Forward<Args>(args)...);
                }
                case REDIS_REPLY_BIGNUM:
                {
                    return  LD::MakeContext(LD::TransactionError{LD::SemanticError{}},LD::Forward<Args>(args)...);
                }
                case REDIS_REPLY_VERB:
                {
                    return  LD::MakeContext(LD::TransactionError{LD::SemanticError{}},LD::Forward<Args>(args)...);
                }

                default:
                {
                    return  LD::MakeContext(LD::TransactionError{LD::RandomError{}},LD::Forward<Args>(args)...);
                }
            }
        }


    };
}
#endif //LANDESSDEVCORE_REDISBACKEND_H
