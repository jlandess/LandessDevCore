//
// Created by phoenixflower on 6/18/20.
//

#ifndef LANDESSDEVCORE_MATRIX_HPP
#define LANDESSDEVCORE_MATRIX_HPP
#include "Definitions/Integer.hpp"
namespace LD
{
    template<typename T,const UInteger width, const UInteger height, const UInteger depth = 1>
    class ImmutableMatrix
    {
    private:
        T  Objects[width*height*depth];
    public:
        typedef T ValueType;
        typedef UInteger SizeType;
        typedef const T ConstValueType;
        typedef T * Pointer;
        typedef const T * ConstPointer;
        const T & operator()(const unsigned long x, const unsigned long y, const unsigned long z = 0) const
        {
            return this->Objects[x + width * (y + depth * z)];
        }
        T & operator()(const unsigned long x, const unsigned long y, const unsigned long z = 0)
        {
            return this->Objects[x + width * (y + depth * z)];
        }
        const T & operator[](const unsigned long n) const
        {
            return this->Objects[n];
        }
        T & operator[](const unsigned long n)
        {
            return this->Objects[n];
        }
        const unsigned long GetWidth() const
        {
            return width;
        }
        const unsigned long GetHeight() const
        {
            return height;
        }
        const unsigned long GetDepth() const
        {
            return depth;
        }
        const bool Contains(const T & value) const
        {
            return this->Find(value).Valid;
        }
        /*
        const Point Find(const T & value) const
        {
            Point foundIndex;
            foundIndex.Valid = false;
            foundIndex.X = 0;
            foundIndex.Y = 0;
            unsigned long size = width*height*depth;
            for (unsigned long n = 0; n<size; n++)
            {
                unsigned long z = n % depth;
                unsigned long y = (n / depth) % height;
                unsigned long x = n / (width * height);

                if (this->Objects[x + width * (y + depth * z)] == value)
                {
                    foundIndex.Valid = true;
                    foundIndex.Z = z;
                    foundIndex.Y = y;
                    foundIndex.X = x;
                    break;
                }
            }
            return foundIndex;
        }
         */

        void GetBuffer(T buffer[width*height*depth]) const
        {
            unsigned long size = width*height*depth;
            for (unsigned long n = 0; n<size; n++)
            {
                buffer[n] = this->Objects[n];
            }
        }
        const T * GetBuffer(unsigned long & usableWidth, unsigned long & usableHeight, unsigned long & usableDepth) const
        {
            unsigned long size = width*height*depth;
            T * buffer = new T[size];
            for (unsigned long n = 0; n<size; n++)
            {
                buffer[n] = this->Objects[n];
            }
            usableWidth = width;
            usableHeight = height;
            usableDepth = depth;
            return buffer;
        }
        const T * GetBuffer() const
        {
            unsigned long size = width*height*depth;
            T * buffer = new T[size];
            for (unsigned long n = 0; n<size; n++)
            {
                buffer[n] = this->Objects[n];
            }
            return buffer;
        }
    };
}
#endif //LANDESSDEVCORE_MATRIX_HPP
