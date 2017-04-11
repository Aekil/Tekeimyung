/*
** Author : Simon AMBROISE
*/

#include <Game/Weapons/DefaultWeapon.hpp>

DefaultWeapon::DefaultWeapon()
{
    this->_attributes["FireRate"] = new Attribute(1.0f);
    this->_attributes["MaxAmmo"] = new Attribute(100.0f);
    this->_attributes["CurrentAmmo"] = new Attribute(100.0f);
    this->_attributes["Damage"] = new Attribute(10.0f);
}

void DefaultWeapon::start()
{

}

void DefaultWeapon::update(float dt)
{

}

void DefaultWeapon::fire()
{

}

void DefaultWeapon::reload()
{

}