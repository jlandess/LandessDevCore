#include "Examples/TermBoxMenuExample.hpp"
#include "Examples/ReflectionExample.hpp"
#include "Algorithms/StringAsNumber.h"
#include "Chrono/Timer.h"
#include "Reflection/reflectable.hpp"
#include "Primitives/General/Immutable.hpp"
#include "TypeTraits/IsArray.hpp"
#include "Primitives/General/ContextualVariant.h"
#include <iostream>
#include "Primitives/General/Span.hpp"
#include "IO/FetchRequest.h"
#include "Examples/ReflectionDemoTypes.h"
#include <string_view>
template<typename MembersSeperator, typename ObjectSeperator, class  = void>
class BasicDelimeterSeperatedFile;
template<typename MembersSeperator, typename ObjectSeperator>
class BasicDelimeterSeperatedFile<MembersSeperator,ObjectSeperator,LD::Enable_If_T<LD::Require<
        LD::IsTypeString<MembersSeperator>,
        LD::IsTypeString<ObjectSeperator>
        >>>
{
private:

    static constexpr auto MemberPattern = ctll::fixed_string{MembersSeperator{}};
    FILE * mHandle;
    char * mLineBuffer;
    size_t mLineBufferSize;
public:

    template<typename Object>
    class Iterator;
    template<typename ... Reflectables, typename ... Context>
    class Iterator<LD::Variant<Reflectables...>(Context...)>
    {
    private:
        LD::QueryResult<LD::Variant<Reflectables...>(Context...)> mContext;
        LD::UInteger mLineBufferSize;
        LD::ElementReference<BasicDelimeterSeperatedFile> mInstance;
    public:
        constexpr Iterator(const LD::ElementReference<BasicDelimeterSeperatedFile> & instance, Context && ... context) noexcept :mLineBufferSize{0},mInstance{instance}
        {

            using CurrentTypeList = LD::CT::TypeList<Reflectables...>;
            LD::For<sizeof...(Reflectables)>([](auto I)
            {
                using CurrentType = LD::CT::TypeAtIndex<I,CurrentTypeList>;

                return true;
            });
        }
        constexpr Iterator(Context && ... context) noexcept :mLineBufferSize{0},mInstance{nullptr}
        {

            using CurrentTypeList = LD::CT::TypeList<Reflectables...>;
            LD::For<sizeof...(Reflectables)>([](auto I)
            {
                  using CurrentType = LD::CT::TypeAtIndex<I,CurrentTypeList>;


                  return true;

            });
        }

        constexpr bool operator != (const Iterator & it) noexcept
        {
            return this->mLineBufferSize != 0 ||it.mInstance == nullptr;
        }
        const LD::QueryResult<LD::Variant<Reflectables...>(Context...)> & operator*() const noexcept
        {
            return this->mContext;
        }

        template<typename ... Args>
        LD::QueryResult<LD::Variant<Reflectables...>(Args...)> operator()(Args && ... arguments) noexcept
        {
            return {};
        }
    };
    BasicDelimeterSeperatedFile() noexcept :mLineBuffer(nullptr)
    {
        mHandle = fopen("/proc/stat","r");
        //getline(&mLineBuffer,&mLineBufferSize,this->mHandle);

        //printf("%s \n",mLineBuffer);
    }


    template<typename ... Reflectables, typename ... Args>
    Iterator<LD::Variant<Reflectables...>(Args...)> Begin(const LD::CT::TypeList<Reflectables...> &,Args && ... arguments) noexcept
    {
        return Iterator<LD::Variant<Reflectables...>(Args...)>{LD::ElementReference<BasicDelimeterSeperatedFile>{this},LD::Forward<Args>(arguments)...};
    }

    template<typename ... Reflectables, typename ... Args>
    Iterator<LD::Variant<Reflectables...>(Args...)> End(const LD::CT::TypeList<Reflectables...> &,Args && ... arguments) noexcept
    {
        //it just works as a dummy sentinal to stop the possible loop
        return Iterator<LD::Variant<Reflectables...>(Args...)>{LD::Forward<Args>(arguments)...};
    }


    template<typename ... Reflectables, typename ... Args>
    LD::Enable_If_T<
            LD::Require<
                    (LD::IsReflectable<Reflectables> && ...)
                    >,LD::QueryResult<LD::Variant<Reflectables...>(Args...)>> operator()(const LD::CT::TypeList<Reflectables...> &,const LD::UInteger & index, Args && ... arguments) noexcept
    {
        auto cachedOffset = ftello(this->mHandle);
        fseeko(this->mHandle,0,SEEK_SET);
        LD::UInteger n = 0;
        do
        {
            getline(&mLineBuffer,&mLineBufferSize,this->mHandle);
        }while(n < index);

        auto input = LD::StringView {mLineBuffer};
        //auto match = ctre::range<MemberPattern>(input);

        using CurrentTypeList = LD::CT::TypeList<Reflectables...>;
        //iterate through all the types we care about at compile time
        auto onClassReanimate = [](const LD::Context<LD::StringView,LD::StringView> & context) noexcept -> bool
        {
            return true;
        };
        LD::For<sizeof...(Reflectables)>([](
                auto I,
                auto && input,
                auto && onClassReanimate)
        {

            using CurrentType = LD::CT::TypeAtIndex<I,CurrentTypeList>;
            using Var = LD::CT::RebindList<LD::CT::ReflectiveTransformation<CurrentType ,LD::AccessWriteOnly>,LD::Variant>;
            //get the types which compose the hiarchacy of the current Type we're looking at.
            //using ExpandedMembers = LD::CT::GenerateNamedReflectiveTypeStructure<decltype(""_ts),CurrentType>;

            //LD::StaticArray<int,ExpandedMembers::size()/2> membersInStringForm;
            auto currentView = input;

            LD::UInteger offset = 0;

            auto && classNameSearch = ctre::range<MemberPattern>(currentView);

            bool keepSearching = true;
             //we found the type associated with the current row
            if (classNameSearch.begin() != classNameSearch.end())
            {
                keepSearching = false;
                using CurrentClassName = decltype(CurrentType::GetClassNameTypeString());

                auto  classNameMatch = classNameSearch.begin().current_match;

                //LD::UInteger indexOfIncidence = (classNameMatch.template get<0>().begin()-input.data());
                offset = classNameMatch.view().size();
                //we found the class we care about
                auto foundClassName = LD::StringView{(const char*)classNameMatch.view().data(),classNameMatch.view().size()};
                if(foundClassName == LD::StringView{CurrentClassName::data(),CurrentClassName::size()})
                {
                    /*
                    //keepSearching = false;//stop the compile time for loop
                    std::cout << "Classname : " << foundClassName << std::endl;
                    auto && memberSearch = ctre::range<MemberPattern>(LD::StringView{input.data()+offset});
                    auto  currentMemberMatch = memberSearch.begin().current_match;
                    LD::UInteger indexOfIncidence = (currentMemberMatch.template get<0>().begin()-(input.data()));
                    offset += currentMemberMatch.view().size();

                    std::cout << "Offset Size : " << currentMemberMatch.view().size() << std::endl;
                    std::cout << "Member : " << LD::StringView {(const char*)currentMemberMatch.view().data(),currentMemberMatch.view().size()} << std::endl;

                    auto && memberSearch1 = ctre::range<MemberPattern>(LD::StringView{input.data()+6+3});
                    auto  currentMemberMatch1 = memberSearch1.begin().current_match;
                    indexOfIncidence = (currentMemberMatch1.template get<0>().begin()-(input.data()));
                    offset += currentMemberMatch1.view().size();

                    std::cout << "Offset Size : " << currentMemberMatch1.view().size() << std::endl;
                    std::cout << "Member : " << LD::StringView {(const char*)currentMemberMatch1.view().data(),currentMemberMatch1.view().size()} << std::endl;

                    */
                    /*
                    auto && memberSearch = ctre::range<MemberPattern>(LD::StringView {input.data()+offset});
                    if (memberSearch.begin() != memberSearch.end())
                    {
                        auto  currentMemberMatch = memberSearch.begin().current_match;
                        LD::UInteger indexOfIncidence = (currentMemberMatch.template get<0>().begin()-input.data());
                        offset = currentMemberMatch.get_end_position()-input.data();
                        std::cout << "Member : " << indexOfIncidence << std::endl;

                    }
                     */


                    auto onMemberReanimate = [](const LD::ContextualVariant<Var(LD::StringView)> & context) noexcept
                    {
                        std::cout << "member found " << std::endl;
                        return true;
                    };

                    CurrentType typeToDeserialize;
                    LD::CT::ReflectiveWalk(""_ts,typeToDeserialize,onClassReanimate,onMemberReanimate,LD::AccessWriteOnly{});

                    for (auto && match: ctre::range<MemberPattern>(LD::StringView {input.data()+offset}))
                    {
                        //LD::StringView abc = LD::StringView {match.get<0>()};
                        auto currentMatch = match.template get<0>();

                        //std::cout <<  LD::StringView {(const char*)currentMatch.view().data(),currentMatch.view().size()} << "\n";
                        //auto abc = LD::StringView{(const char*)match.get<0>()};
                        LD::UInteger indexOfIncidence = (currentMatch.begin()-input.data());
                        const char * matchBegin = (input.data()+(currentMatch.begin()-input.data()));

                        auto abc = LD::StringView{matchBegin,currentMatch.size()};
                        //std::cout << abc << "\n";stuffings
                        //abc.substr(0,0);
                        std::cout << "Beginning : " << indexOfIncidence << "," << "End : " << currentMatch.size() << std::endl;
                        std::cout << input.substr(indexOfIncidence,currentMatch.size()) << std::endl;
                    }







                }
            }
             return keepSearching;
        },input,onClassReanimate);
        fseeko(this->mHandle,cachedOffset,SEEK_SET);
        return {};
    }



    ~BasicDelimeterSeperatedFile() noexcept
    {
        fclose(mHandle);
        free(this->mLineBuffer);
    }
};

using CommaSeperateValueFile = BasicDelimeterSeperatedFile<decltype("[^,]+"_ts),decltype("[^\n]"_ts)>;
using SpaceSpeerateValueFile = BasicDelimeterSeperatedFile<decltype("[^ ]+"_ts),decltype("[^\n]"_ts)>;
int main()
{


    using stuffings = LD::CT::GenerateNamedReflectiveTypeStructure<decltype(""_ts),LD::Pyramid>;


    //LD::CT::DebugTemplate<stuffings>{};

    /*
    auto input = LD::StringView {"123,456,768"};
    static constexpr auto pattern = ctll::fixed_string{"[0-9]+"};
    auto match = ctre::range<pattern>(input);

    match.begin().current_match.get_end_position();

    for (auto match: ctre::range<pattern>(input))
    {
        //LD::StringView abc = LD::StringView {match.get<0>()};
        auto currentMatch = match.get<0>();


        currentMatch.get_end();

        //std::cout <<  LD::StringView {(const char*)currentMatch.view().data(),currentMatch.view().size()} << "\n";
        //auto abc = LD::StringView{(const char*)match.get<0>()};
        const char * indexOf = (input.data()+(currentMatch.begin()-input.data()));
        auto abc = LD::StringView{indexOf,currentMatch.size()};
        std::cout << abc << "\n";
    }

     */
    SpaceSpeerateValueFile file;

    file(LD::CT::TypeList<LD::CPUPackageMetric>{},0);
    //auto bar = LD::ImmutableString<9>{'_'};
    //auto edge = LD::ToImmutableString("abc");
    //std::cout << (bar+edge).Data() << std::endl;
    //std::cout << edge.Data() << std::endl;
    //std::cout << bar.GetSize() << std::endl;
    //std::cout << edge.GetSize() << std::endl;
    //LD::Example::TermBoxMenuExample();
    LD::Timer currentTimer;
    currentTimer.Start();

    //LD::Example::ReflectionExample();
    //LD::Example::IMGUITUIExample();
    currentTimer.Stop();

    //std::cout << "Execution Time: " << currentTimer.Time()/1.0_us<< std::endl;
    return 0;
}