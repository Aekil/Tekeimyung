/*
** Author : Simon AMBROISE
*/

#include <vector>

#include <Engine/EntityFactory.hpp>
#include <Engine/Physics/Collisions.hpp>
#include <Engine/Physics/Physics.hpp>

#include <Game/Scripts/TeslaOrb.hpp>
#include <Game/Weapons/TeslaWeapon.hpp>
#include <Game/Scripts/Enemy.hpp>

const std::string TeslaWeapon::Name = "Tesla";

TeslaWeapon::TeslaWeapon()
{
    this->_attributes["FireRate"] = new Attribute(0.42f);
    this->_attributes["Ammo"] = new Attribute(100.0f);
    this->_attributes["Damage"] = new Attribute(12.0f);
    this->_attributes["MaxRange"] = new Attribute(35.0f);
    this->_attributes["HitAmount"] = new Attribute(2);
    this->_attributes["StunChance"] = new Attribute(0.0f);
}

void    TeslaWeapon::fire(Player* player, sTransformComponent* playerTransform, sRenderComponent* playerRender, glm::vec3 playerDirection)
{
    switch (this->_level)
    {
        case 0:
        case 1:
        case 2:
            this->fireLightning(player, playerTransform, playerRender, playerDirection);
            break;
        case 3:
            this->fireOrb(player, playerTransform, playerRender, playerDirection);
            break;
    }
}

void    TeslaWeapon::reload()
{

}

void    TeslaWeapon::upgradeByLevel()
{
    switch (this->_level)
    {
        case 1:
            this->_attributes["Damage"]->addModifier(Modifier(100.0 / 100.0));
            this->_attributes["HitAmount"]->addModifier(Modifier(100.0 / 100.0));
            break;
        case 2:
            this->_attributes["StunChance"]->addModifier(Modifier(35.0 / 100.0));
            break;
        case 3:
            break;
        default:
            break;
    }
}

void    TeslaWeapon::fireLightning(Player* player, sTransformComponent* playerTransform, sRenderComponent* playerRender, glm::vec3 playerDirection)
{
    glm::vec3               playerPos;
    Ray                     raycastHit;
    std::vector<Entity*>    hitEntities;
    Entity*                 hitEntity;

    playerPos = playerTransform->getPos() + (playerRender->getModel()->getSize().y / 2.0f * playerTransform->getScale().y);
    raycastHit = Ray(playerPos, glm::vec3{ playerDirection.x, 0.0f, playerDirection.z });

    if (Physics::raycast(raycastHit, &hitEntity, std::vector<Entity*> { player->getEntity() }, false) == true &&
        hitEntity->getTag() == "Enemy")
    {
        this->spreadLightning(hitEntity, this->_attributes["HitAmount"]->getFinalValue());
    }
}

void    TeslaWeapon::spreadLightning(Entity* hitEntity, unsigned int hitLeft, std::vector<Entity*> entitiesAlreadyHit)
{
    if (hitLeft > 0)
    {
        auto    em = EntityFactory::getBindedEntityManager();
        auto&   enemies = em->getEntitiesByTag("Enemy");

        float   nearestDistance = std::numeric_limits<float>::max();
        Entity* nearestEntity = nullptr;
        Enemy*  nearestEnemy = nullptr;

        for (Entity* entity : enemies)
        {
            //  If the entity has not been hit by the lightning yet.
            if (std::find(entitiesAlreadyHit.begin(), entitiesAlreadyHit.end(), entity) == entitiesAlreadyHit.end())
            {
                sScriptComponent*   scriptComponent = entity->getComponent<sScriptComponent>();

                if (scriptComponent != nullptr)
                {
                    Enemy*  enemyScript = scriptComponent->getScript<Enemy>("Enemy");

                    if (enemyScript != nullptr)
                    {
                        sBoxColliderComponent*  box = entity->getComponent<sBoxColliderComponent>();

                        if (box != nullptr)
                        {
                            sTransformComponent*    hitEntityPos = hitEntity->getComponent<sTransformComponent>();
                            sTransformComponent*    entityPos = entity->getComponent<sTransformComponent>();

                            if (hitEntityPos != nullptr && entityPos != nullptr)
                            {
                                float   distance = Physics::distance(hitEntityPos->getPos(), entityPos->getPos());

                                if (nearestEntity == nullptr || distance < nearestDistance)
                                {
                                    nearestDistance = distance;
                                    nearestEntity = entity;
                                    nearestEnemy = enemyScript;
                                }
                            }
                        }
                    }
                }
            }
        }

        if (nearestEntity != nullptr)
        {
            entitiesAlreadyHit.push_back(nearestEntity);
            nearestEnemy->takeDamage(this->_attributes["Damage"]->getFinalValue());
            this->triggerLightningEffect(nearestEntity);
            this->spreadLightning(nearestEntity, hitLeft - 1, entitiesAlreadyHit);
        }
    }
}

void    TeslaWeapon::triggerLightningEffect(Entity* entity)
{
    sScriptComponent*   scriptComponent = entity->getComponent<sScriptComponent>();

    if (scriptComponent != nullptr)
    {
        Enemy*              enemyScript = scriptComponent->getScript<Enemy>("Enemy");

        if (enemyScript != nullptr)
        {
            sRenderComponent*   render = entity->getComponent<sRenderComponent>();

            if (render != nullptr)
                render->_animator.play("lightning_effect", false);
        }
    }
}

void            TeslaWeapon::fireOrb(Player* player, sTransformComponent* playerTransform, sRenderComponent* playerRender, glm::vec3 playerDirection)
{
    Entity*     teslaOrb = EntityFactory::createEntity("TESLA_ORB");

    if (teslaOrb == nullptr)
    {
        LOG_WARN("Could not create Entity with archetype \"%s\"", "TESLA_ORB");
        return;
    }

    sScriptComponent*   scriptComponent = teslaOrb->getComponent<sScriptComponent>();

    if (scriptComponent == nullptr)
    {
        LOG_WARN("Could not retrieve %s from Entity with archetype \"%s\"", "sScriptComponent", "TESLA_ORB");
        return;
    }

    TeslaOrb*   orbScript = scriptComponent->getScript<TeslaOrb>("TeslaOrb");

    if (scriptComponent == nullptr)
    {
        LOG_WARN("Could not retrieve script %s from Entity with archetype \"%s\"", "TeslaOrb", "TESLA_ORB");
        return;
    }

    //  Let's set the orb's direction according to the player's.
    orbScript->setPosition(playerTransform->getPos());
    orbScript->setDirection({ playerDirection.x, 0.0f, playerDirection.z });
}