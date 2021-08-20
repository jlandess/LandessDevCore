//
// Created by phoenixflower on 1/27/21.
//

#ifndef LANDESSDEVCORE_NAMESERVER_HPP
#define LANDESSDEVCORE_NAMESERVER_HPP
#include "Definitions/Common.hpp"
namespace LD
{
    namespace PVE
    {
        template<typename StringType>
        class NameServer
        {
        private:
            StringType mNameServer;
        public:
            NameServer() noexcept
            {

            }
            NameServer(StringType && server) noexcept:mNameServer{LD::Forward<StringType>(server)}
            {

            }
            auto Key() const noexcept
            {
                return LD::ImmutableString{"searchdomain"};
            }
            auto Value() const noexcept
            {
                return this->mNameServer;
            }
        };

        template<typename StringType> NameServer(StringType &&) -> NameServer<StringType>;
    }
}
#endif //LANDESSDEVCORE_NAMESERVER_HPP
