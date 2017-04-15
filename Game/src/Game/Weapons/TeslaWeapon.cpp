/*
** Author : Simon AMBROISE
*/

#include <vector>

#include <Engine/Physics/Physics.hpp>

#include <Game/Weapons/TeslaWeapon.hpp>
#include <Game/Scripts/Enemy.hpp>

TeslaWeapon::TeslaWeapon()
{
    this->_attributes["FireRate"] = new Attribute(1.0f);
    this->_attributes["CurrentAmmo"] = new Attribute(100.0f);
    this->_attributes["Damage"] = new Attribute(60.0f);
    this->_attributes["MaxRange"] = new Attribute(35.0f);
}

void TeslaWeapon::fire(sTransformComponent* playerTransform, sRenderComponent* playerRender, glm::vec3 playerDirection)
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

void TeslaWeapon::reload()
{

}

void TeslaWeapon::upgradeByLevel()
{

}