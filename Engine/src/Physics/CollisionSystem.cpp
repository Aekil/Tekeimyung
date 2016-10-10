#include "Physics/CollisionSystem.hpp"
#include "Core/Components.hh"

CollisionSystem::CollisionSystem(Map* map): _map(map)
{
    this->addDependency<sHitBoxComponent>();
    this->addDependency<sPositionComponent>();
    this->addDependency<sDirectionComponent>();
}

void    CollisionSystem::update(EntityManager &em, float elapsedTime)
{
    this->forEachEntity(em, [&](Entity* entity) {
        this->moveHitBox(entity);
        sDirectionComponent* direction = entity->getComponent<sDirectionComponent>();
        sPositionComponent* position = entity->getComponent<sPositionComponent>();

        this->forEachEntity(em, [&](Entity* entityB) {
            sPositionComponent* positionB = entityB->getComponent<sPositionComponent>();
            if (entity->id != entityB->id && position->z == positionB->z && this->isColliding(entity, entityB)) {
                //TODO: Resolution of collisions
                position->value += -direction->value * elapsedTime;
            }
        });
    });
}

void    CollisionSystem::moveHitBox(Entity *entity)
{
    sDirectionComponent* direction = entity->getComponent<sDirectionComponent>();


    if (entity->getComponent<sHitBoxComponent>() != nullptr)
    {
        sHitBoxComponent* hitBox = entity->getComponent<sHitBoxComponent>();

        hitBox->min += direction->value;
        hitBox->max += direction->value;
    }
    else if (entity->getComponent<sCircleHitBoxComponent>())
    {
        sCircleHitBoxComponent* circleHitBox = entity->getComponent<sCircleHitBoxComponent>();

        circleHitBox->center += direction->value;
    }
}

bool    CollisionSystem::isColliding(Entity *firstEntity, Entity *secondEntity)
{
    if (nullptr != firstEntity->getComponent<sHitBoxComponent>() && nullptr != secondEntity->getComponent<sHitBoxComponent>())
    {
        sHitBoxComponent* hitBoxFirst = firstEntity->getComponent<sHitBoxComponent>();
        sHitBoxComponent* hitBoxSecond = secondEntity->getComponent<sHitBoxComponent>();

        if (hitBoxFirst->max.x < hitBoxSecond->min.x || hitBoxFirst->min.x > hitBoxSecond->max.x) return false;
        if (hitBoxFirst->max.y < hitBoxSecond->min.y || hitBoxFirst->min.y > hitBoxSecond->max.y) return false;

        return true;
    }
    else if (nullptr != firstEntity->getComponent<sCircleHitBoxComponent>() && nullptr != secondEntity->getComponent<sCircleHitBoxComponent>())
    {
        sCircleHitBoxComponent* circleHitBoxFirst = firstEntity->getComponent<sCircleHitBoxComponent>();
        sCircleHitBoxComponent* circleHitBoxSecond = secondEntity->getComponent<sCircleHitBoxComponent>();

        float r = circleHitBoxFirst->radius + circleHitBoxSecond->radius;
        r *= r;
        return r < pow((circleHitBoxFirst->center.x + circleHitBoxSecond->center.x), 2) + pow((circleHitBoxFirst->center.y + circleHitBoxFirst->center.y), 2);
    }
    
    return false;
}
