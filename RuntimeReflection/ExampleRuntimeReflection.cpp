//
// Created by phoenixflower on 9/27/21.
//

#include "PropertySystem.hpp"
#include "ExampleRuntimeReflection.hpp"


CWeapon::CWeapon()
{
    m_szName = "Big Ass Whooping Weapon";
    m_iMinDmg = 10;
    m_iMaxDmg = 140;
    m_fAreaOfEffect = 4.5f;
}

void CWeapon::RegisterReflection()
{
    RegisterProperty<char*>( "Name", &CWeapon::GetName, &CWeapon::SetName );
    RegisterProperty<int>( "MinDmg", &CWeapon::GetMinDmg, &CWeapon::SetMinDmg );
    RegisterProperty<int>( "MaxDmg", &CWeapon::GetMaxDmg, &CWeapon::SetMaxDmg );
    RegisterProperty<float>( "AreaOfEffect", &CWeapon::GetAreaOfEffect, &CWeapon::SetAreaOfEffect );
    RegisterProperty<long>("pooof", &CWeapon::GetChar, &CWeapon::SetChar);
}

