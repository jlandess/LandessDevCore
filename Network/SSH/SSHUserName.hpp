//
// Created by phoenixflower on 2/14/21.
//

#ifndef LANDESSDEVCORE_SSHUSERNAME_HPP
#define LANDESSDEVCORE_SSHUSERNAME_HPP
#include "Definitions/Integer.hpp"
#include "Primitives/General/Immutable.hpp"
namespace LD
{
    template<typename T>
    class SSHUserName;


    template<LD::UInteger Size>
    class SSHUserName<LD::ImmutableString<Size>>
    {
    private:
        LD::ImmutableString<Size> mUserName;
    public:
        SSHUserName() noexcept
        {

        }
        SSHUserName(LD::ImmutableString<Size> name) noexcept:mUserName{name}
        {

        }
        LD::StringView Value() const noexcept
        {
            return LD::StringView {this->mUserName.Data()};
        }
    };

    template<LD::UInteger Size> SSHUserName(LD::ImmutableString<Size> ) -> SSHUserName<LD::ImmutableString<Size>>;
}
#endif //LANDESSDEVCORE_SSHUSERNAME_HPP
