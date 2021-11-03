//
// Created by phoenixflower on 9/26/21.
//

//
//  RTTI.cpp
//  Reflection
//
//  Created by James Landess on 12/6/13.
//  Copyright (c) 2013 LandessDev. All rights reserved.
//
#include <string.h>
#include "RTTI.hpp"
namespace LD
{
    namespace RT
    {
        RTTI::RTTI(	unsigned long dwStub, ClassID CLID, const char* szClassName, RTTI* pBaseClassRTTI,
                       ClassFactoryFunc pFactory, RegisterReflectionFunc pReflectionFunc ) :
                m_CLID			( CLID				),
                m_pBaseRTTI		( pBaseClassRTTI	),
                m_pObjectFactory( pFactory			)
        {
            //ASSERT( CLID != 0 );
            //ASSERT( strlen(szClassName) <= CLASSNAME_LENGTH );
            strcpy( m_szClassName, szClassName );

            if ( pReflectionFunc )
                pReflectionFunc();
        }

        void RTTI::EnumProperties( std::vector<AbstractProperty*>& o_Result )
        {
            if ( m_pBaseRTTI )
                m_pBaseRTTI->EnumProperties( o_Result );
            for ( std::list<AbstractProperty*>::iterator it = m_Properties.begin(); it != m_Properties.end(); ++it )
                o_Result.push_back( *it );
        }
    }
}
