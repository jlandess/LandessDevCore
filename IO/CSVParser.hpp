//
// Created by phoenixflower on 11/25/20.
//

#ifndef LANDESSDEVCORE_CSVPARSER_HPP
#define LANDESSDEVCORE_CSVPARSER_HPP
#include "TypeTraits/CanBeMadeFromStringView.hpp"
#include "TypeTraits/TypeList.hpp"
#include "Primitives/General/ctre.hpp"
#include "Primitives/General/StringView.hpp"
#include "Unicode/UTF8.hpp"
#include "Memory/Optional.h"
#include "Reflection/Reflection.hpp"
#include "Algorithms/FromString.hpp"
#include <iostream>
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
        //static constexpr auto Regex = ctll::basic_fixed_string{"([^,]+)|(,,)"};
        static constexpr auto Regex = ctll::basic_fixed_string{"([^,^(\0)]+)|(\0)"};
        static constexpr auto NotSpaceRegularExpression = ctll::basic_fixed_string{"[^\\s]+"};
        static constexpr auto CommaExpression = ctll::basic_fixed_string{"([,]{2,}+)"};



        class CSVParserBlockView
        {
        private:
            LD::StringView mView;
        public:
            CSVParserBlockView(LD::StringView view) noexcept:mView{view}
            {

            }

            LD::StringView view() const noexcept
            {
                return this->mView;
            }
        };
        class CSVParserBlock
        {
        private:
            LD::StringView mView;
        public:
            class Iterator
            {
            private:
                LD::StringView mView;
                LD::UInteger mIndex;
            public:
                Iterator(LD::StringView view, LD::UInteger index) noexcept:mView{view},mIndex{index}
                {

                }

                bool operator == (const CSVParser::CSVParserBlock::Iterator it) const noexcept
                {
                    return this->mIndex == it.mIndex;
                }

                bool operator != (const CSVParser::CSVParserBlock::Iterator it) const noexcept
                {
                    return this->mIndex != it.mIndex;
                }
                CSVParser::CSVParserBlock::Iterator & operator++() noexcept
                {
                    this->mIndex++;
                    return (*this);
                }

                CSVParser::CSVParserBlock::Iterator & operator++(int) noexcept
                {
                    this->mIndex++;
                    return (*this);
                }

                CSVParser::CSVParserBlockView operator*() const noexcept
                {
                    return CSVParser::CSVParserBlockView{this->mView};
                }
            };

            CSVParserBlock(LD::StringView view) noexcept:mView{view}
            {

            }

            CSVParser::CSVParserBlock::Iterator begin() const noexcept
            {
                return CSVParser::CSVParserBlock::Iterator{this->mView,0};
            }

            CSVParser::CSVParserBlock::Iterator end() const noexcept
            {
                return CSVParser::CSVParserBlock::Iterator{this->mView,1};
            }
        };
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

        static LD::StringView ProcessRow(LD::StringView data) noexcept
        {
            std::cout << "Row : " << data << std::endl;
            auto leftShift = [](char* words, int len) noexcept
            {
                int i;
                for(i = 1; i < len; i++)
                {
                    words[i - 1] = words[i];
                }
                len--;
                return len;
            };

            //auto matchSet = ctre::range<CommaExpression>(LD::StringView {data.data(),data.size()});
            //LD::StringView pattern{"zq"};
            //auto it = matchSet.begin();

            bool attempt = true;
            do {
                auto matchSet123 = ctre::range<CommaExpression>(LD::StringView {data.data(),data.size()});
                attempt = (matchSet123.begin() != matchSet123.end());
                if (attempt)
                {
                    auto it = matchSet123.begin();
                    char * view = (char*)data.data();
                    LD::UInteger numberOfReplacements = (*it).view().size()-1;
                    LD::UInteger offset = (it.current-data.begin())-((*it).view().size());
                    for(LD::UInteger n = 0;n<(*it).view().size()-1;++n)
                    {
                        view[offset+n] = '\0';
                    }
                    char * location = (view+offset+numberOfReplacements);
                    LD::UInteger subSize = (data.end()-(view+offset+numberOfReplacements))-1;
                    leftShift(location,subSize);
                }
            }while(attempt);
            return data;
        }
    public:
        template<typename T>
        class Iterator;
        template<typename ...T, typename ... Args>
        class Iterator<LD::Variant<T...>(Args...)>
        {
        private:
            LD::QueryResult<LD::Variant<T...>(Args...)> mContext;
            LD::Context<Args...> mPassableContext;
            LD::ElementReference<CSVParser> mInstance;
        public:

            Iterator(Args && ... arguments) noexcept :mInstance{nullptr},mPassableContext{LD::MakeContext(LD::Forward<Args>(arguments)...)}
            {
                this->mContext = LD::MakeContext(LD::TransactionError{},LD::Forward<Args>(arguments)...);
            }

            Iterator(const LD::ElementReference<CSVParser> & instance, Args && ... arguments) noexcept:mInstance{instance},mPassableContext{LD::MakeContext(LD::Forward<Args>(arguments)...)}
            {
                //this->mPassableContext = LD::MakeContext(LD::Forward<Args>(arguments)...);
                ++(*this);
            }

            constexpr bool operator != (const Iterator & it) noexcept
            {
                auto isValid = [](auto && currentContext) noexcept
                {
                    using ContextType = decltype(currentContext);

                    if(LD::IsTransactionalContext(currentContext))
                    {
                        return true;
                    }
                    return LD::DoesContextContainError(currentContext,LD::Type<LD::NotFoundError>{});
                };
                return LD::Match(this->mContext,isValid);;
            }
            Iterator & operator ++ (int) noexcept
            {
                return ++(*this);
            }

            Iterator & operator ++ () noexcept
            {
                LD::QueryResult<LD::Variant<T...>(LD::Context<Args...>&)> possibleRow = (*this->mInstance)(this->mPassableContext);
                auto onRow = [](const auto & transactionalContext) noexcept //-> LD::Enable_If_T<LD::Require<LD::IsTransactionalContext(LD::Declval<LD::Detail::Decay_T<decltype(transactionalContext)>>())>,LD::QueryResult<LD::Variant<T...>(Args...)>>
                {
                    LD::IsTransactionalContext(transactionalContext);
                    //LD::IsTransactionalQuery(LD::Declval<LD::Detail::Decay_T<decltype(transactionalContext)>>());
                    //constexpr auto CurrentTypeList = LD::GetContextTypeList(LD::Type<LD::Detail::Decay_T<decltype(transactionalContext)>>{});
                    using FetchedType = LD::Detail::Decay_T<decltype(LD::Get(LD::Get<1>(transactionalContext)))>;

                    return LD::QueryResult<LD::Variant<T...>(Args...)>{LD::TransactionResult{},FetchedType{LD::Get(LD::Get<1>(transactionalContext))},LD::Get(LD::Get<2>(transactionalContext))};
                        //return LD::QueryResult<LD::Variant<T...>(Args...)>{LD::TransactionResult{},FetchedType{LD::Get(LD::Get<1>(transactionalContext))},LD::Get(LD::Get<2>(transactionalContext))};
                    return LD::QueryResult<LD::Variant<T...>(Args...)>{LD::TransactionError{},LD::Get(LD::Get<2>(transactionalContext))};
                };
                auto onError = [](const LD::Context<LD::TransactionError,LD::Context<Args...>&> & error) noexcept
                {
                    //printf("error found\n");
                    //LD::IsNotFoundDatabaseError(error);
                    return LD::QueryResult<LD::Variant<T...>(Args...)>{LD::TransactionError{},LD::Get<1>(error)};
                };
                this->mContext = LD::Match(possibleRow,onRow,onError);
                return (*this);
            }
            const LD::QueryResult<LD::Variant<T...>(Args...)> & operator*() const noexcept
            {
                return this->mContext;
            }
        };


        template<typename ... Args>
        LD::Enable_If_T<LD::Require<LD::CT::IsReflectable<A>()...>,CSVParser::Iterator<LD::Variant<A...>(Args...)>> Begin(Args && ... arguments) noexcept
        {
            //we're starting at the beginning so we should probably reflect that in the backing store
            this->mBackingStore->Reset();
            return CSVParser::Iterator<LD::Variant<A...>(Args...)>{LD::ElementReference<CSVParser>{this},LD::Forward<Args>(arguments)...}++;
        }

        template<typename ... Args>
        Iterator<LD::Variant<A...>(Args...)> End(Args && ... arguments) noexcept
        {
            //it just works as a dummy sentinal to stop the possible loop
            return Iterator<LD::Variant<A...>(Args...)>{LD::Forward<Args>(arguments)...};
        }
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
                //std::cout << row << std::endl;
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


                                        auto onTransaction = [](
                                                const LD::Context<LD::TransactionResult,uint32_t> & context1,
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
                constexpr auto MetaType = LD::CT::RemoveQualifiers(LD::Type<LD::Detail::Decay_T<T>>{});
                if constexpr (LD::CT::IsPrimitive(MetaType))
                {
                    //std::cout << "Stuff:" << (*beginningOfPattern).view() << std::endl;
                    auto ToPrimitive = LD::FromStringView(MetaType,(*beginningOfPattern).view(),object);

                    auto onError = [](const LD::Context<LD::TransactionError,T&> & context) noexcept
                    {

                    };

                    auto onConversion = [](const LD::Context<LD::TransactionResult,T,T&> & context) noexcept
                    {
                        LD::Get(LD::Get<2>(context)) = LD::Get(LD::Get<1>(context));
                    };

                    LD::MultiMatch(LD::Overload{onError,onConversion},ToPrimitive);
                    //std::cout << "Primitive type found" << std::endl;

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
                        CSVBlockToObject(LD::Get(LD::Get<I>(tuple)),beginning,LD::Forward<EndOfPattern>(end));
                        beginning++;
                        return true;
                    },object,beginningOfPattern,LD::Forward<EndOfPattern>(endOfPattern));
                }
                else if constexpr(LD::CT::CanBeMadeFromStringView(MetaType,LD::CT::TypeList<T&>{}))
                {
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
                }else
                {
                    std::cout << "Stuff is confused:" << (*beginningOfPattern).view() << std::endl;
                }

            }
        }

        template<typename ... B>
        LD::QueryResult<LD::Variant<A...>(B...)> operator()(B && ... arguments) noexcept
        {
            LD::QueryResult<LD::Variant<A...>(B...)> result = LD::MakeContext(LD::TransactionError{},LD::Forward<B>(arguments)...);
            if (this->mSelectedType)
            {
                LD::Variant<A...> objectFetch;
                auto passableContext = LD::MakeContext(LD::Forward<B>(arguments)...);
                auto primaryLine = this->mBackingStore->GetLine(this->mSelectedType,objectFetch,passableContext,result);
                constexpr auto NumberOfSearchableTypes = sizeof...(A);

                auto onError = [](const LD::Context<LD::TransactionError,LD::Optional<LD::UInteger>&,LD::Variant<A...>&,decltype(passableContext)&,decltype(result)&> & error) noexcept
                {

                };


                auto onRow = [](const LD::Context<LD::TransactionResult,LD::StringView ,LD::Optional<LD::UInteger>&,LD::Variant<A...>&,decltype(passableContext)&,decltype(result)&> & transaction) noexcept
                {
                    auto selectionIndex = LD::Get(LD::Get<2>(transaction));
                    auto & object = LD::Get(LD::Get<3>(transaction));
                    auto row = LD::Get(LD::Get<1>(transaction));
                    ProcessRow(LD::StringView{row.data(),row.size()});
                    auto matchSet = ctre::range<Regex>(LD::StringView {row.data(),row.size()});
                    for(auto it = matchSet.begin();it!=matchSet.end();++it)
                    {
                        //std::cout << "view inside onRow: " << (*it).view() << std::endl;
                    }

                    LD::For<NumberOfSearchableTypes>([](
                            auto I,
                            LD::UInteger selectedIndex,
                            auto  it,
                            auto end,
                            LD::Variant<A...>& object,
                            decltype(passableContext) & context,
                            decltype(result)& result)
                    {
                        bool shouldContinue = true;
                        if (I == selectedIndex)
                        {
                            shouldContinue = false;
                            using TypeToDeserialize = LD::CT::TypeAtIndex<I,LD::CT::TypeList<A...>>;

                            constexpr auto traits = LD::CT::Reflect(LD::Type<TypeToDeserialize>{});

                            constexpr auto members = LD::CT::GetMemberDescriptors(traits);

                            constexpr auto NumberOfMembers = LD::CT::GetNumberOfMembers(traits);

                            TypeToDeserialize object;
                            //std::cout << "Beginning" << std::endl;
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

                                    //std::cout << "view : " << (*it).view() << std::endl;
                                    if ((*it).view().size() > 0 && (*it).view()[0] != '\0')
                                    {

                                        //if it's an array, tuple or some traversable structure we want to create even more tokens with a regular expression
                                        //std::cout << "Index: " << I << std::endl;


                                        //std::cout << "Row : " << (*it).view() << std::endl;
                                        //once we have an object (something which is found between the commas, we can now start to de-serialize it)
                                        if constexpr(LD::CT::IsTuple(MemberType))
                                        {

                                            auto matchSet = ctre::range<NotSpaceRegularExpression>(LD::StringView {(*it).view().data(),(*it).view().size()});

                                            CSVBlockToObject(memberAccessor(),matchSet.begin(),matchSet.end());


                                        }else
                                        {

                                            CSVParser::CSVParserBlock block{LD::StringView {(*it).view().data(),(*it).view().size()}};

                                            CSVBlockToObject(memberAccessor(),block.begin(),block.end());

                                        }
                                    }
                                    ++it;

                                }

                                return true;

                            },it,end,members,object);
                            result = LD::QueryResult<LD::Variant<A...>(B...)>{LD::TransactionResult{},TypeToDeserialize{object},context};
                        }
                        return shouldContinue;

                    },*selectionIndex,matchSet.begin(),matchSet.end(),object,LD::Get(LD::Get<4>(transaction)),LD::Get(LD::Get<5>(transaction)));
                    //std::cout << std::endl;
                };
                LD::MultiMatch(LD::Overload{onError,onRow},primaryLine);
            }
            return result;
        }
    };

    template<typename BackingStore,typename ... A> CSVParser(BackingStore & backingStore, LD::CT::TypeList<A...>) -> CSVParser<BackingStore,LD::CT::TypeList<A...>>;
}
#endif //LANDESSDEVCORE_CSVPARSER_HPP
