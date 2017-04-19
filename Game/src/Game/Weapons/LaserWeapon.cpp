/*
** Author : Simon AMBROISE
*/

#include <Engine/Physics/Physics.hpp>
#include <Engine/EntityFactory.hpp>

#include <Game/Weapons/LaserWeapon.hpp>
#include <Game/Scripts/Enemy.hpp>

const std::string LaserWeapon::Name = "Laser";

LaserWeapon::LaserWeapon()
{
    this->_attributes["FireRate"] = new Attribute(0.0f);
    this->_attributes["Ammo"] = new Attribute(100.0f);
    this->_attributes["Damage"] = new Attribute(1.0f);
    this->_attributes["ExplosionPercentage"] = new Attribute(1.0 / 100.0);
    this->_attributes["MaxRange"] = new Attribute(150.0f);

    _material = ResourceManager::getInstance()->getResource<Material>("weapon_laser.mat");
}

void    LaserWeapon::fire(Player* player, sTransformComponent* playerTransform, sRenderComponent* playerRender, glm::vec3 playerDirection)
{
    this->_attributes["Ammo"]->addBonus(Bonus(-1));

    if (this->_attributes["Ammo"]->getFinalValue() == 0)
        this->reload();

    if (this->_laser == nullptr)
        this->_laser = EntityFactory::createEntity("LASER_CYLINDER");

    glm::vec3 playerPos = playerTransform->getPos() + (playerRender->getModel()->getSize().y / 2.0f * playerTransform->getScale().y);
    this->_laser->getComponent<sTransformComponent>()->setPos(playerPos);
    this->_laser->getComponent<sTransformComponent>()->setScale(glm::vec3{ 0.2f, this->_attributes["MaxRange"]->getFinalValue() / SIZE_UNIT, 0.2f });
    this->_laser->getComponent<sTransformComponent>()->setRotation(glm::vec3(90.0f, playerTransform->getRotation().y, 0.0f));

    if (!this->_pierceEnemy)
        this->fireOneEnemy(player, playerTransform, playerRender, playerDirection);
    else
        this->fireMultipleEnemy(player, playerTransform, playerRender, playerDirection);
}

Entity* LaserWeapon::farestEnemy(sTransformComponent* playerTransform, std::vector<Entity*> entities)
{
    float distance = std::numeric_limits<float>::min();
    Entity *farestEntity = nullptr;

    for (auto &entity : entities)
    {
        auto distanceBetween = glm::distance(entity->getComponent<sTransformComponent>()->getPos(), playerTransform->getPos());
        if (distanceBetween > distance)
        {
            distance = distanceBetween;
            farestEntity = entity;
        }
    }

    return farestEntity;
}

void LaserWeapon::fireMultipleEnemy(Player* player, sTransformComponent* playerTransform, sRenderComponent* playerRender, glm::vec3 playerDirection)
{
    glm::vec3 playerPos = playerTransform->getPos() + (playerRender->getModel()->getSize().y / 2.0f * playerTransform->getScale().y);
    Ray raycastHit = Ray(playerPos, glm::vec3{ playerDirection.x, 0.0f, playerDirection.z });

    std::vector<Entity*> hitedEntities;
    if (Physics::raycastAll(raycastHit, hitedEntities, std::vector<Entity*>{ player->getEntity(), this->_laser }, this->_attributes["MaxRange"]->getFinalValue()))
    {
        if (hitedEntities.size() == 0)
        {
            auto em = EntityFactory::getBindedEntityManager();

            em->destroyEntityRegister(this->_laser);
            this->_laser = nullptr;
            this->_oldEntitiesHited.clear();
            return;
        }

        auto farestEntity = this->farestEnemy(playerTransform, hitedEntities);

        if (this->_laser == nullptr)
            this->_laser = EntityFactory::createEntity("LASER_CYLINDER");

        this->_laser->getComponent<sTransformComponent>()->setPos(playerPos);
        this->_laser->getComponent<sTransformComponent>()->setScale(glm::vec3{ 0.2f, glm::distance(farestEntity->getComponent<sTransformComponent>()->getPos(), playerTransform->getPos()) / SIZE_UNIT, 0.2f });
        this->_laser->getComponent<sTransformComponent>()->setRotation(glm::vec3(90.0f, playerTransform->getRotation().y, 0.0f));

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
    if (Physics::raycast(raycastHit, &hitedEntity, std::vector<Entity*>{ player->getEntity(), this->_laser }, this->_attributes["MaxRange"]->getFinalValue()))
    {
        if (hitedEntity != nullptr && hitedEntity->getTag() == "Enemy")
        {
            if (this->_laser == nullptr)
                this->_laser = EntityFactory::createEntity("LASER_CYLINDER");

            this->_laser->getComponent<sTransformComponent>()->setPos(playerPos);
            this->_laser->getComponent<sTransformComponent>()->setScale(glm::vec3{ 0.2f, glm::distance(hitedEntity->getComponent<sTransformComponent>()->getPos(), playerTransform->getPos()) / SIZE_UNIT, 0.2f });
            this->_laser->getComponent<sTransformComponent>()->setRotation(glm::vec3(90.0f, playerTransform->getRotation().y, 0.0f));

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
            auto em = EntityFactory::getBindedEntityManager();

            em->destroyEntityRegister(this->_laser);
            this->_laser = nullptr;
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