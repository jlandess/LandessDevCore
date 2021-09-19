
#include "Examples/ReflectionDemoTypes.h"
#include "Examples/IMGUIExample1.h"
#include "Examples/IMGUIExample2.h"
#include "REST/CPR/cpr.h"
#include "Primitives/General/StringView.hpp"
#include "Primitives/General/ctre.hpp"
#include <IO/UnqliteDatabaseBackend.h>
#include <IO/Database.hpp>
#include <IO/RedisBackend.h>
#include <Algorithms/Benchmark.hpp>
#include "REST/Proxmox/Parameters/NetworkBridge.hpp"
#include "REST/Proxmox/Parameters/FeatureSet.hpp"
#include "REST/Proxmox/Parameters/SSHKey.h"
#include "REST/Proxmox/Parameters/Swap.hpp"
#include "REST/Proxmox/ProxyAPI.h"
#include "REST/Proxmox/Parameters/BridgeIndex.hpp"
#include "REST/Proxmox/Parameters/BridgeName.hpp"
#include "REST/Proxmox/Parameters/InterfaceIndex.hpp"
#include "REST/Proxmox/Parameters/FirewallStatus.hpp"
#include "Primitives/General/Unit.hpp"
#include "Network/MacAddress.hpp"
#include "Network/IPV4Address.hpp"
#include "SumTypes/MPark/variant.hpp"
#include "Reflection/NamedTuple.h"
#include "Core/RequestResponse.hpp"
#include "Network/SSH/SSH.hpp"
#include "IO/json.hpp"
#include "Memory/Optional.h"
#include "Reflection/Reflection.hpp"
#include "IaaS/DockerManifest.hpp"
#include "Algorithms/ToJSON.hpp"
#include "Async/GrandCentralDispatch.h"
#include <webdav/client.hpp>
#include "Algorithms/CTREImmutableExtension.hpp"
#include "IaaS/ClusteringMetaData.hpp"
#include "IO/Database1.h"
#include "IO/JsonDatabaseBackend.h"
#include "Process/Command.hpp"
#include "Algorithms/TDD.hpp"
#include <opendht.h>
#include <PBX/Mirta/Date.hpp>
#include "IO/DHTBackend.hpp"
#include "IO/TieredDatabase.h"
#include "IaaS/ArtifactRepository.hpp"
#include "Algorithms/FromJSON.hpp"
#include "IaaS/HashiVault/HashiVault.hpp"
#include "Examples/WebDavExample.h"
#include "Async/Scheduler.hpp"
#include "Async/Then.hpp"
#include "Async/SyncWait.hpp"
#include "Async/Just.hpp"
#include "Algorithms/Visitation.hpp"
#include "Async/WhenAll.hpp"
#include "Async/WhenAny.hpp"
#include "Async/Execute.hpp"
#include "Async/Let.hpp"
#include "Async/ScheduleAfter.hpp"
#include "Examples/WebServerExample.hpp"
#include "Examples/WebServerExample.hpp"
//{"data":"UPID:virtualhome:00004556:065341E5:6020F7FA:vzcreate:110:root@pam:"}
//UPID:([a-zA-Z0-9]+):([0-9a-fA-F]+):([0-9a-fA-F]+):([0-9a-fA-F]+):(\\w+):(\\d+):(\\w+)@([a-zA-Z0-9:]+)
#include "IO/PublisherSubscriber.h"
#include "Async/Thread.h"
void ParseResponse(LD::StringView view) noexcept
{
    if (auto [whole, nodeName, hex1, hex2,hex3,creationMethod,index,username,base] = ctre::match<LD::PVE::Detail::CreationRegex>(view); whole)
    {
        std::cout << nodeName.view() << std::endl;
        std::cout << hex1.view() << std::endl;
        std::cout << hex2.view() << std::endl;
        std::cout << hex3.view() << std::endl;
        std::cout << creationMethod.view() << std::endl;
        std::cout << index.view() << std::endl;
        std::cout << username.view() << std::endl;
        std::cout << base.view() << std::endl;
        //return LD::MakeContext(LD::TransactionResult{},LD::VW::DateTime{LD::Date(LD::Year<LD::UInteger>{LD::UInteger{year}},LD::Month<LD::UInteger>{month},LD::Day<LD::UInteger>{day}),LD::Time(hour,minute,second)},LD::Forward<A>(args)...);
    }
}



class DNSResponse
{
private:
    LD::ImmutableString<15> mDomainName;
    LD::StaticArray<LD::Variant<LD::IPV4Address,LD::IPV6Address>,30> mIPAddress;
    LD::UInteger mTTL;
public:

};

template<typename T>
class PublishedEntity
{
private:
    LD::ImmutableString<128> mNodeID;
    LD::DateTime mTimeStamp;
    T mData;
public:
};

namespace LD
{

    namespace Async
    {
        auto NewThread()
        {
            return [](auto p)
            {
                std::thread t{
                        [p = std::move(p)]() mutable
                        {
                            p.set_value();
                        }
                };
                t.detach();
            };
        }




        /*
        template<typename Task, typename F>
        auto async_algo(Task task, F && work)
        {
            return LD::Async::Then(task, [=]()
            {
                //int answer = 7;
                return work();
            });

        }
         */




        auto NewWorker(LD::Async::Scheduler & scheduler)
        {
            return [&](auto p)
            {

                auto function = [p = std::move(p)]() mutable
                {
                    p.set_value();
                };

                scheduler << function;
            };
        }

        auto NewWorker1(LD::Async::Scheduler & scheduler)
        {
            auto function =  [&](auto p)
            {

                auto function = [p = std::move(p)]() mutable
                {
                    p.set_value();
                };

                scheduler << function;
            };

            return LD::Async::Detail::Runnable<decltype(function),LD::Type<void>>{function};
        }
    }
}

namespace LD
{
    template<typename T,
            typename TL = decltype(GetSignatureArguments(LD::CT::FunctionSignature<decltype(&T::operator())>{}))>
    class LazyLambda
    {
    private:
        T mFunctor;
        LD::CT::RebindList<LD::CT::Prepend<LD::Detail::Decay_T<LD::CT::TypeAtIndex<0,TL>>,typename LD::tlist_erase_at_impl<0,TL>::type>,LD::Context> mContext;
    public:
        LazyLambda(T && functor) noexcept:mFunctor{LD::Forward<T>(functor)}
        {
        }

        LazyLambda & operator = (const LD::CT::TypeAtIndex<0,TL> & value) noexcept
        {
            LD::Get(LD::Get<0>(this->mContext)) = value;
            LD::Invoke(this->mFunctor,this->mContext);
            return (*this);
        }
    };

    template<typename T> LazyLambda(T && functor) -> LazyLambda<T>;
}





#include "Definitions/TimeExtension.hpp"
#include "Primitives/General/Unit.hpp"
int main(int argc, char **argv)
{





    LD::Second<int>{};
    //LD::WebServerExample();
    LD::Visit([](auto){ std::cout << "Visitiation:" << "\n";},mpark::variant<int>{7});
    LD::Visit([](auto){ std::cout << "Visitiation:" << "\n";},mapbox::util::variant<int>{7});

    LD::VisitWithContext(LD::Overload{[](int, std::string){},[](float, std::string){}},LD::MakeTuple(std::string{}),mpark::variant<int,float>{});


    LD::Async::Scheduler scheduler{8};
    //scheduler << [](){std::cout << "thread is executing function" << "\n";};
    //auto f = LD::Async::async_algo(NewWorker(scheduler),[]()->std::string{return std::string{"abc1"};});


    //LD::Async::LetValue(LD::Async::NewWorker1(scheduler),5);
    //LD::Async::Then(LD::Async::Then(LD::Async::NewWorker1(scheduler),LD::Async::Just(7)),[](int){ return 7;});

    //auto abc = LD::Async::Then(LD::Async::Just1(7),[](int){});

    LD::Async::Then(LD::Async::NewWorker1(scheduler),LD::Async::Just(7));
    //LD::CT::DebugTemplate<decltype(abc)>{};
    //auto asyncF1 = LD::Async::Then(LD::Async::NewWorker1(scheduler),[](){ return  7;});
    //auto asyncF12 = LD::Async::Then(LD::Async::NewWorker1(scheduler),[](){ });
    //auto asyncF2 = LD::Async::Then(asyncF1,[](int number){ return 7*number;});


    auto thenCaller = LD::Async::Then(LD::Async::Just(7),[](int){ return 7;});
    //LD::Async::Then(LD::Async::LetValue(7,LD::Async::NewWorker1(scheduler)),[](int){ return 7;});
    auto asyncF1 = LD::Async::Then(LD::Async::NewWorker1(scheduler),[](){ return  7;});



    LD::Second<int> abc = 79_ms;
    LD::Async::ScheduleAfter(abc,LD::Async::NewWorker1(scheduler));
    std::cout << "LetValue Example: " << LD::Async::SyncWait(LD::Async::LetValue(LD::Async::NewWorker1(scheduler),7,[](int){ return 8;})) << "\n";
    //LD::Async::Then(LD::Async::NewWorker1(scheduler),LD::Async::LetValue(7,[](int){ return 8;}));
    auto asyncF2 = LD::Async::Then(asyncF1,[](int number){ return 7*number;});
    //static_assert(LD::Async::Detail::ArgumentCount<decltype(asyncF2)>::Count == 0);
    auto asyncF12 = LD::Async::Then(LD::Async::NewWorker1(scheduler),[](){ });
    auto syncWaitJust1 = LD::Async::Then(LD::Async::Then(LD::Async::NewWorker1(scheduler),LD::Async::Just(6)),[](int a){ return std::sin(a);});

    //auto syncWaitJust1Then = LD::Async::Then1(syncWaitJust1,[](int a){ return 7*a;});
    std::cout << "LawlRawr: " << LD::Async::SyncWait(syncWaitJust1) << "\n";
    //auto fsauce = LD::Async::Then(LD::Async::Just1(5),[](int ){ return 9;});

    auto testJust1 = LD::Async::Just(7);

    //LD::CT::DebugTemplate<decltype(LD::Async::Just1(7))>{};
    auto memez12 = LD::Async::Then(LD::Async::NewWorker1(scheduler),LD::Async::Just(7));

    //LD::Async::SyncWait(LD::Async::Then1(LD::Async::NewWorker1(scheduler),memez12));
    //LD::Async::Then(memez12,[](int){ return 8;});
    //auto fmeme = LD::Async::Then(fsauce,[](int ){});
    LD::Async::WhenAll(LD::Async::Then(LD::Async::NewWorker1(scheduler),[](){ return  7;}),
                        LD::Async::Then(asyncF1,[](int number){ return 7*number;}));
    auto whenAllAreDone = LD::Async::WhenAll(
            LD::Async::Then(LD::Async::NewWorker1(scheduler),[](){ return  7;}),
            LD::Async::Then(asyncF1,[](int number){ return 7*number;})
            );


    auto whenOneIsDone =  LD::Async::WhenAny(
            LD::Async::Then(LD::Async::NewWorker1(scheduler),[](){ return  7;}),
            LD::Async::Then(LD::Async::NewWorker1(scheduler),[](){ return  7.0;})
            );




    //LD::Async::SyncWait(asyncF12);
    LD::Async::Execute(asyncF12);
    LD::Tuple<int,int > res = LD::Async::SyncWait(whenAllAreDone);

    std::cout << "Number: " << LD::Async::SyncWait(asyncF2) << "\n";

    std::cout << LD::Get<0>(res) << " , " << LD::Get<1>(res) << "\n";
    /*
    auto f = NewWorker(scheduler);
    auto f2 = LD::Async::Then(f, [](){return "abc";});
    auto f3 = LD::Async::Then(f2, [](std::string i ){return i+std::to_string(3);});
    printf("Aysnc Execution lol: %s \n", LD::Async::SyncWait<std::string>(f3).c_str());


    auto f5 = LD::Async::Then(LD::Async::NewWorker(scheduler),[](){ return 7;});
     */

    //std::cout << "Value to be seen: " << LD::Async::SyncWait<LD::UInteger>(f5) << "\n";
    LD::OpenDHTBackend mBackend{LD::IPV6Address{"fd00:1700:81b8:401e:0:d9:191:4a34"},LD::Port{4225},LD::Port{4222}};

    LD::Channel<LD::UInteger> channel;
    LD::BackInserter<LD::Channel<LD::UInteger>> inserter{&channel};
    LD::LazyLambda lambda{[](LD::UInteger index)
    {
         std::cout << "Found DHT Value: " << index << "\n";
    }};

    //LD::Tuple<decltype(lambda)> rawr;
    using LambdaType = decltype(lambda);

    /*
    auto f1 = LD::Async::async_algo(
            LD::Async::NewWorker(scheduler),
            LD::Subscribe1(mBackend,LD::ImmutableString{"key"},
                           LD::ElementReference<decltype(lambda)> {lambda},
                           LD::Type<LD::UInteger>{}));
   // auto f4 = then(f1, [](LD::Channel<LD::UInteger> i){return  i;});
    LD::Async::SyncWait<LD::UInteger>(f1);
     */


    sleep(5);

    while (channel.Size() > 0)
    {
        LD::UInteger integer;
        channel >> integer;
        std::cout << integer << "\n";
    }
    //printf("Aysnc Execution lol: %li \n", SyncWait<LD::UInteger>(f1));

    //auto f4 = then(f1, [](LD::UInteger i){return i;});
    //printf("Aysnc Execution lol: %s \n", SyncWait<LD::UInteger>(f4));
    /*
    nlohmann::json json123;
    LD::JsonBackend jsonBackend123{json123};





    LD::Insert(
            jsonBackend123,
            LD::ImmutableString{"key1"},
            LD::Pyramid{LD::Square{8},LD::Triangle{7,98}});


    LD::Insert(
            jsonBackend123,
            LD::ImmutableString{"keyone"},
            LD::MakeTuple(1.9,7,'a'));


    LD::StaticArray<LD::Pyramid,5> memeArray;
    memeArray.PushBack(LD::Pyramid{LD::Square{8},LD::Triangle{7,98}});
    memeArray.PushBack(LD::Pyramid{LD::Square{19},LD::Triangle{79,22}});
    memeArray.PushBack(LD::Pyramid{LD::Square{25},LD::Triangle{19,9}});
    memeArray.PushBack(LD::Pyramid{LD::Square{9},LD::Triangle{12,36}});
    memeArray.PushBack(LD::Pyramid{LD::Square{8},LD::Triangle{7,98}});
    LD::Insert(
            jsonBackend123,
            LD::ImmutableString{"staticArray"},
            memeArray);

    LD::Insert(
            jsonBackend123,
            LD::ImmutableString{"keytwo"},
            LD::Variant<LD::Square,int>{LD::Square{96}});

    LD::Pyramid testPyramid;
    LD::Fetch(
            jsonBackend123,
            LD::ImmutableString{"key1"},
            testPyramid);

    LD::Insert(
            jsonBackend123,
            LD::ImmutableString{"abc"},
            23);
    LD::UInteger memesz;
    LD::Fetch(
            jsonBackend123,
            LD::ImmutableString{"abc"},
            memesz);

    std::cout << "ABC Number: " << memesz << std::endl;

    std::cout << json123.dump(2) << std::endl;
    std::cout << testPyramid.Side().Base() << std::endl;

    LD::Tuple<LD::Float,LD::UInteger,char,LD::Square> tupleToDeserialize;
    LD::Fetch(
            jsonBackend123,
            LD::ImmutableString{"keyone"},
            tupleToDeserialize);

    std::cout <<
    LD::Get(LD::Get<0>(tupleToDeserialize)) <<
    "," <<
    LD::Get(LD::Get<1>(tupleToDeserialize)) <<
    "," <<
    LD::Get(LD::Get<2>(tupleToDeserialize)) <<
    "," <<
    LD::Get(LD::Get<3>(tupleToDeserialize)).Length() <<
    "\n";

    LD::Variant<LD::Square,int> mMemeVariant;
    LD::Fetch(
            jsonBackend123,
            LD::ImmutableString{"keytwo"},
            mMemeVariant);

    auto onMemeSquare = [](const LD::Square & square) noexcept
    {
        std::cout << "Meme Square: " << square.Length() << "\n";
    };


    LD::MultiMatch(LD::Overload{onMemeSquare,[](auto){}},mMemeVariant);


    //fd00:1700:81b8:401e:0:d9:191:4a34
    LD::OpenDHTBackend mBackend{LD::IPV6Address{"fd00:1700:81b8:401e:0:d9:191:4a34"},LD::Port{4225},LD::Port{4222}};


    LD::Insert(
            mBackend,
            LD::ImmutableString{"dhttuple"},
            LD::MakeTuple(1.9,7,'a'));

    mBackend.Query(LD::StringView{"room.Side.Height"},[](LD::StringView key, LD::StringView value)->int
    {
        std::cout << "Key : " << key << "     |   Value: " << value << std::endl;
        return 5;
    });

    LD::Insert(mBackend,LD::ImmutableString{"room"},
               LD::Pyramid{LD::Square{8},LD::Triangle{7,105}});





    std::cout << "ClassName: " << LD::CT::GetClassName(LD::Type<int>{}).Data() << std::endl;

    LD::Channel<LD::UInteger> queue;
    auto subscriptionResponse = LD::Subscribe(
             mBackend,
             LD::ImmutableString{"room.Side.Height"},
             LD::Type<LD::UInteger>{});


    auto onSuccessfulSubscription = [&](LD::Channel<LD::UInteger> channel) noexcept
    {
        queue = channel;
    };

    auto onSubscriptionFailure = [](LD::TransactionError) noexcept
    {

    };

    LD::InvokeVisitation(LD::Overload{onSuccessfulSubscription,onSubscriptionFailure},subscriptionResponse);


    LD::SharedLock<LD::Mutex> sharedLock;

   LD::Subscribe(
            mBackend,
            LD::ImmutableString{"memez1"},
            LD::Type<LD::Variant<LD::UInteger,bool>>{},
            [](LD::Variant<LD::UInteger,bool> variant, LD::SharedLock<LD::Mutex> sharedLock)
            {
                auto onInteger = [](LD::UInteger value) noexcept
                {
                    std::cout << "Found UInteger in Variant: " << value << "\n";
                };

                auto onBool = [](bool value) noexcept
                {

                };

                LD::MultiMatch(LD::Overload{onInteger,onBool}, variant);

            },sharedLock);


    LD::Subscribe(
            mBackend,
            LD::ImmutableString{"keyone"},
            LD::Type<LD::Tuple<double,int,char>>{},
            [](LD::Tuple<double,int,char> tuple, LD::SharedLock<LD::Mutex> sharedLock)
            {
                LD::For<3>([](
                        auto I,
                        LD::Tuple<double,int,char> tuple)
                {
                    std::cout << "Found Tuple: " << LD::UInteger(I) << " : " << LD::Get(LD::Get<I>(tuple)) << "\n";
                    return true;
                },tuple);
                printf("found tuple from data set \n");

            },sharedLock);


    LD::UInteger abc = 0;
    LD::Subscribe(mBackend,LD::ImmutableString{"room.Side.Height"},LD::Type<LD::UInteger>{},
    [&](LD::UInteger response, LD::SharedLock<LD::Mutex> sharedLock)
    {
        LD::ScopeLock<LD::SharedLock<LD::Mutex>> scopedLock{sharedLock};

        abc = response;
        std::cout << "the wheels on the bus go round and round all through the town!" << "\n";
    },sharedLock);


    LD::Tuple<int> adsfsadfs;
    queue << 7;

    queue << 7;

    queue << 7;

    queue << 7;
    LD::StaticArray<LD::Pyramid,5> usableMemeArray;

    LD::Fetch(
            jsonBackend123,
            LD::ImmutableString{"staticArray"},
            usableMemeArray);

    std::cout << usableMemeArray[1].Base().Length() << std::endl;
    /*
    LD::HashiVault vault{
        LD::ImmutableString{"http://192.168.30.50:8200"},
        LD::ImmutableString{"s.w8hIFCNwl59CMABCvaZ1qlGV"}};

    auto vaultResponse = vault.Query(
            LD::Type<LD::Square>{},
            LD::ImmutableString{"demoapplications"},
            LD::ImmutableString{"square"});


    auto onVaultSquare = [](const LD::Square & square) noexcept
    {
        std::cout << "Vault Square Length: " << square.Length() << std::endl;
    };

    LD::InvokeVisitation(LD::Overload{onVaultSquare,[](auto){}},vaultResponse);


    nlohmann::json jsonToJsonTest;
    LD::ToJson(jsonToJsonTest,LD::Pyramid{LD::Square{7},LD::Triangle{7,9}});


    std::cout << jsonToJsonTest.dump(2) << std::endl;

    simdjson::dom::parser parser;

    simdjson::dom::element parsedResponse = parser.parse(jsonToJsonTest.dump(2));


    LD::TieredKeyValueStore<LD::RedisBackingStore,LD::CT::TypeList<LD::OpenDHTBackend>> dnsEntries;
    LD::UnQliteBackend<char> sqliteDB{"database1.db",LD::OpenAndCreateIfNotExists{}};
    LD::Timer currentTimer;
    nlohmann::json json;
    LD::JsonBackend jsonBackend{json};
    LD::OpenDHTBackend mBackend{LD::IPV6Address{"fd00:1700:81b8:401e:0:d9:191:4a34"},LD::Port{4222},LD::Port{4222}};
    auto test1 = [&](LD::UnQliteBackend<char> & db, const auto & serializableObject) -> int
    {
        LD::Insert(db,LD::ImmutableString{"key1"},serializableObject);

        mBackend.Query(LD::StringView{"lucario"},[](LD::StringView key, LD::StringView value)->int
        {
            std::cout << "Key : " << key << "     |   Value: " << value << std::endl;
            return 5;
        });

        return 5;
    };
    auto testResult = LD::TDD::Execute(
            LD::ImmutableString{"Insertion Test"},
            currentTimer,
            test1,
            5,
            sqliteDB,
            LD::Pyramid{LD::Square{928},LD::Triangle{65,50}});

    LD::ArtifactRepository repository{LD::ElementReference<LD::JsonBackend>{jsonBackend}};


    LD::Insert(jsonBackend,LD::ImmutableString{"keyone"},LD::MakeTuple(1.9,7,'a',LD::Square{7}));
    repository.Insert(
            testResult,
            testResult,
            testResult
    );
    std::cout << json.dump(2) << std::endl;
    /*
    int abc = 0;
    mBackend.Listen();
    while(abc < 25)
    {

        ++abc;
        sleep(1);
    }
     */
    /*
    // put some data on the dht
    /*
     */
    /*
    LD::Timer timer;
    timer.Start();
    // get data from the dht
    dht::InfoHash id;

    LD::UnQliteBackend<char> unqliteBackingStore{"database.db",LD::OpenAndCreateIfNotExists{}};

    unqliteBackingStore.Query(LD::StringView{"key.Length"},[](auto a, auto b)
    {
        //std::cout << "Key : " << a << " Value:" << b << std::endl;
        return 7;
    });

    LD::Insert(
            unqliteBackingStore,
            LD::ImmutableString{"key1"},
            LD::Pyramid{LD::Square{928},LD::Triangle{65,50}});
    auto fetchResponse = LD::Fetch(unqliteBackingStore,LD::ImmutableString{"key1"},LD::Type<LD::Pyramid>{});



    auto onResponse = [](LD::Pyramid geometry) noexcept
    {
        std::cout << "Geometry length: " << geometry.Base().Length() << std::endl;
        std::cout << "Side Base:" << geometry.Side().Base() << std::endl;
        std::cout << "Side Height:" << geometry.Side().Height() << std::endl;
    };

    auto onError = [](LD::TransactionError ) noexcept
    {

    };
    LD::InvokeVisitation(LD::Overload{onResponse,onError},fetchResponse);
    LD::Example::WebDavExample();
    //LD::Example::IMGUIExample1();
    //LD::Example::IMGUIExample2();
    /*
    ParseResponse("UPID:virtualhome:00004556:065341E5:6020F7FA:vzcreate:110:root@pam:");
    */

    LD::Timer timer;
    timer.Start();

    /*
    while (timer.GetElapsedTimeInSec() < 15)
    {


        std::cout << "ABC: " << abc << std::endl;
        if (queue.Size() > 0)
        {
            LD::UInteger number;
            queue >> number;

            std::cout << "Number: " << number << "\n";
        }

        if (queue->try_dequeue(number))
        {
            std::cout << number << std::endl;
        }

        //std::cout << value << std::endl;
        sleep(1);
    }
    */
    //sleep(5);
    return 0;
}