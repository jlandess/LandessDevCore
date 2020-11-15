#include "Chrono/Timer.h"
#include <iostream>
#include "IO/FetchRequest.h"
#include "Examples/ReflectionDemoTypes.h"
#include "Primitives/General/Range.h"
#include "Primitives/General/ctre.hpp"
#include "TUI/ascii_art.h"
#include "IO/Database.hpp"
#include "TypeTraits/TypeList.hpp"
#include "IO/toml.hpp"
#include <stdexcept>
#include "SPA/Backend.hpp"
#include "SPA/Compositor.hpp"
#include "SPA/Linker.hpp"
#include "SPA/Button.hpp"
#include "SPA/HTML.hpp"
#include "SPA/EventHandler.hpp"
#include "SPA/Attributes.hpp"
#include "SPA/Text.hpp"
#include "IO/XML/tinyxml2.h"
#include "SPA/Div.hpp"
#include "SPA/Variable.hpp"
#include "Random/MarsagliaRandomNumberGenerator.hpp"
#include "TypeTraits/CanBeAnImmutableString.hpp"
#include "REST/HTMLEmitter.hpp"
#include "Memory/Optional.h"
#include "IO/RowBackingStore.h"
#include "Reflection/Reflection.hpp"
class IVRDate
{
private:

public:
};

class IVRDestination
{
private:

public:
};

class IVRAttendant
{
private:

};

enum IVRActionType{IVRActionSelect,IVRActionTimeout,IVRHangUp,IVRWrong};
class IVRAction
{
private:
    IVRActionType mAction;
public:
};

class IVRCallerID
{
private:
    LD::ImmutableString<64> mID;
    LD::StaticArray<char,10> mNumber;
    LD::UInteger mCountryCode;
public:
};

class IVROption
{
private:
    LD::UInteger mOption;
public:
    const LD::UInteger & Option() const noexcept
    {
        return this->mOption;
    }

    LD::UInteger & Option() noexcept
    {
        return this->mOption;
    }
};

class IVREntry
{
private:
    IVRDate mCallDate;
    IVRDate mCallStart;
    IVRDate mCallEnd;
    IVRCallerID mCallerID;
    IVRDestination mDestination;
    IVRAttendant mAttendant;
    IVRAction mAction;
    IVROption mOption;
public:
    const IVRDate & CallDate() const noexcept
    {
        return this->mCallDate;
    }

    IVRDate & CallDate() noexcept
    {
        return this->mCallDate;
    }

    const IVRDate & CallStart() const noexcept
    {
        return this->mCallStart;
    }

    IVRDate & CallStart() noexcept
    {
        return this->mCallStart;
    }

    const IVRDate & CallEnd() const noexcept
    {
        return this->mCallEnd;
    }

    IVRDate & CallEnd() noexcept
    {
        return this->mCallEnd;
    }

    const IVRCallerID & CallerID() const noexcept
    {
        return this->mCallerID;
    }

    IVRCallerID & CallerID() noexcept
    {
        return this->mCallerID;
    }

    const IVRDestination & Destination() const noexcept
    {
        return this->mDestination;
    }
    IVRDestination & Destination() noexcept
    {
        return this->mDestination;
    }

    const IVRAttendant & Attendant() const noexcept
    {
        return this->mAttendant;
    }

    IVRAttendant & Attendant() noexcept
    {
        return this->mAttendant;
    }

    const IVRAction & Action() const noexcept
    {
        return this->mAction;
    }

    IVRAction & Action() noexcept
    {
        return this->mAction;
    }

    const IVROption & Option() const noexcept
    {
        return this->mOption;
    }

    IVROption & Option() noexcept
    {
        return this->mOption;
    }
};




namespace LD
{
    template<typename BackingStore, typename Types>
    class CSVParser;
    template<typename BackingStore, typename ... A>
    class CSVParser<BackingStore,LD::CT::TypeList<A...>>
    {
    private:
        LD::Ref<BackingStore> mBackingStore;
        LD::Optional<LD::UInteger> mSelectedType;
    public:
        CSVParser(LD::Ref<BackingStore> backingStore, LD::CT::TypeList<A...> ) noexcept:mBackingStore{backingStore}
        {
            constexpr auto NumberOfSearchableTypes = sizeof...(A);


            auto primaryLine = this->mBackingStore.GetLine(this->mSelectedType);

            auto onError = [](const LD::Context<LD::TransactionError,LD::Optional<LD::UInteger>&> & error) noexcept
            {

            };

            auto onHeader = [](const LD::Context<LD::TransactionResult,LD::StringView,LD::Optional<LD::UInteger>&> & transaction) noexcept
            {
                auto & selectedType = LD::Get(LD::Get<2>(transaction));
                auto row = LD::Get(LD::Get<1>(transaction));
                LD::For<NumberOfSearchableTypes>([](
                        auto I,
                        LD::Optional<LD::UInteger> & selectedIndex,
                        LD::StringView row) noexcept
                {

                    return true;
                },selectedType,row);
            };

            LD::MultiMatch(LD::Overload{onError,onHeader},primaryLine);


        }

        LD::Variant<A...> operator()() noexcept
        {
            constexpr auto NumberOfSearchableTypes = sizeof...(A);


            if (this->mSelectedType)
            {
                LD::For<NumberOfSearchableTypes>([](
                        auto I,
                        LD::UInteger selectedIndex) noexcept
                {
                    return true;
                },*this->mSelectedType);
                return {};
            }
        }
    };

    template<typename BackingStore,typename ... A> CSVParser(BackingStore & backingStore, LD::CT::TypeList<A...>) -> CSVParser<BackingStore,LD::CT::TypeList<A...>>;
}

template<>
class LD::CT::TypeDescriptor<IVRDate>
{
private:
    static constexpr auto UserName = ctll::basic_fixed_string("User");
    static constexpr auto NiceName = ctll::basic_fixed_string("Nice");
    static constexpr auto SystemName = ctll::basic_fixed_string("System");
    static constexpr auto IdleName = ctll::basic_fixed_string("Idle");
    static constexpr auto IOWaitName = ctll::basic_fixed_string("IOWait");
    static constexpr auto IRQName = ctll::basic_fixed_string("IRQ");
    static constexpr auto SoftIRQName = ctll::basic_fixed_string("SoftIRQ");
    static constexpr auto StealName = ctll::basic_fixed_string("Steal");
    static constexpr auto GuestName = ctll::basic_fixed_string("Guest");
    static constexpr auto GuestNiceName = ctll::basic_fixed_string("GuestNice");
public:
    static constexpr auto ClassName = ctll::basic_fixed_string("cpu");
    using MemberList = LD::CT::TypeList<
    >;
    static constexpr MemberList Members{  };
};

int main(int argc, char* argv[])
{

    LD::Exists

    LD::RowBackingStore store{"/home/phoenixflower/Desktop/export.csv"};

    auto line = store.GetLine();

    auto onLineError = [](const LD::Context<LD::TransactionError> &) noexcept
    {

    };

    auto onLineRow = [](const LD::Context<LD::TransactionResult,LD::StringView> & transaction) noexcept
    {
        std::cout << LD::Get(LD::Get<1>(transaction)) << std::endl;
    };

    LD::MultiMatch(LD::Overload{onLineError,onLineRow},line);
    /*
    std::ifstream inputFile{"/home/phoenixflower/Desktop/PatientListReport-3.csv"};
    std::string buffer;
    std::getline(inputFile,buffer);
    if(std::getline(inputFile,buffer))
    {
        static constexpr auto CommaSeperator = ctll::basic_fixed_string{"([^,]+)|(,,)"};
        auto matchSet = ctre::range<CommaSeperator>(LD::StringView {buffer.c_str()});
        auto it = matchSet.begin();
        auto matchSetEnd = matchSet.end();
        for(auto it = matchSet.begin();it!=matchSetEnd;++it)
        {
            std::cout << (*it).to_string() << std::endl;
        }
        std::cout << buffer << std::endl;
    }
     */
    //FILE  * file = fopen("/home/phoenixflower/Desktop/PatientListReport-3.csv","r");
    LD::MarsagliaMultiplyWithCarryGenerator randomNumberGenerator{static_cast<unsigned long>(time(0))};

    auto randomLambda = [](LD::MarsagliaMultiplyWithCarryGenerator & generator) noexcept
    {
        return generator.NextInteger();
    };

    LD::SPA::Var varType{7,randomLambda,randomNumberGenerator};
    LD::SPA::Var varType3{7,randomLambda,randomNumberGenerator};
    LD::SPA::Var varType4{7,randomLambda,randomNumberGenerator};

    auto expression = (varType+varType3)+(varType4+varType4);

    LD::SPA::EventHandler handler{LD::SPA::OnLoad{},expression};
    LD::SPA::Div gotcha2{handler,LD::SPA::Text{7}};



    std::cout << LD::ToImmutableString(expression).Data() << std::endl;

    LD::SPA::ClassName divClassName77{LD::StringView {"bomb"}};
    LD::SPA::Language domLanguage77{LD::StringView {"en-US"}};
    //LD::SPA::StyleSheet emiiter{gotcha2};
    //std::cout << emiiter.ToString().GetData() << std::endl;
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
}
