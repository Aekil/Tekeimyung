/*
** Author : Simon AMBROISE
*/

#include <Engine/Physics/Physics.hpp>

#include <Game/Weapons/LaserWeapon.hpp>
#include <Game/Scripts/Enemy.hpp>

LaserWeapon::LaserWeapon()
{
    this->_attributes["FireRate"] = new Attribute(0.0f);
    this->_attributes["CurrentAmmo"] = new Attribute(100.0f);
    this->_attributes["Damage"] = new Attribute(1.0f);
}

void    LaserWeapon::fire(Player* player, sTransformComponent* playerTransform, sRenderComponent* playerRender, glm::vec3 playerDirection)
{
    glm::vec3 playerPos = playerTransform->getPos() + (playerRender->getModel()->getSize().y / 2.0f * playerTransform->getScale().y);
    Ray raycastHit = Ray(playerPos, glm::vec3{ playerDirection.x, 0.0f, playerDirection.z });

    std::vector<Entity*> hitedEntities;
    if (Physics::raycastAll(raycastHit, hitedEntities))
    {
        for (auto entity : hitedEntities)
        {
            if (entity->getTag() == "Enemy")
            {
                auto entityScriptComponent = entity->getComponent<sScriptComponent>();
                if (entityScriptComponent == nullptr)
                    continue;

                auto enemyScript = entityScriptComponent->getScript<Enemy>("Enemy");

                enemyScript->takeDamage(this->_attributes["Damage"]->getFinalValue());
            }
        }
    }
}

void LaserWeapon::upgradeByLevel()
{
    switch (this->_level)
    {
    case 1:
        break;
    case 2:
        break;    
    case 3:
        break;
    default:
        break;
    }
}

void    LaserWeapon::reload()
{

}