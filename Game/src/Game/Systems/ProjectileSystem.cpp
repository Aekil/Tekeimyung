#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

#include <Game/Components.hh>
#include <Game/EntityFactory.hpp>

#include <Game/Systems/ProjectileSystem.hpp>


ProjectileSystem::ProjectileSystem()
{
    addDependency<sProjectileComponent>();
    addDependency<sPositionComponent>();
    addDependency<sDirectionComponent>();
}

ProjectileSystem::~ProjectileSystem() {}

void    ProjectileSystem::update(EntityManager &em, float elapsedTime)
{
    forEachEntity(em, [&](Entity *entity)
    {
        sProjectileComponent* component = entity->getComponent<sProjectileComponent>();
        Entity* shooter = em.getEntity(component->shooterId);
        if (shooter)
        {
            sPositionComponent* towerPos = shooter->getComponent<sPositionComponent>();
            sPositionComponent* projectilePosition = entity->getComponent<sPositionComponent>();

            float dist = glm::length(towerPos->value - projectilePosition->value);
            if (dist > component->rangeMax)
            {
                em.destroyEntityRegister(entity);
            }
            else if (component->guided == true)
            {
                Entity* target = em.getEntity(component->targetId);

                if (target != nullptr)
                {
                    sPositionComponent* targetPosition = target->getComponent<sPositionComponent>();
                    sDirectionComponent*    projectileDirection = entity->getComponent<sDirectionComponent>();

                    projectileDirection->value = glm::normalize(targetPosition->value - projectilePosition->value) * projectileDirection->speed;
                }
            }
        }
    });
}
