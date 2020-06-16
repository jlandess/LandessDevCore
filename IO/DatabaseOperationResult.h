//
// Created by phoenixflower on 5/21/20.
//

#ifndef LANDESSDEVCORE_DATABASEOPERATIONRESULT_H
#define LANDESSDEVCORE_DATABASEOPERATIONRESULT_H
#include "Primitives/General/mapboxvariant.hpp"
#include "Primitives/General/ContextualVariant.h"
namespace LD
{
    class DatabaseFoundResult
    {

    };

    class DatabaseInsertResult
    {

    };

    class DatabaseIOError
    {

    };

    class DatabaseNotFoundError
    {

    };

    class DatabaseNotInitializedError
    {

    };
    class DatabaseError
    {
    private:
        LD::Variant<DatabaseIOError,DatabaseNotFoundError,DatabaseNotInitializedError> mError;
    public:

        template<typename ... Args>
        constexpr LD::ContextualVariant<LD::Variant<DatabaseIOError,DatabaseNotFoundError,DatabaseNotInitializedError>(Args...)> operator()(Args && ... arguments) const noexcept
        {
            auto onIOError = [](const LD::DatabaseIOError & error) noexcept
            {
                return 0;
            };

            auto onNotFoundError = [](const LD::DatabaseNotFoundError & error) noexcept
            {
                return 1;
            };

            auto onNotInitializedError = [](const LD::DatabaseNotInitializedError & error) noexcept
            {
                return 2;
            };
            LD::ContextualVariant<LD::Variant<DatabaseIOError,DatabaseNotFoundError,DatabaseNotInitializedError>(Args...)> ret[3];
            ret[0] = LD::MakeContext(LD::DatabaseIOError{},LD::Forward<Args>(arguments)...);
            ret[1] = LD::MakeContext(LD::DatabaseNotFoundError{},LD::Forward<Args>(arguments)...);
            ret[2] = LD::MakeContext(LD::DatabaseNotInitializedError{},LD::Forward<Args>(arguments)...);
            return ret[LD::Match(this->mError,onIOError,onNotFoundError,onNotInitializedError)];
        }
    };

    class DatabaseTransactionResult
    {

    };

    using DatabaseResult = LD::Variant<DatabaseFoundResult,DatabaseInsertResult,DatabaseError>;
}
#endif //LANDESSDEVCORE_DATABASEOPERATIONRESULT_H
