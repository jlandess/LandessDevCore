//
// Created by phoenixflower on 9/26/21.
//

#ifndef LANDESSDEVCORE_RTTICLASS_HPP
#define LANDESSDEVCORE_RTTICLASS_HPP
//
//  RTTIClass.h
//  Reflection
//
//  Created by James Landess on 12/6/13.
//  Copyright (c) 2013 LandessDev. All rights reserved.
//


#include <typeinfo>
#include "RTTI.hpp"
#include "Property.hpp"
#include "PropertySystem.hpp"
#include "Memory/shared_ptr.hpp"

///////////////////////////////////////////////////////////////////////////////////////////////////
//
// CRTTIClass
//
// CRTTIClass is the RTTI "sandwich class" being used to augment a class with RTTI support. Classes
// supporting RTTI need to derive from this class, with their ancestor specified as the BaseClass
// template parameter.
//
///////////////////////////////////////////////////////////////////////////////////////////////////
namespace LD
{
    namespace RT
    {
        template <class T, class BaseClass, ClassID CLID> class RTTIClass : public BaseClass
        {

        public :

            const static	ClassID	ClassCLID	= CLID;

            //----------------------------------------------------------------------------------------------
            // Constructor
            RTTIClass();

            //----------------------------------------------------------------------------------------------
            // Default factory function. Creates an instance of T. Called by the system to dynamically create
            // class instances from class IDs.
            static T*	Create();

            static LD::SharedPointer<T>	CreateShared(LD::Mem::MemoryResource * resource = LD::Mem::GetNewDeleteResource());

            //----------------------------------------------------------------------------------------------
            // Default reflection registration function. Does nothing by default.
            static void	RegisterReflection();

            //----------------------------------------------------------------------------------------------
            // Registers a property. Takes in the property name, its getter and setter functions, and the property
            // type as a template parameter. Should be called from within a user-defined RegisterReflection function.
            template <class PropertyType>
            static	void RegisterProperty(	const char* szName, typename Property<T, PropertyType>::GetterType Getter,
                                                typename Property<T, PropertyType>::SetterType Setter , LD::Mem::MemoryResource * resource = LD::Mem::GetNewDeleteResource())
            {
                Property<T, PropertyType>* pProperty = new Property<T, PropertyType>( szName, Getter, Setter );
                T::GetClassRTTI()->GetProperties()->push_back( pProperty );
                //PropertySystem::GetProperties()->push_back( nullptr );
                //std::cout << PropertySystem::GetProperties()->size() << std::endl;
            }

            //----------------------------------------------------------------------------------------------
            // Returns RTTI info associated with this class type.
            static inline RTTI* GetClassRTTI()
            {
                return &ms_RTTI;
            }

            //----------------------------------------------------------------------------------------------
            // Returns RTTI info associated with this class instance.
            virtual RTTI* GetRTTI()
            {
                return &ms_RTTI;
            }

        protected :

            static RTTI	ms_RTTI;				// RTTI structure

        };

// The first parameter (0) is a stub. See CRTTI constructor for details.
        template <class T, class BaseClass, ClassID CLID> RTTI RTTIClass<T, BaseClass, CLID>::ms_RTTI
                ( 0, CLID, typeid(T).name(), BaseClass::GetClassRTTI(), (ClassFactoryFunc)T::Create,
                  (RegisterReflectionFunc)T::RegisterReflection );

        template <class T, class BaseClass, ClassID CLID>
        RTTIClass<T, BaseClass, CLID>::RTTIClass()
        {
        }

        template <class T, class BaseClass, ClassID CLID>
        T* RTTIClass<T, BaseClass, CLID>::Create()
        {
            return new T();
        }
        template <class T, class BaseClass, ClassID CLID>
        LD::SharedPointer<T> RTTIClass<T, BaseClass, CLID>::CreateShared(LD::Mem::MemoryResource *resource)
        {
            return LD::MakeShared<T>();
        }

        template <class T, class BaseClass, ClassID CLID>
        void RTTIClass<T, BaseClass, CLID>::RegisterReflection()
        {
        }
    }
}


#endif //LANDESSDEVCORE_RTTICLASS_HPP
