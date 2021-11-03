//
// Created by phoenixflower on 9/27/21.
//

#ifndef LANDESSDEVCORE_EXAMPLERUNTIMEREFLECTION_HPP
#define LANDESSDEVCORE_EXAMPLERUNTIMEREFLECTION_HPP
#include "BaseObject.hpp"
#include "RTTIClass.hpp"
class CWeapon : public LD::RT::RTTIClass<CWeapon, LD::RT::BaseObject, 0x87feda90>
{

public :

    //----------------------------------------------------------------------------------------------
    CWeapon();

    //----------------------------------------------------------------------------------------------
    // Sample accessors.
    char*	GetName()						{ return m_szName;			};
    void	SetName( char* szValue )		{ m_szName = szValue;		};

    int		GetMinDmg()						{ return m_iMinDmg;			};
    void	SetMinDmg( int iValue )			{ m_iMinDmg = iValue;		};

    int		GetMaxDmg()						{ return m_iMaxDmg;			};
    void	SetMaxDmg( int iValue )			{ m_iMaxDmg = iValue;		};

    float	GetAreaOfEffect()				{ return m_fAreaOfEffect;	};
    void	SetAreaOfEffect( float fValue )	{ m_fAreaOfEffect = fValue;	};

    void SetChar(const long character){};
    long GetChar() {return 99;};


    //----------------------------------------------------------------------------------------------
    // Properties are registered by implementing this method in each class.
    static	void RegisterReflection();

private :

    char*	m_szName;
    int		m_iMinDmg;
    int		m_iMaxDmg;
    float	m_fAreaOfEffect;
};
#endif //LANDESSDEVCORE_EXAMPLERUNTIMEREFLECTION_HPP
