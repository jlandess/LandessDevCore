//
// Created by phoenixflower on 6/18/20.
//

#ifndef LANDESSDEVCORE_ARCHIVER_HPP
#define LANDESSDEVCORE_ARCHIVER_HPP
#include "Primitives/General/StaticArray.hpp"
#include "Primitives/General/StringView.hpp"
#include "Primitives/General/Immutable.hpp"
#include "TypeTraits/Type.h"
namespace LD
{
    class Archiver
    {
    private:

    public:

        template<typename T>
        LD::Enable_If_T<LD::Require<LD::IsSame<T,unsigned long>>,LD::StaticArray<unsigned char,8>> Archive(const T & integer) const noexcept
        {

            LD::StaticArray<unsigned char,8> buffer;
            unsigned int firstSegment = ((int*)&integer)[0];
            unsigned int secondSegment = ((int*)&integer)[1];


            buffer[0] = (firstSegment & 0xff000000) >> 24;
            buffer[1] = (firstSegment & 0x00ff0000) >> 16;
            buffer[2] = (firstSegment & 0x0000ff00) >>  8;
            buffer[3] = (firstSegment & 0x000000ff) ;

            buffer[4] = (secondSegment & 0xff000000) >> 24;
            buffer[5] = (secondSegment & 0x00ff0000) >> 16;
            buffer[6] = (secondSegment & 0x0000ff00) >>  8;
            buffer[7] = (secondSegment & 0x000000ff) ;
            return buffer;
        }

        template<typename T>
        LD::Enable_If_T<LD::Require<LD::IsSame<T,long>>,LD::StaticArray<unsigned char,8>> Archive(const T & integer) const noexcept
        {

            LD::StaticArray<unsigned char,8> buffer;
            int firstSegment = ((int*)&integer)[0];
            int secondSegment = ((int*)&integer)[1];


            buffer[0] = (firstSegment & 0xff000000) >> 24;
            buffer[1] = (firstSegment & 0x00ff0000) >> 16;
            buffer[2] = (firstSegment & 0x0000ff00) >>  8;
            buffer[3] = (firstSegment & 0x000000ff) ;

            buffer[4] = (secondSegment & 0xff000000) >> 24;
            buffer[5] = (secondSegment & 0x00ff0000) >> 16;
            buffer[6] = (secondSegment & 0x0000ff00) >>  8;
            buffer[7] = (secondSegment & 0x000000ff) ;
            return buffer;
        }

        template<typename T>
        LD::Enable_If_T<LD::Require<LD::IsSame<T,double>>,LD::StaticArray<unsigned char,8>> Archive(const T & floatingPoint) const noexcept
        {

            LD::StaticArray<unsigned char,8> buffer;
            long integer = *(long*)&floatingPoint;

            int firstSegment = ((int*)&integer)[0];
            int secondSegment = ((int*)&integer)[1];


            buffer[0] = (firstSegment & 0xff000000) >> 24;
            buffer[1] = (firstSegment & 0x00ff0000) >> 16;
            buffer[2] = (firstSegment & 0x0000ff00) >>  8;
            buffer[3] = (firstSegment & 0x000000ff) ;

            buffer[4] = (secondSegment & 0xff000000) >> 24;
            buffer[5] = (secondSegment & 0x00ff0000) >> 16;
            buffer[6] = (secondSegment & 0x0000ff00) >>  8;
            buffer[7] = (secondSegment & 0x000000ff) ;
            return buffer;
        }

        template<LD::UInteger Size>
        StaticArray<unsigned char,Size> Archive(const LD::ImmutableString<Size> & string) const noexcept
        {
            LD::StaticArray<unsigned char,Size> buffer;
            LD::For<Size>([](
                    auto I,
                    auto && input,
                    auto && string)
            {
                string[I] = input[I]+128;
                return true;
            },string,buffer);
            return buffer;
        }

        template<typename T>
        LD::Enable_If_T<LD::Require<LD::IsSame<T,unsigned long>>,T> UnArchive(const LD::Type<T> & type, const LD::StaticArray<unsigned char,8> & buffer) const noexcept
        {
            unsigned int firstSegment = buffer[0] << 24;
            firstSegment |= buffer[1] << 16;
            firstSegment |= buffer[2] << 8;
            firstSegment |= buffer[3];


            unsigned int secondSegment = buffer[4] << 24;
            secondSegment |= buffer[5] << 16;
            secondSegment |= buffer[6] << 8;
            secondSegment |= buffer[7];

            unsigned long value = {};
            ((int*)&value)[0] = firstSegment;
            ((int*)&value)[1] = secondSegment;

            return value;
        }

        template<typename T>
        LD::Enable_If_T<LD::Require<LD::IsSame<T,long>>,T> UnArchive(const LD::Type<T> & type, const LD::StaticArray<unsigned char,8> & buffer) const noexcept
        {
            int firstSegment = buffer[0] << 24;
            firstSegment |= buffer[1] << 16;
            firstSegment |= buffer[2] << 8;
            firstSegment |= buffer[3];


            int secondSegment = buffer[4] << 24;
            secondSegment |= buffer[5] << 16;
            secondSegment |= buffer[6] << 8;
            secondSegment |= buffer[7];

            long value = {};
            ((int*)&value)[0] = firstSegment;
            ((int*)&value)[1] = secondSegment;

            return value;
        }

        template<typename T>
        LD::Enable_If_T<LD::Require<LD::IsSame<T,double>>,T> UnArchive(const LD::Type<T> & type, const LD::StaticArray<unsigned char,8> & buffer) const noexcept
        {
            long number = this->UnArchive(LD::Type<long>{},buffer);
            double currentFloatValue = *(double*)&number;
            return currentFloatValue;
        }

    };
}
#endif //LANDESSDEVCORE_ARCHIVER_HPP
