#ifndef SCP_LANDESSDEVCORE_HPP
#define SCP_LANDESSDEVCORE_HPP
#include "SSH.hpp"
#include "Memory/ElementReference.h"
#include "Primitives/General/StringView.hpp"
#include "Core/RequestResponse.hpp"
#include <sys/stat.h>
namespace LD
{
    class SCPWriter
    {
    private:
        std::unique_ptr<ssh_scp_struct,void(*)(ssh_scp_struct*)> mHandle;
    public:
        SCPWriter(): mHandle{nullptr,ssh_scp_free}
        {

        }
        SCPWriter(LD::ElementReference<LD::SSH> session, LD::StringView directory = ".") noexcept:mHandle{ssh_scp_new(session->Handle(), SSH_SCP_WRITE | SSH_SCP_RECURSIVE, directory.data()),ssh_scp_free}
        {
            LD::UInteger connectionStatus = ssh_scp_init(this->mHandle.get());

            if(connectionStatus != SSH_OK)
            {

            }
        }

        template<typename ... A>
        LD::RequestResponse<bool(A...)> PushToDirectory(LD::StringView directory, LD::StringView fileName,LD::StringView name,LD::StringView data, A && ... args) noexcept
        {
            LD::UInteger actionStatus = ssh_scp_push_directory(this->mHandle.get(), directory.data(), S_IRWXU);
            actionStatus = ssh_scp_push_file(this->mHandle.get(), data.data(), data.length(), S_IRUSR |  S_IWUSR);

            if(actionStatus != SSH_OK)
            {

            }
            return return LD::CreateResponse(LD::Type<bool>{},LD::TransactionError{},LD::Forward<A>(args)...);
        }

        template<LD::UInteger Size, typename ... A>
        LD::RequestResponse<bool(A...)> PushToDirectory(LD::StringView directory, LD::StringView fileName,const LD::ImmutableString<Size> & string, A && ... args) noexcept
        {
            LD::UInteger actionStatus = ssh_scp_push_directory(this->mHandle.get(), directory.data(), S_IRWXU);
            actionStatus = ssh_scp_push_file(this->mHandle.get(), string.Data(), string.GetSize(), S_IRUSR |  S_IWUSR);

            if(actionStatus != SSH_OK)
            {

            }
            return  LD::CreateResponse(LD::Type<bool>{},LD::TransactionError{},LD::Forward<A>(args)...);
        }
    };
}


#endif