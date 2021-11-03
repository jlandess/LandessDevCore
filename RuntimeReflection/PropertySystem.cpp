//
// Created by phoenixflower on 9/26/21.
//

#include "PropertySystem.hpp"

namespace LD
{
    namespace RT
    {
        std::list<AbstractProperty*> PropertySystem::ms_Properties;

        PropertySystem::PropertySystem()
        {
        }

        PropertySystem::~PropertySystem()
        {
            // Destroy properties
            for ( std::list<AbstractProperty*>::iterator it = ms_Properties.begin(); it != ms_Properties.end(); ++it )
                delete *it;
            ms_Properties.clear();
        }
    }
}
