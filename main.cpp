#include "Chrono/Timer.h"
#include <iostream>
#include "IO/FetchRequest.h"
#include "Examples/ReflectionDemoTypes.h"
#include "Primitives/General/Range.h"
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
#include "IO/XML/tinyxml2.h"
#include "SPA/Div.hpp"
#include "SPA/Variable.hpp"
#include "Random/MarsagliaRandomNumberGenerator.hpp"
#include "TypeTraits/CanBeAnImmutableString.hpp"
#include "REST/HTMLEmitter.hpp"
#include "Memory/Optional.h"
#include "IO/RowBackingStore.h"
#include "Reflection/Reflection.hpp"
#include "Primitives/General/DateTime.h"
#include "VirtuWorks/QueueLog.hpp"
#include "VirtuWorks/VirtuWorksDate.hpp"
#include "Algorithms/FromString.hpp"





namespace LD
{
    template<typename BackingStore, typename Types>
    class CSVParser;
    template<typename BackingStore, typename ... A>
    class CSVParser<BackingStore,LD::CT::TypeList<A...>>
    {
    private:

        //template<typename T, typename ... B>
        //using FromString = decltype(LD::FromString(LD::Declval<LD::Type<T>>(),LD::Declval<B>()...));
        LD::Ref<BackingStore> mBackingStore;
        LD::Optional<LD::UInteger> mSelectedType;
        static constexpr auto Regex = ctll::basic_fixed_string{"([^,]+)|(,,)"};
        static constexpr auto NotSpaceRegularExpression = ctll::basic_fixed_string{"[^\\s^\"]+"};
        template<typename Key>
        static auto IncorporateKey(const Key & key, LD::StaticArray<char,1024> & keyedBuffer) noexcept -> LD::QueryResult<bool()>
        {

            auto keyLength = LD::UTF8::Distance(
                    LD::UTF8::Begin(key),
                    LD::UTF8::End(key),
                    LD::ElementReference<LD::StaticArray<char,1024>>{keyedBuffer},
                    LD::ElementReference<Key>{key});

            auto onError = [](const LD::Context<LD::TransactionError,LD::StaticArray<char,1024> &,LD::ElementReference<Key>> & context) noexcept-> LD::QueryResult<bool()>
            {
                return LD::MakeContext(LD::TransactionError{});
            };

            auto onLength = [](const LD::Context<LD::TransactionResult,LD::UInteger,LD::StaticArray<char,1024> &,LD::ElementReference<Key>> & context) noexcept -> LD::QueryResult<bool()>
            {

                auto length = LD::Get(LD::Get<1>(context));

                LD::StaticArray<char,1024>  & bufferToUse = LD::Get(LD::Get<2>(context));

                const auto & key = LD::Get(LD::Get<3>(context));

                LD::BackInserter<LD::StaticArray<char,1024>> backInserterToUse{bufferToUse};
                //backInserterToUse = 'a';
                using IT = LD::Detail::Decay_T<decltype(backInserterToUse)>;

                LD::QueryResult<IT()> unicodeAppendingState = LD::MakeContext(LD::TransactionResult{},IT{backInserterToUse});

                for(auto i = 0;i<length && LD::IsTransactionalQuery(unicodeAppendingState);++i)
                {
                    unicodeAppendingState = LD::UTF8::Append(key[i],backInserterToUse);
                }


                unicodeAppendingState = LD::UTF8::Append('\0',backInserterToUse);
                auto onTransaction = [](const LD::Context<LD::TransactionResult,IT> & ) noexcept -> LD::QueryResult<bool()>
                {
                    return LD::MakeContext(LD::TransactionResult{},true);
                };

                auto onError = [](const LD::Context<LD::TransactionError> &) noexcept -> LD::QueryResult<bool()>
                {
                    return LD::MakeContext(LD::TransactionError{});
                };
                return LD::Match(unicodeAppendingState,onTransaction,onError);
            };

            return LD::Match(keyLength,onError,onLength);
        }
    public:
        CSVParser(LD::Ref<BackingStore> backingStore, LD::CT::TypeList<A...> ) noexcept:mBackingStore{backingStore}
        {
            //validation
            constexpr auto NumberOfSearchableTypes = sizeof...(A);


            //auto primaryLine1 = this->mBackingStore->GetLine(this->mSelectedType);
            auto primaryLine = this->mBackingStore->GetLine(this->mSelectedType);

            auto onError = [](const LD::Context<LD::TransactionError,LD::Optional<LD::UInteger>&> & error) noexcept
            {

            };

            auto onHeader = [](const LD::Context<LD::TransactionResult,LD::StringView,LD::Optional<LD::UInteger>&> & transaction) noexcept
            {
                auto & selectedType = LD::Get(LD::Get<2>(transaction));
                auto row = LD::Get(LD::Get<1>(transaction));
                std::cout << row << std::endl;
                auto matchSet = ctre::range<Regex>(LD::StringView {row.data(),row.size()});
                LD::For<NumberOfSearchableTypes>([](
                        auto I,
                        LD::Optional<LD::UInteger> & selectedIndex,
                        LD::StringView row,
                        auto matchSet) noexcept
                {

                    constexpr auto traits = LD::CT::Reflect<LD::CT::TypeAtIndex<I,LD::CT::TypeList<A...>>>();
                    constexpr auto memberTraits = LD::CT::GetMemberDescriptors(traits);
                    constexpr auto numberOfMembers = LD::CT::GetNumberOfMembers(traits);
                    //auto matchSet = ctre::range<Regex>(LD::StringView {row.data(),row.size()});

                    auto it = matchSet.begin();
                    bool doesHeaderMatch = false;
                    LD::For<numberOfMembers>([](
                            auto I,
                            auto memberTraits,
                            LD::StringView row,
                            auto matchSet,
                            auto &matchIT,
                            bool & doesHeaderMatch) noexcept
                    {
                        constexpr auto currentTrait = LD::Get<I>(memberTraits);
                        constexpr auto memberName = LD::CT::GetMemberDescriptorName(currentTrait);
                        LD::StaticArray<char,1024> buffer;
                        IncorporateKey(memberName,buffer);
                        auto matchSetEnd = matchSet.end();
                        //bool shouldContinue = true;
                        if (matchIT != matchSetEnd)
                        {

                            auto keyLength = LD::UTF8::Distance(
                                    LD::UTF8::Begin(memberName),
                                    LD::UTF8::End(memberName),memberName,matchIT);
                            auto tokenLength = LD::UTF8::Distance(LD::UTF8::Begin((*matchIT).view()),LD::UTF8::End((*matchIT).view()),memberName,matchIT);

                            auto onError = [](auto,auto) noexcept
                            {
                                return true;
                            };

                            auto onLength = [](const LD::Context<LD::TransactionResult,LD::UInteger ,decltype(memberName)&,decltype(matchIT)&> & keyContext,const LD::Context<LD::TransactionResult,LD::UInteger ,decltype(memberName)&,decltype(matchIT)&> & tokenContext) noexcept
                            {

                                auto keyLength = LD::Get(LD::Get<1>(keyContext));
                                auto tokenLength = LD::Get(LD::Get<1>(tokenContext));
                                const auto & memberName = LD::Get(LD::Get<2>(keyContext));
                                const auto token = LD::Get(LD::Get<3>(keyContext));
                                bool didCSVHeaderMatch = false;
                                if (tokenLength == keyLength)
                                {
                                    LD::StaticArray<char,1024> encodedClassName;
                                    LD::BackInserter<LD::StaticArray<char,1024>> encodedBackInserter{encodedClassName};
                                    for(LD::UInteger n = 0;n<keyLength;++n)
                                    {
                                        LD::UTF8::Append(memberName[n],encodedBackInserter);
                                    }
                                    LD::UTF8::Append('\0',encodedBackInserter);

                                    LD::QueryResult<uint32_t()> memberIT = LD::MakeContext(LD::TransactionResult{},uint32_t{});
                                    LD::QueryResult<uint32_t()> tokenIT = LD::MakeContext(LD::TransactionResult{},uint32_t{});
                                    bool doesTargetClassNameMatchFetchClassName = true;
                                    auto memberBeginning = LD::UTF8::Begin(encodedClassName);
                                    auto memberEnd = LD::UTF8::End(encodedClassName);
                                    auto tokenBeginning = LD::UTF8::Begin((*token).view());
                                    auto tokenEnd = LD::UTF8::End((*token).view());
                                    for(LD::UInteger n = 0;n<keyLength && doesTargetClassNameMatchFetchClassName && LD::IsTransactionalQuery(memberIT) && LD::IsTransactionalQuery(tokenIT);++n)
                                    {
                                        memberIT = LD::UTF8::Next(memberBeginning,memberEnd);
                                        tokenIT = LD::UTF8::Next(tokenBeginning,tokenEnd);
                                        auto onError = [](auto,auto) noexcept
                                        {
                                            return false;
                                        };

                                        auto onTransaction = [](const LD::Context<LD::TransactionResult,uint32_t> & context1,
                                                                const LD::Context<LD::TransactionResult,uint32_t> & context2) noexcept
                                        {
                                            return LD::Get(LD::Get<1>(context1)) == LD::Get(LD::Get<1>(context2));
                                        };

                                        doesTargetClassNameMatchFetchClassName = LD::MultiMatch(LD::Overload{onError,onTransaction},tokenIT,memberIT);

                                    }
                                    didCSVHeaderMatch = doesTargetClassNameMatchFetchClassName;
                                }
                                return didCSVHeaderMatch;
                            };


                            doesHeaderMatch =  LD::MultiMatch(LD::Overload{onError,onLength},keyLength,tokenLength);

                            ++matchIT;
                        }
                        return !doesHeaderMatch;
                    },memberTraits,row,matchSet,it,doesHeaderMatch);
                    if (doesHeaderMatch)
                    {
                        selectedIndex = LD::UInteger(I);
                    }
                    return !doesHeaderMatch;
                },selectedType,row,matchSet);
            };

            LD::MultiMatch(LD::Overload{onError,onHeader},primaryLine);
        }

        template<typename T, typename BeginningOfPattern, typename EndOfPattern>
        static void CSVBlockToObject(T & object, BeginningOfPattern && beginningOfPattern, EndOfPattern && endOfPattern) noexcept
        {

            if (beginningOfPattern != endOfPattern)
            {
                constexpr auto MetaType = LD::CT::RemoveQualifiers(LD::Type<T>{});
                if constexpr (LD::CT::IsPrimitive(MetaType))
                {
                    std::cout << "Stuff:" << (*beginningOfPattern).view() << std::endl;
                    std::cout << "Primitive type found" << std::endl;

                }else if constexpr(LD::CT::IsTuple(MetaType))
                {
                    constexpr auto Size = LD::CT::TupleSize(MetaType);
                    //iterate through the objects of the tuple
                    LD::For<Size>([](
                            auto I,
                            auto & tuple,
                            BeginningOfPattern & beginning,
                            EndOfPattern && end)
                    {
                        CSVBlockToObject(LD::Get(LD::Get<I>(tuple)),beginning++,LD::Forward<EndOfPattern>(end));
                        return true;
                    },object,beginningOfPattern,LD::Forward<EndOfPattern>(endOfPattern));
                }
                else if constexpr(LD::CT::CanBeMadeFromStringView(MetaType,LD::CT::TypeList<T&>{}))
                {
                    std::cout << "Stuff:" << (*beginningOfPattern).view() << std::endl;
                    std::cout << "Can be made from stringview" << std::endl;
                    //LD::Ref ObjectReference{object};
                    auto convertStringViewToObjectResult =  LD::FromStringView(MetaType,(*beginningOfPattern).view(),object);

                    auto onError = [](const LD::Context<LD::TransactionError,T&> & error) noexcept
                    {

                    };
                    auto onTransaction = [](const LD::Context<LD::TransactionResult,T,T&> context) noexcept
                    {
                        LD::Get(LD::Get<2>(context)) = LD::Get(LD::Get<1>(context));
                    };

                    LD::MultiMatch(LD::Overload{onError,onTransaction},convertStringViewToObjectResult);
                }
                else if constexpr(LD::CT::IsReflectable(MetaType))
                {
                    constexpr auto traits = LD::CT::Reflect(MetaType);
                    constexpr auto members = LD::CT::GetMemberDescriptors(traits);
                    constexpr auto NumberOfMembers = LD::CT::GetNumberOfMembers(traits);
                    LD::For<NumberOfMembers>([](
                            auto I,
                            auto memebers,
                            T & object,
                            BeginningOfPattern & beginning,
                            EndOfPattern && end) noexcept
                    {
                        constexpr auto memberInfo = LD::Get<I>(members);
                        auto memberAccessor = memberInfo(object);
                        CSVBlockToObject(memberAccessor(),beginning++,LD::Forward<EndOfPattern>(end));
                        return true;
                    },members,object,beginningOfPattern,LD::Forward<EndOfPattern>(endOfPattern));
                }

            }

        }

        template<typename ... B>
        LD::QueryResult<LD::Variant<A...>(B...)> operator()(B && ... arguments) noexcept
        {
            if (this->mSelectedType)
            {
                std::cout << "CSV was validated at index " << *this->mSelectedType << std::endl;
                LD::Variant<A...> objectFetch;
                auto primaryLine = this->mBackingStore->GetLine(this->mSelectedType,objectFetch);
                constexpr auto NumberOfSearchableTypes = sizeof...(A);
                auto onError = [](const LD::Context<LD::TransactionError,LD::Optional<LD::UInteger>&,LD::Variant<A...>&> & error) noexcept
                {

                };


                auto onRow = [](const LD::Context<LD::TransactionResult,LD::StringView ,LD::Optional<LD::UInteger>&,LD::Variant<A...>&> & transaction) noexcept
                {
                    auto selectionIndex = LD::Get(LD::Get<2>(transaction));
                    auto & object = LD::Get(LD::Get<3>(transaction));
                    auto row = LD::Get(LD::Get<1>(transaction));
                    auto matchSet = ctre::range<Regex>(LD::StringView {row.data(),row.size()});
                    for(auto it = matchSet.begin();it!=matchSet.end();++it)
                    {
                        //std::cout << (*it).view() << std::endl;
                    }

                    LD::For<NumberOfSearchableTypes>([](
                            auto I,
                            LD::UInteger selectedIndex,
                            auto  it,
                            auto end,
                            LD::Variant<A...>& object)
                    {
                        if (I == selectedIndex)
                        {
                            using TypeToDeserialize = LD::CT::TypeAtIndex<I,LD::CT::TypeList<A...>>;
                            constexpr auto traits = LD::CT::Reflect(LD::Type<TypeToDeserialize>{});
                            constexpr auto members = LD::CT::GetMemberDescriptors(traits);
                            constexpr auto NumberOfMembers = LD::CT::GetNumberOfMembers(traits);
                            TypeToDeserialize object;
                            LD::For<NumberOfMembers>([](
                                    auto I,
                                    decltype(it) & it,
                                    auto end,
                                    auto members,
                                    TypeToDeserialize & object)
                            {
                                constexpr auto memberInfo = LD::Get<I>(members);
                                auto memberAccessor = memberInfo(object);
                                constexpr auto MemberType = LD::CT::RemoveQualifiers(LD::CT::GetDescriptorType(memberInfo));
                                if (it != end)
                                {
                                    std::cout << "view : " << (*it).view() << std::endl;
                                    if ((*it).view() != ",,")
                                    {
                                        //std::cout << "Index: " << I << std::endl;
                                        auto matchSet = ctre::range<NotSpaceRegularExpression>(LD::StringView {(*it).view().data(),(*it).view().size()});
                                        //once we have an object (something which is found between the commas, we can now start to de-serialize it)
                                        CSVBlockToObject(memberAccessor(),matchSet.begin(),matchSet.end());
                                    }

                                    ++it;
                                }
                                return true;
                            },it,end,members,object);

                        }
                        return true;
                    },*selectionIndex,matchSet.begin(),matchSet.end(),object);
                };
                LD::MultiMatch(LD::Overload{onError,onRow},primaryLine);
            }
            return {};
        }
    };

    template<typename BackingStore,typename ... A> CSVParser(BackingStore & backingStore, LD::CT::TypeList<A...>) -> CSVParser<BackingStore,LD::CT::TypeList<A...>>;
}


static constexpr auto  D = ctll::basic_fixed_string{"(\\d{4})-(\\d{1,2})-(\\d{1,2})"};
static constexpr auto  F = ctll::basic_fixed_string("(\\d{4})-(\\d{1,2})-(\\d{1,2})");

static constexpr auto  NotSpaceRegularExpression = ctll::basic_fixed_string("[^\\s]+");
template<typename ... A>
LD::QueryResult<LD::VW::Date()> FromStringView(LD::StringView view, A && ... args) noexcept
{
    if (auto [whole, year, month, day] = ctre::match<D>("2020-11-17"); whole)
    {
        LD::StringToPrimitive<LD::UInteger > converter{};
        LD::UInteger yearAsNumber = LD::Match(converter(year),[](auto){ return LD::UInteger {0};},[](const LD::UInteger & obj){return obj;});
        LD::UInteger monthAsNumber = LD::Match(converter(month),[](auto){ return LD::UInteger {0};},[](const LD::UInteger & obj){return obj;});
        LD::UInteger dayAsNumber = LD::Match(converter(day),[](auto){ return LD::UInteger {0};},[](const LD::UInteger & obj){return obj;});
        std::cout << yearAsNumber << std::endl;
        std::cout << monthAsNumber << std::endl;
        std::cout << dayAsNumber << std::endl;
        return LD::MakeContext(LD::TransactionResult{},LD::VW::Date{LD::Date{dayAsNumber,monthAsNumber,yearAsNumber}},LD::Forward<A>(args)...);
    }
    return LD::MakeContext(LD::TransactionError{},LD::Forward<A>(args)...);
}

LD::StringView ProcessRow(LD::StringView data) noexcept
{
    static constexpr auto CommaExpression = ctll::basic_fixed_string{"[,]+{2,}"};
    auto matchSet = ctre::range<NotSpaceRegularExpression>(LD::StringView {data.data(),data.size()});
    return data;
}

int main(int argc, char* argv[])
{
    ProcessRow(LD::StringView{"\"2020-11-17 07:20:15\",\"English Queue\",,LOGIN,101,,,,5fb3bfff602d5"});
    auto stringToNumber = LD::StringToNumber(LD::Type<int>{},LD::StringView {"253"});

    auto stringToNumberError = [](const LD::Context<LD::TransactionError> & ) noexcept
    {

    };

    auto stringToNumberResult = [](const LD::Context<LD::TransactionResult,int> & result) noexcept
    {
        std::cout << LD::Get(LD::Get<1>(result)) << std::endl;
    };


    LD::MultiMatch(LD::Overload{stringToNumberError,stringToNumberResult},stringToNumber);


    auto fromStringResult = LD::FromStringView(LD::Type<int>{},LD::StringView{"55"});


    auto StringResultError = [](const LD::Context<LD::TransactionError> &) noexcept
    {

    };

    auto StringResultViewResult = [](const LD::Context<LD::TransactionResult,int> & result) noexcept
    {
        std::cout << LD::Get(LD::Get<1>(result)) << std::endl;
    };
    LD::MultiMatch(LD::Overload{StringResultError,StringResultViewResult},fromStringResult);
    LD::StringView sampleData{"2020-11-17 07:20:15"};
    auto matchSet = ctre::range<NotSpaceRegularExpression>(LD::StringView {sampleData.data(),sampleData.size()});

    for(auto it = matchSet.begin();it!=matchSet.end();++it)
    {
        std::cout << (*it).view() << std::endl;
    }
    FromStringView(LD::StringView{"2020-11-17 07:20:15"});
    LD::RowBackingStore store{"/home/phoenixflower/Documents/DailyQueue.txt"};

    LD::CSVParser csvParser{store,LD::CT::TypeList<LD::VW::QueueLog>{}};
    csvParser();

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
