//
// Created by phoenixflower on 11/27/20.
//

#ifndef LANDESSDEVCORE_RECORD_HPP
#define LANDESSDEVCORE_RECORD_HPP
#include "Primitives/General/Immutable.hpp"
#include "DateOfBirth.hpp"
#include "Age.hpp"
#include "PhoneNumber.h"
#include "Status.hpp"
#include "Address.hpp"
#include "Chrono/DateTime.h"
#include "Encounter.hpp"
#include "RecordNumber.hpp"
#include "Prefix/NamePrefix.hpp"
#include "Suffix/NameSuffix.hpp"
namespace LD
{
    namespace PF
    {
        class Record
        {
        private:
            LD::PF::RecordNumber mID;
            LD::NamePrefix mPrefix;
            LD::ImmutableString<25> mFirstName;
            LD::ImmutableString<25> mMiddleName;
            LD::ImmutableString<25> mLastName;
            LD::NameSuffix mSuffix;
            LD::PF::DateOfBirth mBirth;
            LD::PF::Age mAge;
            char mSex;
            LD::PF::PhoneNumber mPreferredContact;
            LD::PF::Status mStatus;
            LD::PF::Address mAddress1;
            LD::PF::Address mAddress2;
            LD::ImmutableString<85> mCity;
            LD::ImmutableString<2> mState;
            LD::UInteger mZipCode;
            LD::PF::PhoneNumber mHomePhone;
            LD::PF::PhoneNumber mCellPhone;
            LD::PF::PhoneNumber mOfficePhone;
            LD::UInteger mOfficePhoneExtension;
            LD::ImmutableString<25> mEmail;
            LD::PF::Encounter mRecentEncounter;
        public:

        };
    }
}
#endif //LANDESSDEVCORE_RECORD_HPP
