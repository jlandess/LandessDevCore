//
// Created by phoenixflower on 2/14/21.
//

#ifndef LANDESSDEVCORE_SSHPASSWORD_HPP
#define LANDESSDEVCORE_SSHPASSWORD_HPP
namespace LD
{
    template<typename T>
    class SSHPassword;


    template<LD::UInteger Size>
    class SSHPassword<LD::ImmutableString<Size>>
    {
    private:
        LD::ImmutableString<Size> mUserName;
    public:
        SSHPassword() noexcept
        {

        }
        SSHPassword(LD::ImmutableString<Size> name) noexcept:mUserName{name}
        {

        }
        LD::StringView Value() const noexcept
        {
            return LD::StringView {this->mUserName.Data()};
        }
    };

    template<LD::UInteger Size> SSHPassword(LD::ImmutableString<Size> ) -> SSHPassword<LD::ImmutableString<Size>>;
}
#endif //LANDESSDEVCORE_SSHPASSWORD_HPP
