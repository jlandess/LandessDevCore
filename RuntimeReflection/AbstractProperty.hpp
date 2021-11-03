//
// Created by phoenixflower on 9/26/21.
//

#ifndef LANDESSDEVCORE_ABSTRACTPROPERTY_HPP
#define LANDESSDEVCORE_ABSTRACTPROPERTY_HPP
//
//  AbstractProperty.h
//  Reflection
//
//  Created by James Landess on 12/6/13.
//  Copyright (c) 2013 LandessDev. All rights reserved.
//


#include "PropertyTypes.hpp"

///////////////////////////////////////////////////////////////////////////////////////////////////
//
// CAbstractProperty
//
// Base class for all properties.
//
///////////////////////////////////////////////////////////////////////////////////////////////////
namespace LD
{
    namespace RT
    {
        class AbstractProperty
        {

        public:

            //----------------------------------------------------------------------------------------------
            // Constructor. Takes in property name.
            inline AbstractProperty( const char* szName );

            //----------------------------------------------------------------------------------------------
            // Returns the name of this property.
            inline const char*              GetName() const;

            //----------------------------------------------------------------------------------------------
            // Returns the type of this property.
            virtual ePropertyType			GetType() const = 0;

            inline virtual ~AbstractProperty(){}
        protected :

            const char*				m_szName;			// Property name


        };

        AbstractProperty::AbstractProperty( const char* szName ) :
                m_szName		( szName			)
        {

        }

        const char* AbstractProperty::GetName() const
        {
            return m_szName;
        }
    }
}


#endif //LANDESSDEVCORE_ABSTRACTPROPERTY_HPP
