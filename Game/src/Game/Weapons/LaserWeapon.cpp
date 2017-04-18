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
    this->_attributes["ExplosionPercentage"] = new Attribute(1.0 / 100.0);
}

void    LaserWeapon::fire(Player* player, sTransformComponent* playerTransform, sRenderComponent* playerRender, glm::vec3 playerDirection)
{
    if (!this->_pierceEnemy)
        this->fireOneEnemy(player, playerTransform, playerRender, playerDirection);
    else
        this->fireMultipleEnemy(player, playerTransform, playerRender, playerDirection);
}

void LaserWeapon::fireMultipleEnemy(Player* player, sTransformComponent* playerTransform, sRenderComponent* playerRender, glm::vec3 playerDirection)
{
    glm::vec3 playerPos = playerTransform->getPos() + (playerRender->getModel()->getSize().y / 2.0f * playerTransform->getScale().y);
    Ray raycastHit = Ray(playerPos, glm::vec3{ playerDirection.x, 0.0f, playerDirection.z });

    std::vector<Entity*> hitedEntities;
    if (Physics::raycastAll(raycastHit, hitedEntities, std::vector<Entity*>{ player->getEntity() }, false))
    {
        if (hitedEntities.size() == 0)
        {
            this->_oldEntitiesHited.clear();
            return;
        }

        for (auto hitedEntity : hitedEntities)
        {
            if (hitedEntity != nullptr && hitedEntity->getTag() == "Enemy")
            {
                auto entityScriptComponent = hitedEntity->getComponent<sScriptComponent>();
                if (entityScriptComponent == nullptr)
                    return;

                auto enemyScript = entityScriptComponent->getScript<Enemy>("Enemy");

                if (this->_timeExplosion)
                {
                    if (std::find(this->_oldEntitiesHited.begin(), this->_oldEntitiesHited.end(), hitedEntity) != this->_oldEntitiesHited.end())
                        this->_attributes["ExplosionPercentage"]->addModifier(Modifier(this->_timeExplosionPercent));
                    else
                        this->_attributes["ExplosionPercentage"]->clearAll();
                }

                enemyScript->takeDamage(this->_attributes["Damage"]->getFinalValue());

                this->_oldEntitiesHited.push_back(hitedEntity);
            }
        }
    }
}

void LaserWeapon::fireOneEnemy(Player* player, sTransformComponent* playerTransform, sRenderComponent* playerRender, glm::vec3 playerDirection)
{
    glm::vec3 playerPos = playerTransform->getPos() + (playerRender->getModel()->getSize().y / 2.0f * playerTransform->getScale().y);
    Ray raycastHit = Ray(playerPos, glm::vec3{ playerDirection.x, 0.0f, playerDirection.z });

    Entity* hitedEntity;
    if (Physics::raycast(raycastHit, &hitedEntity, std::vector<Entity*>{ player->getEntity() }, false))
    {
        if (hitedEntity != nullptr && hitedEntity->getTag() == "Enemy")
        {
            auto entityScriptComponent = hitedEntity->getComponent<sScriptComponent>();
            if (entityScriptComponent == nullptr)
                return;

            auto enemyScript = entityScriptComponent->getScript<Enemy>("Enemy");

            if (this->_timeDamage)
            {
                if (std::find(this->_oldEntitiesHited.begin(), this->_oldEntitiesHited.end(), hitedEntity) != this->_oldEntitiesHited.end())
                    this->_attributes["Damage"]->addModifier(Modifier(this->_timeDamageDealt));
                else
                    this->_attributes["Damage"]->clearAll();
            }

            enemyScript->takeDamage(this->_attributes["Damage"]->getFinalValue());

            this->_oldEntitiesHited.push_back(hitedEntity);
        }
        else
        {
            this->_oldEntitiesHited.clear();
        }
    }
}

void LaserWeapon::upgradeByLevel()
{
    switch (this->_level)
    {
    case 1:
        this->_timeDamage = true;
        break;
    case 2:
        this->_timeDamage = false;
        this->_pierceEnemy = true;
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