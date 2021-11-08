//
// Created by phoenixflower on 11/8/21.
//

#ifndef LANDESSDEVCORE_CERTIFICATE_HPP
#define LANDESSDEVCORE_CERTIFICATE_HPP
#include "Chrono/DateTime.h"
#include "Primitives/General/Immutable.hpp"
#include "Memory/UniquePointer.h"
namespace LD
{
    class CertificateSubject
    {
    private:
        LD::ImmutableString<60> mCommonName;
        LD::ImmutableString<60> mOrganizationUnit;
        LD::ImmutableString<60> mOrganization;
        LD::ImmutableString<60> mLocality;
        LD::ImmutableString<60> mStateOfProvince;
        LD::ImmutableString<60> mCountryName;
    public:
    };

    class Certificate
    {
    private:
        CertificateSubject mSubject;
    public:
        Certificate(CertificateSubject subject = CertificateSubject{}) noexcept;
        LD::StringView HostName() const noexcept;
        LD::StringView  OrganizationalUnit() const noexcept;
        LD::StringView  Organization() const noexcept;
        LD::StringView Locality() const noexcept;
        LD::StringView StateOfProvince() const noexcept;
        LD::StringView CountryName() const noexcept;
        LD::DateTime StartDate() const noexcept;
        LD::DateTime EndDate() const noexcept;
    };


    class CertificateParser
    {
    private:
    public:
        virtual CertificateSubject Parse(LD::StringView view) noexcept = 0;
    };



    extern CertificateParser & DefaultParser() noexcept;
}
#endif //LANDESSDEVCORE_CERTIFICATE_HPP
