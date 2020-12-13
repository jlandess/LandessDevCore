//
// Created by phoenixflower on 11/20/20.
//

#ifndef LANDESSDEVCORE_QUEUELOG_HPP
#define LANDESSDEVCORE_QUEUELOG_HPP
#include "VirtuWorks/DateTime.hpp"
#include "Language.hpp"
#include "Primitives/General/Immutable.hpp"
#include "QueueLogStatus.hpp"
#include "Identification.hpp"

#include "Reflection/Reflection.hpp"
#include "Primitives/General/ctre.hpp"
#include "Primitives/General/PhoneNumber.hpp"

namespace LD
{
    namespace VW
    {
        class QueueLog
        {
        private:
            //LD::Tuple<LD::VW::Date,LD::Time> mDateTime;
            LD::VW::DateTime mDateTime;
            LD::VW::Language mLanguage;
            //LD::ImmutableString<14> mPhoneNumber;
            LD::PhoneNumber mPhoneNumber;
            LD::VW::QueueLogStatus mStatus;
            LD::UInteger mExtension;
            LD::UInteger mHoldtime;
            LD::UInteger mCallTime;
            LD::UInteger mOriginalPosition;
            LD::VW::CallIdentification mID;
        public:

            const LD::VW::DateTime & DateTime() const noexcept
            {
                return this->mDateTime;
            }

            LD::VW::DateTime & DateTime() noexcept
            {
                return this->mDateTime;
            }

            const LD::VW::Language & Language() const noexcept
            {
                return this->mLanguage;
            }

            LD::VW::Language & Language() noexcept
            {
                return this->mLanguage;
            }

            const LD::PhoneNumber & PhoneNumber() const noexcept
            {
                return this->mPhoneNumber;
            }
            LD::PhoneNumber & PhoneNumber() noexcept
            {
                return this->mPhoneNumber;
            }

            const LD::VW::QueueLogStatus & Status() const noexcept
            {
                return this->mStatus;
            }

            LD::VW::QueueLogStatus & Status() noexcept
            {
                return this->mStatus;
            }

            const LD::UInteger  & Extension() const noexcept
            {
                return this->mExtension;
            }

            LD::UInteger & Extension() noexcept
            {
                return this->mExtension;
            }
            const LD::UInteger & HoldTime() const noexcept
            {
                return this->mHoldtime;
            }
            LD::UInteger & HoldTime() noexcept
            {
                return this->mHoldtime;
            }

            const LD::UInteger & CallTime() const noexcept
            {
                return this->mCallTime;
            }

            LD::UInteger & CallTime() noexcept
            {
                return this->mCallTime;
            }

            const LD::UInteger & OriginalPosition() const noexcept
            {
                return this->mOriginalPosition;
            }

            LD::UInteger & OriginalPosition() noexcept
            {
                return this->mOriginalPosition;
            }

            const LD::VW::CallIdentification & ID() const noexcept
            {
                return this->mID;
            }
            LD::VW::CallIdentification & ID() noexcept
            {
                return this->mID;
            }


        };
    }
}
template<>
class LD::CT::TypeDescriptor<LD::VW::QueueLog>
{
private:
    static constexpr auto TimeLabel = ctll::basic_fixed_string("time");
    static constexpr auto QueueName = ctll::basic_fixed_string("qu_name");
    static constexpr auto PhoneNumberLabel = ctll::basic_fixed_string("callerid");
    static constexpr auto DispositionLabel = ctll::basic_fixed_string("disposition");
    static constexpr auto AgentLabel = ctll::basic_fixed_string("agent");
    static constexpr auto HoldTimeLabel = ctll::basic_fixed_string("holdtime");
    static constexpr auto CallTimeLabel = ctll::basic_fixed_string("calltime");
    static constexpr auto OriginalPositionLabel = ctll::basic_fixed_string("origpos");
    static constexpr auto CallerIDLabel = ctll::basic_fixed_string("callid");
public:
    static constexpr auto ClassName = ctll::basic_fixed_string("VirtuWorksCall");
    using MemberList = LD::CT::TypeList<
            LD::CT::EncapsulatedMemberDescriptor<TimeLabel,LD::CT::SelectOverload<LD::VW::DateTime & (LD::VW::QueueLog::*)(),&LD::VW::QueueLog::DateTime>(),LD::CT::SelectOverload<const LD::VW::DateTime & (LD::VW::QueueLog::*)() const,&LD::VW::QueueLog::DateTime>()>,
            LD::CT::EncapsulatedMemberDescriptor<QueueName,LD::CT::SelectOverload<LD::VW::Language & (LD::VW::QueueLog::*)(),&LD::VW::QueueLog::Language>(),LD::CT::SelectOverload<const LD::VW::Language & (LD::VW::QueueLog::*)() const,&LD::VW::QueueLog::Language>()>,
            LD::CT::EncapsulatedMemberDescriptor<PhoneNumberLabel,LD::CT::SelectOverload<LD::PhoneNumber & (LD::VW::QueueLog::*)(),&LD::VW::QueueLog::PhoneNumber>(),LD::CT::SelectOverload<const LD::PhoneNumber & (LD::VW::QueueLog::*)() const,&LD::VW::QueueLog::PhoneNumber>()>,
            LD::CT::EncapsulatedMemberDescriptor<DispositionLabel,LD::CT::SelectOverload<LD::VW::QueueLogStatus & (LD::VW::QueueLog::*)(),&LD::VW::QueueLog::Status>(),LD::CT::SelectOverload<const LD::VW::QueueLogStatus & (LD::VW::QueueLog::*)() const,&LD::VW::QueueLog::Status>()>,
            LD::CT::EncapsulatedMemberDescriptor<AgentLabel,LD::CT::SelectOverload<LD::UInteger & (LD::VW::QueueLog::*)(),&LD::VW::QueueLog::Extension>(),LD::CT::SelectOverload<const LD::UInteger & (LD::VW::QueueLog::*)() const,&LD::VW::QueueLog::Extension>()>,
            LD::CT::EncapsulatedMemberDescriptor<HoldTimeLabel,LD::CT::SelectOverload<LD::UInteger & (LD::VW::QueueLog::*)(),&LD::VW::QueueLog::HoldTime>(),LD::CT::SelectOverload<const LD::UInteger & (LD::VW::QueueLog::*)() const,&LD::VW::QueueLog::HoldTime>()>,
            LD::CT::EncapsulatedMemberDescriptor<CallTimeLabel,LD::CT::SelectOverload<LD::UInteger & (LD::VW::QueueLog::*)(),&LD::VW::QueueLog::CallTime>(),LD::CT::SelectOverload<const LD::UInteger & (LD::VW::QueueLog::*)() const,&LD::VW::QueueLog::CallTime>()>,
            LD::CT::EncapsulatedMemberDescriptor<OriginalPositionLabel,LD::CT::SelectOverload<LD::UInteger & (LD::VW::QueueLog::*)(),&LD::VW::QueueLog::OriginalPosition>(),LD::CT::SelectOverload<const LD::UInteger & (LD::VW::QueueLog::*)() const,&LD::VW::QueueLog::OriginalPosition>()>,
            LD::CT::EncapsulatedMemberDescriptor<CallerIDLabel,LD::CT::SelectOverload<LD::VW::CallIdentification & (LD::VW::QueueLog::*)(),&LD::VW::QueueLog::ID>(),LD::CT::SelectOverload<const LD::VW::CallIdentification & (LD::VW::QueueLog::*)() const,&LD::VW::QueueLog::ID>()>
    >;
    static constexpr MemberList Members{  };
};
#endif //LANDESSDEVCORE_QUEUELOG_HPP
