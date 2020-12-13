//
// Created by phoenixflower on 11/28/20.
//

#ifndef LANDESSDEVCORE_EPHEMIRALROWBACKINGSTORE_HPP
#define LANDESSDEVCORE_EPHEMIRALROWBACKINGSTORE_HPP
#include "Primitives/General/ctre.hpp"
#include <memory>
namespace LD
{
    class EphemiralBase
    {
    public:
        virtual LD::StringView NextRow() noexcept = 0;
        virtual bool CanFetchRow() const noexcept = 0;
        virtual ~EphemiralBase(){}
    };

    template<typename Match,typename Begin, typename End>
    class EphemiralIterator: public EphemiralBase
    {
    private:
        Begin mBegin;
        End mEnd;
        Match mMatch;
    public:
        EphemiralIterator(Match && match,Begin && begin, End && end):mMatch{match},mBegin{begin},mEnd{end}
        {

        }
        virtual LD::StringView NextRow() noexcept
        {
            if (this->mBegin != this->mEnd)
            {
                return LD::StringView{(*this->mBegin).view().data(),(*this->mBegin++).view().size()};
            }
            return LD::StringView{};
        }
        virtual bool CanFetchRow() const noexcept
        {
            return (this->mBegin != this->mEnd);
        }
    };

    template<typename Match,typename Begin, typename End> EphemiralIterator(Match &&,Begin &&, End &&) -> EphemiralIterator<Match,Begin,End>;
    class EphemiralRowBackingStore
    {
    private:
        std::unique_ptr<std::string> mBuffer;
        static constexpr auto Regex = ctll::fixed_string{"[^\n]+"};
        //unsigned char mData[16];
        std::unique_ptr<EphemiralBase> mRows;
        //EphemiralBase * mRows;
    public:
        EphemiralRowBackingStore(): mRows{nullptr}
        {

        }

        EphemiralRowBackingStore( std::unique_ptr<std::string> && buffer) noexcept
        {
            this->mBuffer = std::move(buffer);
            auto matchSet = ctre::range<Regex>(LD::StringView {this->mBuffer.get()->c_str(),this->mBuffer.get()->size()});
            auto rawr = EphemiralIterator{matchSet,matchSet.begin(),matchSet.end()};
            this->mRows = std::make_unique<decltype(rawr)>(EphemiralIterator{matchSet,matchSet.begin(),matchSet.end()});
        }
        EphemiralRowBackingStore(EphemiralRowBackingStore && backingStore) noexcept
        {
            (*this) = LD::Move(backingStore);
        }

        EphemiralRowBackingStore & operator = (EphemiralRowBackingStore && backingStore) noexcept
        {
            this->mRows = std::move(backingStore.mRows);
            this->mBuffer = std::move(backingStore.mBuffer);
            return (*this);
        }

        template<typename ... Args>
        LD::QueryResult<LD::StringView(Args...)> GetLine(Args && ... arguments) noexcept
        {
            if (this->mRows->CanFetchRow())
            {
                return LD::MakeContext(LD::TransactionResult{},LD::StringView{this->mRows->NextRow()},LD::Forward<Args>(arguments)...);
            }
            return LD::MakeContext(LD::TransactionError{},LD::Forward<Args>(arguments)...);
        }

        EphemiralRowBackingStore & Reset() noexcept
        {
            auto matchSet = ctre::range<Regex>(LD::StringView {this->mBuffer.get()->c_str(),this->mBuffer.get()->size()});
            auto rawr = EphemiralIterator{matchSet,matchSet.begin(),matchSet.end()};
            this->mRows = std::make_unique<decltype(rawr)>(EphemiralIterator{matchSet,matchSet.begin(),matchSet.end()});
            return (*this);
        }
    };
}
#endif //LANDESSDEVCORE_EPHEMIRALROWBACKINGSTORE_HPP
