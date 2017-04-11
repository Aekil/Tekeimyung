/*
** Author : Simon AMBROISE
*/

#include <Game/Weapons/TeslaWeapon.hpp>

TeslaWeapon::TeslaWeapon()
{
    this->_attributes["FireRate"] = new Attribute(1.0f);
    this->_attributes["CurrentAmmo"] = new Attribute(100.0f);
    this->_attributes["Damage"] = new Attribute(10.0f);
}

void TeslaWeapon::fire(sTransformComponent* playerTransform, sRenderComponent* playerRender, glm::vec3 playerDirection)
{

}

void TeslaWeapon::reload()
{

}