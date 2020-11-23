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

        };
    }

    template<typename ... A>
    LD::QueryResult<LD::VW::QueueLogStatus(A...)> FromStringView(LD::Type<LD::VW::QueueLogStatus>,LD::StringView view,A && ... args) noexcept
    {
        LD::StringView answered{"Answered"};
        LD::StringView abandoned{"Abandoned"};
        LD::StringView timeout{"Timeout"};
        LD::StringView login{"Login"};
        LD::StringView logout{"Logout"};
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
