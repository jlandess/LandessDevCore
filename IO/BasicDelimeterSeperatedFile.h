//
// Created by phoenixflower on 6/27/20.
//

#ifndef LANDESSDEVCORE_BASICDELIMETERSEPERATEDFILE_H
#define LANDESSDEVCORE_BASICDELIMETERSEPERATEDFILE_H
#include "Definitions/Integer.hpp"
#include "Algorithms/CompileTimeControlFlow.hpp"
#include "FetchRequest.h"
#include "Primitives/General/StringView.hpp"
#include "Memory/ElementReference.h"
#include "Primitives/General/ctre.hpp"
#include "TypeTraits/TypeList.hpp"
#include "Reflection/reflectable.hpp"
#include "Algorithms/StringAsNumber.h"
namespace LD
{
    template<typename BackingStore,typename MembersSeperator, typename ObjectSeperator, class  = void>
    class BasicDelimeterSeperatedFile;
    template<typename BackingStore,typename MembersSeperator, typename ObjectSeperator>
    class BasicDelimeterSeperatedFile<BackingStore,MembersSeperator,ObjectSeperator,LD::Enable_If_T<LD::Require<
            LD::IsTypeString<MembersSeperator>,
            LD::IsTypeString<ObjectSeperator>
    >>>
    {
    private:

        static constexpr auto MemberPattern = ctll::fixed_string{MembersSeperator{}};
        LD::ElementReference<BackingStore> mBackingStore;

        template<typename ... Reflectables, typename ... Args>
        static LD::Enable_If_T<
                LD::Require<
                        (LD::IsReflectable<Reflectables> && ...)
                >,LD::QueryResult<LD::Variant<Reflectables...>(Args...)>> RowToObject(const LD::CT::TypeList<Reflectables...> & tl,const LD::StringView  & row,const LD::Tuple<Args...> & context) noexcept
        {
            using ResultSetType = decltype(ctre::range<MemberPattern>(LD::Declval<LD::StringView>()));
            using ResultSetIteratorType = decltype(LD::Declval<ResultSetType>().begin());


            LD::QueryResult<LD::Variant<Reflectables...>(Args...)> result;
            //result = LD::MakeContext(LD::DatabaseError{},LD::Forward<Args>(arguments)...);
            //by the time we get to this statement we can just assume it's not an IO error (as it's been loaded into the row presented in the parameters)
            //we can also assume the database has been initialized
            result = LD::QueryResult<LD::Variant<Reflectables...>(Args...)>{LD::DatabaseError{LD::DatabaseNotFoundError{}},context};
            using CurrentTypeList = LD::CT::TypeList<Reflectables...>;
            //iterate through all the types we care about at compile time
            auto onClassReanimate = [](const LD::Context<LD::StringView,LD::StringView> & context) noexcept -> bool
            {
                return true;
            };

            //LD::ContextualVariant<LD::Variant<LD::DatabaseError,LD::DatabaseTransactionResult>(LD::QueryResult<LD::Variant<Reflectables...>(Args...)>&)> assignment;
            //assignment = LD::MakeContext(LD::DatabaseError{},result);
            LD::For<sizeof...(Reflectables)>([](
                    auto I,
                    auto && row,
                    auto && onClassReanimate,
                    auto && result,
                    const LD::Context<Args...> & context)
            {


                using CurrentType = LD::CT::TypeAtIndex<I,CurrentTypeList>;
                using Var = LD::CT::RebindList<LD::CT::ReflectiveTransformation<CurrentType ,LD::AccessWriteOnly>,LD::Variant>;


                //get the types which compose the hiarchacy of the current Type we're looking at.
                //using ExpandedMembers = LD::CT::GenerateNamedReflectiveTypeStructure<decltype(""_ts),CurrentType>;

                //LD::StaticArray<int,ExpandedMembers::size()/2> membersInStringForm;
                auto currentView = row;

                LD::UInteger offset = 0;

                auto && classNameSearch = ctre::range<MemberPattern>(currentView);

                bool keepSearching = true;
                //we found the type associated with the current row
                if (classNameSearch.begin() != classNameSearch.end())
                {
                    using CurrentClassName = decltype(CurrentType::GetClassNameTypeString());

                    auto  classNameMatch = classNameSearch.begin().current_match;

                    //LD::UInteger indexOfIncidence = (classNameMatch.template get<0>().begin()-input.data());
                    offset = classNameMatch.view().size();
                    //we found the class we care about
                    auto foundClassName = LD::StringView{(const char*)classNameMatch.view().data(),classNameMatch.view().size()};
                    if(foundClassName == LD::StringView{CurrentClassName::data(),CurrentClassName::size()})
                    {
                        //we can stop searching now
                        keepSearching = false;
                        //iterate through the members to deserialize
                        auto matchSet = ctre::range<MemberPattern>(LD::StringView {row.data()+offset});
                        auto regexIterator = matchSet.begin();
                        auto onMemberReanimate = [&](const LD::ContextualVariant<Var(LD::StringView)> & context) noexcept
                        {
                            auto onPrimitiveAction = [&](auto && context) noexcept
                            {
                                using MemberType = LD::Detail::Decay_T<decltype(LD::Get(LD::Get<0>(context)))>;
                                if (regexIterator != matchSet.end())
                                {
                                    if constexpr(LD::IsPrimitive<MemberType>)
                                    {

                                        //it's a handle to the instance variable itself
                                        LD::ElementReference<MemberType> memberReference = LD::Get<0>(context);
                                        //turn the string view into a primtive (hopefully!) if not just return the default value of the primitive type (which is usually 0)
                                        auto resultVariant = LD::StringAsNumber<MemberType>(LD::StringView{(const char *)regexIterator.current_match.view().data(),regexIterator.current_match.view().size()});
                                        MemberType result = LD::Match(resultVariant,[](const MemberType & obj){ return obj;},[](auto &&){ return MemberType {};});
                                        LD::Get(memberReference) = result;
                                        //std::cout << "Member : " << regexIterator.current_match.view() << std::endl;


                                    }
                                    ++regexIterator;

                                }
                            };

                            LD::Match(context,onPrimitiveAction);
                            //using MemberType = LD::Detail::Decay_T<decltype(LD::Get(LD::Get<0>(context)))>;
                            //does our current model have more members that are actually in the file? probably shouldn't try to deserialize what's not there
                            return true;
                        };

                        //walk through the object's hiarchachy, the following function will call the callbacks on events like finding a new class or member
                        CurrentType typeToDeserialize;
                        //we're writing to the object, use the write only transform optimizations
                        LD::CT::ReflectiveWalk(""_ts,typeToDeserialize,onClassReanimate,onMemberReanimate,LD::AccessWriteOnly{});

                        //result = LD::MakeContext(LD::DatabaseTransactionResult{},CurrentType{typeToDeserialize},LD::Forward<Args>(arguments)...);
                        result = LD::QueryResult<LD::Variant<Reflectables...>(Args...)>{LD::DatabaseTransactionResult{},CurrentType {typeToDeserialize},context};

                    }
                }
                return keepSearching;
            },row,onClassReanimate,result,context);
            return result;
        }
    public:

        template<typename Object>
        class Iterator;
        template<typename ... Reflectables, typename ... Context>
        class Iterator<LD::Variant<Reflectables...>(Context...)>
        {
        private:
            LD::QueryResult<LD::Variant<Reflectables...>(Context...)> mContext;
            LD::Context<Context...> mPassableContext;
            LD::ElementReference<BasicDelimeterSeperatedFile> mInstance;
        public:
            Iterator(const LD::ElementReference<BasicDelimeterSeperatedFile> & instance, Context && ... context) noexcept :mInstance{instance},mPassableContext{LD::Forward<Context>(context)...}
            {

                using CurrentTypeList = LD::CT::TypeList<Reflectables...>;
                this->mContext = LD::MakeContext(LD::DatabaseError{},LD::Forward<Context>(context)...);
                LD::QueryResult<LD::StringView(LD::Context<Context...>&)> possibleRow = this->mInstance->mBackingStore->GetLine(this->mPassableContext);
                auto onError = [](const LD::Context<LD::DatabaseError,LD::Context<Context...>&> & context) noexcept
                {
                    LD::Context<Context...> & passableContext = LD::Get(LD::Get<1>(context));
                    return LD::QueryResult<LD::Variant<Reflectables...>(Context...)>{LD::DatabaseError{},passableContext};
                };


                auto onRow = [](const LD::Context<LD::DatabaseTransactionResult,LD::StringView,LD::Context<Context...>&> & context) noexcept
                {
                    LD::StringView row = LD::Get(LD::Get<1>(context));
                    //std::cout << row;
                    LD::Context<Context...> & passableContext = LD::Get(LD::Get<2>(context));
                    return RowToObject(LD::CT::TypeList<Reflectables...>{},row,passableContext);
                };
                this->mContext = LD::Match(possibleRow,onError,onRow);

            }
            constexpr Iterator(Context && ... context) noexcept :mInstance{nullptr},mPassableContext{LD::MakeContext(LD::Forward<Context>(context)...)}
            {
                this->mContext = LD::MakeContext(LD::DatabaseError{},LD::Forward<Context>(context)...);
            }

            Iterator & operator ++ () noexcept
            {
                LD::QueryResult<LD::StringView(LD::Context<Context...>&)> possibleRow = this->mInstance->mBackingStore->GetLine(this->mPassableContext);
                auto onError = [](const LD::Context<LD::DatabaseError,LD::Context<Context...>&> & context) noexcept
                {
                    LD::Context<Context...> & passableContext = LD::Get(LD::Get<1>(context));
                    return LD::QueryResult<LD::Variant<Reflectables...>(Context...)>{LD::DatabaseError{},passableContext};
                };
                auto onRow = [](const LD::Context<LD::DatabaseTransactionResult,LD::StringView,LD::Context<Context...>&> & context) noexcept
                {
                    const LD::StringView & row = LD::Get(LD::Get<1>(context));
                    //std::cout << row << std::endl;
                    LD::Context<Context...> & passableContext = LD::Get(LD::Get<2>(context));
                    return RowToObject(LD::CT::TypeList<Reflectables...>{},row,passableContext);
                };

                this->mContext = LD::Match(possibleRow,onError,onRow);
                return (*this);
            }


            /*
            Iterator & operator ++ (int) noexcept
            {
                LD::QueryResult<LD::StringView(LD::Context<Context...>&)> possibleRow = this->mInstance->mBackingStore->GetLine(this->mPassableContext);
                auto onError = [](const LD::Context<LD::DatabaseError,LD::Context<Context...>&> & context) noexcept
                {
                    LD::Context<Context...> & passableContext = LD::Get(LD::Get<1>(context));
                    return LD::QueryResult<LD::Variant<Reflectables...>(Context...)>{LD::DatabaseError{},passableContext};
                };
                auto onRow = [](const LD::Context<LD::DatabaseTransactionResult,LD::StringView,LD::Context<Context...>&> & context) noexcept
                {
                    LD::StringView row = LD::Get(LD::Get<1>(context));
                    //std::cout << row << std::endl;
                    LD::Context<Context...> & passableContext = LD::Get(LD::Get<2>(context));
                    return RowToObject(LD::CT::TypeList<Reflectables...>{},row,passableContext);
                };
                this->mContext = LD::Match(possibleRow,onError,onRow);
                return (*this);
            }
             */


            constexpr bool operator != (const Iterator & it) noexcept
            {
                auto isValid = [](auto && currentContext) noexcept
                {
                    using ContextType = decltype(currentContext);
                    if(LD::IsTransactionalContext(currentContext))
                    {

                        return true;
                    }

                    return LD::IsNotFoundDatabaseError(currentContext);
                };
                return LD::Match(this->mContext,isValid);;
            }
            const LD::QueryResult<LD::Variant<Reflectables...>(Context...)> & operator*() const noexcept
            {
                return this->mContext;
            }
        };
        BasicDelimeterSeperatedFile(const LD::ElementReference<BackingStore> & backingStore) noexcept :mBackingStore{backingStore}
        {

        }


        template<typename ... Reflectables, typename ... Args>
        Iterator<LD::Variant<Reflectables...>(Args...)> Begin(const LD::CT::TypeList<Reflectables...> &,Args && ... arguments) noexcept
        {
            //we're starting at the beginning so we should probably reflect that in the backing store
            this->mBackingStore->Reset();
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
                >,LD::QueryResult<LD::Variant<Reflectables...>(Args...)>> operator()(const LD::CT::TypeList<Reflectables...> & tl,const LD::UInteger & index, Args && ... arguments) noexcept
        {
            this->mBackingStore->Reset();
            auto onError = [](const LD::Context<LD::DatabaseError> & context) noexcept
            {
                //
                return LD::StringView{"",0};
            };

            auto onRow = [](const LD::Context<LD::DatabaseTransactionResult,LD::StringView> & context)
            {
                LD::StringView row = LD::Get<1>(context);
                return LD::Get<1>(context);
            };

            //get a specific row from the underlying BackingStore
            LD::StringView row = LD::Match((*this->mBackingStore)(index),onError,onRow);
            if (!row.empty())
            {
                //cool the row isn't empty, get the object
                return RowToObject(tl,row,LD::MakeContext(LD::Forward<Args>(arguments)...));
            }
            //by default it will return an error
            return {};
        }



        ~BasicDelimeterSeperatedFile() noexcept
        {
        }
    };

    template<typename BackingStore>
    using CommaSeperateValueFile = BasicDelimeterSeperatedFile<BackingStore,decltype("[^,]+"_ts),decltype("[^\n]"_ts)>;
    template<typename BackingStore>
    using SpaceSpeerateValueFile = BasicDelimeterSeperatedFile<BackingStore,decltype("[^ ]+"_ts),decltype("[^\n]"_ts)>;
}
#endif //LANDESSDEVCORE_BASICDELIMETERSEPERATEDFILE_H
