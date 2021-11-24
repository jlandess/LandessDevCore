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
/**
 *      //OpenSSL_add_all_algorithms();
            //ERR_load_crypto_strings();
            //OPENSSL_no_config();



            //
            // Read the certificate contents from disk
            //
            //std::string cert_content = R"()";
            // Put the certificate contents into an openssl IO stream (BIO)
            //BIO_ptr input(BIO_new(BIO_s_mem()), BIO_free);
            //BIO_write(input.get(), cert_content.c_str(), cert_content.size());

            // Create an openssl certificate from the BIO
            //X509_ptr cert(PEM_read_bio_X509_AUX(input.get(), NULL, NULL, NULL), X509_free);

            // Create a BIO to hold info from the cert
            //BIO_ptr output_bio(BIO_new(BIO_s_mem()), BIO_free);

/*
    //
    // Get the full human readable representation of the certificate
    //
    X509_print_ex(output_bio.get(), cert.get(), 0, 0);
    // Put the contents of the BIO into a C++ string
    std::string cert_details = bio_to_string(output_bio, 32768);
    BIO_reset(output_bio.get());
    std::cout << "========================================" << std::endl;
    std::cout << "  Certificate details:" << std::endl;
    std::cout << cert_details << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << std::endl;

       // Smart pointers to wrap openssl C types that need explicit free
        using BIO_ptr = std::unique_ptr<BIO, decltype(&BIO_free)>;
        using X509_ptr = std::unique_ptr<X509, decltype(&X509_free)>;
        using ASN1_TIME_ptr = std::unique_ptr<ASN1_TIME, decltype(&ASN1_STRING_free)>;


// Convert the contents of an openssl BIO to a std::string
        std::string bio_to_string(const BIO_ptr& bio, const int& max_len)
        {
            // We are careful to do operations based on explicit lengths, not depending
            // on null terminated character streams except where we ensure the terminator

            // Create a buffer and zero it out
            char buffer[max_len];
            memset(buffer, 0, max_len);
            // Read one smaller than the buffer to make sure we end up with a null
            // terminator no matter what
            BIO_read(bio.get(), buffer, max_len - 1);
            return std::string(buffer);
        }



        int convert_ASN1TIME(ASN1_TIME *t, char* buf, size_t len)
        {
            int rc;
            BIO *b = BIO_new(BIO_s_mem());
            rc = ASN1_TIME_print(b, t);

            if (rc <= 0) {
                //log_error("fetchdaemon", "ASN1_TIME_print failed or wrote no data.\n");
                BIO_free(b);
                return EXIT_FAILURE;
            }
            rc = BIO_gets(b, buf, len);

            if (rc <= 0) {
                //log_error("fetchdaemon", "BIO_gets call failed to transfer contents to buf");
                BIO_free(b);
                return EXIT_FAILURE;
            }
            BIO_free(b);
            return EXIT_SUCCESS;
        }
*/

//
// Get the subject of the certificate.
//

// According to the openssl documentation:
// The returned value is an internal pointer which MUST NOT be freed
//X509_NAME *subject = X509_get_subject_name(cert.get());

// Print the subject into a BIO and then get a string
//X509_NAME_print_ex(output_bio.get(), subject, 0, 0);
//std::string cert_subject = bio_to_string(output_bio, 4096);

// Max subject length should be 555 bytes with ascii characters or 3690
// bytes with unicode, based on the max allowed lengths for each component
// of the subject plus the formatting.
// Country code - 2 bytes
// State/locality name - 128 bytes
// Organization name - 64 bytes
// Organizational unit name - 64 bytes
// Common name - 64 bytes
// email address - 64 bytes

//std::cout << "Subject:" << std::endl;
//std::cout << cert_subject << std::endl;
//std::cout << std::endl;



//
// Get the expiration date of the certificate
//

// X509_get_notAfter returns the time that the cert expires, in Abstract
// Syntax Notation
// According to the openssl documentation:
// The returned value is an internal pointer which MUST NOT be freed
//ASN1_TIME *expires = X509_get_notAfter(cert.get());

//ASN1_TIME *not_before = X509_get_notBefore(cert.get());
//ASN1_TIME *not_after = X509_get_notAfter(cert.get());

//char not_after_str[100];
//convert_ASN1TIME(not_after, not_after_str, 100);


//char not_before_str[100];
//convert_ASN1TIME(not_before, not_before_str, 100);

//std::cout << "Cert not valid after: " << not_after_str << "\n";
//std::cout << ASN1_TIME_print(input.get(), X509_get_notAfter(cert.get())) << "\n";
// Construct another ASN1_TIME for the unix epoch, get the difference
// between them and use that to calculate a unix timestamp representing
// when the cert expires
//ASN1_TIME_ptr epoch(ASN1_TIME_new(), ASN1_STRING_free);
//ASN1_TIME_set_string(epoch.get(), "Thu, 01 Jan 1970 00:00:00 GMT");
//int days, seconds;

//std::cout << "Time: " << ASN1_TIME_print(input.get(),not_after) << "\n";
//ASN1_TIME_diff(&days, &seconds, epoch.get(), expires);
//std::cout << "Days" << days << "\n";
//std::cout << "Seconds " << seconds << "\n";
//std::cout << (days *24*60*60) + seconds << "\n";
//time_t expire_timestamp = (days * 24 * 60 * 60) + seconds;

//std::cout << "Expiration timestamp:" << std::endl;
//std::cout << expire_timestamp << std::endl;
//std::cout << std::endl;


//FIPS_mode_set(0);
//CONF_modules_unload(1);
//CONF_modules_free();
//ERR_free_strings();
//EVP_cleanup();
//CRYPTO_cleanup_all_ex_data();
//ERR_remove_state(0);

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