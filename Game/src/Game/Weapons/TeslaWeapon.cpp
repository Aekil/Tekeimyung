/*
** Author : Simon AMBROISE
*/

#include <Game/Weapons/TeslaWeapon.hpp>

TeslaWeapon::TeslaWeapon()
{
    this->_attributes["FireRate"] = new Attribute(1.0f);
    this->_attributes["MaxAmmo"] = new Attribute(100.0f);
    this->_attributes["CurrentAmmo"] = new Attribute(100.0f);
    this->_attributes["Damage"] = new Attribute(10.0f);
}

void TeslaWeapon::start()
{

}

void TeslaWeapon::update(float dt)
{

}

void TeslaWeapon::fire()
{

}

void TeslaWeapon::reload()
{

}