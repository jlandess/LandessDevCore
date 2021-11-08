//
// Created by phoenixflower on 11/8/21.
//
#include <memory>
#include "Certificate.hpp"
#include <openssl/asn1.h>
#include <openssl/bio.h>
#include <openssl/conf.h>
#include <openssl/err.h>
#include <openssl/pem.h>
#include <openssl/x509.h>

using BIO_ptr = std::unique_ptr<BIO, decltype(&BIO_free)>;
using X509_ptr = std::unique_ptr<X509, decltype(&X509_free)>;
using ASN1_TIME_ptr = std::unique_ptr<ASN1_TIME, decltype(&ASN1_STRING_free)>;
namespace LD
{

    Certificate::Certificate(CertificateSubject subject) noexcept:mSubject{subject}
    {
        //this->mParserObject =  LD::MakeUnique<OpenSSLCertificateParser>(LD::StringView{});
    }

    class OpenSSLCertificateParser: public CertificateParser
    {
    public:
        virtual CertificateSubject Parse(LD::StringView view) noexcept
        {

        }
    };

    CertificateParser & DefaultParser() noexcept
    {
        static OpenSSLCertificateParser parser;
        return parser;
    }

}