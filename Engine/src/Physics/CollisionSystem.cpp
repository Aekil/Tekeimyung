#include "Physics/CollisionSystem.hpp"
#include "Core/Components.hh"
#include "Window/GameWindow.hpp"

CollisionSystem::CollisionSystem(Map* map): _map(map)
{
    this->addDependency<sHitBoxComponent>();
    this->addDependency<sPositionComponent>();
    this->addDependency<sDirectionComponent>();
}

void    CollisionSystem::update(EntityManager &em, float elapsedTime)
{
    CollisionMap* collisionMap = _map->getCollisionMap();

    this->forEachEntity(em, [&](Entity* entity)
    {
        this->moveHitBox(entity);
        sDirectionComponent* direction = entity->getComponent<sDirectionComponent>();
        sPositionComponent* position = entity->getComponent<sPositionComponent>();
        uint16_t layer = position->z;

        // Check Collision with tile
        if ((*_map)[layer][std::floor(position->value.y)][std::floor(position->value.x)].get() != 0 ||
            (layer > 0 && (*collisionMap)[layer - 1][std::floor(position->value.y)][std::floor(position->value.x)] == eColType::CAN_NOT_WALK))
        {
            position->value += -direction->value * elapsedTime;
        }
        else
        {
            // Check Collision with dynamic entities
            for (auto &&entityId: (*_map)[layer].getEntities())
            {
                Entity* entityB = em.getEntity(entityId);

                if (!entityB || !entityB->getComponent<sHitBoxComponent>())
                    continue;

                sPositionComponent* positionB = entityB->getComponent<sPositionComponent>();
                if (entity->id != entityB->id && position->z == positionB->z && this->isColliding(entity, entityB))
                {
                    //TODO: Resolution of collisions
                    position->value += -direction->value * elapsedTime;
                }
            }

        }


    });
}

void    CollisionSystem::moveHitBox(Entity *entity)
{
    sPositionComponent* position = entity->getComponent<sPositionComponent>();
    float offsetX = GameWindow::getInstance()->getWidth() / 2.0f - 66.0f;
    float offsetY = GameWindow::getInstance()->getHeight() - (33.0f * 3.0f);

    if (entity->getComponent<sHitBoxComponent>() != nullptr)
    {
        sHitBoxComponent* hitBox = entity->getComponent<sHitBoxComponent>();

        hitBox->min.x = offsetX + (position->value.x - position->value.y) * 66.0f;
        hitBox->min.y = offsetY - (position->value.x + position->value.y) * 33.0f + (32.0f * position->z);

        hitBox->max.x = (offsetX + (position->value.x - position->value.y) * 66.0f )+ 55.0f;
        hitBox->max.y = (offsetY - (position->value.x + position->value.y) * 33.0f + (32.0f * position->z)) + 64.9f;
    }
    else if (entity->getComponent<sCircleHitBoxComponent>() != nullptr)
    {
        sCircleHitBoxComponent* circleHitBox = entity->getComponent<sCircleHitBoxComponent>();

        circleHitBox->center.x = offsetX + (position->value.x - position->value.y) * 66.0f;
        circleHitBox->center.y = offsetY - (position->value.x + position->value.y) * 33.0f + (32.0f * position->z);
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
