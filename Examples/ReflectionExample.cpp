//
// Created by phoenixflower on 5/13/20.
//
#include "ReflectionExample.hpp"
#include "Reflection/reflectable.hpp"
#include "Functor/FunctionView.h"
#include "Primitives/General/Immutable.hpp"
#include "IO/Database.hpp"
namespace LD
{
    class Square: public LD::Reflectable<
            decltype("Square"_ts)(
                    decltype("Length"_ts),      LD::Float,
                    decltype("area"_ts),LD::FunctionView<LD::Float ()>)>
    {
    public:
        inline Square()
        {
            (*this)["Length"_ts] = 0;
            (*this)["area"_ts] = LD::FunctionView<LD::Float()>(this,&Square::GetArea);
        }

        inline Square(const LD::Float & length)
        {
            (*this)["Length"_ts] = length;
            (*this)["area"_ts] = LD::FunctionView<LD::Float()>(this,&Square::GetArea);
        }
        LD::Float GetArea() const
        {
            return (*this)["Length"_ts] * (*this)["Length"_ts];
        }
    };

    class Triangle: public LD::Reflectable<
            decltype("Triangle"_ts)(
                    decltype("Base"_ts),      LD::Float,
                    decltype("Height"_ts),      LD::Float,
                    decltype("area"_ts),LD::FunctionView<LD::Float ()>)>
    {
    public:
        inline Triangle()
        {
            (*this)["Base"_ts] = 0;
            (*this)["Height"_ts] = 0;
            (*this)["area"_ts] = LD::FunctionView<LD::Float()>(this,&Triangle::GetArea);
        }
        inline Triangle(const LD::Float & base, const LD::Float & height)
        {
            (*this)["Base"_ts] = base;
            (*this)["Height"_ts] = height;
            (*this)["area"_ts] = LD::FunctionView<LD::Float()>(this,&Triangle::GetArea);
        }
        LD::Float GetArea() const
        {
            return 0.5 * (*this)["Base"_ts] * (*this)["Height"_ts];
        }

    };
    class Circle: public LD::Reflectable<
            decltype("Circle"_ts)(
                    decltype("Radius"_ts),      LD::Float,
                    decltype("area"_ts),LD::FunctionView<LD::Float ()>)>
    {
    public:
        inline Circle()
        {
            (*this)["Radius"_ts] = 0;
            (*this)["area"_ts] = LD::FunctionView<LD::Float()>(this,&Circle::GetArea);
        }
        inline Circle(const LD::Float & radius)
        {
            (*this)["Radius"_ts] = radius;
            (*this)["area"_ts] = LD::FunctionView<LD::Float()>(this,&Circle::GetArea);
        }
        LD::Float GetArea() const
        {
            return 3.14 * (*this)["Radius"_ts] * (*this)["Radius"_ts];
        }
    };

    class Pyramid: public LD::Reflectable<
            decltype("Pyramid"_ts)(
                    decltype("Base"_ts),      LD::Square,
                    decltype("Side"_ts),      LD::Triangle,
                    decltype("area"_ts),LD::FunctionView<LD::Float ()>)>
    {
    public:
        inline Pyramid()
        {
            (*this)["Base"_ts] = LD::Square{};
            (*this)["Side"_ts] =  LD::Triangle{};
            (*this)["area"_ts] = LD::FunctionView<LD::Float()>(this,&Pyramid::GetArea);
        }
        inline Pyramid(const LD::Square & width, const LD::Triangle & height)
        {
            (*this)["Base"_ts] = LD::Square{width};
            (*this)["Side"_ts] =  LD::Triangle{height};
            (*this)["area"_ts] = LD::FunctionView<LD::Float()>(this,&Pyramid::GetArea);
        }
        LD::Float GetArea() const
        {
            return 5;
        }
    };
    namespace Example
    {
        void ReflectionExample()
        {
            Database currentData{{"database"}};
            Square currentSquare;
            Pyramid currentPyramid;
            currentPyramid["Base"_ts]["Length"_ts] = 17*7;
            currentSquare["Length"_ts] = 17.96;
            currentData.Insert("key"_ts,currentSquare);
            //currentData.Insert("pyramid"_ts,currentPyramid);
            currentData.Fetch<Square>("key"_ts);


            /*
            printf("%s\n","beginning of reflection example");
            unqlite *pDb;
            int rc;
            rc = unqlite_open(&pDb,"test.db",UNQLITE_OPEN_CREATE);
            Square currentSquare;
            Pyramid currentPyramid;
            currentPyramid["Base"_ts]["Length"_ts] = 17*7;
            currentSquare["Length"_ts] = 17.96;

            using ClassName = decltype(Square::GetClassNameTypeString());
            SerializeObject(pDb,"key"_ts,currentSquare);
            SerializeObject(pDb,"pyramid"_ts,currentPyramid);

            using KeyTest = decltype("key.Square.Length"_ts);
            using PyramidTest = decltype("pyramid.Pyramid.Base.Square.Length"_ts);
            if( rc != UNQLITE_OK ){ return; }

            auto fetchCallback = [](const void * data, unsigned int dataSize, void * inputPointer)->int
            {
                printf("found it %s\n",(char*)data);
                //std::cout << "fetch being called correctly " << (const char*)data << "  size : " << dataSize << "   " << strlen((const char*)data) <<  std::endl;
                return 0;
            };
            unqlite_kv_fetch_callback(pDb,PyramidTest::data(),PyramidTest::size(),fetchCallback, nullptr);

            //unqlite_kv_store(pDb,key.c_str(),key.size(),cachedDocument.c_str(),cachedDocument.size());
            unqlite_commit(pDb);
            unqlite_close(pDb);
             */
        }
    }
}