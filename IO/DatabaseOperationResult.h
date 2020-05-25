//
// Created by phoenixflower on 5/21/20.
//

#ifndef LANDESSDEVCORE_DATABASEOPERATIONRESULT_H
#define LANDESSDEVCORE_DATABASEOPERATIONRESULT_H
#include "Primitives/General/mapboxvariant.hpp"
namespace LD
{
    class DatabaseFoundResult
    {

    };

    class DatabaseInsertResult
    {

    };

    class DatabaseError
    {

    };

    class DatabaseTransactionResult
    {

    };

    using DatabaseResult = LD::Variant<DatabaseFoundResult,DatabaseInsertResult,DatabaseError>;
}
#endif //LANDESSDEVCORE_DATABASEOPERATIONRESULT_H
