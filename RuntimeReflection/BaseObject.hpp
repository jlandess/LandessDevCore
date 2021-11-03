//
// Created by phoenixflower on 9/26/21.
//

#ifndef LANDESSDEVCORE_BASEOBJECT_HPP
#define LANDESSDEVCORE_BASEOBJECT_HPP
#include "RTTI.hpp"
#include "RTTIClass.hpp"
// CNullClass
//
// CNullClass is an empty class with no attached RTTI information. We use this class as an ancestor
// to our CBaseObject which is the true ultimate ancestor for all our classes. Without the null class
// we would have no templater parameter for CBaseObject's ancestor. CNullClass itself has no RTTI support.
//
///////////////////////////////////////////////////////////////////////////////////////////////////
namespace LD
{
    namespace RT
    {
        class NullClass
        {

        protected :

            // It is necessary to implement this to avoid compilation errors in the templatized RTTI code
            // In any case no RTTI support is provided from this class
            static inline RTTI* GetClassRTTI()			{ return NULL;									};

        };

///////////////////////////////////////////////////////////////////////////////////////////////////
//
// CBaseObject
//
// Ultimate ancestor for all classes. CBaseObject implements no specific functionality, apart from
// the fact it provides the ultimate ancestor RTTI for all other classes. The number in the
// template parameters is a random user-generated unique identifier.
//
///////////////////////////////////////////////////////////////////////////////////////////////////

        class BaseObject : public RTTIClass<BaseObject, NullClass, 0x1622afe>
        {

        };
    }
}

#endif //LANDESSDEVCORE_BASEOBJECT_HPP
