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
        sProjectileComponent*   component = entity->getComponent<sProjectileComponent>();

        if (component->guided == true)
        {
            Entity* target = em.getEntity(component->targetId);

            if (target != nullptr)
            {
                sPositionComponent* projectilePosition = entity->getComponent<sPositionComponent>();
                sPositionComponent* targetPosition = target->getComponent<sPositionComponent>();
                sDirectionComponent*    projectileDirection = entity->getComponent<sDirectionComponent>();

                projectileDirection->value = glm::normalize(targetPosition->value - projectilePosition->value) * projectileDirection->speed;
            }
        }
    });
}
