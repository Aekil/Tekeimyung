#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

#include <Engine/Utils/Timer.hpp>

#include <Game/Components.hh>
#include <Game/EntityFactory.hpp>

#include <Game/Systems/TowerAISystem.hpp>

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
        std::list<uint32_t>&    entities = (*_map)[(unsigned int) position->z].getEntities();
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
            else
            {
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
        if (towerAIComponent->lastShotTime >= towerAIComponent->fireRate)
            towerAIComponent->lastShotTime = 0.0f;
    });
}

/** @brief  This function checks for the nearest entity in range of an entity that gets the TowerAIComponent component.
 *
 *  @param  tower       The entity that will be used to check for the nearest entity around it
 *
 *  @param  entities    A list of entities' id
 *  @note   The list of entities' id comes from the same layer of the tower, despite the algorithm is developed for a 3D space.
 *
 *  @param  em          the EntityManager used to retrieve pointers from entities
 *
 *  @return The nearest entity id
 */
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

                glm::vec3   nearestEntityVector = glm::vec3(entityPosition->value.x, entityPosition->value.y, entityPosition->z);
                glm::vec3   towerVector = glm::vec3(towerPosition->value.x, towerPosition->value.y, towerPosition->z);
                distance = glm::distance(towerVector, nearestEntityVector);
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

    result = (
        std::pow(targetPosition->value.x - towerPosition->value.x, 2) +
        std::pow(targetPosition->value.y - towerPosition->value.y, 2) +
        std::pow(targetPosition->z - towerPosition->z, 2)
        ) <= std::pow(component->radius, 2);
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
    sTowerAIComponent*      towerAIComponent;
    sPositionComponent*     enemyPosition;

    Entity*                 fireball;
    sPositionComponent*     fireballPosition;
    sDirectionComponent*    fireballDirection;
    sRectHitboxComponent*   fireballHitbox;

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
            fireball->addComponent<sDirectionComponent>();
        fireballDirection = fireball->getComponent<sDirectionComponent>();
        fireballDirection->value = glm::normalize(enemyPosition->value - fireballPosition->value);
        fireballDirection->speed = towerAIComponent->projectileSpeed;

        fireball->addComponent<sRectHitboxComponent>();
        fireballHitbox = fireball->getComponent<sRectHitboxComponent>();
        fireballHitbox->min = glm::vec2(fireballPosition->value - 14.0f);
        fireballHitbox->max = glm::vec2(fireballPosition->value + 14.0f);
    }
}
