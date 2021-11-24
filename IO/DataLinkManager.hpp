//
// Created by phoenixflower on 11/17/21.
//

#ifndef LANDESSDEVCORE_DATALINKMANAGER_HPP
#define LANDESSDEVCORE_DATALINKMANAGER_HPP
#include "DataLink.hpp"
#include "Primitives/General/StringView.hpp"
#include "Memory/MemoryResource.hpp"
namespace LD
{
    class PolymorphicLinkManager
    {
    private:

    public:
        virtual LD::DataLink & operator[](LD::StringView linkName) noexcept = 0;
    };
    template<typename Backend, typename Array>
    class BasicDataLinkManager: public LD::PolymorphicLinkManager
    {
    private:
        Array mCurrentConnections;

    public:

        LD::DataLink & operator[](LD::StringView linkName) noexcept
        {

        }
    };
}
#endif //LANDESSDEVCORE_DATALINKMANAGER_HPP
