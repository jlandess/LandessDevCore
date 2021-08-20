//
// Created by phoenixflower on 2/14/21.
//

#ifndef LANDESSDEVCORE_SSHCOMMAND_HPP
#define LANDESSDEVCORE_SSHCOMMAND_HPP
#include "Primitives/General/Immutable.hpp"
#include "Definitions/Integer.hpp"
#include "Primitives/General/StringView.hpp"
namespace LD
{
    template<typename T>
    class RawPackageName;



    template<LD::UInteger Size>
    class RawPackageName<LD::ImmutableString<Size>>
    {
    private:
        LD::ImmutableString<Size> mPackageName;
    public:
        RawPackageName() noexcept
        {

        }
        RawPackageName(LD::ImmutableString<Size> string) noexcept:mPackageName{string}
        {

        }
        auto PackageName() const noexcept
        {
            return this->mPackageName;
        }
    };

    template<LD::UInteger Size> RawPackageName(LD::ImmutableString<Size> ) -> RawPackageName<LD::ImmutableString<Size>>;

    template<typename Package>
    class AptInstall
    {
    private:
        Package mPackage;
    public:
        AptInstall() noexcept
        {

        }
        AptInstall(Package && package) noexcept:mPackage{package}
        {

        }
        auto CommandFragment() const
        {
            return LD::ImmutableString{"install -y "}+this->mPackage.PackageName();
        }
    };

    template<typename T> AptInstall(T && package) -> AptInstall<T>;

    class AptUpdate
    {
    private:
    public:
        auto CommandFragment() const
        {
            return LD::ImmutableString{"update"};
        }
    };
    class AptUpgrade
    {
    private:

    public:
        auto CommandFragment() const noexcept
        {
            return LD::ImmutableString{"upgrade"};
        }
    };


    class AptReinstall
    {

    };

    class AptPurge
    {

    };

    class AptRemove
    {

    };

    class AptAutoRemove
    {

    };

    class AptSearch
    {

    };


    template<typename T>
    class AptCommand
    {
    private:
        T mSubCommand;
    public:
        AptCommand() noexcept
        {

        }
        AptCommand(T && command) noexcept:mSubCommand{command}
        {

        }
        auto Command() const noexcept
        {
            return LD::ImmutableString{"apt "}+this->mSubCommand.CommandFragment();
        }
    };



    template<typename T> AptCommand(T && ) -> AptCommand<T>;
    template<LD::UInteger Size>
    class SSHRawCommand
    {
    private:
        LD::ImmutableString<Size> mCommand;
    public:
        constexpr SSHRawCommand() noexcept
        {

        }
        SSHRawCommand(LD::ImmutableString<Size> command) noexcept:mCommand{command}
        {

        }

        auto Command() const noexcept
        {
            return LD::StringView{this->mCommand.Data()};
        }

    };

    template<LD::UInteger Size> SSHRawCommand(LD::ImmutableString<Size> ) -> SSHRawCommand<Size>;


    template<typename T>
    class SSHCommand
    {
    private:
        T mCommand;
    public:
        SSHCommand() noexcept
        {

        }
        SSHCommand(T && command) noexcept: mCommand{LD::Forward<T>(command)}
        {

        }
        LD::StringView Command() const noexcept
        {
            return this->mCommand.Command();
        }
    };

    template<typename T> SSHCommand(T && ) -> SSHCommand<T>;
}
#endif //LANDESSDEVCORE_SSHCOMMAND_HPP
