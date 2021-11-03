//
// Created by phoenixflower on 9/26/21.
//

#ifndef LANDESSDEVCORE_PROPERTYSYSTEM_HPP
#define LANDESSDEVCORE_PROPERTYSYSTEM_HPP
#include <list>
#include "AbstractProperty.hpp"
namespace LD
{
    namespace RT
    {
        class PropertySystem
        {

        public :

            //----------------------------------------------------------------------------------------------
            // Constructor
            PropertySystem();
            //----------------------------------------------------------------------------------------------
            // Destructor
            virtual ~PropertySystem();

            //----------------------------------------------------------------------------------------------
            // Returns global list of properties.
            inline	static	std::list<LD::RT::AbstractProperty*>*	GetProperties();

        private :

            static	std::list<LD::RT::AbstractProperty*>	ms_Properties;

        };

        inline std::list<LD::RT::AbstractProperty*>* PropertySystem::GetProperties()
        {
            return &ms_Properties;
        }
    }
}

#endif //LANDESSDEVCORE_PROPERTYSYSTEM_HPP
