#include <cmath>
#include <algorithm>

#include <Engine/Window/GameWindow.hpp>
#include <Engine/Physics/Collisions.hpp>
#include <Engine/Graphics/Geometries/Geometry.hpp>

#include <Game/Components.hh>
#include <Game/EntityFactory.hpp>

#include <Game/Systems/CollisionSystem.hpp>

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
        if (entity->getComponent<sBoxColliderComponent>() == nullptr && entity->getComponent<sSphereColliderComponent>() == nullptr) return;

        sDirectionComponent* direction = entity->getComponent<sDirectionComponent>();
        sPositionComponent* position = entity->getComponent<sPositionComponent>();
        uint16_t layer = (uint16_t)position->z;

        //// Check Collision with tile
        //// CRASH ON SPAWN RANDOM ENTITIES
        //if ((*_map)[layer][(uint32_t)std::floor(position->value.y)][(uint32_t)std::floor(position->value.x)].get() != 0 ||
        //    (layer > 0 && (*collisionMap)[layer - 1][(uint32_t)std::floor(position->value.y)][(uint32_t)std::floor(position->value.x)] == eColType::CAN_NOT_WALK))
        //{
        //    if (this->staticResolution(entity))
        //    {
        //        position->value -= direction->value * elapsedTime;
        //    }
        //}
        //else
        //{

        this->forEachEntity(em, [&](Entity* entityB)
        {
            if (entityB->getComponent<sBoxColliderComponent>() == nullptr && entityB->getComponent<sSphereColliderComponent>() == nullptr) return;

            sPositionComponent* positionB = entityB->getComponent<sPositionComponent>();
            if (entity->id != entityB->id)
            {
                sResolutionComponent* resolution = entity->getComponent<sResolutionComponent>();

                if (this->isColliding(entity, entityB))
                {
                    if (!entityB->getComponent<sSphereColliderComponent>()->isTrigger)
                        position->value -= direction->value * elapsedTime;

                    if (resolution->collidingState == eCollisionState::NO_COLLISION) 
                    {
                        resolution->collidingState = eCollisionState::ENTERING_COLLISION;
                        resolution->entityId = entityB->id;
                    }
                }
                else
                {
                    if (resolution->collidingState == eCollisionState::IS_COLLIDING)
                        resolution->collidingState = eCollisionState::NO_COLLISION;
                }
            }
        });
    });
}

bool    CollisionSystem::isColliding(Entity *firstEntity, Entity *secondEntity)
{
    if (firstEntity->getComponent<sSphereColliderComponent>() != nullptr && secondEntity->getComponent<sSphereColliderComponent>() != nullptr)
    {
        sTransformComponent* firstTransform = firstEntity->getComponent<sTransformComponent>();
        sTransformComponent* secondTransform = secondEntity->getComponent<sTransformComponent>();
        sSphereColliderComponent* firstSphereCollider = firstEntity->getComponent<sSphereColliderComponent>();
        sSphereColliderComponent* secondSphereCollider = secondEntity->getComponent<sSphereColliderComponent>();

        return (Collisions::sphereColliderCheck(
            firstSphereCollider->pos + firstTransform->pos,
            firstSphereCollider->radius * std::max({ firstTransform->scale.x, firstTransform->scale.y, firstTransform->scale.z }) * SIZE_UNIT,
            secondSphereCollider->pos + secondTransform->pos,
            secondSphereCollider->radius * std::max({ secondTransform->scale.x, secondTransform->scale.y, secondTransform->scale.z }) * SIZE_UNIT
        ));
    }

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
