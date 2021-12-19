//
// Created by phoenixflower on 11/13/21.
//
#include "IO/DHTBackend.hpp"
#include "Services/Logging.hpp"
#include "Async/Channel.hpp"

#include <iostream>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "Primitives/General/CircularQueue.hpp"
#include "Patterns/Configuration.hpp"
#include "Patterns/DependencyInjection.hpp"
#include "IO/ConnectionBroker.hpp"
#include "IO/Database1.h"
#include "Examples/ReflectionDemoTypes.h"
#include "Patterns/LazyLambda.hpp"
#include "Patterns/Scheduling.hpp"
namespace LD
{

    class Dowhatiwantmofo
    {
    public:

        bool operator()(unsigned  char character) noexcept
        {
            //std::cout << "being called" << "\n";
            std::cout << character;
            return true;
        }
    };

    template<>
    class BackInserter<Dowhatiwantmofo>
    {
    private:
        Dowhatiwantmofo & mObject;
    public:
        BackInserter(Dowhatiwantmofo & object  ) noexcept:mObject{object}{}
        BackInserter<Dowhatiwantmofo> & operator = (unsigned char character)noexcept{ mObject(character);return (*this);}
    };
    namespace Example
    {
        int Send(const std::string& filename, const std::string& msg) noexcept // if you have no return value,
        // you should throw on error,
        // let's avoid that
        {
            key_t key = ftok(filename.c_str(), 65); // you should maybe consider using a named constant
            // for your project ID
            if (key == -1)
                return errno;

            int shmid = shmget(key, msg.length() + 1, 0666 | IPC_CREAT);  // allocate enough memory for the
            // message, plus its NULL terminator
            if (shmid == -1)
                return errno;

            void *shared_mem = shmat(shmid, nullptr, 0);
            if (shared_mem == (void*)-1)
            {
                //  the systeml failed to lock the allocated memory.
                //  do some cleanup by de-allocating the shared memory block.
                int ret = errno; // keep original error for return.
                shmctl(shmid , IPC_RMID, nullptr);
                return ret;
            }

            // copy message string with its NULL terminator to shared memory
            memcpy(shared_mem, msg.c_str(), msg.length() + 1); // using length() + 1 is ok here, result of
            // c_str() always has a NULL terminator.
            shmdt(shared_mem);
            return 0;
        }


        std::string Receive(const std::string & filename, int size=1024 )
        {
            key_t key = ftok(filename.c_str(),65);

            if (key == -1)
                return "";


            int shmid = shmget(key,size,0666|IPC_CREAT);

            if (shmid == -1)
                return "";
            char *str = (char*) shmat(shmid,(void*)0,0);

            if (str == (void*)-1)
            {
                //  the systeml failed to lock the allocated memory.
                //  do some cleanup by de-allocating the shared memory block.
                int ret = errno; // keep original error for return.
                shmctl(shmid , IPC_RMID, nullptr);
                return "";
            }
            std::string ret(str);
            shmdt(str);
            shmctl(shmid,IPC_RMID,NULL);
            return ret;
        }

        class SubscriptionPacket
        {

        };
        template<typename Token, typename UsableType = LD::Detail::Decay_T<Token>, typename Ret = typename UsableType::type>
        LD::Optional<Ret> ReadSubscription(Token && token) noexcept
        {
            LD::Optional<Ret> ret;
            token >> ret;
            return ret;
        }

        class ConfigurableDHT: public LD::DependencyInjection<ConfigurableDHT(LD::Configuration&,int,LD::BasicLogger&)>
        {
        private:
            int & mTest;
            LD::UInteger mPort;
            LD::UInteger mBootstrap;
            LD::ImmutableString<64> mHostName;
            LD::Optional<LD::Variant<LD::OpenDHTBackend>> mBackend;
        public:
            ConfigurableDHT(LD::Configuration & config,int & test, LD::BasicLogger & logger):mTest{test}
            {
                LD::Optional<LD::Integer> bootstrapPort = config("bootstrap",LD::Type<LD::Integer>{});
                LD::Optional<LD::Integer> bindPort = config("bind",LD::Type<LD::Integer>{});
                LD::Optional<LD::StringView> host = config("host",LD::Type<LD::StringView>{});
                LD::Optional<LD::StringView> type = config("type",LD::Type<LD::StringView>{});
                if (host)
                {
                    logger << "Found hostname: " << *host << "\n";
                    this->mHostName = *host;
                }
                if (bootstrapPort)
                {
                    logger << "Found Value From Config: " << *bootstrapPort << "\n";
                }

                if (type && *type == "OpenDHT")
                {
                    if (host && bootstrapPort && bindPort)
                    {
                        //logger << "OpenDHT node can be created" << "\n";
                        //this->mBackend = LD::Variant<LD::OpenDHTBackend>{LD::Move(LD::OpenDHTBackend{LD::IPV6Address{"fd00:1700:81b8:401e:0:d9:191:4a34"},LD::Port{4225},LD::Port{4222}})};
                    }
                }
                logger << "Test Number: " << this->mTest << "\n";
                //std::cout << "Test Number: " <<  mTest << "\n";
            }

            float CalculateMeme()
            {
                return LD::Sqrt(this->mTest);
            }
        };


        void PublisherSubscriber(LD::AbstractLogger & passedInLogger) noexcept
        {
            LD::BasicLogger log{passedInLogger};
            LD::Optional<std::string> example = std::string {"abc"};

            LD::Optional<std::size_t> possibleSize = example.Map(&std::string::size);
            possibleSize.Map([](auto & size, LD::BasicLogger & log) noexcept
            {
                log << "Found StringSize: " << size << "\n";
                //return 0;
            },log);


            auto j2 = R"(
                {
                "happy": true,
                "pi": 3.141,
                "key": 12.97,
                "messagebroker" :{"host": "fd00:1700:81b8:401e:0:d9:191:4a34", "bind": 4225, "bootstrap": 4222, "type": "OpenDHT" },
                "object" : {"key": 75 },
                "connections":{"outbound":{"type": "TCP", "port": 22000, "hostname": "1.1.1.1", "client": true, "server": false}}
                }
            )"_json;

            //nlohmann::json mConfiguration{j2};

            LD::JsonConfiguration configuration(LD::Mem::GetNewDeleteResourceReference(),j2);

            LD::JsonConfiguration brokerConfig(LD::Mem::GetNewDeleteResourceReference(),j2["messagebroker"]);
            LD::JsonConfiguration connectionsConfig(LD::Mem::GetNewDeleteResourceReference(),j2["connections"]);
            LD::BasicConnectionBroker<LD::StaticArray<LD::SharedPointer<LD::DataLink>,12>> broker{brokerConfig,connectionsConfig};
            LD::Optional<LD::DataLink&> possibleLink =  broker.Link("abcOne");

            LD::BasicServiceExecutor<void(LD::AbstractLogger&)> coreServices{configuration,passedInLogger};
            auto logger = coreServices.Make<LD::BasicLogger>();

            LD::BasicServiceExecutor<void(int,float,LD::BasicLogger&)> executionContext{configuration,int{99},float{},logger};

            LD::Optional<ConfigurableDHT> test =  executionContext.Make<ConfigurableDHT>("messagebroker");



            auto applicationHasStarted = [&](const LD::ApplicationStartedEvent<LD::Second<LD::Float>> & applicaitonStartedEvent) noexcept ->bool
            {
                //LD::Get<LD::TermBoxRenderContext>(applicaitonStartedEvent)->EnableMouse();

                possibleLink.Map([](LD::DataLink & link) noexcept
                {
                    LD::ToJson(link,LD::Pyramid{LD::Square{19},LD::Triangle{7,6}});
                });
                return LD::ApplicationRunningPredicate{};
            };
            auto applicationFrameHasStarted = [&](const LD::ApplicationFrameStartedEvent<LD::Second<LD::Float>> & frameStartedEvent ) noexcept -> bool
            {
                log << "Frame Started" << "\n";
                return LD::ApplicationQuittingPredicate{ (*LD::Get<LD::Second<LD::Float>>(frameStartedEvent)) > 10_s};;
            };
            auto applicationPeriodRequest = [](const LD::ApplicationPeriodEvent<LD::Second<LD::Float>> &) noexcept -> LD::Second<LD::Float>
            {
                return 1.0_s;
            };

            auto applicationExecution = [&](const LD::ApplicationExecutionEvent<LD::Second<LD::Float>> & applicationExecutionEvent ) noexcept
            {

                possibleLink.Map([](LD::DataLink & link, auto & log)
                {
                    LD::Pyramid possiblePyramid;
                    LD::Optional<LD::Pyramid&> pyramid =  LD::FromJSON(link,possiblePyramid);
                    if(pyramid)
                    {

                        log << "Found pyramid" << "\n";
                        log << "Possible Pyramid Base Length: " << possiblePyramid.Base().Length()<< "\n";

                    }

                },log);
            };

            auto applicationFrameEnded = [](const LD::ApplicationFrameEndedEvent<LD::Second<LD::Float>> & applicationFrameEndedEvent) noexcept
            {
                (*LD::Get<LD::Second<LD::Float>>(applicationFrameEndedEvent))+=1.0_s;
            };

            auto applicationQuit = [](const LD::ApplicationQuittingEvent<LD::Second<LD::Float>> &) noexcept
            {

            };

            auto generatedApp = LD::Overload{
                applicationHasStarted,
                applicationFrameHasStarted,
                applicationPeriodRequest,
                applicationExecution,
                applicationFrameEnded,
                applicationQuit};

            LD::BasicApplication<decltype(generatedApp)(LD::Second<LD::Float>)> application{generatedApp};

            LD::Timer currentTimer;
            LD::Second<LD::Float> accumulatedTime = 0.0_s;
            LD::ApplicationLoop(application,currentTimer,accumulatedTime);
        }
    }
}