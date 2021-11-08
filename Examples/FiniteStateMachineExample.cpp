//
// Created by phoenixflower on 11/5/21.
//
#include <iostream>
#include "Primitives/General/mapboxvariant.hpp"
#include "Memory/Optional.h"
#include "Patterns/FiniteStateMachine.hpp"
#include "Core/Unit.hpp"
#include "Definitions/TimeExtension.hpp"
#include "Patterns/DependencyInjection.hpp"
#include <filesystem>
#include <openssl/asn1.h>
#include <openssl/bio.h>
#include <openssl/conf.h>
#include <openssl/err.h>
#include <openssl/pem.h>
#include <openssl/x509.h>
namespace LD
{
    namespace Example
    {
        class ApplicationInitializedState;
        class ApplicationHasStartedState;
        class ApplicationFrameHasStartedState;
        class ApplicationExecutionState;
        class ApplicationSleepingState;
        class ApplicationFrameHasEndedState;
        class ApplicationQuittingState;

        using ApplicationState = LD::Variant<
                ApplicationInitializedState,
                ApplicationHasStartedState,
                ApplicationFrameHasStartedState,
                ApplicationExecutionState,
                ApplicationSleepingState,
                ApplicationFrameHasEndedState,
                ApplicationQuittingState>;

        class ApplicationStartedEvent{};
        class ApplicationFrameStartedEvent{};
        class ApplicationExecutionEvent{};
        class ApplicationFrameEndedEvent{};
        class ApplicationSleepingEvent{};
        class ApplicationQuittingEvent{};

        using ApplicationEvent = LD::Variant<
                ApplicationStartedEvent,
                ApplicationFrameStartedEvent,
                ApplicationExecutionEvent,
                ApplicationSleepingEvent,
                ApplicationFrameEndedEvent,
                ApplicationQuittingEvent>;


        class ApplicationInitializedState
        {
        public:
            template<typename Event>
            LD::Optional<ApplicationState> operator()(Event && event)  noexcept;
        };


        class ApplicationHasStartedState
        {
        public:
            template<typename Event>
            LD::Optional<ApplicationState> operator()(Event && event)  noexcept;
        };

        class ApplicationFrameHasStartedState
        {
        public:
            template<typename Event>
            LD::Optional<ApplicationState> operator()(Event && event)  noexcept;

        };

        class ApplicationExecutionState
        {
        public:
            template<typename Event>
            LD::Optional<ApplicationState> operator()(Event && event)  noexcept;
        };

        class ApplicationSleepingState
        {
        public:
            template<typename Event>
            LD::Optional<ApplicationState> operator()(Event && event)  noexcept;
        };

        class ApplicationFrameHasEndedState
        {
        public:
            template<typename Event>
            LD::Optional<ApplicationState> operator()(Event  event)  noexcept;

            LD::Optional<ApplicationState> operator()( ApplicationQuittingEvent)  noexcept;
        };

        class ApplicationQuittingState
        {
        public:
            template<typename Event>
            LD::Optional<ApplicationState> operator()(Event && event)  noexcept;
        };


        template<typename Event>
        LD::Optional<ApplicationState> ApplicationInitializedState::operator()(Event &&event)  noexcept
        {
            //printf("Application Has Initialized State\n");
            return ApplicationState{ApplicationHasStartedState{}};
        }
        template<typename Event>
        LD::Optional<ApplicationState> ApplicationHasStartedState::operator()(Event &&event)  noexcept
        {
            //printf("Application Has Started State\n");
            return ApplicationState{ApplicationFrameHasStartedState{}};
        }


        template<typename Event>
        LD::Optional<ApplicationState> ApplicationFrameHasStartedState::operator()(Event &&event)  noexcept
        {
            if constexpr(LD::CT::IsSame(LD::Type<Event>{},LD::Type<ApplicationQuittingEvent>{}))
            {
                return ApplicationState{ApplicationQuittingState{}};
            }
            //("Application Frame Has Started State\n");
            return ApplicationState{ApplicationExecutionState{}};
        }


        template<typename Event>
        LD::Optional<ApplicationState> ApplicationExecutionState::operator()(Event &&event)  noexcept
        {
            //printf("Application Execution State\n");
            return ApplicationState{ApplicationSleepingState{}};;
        }

        template<typename Event>
        LD::Optional<ApplicationState> ApplicationSleepingState::operator()(Event &&event)  noexcept
        {
            //printf("Application Sleeping State\n");
            return ApplicationState{ApplicationFrameHasEndedState{}};
        }

        template<typename Event>
        LD::Optional<ApplicationState> ApplicationFrameHasEndedState::operator()(Event  event)  noexcept
        {
            return ApplicationState{ApplicationFrameHasStartedState{}};
        }

        LD::Optional<ApplicationState> ApplicationFrameHasEndedState::operator()(ApplicationQuittingEvent)  noexcept
        {
            //printf("Application is Quitting\n");
            return ApplicationState{ApplicationQuittingState{}};
        }

        template<typename Event>
        LD::Optional<ApplicationState> ApplicationQuittingState::operator()(Event &&event)  noexcept
        {
            return LD::nullopt;
        }



        class StateTransitionExecutor
        {
        private:
        public:

            template<typename U, typename V>
            void operator()(U &&, V && ) noexcept
            {
            }
            void operator()(ApplicationInitializedState,ApplicationHasStartedState)
            {
                //printf("Transitioned from Initialized to Application Has Started\n");
            }

            void operator()(ApplicationHasStartedState,ApplicationFrameHasStartedState)
            {
                //printf("Transitioned from Application Started to Application Frame Has Started\n");
            }

            void operator()(ApplicationFrameHasStartedState,ApplicationExecutionState)
            {
                //printf("Transitioned from Application Frame Started to Application Execution State\n");
            }

            void operator()(ApplicationExecutionState,ApplicationSleepingState)
            {
                //printf("Transitioned from Application Execution State to Application Sleeping State\n");
            }

            void operator()(ApplicationSleepingState,ApplicationFrameHasEndedState)
            {
                //printf("Transitioned from Application Sleeping State to Application FrameHasEnded State\n");
            }
            void operator()(ApplicationFrameHasEndedState,ApplicationFrameHasStartedState)
            {
                //printf("Transitioned from Application FrameHasEnded State to Application FrameHasStarted State\n");
            }

            void operator()(ApplicationFrameHasEndedState,ApplicationQuittingState)
            {
                //printf("Transitioned from Application FrameHasEnded State to Application Quitting State\n");
            }

            void operator()(ApplicationHasStartedState,ApplicationQuittingState)
            {

            }
            void operator()(ApplicationInitializedState,ApplicationQuittingState)
            {

            }

        };

        class StateExecutorBase
        {
        private:

        public:
            virtual void operator()(ApplicationInitializedState) noexcept = 0;
            virtual void operator()(ApplicationHasStartedState) noexcept= 0;
            virtual void operator()(ApplicationFrameHasStartedState) noexcept= 0;
            virtual void operator()(ApplicationExecutionState)noexcept = 0;
            virtual void operator()(ApplicationFrameHasEndedState) noexcept= 0;
            virtual LD::Second<LD::Float> operator()(ApplicationSleepingState) noexcept= 0;
            virtual void operator()(ApplicationQuittingState) noexcept= 0;
        };
        class StateExecutor
        {
        private:
        public:
            void operator()(ApplicationInitializedState)
            {

            }
            void operator()(ApplicationHasStartedState)
            {

            }

            void operator()(ApplicationFrameHasStartedState)
            {

            }

            void operator()(ApplicationExecutionState)
            {

            }

            void operator()(ApplicationFrameHasEndedState)
            {

            }

            void operator()(ApplicationSleepingState)
            {
                LD::SleepFor(1_s);
            }


            void operator()(ApplicationQuittingState)
            {

            }
        };


        template<typename Executor>
        class BasicStateExecutor
        {
        private:
            Executor mExecutor;
        public:
            BasicStateExecutor(Executor && executor) noexcept:mExecutor{LD::Forward<Executor>(executor)}
            {

            }
            void operator()(ApplicationInitializedState state)
            {
                this->mExecutor(state);
            }
            void operator()(ApplicationHasStartedState state)
            {
                this->mExecutor(state);
            }

            void operator()(ApplicationFrameHasStartedState state)
            {
                this->mExecutor(state);
            }

            void operator()(ApplicationExecutionState state)
            {
                this->mExecutor(state);
            }

            void operator()(ApplicationFrameHasEndedState state)
            {
                this->mExecutor(state);
            }

            void operator()(ApplicationSleepingState state)
            {
                LD::SleepFor(this->mExecutor(state));
            }


            void operator()(ApplicationQuittingState state)
            {
                this->mExecutor(state);
            }
        };

        class PolymorhpicStateExecutor: public LD::DependencyInjection<PolymorhpicStateExecutor(StateExecutorBase&)>
        {
        private:
            StateExecutorBase & mExecutor;
        public:
            PolymorhpicStateExecutor(StateExecutorBase & polymorphicExecutor) noexcept:mExecutor{polymorphicExecutor}
            {

            }
            void operator()(ApplicationInitializedState state)
            {
                this->mExecutor(state);
            }
            void operator()(ApplicationHasStartedState state)
            {
                this->mExecutor(state);
            }

            void operator()(ApplicationFrameHasStartedState state)
            {
                this->mExecutor(state);
            }

            void operator()(ApplicationExecutionState state)
            {
                this->mExecutor(state);
            }

            void operator()(ApplicationFrameHasEndedState state)
            {
                this->mExecutor(state);
            }

            void operator()(ApplicationSleepingState state)
            {
                LD::SleepFor(this->mExecutor(state));
            }


            void operator()(ApplicationQuittingState state)
            {
                this->mExecutor(state);
            }
        };


        class EventGeneratorBase
        {
        private:
        public:
            virtual auto operator()(ApplicationInitializedState) noexcept -> LD::Optional<ApplicationEvent> = 0;

            virtual auto  operator()(ApplicationHasStartedState) noexcept -> LD::Optional<ApplicationEvent> = 0;

            virtual auto  operator()(ApplicationFrameHasStartedState) noexcept -> LD::Optional<ApplicationEvent> = 0;

            virtual auto  operator()(ApplicationExecutionState) noexcept -> LD::Optional<ApplicationEvent> = 0;

            virtual auto  operator()(ApplicationSleepingState) noexcept -> LD::Optional<ApplicationEvent> = 0;

            virtual auto  operator()(ApplicationFrameHasEndedState) noexcept -> LD::Optional<ApplicationEvent> = 0;

            virtual auto  operator()(ApplicationQuittingState) noexcept -> LD::Optional<ApplicationEvent> = 0;
        };
        class EventExecutor
        {
        private:
            LD::UInteger mCycleCount;
        public:
            EventExecutor() noexcept:mCycleCount{0}
            {

            }
            auto operator()(ApplicationInitializedState) noexcept -> LD::Optional<ApplicationEvent>
            {
                //todo add entry point to determine if the application should quit at this point
                return ApplicationEvent{ApplicationStartedEvent{}};
            }
            auto  operator()(ApplicationHasStartedState) noexcept -> LD::Optional<ApplicationEvent>
            {
                //todo add entry point to determine if the application should quit at this point
                return ApplicationEvent{ApplicationFrameStartedEvent{}};
            }

            auto  operator()(ApplicationFrameHasStartedState) noexcept -> LD::Optional<ApplicationEvent>
            {
                //todo add entry point to determine if the application should quit at this point
                return ApplicationEvent{ApplicationExecutionEvent{}};
            }

            auto  operator()(ApplicationExecutionState) noexcept -> LD::Optional<ApplicationEvent>
            {
                //todo add entry point to determine if the application should quit at this point
                return ApplicationEvent{ApplicationSleepingEvent{}};
            }

            auto  operator()(ApplicationSleepingState) noexcept -> LD::Optional<ApplicationEvent>
            {
                return ApplicationEvent{ApplicationFrameEndedEvent{}};
            }

            auto  operator()(ApplicationFrameHasEndedState) noexcept -> LD::Optional<ApplicationEvent>
            {
                if (this->mCycleCount == 5)
                {
                    return ApplicationEvent{ApplicationQuittingEvent{}};
                }
                this->mCycleCount++;
                return ApplicationEvent{ApplicationFrameStartedEvent{}};
            }
            auto  operator()(ApplicationQuittingState) noexcept -> LD::Optional<ApplicationEvent>
            {
                return LD::nullopt;
            }
        };

        class PolymorhpicEventExecutor: public LD::DependencyInjection<PolymorhpicStateExecutor(EventGeneratorBase&)>
        {
        private:
            EventGeneratorBase & mExecutor;
        public:
            PolymorhpicEventExecutor(EventGeneratorBase & executor):mExecutor{executor}
            {

            }

            auto operator()(ApplicationInitializedState state) noexcept -> LD::Optional<ApplicationEvent>
            {
                return this->mExecutor(state);
            }
            auto  operator()(ApplicationHasStartedState state) noexcept -> LD::Optional<ApplicationEvent>
            {
                return this->mExecutor(state);
            }

            auto  operator()(ApplicationFrameHasStartedState state) noexcept -> LD::Optional<ApplicationEvent>
            {
                return this->mExecutor(state);
            }

            auto  operator()(ApplicationExecutionState state) noexcept -> LD::Optional<ApplicationEvent>
            {
                return this->mExecutor(state);
            }

            auto  operator()(ApplicationSleepingState state) noexcept -> LD::Optional<ApplicationEvent>
            {
                return this->mExecutor(state);
            }

            auto  operator()(ApplicationFrameHasEndedState state) noexcept -> LD::Optional<ApplicationEvent>
            {
                return this->mExecutor(state);
            }
            auto  operator()(ApplicationQuittingState state) noexcept -> LD::Optional<ApplicationEvent>
            {
                return this->mExecutor(state);
            }
        };

        template<typename Executor>
        class BasicEventExecutor
        {
        private:
            Executor mExecutor;
        public:
            BasicEventExecutor(Executor && executor) noexcept:mExecutor{LD::Forward<Executor>(executor)}
            {

            }
            auto operator()(ApplicationInitializedState state) noexcept -> LD::Optional<ApplicationEvent>
            {
                return this->mExecutor(state);
            }
            auto  operator()(ApplicationHasStartedState state) noexcept -> LD::Optional<ApplicationEvent>
            {
                return this->mExecutor(state);
            }

            auto  operator()(ApplicationFrameHasStartedState state) noexcept -> LD::Optional<ApplicationEvent>
            {
                return this->mExecutor(state);
            }

            auto  operator()(ApplicationExecutionState state) noexcept -> LD::Optional<ApplicationEvent>
            {
                return this->mExecutor(state);
            }

            auto  operator()(ApplicationSleepingState state) noexcept -> LD::Optional<ApplicationEvent>
            {
                return this->mExecutor(state);
            }

            auto  operator()(ApplicationFrameHasEndedState state) noexcept -> LD::Optional<ApplicationEvent>
            {
                return this->mExecutor(state);
            }
            auto  operator()(ApplicationQuittingState state) noexcept -> LD::Optional<ApplicationEvent>
            {
                return this->mExecutor(state);
            }
        };


        enum class FileStatus {created, modified, erased};
        template<typename Executor>
        class FileWatcher: public StateExecutorBase
        {
        private:
            Executor mExecutor;
            std::unordered_map<std::string, std::filesystem::file_time_type> paths_;
            std::string path_to_watch;
        public:


            FileWatcher(std::string path_to_watch, Executor && exeuctor) : path_to_watch{path_to_watch},mExecutor{exeuctor}
            {
                for(auto &file : std::filesystem::recursive_directory_iterator(path_to_watch))
                {
                    paths_[file.path().string()] = std::filesystem::last_write_time(file);

                }
            }
            virtual void operator()(ApplicationInitializedState) noexcept
            {

            }
            virtual void operator()(ApplicationHasStartedState) noexcept
            {

            }
            virtual void operator()(ApplicationFrameHasStartedState) noexcept
            {

            }
            virtual void operator()(ApplicationExecutionState)noexcept
            {
                printf("being called\n");
                auto it = paths_.begin();
                while (it != paths_.end())
                {
                    if (!std::filesystem::exists(it->first))
                    {
                        mExecutor(LD::StringView{it->first}, FileStatus::erased);
                        it = paths_.erase(it);
                    }
                    else
                    {
                        it++;
                    }

                    for(auto &file : std::filesystem::recursive_directory_iterator(path_to_watch))
                    {
                        auto current_file_last_write_time = std::filesystem::last_write_time(file);
                        if(!contains(file.path().string()))
                        {
                            paths_[file.path().string()] = current_file_last_write_time;
                            mExecutor(LD::StringView{file.path().string()}, FileStatus::created);
                        }else
                        {
                            if(paths_[file.path().string()] != current_file_last_write_time)
                            {
                                paths_[file.path().string()] = current_file_last_write_time;
                                mExecutor(LD::StringView{file.path().string()}, FileStatus::modified);
                            }
                        }
                    }

                }
            }
            virtual void operator()(ApplicationFrameHasEndedState) noexcept
            {

            }
            virtual LD::Second<LD::Float> operator()(ApplicationSleepingState) noexcept
            {
                return 1_s;
            }
            virtual void operator()(ApplicationQuittingState) noexcept
            {

            }

            bool contains(const std::string &key)
            {
                auto el = paths_.find(key);

                return el != paths_.end();
            }

         };


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

        extern void FiniteStateMachineExample()
        {
            OpenSSL_add_all_algorithms();
            ERR_load_crypto_strings();
            OPENSSL_no_config();



            //
            // Read the certificate contents from disk
            //
            std::string cert_content = R"(
-----BEGIN CERTIFICATE-----
MIIFNzCCBB+gAwIBAgISBNhgAv18592dxupIrRpaZlGJMA0GCSqGSIb3DQEBCwUA
MDIxCzAJBgNVBAYTAlVTMRYwFAYDVQQKEw1MZXQncyBFbmNyeXB0MQswCQYDVQQD
EwJSMzAeFw0yMTA5MDcwMjA5NTVaFw0yMTEyMDYwMjA5NTRaMBoxGDAWBgNVBAMT
D2xhbmRlc3Nob21lLmNvbTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEB
AONLcxpjmbm3tb+v/nQ5jrz5asZs+hKjTAwkaXvaAZBw3FvEQI2Lx9Ino4H/CVmq
SVU7HUaz4MlR5sXpvaCR2HiPoCbL6FJJG8AsfQJtg2iHSc9q/nGGD7xEJaG7dTk9
JnA2rV0W8PftTqeFyfctH23J0eA0zXlJxpA04Y5n79Xq5zR6vNlLp9JfLS/q4vZs
niOHD/j/Xbg/7a0sb5aoIEbD1FmgvMtB/VHJxpSMctWT3tLD/7Z4UU/38lFKIOUX
c9134keADHwPV8UYqW+8r0udgR/UBUb8s1bnbLTedymGmwpiuQ3VGr7A60q+iBKi
sGjW6toUkkdAEb76EXkNeSkCAwEAAaOCAl0wggJZMA4GA1UdDwEB/wQEAwIFoDAd
BgNVHSUEFjAUBggrBgEFBQcDAQYIKwYBBQUHAwIwDAYDVR0TAQH/BAIwADAdBgNV
HQ4EFgQUTm0NGIZBCgQBkaPqxrlstKNTs3AwHwYDVR0jBBgwFoAUFC6zF7dYVsuu
UAlA5h+vnYsUwsYwVQYIKwYBBQUHAQEESTBHMCEGCCsGAQUFBzABhhVodHRwOi8v
cjMuby5sZW5jci5vcmcwIgYIKwYBBQUHMAKGFmh0dHA6Ly9yMy5pLmxlbmNyLm9y
Zy8wLQYDVR0RBCYwJIIRKi5sYW5kZXNzaG9tZS5jb22CD2xhbmRlc3Nob21lLmNv
bTBMBgNVHSAERTBDMAgGBmeBDAECATA3BgsrBgEEAYLfEwEBATAoMCYGCCsGAQUF
BwIBFhpodHRwOi8vY3BzLmxldHNlbmNyeXB0Lm9yZzCCAQQGCisGAQQB1nkCBAIE
gfUEgfIA8AB2AJQgvB6O1Y1siHMfgosiLA3R2k1ebE+UPWHbTi9YTaLCAAABe747
zKEAAAQDAEcwRQIgcaQwBAxqq+rz0PgXR4mtz+NIY9PRYynWbUavY3pknkMCIQDW
PxGBGwx+D5luBXGPgND4kiWmL+CPwpos4kjRM/XbEAB2AH0+8viP/4hVaCTCwMqe
Uol5K8UOeAl/LmqXaJl+IvDXAAABe747zMYAAAQDAEcwRQIhAJRCOPcwMi6xlLWf
RNzNPu3emty6nlu1R8o4VNz7m7wuAiB23C0jjoEZC5d3WtFWKIU+ZewHY1O4WVlT
Mv6Qci8z0TANBgkqhkiG9w0BAQsFAAOCAQEAdQhr2s1kRNLXUrNgPYuAox7NcL3y
Jru1/BXFPj0US43fgr6UTzdsTcDifUk//DcyCFCWsXBxOfcI2BU2vLlViFrjdjm4
55JAMdhab0caOyQOeiERmVg7vt03CxdFVJ2SwUA1nxdLXhn8FDCdsXLeKMaiIYpv
JXxW1pftPSM9+2NxY8gtVNpYpPzdkD1taka7VZfXFAXbgcjKB/BvK8kn0Q2e9fU9
y7F2Qffsxfsn5aqaxSuHGzS0d3MxuDzzs6vfpU8n1GuogkEo3nwWq0b1x+Jj9hqi
d2leO6/XGTg1LmWe9oDeq3JToS8ze/ZMVv+8bNE16/UWjMNZwy3Elt+1pw==
-----END CERTIFICATE-----

)";

            std::string cert_content1 = R"(
-----BEGIN CERTIFICATE-----
MIIFFjCCAv6gAwIBAgIRAJErCErPDBinU/bWLiWnX1owDQYJKoZIhvcNAQELBQAw
TzELMAkGA1UEBhMCVVMxKTAnBgNVBAoTIEludGVybmV0IFNlY3VyaXR5IFJlc2Vh
cmNoIEdyb3VwMRUwEwYDVQQDEwxJU1JHIFJvb3QgWDEwHhcNMjAwOTA0MDAwMDAw
WhcNMjUwOTE1MTYwMDAwWjAyMQswCQYDVQQGEwJVUzEWMBQGA1UEChMNTGV0J3Mg
RW5jcnlwdDELMAkGA1UEAxMCUjMwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEK
AoIBAQC7AhUozPaglNMPEuyNVZLD+ILxmaZ6QoinXSaqtSu5xUyxr45r+XXIo9cP
R5QUVTVXjJ6oojkZ9YI8QqlObvU7wy7bjcCwXPNZOOftz2nwWgsbvsCUJCWH+jdx
sxPnHKzhm+/b5DtFUkWWqcFTzjTIUu61ru2P3mBw4qVUq7ZtDpelQDRrK9O8Zutm
NHz6a4uPVymZ+DAXXbpyb/uBxa3Shlg9F8fnCbvxK/eG3MHacV3URuPMrSXBiLxg
Z3Vms/EY96Jc5lP/Ooi2R6X/ExjqmAl3P51T+c8B5fWmcBcUr2Ok/5mzk53cU6cG
/kiFHaFpriV1uxPMUgP17VGhi9sVAgMBAAGjggEIMIIBBDAOBgNVHQ8BAf8EBAMC
AYYwHQYDVR0lBBYwFAYIKwYBBQUHAwIGCCsGAQUFBwMBMBIGA1UdEwEB/wQIMAYB
Af8CAQAwHQYDVR0OBBYEFBQusxe3WFbLrlAJQOYfr52LFMLGMB8GA1UdIwQYMBaA
FHm0WeZ7tuXkAXOACIjIGlj26ZtuMDIGCCsGAQUFBwEBBCYwJDAiBggrBgEFBQcw
AoYWaHR0cDovL3gxLmkubGVuY3Iub3JnLzAnBgNVHR8EIDAeMBygGqAYhhZodHRw
Oi8veDEuYy5sZW5jci5vcmcvMCIGA1UdIAQbMBkwCAYGZ4EMAQIBMA0GCysGAQQB
gt8TAQEBMA0GCSqGSIb3DQEBCwUAA4ICAQCFyk5HPqP3hUSFvNVneLKYY611TR6W
PTNlclQtgaDqw+34IL9fzLdwALduO/ZelN7kIJ+m74uyA+eitRY8kc607TkC53wl
ikfmZW4/RvTZ8M6UK+5UzhK8jCdLuMGYL6KvzXGRSgi3yLgjewQtCPkIVz6D2QQz
CkcheAmCJ8MqyJu5zlzyZMjAvnnAT45tRAxekrsu94sQ4egdRCnbWSDtY7kh+BIm
lJNXoB1lBMEKIq4QDUOXoRgffuDghje1WrG9ML+Hbisq/yFOGwXD9RiX8F6sw6W4
avAuvDszue5L3sz85K+EC4Y/wFVDNvZo4TYXao6Z0f+lQKc0t8DQYzk1OXVu8rp2
yJMC6alLbBfODALZvYH7n7do1AZls4I9d1P4jnkDrQoxB3UqQ9hVl3LEKQ73xF1O
yK5GhDDX8oVfGKF5u+decIsH4YaTw7mP3GFxJSqv3+0lUFJoi5Lc5da149p90Ids
hCExroL1+7mryIkXPeFM5TgO9r0rvZaBFOvV2z0gp35Z0+L4WPlbuEjN/lxPFin+
HlUjr8gRsI3qfJOQFy/9rKIJR0Y/8Omwt/8oTWgy1mdeHmmjk7j1nYsvC9JSQ6Zv
MldlTTKB3zhThV1+XWYp6rjd5JW1zbVWEkLNxE7GJThEUG3szgBVGP7pSWTUTsqX
nLRbwHOoq7hHwg==
-----END CERTIFICATE-----)";

            std::string cert_content2 = R"(
-----BEGIN CERTIFICATE-----
MIIFYDCCBEigAwIBAgIQQAF3ITfU6UK47naqPGQKtzANBgkqhkiG9w0BAQsFADA/
MSQwIgYDVQQKExtEaWdpdGFsIFNpZ25hdHVyZSBUcnVzdCBDby4xFzAVBgNVBAMT
DkRTVCBSb290IENBIFgzMB4XDTIxMDEyMDE5MTQwM1oXDTI0MDkzMDE4MTQwM1ow
TzELMAkGA1UEBhMCVVMxKTAnBgNVBAoTIEludGVybmV0IFNlY3VyaXR5IFJlc2Vh
cmNoIEdyb3VwMRUwEwYDVQQDEwxJU1JHIFJvb3QgWDEwggIiMA0GCSqGSIb3DQEB
AQUAA4ICDwAwggIKAoICAQCt6CRz9BQ385ueK1coHIe+3LffOJCMbjzmV6B493XC
ov71am72AE8o295ohmxEk7axY/0UEmu/H9LqMZshftEzPLpI9d1537O4/xLxIZpL
wYqGcWlKZmZsj348cL+tKSIG8+TA5oCu4kuPt5l+lAOf00eXfJlII1PoOK5PCm+D
LtFJV4yAdLbaL9A4jXsDcCEbdfIwPPqPrt3aY6vrFk/CjhFLfs8L6P+1dy70sntK
4EwSJQxwjQMpoOFTJOwT2e4ZvxCzSow/iaNhUd6shweU9GNx7C7ib1uYgeGJXDR5
bHbvO5BieebbpJovJsXQEOEO3tkQjhb7t/eo98flAgeYjzYIlefiN5YNNnWe+w5y
sR2bvAP5SQXYgd0FtCrWQemsAXaVCg/Y39W9Eh81LygXbNKYwagJZHduRze6zqxZ
Xmidf3LWicUGQSk+WT7dJvUkyRGnWqNMQB9GoZm1pzpRboY7nn1ypxIFeFntPlF4
FQsDj43QLwWyPntKHEtzBRL8xurgUBN8Q5N0s8p0544fAQjQMNRbcTa0B7rBMDBc
SLeCO5imfWCKoqMpgsy6vYMEG6KDA0Gh1gXxG8K28Kh8hjtGqEgqiNx2mna/H2ql
PRmP6zjzZN7IKw0KKP/32+IVQtQi0Cdd4Xn+GOdwiK1O5tmLOsbdJ1Fu/7xk9TND
TwIDAQABo4IBRjCCAUIwDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8EBAMCAQYw
SwYIKwYBBQUHAQEEPzA9MDsGCCsGAQUFBzAChi9odHRwOi8vYXBwcy5pZGVudHJ1
c3QuY29tL3Jvb3RzL2RzdHJvb3RjYXgzLnA3YzAfBgNVHSMEGDAWgBTEp7Gkeyxx
+tvhS5B1/8QVYIWJEDBUBgNVHSAETTBLMAgGBmeBDAECATA/BgsrBgEEAYLfEwEB
ATAwMC4GCCsGAQUFBwIBFiJodHRwOi8vY3BzLnJvb3QteDEubGV0c2VuY3J5cHQu
b3JnMDwGA1UdHwQ1MDMwMaAvoC2GK2h0dHA6Ly9jcmwuaWRlbnRydXN0LmNvbS9E
U1RST09UQ0FYM0NSTC5jcmwwHQYDVR0OBBYEFHm0WeZ7tuXkAXOACIjIGlj26Ztu
MA0GCSqGSIb3DQEBCwUAA4IBAQAKcwBslm7/DlLQrt2M51oGrS+o44+/yQoDFVDC
5WxCu2+b9LRPwkSICHXM6webFGJueN7sJ7o5XPWioW5WlHAQU7G75K/QosMrAdSW
9MUgNTP52GE24HGNtLi1qoJFlcDyqSMo59ahy2cI2qBDLKobkx/J3vWraV0T9VuG
WCLKTVXkcGdtwlfFRjlBz4pYg1htmf5X6DYO8A4jqv2Il9DjXA6USbW1FzXSLr9O
he8Y4IWS6wY7bCkjCWDcRQJMEhg76fsO3txE+FiYruq9RUWhiF1myv4Q6W+CyBFC
Dfvp7OOGAN6dEOM4+qR9sdjoSYKEBpsr6GtPAQw4dy753ec5
-----END CERTIFICATE-----)";
            // Put the certificate contents into an openssl IO stream (BIO)
            BIO_ptr input(BIO_new(BIO_s_mem()), BIO_free);
            BIO_write(input.get(), cert_content.c_str(), cert_content.size());

            // Create an openssl certificate from the BIO
            X509_ptr cert(PEM_read_bio_X509_AUX(input.get(), NULL, NULL, NULL), X509_free);

            // Create a BIO to hold info from the cert
            BIO_ptr output_bio(BIO_new(BIO_s_mem()), BIO_free);

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
*/

            //
            // Get the subject of the certificate.
            //

            // According to the openssl documentation:
            // The returned value is an internal pointer which MUST NOT be freed
            X509_NAME *subject = X509_get_subject_name(cert.get());

            // Print the subject into a BIO and then get a string
            X509_NAME_print_ex(output_bio.get(), subject, 0, 0);
            std::string cert_subject = bio_to_string(output_bio, 4096);

            // Max subject length should be 555 bytes with ascii characters or 3690
            // bytes with unicode, based on the max allowed lengths for each component
            // of the subject plus the formatting.
            // Country code - 2 bytes
            // State/locality name - 128 bytes
            // Organization name - 64 bytes
            // Organizational unit name - 64 bytes
            // Common name - 64 bytes
            // email address - 64 bytes

            std::cout << "Subject:" << std::endl;
            std::cout << cert_subject << std::endl;
            std::cout << std::endl;



            //
            // Get the expiration date of the certificate
            //

            // X509_get_notAfter returns the time that the cert expires, in Abstract
            // Syntax Notation
            // According to the openssl documentation:
            // The returned value is an internal pointer which MUST NOT be freed
            ASN1_TIME *expires = X509_get_notAfter(cert.get());

            ASN1_TIME *not_before = X509_get_notBefore(cert.get());
            ASN1_TIME *not_after = X509_get_notAfter(cert.get());

            char not_after_str[100];
            convert_ASN1TIME(not_after, not_after_str, 100);


            char not_before_str[100];
            convert_ASN1TIME(not_before, not_before_str, 100);

            std::cout << "Cert not valid after: " << not_after_str << "\n";
            std::cout << ASN1_TIME_print(input.get(), X509_get_notAfter(cert.get())) << "\n";
            // Construct another ASN1_TIME for the unix epoch, get the difference
            // between them and use that to calculate a unix timestamp representing
            // when the cert expires
            ASN1_TIME_ptr epoch(ASN1_TIME_new(), ASN1_STRING_free);
            ASN1_TIME_set_string(epoch.get(), "Thu, 01 Jan 1970 00:00:00 GMT");
            int days, seconds;

            std::cout << "Time: " << ASN1_TIME_print(input.get(),not_after) << "\n";
            ASN1_TIME_diff(&days, &seconds, epoch.get(), expires);
            std::cout << "Days" << days << "\n";
            std::cout << "Seconds " << seconds << "\n";
            std::cout << (days *24*60*60) + seconds << "\n";
            time_t expire_timestamp = (days * 24 * 60 * 60) + seconds;

            std::cout << "Expiration timestamp:" << std::endl;
            std::cout << expire_timestamp << std::endl;
            std::cout << std::endl;


            FIPS_mode_set(0);
            CONF_modules_unload(1);
            CONF_modules_free();
            ERR_free_strings();
            EVP_cleanup();
            CRYPTO_cleanup_all_ex_data();
            ERR_remove_state(0);
            auto fileWatcherFunctor = [](auto filePath,auto) noexcept {std::cout << filePath << "\n";};
            FileWatcher<decltype(fileWatcherFunctor)&> watcher("/home/phoenixflower/Desktop/",fileWatcherFunctor);

            PolymorhpicStateExecutor polymorhpicStateExecutor{watcher};
            StateTransitionExecutor stateTransitionExecutor;

            StateExecutor stateExecutor;
            auto stateManagerExecutor = LD::Overload{stateTransitionExecutor,polymorhpicStateExecutor};
            LD::StateManager<decltype(stateManagerExecutor)&,ApplicationState(ApplicationStartedEvent)> stateManager{stateManagerExecutor};

            EventExecutor eventGenerator;
            LD::EventDispatcher<LD::StateManager<decltype(stateManagerExecutor)&,ApplicationState(ApplicationStartedEvent)>&, EventExecutor&(ApplicationEvent)> eventDispatcher
            {
                stateManager,
                eventGenerator
            };

            auto runningPredicate = [](auto && state) noexcept
            {
                return true;
            };
            auto quittingPredicate = [](ApplicationQuittingState) noexcept
            {
                return false;
            };
            //we want to quit when the quitting state has been achieved
            while (LD::Visit(LD::Overload{runningPredicate,quittingPredicate},stateManager.State()))
            {
                eventDispatcher();
            }
        }
    }
}
