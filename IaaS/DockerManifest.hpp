#ifndef DOCKER_MANIFEST_LANDESSDEV_HPP
#define DOCKER_MANIFEST_LANDESSDEV_HPP

#include <Reflection/NamedTuple.h>
#include "Primitives/General/Immutable.hpp"
#include "Network/IPV4Address.hpp"
#include "Network/IPV6Address.hpp"
#include "Reflection/Reflection.hpp"
namespace LD
{
    namespace Docker
    {
        class APICorsHeader
        {
        public:
            static constexpr auto Name = LD::ImmutableString{"api-cors-header"};
        };

        class AuthorizationPlugins
        {
        public:
            static constexpr auto Name = LD::ImmutableString{"authorization-plugins"};
        };

        class Bridge
        {
        public:
            static constexpr auto Name = LD::ImmutableString{"bridge"};
        };
        class CGroupParent
        {
        public:
            static constexpr auto Name = LD::ImmutableString{"cgroup-parent"};
        };
        class Debug
        {
        private:
            bool mValue;
        public:
            static constexpr auto Name = LD::ImmutableString{"debug"};
            using Type = bool;
            Debug() = default;
            Debug(bool value) noexcept:mValue{value}{}

            const bool & Value() const noexcept
            {
                return this->mValue;
            }
            bool & Value() noexcept
            {
                return this->mValue;
            }
        };

        class DefaultGateway
        {
        private:
            LD::IPV4Address mGatewayAddress;
        public:
            static constexpr auto Name = LD::ImmutableString{"default-gateway"};
        };

        class DefaultGateway6
        {
        private:
            LD::IPV6Address mGatewayAddress;
        public:
            static constexpr auto Name = LD::ImmutableString{"default-gateway-v6"};
        };

        template<typename StringType>
        class DefaultRuntime
        {
        private:
            StringType mRunTimeName;
        public:
            static constexpr auto Name = LD::ImmutableString{"default-runtime"};
        };


        class DisableLegacyRegistry
        {
        private:
            bool mValue;
        public:
            static constexpr auto Name = LD::ImmutableString{"disable-legacy-registry"};
        };

        class DNS
        {
        public:
            static constexpr auto Name = LD::ImmutableString{"dns"};
        };

        class DNSOptions
        {
        public:
            static constexpr auto Name = LD::ImmutableString{"dns-opts"};
        };

        class DNSSearch
        {
        public:
            static constexpr auto Name = LD::ImmutableString{"dns-search"};
        };

        class ExecOptions
        {
        public:
            static constexpr auto Name = LD::ImmutableString{"exec-opts"};
        };

        class ExecRoot
        {
        public:
            static constexpr auto Name = LD::ImmutableString{"exec-root"};
        };
        class FixedCIDR
        {
        public:
            static constexpr auto Name = LD::ImmutableString{"fixed-cidr"};
        };

        class FixedCIDR6
        {
        private:
            LD::IPV6NetworkSegment mSegment;
        public:
            static constexpr auto Name = LD::ImmutableString{"fixed-cidr-v6"};
            using Type = LD::IPV6NetworkSegment;
            FixedCIDR6() = default;

            FixedCIDR6(LD::IPV6NetworkSegment segment) noexcept:mSegment{segment}{}

            const LD::IPV6NetworkSegment & Value() const noexcept
            {
                return this->mSegment;
            }

        };
        class Graph
        {
        public:
            static constexpr auto Name = LD::ImmutableString{"graph"};
        };

        class Group
        {
        public:
            static constexpr auto Name = LD::ImmutableString{"group"};
        };

        //"hosts": ["fd://","unix:///var/run/docker.sock","tcp://0.0.0.0:2376"]
        class Hosts
        {
        public:
            static constexpr auto Name = LD::ImmutableString{"hosts"};
        };

        class ICC
        {
        private:
            bool mValue;
        public:
            static constexpr auto Name = LD::ImmutableString{"icc"};
            using Type = bool;
            ICC() = default;
            ICC(bool value) noexcept:mValue{value}{}
            const bool & Value() const noexcept
            {
                return this->mValue;
            }
            bool & Value() noexcept
            {
                return this->mValue;
            }
        };

        class InsecureRegistries
        {
        public:
            static constexpr auto Name = LD::ImmutableString{"insecure-registries"};
        };
        class BIP
        {
        private:
            LD::IPV4Address mHost;
        public:
            static constexpr auto Name = LD::ImmutableString{"bip"};
        };
        class IP
        {
        public:
            static constexpr auto Name = LD::ImmutableString{"ip"};
        };

        class IPTables
        {
        private:
            bool mValue;
        public:
            static constexpr auto Name = LD::ImmutableString{"iptables"};
            using Type = bool;
            IPTables() = default;
            IPTables(bool value) noexcept:mValue{value}{}

            const bool & Value() const noexcept
            {
                return this->mValue;
            }

            bool & Value() noexcept
            {
                return this->mValue;
            }
        };

        class IPV6
        {
        private:
            bool mValue;
        public:
            static constexpr auto Name = LD::ImmutableString{"ipv6"};
            using Type = bool;
            IPV6() = default;
            IPV6(bool value) noexcept:mValue{value}
            {

            }

            const bool & Value() const noexcept
            {
                return this->mValue;
            }

            bool & Value() noexcept
            {
                return this->mValue;
            }
        };

        class IPForward
        {
        private:
            bool mValue;
        public:
            static constexpr auto Name = LD::ImmutableString{"ip-forward"};
            using Type = bool;
            IPForward() = default;
            IPForward(bool value) noexcept:mValue{value}{}
            const bool & Value() const noexcept{return this->mValue;}
            bool & Value() noexcept{return this->mValue;}
        };
        class IPMasq
        {
        private:
            bool mValue;
        public:
            static constexpr auto Name = LD::ImmutableString{"ip-masq"};
            using Type = bool;
            IPMasq() = default;
            IPMasq(bool value) noexcept:mValue{value}{}
            const bool & Value() const noexcept{return this->mValue;}
            bool & Value() noexcept {return this->mValue;}
        };
        template<typename ... T>
        class Labels
        {
        private:
            LD::StaticArray<mpark::variant<T...>,sizeof...(T)> mLabels;
        public:
            static constexpr auto Name = LD::ImmutableString{"labels"};
        };

        class LiveRestore
        {
        private:
            bool mValue;
        public:
            static constexpr auto Name = LD::ImmutableString{"live-restore"};
            using Type = bool;
            LiveRestore() = default;
            LiveRestore(bool value) noexcept:mValue{value}{}
            const bool & Value() const noexcept{return this->mValue;}
            bool & Value() noexcept{return this->mValue;}
        };

        class LogDriver
        {
        public:
            static constexpr auto Name = LD::ImmutableString{"log-driver"};
        };

        class LogLevel
        {
        public:
            static constexpr auto Name = LD::ImmutableString{"log-level"};
        };

        class LogOptions
        {
        public:
            static constexpr auto Name = LD::ImmutableString{"log-opts"};
        };

        class MaxConcurrentDownlaods
        {
        private:
            LD::UInteger mValue;
        public:
            static constexpr auto Name = LD::ImmutableString{"max-concurrent-downloads"};
            using Type = LD::UInteger;
            MaxConcurrentDownlaods() noexcept:mValue{3}{}
            MaxConcurrentDownlaods(LD::UInteger value) noexcept:mValue{value}{}
            const LD::UInteger & Value() const noexcept{return this->mValue;}
            LD::UInteger & Value() noexcept{return this->mValue;}

        };

        class MaxConcurrentUploads
        {
        private:
            LD::UInteger mValue;
        public:
            static constexpr auto Name = LD::ImmutableString{"max-concurrent-uploads"};
            using Type = LD::UInteger;
            MaxConcurrentUploads() noexcept:mValue{5}{}
            MaxConcurrentUploads(LD::UInteger value) noexcept:mValue{value}{}
            const LD::UInteger & Value() const noexcept{return this->mValue;}
            LD::UInteger & Value() noexcept{return this->mValue;}
        };

        class MTU
        {
        private:
            LD::UInteger mValue;
        public:
            using Type = LD::UInteger;
            static constexpr auto Name = LD::ImmutableString{"mtu"};
            MTU() noexcept:mValue{1500}
            {

            }
            MTU(const LD::UInteger & mtu) noexcept:mValue{mtu}
            {

            }
            LD::UInteger Value() const noexcept
            {
                return this->mValue;
            }
        };

        class OOMScoreAdjust
        {
        private:
            LD::Integer mValue;
        public:
            using Type = LD::Integer;
            static constexpr auto Name = LD::ImmutableString{"oom-score-adjust"};
            OOMScoreAdjust() = default;
            OOMScoreAdjust(LD::Integer value) noexcept:mValue{value}{}
            const LD::Integer & Value() const noexcept{return this->mValue;}
            LD::Integer & Value() noexcept{return this->mValue;}
        };

        class PIDFile
        {
        public:
            static constexpr auto Name = LD::ImmutableString{"pidfile"};
        };

        class RawLogs
        {
        private:
            bool mValue;
        public:
            using Type = bool;
            static constexpr auto Name = LD::ImmutableString{"raw-logs"};
            RawLogs() = default;
            RawLogs(bool value) noexcept:mValue{value}{}
            const bool & Value() const noexcept{return this->mValue;}
            bool & Value() noexcept{return this->mValue;}
        };

        template<typename ... Mirrors>
        class RegistryMirrors
        {
        private:
            LD::ImmutableArray<mpark::variant<Mirrors...>,sizeof...(Mirrors)> mMirrors;
        public:
            using Type = LD::ImmutableArray<mpark::variant<Mirrors...>,sizeof...(Mirrors)>;
            static constexpr auto Name = LD::ImmutableString{"registry-mirrors"};
            RegistryMirrors() noexcept
            {

            }
            RegistryMirrors(Mirrors && ... mirrors) noexcept:mMirrors{LD::Forward<Mirrors>(mirrors)...}
            {

            }
            const Type & Value() const noexcept
            {
                return this->mMirrors;
            }
        };

        template<typename ... T> RegistryMirrors(T&& ... registries) -> RegistryMirrors<T...>;

        template<typename ... Runtimes>
        class RunTimes
        {
        private:
            LD::ImmutableArray<mpark::variant<Runtimes>...> mMirrors;
        public:
            static constexpr auto Name = LD::ImmutableString{"runtimes"};
        };

        class SELinux
        {
        private:
            bool mValue;
        public:
            static constexpr auto Name = LD::ImmutableString{"selinux-enabled"};
            using Type = bool;
            SELinux() = default;
            SELinux(bool value) noexcept:mValue{value}{}
            const bool & Value() const noexcept{return this->mValue;}
            bool & Value() noexcept{return this->mValue;}
        };

        class StorageDriver
        {
        public:
            static constexpr auto Name = LD::ImmutableString{"storage-driver"};
        };

        class StorageOptions
        {
        public:
            static constexpr auto Name = LD::ImmutableString{"storage-opts"};
        };

        class TLS
        {
        private:
            bool mValue;
        public:
            static constexpr auto Name = LD::ImmutableString{"tls"};
            using Type = bool;
            TLS() = default;
            TLS(bool value) noexcept:mValue{value}{}
            const bool & Value() const noexcept{return this->mValue;}
            bool & Value() noexcept{return this->mValue;}

        };
        class TLSCaCert
        {
        public:
            static constexpr auto Name = LD::ImmutableString{"tlscacert"};
        };

        class TLSCert
        {
        public:
            static constexpr auto Name = LD::ImmutableString{"tlscert"};
        };

        class TLSKey
        {
        public:
            static constexpr auto Name = LD::ImmutableString{"tlskey"};
        };

        class TLSVerify
        {
        public:
            static constexpr auto Name = LD::ImmutableString{"tlsverify"};
        };

        class UserlandProxy
        {
        private:
            bool mValue;
        public:
            static constexpr auto Name = LD::ImmutableString{"userland-proxy"};
            UserlandProxy() = default;
            UserlandProxy(bool value) noexcept:mValue{value}{}
            const bool & Value() const noexcept{return this->mValue;}
            bool & Value() noexcept{return this->mValue;}
        };
        class UserlandProxyPath
        {
        public:
            static constexpr auto Name = LD::ImmutableString{"userland-proxy-path"};
        };
        class UserNSRemap
        {
        public:
            static constexpr auto Name = LD::ImmutableString{"userns-remap"};
        };




        class Experimental
        {
        public:
            static constexpr auto Name = LD::ImmutableString{"experimental"};
        };
        class SecComp
        {
        public:
            static constexpr auto Name = ctre::fixed_string{"seccomp-profile"};
        };

        class ShutdownTimeout
        {
        public:
            static constexpr auto Name = ctre::fixed_string{"shutdown-timeout"};
        };
        class DefaultULimits
        {
        public:
            static constexpr auto Name = ctre::fixed_string{"default-ulimits"};

        };
        class Init
        {
        public:
            static constexpr auto Name = ctre::fixed_string{"init"};
        };
        class InitPath
        {
        public:
            static constexpr auto Name = ctre::fixed_string{"init-path"};
        };


        template<typename ... T>
        class Manifest
        {
        private:
            LD::Tuple<LD::NamedType<T::Name, typename T::Type>...> mVariables;
        public:
            Manifest() = default;

            Manifest(T && ... objects) noexcept:mVariables{LD::NamedType<T::Name, typename T::Type>{typename T::Type(objects.Value())}...}
            {

            }

            using OptionsType = LD::Tuple<LD::NamedType<T::Name, typename T::Type>...> ;
            LD::Tuple<LD::NamedType<T::Name, typename T::Type>...>  & Options() noexcept
            {
                return this->mVariables;
            }

            const LD::Tuple<LD::NamedType<T::Name, typename T::Type>...>  & Options() const noexcept
            {
                return this->mVariables;
            }
        };

        template<typename ... T> Manifest(T && ...) -> Manifest<T...>;

    }
}

namespace LD
{
    inline auto ToImmutableString(LD::Docker::MTU mtu) noexcept
    {
        return LD::ToImmutableString(mtu.Value());
    }
}

template<typename ... Options>
struct LD::CT::TypeDescriptor<LD::Docker::Manifest<Options...>>
{
private:
    static constexpr auto BaseName = ctll::basic_fixed_string("Options");
public:
    static constexpr auto ClassName = ctll::fixed_string{"DockerManifest"};

    using MemberList = LD::CT::TypeList<
            LD::CT::EncapsulatedMemberDescriptor<BaseName,LD::CT::SelectOverload<typename LD::Docker::Manifest<Options...>::OptionsType & (LD::Docker::Manifest<Options...>::*)(),&LD::Docker::Manifest<Options...>::Options>(),LD::CT::SelectOverload<const typename LD::Docker::Manifest<Options...>::OptionsType & (LD::Docker::Manifest<Options...>::*)() const,&LD::Docker::Manifest<Options...>::Options>()>
    >;


    static constexpr MemberList Members{  };

};
#endif