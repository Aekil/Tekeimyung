#include <cmath>
#include <algorithm>

#include <Engine/Window/GameWindow.hpp>
#include <Engine/Physics/Collisions.hpp>

#include <Game/Components.hh>
#include <Game/EntityFactory.hpp>

#include <Game/Systems/CollisionSystem.hpp>

CollisionSystem::CollisionSystem(Map* map) : _map(map)
{
    this->addDependency<sPositionComponent>();
    this->addDependency<sDirectionComponent>();

    _keyMonitoring = MonitoringDebugWindow::getInstance()->registerSystem(COLLISION_SYSTEM_NAME);
}

void    CollisionSystem::update(EntityManager &em, float elapsedTime)
{
    Timer   timer;
    CollisionMap* collisionMap = _map->getCollisionMap();

    this->forEachEntity(em, [&](Entity* entity)
    {
        if (entity->getComponent<sBoxColliderComponent>())
        {
            this->moveHitbox(entity, elapsedTime);
            sDirectionComponent* direction = entity->getComponent<sDirectionComponent>();
            sPositionComponent* position = entity->getComponent<sPositionComponent>();
            uint16_t layer = (uint16_t)position->z;

            // Check Collision with tile
            // CRASH ON SPAWN RANDOM ENTITIES
            if ((*_map)[layer][(uint32_t)std::floor(position->value.y)][(uint32_t)std::floor(position->value.x)].get() != 0 ||
                (layer > 0 && (*collisionMap)[layer - 1][(uint32_t)std::floor(position->value.y)][(uint32_t)std::floor(position->value.x)] == eColType::CAN_NOT_WALK))
            {
                if (this->staticResolution(entity))
                {
                    position->value -= direction->value * elapsedTime;
                }
            }
            else
            {

                // Check Collision with dynamic entities
                for (auto &&entityId : (*_map)[layer].getEntities())
                {
                    Entity* entityB = em.getEntity(entityId);
                    if (!entityB) continue;

                    sPositionComponent* positionB = entityB->getComponent<sPositionComponent>();
                    if (entity->id != entityB->id)
                    {
                        //position->value -= direction->value;
                        if (this->isColliding(entity, entityB))
                        {
                            if (this->dynamicResolution(em, entity, entityB))
                            {
                                position->value -= direction->value * elapsedTime;
                            }
                        }

                    }
                }

            }
         /*   if (entity->getComponent<sRenderComponent>()->_sprite != nullptr) // undo movement simulation
            {
                sRectHitboxComponent* rectHitbox = entity->getComponent<sRectHitboxComponent>();
                sRenderComponent* renderComponent = entity->getComponent<sRenderComponent>();
                glm::vec3 oldGraphPos = Map::mapToGraphPosition(position->value, position->z, renderComponent->_sprite);
                rectHitbox->min.x = oldGraphPos.x;
                rectHitbox->min.y = oldGraphPos.y;
                rectHitbox->max.x = oldGraphPos.x + renderComponent->spriteSize.x;
                rectHitbox->max.y = oldGraphPos.y + renderComponent->spriteSize.y;
                position->value -= direction->value * elapsedTime;
            }*/
        }
    });
    _data.timeSec = timer.getElapsedTime();
    MonitoringDebugWindow::getInstance()->updateSystem(_keyMonitoring, _data);
}

void    CollisionSystem::moveHitbox(Entity *entity, float elapsedTime) // simulate movement
{
/*    if (entity->getComponent<sRenderComponent>()->_sprite != nullptr)
    {
        sPositionComponent* position = entity->getComponent<sPositionComponent>();
        sDirectionComponent* direction = entity->getComponent<sDirectionComponent>();
        sRenderComponent* renderComponent = entity->getComponent<sRenderComponent>();

        glm::vec2 nextPos = position->value + direction->value * elapsedTime;
        glm::vec3 nextGraphPos = Map::mapToGraphPosition(nextPos, position->z, renderComponent->_sprite);

        if (entity->getComponent<sRectHitboxComponent>() != nullptr)
        {
            sRectHitboxComponent* rectHitbox = entity->getComponent<sRectHitboxComponent>();

            rectHitbox->min.x = nextGraphPos.x;
            rectHitbox->min.y = nextGraphPos.y;;
            rectHitbox->max.x = nextGraphPos.x + renderComponent->spriteSize.x;
            rectHitbox->max.y = nextGraphPos.y + renderComponent->spriteSize.y;

            position->value = nextPos;

            // same in movement system, common code ?
            position->value.x = std::max(0.0f, position->value.x);
            position->value.x = std::min((float)_map->getWidth() - 0.01f, position->value.x);
            position->value.y = std::max(0.0f, position->value.y);
            position->value.y = std::min((float)_map->getHeight() - 0.01f, position->value.y);
        }
        else if (entity->getComponent<sCircleHitboxComponent>() != nullptr)
        {
            sCircleHitboxComponent* circleHitbox = entity->getComponent<sCircleHitboxComponent>();

            circleHitbox->center.x = nextGraphPos.x + (renderComponent->spriteSize.x / 2);
            circleHitbox->center.y = nextGraphPos.y + (renderComponent->spriteSize.y / 2);
        }
    }*/
}

bool    CollisionSystem::isColliding(Entity *firstEntity, Entity *secondEntity)
{
/*    if (firstEntity->getComponent<sRectHitboxComponent>() != nullptr && secondEntity->getComponent<sRectHitboxComponent>() != nullptr)
    {
        sRectHitboxComponent* rectHitboxFirst = firstEntity->getComponent<sRectHitboxComponent>();
        sRectHitboxComponent* rectHitboxSecond = secondEntity->getComponent<sRectHitboxComponent>();

        return (Collisions::rectHitboxCheck(&rectHitboxFirst->min, &rectHitboxFirst->max,
            &rectHitboxSecond->min, &rectHitboxSecond->max));
    }
    else if (firstEntity->getComponent<sCircleHitboxComponent>() != nullptr && secondEntity->getComponent<sCircleHitboxComponent>() != nullptr)
    {
        sCircleHitboxComponent* circleHitboxFirst = firstEntity->getComponent<sCircleHitboxComponent>();
        sCircleHitboxComponent* circleHitboxSecond = secondEntity->getComponent<sCircleHitboxComponent>();

        return (Collisions::circleHitboxCheck(&circleHitboxFirst->center, circleHitboxFirst->radius,
            &circleHitboxSecond->center, circleHitboxSecond->radius));
    }*/
    return (false);
}

bool    CollisionSystem::staticResolution(Entity *firstEntity)
{
    if (firstEntity->getComponent<sProjectileComponent>() != nullptr)
    {
        return (false);
    }
    return (true);
}

bool    CollisionSystem::dynamicResolution(EntityManager &em, Entity *firstEntity, Entity *secondEntity)
{
    if (firstEntity->getComponent<sPlayerComponent>() != nullptr)
    {
        if (secondEntity->getComponent<sPlayerComponent>() != nullptr)
        {
            return (false);
        }
    }
    else if (firstEntity->getComponent<sAIComponent>() != nullptr)
    {
        if (secondEntity->getComponent<sAIComponent>() != nullptr)
        {
            return (false);
        }
    }
    else if (firstEntity->getComponent<sProjectileComponent>() != nullptr)
    {
        if (secondEntity->getComponent<sPlayerComponent>() != nullptr)
        {
            return (false);
        }
        if (secondEntity->getComponent<sAIComponent>() != nullptr)
        {
            Entity* ps;
            sPositionComponent* psPos;
            sPositionComponent* entityPosition = secondEntity->getComponent<sPositionComponent>();

            ps = EntityFactory::createEntity(eArchetype::EMITTER_FIRE);
            psPos = ps->getComponent<sPositionComponent>();
            psPos->value.x = entityPosition->value.x;
            psPos->value.y = entityPosition->value.y;
            psPos->z = entityPosition->z;
            ps->getComponent< sParticleEmitterComponent>()->emitterLife = 1.5f;

            // collision projectile avec ennemi
            em.destroyEntityRegister(firstEntity);
            em.destroyEntityRegister(secondEntity);
        }
    }
    return (true);
}
