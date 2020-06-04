//
// Created by phoenixflower on 6/4/20.
//

#ifndef LANDESSDEVCORE_INSERTIONREQUEST_H
#define LANDESSDEVCORE_INSERTIONREQUEST_H
#include "Primitives/General/Context.h"
#include "Primitives/General/mapboxvariant.hpp"
#include "DatabaseOperationResult.h"

namespace LD
{
    template<typename T>
    class InsertionRequest;

    template<typename ... Pack, typename ... Args>
    class InsertionRequest<LD::CT::TypeList<Pack...>(Args...)> : public
            LD::Variant<
            LD::Context<LD::DatabaseTransactionResult,LD::Context<Pack...>,Args...>,
            LD::Context<LD::DatabaseError,Args...>
            >
    {

    };
}
#endif //LANDESSDEVCORE_INSERTIONREQUEST_H
