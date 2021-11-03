//
// Created by phoenixflower on 9/26/21.
//

#ifndef LANDESSDEVCORE_PROPERTY_HPP
#define LANDESSDEVCORE_PROPERTY_HPP
#include "TypedProperty.hpp"
namespace LD
{
    namespace RT
    {
        template <class OwnerType, class T> class Property : public TypedProperty<T>
        {

        public:

            //----------------------------------------------------------------------------------------------
            typedef T		(OwnerType::*GetterType)();				// Getter function
            typedef void	(OwnerType::*SetterType)( T Value );	// Setter function

            //----------------------------------------------------------------------------------------------
            // Constructor. Takes in property's name, getter and setter functions.
            inline Property( const char* szName, GetterType Getter, SetterType Setter );

            //----------------------------------------------------------------------------------------------
            // Determines the value of this property.
            virtual T		GetValue( BaseObject* pObject );
            virtual void	SetValue( BaseObject* pObject, T Value );

        protected:

            GetterType		m_Getter;
            SetterType		m_Setter;

        };

        template <class OwnerType, class T>
        inline Property<OwnerType, T>::Property( const char* szName, GetterType Getter, SetterType Setter ) :
                TypedProperty<T>	( szName		),
                m_Getter			( Getter		),
                m_Setter			( Setter		)
        {
        }

        template <class OwnerType, class T>
        T Property<OwnerType, T>::GetValue( BaseObject* pOwner )
        {
            return (((OwnerType*)pOwner)->*m_Getter)();
        }

        template <class OwnerType, class T>
        void Property<OwnerType, T>::SetValue(BaseObject* pOwner, T Value )
        {
            if ( !m_Setter)
            {
                //ASSERT( false ); // Cannot write to a read-only property
                return;
            }
            (((OwnerType*)pOwner)->*m_Setter)( Value );
        }
    }
}

#endif //LANDESSDEVCORE_PROPERTY_HPP
