//
// Created by phoenixflower on 5/21/20.
//

#ifndef LANDESSDEVCORE_DATABASEOPERATIONRESULT_H
#define LANDESSDEVCORE_DATABASEOPERATIONRESULT_H
#include "Primitives/General/mapboxvariant.hpp"
#include "Primitives/General/ContextualVariant.h"
#include "Primitives/General/mapboxvariantvisitor.h"
#include "Primitives/General/StaticArray.hpp"
namespace LD
{


    class IOError
    {

    };

    class NotFoundError
    {

    };

    class NotInitializedError
    {

    };

    class RandomError
    {

    };

    class MathError
    {

    };

    class QuantitativeError
    {

    };

    class LogicalError
    {

    };

    class SyntaxError
    {

    };

    class SemanticError
    {

    };

    class LinkerError
    {

    };

    class RunTimeError
    {

    };

    class PagingError
    {

    };

    class MemoryError
    {

    };

    class EncodingError
    {

    };


    using ErrorTypes = LD::CT::TypeList<
                LD::IOError,
                LD::NotFoundError,
                LD::NotInitializedError,
                LD::RandomError,
                LD::MathError,
                LD::QuantitativeError,
                LD::LogicalError,
                LD::SyntaxError,
                LD::SemanticError,
                LD::LinkerError,
                LD::RunTimeError,
                LD::PagingError,
                LD::MemoryError,
                LD::EncodingError
            >;

    template<typename T>
    constexpr bool IsError(LD::Type<T> && ) noexcept
    {
        //if the type exists in the typelist of valid error types, then it's a valid error type
        return (LD::CT::GetTypeCountInTypeList<T,ErrorTypes> > 0);
    }
    template<typename T, typename U>
    constexpr bool IsSpecificError(LD::Type<T>, LD::Type<U>) noexcept
    {

        return false;
    }

    template<typename T>
    constexpr LD::Enable_If_T<LD::Require<LD::IsError(LD::Type<T>{})>,bool> IsSpecificError(LD::Type<T>, LD::Type<T>) noexcept
    {
        return true;
    }
    template<typename T>
    constexpr LD::Enable_If_T<LD::FallBack<LD::IsError(LD::Type<T>{})>,bool> IsSpecificError(LD::Type<T>, LD::Type<T>) noexcept
    {
        return false;
    }

    /*
    class DatabaseError
    {
    private:
        LD::Variant<IOError,NotFoundError,NotInitializedError> mError;
    public:
        DatabaseError() noexcept
        {

        }
        template<typename T, typename = LD::Enable_If_T<
                LD::Require<
                        LD::Either<LD::IsSame<T,IOError>,LD::IsSame<T,NotFoundError>,LD::IsSame<T,NotInitializedError>>
                        >>>
        DatabaseError(const T & error) noexcept:mError{error}
        {


        }

        template<typename ... Args>
        constexpr LD::ContextualVariant<LD::Variant<IOError,NotFoundError,NotInitializedError>(Args...)> operator()(Args && ... arguments) const noexcept
        {
            auto onIOError = [](const LD::IOError & error) noexcept
            {
                return 0;
            };

            auto onNotFoundError = [](const LD::NotFoundError & error) noexcept
            {
                return 1;
            };

            auto onNotInitializedError = [](const LD::NotInitializedError & error) noexcept
            {
                return 2;
            };
            LD::ContextualVariant<LD::Variant<IOError,NotFoundError,NotInitializedError>(Args...)> ret[3];
            ret[0] = LD::MakeContext(LD::IOError{},LD::Forward<Args>(arguments)...);
            ret[1] = LD::MakeContext(LD::NotFoundError{},LD::Forward<Args>(arguments)...);
            ret[2] = LD::MakeContext(LD::NotInitializedError{},LD::Forward<Args>(arguments)...);
            return ret[LD::Match(this->mError,onIOError,onNotFoundError,onNotInitializedError)];
        }
    };
     */

    class DatabaseTransactionResult
    {

    };

    class TransactionResult
    {

    };

    class TransactionError
    {
    public:
        using ErrorType = LD::Rebind<LD::ErrorTypes,LD::Variant>;
    private:
        ErrorType mError;
    public:
        TransactionError() = default;

        template<typename T, typename =  LD::Enable_If_T<LD::Require<(LD::CT::GetTypeCountInTypeList<T,LD::ErrorTypes> > 0)>>>
        TransactionError(T && error) noexcept:mError{LD::Forward<T>(error)}
        {
        }

        template<typename ... Args>
        constexpr LD::ContextualVariant<ErrorType (Args...)> operator()(Args && ... arguments) const noexcept
        {
            LD::StaticArray<LD::ContextualVariant<ErrorType (Args...)>,ErrorTypes::size()> returnable;
            auto getErrorIndex = [](auto && error) noexcept
            {
                auto currentError = LD::CT::RemoveQualifiers(LD::Type<decltype(error)>{});
                using ErrorType = decltype(currentError);
                using CoreType = typename ErrorType::type;
                return LD::tlist_index_of<CoreType,ErrorTypes>::value;
            };
            LD::For<ErrorTypes::size()>([](
                    auto I,
                    LD::StaticArray<LD::ContextualVariant<ErrorType (Args...)>,ErrorTypes::size()> & buffer,
                    Args && ... arguments)
            {
                using CurrentErrorType = decltype(LD::Get<I>(ErrorTypes{}));
                buffer[I] = LD::MakeContext(CurrentErrorType {},LD::Forward<Args>(arguments)...);
                return true;
            },returnable,LD::Forward<Args>(arguments)...);
            return returnable[LD::Match(this->mError,getErrorIndex)];
        }


    };


    //using DatabaseResult = LD::Variant<DatabaseFoundResult,DatabaseInsertResult,DatabaseError>;
}
#endif //LANDESSDEVCORE_DATABASEOPERATIONRESULT_H
