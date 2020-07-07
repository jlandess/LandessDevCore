//
//  ElementReference.h
//  DataStructures
//
//  Created by James Landess on 3/20/17.
//  Copyright (c) 2017 James Landess. All rights reserved.
//

#ifndef DataStructures_ElementReference_h
#define DataStructures_ElementReference_h

#include "Definitions/Common.hpp"
#include "Primitives/General/Tuple.hpp"
namespace PDP
{
    //
    template <class T>
    class ElementReference
    {
    private:
        //bool rvalue;
        //bool ownsValue;
        T* value;

    public:

        typedef T ValueType;

        constexpr ElementReference() noexcept ;
        constexpr ElementReference(const ElementReference& other) noexcept ;
        constexpr ElementReference(T& otherValue) noexcept ;
        constexpr ElementReference(const T& otherValue) noexcept ;
        constexpr ElementReference(T * otherValue) noexcept :value(nullptr)//,rvalue(false),ownsValue(false)
        {
            (*this) = otherValue;
        }
        ~ElementReference();

        constexpr ElementReference& operator=(const ElementReference& rhs) noexcept ;
        constexpr ElementReference& operator=(const T& rhs) noexcept ;
        constexpr ElementReference& operator=(T& rhs) noexcept ;
        constexpr ElementReference& operator = (T * otherValue) noexcept
        {
            this->value = otherValue;
            //this->rvalue = false;
            //this->ownsValue = false;
            return (*this);
        }


        //todo add operator forwarding
       // template <class U>
        //operator U&() { return static_cast<U&>(*value); }
        template <class U>
        //operator const U&() const { return static_cast<const U&>(*value); }
        constexpr operator T&() noexcept { return *value; }
        constexpr operator const T&()  const noexcept { return *value; }

        constexpr T * GetPointer() const noexcept {return this->value;}
        constexpr T * operator -> () noexcept {return this->value;}
        constexpr const T * operator ->() const noexcept {return this->value;}


        constexpr T & operator*() noexcept {return (*this->value);}
        constexpr const T & operator*() const noexcept {return (*this->value);}

        constexpr const bool IsValid() const noexcept {return this->value != nullptr;}
    };

    template <class T>
    constexpr ElementReference<T>::ElementReference() noexcept
            : value(nullptr)
    {

    }

    template <class T>
    constexpr ElementReference<T>::ElementReference(const ElementReference& other) noexcept
            : value(other.value)//, ownsValue(false), rvalue(false)
    { }

    template <class T>
    constexpr ElementReference<T>::ElementReference(T& otherValue) noexcept
            : value(&otherValue)//, ownsValue(false), rvalue(false)
    { }

    template <class T>
    constexpr ElementReference<T>::ElementReference(const T& otherValue) noexcept
    {
        const T * data = (const T*)&otherValue;
        this->value = (T*)data;
    }

    template <class T>
    ElementReference<T>::~ElementReference()
    {
    }

    template <class T>
    constexpr ElementReference<T>& ElementReference<T>::operator=(const ElementReference& rhs) noexcept
    {
        /*
        if (this != &rhs)
        {
            if (rhs.rvalue)
            {
                if (value == nullptr)
                {
                    value = new T(*rhs.value);
                    ownsValue = true;
                }
                else
                {
                    *value = *rhs.value;
                }
            }
            else
            {
                value = rhs.value;
                ownsValue = false;
            }
            rvalue = false;
        }
         */
        //this->ownsValue = false;
        //this->rvalue = false;
        this->value = rhs.value;
        return *this;
    }

    template <class T>
    constexpr ElementReference<T>& ElementReference<T>::operator=(const T& rhs) noexcept
    {
        /*
        if (value == nullptr)
        {
            value = new T(rhs);
            ownsValue = true;
        }
        else
        {
            *value = rhs;
        }
        rvalue = false;
         */
        //this->ownsValue = false;
        //this->rvalue = false;
        const T * data = (const T*)&rhs;

        this->value = (T*)data;
        return *this;
    }

    template <class T>
    constexpr ElementReference<T>& ElementReference<T>::operator=(T& rhs) noexcept
    {
        /*
        if (value != nullptr && ownsValue)
        {
            delete value;
            value = nullptr;
        }
         */
        value = &rhs;
        //ownsValue = false;
        //rvalue = false;
        return *this;
    }
}
namespace LD
{
    template<typename T>
    using ElementReference = PDP::ElementReference<T>;

    template<typename T>
    using Ref = PDP::ElementReference<T>;

    template<typename T>
    constexpr const T & Get(const T & stuff) noexcept
    {
        return stuff;
    }

    template<typename T>
    constexpr T & Get(T & stuff) noexcept
    {
        return stuff;
    }

    template<typename T>
    constexpr const T & Get(const LD::ElementReference<T> & elementReference) noexcept
    {
        const T* ptr = (const T*)elementReference.GetPointer();
        return *ptr;
    }

    template<typename T>
    constexpr T & Get(LD::ElementReference<T> & elementReference) noexcept
    {
        T* ptr = ( T*)elementReference.GetPointer();
        return *ptr;
    }

    template<typename ... Arguements,
            typename R = LD::Tuple<LD::Detail::Conditional_T<(LD::Detail::IsLValueReference<Arguements>::value),LD::ElementReference <LD::Detail::Decay_T<Arguements>>,LD::Detail::Decay_T<Arguements>>...>>
    constexpr auto MakeReferenceableTuple(Arguements && ... arguements) noexcept -> R
    {

        return R{LD::Forward<Arguements>(arguements)...};

    }

    template<typename ... Arguements>
    using ReferenceableTuple = LD::Tuple<LD::Detail::Conditional_T<(LD::Detail::IsLValueReference<Arguements>::value),LD::ElementReference <LD::Detail::Decay_T<Arguements>>,LD::Detail::Decay_T<Arguements>>...>;

}
#endif
