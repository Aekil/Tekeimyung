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
        sDirectionComponent* direction = entity->getComponent<sDirectionComponent>();
        sPositionComponent* position = entity->getComponent<sPositionComponent>();

        this->forEachEntity(em, [&](Entity* entityB) {
            sPositionComponent* positionB = entityB->getComponent<sPositionComponent>();
            if (entity->id != entityB->id && position->z == positionB->z && this->isColliding(entity, entityB)) {
                //TODO: Resolution of collisions
                position->x += -direction->x * elapsedTime;
                position->y += -direction->y * elapsedTime;
            }
        });
    });
}

bool    CollisionSystem::isColliding(Entity *firstEntity, Entity *secondEntity)
{
    sHitBoxComponent* hitBoxFirst = firstEntity->getComponent<sHitBoxComponent>();
    sHitBoxComponent* hitBoxSecond = secondEntity->getComponent<sHitBoxComponent>();

    if (hitBoxFirst->max.x < hitBoxSecond->min.x || hitBoxFirst->min.x > hitBoxSecond->max.x) return false;
    if (hitBoxFirst->max.y < hitBoxSecond->min.y || hitBoxFirst->min.y > hitBoxSecond->max.y) return false;

    return true;
}
