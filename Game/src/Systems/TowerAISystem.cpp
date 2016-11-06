#include <glm/glm.hpp>

#include <Systems/TowerAISystem.hpp>
#include <Core/Components.hh>
#include <EntityFactory.hpp>

#include <Utils/Timer.hpp>

TowerAISystem::TowerAISystem(Map* map) : _map(map)
{
    addDependency<sTowerAIComponent>();
    addDependency<sPositionComponent>();
}

TowerAISystem::~TowerAISystem() {}

void    TowerAISystem::update(EntityManager &em, float elapsedTime)
{
    forEachEntity(em, [&](Entity *entity)
    {
        sTowerAIComponent*      towerAIComponent = entity->getComponent<sTowerAIComponent>();
        sPositionComponent*     position = entity->getComponent<sPositionComponent>();
        std::list<uint32_t>&    entities = (*_map)[position->z].getEntities();
        uint32_t                targetId = 0;

        towerAIComponent->lastShotTime += elapsedTime;
        if (towerAIComponent->targetId != 0)
        {
            Entity*             target = em.getEntity(towerAIComponent->targetId);

            if (target != nullptr)
            {
                if (isEntityInRange(entity, target) == true)
                    fire(entity, target);
                else
                    towerAIComponent->targetId = 0;
            }
        }
        if (towerAIComponent->targetId == 0)
        {
            targetId = getNearestEntityInRange(entity, entities, em);
            if (targetId != 0)
            {
                Entity*             target = em.getEntity(targetId);

                if (target != nullptr)
                {
                    towerAIComponent->targetId = targetId;
                    fire(entity, target);
                }
            }
        }
    });
}

uint32_t    TowerAISystem::getNearestEntityInRange(Entity* tower, std::list<uint32_t>& entities, EntityManager &em)
{
    Entity*             nearestEntity;
    uint32_t            nearestEntityId = 0;
    sPositionComponent* towerPosition;
    sPositionComponent* entityPosition;
    float               distance = 0.0f, smallestDistance;

    towerPosition = tower->getComponent<sPositionComponent>();
    smallestDistance = std::numeric_limits<float>::max();
    for (auto &&entityId : entities)
    {
        nearestEntity = em.getEntity(entityId);
        if (nearestEntity != nullptr)
        {
            sAIComponent*   aiComponent;

            aiComponent = nearestEntity->getComponent<sAIComponent>();
            if (aiComponent != nullptr)
            {
                entityPosition = nearestEntity->getComponent<sPositionComponent>();
                distance = glm::distance(towerPosition->value, entityPosition->value);
                if (isEntityInRange(tower, nearestEntity) && distance <= smallestDistance)
                {
                    smallestDistance = distance;
                    nearestEntityId = entityId;
                }
            }
        }
    }
    return (nearestEntityId);
}

bool    TowerAISystem::isEntityInRange(Entity* tower, Entity* potentialTarget)
{
    sTowerAIComponent*  component = tower->getComponent<sTowerAIComponent>();
    sPositionComponent* towerPosition = tower->getComponent<sPositionComponent>();
    sPositionComponent* targetPosition = potentialTarget->getComponent<sPositionComponent>();
    bool                result = false;

    result = (std::pow(targetPosition->value.x - towerPosition->value.x, 2) +
        std::pow(targetPosition->value.y - towerPosition->value.y, 2)) <= std::pow(component->radius, 2);
    return (result);
}

Entity* TowerAISystem::createFireball(Entity* shooter, Entity* target)
{
    Entity*                 fireball;
    sPositionComponent*     shooterPosition;
    sPositionComponent*     fireballPosition;
    sProjectileComponent*   projectileComponent;

    fireball = EntityFactory::createEntity(eArchetype::FIRE_BALL);
    shooterPosition = shooter->getComponent<sPositionComponent>();
    fireballPosition = fireball->getComponent<sPositionComponent>();
    fireballPosition->value.x = shooterPosition->value.x;
    fireballPosition->value.y = shooterPosition->value.y;
    fireballPosition->z = shooterPosition->z;

    projectileComponent = fireball->getComponent<sProjectileComponent>();
    projectileComponent->shooterId = shooter->id;
    projectileComponent->guided = true;
    projectileComponent->targetId = target->id;

    return (fireball);
}

void    TowerAISystem::fire(Entity* shooter, Entity* enemy)
{
    Entity*                 fireball;
    sPositionComponent*     fireballPosition;
    sPositionComponent*     enemyPosition;
    sDirectionComponent*    fireballDirection;
    sTowerAIComponent*      towerAIComponent;

    towerAIComponent = shooter->getComponent<sTowerAIComponent>();
    if (towerAIComponent->lastShotTime >= towerAIComponent->fireRate)
    {
        fireball = createFireball(shooter, enemy);
        fireballPosition = fireball->getComponent<sPositionComponent>();
        enemyPosition = enemy->getComponent<sPositionComponent>();

        if (fireballPosition == nullptr || enemyPosition == nullptr)
            EXCEPT(InternalErrorException, "The sPositionComponent is missing.");

        fireballDirection = fireball->getComponent<sDirectionComponent>();
        if (fireballDirection == nullptr)
        {
            fireball->addComponent<sDirectionComponent>();
            fireballDirection = fireball->getComponent<sDirectionComponent>();
        }
        fireballDirection->value = glm::normalize(enemyPosition->value - fireballPosition->value);
        fireballDirection->speed = towerAIComponent->projectileSpeed;
        towerAIComponent->lastShotTime = 0.0f;
    }
}