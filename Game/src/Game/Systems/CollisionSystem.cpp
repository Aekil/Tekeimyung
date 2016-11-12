#include <cmath>

#include <Engine/Window/GameWindow.hpp>
#include <Engine/Physics/Collisions.hpp>

#include <Game/Components.hh>

#include <Game/Systems/CollisionSystem.hpp>

CollisionSystem::CollisionSystem(Map* map): _map(map)
{
    this->addDependency<sRectHitboxComponent>();
    this->addDependency<sPositionComponent>();
    this->addDependency<sDirectionComponent>();
}

void    CollisionSystem::update(EntityManager &em, float elapsedTime)
{
    CollisionMap* collisionMap = _map->getCollisionMap();

    this->forEachEntity(em, [&](Entity* entity)
    {
        this->moveHitbox(entity);
        sDirectionComponent* direction = entity->getComponent<sDirectionComponent>();
        sPositionComponent* position = entity->getComponent<sPositionComponent>();
        uint16_t layer = (uint16_t)position->z;

        // Check Collision with tile
        if ((*_map)[layer][(uint32_t)std::floor(position->value.y)][(uint32_t)std::floor(position->value.x)].get() != 0 ||
            (layer > 0 && (*collisionMap)[layer - 1][(uint32_t)std::floor(position->value.y)][(uint32_t)std::floor(position->value.x)] == eColType::CAN_NOT_WALK))
        {
            position->value += -direction->value * elapsedTime;
        }
/*        else
        {
            // Check Collision with dynamic entities
            for (auto &&entityId: (*_map)[layer].getEntities())
            {
                Entity* entityB = em.getEntity(entityId);

                if (!entityB || !entityB->getComponent<sRectHitboxComponent>())
                    continue;

                sPositionComponent* positionB = entityB->getComponent<sPositionComponent>();
                if (entity->id != entityB->id && position->z == positionB->z && this->isColliding(entity, entityB))
                {
                    //TODO: Resolution of collisions
                    position->value += -direction->value * elapsedTime;
                }
            }

        }*/


    });
}

void    CollisionSystem::moveHitbox(Entity *entity)
{
    sPositionComponent* position = entity->getComponent<sPositionComponent>();
    float offsetX = GameWindow::getInstance()->getScreenWidth() / 2.0f - 66.0f;
    float offsetY = GameWindow::getInstance()->getScreenHeight() - (33.0f * 3.0f);

    if (entity->getComponent<sRectHitboxComponent>() != nullptr)
    {
        sRectHitboxComponent* hitbox = entity->getComponent<sRectHitboxComponent>();

        hitbox->min.x = offsetX + (position->value.x - position->value.y) * 66.0f;
        hitbox->min.y = offsetY - (position->value.x + position->value.y) * 33.0f + (32.0f * position->z);

        hitbox->max.x = (offsetX + (position->value.x - position->value.y) * 66.0f )+ 55.0f;
        hitbox->max.y = (offsetY - (position->value.x + position->value.y) * 33.0f + (32.0f * position->z)) + 64.9f;
    }
    else if (entity->getComponent<sCircleHitboxComponent>() != nullptr)
    {
        sCircleHitboxComponent* circleHitbox = entity->getComponent<sCircleHitboxComponent>();

        circleHitbox->center.x = offsetX + (position->value.x - position->value.y) * 66.0f;
        circleHitbox->center.y = offsetY - (position->value.x + position->value.y) * 33.0f + (32.0f * position->z);
    }
}

bool    CollisionSystem::isColliding(Entity *firstEntity, Entity *secondEntity)
{
    if (firstEntity->getComponent<sRectHitboxComponent>() != nullptr && secondEntity->getComponent<sRectHitboxComponent>() != nullptr)
    {
        sRectHitboxComponent* hitboxFirst = firstEntity->getComponent<sRectHitboxComponent>();
        sRectHitboxComponent* hitboxSecond = secondEntity->getComponent<sRectHitboxComponent>();

        return (Collisions::rectHitboxCheck(&hitboxFirst->min, &hitboxFirst->max,
                                            &hitboxSecond->min, &hitboxSecond->max));
    }
    else if (firstEntity->getComponent<sCircleHitboxComponent>() != nullptr && secondEntity->getComponent<sCircleHitboxComponent>() != nullptr)
    {
        sCircleHitboxComponent* circleHitboxFirst = firstEntity->getComponent<sCircleHitboxComponent>();
        sCircleHitboxComponent* circleHitboxSecond = secondEntity->getComponent<sCircleHitboxComponent>();

        return (Collisions::circleHitboxCheck(&circleHitboxFirst->center, circleHitboxFirst->radius,
                                              &circleHitboxSecond->center, circleHitboxSecond->radius));
    }
    return (false);
}
