//
//  StaticArray.h
//  DataStructures
//
//  Created by James Landess on 10/23/14.
//  Copyright (c) 2014 LandessDev. All rights reserved.
//

#ifndef DataStructures_StaticArray_h
#define DataStructures_StaticArray_h

#include "Definitions/Integer.hpp"
#include "Definitions/Float.hpp"
#include "Functor/Lambda.h"
#include "Algorithms/GenericResize.h"
#include "TypeTraits/CompileTimeControlFlow.hpp"
//#include "Definitions/Common.hpp"
#include "TypeTraits/CompileTimeControlFlow.hpp"
#include "TypeTraits/IsImmutable.h"
#include "TypeTraits/Iterable.h"
#include "TypeTraits/Detection.hpp"
#include "TypeTraits/IsConstructible.hpp"
namespace LD
{

    template <typename U, PDP::UInteger K> using CArray = U[K];


    typedef CArray<float, 2> Float2;
    typedef CArray<float, 3> Float3;
    typedef CArray<float, 4> Float4;
    typedef CArray<float, 6> Float6;
    typedef CArray<float, 8> Float8;
    typedef CArray<float, 9> Float9;
    typedef CArray<float, 12> Float12;
    typedef CArray<float, 16> Float16;


    typedef CArray<double, 2> Double2;
    typedef CArray<double, 3> Double3;
    typedef CArray<double, 4> Double4;
    typedef CArray<double, 6> Double6;
    typedef CArray<double, 8> Double8;
    typedef CArray<double, 9> Double9;
    typedef CArray<double, 12> Double12;
    typedef CArray<double, 16> Double16;

    typedef CArray<PDP::UInteger, 2> UInt2;
    typedef CArray<PDP::UInteger, 3> UInt3;
    typedef CArray<PDP::UInteger, 4> UInt4;
    typedef CArray<PDP::UInteger, 6> UInt6;
    typedef CArray<PDP::UInteger, 8> UInt8;
    typedef CArray<PDP::UInteger, 9> UInt9;
    typedef CArray<PDP::UInteger, 12> UInt12;
    typedef CArray<PDP::UInteger, 16> UInt16;


    typedef CArray<PDP::Integer, 2> Int2;
    typedef CArray<PDP::Integer, 3> Int3;
    typedef CArray<PDP::Integer, 4> Int4;
    typedef CArray<PDP::Integer, 6> Int6;
    typedef CArray<PDP::Integer, 8> Int8;
    typedef CArray<PDP::Integer, 9> Int9;
    typedef CArray<PDP::Integer, 12> Int12;
    typedef CArray<PDP::Integer, 16> Int16;


    namespace Detail
    {
        template<typename T, const PDP::UInteger N>
        class TypeAid
        {

        };

        template<>
        class TypeAid<float, 2>
        {
        public:
            typedef LD::Float2 Type;
            typedef LD::Float SinglePrecision;
            typedef LD::Float2 DoublePrecision;
            typedef LD::Float CoreType;
        };

        template<>
        class TypeAid<float, 3>
        {
        public:
            typedef LD::Float3 Type;
            typedef LD::Float SinglePrecision;
            typedef LD::Float2 DoublePrecision;
            typedef LD::Float3 TriplePrecision;
            typedef LD::Float CoreType;
        };


        template<>
        class TypeAid<float, 4>
        {
        public:
            typedef LD::Float4 Type;
            typedef LD::Float SinglePrecision;
            typedef LD::Float2 DoublePrecision;
            typedef LD::Float3 TriplePrecision;
            typedef LD::Float4 QuadPrecision;
            typedef LD::Float CoreType;
        };


        template<>
        class TypeAid<float, 6>
        {
        public:
            typedef LD::Float6 Type;
            typedef LD::Float SinglePrecision;
            typedef LD::Float CoreType;
        };

        template<>
        class TypeAid<float, 8>
        {
        public:
            typedef LD::Float8 Type;
            typedef LD::Float SinglePrecision;
            typedef LD::Float CoreType;
        };

        template<>
        class TypeAid<float, 9>
        {
        public:
            typedef LD::Float9 Type;
            typedef LD::Float SinglePrecision;
            typedef float CoreType;
        };

        template<>
        class TypeAid<float, 12>
        {
        public:
            typedef LD::Float12 Type;
            typedef LD::Float SinglePrecision;
            typedef LD::Float CoreType;
        };


        template<>
        class TypeAid<float, 16>
        {
        public:
            typedef LD::Float16 Type;
            typedef LD::Float SinglePrecision;
            typedef LD::Float CoreType;
        };


        template<>
        class TypeAid<double, 2>
        {
        public:
            typedef LD::Float2 Type;
            typedef LD::Float SinglePrecision;
            typedef LD::Float2 DoublePrecision;
            typedef LD::Float CoreType;
        };

        template<>
        class TypeAid<double, 3>
        {
        public:
            typedef LD::Float3 Type;
            typedef LD::Float SinglePrecision;
            typedef LD::Float2 DoublePrecision;
            typedef LD::Float3 TriplePrecision;
            typedef LD::Float CoreType;
        };


        template<>
        class TypeAid<double, 4>
        {
        public:
            typedef LD::Float4 Type;
            typedef LD::Float SinglePrecision;
            typedef LD::Float2 DoublePrecision;
            typedef LD::Float3 TriplePrecision;
            typedef LD::Float4 QuadPrecision;
            typedef LD::Float CoreType;
        };


        template<>
        class TypeAid<double, 6>
        {
        public:
            typedef LD::Float6 Type;
            typedef LD::Float SinglePrecision;
            typedef LD::Float CoreType;
        };

        template<>
        class TypeAid<double, 8>
        {
        public:
            typedef LD::Float8 Type;
            typedef LD::Float SinglePrecision;
            typedef LD::Float CoreType;
        };

        template<>
        class TypeAid<double, 9>
        {
        public:
            typedef LD::Float9 Type;
            typedef LD::Float SinglePrecision;
            typedef LD::Float CoreType;
        };

        template<>
        class TypeAid<double, 12>
        {
        public:
            typedef LD::Float12 Type;
            typedef LD::Float SinglePrecision;
            typedef LD::Float CoreType;
        };


        template<>
        class TypeAid<double, 16>
        {
        public:
            typedef LD::Float16 Type;
            typedef LD::Float SinglePrecision;
            typedef LD::Float CoreType;
        };

        template<>
        class TypeAid<LD::UInteger, 2>
        {
        public:
            typedef LD::UInt2 Type;
            typedef LD::UInt SinglePrecision;
            typedef LD::UInt2 DoublePrecision;
            typedef LD::UInt CoreType;

        };

        template<>
        class TypeAid<LD::UInteger, 3>
        {
        public:
            typedef LD::UInt3 Type;
            typedef LD::UInt SinglePrecision;
            typedef LD::UInt2 DoublePrecision;
            typedef LD::UInt3 TriplePrecision;
            typedef LD::UInt CoreType;
        };


        template<>
        class TypeAid<LD::UInteger, 4>
        {
        public:
            typedef LD::UInt4 Type;
            typedef LD::UInt SinglePrecision;
            typedef LD::UInt2 DoublePrecision;
            typedef LD::UInt3 TriplePrecision;
            typedef LD::UInt4 QuadPrecision;
            typedef LD::UInt CoreType;
        };


        template<>
        class TypeAid<LD::UInteger, 6>
        {
        public:
            typedef LD::UInt6 Type;
            typedef LD::UInt SinglePrecision;
            typedef LD::UInt CoreType;
        };

        template<>
        class TypeAid<LD::UInteger, 8>
        {
        public:
            typedef LD::UInt8 Type;
            typedef LD::UInt SinglePrecision;
            typedef LD::UInt CoreType;
        };

        template<>
        class TypeAid<LD::UInteger, 9>
        {
        public:
            typedef LD::UInt9 Type;
            typedef LD::UInt SinglePrecision;
            typedef LD::UInt CoreType;
        };

        template<>
        class TypeAid<LD::UInteger, 12>
        {
        public:
            typedef LD::UInt12 Type;
            typedef LD::UInt SinglePrecision;
            typedef LD::UInt CoreType;
        };


        template<>
        class TypeAid<LD::UInteger, 16>
        {
        public:
            typedef LD::UInt16 Type;
            typedef LD::UInt SinglePrecision;
            typedef LD::UInt CoreType;
        };


        template<>
        class TypeAid<LD::Integer, 2>
        {
        public:
            typedef LD::Int2 Type;
            typedef LD::Int SinglePrecision;
            typedef LD::Int2 DoublePrecision;
            typedef LD::Int CoreType;
        };

        template<>
        class TypeAid<PDP::Integer, 3>
        {
        public:
            typedef LD::Int3 Type;
            typedef LD::Int SinglePrecision;
            typedef LD::Int2 DoublePrecision;
            typedef LD::Int3 TriplePrecision;
            typedef LD::Int CoreType;
        };


        template<>
        class TypeAid<LD::Integer, 4>
        {
        public:
            typedef LD::Int4 Type;
            typedef LD::Int SinglePrecision;
            typedef LD::Int2 DoublePrecision;
            typedef LD::Int3 TriplePrecision;
            typedef LD::Int4 QuadPrecision;
            typedef LD::Int CoreType;
        };


        template<>
        class TypeAid<LD::Integer, 6>
        {
        public:
            typedef LD::Int6 Type;
            typedef LD::Int SinglePrecision;
            typedef LD::Int CoreType;
        };

        template<>
        class TypeAid<LD::Integer, 8>
        {
        public:
            typedef LD::Int8 Type;
            typedef LD::Int SinglePrecision;
            typedef LD::Int CoreType;
        };

        template<>
        class TypeAid<LD::Integer, 9>
        {
        public:
            typedef LD::Int9 Type;
            typedef LD::Int SinglePrecision;
            typedef LD::Int CoreType;
        };

        template<>
        class TypeAid<LD::Integer, 12>
        {
        public:
            typedef LD::Int12 Type;
            typedef LD::Int SinglePrecision;
            typedef LD::Int CoreType;
        };


        template<>
        class TypeAid<LD::Integer, 16>
        {
        public:
            typedef LD::Int16 Type;
            typedef LD::Int SinglePrecision;
            typedef LD::Int CoreType;
        };
    }

    template<typename T, PDP::UInteger Amount>
    class StaticArray
    {
    private:
        T InternalBuffer[Amount ? Amount:1];
        LD::UInteger Size;
        //unsigned char InternalBuffer[PDP::CompileTimeAlignmentSize<sizeof(T)*Amount + sizeof(PDP::UInteger)>::value];
    public:


        //StaticArray<int,10>::Iterator;
        //Iterator<StaticArray<int,10>>
        void PushBack(const T &  object) noexcept
        {

            this->InternalBuffer[this->Size++] = object;
        }

        void Emplace(T &&  object) noexcept
        {

            this->InternalBuffer[this->Size++] = LD::Move(object);
        }

        void Resize(LD::UInteger requestedSize) noexcept
        {
            this->Size = requestedSize;
        }

        //describe ownership
        class Iterator
        {
        private:
            T * mBuffer;
            LD::UInteger mIndex;
        public:

            typedef T Type;
            constexpr Iterator() noexcept:mBuffer(nullptr),mIndex(0)
            {

            }

            constexpr Iterator(const Iterator & it) noexcept:mBuffer(it.mBuffer),mIndex(it.mIndex)
            {

            }



            LD::UInteger operator-(const Iterator & it) const noexcept
            {
                return this->mIndex-it.mIndex;
            }

            constexpr bool operator < (const Iterator & iterator) noexcept
            {
                return this->mIndex < iterator.mIndex;
            }

            const bool operator == (const Iterator & iterator)
            {
                return (this->mBuffer+this->mIndex) == (iterator.mBuffer + iterator.mIndex);
            }

            const bool operator != (const Iterator & iterator)
            {
                return (this->mBuffer+this->mIndex) != (iterator.mBuffer + iterator.mIndex);
            }

            constexpr Iterator(T * currentIndex, const LD::UInteger & index) noexcept :mBuffer(currentIndex),mIndex(index)
            {

            }

            T & operator *() noexcept
            {
                return *(this->mBuffer + this->mIndex);
            }

            T & operator[](const LD::UInteger & index) noexcept
            {
                return *(this->mBuffer + index);
            }
            Iterator & operator++() noexcept
            {
                this->mIndex+=1;
                return (*this);
                //return Iterator{this->mBuffer,this->mIndex+1};
            }

            Iterator & operator++(int) noexcept
            {
                this->mIndex+=1;
                return (*this);
                //return Iterator{this->mBuffer,this->mIndex+1};
            }

            Iterator & operator--() noexcept
            {
                this->mIndex-=1;
                return (*this);
                //return Iterator{this->mBuffer,this->mIndex-1};
            }

            Iterator & operator--(int) noexcept
            {
                this->mIndex-=1;
                return (*this);
                //return Iterator{this->mBuffer,this->mIndex-1};
            }
        };

        Iterator Begin()
        {
            return Iterator{this->InternalBuffer,0};
        }
        Iterator End()
        {
            return Iterator{nullptr,this->Size};
        }

        constexpr void PopBack() noexcept
        {
            this->InternalBuffer[this->Size-1] = T{};
            this->Size--;
        }

        constexpr void Clear(const T & value = T{}) noexcept
        {

            LD::For<Amount>([](auto Index,T buffer[Amount ? Amount:1], const T & init)
            {
               buffer[Index] = init;
               return true;
            },this->InternalBuffer,value);
        }
        template<typename U,LD::UInteger Index, LD::UInteger A>
        friend constexpr U & Get(LD::StaticArray<U,Amount> & array) noexcept ;
        template<typename U,LD::UInteger Index, LD::UInteger A>
        friend constexpr const U & Get(const LD::StaticArray<U,Amount> & array) noexcept ;

        inline StaticArray();
        inline StaticArray(const CArray<T, Amount> & cArray);


        inline constexpr StaticArray(const T & initializedState) noexcept
        {
            LD::For<Amount>([](auto Index,T buffer[Amount ? Amount:1], const T & init)
            {
                buffer[Index] = init;
                return true;
            },this->InternalBuffer,initializedState);
        }
        inline const T & operator[](const PDP::UInteger & index) const;
        inline T & operator[](const PDP::UInteger & index);

        inline const LD::UInteger GetSize() const;

        inline const LD::UInteger GetCapacity() const;

        inline constexpr const T * GetData() const noexcept
        {
             const T * buffer = (const T*)&this->InternalBuffer;
            return buffer;
        }
        inline constexpr  T * GetData()  noexcept
        {
            T * buffer = (T*)&this->InternalBuffer;
            return buffer;
        }
        inline StaticArray & operator = (const CArray<T, Amount> & cArray);


    };


    template<typename T, PDP::UInteger Amount>
    StaticArray<T, Amount>::StaticArray():Size(0)
    {
        //PDP::UInteger * size = (PDP::UInteger*)this->InternalBuffer;
        //*size = 0;
    }

    template<typename T, PDP::UInteger Amount>
    StaticArray<T, Amount>::StaticArray(const CArray<T, Amount> & cArray)
    {
        //PDP::UInteger * size = (PDP::UInteger*)this->InternalBuffer;
        //*size = Amount;
        this->Size = Amount;
        T * buffer = (T*)((unsigned char*)InternalBuffer+sizeof(PDP::UInteger));

        for (PDP::UInteger n = 0; n<Amount; ++n)
        {
            this->InternalBuffer[n] = cArray[n];
            //this->InternalBuffer[n] = cArray[n];
        }
    }

    template<typename T, PDP::UInteger Amount>
    const T & StaticArray<T,Amount>::operator[](const LD::UInteger &index) const
    {
        //const T * buffer = (const T*)((unsigned char*)InternalBuffer+sizeof(PDP::UInteger));
        //return *(buffer+index);

        return this->InternalBuffer[index];
    }

    template<typename T, PDP::UInteger Amount>
    T & StaticArray<T,Amount>::operator[](const LD::UInteger &index)
    {
        //T * buffer = (T*)((unsigned char*)InternalBuffer+sizeof(PDP::UInteger));
        //return *(buffer+index);

        return this->InternalBuffer[index];
    }
    template<typename T, PDP::UInteger Amount>
    const LD::UInteger  StaticArray<T,Amount>::GetSize() const
    {
        //PDP::UInteger * size = (PDP::UInteger*)this->InternalBuffer;
        //return *size;

        return this->Size;
    }

    template<typename T, PDP::UInteger Amount>
    const LD::UInteger  StaticArray<T,Amount>::GetCapacity() const
    {
        return Amount;
    }


    template<typename T, PDP::UInteger Amount>
    StaticArray<T, Amount> & StaticArray<T,Amount>::operator=(const CArray<T, Amount> &cArray)
    {
        //const T * buffer = (const T*)((unsigned char*)InternalBuffer+sizeof(PDP::UInteger));
        for (LD::UInteger n = 0; n<Amount; ++n)
        {
            //*(buffer+n) = cArray[n];
            this->InternalBuffer[n] = cArray[n];
        }


        //PDP::UInteger * size = (PDP::UInteger*)this->InternalBuffer;
        //*size = Amount;
        this->Size = Amount;
        return (*this);
    }

    template<typename T,LD::UInteger Index, LD::UInteger Amount>
    constexpr T & Get(LD::StaticArray<T,Amount> & array) noexcept
    {
        return array.InternalBuffer[Index];
    }

    template<typename T,LD::UInteger Index, LD::UInteger Amount>
    constexpr const T & Get(const LD::StaticArray<T,Amount> & array) noexcept
    {
        return array.InternalBuffer[Index];
    }
}



namespace LD
{
    template<typename T, LD::UInteger Size>
    class BackInserter<LD::StaticArray<T,Size>>
    {
    private:
        LD::StaticArray<T,Size> & mArray;
    public:
        constexpr BackInserter(LD::StaticArray<T,Size> & array) noexcept: mArray{array}
        {

        }

        BackInserter(const BackInserter<LD::StaticArray<T,Size>> & array) noexcept:mArray{array.mArray}
        {

        }

        BackInserter<LD::StaticArray<T,Size>> & operator = (const BackInserter<LD::StaticArray<T,Size>> & array) noexcept
        {
            this->mArray = array.mArray;
            return (*this);
        }
        BackInserter<LD::StaticArray<T,Size>> &  operator*() noexcept
        {
            return (*this);
        }
        /*
        BackInserter<LD::StaticArray<T,Size>> & operator = (T && object) noexcept
        {
            //std::cout << "Size : " << mArray.GetSize() << std::endl;
            //mArray[mArray.GetSize()] = object;
            mArray.PushBack(LD::Forward<T>(object));
            return (*this);
        }
         */
        template<typename U>
        LD::Enable_If_T<LD::Require<LD::IsCopyConstructible<U>::value>,BackInserter<LD::StaticArray<T,Size>>> & operator = (const U &  object) noexcept
        {
            //std::cout << "Size : " << mArray.GetSize() << std::endl;
            //mArray[mArray.GetSize()] = object;
            mArray.PushBack(object);
            return (*this);
        }

        template<typename U>
        LD::Enable_If_T<LD::Require<LD::IsMoveConstructible<U>::value>,BackInserter<LD::StaticArray<T,Size>>> & operator = ( U &&  object) noexcept
        {
            //std::cout << "Size : " << mArray.GetSize() << std::endl;
            //mArray[mArray.GetSize()] = object;
            //mArray.PushBack(object);
            mArray.Emplace(LD::Move(LD::Forward<U>(object)));
            return (*this);
        }
        constexpr BackInserter<LD::StaticArray<T,Size>> operator++() noexcept
        {
            return (*this);
        }

        constexpr BackInserter<LD::StaticArray<T,Size>> operator++(int) noexcept
        {
            return (*this);
        }
    };
}
struct Point
{
    float x;
    float y;

    constexpr Point(const float & x1 = 0, const float & x2 = 0) noexcept :x(x1),y(x2){}
    constexpr const float & X() const noexcept
    {
        return this->x;
    }

    constexpr float & X() noexcept
    {
        return this->x;
    }

    void SetX(const float & x) noexcept
    {
        this->x = x;
    }
};

namespace LD
{
    namespace Detail
    {
        template<typename T, LD::UInteger Size>
        struct IsImmutable<LD::StaticArray<T,Size>>
        {
            constexpr static bool value = true;
        };
    }
}


namespace LD
{
    template<typename T, LD::UInteger Size>
    class GenericResize<LD::StaticArray<T,Size>>
    {
    private:
        LD::StaticArray<T,Size> & mStaticArray;
    public:
        GenericResize(LD::StaticArray<T,Size> & array) noexcept:mStaticArray{array}{}


        void Resize(const LD::UInteger size) noexcept
        {
            this->mStaticArray.Resize(size);
        }


    };
}
#endif
