//
// Created by phoenixflower on 9/26/21.
//

#ifndef LANDESSDEVCORE_TYPEDPROPERTY_HPP
#define LANDESSDEVCORE_TYPEDPROPERTY_HPP
#include "AbstractProperty.hpp"


namespace LD
{
    namespace RT
    {
        class BaseObject;

///////////////////////////////////////////////////////////////////////////////////////////////////
//
// CTypedProperty
//
// This intermediate class defines a property that is typed, but not bound as a member of a particular
// class.
//
///////////////////////////////////////////////////////////////////////////////////////////////////

        template <class T> class TypedProperty: public AbstractProperty
        {

        public:

            //----------------------------------------------------------------------------------------------
            // Constructor. Takes in property name.
            inline TypedProperty( const char* szName );

            //----------------------------------------------------------------------------------------------
            // Returns the type of this property.
            virtual ePropertyType	GetType() const;

            //----------------------------------------------------------------------------------------------
            // Determines the value of this property.
            virtual T		GetValue( BaseObject* pObject ) = 0;
            virtual void	SetValue( BaseObject* pObject, T Value ) = 0;

        };

        template <class T>
        inline TypedProperty<T>::TypedProperty( const char* szName ) :
                AbstractProperty( szName			)
        {
        }

        template <class T>
        ePropertyType TypedProperty<T>::GetType() const
        {
            return PropertyType<T>::GetTypeID();
        }
    }
}

#endif //LANDESSDEVCORE_TYPEDPROPERTY_HPP
