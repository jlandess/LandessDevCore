//
// Created by phoenixflower on 2/14/21.
//

#ifndef LANDESSDEVCORE_SSH_HPP
#define LANDESSDEVCORE_SSH_HPP
#include <libssh/libssh.h>
#include <memory>
#include "SSHHost.hpp"
#include "SSHLogLevel.hpp"
#include "SSHPassword.hpp"
#include "SSHPort.hpp"
#include "SSHPublicAuthenticationAuto.hpp"
#include "SSHUserName.hpp"
#include "SSHCommand.hpp"
#include "SSHCommandSet.hpp"
#include "Core/RequestResponse.hpp"
#include <pwd.h>
namespace LD
{
    inline int show_remote_files(ssh_session session)
    {
        ssh_channel channel;
        int rc;

        channel = ssh_channel_new(session);
        if (channel == NULL) return SSH_ERROR;

        rc = ssh_channel_open_session(channel);
        if (rc != SSH_OK)
        {
            ssh_channel_free(channel);
            return rc;
        }

        rc = ssh_channel_request_exec(channel, "apt-get install -y keepalived");
        if (rc != SSH_OK)
        {
            ssh_channel_close(channel);
            ssh_channel_free(channel);
            return rc;
        }

        char buffer[256];
        int nbytes;

        nbytes = ssh_channel_read(channel, buffer, sizeof(buffer), 0);
        while (nbytes > 0)
        {
            if (fwrite(buffer, 1, nbytes, stdout) != nbytes)
            {
                ssh_channel_close(channel);
                ssh_channel_free(channel);
                return SSH_ERROR;
            }
            nbytes = ssh_channel_read(channel, buffer, sizeof(buffer), 0);
        }

        if (nbytes < 0)
        {
            ssh_channel_close(channel);
            ssh_channel_free(channel);
            return SSH_ERROR;
        }

        ssh_channel_send_eof(channel);
        ssh_channel_close(channel);
        ssh_channel_free(channel);

        return SSH_OK;
    }
    template<typename T>
    inline int ExecuteCommand(ssh_session session, LD::SSHCommand<T> command) noexcept
    {
        auto reclaimChannel = [](ssh_channel_struct * channel) noexcept
        {
            ssh_channel_close(channel);
            ssh_channel_free(channel);
        };
        std::unique_ptr<ssh_channel_struct,void(*)(ssh_channel_struct*)> channel{ssh_channel_new(session),reclaimChannel};
        int rc;

        if (channel == NULL) return SSH_ERROR;

        rc = ssh_channel_open_session(channel.get());
        if (rc != SSH_OK)
        {
            return rc;
        }

        rc = ssh_channel_request_exec(channel.get(), command.Command().data());
        if (rc != SSH_OK)
        {
            return rc;
        }

        char buffer[256];
        int nbytes;

        nbytes = ssh_channel_read(channel.get(), buffer, sizeof(buffer), 0);
        while (nbytes > 0)
        {
            if (fwrite(buffer, 1, nbytes, stdout) != nbytes)
            {
                return SSH_ERROR;
            }
            nbytes = ssh_channel_read(channel.get(), buffer, sizeof(buffer), 0);
        }

        if (nbytes < 0)
        {
            return SSH_ERROR;
        }

        ssh_channel_send_eof(channel.get());
        return SSH_OK;
    }
    inline int verify_knownhost(ssh_session session) noexcept
    {
        enum ssh_known_hosts_e state;
        unsigned char *hash = NULL;
        ssh_key srv_pubkey = NULL;
        size_t hlen;
        char buf[10];
        char *hexa;
        char *p;
        int cmp;
        int rc;

        rc = ssh_get_server_publickey(session, &srv_pubkey);
        if (rc < 0) {
            return -1;
        }

        rc = ssh_get_publickey_hash(srv_pubkey,
                                    SSH_PUBLICKEY_HASH_SHA1,
                                    &hash,
                                    &hlen);
        ssh_key_free(srv_pubkey);
        if (rc < 0) {
            return -1;
        }

        state = ssh_session_is_known_server(session);
        switch (state) {
            case SSH_KNOWN_HOSTS_OK:
                /* OK */

                break;
            case SSH_KNOWN_HOSTS_CHANGED:
                fprintf(stderr, "Host key for server changed: it is now:\n");
                ssh_print_hexa("Public key hash", hash, hlen);
                fprintf(stderr, "For security reasons, connection will be stopped\n");
                ssh_clean_pubkey_hash(&hash);

                return -1;
            case SSH_KNOWN_HOSTS_OTHER:
                fprintf(stderr, "The host key for this server was not found but an other"
                                "type of key exists.\n");
                fprintf(stderr, "An attacker might change the default server key to"
                                "confuse your client into thinking the key does not exist\n");
                ssh_clean_pubkey_hash(&hash);

                return -1;
            case SSH_KNOWN_HOSTS_NOT_FOUND:
                fprintf(stderr, "Could not find known host file.\n");
                fprintf(stderr, "If you accept the host key here, the file will be"
                                "automatically created.\n");

                /* FALL THROUGH to SSH_SERVER_NOT_KNOWN behavior */

            case SSH_KNOWN_HOSTS_UNKNOWN:
                hexa = ssh_get_hexa(hash, hlen);
                fprintf(stderr,"The server is unknown. Do you trust the host key?\n");
                fprintf(stderr, "Public key hash: %s\n", hexa);
                ssh_string_free_char(hexa);
                ssh_clean_pubkey_hash(&hash);
                p = fgets(buf, sizeof(buf), stdin);
                if (p == NULL) {
                    return -1;
                }

                cmp = strncasecmp(buf, "yes", 3);
                if (cmp != 0) {
                    return -1;
                }

                rc = ssh_session_update_known_hosts(session);
                if (rc < 0) {
                    fprintf(stderr, "Error %s\n", strerror(errno));
                    return -1;
                }

                break;
            case SSH_KNOWN_HOSTS_ERROR:
                fprintf(stderr, "Error %s", ssh_get_error(session));
                ssh_clean_pubkey_hash(&hash);
                return -1;
        }

        ssh_clean_pubkey_hash(&hash);
        return 0;
    }

    class SSH
    {
        std::unique_ptr<ssh_session_struct,void(*)(ssh_session_struct*)> mHandle;
        LD::UInteger mRC;
        int mPort;
        int mVerbosity;
    public:
        SSH() noexcept:mHandle{ssh_new(),ssh_free},mPort{22},mVerbosity{SSH_LOG_PROTOCOL}
        {
            //ssh_options_set(this->mHandle.get(), SSH_OPTIONS_HOST, "2600:1700:6070:65df:3ca7:b1ff:fe83:7d9c");
            //ssh_options_set(this->mHandle.get(), SSH_OPTIONS_LOG_VERBOSITY, &this->mVerbosity);
            //ssh_options_set(this->mHandle.get(), SSH_OPTIONS_PORT, &this->mPort);

            //ssh_options_set(this->mHandle.get(),SSH_OPTIONS_USER,"root");
            //this->mRC = ssh_connect(this->mHandle.get());

            //if (LD::verify_knownhost(this->mHandle.get()) < 0)
            //{
            //}

            //rc = ssh_userauth_password(my_ssh_session, NULL, password);

            //this->mRC = ssh_userauth_publickey_auto(this->mHandle.get(), NULL, NULL);
            //if (this->mRC != SSH_AUTH_SUCCESS)
            //{
                //fprintf(stderr, "Error authenticating with password: %s\n",
                        //ssh_get_error(this->mHandle.get()));
                //exit(-1);
            //}
            //LD::SSHCommand command{LD::AptCommand{LD::AptInstall{LD::RawPackageName{LD::ImmutableString{"keepalived"}}}}};


            //std::cout << command.Command().Data() << std::endl;
            //LD::ExecuteCommand(this->mHandle.get(),command);
            //ssh_free(my_ssh_session);
        }


        SSH(LD::StringView hostname) noexcept:mHandle{ssh_new(),ssh_free}
        {
            ssh_options_set(this->mHandle.get(), SSH_OPTIONS_LOG_VERBOSITY, &this->mVerbosity);
        }
        template<typename U>
        void SetHost(LD::SSHHost host, LD::SSHPort<U> port) noexcept
        {


            //ssh_options_set(this->mHandle.get(), SSH_OPTIONS_HOST, host.Value().data());
            //ssh_options_set(this->mHandle.get(), SSH_OPTIONS_HOST, "2600:1700:6070:65df:3ca7:b1ff:fe83:7d9c");
            //ssh_options_set(this->mHandle.get(), SSH_OPTIONS_PORT, &port.Port());

        }

        void SetHost(LD::SSHHost host) noexcept
        {
            std::cout << LD::ToImmutableString(host.Address()).Data() << std::endl;
            //ssh_options_set(this->mHandle.get(), SSH_OPTIONS_HOST, "2600:1700:6070:65df:3ce7:cfff:fe00:bc9a");
            ssh_options_set(this->mHandle.get(),SSH_OPTIONS_HOST,LD::ToImmutableString(host.Address()).Data());
            ssh_options_set(this->mHandle.get(), SSH_OPTIONS_LOG_VERBOSITY, &this->mVerbosity);
            ssh_options_set(this->mHandle.get(), SSH_OPTIONS_PORT, &this->mPort);
            ssh_options_set(this->mHandle.get(),SSH_OPTIONS_GLOBAL_KNOWNHOSTS,"~/.ssh/known_hosts");
        }
        template<typename T, typename U, typename ... A>
        LD::RequestResponse<bool(A...)> Connect(LD::SSHUserName<T> userName, LD::SSHPassword<U> password, A && ... args) noexcept
        {
            this->mRC = ssh_connect(this->mHandle.get());

            if (LD::verify_knownhost(this->mHandle.get()) < 0)
            {
                return LD::CreateResponse(LD::Type<bool>{},LD::TransactionError{},LD::Forward<A>(args)...);
            }

            this->mRC = ssh_userauth_password(this->mHandle.get(), NULL, password.Value().get());

            if (this->mRC != SSH_AUTH_SUCCESS)
            {
                return LD::CreateResponse(LD::Type<bool>{},LD::TransactionError{},LD::Forward<A>(args)...);
            }

            return LD::CreateResponse(LD::Type<bool>{},LD::TransactionError{},LD::Forward<A>(args)...);
        }

        ssh_session_struct * Handle() const noexcept
        {
            return this->mHandle.get();
        }

        template<typename T, typename ... A>
        LD::RequestResponse<bool(A...)> Connect(LD::SSHUserName<T> userName, LD::SSHPublicKeyAuthentication publicKeyAuth, A && ... args) noexcept
        {
            ssh_options_set(this->mHandle.get(),SSH_OPTIONS_USER,userName.Value().data());
            this->mRC = ssh_connect(this->mHandle.get());

            ssh_key key = ssh_key_new();
            //ssh_try_publickey_from_file()
            //ssh_publickey_hash_type keyType = ssh_key_type_from_name("phoenixflower@pop-os");
            unsigned char * keyBuffer = nullptr;
            size_t length;

            //ssh_get_publickey_hash(key, static_cast<ssh_publickey_hash_type>(keyType), &keyBuffer, &length);


            //ssh_key
            //ssh_get_publickey_hash()
            if (LD::verify_knownhost(this->mHandle.get()) < 0)
            {
                return LD::CreateResponse(LD::Type<bool>{},LD::TransactionError{},LD::Forward<A>(args)...);
            }

            this->mRC = ssh_userauth_publickey_auto(this->mHandle.get(), NULL, NULL);

            if (this->mRC != SSH_AUTH_SUCCESS)
            {
                return LD::CreateResponse(LD::Type<bool>{},LD::TransactionError{},LD::Forward<A>(args)...);
            }
            return LD::CreateResponse(LD::Type<bool>{},LD::TransactionError{},LD::Forward<A>(args)...);
        }
        template<typename T, typename ... A>
        LD::RequestResponse<bool(A...)> ConnectAndAutoAccept(LD::SSHUserName<T> userName, LD::SSHPublicKeyAuthentication publicKeyAuth, A && ... args) noexcept
        {
            ssh_options_set(this->mHandle.get(),SSH_OPTIONS_USER,userName.Value().data());
            this->mRC = ssh_connect(this->mHandle.get());

            if (this->mRC != SSH_OK)
            {
                return LD::CreateResponse(LD::Type<bool>{},LD::TransactionError{},LD::Forward<A>(args)...);
            }
            this->mRC = ssh_userauth_publickey_auto(this->mHandle.get(), NULL, NULL);

            if (this->mRC != SSH_AUTH_SUCCESS)
            {
                return LD::CreateResponse(LD::Type<bool>{},LD::TransactionError{},LD::Forward<A>(args)...);
            }
            return LD::CreateResponse(LD::Type<bool>{},true,LD::Forward<A>(args)...);
        }
        template<typename T, typename U, typename ... A>
        LD::RequestResponse<bool(A...)> Connect(
                LD::SSHUserName<T> userName,
                LD::SSHPublicKeyAuthentication publicKeyAuth,
                LD::SSHPassword<U> password,
                A && ... args) noexcept
        {
            return LD::CreateResponse(LD::Type<bool>{},LD::TransactionError{},LD::Forward<A>(args)...);
        }

        template<typename T, typename H,typename ... A>
        LD::RequestResponse<H(A...)> Execute(LD::Type<H>,LD::SSHCommand<T> command, A && ... args) const noexcept
        {
            LD::UInteger rc = LD::ExecuteCommand(this->mHandle.get(),command);
            return LD::CreateResponse(LD::Type<H>{},LD::TransactionError{},LD::Forward<A>(args)...);
        }

        template<typename H,typename ... A, typename ... B>
        LD::RequestResponse<H(A...)> Execute(LD::Type<H>,LD::SSHCommandSet<B...> command, A && ... args) const noexcept
        {
            auto commandSet = command.Commands();
            constexpr LD::UInteger Size = LD::CT::TupleSize(LD::Type<decltype(commandSet)>{});
            LD::For<Size>([](
                    auto I,
                    const auto & commands,
                    const SSH * instance)
            {
                instance->Execute(LD::Type<bool>{},LD::Get(LD::Get<I>(commands)));
                return true;
            },commandSet,this);
            //this->mRC = LD::ExecuteCommand(this->mHandle.get(),command);
            return LD::CreateResponse(LD::Type<H>{},LD::TransactionError{},LD::Forward<A>(args)...);
        }

        static LD::ImmutableString<3000> GetPublicKey() noexcept
        {
            LD::ImmutableString<128> mUserName;
            LD::ImmutableString<3000> buffer;
            uid_t uid = geteuid ();
            struct passwd *pw = getpwuid (uid);
            LD::UInteger size = strlen(pw->pw_name);
            for(LD::UInteger n = 0;n<size;++n)
            {
                mUserName[n] = pw->pw_name[n];
            }
            auto keyLocation = LD::ImmutableString{"/home/"}+mUserName+LD::ImmutableString{"/.ssh/id_rsa.pub"};
            std::unique_ptr<FILE,int(*)(FILE*)> f{fopen(keyLocation.Data(),"r"),fclose};
            //FILE * f = fopen(keyLocation.Data(),"r");
            LD::UInteger length = 0;
            if (f)
            {
                fseek (f.get(), 0, SEEK_END);
                length = ftell (f.get());
                fseek (f.get(), 0, SEEK_SET);
                //account for newline
                fread((char*)buffer.Data(),1,length-1,f.get());
            }
            return buffer;
        }

    };
}
#endif //LANDESSDEVCORE_SSH_HPP
