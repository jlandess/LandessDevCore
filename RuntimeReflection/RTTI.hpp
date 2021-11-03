//
// Created by phoenixflower on 9/26/21.
//

#ifndef LANDESSDEVCORE_RTTI_HPP
#define LANDESSDEVCORE_RTTI_HPP
//
//  RTTI.h
//  Reflection
//
//  Created by James Landess on 12/6/13.
//  Copyright (c) 2013 LandessDev. All rights reserved.
//

#ifndef __Reflection__RTTI__
#define __Reflection__RTTI__

#include <iostream>
#include <list>
#include <vector>


#include "AbstractProperty.hpp"
namespace LD
{
    namespace RT
    {
        class BaseObject;
        class RTTI;

//-------------------------------------------------------------------------------------------------
        typedef unsigned long			ClassID;
        typedef BaseObject*	(*ClassFactoryFunc)( ClassID );
        typedef bool			(*RegisterReflectionFunc)();

//-------------------------------------------------------------------------------------------------
#define CLASSNAME_LENGTH 32

///////////////////////////////////////////////////////////////////////////////////////////////////
//
// CRTTI
//
// RTTI description structure.
//
///////////////////////////////////////////////////////////////////////////////////////////////////

        class RTTI
        {

        public:

            //----------------------------------------------------------------------------------------------
            // RTTI constructor.
            // The first paramter is a stub. I am not sure why this stub is necessary - removing ths stub will
            // confuse the .NET compiler and produce compile errors with subsequent parameters. If anybody knows
            // why this is so, feel free to e-mail me at dfilion@hotmail.com
            //
            // The RTTI structure constructor takes in the following parameters:
            //		dwStub			Just a stub
            //		CLID			A unique class ID
            //		szClassName		The name of the class type this RTTI structure represents
            //		pBaseClassRTTI	The parent RTTI structure for this RTTI structure
            //		pFactory		A factory function callback for creating an instance of the bound class type
            //		pReflectionFunc	Callback called by the system to register the reflective properties
            RTTI(	unsigned long dwStub, ClassID CLID, const char* szClassName, RTTI* pBaseClassRTTI,
                     ClassFactoryFunc pFactory, RegisterReflectionFunc pReflectionFunc );

            ~RTTI()
            {
                for ( std::list<AbstractProperty*>::iterator it = m_Properties.begin(); it != m_Properties.end(); ++it )
                {
                    delete *it;
                }

                m_Properties.clear();
            }
            //----------------------------------------------------------------------------------------------
            // Fills a vector with all properties of the represented class type, including all ancestor types.
            void	EnumProperties( std::vector<AbstractProperty*>& o_Result );

            //----------------------------------------------------------------------------------------------
            // Returns true if the RTTI structure is of the type specified by CLID.
            inline bool				IsTypeOf( ClassID CLID );

            // Returns true if the RTTI structure is derived from the type specified by CLID.
            inline bool				IsDerivedFrom( ClassID CLID );

            //----------------------------------------------------------------------------------------------
            // Gets base class' RTTI structure.
            inline RTTI*			GetAncestorRTTI();

            //----------------------------------------------------------------------------------------------
            // Gets the class ID.
            inline ClassID			GetCLID();

            //----------------------------------------------------------------------------------------------
            // Gets the class name.
            inline const char*		GetClassName();

            //----------------------------------------------------------------------------------------------
            // Gets the class factory function.
            inline ClassFactoryFunc	GetClassFactory();

            //----------------------------------------------------------------------------------------------
            // Provides access to the properties bound to this run-time type. Does not include ancestor class
            // properties. Use EnumProperties to include ancestor properties.
            inline	std::list<AbstractProperty*>::iterator	GetFirstProperty();
            inline	std::list<AbstractProperty*>::iterator	GetLastProperty();
            inline	std::list<AbstractProperty*>*			GetProperties();

        private:

            //---------------------------------------------------------------------------------------------
            ClassID						m_CLID;									// Class ID
            char						m_szClassName[CLASSNAME_LENGTH];		// Class name
            RTTI*						m_pBaseRTTI;							// Base class RTTI structure
            ClassFactoryFunc			m_pObjectFactory;						// Factory function
            std::list<AbstractProperty*>	m_Properties;							// Property list

        };

        inline RTTI* RTTI::GetAncestorRTTI()
        {
            return m_pBaseRTTI;
        }

        inline ClassFactoryFunc RTTI::GetClassFactory()
        {
            return m_pObjectFactory;
        }

        inline const char* RTTI::GetClassName()
        {
            return m_szClassName;
        }

        inline ClassID RTTI::GetCLID()
        {
            return m_CLID;
        }

        inline std::list<AbstractProperty*>::iterator RTTI::GetFirstProperty()
        {
            return m_Properties.begin();
        }

        inline std::list<AbstractProperty*>* RTTI::GetProperties()
        {
            return &m_Properties;
        }

        inline bool RTTI::IsDerivedFrom( ClassID CLID )
        {
            if( m_CLID == CLID )
                return true;
            else if( m_pBaseRTTI )
                return m_pBaseRTTI->IsDerivedFrom( CLID );

            return false;
        }

        inline bool RTTI::IsTypeOf( ClassID CLID )
        {
            return m_CLID == CLID;
        }

        inline std::list<AbstractProperty*>::iterator RTTI::GetLastProperty()
        {
            return m_Properties.end();
        }
    }
}
//-------------------------------------------------------------------------------------------------

#endif /* defined(__Reflection__RTTI__) */

#endif //LANDESSDEVCORE_RTTI_HPP
