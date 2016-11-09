#include <cmath>

#include "Core/Components.hh"
#include "Window/GameWindow.hpp"
#include "Physics/Collisions.hpp"

#include "Systems/CollisionSystem.hpp"

CollisionSystem::CollisionSystem(Map* map) : _map(map)
{
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
        else
        {
            // Check Collision with dynamic entities
            for (auto &&entityId : (*_map)[layer].getEntities())
            {
                Entity* entityB = em.getEntity(entityId);
                if (!entityB) continue;

                sPositionComponent* positionB = entityB->getComponent<sPositionComponent>();
                if (entity->id != entityB->id && this->isColliding(entity, entityB))
                {
                    //TODO: Resolution of collisions
                    position->value += -direction->value * elapsedTime;
                }
            }

        }
    });
}

void    CollisionSystem::moveHitbox(Entity *entity)
{
    sPositionComponent* position = entity->getComponent<sPositionComponent>();

    if (entity->getComponent<sRenderComponent>()->_sprite != nullptr) {
        sRenderComponent* renderComponent = entity->getComponent<sRenderComponent>();

        if (entity->getComponent<sRectHitboxComponent>() != nullptr)
        {
            sRectHitboxComponent* hitbox = entity->getComponent<sRectHitboxComponent>();

            hitbox->min.x = renderComponent->_sprite->getPos().x;
            hitbox->min.y = renderComponent->_sprite->getPos().y;

            hitbox->max.x = renderComponent->_sprite->getPos().x + renderComponent->spriteSize.x;
            hitbox->max.y = renderComponent->_sprite->getPos().y + renderComponent->spriteSize.y;
        }
        else if (entity->getComponent<sCircleHitboxComponent>() != nullptr)
        {
            sCircleHitboxComponent* circleHitbox = entity->getComponent<sCircleHitboxComponent>();

            circleHitbox->center.x = renderComponent->_sprite->getPos().x + (renderComponent->spriteSize.x / 2);
            circleHitbox->center.y = renderComponent->_sprite->getPos().y + (renderComponent->spriteSize.y / 2);
        }
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
