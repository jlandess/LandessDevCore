//
// Created by phoenixflower on 11/20/20.
//

#ifndef LANDESSDEVCORE_QUEUELOGSTATUS_HPP
#define LANDESSDEVCORE_QUEUELOGSTATUS_HPP
#include "IO/FetchRequest.h"
#include "Primitives/General/StringView.hpp"
namespace LD
{
    namespace VW
    {
        enum LogStatusTag{Answered,Abandoned,Timeout,Login,Logout};

        class QueueLogStatus
        {
        private:
            LogStatusTag mStatus;
        public:
            constexpr QueueLogStatus() noexcept:mStatus{Abandoned}
            {

            }
            constexpr QueueLogStatus(LogStatusTag tag) noexcept:mStatus{tag}
            {

            }

            LogStatusTag Status() const noexcept
            {
                return this->mStatus;
            }

            bool operator == (const QueueLogStatus & status) const noexcept
            {
                return this->mStatus == status.mStatus;
            }

            bool operator != (const QueueLogStatus & status) const noexcept
            {
                return this->mStatus != status.mStatus;
            }

            bool operator < (const QueueLogStatus & status) const noexcept
            {
                return this->mStatus < status.mStatus;
            }

            bool operator > (const QueueLogStatus & status) const noexcept
            {
                return this->mStatus > status.mStatus;
            }

        };
    }

    inline auto ToImmutableString(const LD::VW::QueueLogStatus & status) noexcept
    {
        if (status.Status() == LD::VW::Answered)
        {
            LD::ImmutableString<15> answered;
            answered[0] = 'A';
            answered[1] = 'n';
            answered[2] = 's';
            answered[3] = 'w';
            answered[4] = 'e';
            answered[5] = 'r';
            answered[6] = 'e';
            answered[7] = 'd';
            return answered;
        }else if(status.Status() == LD::VW::Abandoned)
        {
            LD::ImmutableString<15> abandoned;
            abandoned[0] = 'A';
            abandoned[1] = 'b';
            abandoned[2] = 'a';
            abandoned[3] = 'n';
            abandoned[4] = 'd';
            abandoned[5] = 'o';
            abandoned[6] = 'n';
            abandoned[7] = 'e';
            abandoned[8] = 'd';
            return abandoned;
        }else if(status.Status() == LD::VW::Timeout)
        {
            LD::ImmutableString<15> timeout;
            timeout[0] = 'T';
            timeout[1] = 'i';
            timeout[2] = 'm';
            timeout[3] = 'e';
            timeout[4] = 'o';
            timeout[5] = 'u';
            timeout[6] = 't';
            return timeout;
        }else if(status.Status() == LD::VW::Login)
        {
            LD::ImmutableString<15> login;
            login[0] = 'L';
            login[1] = 'o';
            login[2] = 'g';
            login[3] = 'i';
            login[4] = 'n';
            return login;
        }else if(status.Status() == LD::VW::Logout)
        {
            LD::ImmutableString<15> logout;
            logout[0] = 'L';
            logout[1] = 'o';
            logout[2] = 'g';
            logout[3] = 'o';
            logout[4] = 'u';
            logout[5] = 't';
            return logout;
        }
        return LD::ImmutableString<15>{};
    }

    template<typename ... A>
    LD::QueryResult<LD::VW::QueueLogStatus(A...)> FromStringView(LD::Type<LD::VW::QueueLogStatus>,LD::StringView view,A && ... args) noexcept
    {
        //std::cout << "Call Status View: " << view << std::endl;
        LD::StringView answered{"ANSWERED"};
        LD::StringView abandoned{"ABANDONED"};
        LD::StringView timeout{"TIMEOUT"};
        LD::StringView login{"LOGIN"};
        LD::StringView logout{"LOGOUT"};
        if (answered == view)
        {
            return LD::MakeContext(LD::TransactionResult{},LD::VW::QueueLogStatus{LD::VW::Answered},LD::Forward<A>(args)...);
        }else if(abandoned == view)
        {
            return LD::MakeContext(LD::TransactionResult{},LD::VW::QueueLogStatus{LD::VW::Abandoned},LD::Forward<A>(args)...);
        }
        else if(timeout == view)
        {
            return LD::MakeContext(LD::TransactionResult{},LD::VW::QueueLogStatus{LD::VW::Timeout},LD::Forward<A>(args)...);
        }else if(login == view)
        {
            return LD::MakeContext(LD::TransactionResult{},LD::VW::QueueLogStatus{LD::VW::Login},LD::Forward<A>(args)...);
        }
        else if(logout == view)
        {
            return LD::MakeContext(LD::TransactionResult{},LD::VW::QueueLogStatus{LD::VW::Logout},LD::Forward<A>(args)...);
        }
        return LD::MakeContext(LD::TransactionError{},LD::Forward<A>(args)...);
    }
}
#endif //LANDESSDEVCORE_QUEUELOGSTATUS_HPP
