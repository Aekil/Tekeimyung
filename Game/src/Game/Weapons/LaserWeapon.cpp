/*
** Author : Simon AMBROISE
*/

#include <Engine/Physics/Physics.hpp>
#include <Engine/EntityFactory.hpp>

#include <Game/Weapons/LaserWeapon.hpp>
#include <Game/Character/Enemy.hpp>

const std::string LaserWeapon::Name = "Laser";

LaserWeapon::LaserWeapon()
{
    this->_attributes["FireRate"] = new Attribute(0.0f);
    this->_attributes["Ammo"] = new Attribute(100.0f);
    this->_attributes["Damage"] = new Attribute(1.25f);
    this->_attributes["MaxRange"] = new Attribute(150.0f);
    this->_attributes["ReloadingTime"] = new Attribute(1.5f);

    _shootSound = EventSound::getEventByEventType(eEventSound::PLAYER_SHOOT_LAZR);
    _material = ResourceManager::getInstance()->getResource<Material>("weapon_laser.mat");
}

void    LaserWeapon::fire(Player* player, sTransformComponent* playerTransform, sRenderComponent* playerRender, glm::vec3 playerDirection)
{
    this->_attributes["Ammo"]->addBonus(Bonus(-1));

    if (this->_attributes["Ammo"]->getFinalValue() == 0)
        this->_reloading = true;

    if (this->_laser == nullptr)
        this->_laser = EntityFactory::createEntity("LASER_CYLINDER");

    glm::vec3 playerPos = glm::vec3{ playerTransform->getPos().x, playerTransform->getPos().y + (playerRender->getModel()->getSize().y / 2.0f * playerTransform->getScale().y), playerTransform->getPos().z };
    this->_laser->getComponent<sTransformComponent>()->setPos(playerPos);
    this->_laser->getComponent<sTransformComponent>()->setScale(glm::vec3{ 0.2f, this->_attributes["MaxRange"]->getFinalValue() / SIZE_UNIT, 0.2f });
    this->_laser->getComponent<sTransformComponent>()->setRotation(glm::vec3(90.0f, playerTransform->getRotation().y, 0.0f));

    if (!this->_pierceEnemy)
        this->fireOneEnemy(player, playerTransform, playerRender, playerDirection);
    else
        this->fireMultipleEnemy(player, playerTransform, playerRender, playerDirection);

#if (ENABLE_SOUND)
    if (_shootSound->soundID != -1)
    {
        SoundManager::getInstance()->playSound(_shootSound->soundID);
    }
#endif
}

void    LaserWeapon::sortHitedEntities(sTransformComponent* playerTransform, std::vector<Entity *>& entities)
{
    float distance = std::numeric_limits<float>::min();

    for (auto& entity : entities)
    {
        if (entity->getTag() != "Enemy")
            distance = glm::distance(entity->getComponent<sTransformComponent>()->getPos(), playerTransform->getPos());
    }


    for (int i = entities.size() - 1; i >= 0; i--) 
    {
        auto distanceBetween = glm::distance(entities.at(i)->getComponent<sTransformComponent>()->getPos(), playerTransform->getPos());
        if (distanceBetween > distance) 
            entities.erase(entities.begin() + i);
    }
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
    glm::vec3 playerPos = glm::vec3{ playerTransform->getPos().x, playerTransform->getPos().y + (playerRender->getModel()->getSize().y / 2.0f * playerTransform->getScale().y), playerTransform->getPos().z };
    Ray raycastHit = Ray(playerPos, glm::vec3{ playerDirection.x, 0.0f, playerDirection.z });

    std::vector<Entity*> hitedEntities;
    if (Physics::raycastAll(raycastHit, hitedEntities, std::vector<Entity*>{ player->getEntity(), this->_laser }, this->_attributes["MaxRange"]->getFinalValue()))
    {
        if (hitedEntities.size() == 0)
        {
            this->clean();
            this->_oldEntitiesHited.clear();
            return;
        }

        this->sortHitedEntities(playerTransform, hitedEntities);
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

                if (enemyScript == nullptr)
                    return;

                if (this->_timeExplosion)
                {
                    if (enemyScript->getPercentExplosion() == nullptr)
                        enemyScript->setPercentExplosion(new Attribute(1.0 / 100.0));
                    else
                        enemyScript->getPercentExplosion()->addModifier(Modifier(this->_timeExplosionPercent));
                }

                enemyScript->takeDamage(this->_attributes["Damage"]->getFinalValue());

                this->_oldEntitiesHited.push_back(hitedEntity);
            }
        }
    }
}

void LaserWeapon::fireOneEnemy(Player* player, sTransformComponent* playerTransform, sRenderComponent* playerRender, glm::vec3 playerDirection)
{
    glm::vec3 playerPos = glm::vec3{ playerTransform->getPos().x, playerTransform->getPos().y + (playerRender->getModel()->getSize().y / 2.0f * playerTransform->getScale().y), playerTransform->getPos().z };
    Ray raycastHit = Ray(playerPos, glm::vec3{ playerDirection.x, 0.0f, playerDirection.z });

    Entity* hitedEntity;
    if (Physics::raycast(raycastHit, &hitedEntity, std::vector<Entity*>{ player->getEntity(), this->_laser }, this->_attributes["MaxRange"]->getFinalValue()))
    {
        if (hitedEntity != nullptr)
        {
            if (this->_laser == nullptr)
                this->_laser = EntityFactory::createEntity("LASER_CYLINDER");

            this->_laser->getComponent<sTransformComponent>()->setPos(playerPos);
            this->_laser->getComponent<sTransformComponent>()->setScale(glm::vec3{ 0.2f, glm::distance(hitedEntity->getComponent<sTransformComponent>()->getPos(), playerTransform->getPos()) / SIZE_UNIT, 0.2f });
            this->_laser->getComponent<sTransformComponent>()->setRotation(glm::vec3(90.0f, playerTransform->getRotation().y, 0.0f));

            if (hitedEntity->getTag() == "Enemy")
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
                this->clean();
                this->_oldEntitiesHited.clear();
            }
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
        this->_timeExplosion = true;
        break;
    default:
        break;
    }
}

void    LaserWeapon::reload()
{
    this->_attributes["Ammo"]->clearAllBonuses();
}

void    LaserWeapon::clean()
{
    if (!this->_laser)
        return;

    auto em = EntityFactory::getBindedEntityManager();

    em->destroyEntityRegister(this->_laser->handle);
    this->_laser = nullptr;
}
