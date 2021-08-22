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
//{"data":"UPID:virtualhome:00004556:065341E5:6020F7FA:vzcreate:110:root@pam:"}
//UPID:([a-zA-Z0-9]+):([0-9a-fA-F]+):([0-9a-fA-F]+):([0-9a-fA-F]+):(\\w+):(\\d+):(\\w+)@([a-zA-Z0-9:]+)

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

class TestClass
{
private:
    LD::ImmutableString<15> mLabel;
    LD::Float mPrecision;
public:
    TestClass():mLabel{LD::ToImmutableString("abc")},mPrecision{7.5}
    {

    }
    const LD::ImmutableString<15> & Label() const noexcept {return this->mLabel;}
    LD::ImmutableString<15> & Label() noexcept{return this->mLabel;}


    const LD::Float & Precision() const noexcept{return this->mPrecision;}
    LD::Float  & Precision() noexcept{return this->mPrecision;}

};

class DNSResponse
{
private:
    LD::ImmutableString<15> mDomainName;
    LD::StaticArray<LD::Variant<LD::IPV4Address,LD::IPV6Address>,30> mIPAddress;
    LD::UInteger mTTL;
public:

};

template<>
struct LD::CT::TypeDescriptor<TestClass>
{
private:
    static constexpr auto LabelName = ctll::basic_fixed_string("Label");
    static constexpr auto PrecisionName = ctll::basic_fixed_string("Precision");
public:
    static constexpr auto ClassName = ctll::fixed_string{"TestClass"};

    using MemberList = LD::CT::TypeList<
            LD::CT::EncapsulatedMemberDescriptor<LabelName,LD::CT::SelectOverload<LD::ImmutableString<15> & (TestClass::*)(),&TestClass::Label>(),LD::CT::SelectOverload<const LD::ImmutableString<15> & (TestClass::*)() const,&TestClass::Label>()>,
            LD::CT::EncapsulatedMemberDescriptor<PrecisionName,LD::CT::SelectOverload<LD::Float & (TestClass::*)(),&TestClass::Precision>(),LD::CT::SelectOverload<const LD::Float & (TestClass::*)() const,&TestClass::Precision>()>
    >;


    static constexpr MemberList Members{  };

};

/**
 * Iterate 1-100
If divisible by 3 print “Fizz”
If divisible by 5 print “Buzz”
If divisible by 3 and 5 print “FizzBuzz”
If not divisible by 3 or 5 print number

 * @param size
 */
#define SEC_PER_DAY   86400
#define SEC_PER_HOUR  3600
#define SEC_PER_MIN   60
int main(int argc, char **argv)
{
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


    auto parsedSquare = LD::FromJSON(LD::Type<TestClass>{},parsedResponse);

    auto onParsedSquare = [](const TestClass & square) noexcept
    {
        std::cout << "Parsed Square Length: " << square.Label().Data() << std::endl;
    };

    auto onFailedSquare = [](const LD::TransactionError & error) noexcept
    {

    };

    LD::InvokeVisitation(LD::Overload{onParsedSquare,onFailedSquare},parsedSquare);

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
    LD::Timer timer;
    timer.Start();
    // get data from the dht
    dht::InfoHash id;
    /*
    /*
     */
    LD::UnQliteBackend<char> unqliteBackingStore{"database.db",LD::OpenAndCreateIfNotExists{}};

    unqliteBackingStore.Query(LD::StringView{"key.Length"},[](auto a, auto b)
    {
        //std::cout << "Key : " << a << " Value:" << b << std::endl;
        return 7;
    });

    LD::Insert(unqliteBackingStore,LD::ImmutableString{"key1"},LD::Pyramid{LD::Square{928},LD::Triangle{65,50}});
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
    std::map<std::string, std::string> options =
            {
                    {"webdav_hostname", "https://nextcloud.landesshome.com/remote.php/dav/files/phoenixflower/"},
                    {"webdav_username", "phoenixflower"},
                    {"webdav_password", "Next_Cloud_Dcs1224"}
            };
    std::unique_ptr<WebDAV::Client> client = std::make_unique<WebDAV::Client>(options);

    char * string = "abcad;lfjas;ldfksadjf;sdaflkasj";

    LD::Timer timer;
    timer.Start();
    client->upload_from("Carrie Landess MD/abc.txt",string,strlen(string));
    bool check_connection = client->check();
    std::cout << "test connection with WebDAV drive is "
              << (check_connection ? "" : "not ")
              << "successful"<< std::endl;
    auto directories = client->list();

    timer.Stop();

    std::cout << "Time Elapsed " << LD::ToImmutableString(timer.Time()/LD::MicroSecond<LD::UInteger>{LD::UInteger(1)}).Data() << std::endl;
    for(const auto & dir: directories)
    {
        std::cout << dir << std::endl;
    }
     */
    //ndp1();
    //std::cout << LD::ToImmutableString(address).Data() << std::endl;

    /*
    LD::NDP ndp1;
    ndp * ndpSocket = nullptr;
    ndp_open(&ndpSocket);
    //run_main_loop(ndpSocket);
    ;
    run_cmd_monitor(ndpSocket,NDP_MSG_ALL,if_nametoindex("ens22"));
    ndp_close(ndpSocket);
    ndp1();

    std::visit(overload{
            [](LightItem&, LightItem& ) { std::cout << "2 light items\n"; },
            [](LightItem&, HeavyItem& ) { std::cout << "light & heavy items\n"; },
            [](HeavyItem&, LightItem& ) { std::cout << "heavy & light items\n"; },
            [](HeavyItem&, HeavyItem& ) { std::cout << "2 heavy items\n"; },
    }, basicPackA, basicPackB);
    std::variant<Fluid, LightItem, HeavyItem, FragileItem> package;

    ParseResponse("UPID:virtualhome:00004556:065341E5:6020F7FA:vzcreate:110:root@pam:");


    */
    return 0;
}