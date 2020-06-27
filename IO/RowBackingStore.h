//
// Created by phoenixflower on 6/27/20.
//

#ifndef LANDESSDEVCORE_ROWBACKINGSTORE_H
#define LANDESSDEVCORE_ROWBACKINGSTORE_H
#include "Primitives/General/StringView.hpp"
#include "FetchRequest.h"
#include "DatabaseOperationResult.h"
#include <stdio.h>
namespace LD
{
    class RowBackingStore
    {
    private:
        FILE * mHandle;
        char * mLineBuffer;
        size_t mLineBufferSize;
        off64_t mCachedHandle;
    public:
        RowBackingStore(const LD::StringView & fileToOpen) noexcept:mLineBuffer{nullptr}
        {
            mHandle = fopen(fileToOpen.data(),"r");
            this->mCachedHandle = ftello(this->mHandle);
        }
        RowBackingStore & Reset() noexcept
        {
            fseeko(this->mHandle,0,SEEK_SET);
            fflush(this->mHandle);
            return (*this);
        }

        ~RowBackingStore() noexcept
        {
            fclose(this->mHandle);
            free(this->mLineBuffer);
        }
        template<typename ... Args>
        LD::QueryResult<bool(Args...)> Begin(Args && ... arguments) noexcept
        {
            return LD::MakeContext(LD::DatabaseTransactionResult{},bool{true},LD::Forward<Args>(arguments)...);
        }
        template<typename ... Args>
        LD::QueryResult<bool(Args...)> Commit(Args && ... arguments) noexcept
        {
            return LD::MakeContext(LD::DatabaseTransactionResult{},bool{true},LD::Forward<Args>(arguments)...);
        }

        template<typename ... Args>
        LD::QueryResult<bool(Args...)> RollBack(Args && ... arguments) noexcept
        {
            fflush(this->mHandle);
            return LD::MakeContext(LD::DatabaseTransactionResult{},bool{true},LD::Forward<Args>(arguments)...);
        }
        RowBackingStore & Flush() noexcept
        {
            fflush(this->mHandle);
            return (*this);
        }
        template<typename ... Args>
        LD::QueryResult<LD::StringView(Args...)> GetLine(Args && ... arguments) noexcept
        {
            //this looks like a problem, getline will realloc and alloc on demand.  Trust the getline implementation
            //fflush(this->mHandle);
            if (this->mLineBuffer != nullptr && this->mLineBufferSize != 0)
            {
                //memset((void*)this->mLineBufferSize,'\0',this->mLineBufferSize);
                //this->mLineBuffer[0] = '\0';
                memset(this->mLineBuffer,0,strlen(this->mLineBuffer));
            }
            LD::UInteger readAmount = getline(&mLineBuffer,&mLineBufferSize,this->mHandle);
            //LD::QueryResult<LD::StringView(Args...)> results[2];
            //results[0] = LD::MakeContext(LD::DatabaseError{},LD::Forward<Args>(arguments)...);



            if (readAmount != EOF)
            {
                return LD::MakeContext(LD::DatabaseTransactionResult{},LD::StringView{this->mLineBuffer,strlen(this->mLineBuffer)-1},LD::Forward<Args>(arguments)...);
            }
            //this->mLineBuffer[strlen(this->mLineBuffer)-1] = '\0';

            //std::string temp = this->mLineBuffer;
            //temp[temp.size()-1] = '\0';
            //std::cout << temp << std::endl;
            //std::cout << LD::StringView {this->mLineBuffer,strlen(this->mLineBuffer)-1} << std::endl;
            //results[1] =
            //return results[readAmount > 0 && readAmount != EOF];
            return LD::MakeContext(LD::DatabaseError{},LD::Forward<Args>(arguments)...);
        }

        template<typename ... Args>
        LD::QueryResult<LD::StringView (Args...)> operator()(const LD::UInteger & index, Args && ... arguments) noexcept
        {
            LD::UInteger n = {};
            //there's no formatting, or meta data to let us know which byte offsets the rows are at, just read them one at a time until we get to the
            //indicated number, or indicate we got a snag along the way and indicate that with the return of LD::QueryResult
            do
            {
                if (!getline(&this->mLineBuffer,&this->mLineBufferSize,this->mHandle))
                {
                    break;
                }
            }while(n<index);
            LD::QueryResult<LD::StringView(Args...)> results[2];
            results[0] = LD::MakeContext(LD::DatabaseError{LD::DatabaseIOError{}},LD::Forward<Args>(arguments)...);
            results[1] = LD::MakeContext(LD::DatabaseTransactionResult{},LD::StringView{this->mLineBuffer,this->mLineBufferSize},LD::Forward<Args>(arguments)...);
            return results[n == index];
        }
    };



}
#endif //LANDESSDEVCORE_ROWBACKINGSTORE_H
