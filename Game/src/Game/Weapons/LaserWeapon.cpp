/*
** Author : Simon AMBROISE
*/

#include <Game/Weapons/LaserWeapon.hpp>

LaserWeapon::LaserWeapon()
{
    this->_attributes["FireRate"] = new Attribute(1.0f);
    this->_attributes["CurrentAmmo"] = new Attribute(100.0f);
    this->_attributes["Damage"] = new Attribute(10.0f);
}

void LaserWeapon::fire(sTransformComponent* playerTransform, sRenderComponent* playerRender, glm::vec3 playerDirection)
{

}

void LaserWeapon::reload()
{

}