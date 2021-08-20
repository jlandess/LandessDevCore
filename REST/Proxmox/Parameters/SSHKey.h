//
// Created by phoenixflower on 2/15/21.
//

#ifndef LANDESSDEVCORE_SSHKEY_H
#define LANDESSDEVCORE_SSHKEY_H
#include "Primitives/General/Immutable.hpp"
namespace LD
{
    namespace PVE
    {
        template<typename StringType>
        class SSHKey
        {
        private:
            StringType mKey;
        public:
            SSHKey() = default;
            SSHKey(StringType && key) noexcept:mKey{LD::Forward<StringType>(key)}
            {

            }
            auto Key() const noexcept
            {
                return LD::ImmutableString{"ssh-public-keys"};
            }
            auto Value() const noexcept
            {
                return this->mKey;
            }
        };

        template<typename StringType> SSHKey(StringType &&) -> SSHKey<StringType>;
    }
}
#endif //LANDESSDEVCORE_SSHKEY_H
