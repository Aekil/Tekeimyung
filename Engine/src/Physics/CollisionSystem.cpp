#include "Physics/CollisionSystem.hh"

CollisionSystem::CollisionSystem()
{
    this->addDependency<sHitBoxComponent>();
    this->addDependency<sPositionComponent>();
    this->addDependency<sDirectionComponent>();
}

void    CollisionSystem::update(EntityManager &em, float elapsedTime)
{
    this->forEachEntity(em, [&](Entity* entity) {
        sDirectionComponent *direction = entity->getComponent<sDirectionComponent>();
        sPositionComponent *position = entity->getComponent<sPositionComponent>();

        this->forEachEntity(em, [&](Entity* entityB) {
            if (entity->id != entityB->id && this->isColliding(entity, entityB)) {
                position->x += -direction->x * elapsedTime;
                position->y += -direction->y * elapsedTime;
            }
        });
    });
}

bool    CollisionSystem::isColliding(Entity *firstEntity, Entity *secondEntity)
{
    sHitBoxComponent* hitBoxFirst = firstEntity->getComponent<sHitBoxComponent>();
    sPositionComponent* positionFirst = firstEntity->getComponent<sPositionComponent>();
    sHitBoxComponent* hitBoxSecond = secondEntity->getComponent<sHitBoxComponent>();
    sPositionComponent* positionSecond = secondEntity->getComponent<sPositionComponent>();

    hitBoxFirst->x += positionFirst->x;
    hitBoxFirst->y += positionFirst->y;
    hitBoxSecond->x += positionSecond->x;
    hitBoxSecond->y += positionSecond->y;

    return (hitBoxFirst->x < hitBoxSecond->x + hitBoxSecond->width &&
        hitBoxFirst->x + hitBoxFirst->width > hitBoxSecond->x &&
        hitBoxFirst->y < hitBoxSecond->y + hitBoxSecond->height &&
        hitBoxFirst->y + hitBoxFirst->height > hitBoxSecond->y);
}