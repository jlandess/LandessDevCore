//
// Created by phoenixflower on 1/3/21.
//

#ifndef LANDESSDEVCORE_FILE_HPP
#define LANDESSDEVCORE_FILE_HPP
#include <memory>
#include "Primitives/General/StringView.hpp"
#include "TypeTraits/CanBeAnImmutableString.hpp"
#include "Reflection/Reflection.hpp"
namespace LD
{
    namespace IO
    {
        class ReadOnly
        {

        };
        class WriteOnly
        {

        };
        class Append
        {

        };
        class ReadWrite
        {

        };
        class File
        {
        private:
            std::unique_ptr<FILE,int(*)(FILE*)> mHandle;
        public:

            File() noexcept:mHandle(nullptr,fclose)
            {

            }
            template<typename OpenMode>
            File(LD::StringView fileToOpen, OpenMode && mode) noexcept:mHandle(nullptr,fclose)
            {
                if constexpr (LD::CT::IsSameWithoutQualifiers(LD::Type<OpenMode>{},LD::Type<LD::IO::ReadOnly>{}))
                {
                    mHandle = std::unique_ptr<FILE, int(*)(FILE*)>(fopen(fileToOpen.data(), "r"), fclose);
                }else if constexpr (LD::CT::IsSameWithoutQualifiers(LD::Type<OpenMode>{},LD::Type<LD::IO::WriteOnly>{}))
                {
                    mHandle = std::unique_ptr<FILE, int(*)(FILE*)>(fopen(fileToOpen.data(), "w"), fclose);
                }else if constexpr (LD::CT::IsSameWithoutQualifiers(LD::Type<OpenMode>{},LD::Type<LD::IO::Append>{}))
                {
                    mHandle = std::unique_ptr<FILE, int(*)(FILE*)>(fopen(fileToOpen.data(), "a"), fclose);
                }else if constexpr (LD::CT::IsSameWithoutQualifiers(LD::Type<OpenMode>{},LD::Type<LD::IO::ReadWrite>{}))
                {
                    mHandle = std::unique_ptr<FILE, int(*)(FILE*)>(fopen(fileToOpen.data(), "r+"), fclose);
                }
            }
            template<typename OpenMode,LD::UInteger Size>
            File(LD::ImmutableString<Size> fileToOpen, OpenMode && mode) noexcept:mHandle(nullptr,fclose)
            {
                if constexpr (LD::CT::IsSameWithoutQualifiers(LD::Type<OpenMode>{},LD::Type<LD::IO::ReadOnly>{}))
                {
                    mHandle = std::unique_ptr<FILE, int(*)(FILE*)>(fopen(fileToOpen.Data(), "r"), fclose);
                }else if constexpr (LD::CT::IsSameWithoutQualifiers(LD::Type<OpenMode>{},LD::Type<LD::IO::WriteOnly>{}))
                {
                    mHandle = std::unique_ptr<FILE, int(*)(FILE*)>(fopen(fileToOpen.Data(), "w"), fclose);
                }else if constexpr (LD::CT::IsSameWithoutQualifiers(LD::Type<OpenMode>{},LD::Type<LD::IO::Append>{}))
                {
                    mHandle = std::unique_ptr<FILE, int(*)(FILE*)>(fopen(fileToOpen.Data(), "a"), fclose);
                }else if constexpr (LD::CT::IsSameWithoutQualifiers(LD::Type<OpenMode>{},LD::Type<LD::IO::ReadWrite>{}))
                {
                    mHandle = std::unique_ptr<FILE, int(*)(FILE*)>(fopen(fileToOpen.Data(), "r+"), fclose);
                }
            }

            template<typename T>
            LD::Enable_If_T<LD::Require<LD::CT::CanBeAnImmutableString(LD::CT::RemoveQualifiers(LD::Type<T>{}))>,File&>  operator << (T && object) noexcept
            {
                auto immutableRepresentation = LD::ToImmutableString(LD::Forward<T>(object));
                fwrite(immutableRepresentation.Data(),sizeof(char),immutableRepresentation.GetSize(),this->mHandle.get());
                return (*this);
            }
            File &  operator << (LD::StringView view) noexcept
            {
                fwrite(view.data(),sizeof(char),view.size(),this->mHandle.get());
                return (*this);
            }
            template<typename T>
            LD::Enable_If_T<LD::Require<LD::CT::IsReflectable(LD::CT::RemoveQualifiers(LD::Type<T>{}))>,File &> operator >> (T && object) noexcept
            {
                return (*this);
            }
        };
    }
}
#endif //LANDESSDEVCORE_FILE_HPP
