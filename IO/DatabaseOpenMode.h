//
// Created by phoenixflower on 5/21/20.
//

#ifndef LANDESSDEVCORE_DATABASEOPENMODE_H
#define LANDESSDEVCORE_DATABASEOPENMODE_H
#include "Primitives/General/mapboxvariant.hpp"
namespace LD
{
    class OpenReadOnly
    {

    };

    class OpenAndCreateIfNotExists
    {

    };

    class OpenReadAndWrite
    {

    };

    class OpenInMemory
    {

    };

    using DatabaseOpenMode = LD::Variant<OpenReadOnly,OpenAndCreateIfNotExists,OpenInMemory,OpenReadAndWrite>;
}
#endif //LANDESSDEVCORE_DATABASEOPENMODE_H
