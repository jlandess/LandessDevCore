#include "Chrono/Timer.h"
#include <iostream>
#include "IO/FetchRequest.h"
#include "Examples/ReflectionDemoTypes.h"
#include "TUI/ascii_art.h"
#include "TypeTraits/TypeList.hpp"
#include "IO/toml.hpp"
#include <stdexcept>
#include "SPA/Backend.hpp"
#include "SPA/Compositor.hpp"
#include "SPA/Linker.hpp"
#include "SPA/Button.hpp"
#include "SPA/HTML.hpp"
#include "SPA/EventHandler.hpp"
#include "IO/XML/tinyxml2.h"
#include "SPA/Div.hpp"
#include "SPA/Variable.hpp"
#include "SPA/Function.hpp"
#include "Random/MarsagliaRandomNumberGenerator.hpp"
#include "TypeTraits/CanBeAnImmutableString.hpp"
#include "REST/HTMLEmitter.hpp"
#include "Reflection/Reflection.hpp"
#include "Primitives/General/DateTime.h"
#include "REST/CPR/cpr.h"
#include "PBX/MIrta/Date.hpp"
#include "IO/EphemiralRowBackingStore.hpp"
#include "VirtuWorks/QueueLog.hpp"
#include "IO/CSVParser.hpp"
#include "TypeTraits/__or.hpp"
#include "TypeTraits/IsConstructible.hpp"
#include <map>
#include "TypeTraits/IsErasedOrConvertible.hpp"
#include "Memory/CorePrimitiveAllocator.hpp"
#include "Memory/FreeList.h"

#include <stdio.h>
#include <stdlib.h>
#include "Algorithms/Searching.hpp"
#include "Algorithms/Sorting.hpp"
template<typename KeyType,typename TennantType>
class MirtaPBX
{
private:
    KeyType mAPIKey;
    TennantType mTennant;
public:
    MirtaPBX(KeyType && apiKey, TennantType && tennant) noexcept:mAPIKey{apiKey},mTennant{tennant}
    {

    }
    LD::EphemiralRowBackingStore FetchQueueInfo(LD::PBX::MT::Date mStart, LD::PBX::MT::Date mEnd)
    {

        std::cout << "Date Start: " << LD::ToImmutableString(mStart).Data() << std::endl;
        cpr::Response r = cpr::Get(cpr::Url{"https://voip.virtuworks.net/pbx/proxyapi.php"},
                                   cpr::Parameters
                                           {{"key", this->mAPIKey.Data()},
                                            {"reqtype", "INFO"},
                                            {"info","QUEUELOGS"},
                                            {"format","csv"},
                                            {"tenant",this->mTennant.Data()},
                                            {"start",LD::ToImmutableString(mStart).Data()},//2020-11-17 LD::ToImmutableString(mStart).Data()
                                            {"end",LD::ToImmutableString(mStart++).Data()}});//2020-11-18 LD::ToImmutableString(mEnd).Data()

        return LD::EphemiralRowBackingStore{std::make_unique<std::string>(r.text)};
    }
};
template<typename KeyType, typename TennantType> MirtaPBX(KeyType &&, TennantType &&) -> MirtaPBX<KeyType,TennantType>;

class Encounters
{
private:
    LD::UInteger mNumber;
public:
    Encounters() noexcept:mNumber{0}
    {

    }
    const LD::UInteger Number() const noexcept
    {
        return this->mNumber;
    }
    Encounters & operator++() noexcept
    {
        this->mNumber++;
        return (*this);
    }
    Encounters & operator++(int) noexcept
    {
        this->mNumber++;
        return (*this);
    }
};





void SaveMissedCalls(LD::Date date) noexcept
{



    auto fileName = LD::ImmutableString{"/home/phoenixflower/SeaDrive/My Libraries/PBXTests/"}+LD::ToImmutableString(LD::PBX::MT::Date{date});
    FILE  * file = fopen(fileName.Data(),"w+");
    MirtaPBX pbxInstance{LD::ImmutableString{"EbcHbsnXAqSrTKBb"},LD::ImmutableString{"CarrieLandess"}};
    LD::EphemiralRowBackingStore rows = pbxInstance.FetchQueueInfo(date,++date);
    LD::CSVParser csvParser{rows,LD::CT::TypeList<LD::VW::QueueLog>{}};
    std::map<std::string,Encounters> encounters;
    for(auto beingOfCSV = csvParser.Begin();beingOfCSV!=csvParser.End();++beingOfCSV)
    {
        auto object = (*beingOfCSV);

        auto onLog = [&](
                const LD::Context<LD::TransactionResult,LD::VW::QueueLog> & context
        ) noexcept
        {
            const LD::VW::QueueLog & log = LD::Get(LD::Get<1>(context));
            if (log.Status() == LD::VW::Abandoned || log.Status() == LD::VW::Timeout)
            {
                auto number = log.PhoneNumber().Number();
                encounters[number.Data()]++;
                //fwrite(number.Data(),sizeof(char),number.GetSize(),file);
                //fwrite("\n",sizeof(char),1,file);
            }
        };

        auto onError = [](
                const LD::Context<LD::TransactionError> & context
        ) noexcept
        {};

        LD::MultiMatch(LD::Overload{onLog,onError},object);
    }

    for(auto it = encounters.begin();it!=encounters.end();++it)
    {
        const char * format = ": ";
        (*it).first.c_str();
        fwrite((*it).first.c_str(),sizeof(char),(*it).first.size(),file);
        fwrite(format,sizeof(char),strlen(format),file);
        auto numberAsString = LD::ToImmutableString((*it).second.Number());
        fwrite(numberAsString.Data(),sizeof(char),numberAsString.GetSize(),file);
        fwrite("\n",sizeof(char),1,file);
    }
    fclose(file);
}





int main(int argc, char* argv[])
{


    LD::StaticArray<int,10> rawrbunnies;
    for(LD::UInteger n = 0;n<10;++n)
    {
        rawrbunnies.PushBack(rand()%50);
        //rawrbunnies[n] = rand()%50;
    }
    std::cout << "Static Array Size: " << rawrbunnies.End()-rawrbunnies.Begin() << std::endl;
    LD::QuickSort(rawrbunnies,[](int a, int b) noexcept {return a<b;});
    for(LD::UInteger n = 0;n<10;++n)
    {
        std::cout << rawrbunnies[n] << std::endl;
    }
    auto binarySearchResult = LD::BinarySearch(rawrbunnies,35,[](int a,int b){return a==b;},[](int a,int b){return a<b;});
    auto onBinaryFound = [](const LD::Context<LD::TransactionResult,LD::UInteger> & context) noexcept
    {
        std::cout << "found result at index: " << LD::Get(LD::Get<1>(context)) << std::endl;
    };
    LD::MultiMatch(LD::Overload{onBinaryFound,[](auto){std::cout << "found error" << std::endl;}},binarySearchResult);
    //SaveMissedCalls(LD::Date{2020,12,7});


    LD::MarsagliaMultiplyWithCarryGenerator randomNumberGenerator{static_cast<unsigned long>(time(0))};
    auto randomLambda = [](LD::MarsagliaMultiplyWithCarryGenerator & generator) noexcept
    {
        return generator.NextInteger();
    };
    LD::SPA::Var varType{7,randomLambda,randomNumberGenerator};
    LD::SPA::Var varType3{7,randomLambda,randomNumberGenerator};
    LD::SPA::Var varType4{7,randomLambda,randomNumberGenerator};


    //LD::ImmutableString
    auto expression = (varType+varType3)+(varType4+varType4);

    LD::SPA::EventHandler handler{LD::SPA::OnLoad{},expression};
    LD::SPA::Div gotcha2{handler,LD::SPA::Text{7}};

    LD::SPA::Function function{expression};
    LD::ToImmutableString(function);
    std::cout << LD::ToImmutableString(expression).Data() << std::endl;

    LD::SPA::ClassName divClassName77{LD::StringView {"bomb"}};
    LD::SPA::Language domLanguage77{LD::StringView {"en-US"}};
    LD::SPA::Compositor<LD::CT::TypeList<>> compositor;
    LD::SPA::Button button{compositor,LD::StringView{"agc"}};
    LD::SPA::Div gotcha{LD::SPA::Text{7}};
    LD::SPA::Language domLanguage{LD::StringView {"en-US"}};
    LD::SPA::Language divLanguage{LD::StringView {"en-US"}};
    LD::SPA::ClassName divClassName{LD::StringView {"bomb"}};
    LD::SPA::Div div{LD::Pyramid{LD::Square{5},LD::Triangle{5,5}},divClassName,divLanguage};
    LD::SPA::HTML document{div,domLanguage,gotcha2};

    LD::HTMLEmitter printer;
    printer.Print(document);
    tinyxml2::XMLPrinter prettyPrinter;
    printer.xmlDoc.Accept( &prettyPrinter );

    std::cout << prettyPrinter.CStr() << std::endl;

    LD::SPA::Linker<LD::CT::TypeList<>> linker{compositor};
    
    return 0;
}
