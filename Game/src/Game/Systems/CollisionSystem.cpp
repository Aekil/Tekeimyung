/**
* @Author   Simon Ambroise
*/

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

    _monitoringKey = MonitoringDebugWindow::getInstance()->registerSystem(COLLISION_SYSTEM_NAME);
}

void    CollisionSystem::update(EntityManager &em, float elapsedTime)
{
    Timer   timer;
    uint32_t nbEntities = 0;

    this->forEachEntity(em, [&](Entity* entity)
    {
        if (entity->getComponent<sBoxColliderComponent>() == nullptr && entity->getComponent<sSphereColliderComponent>() == nullptr) return;

        sDirectionComponent* direction = entity->getComponent<sDirectionComponent>();
        sPositionComponent* position = entity->getComponent<sPositionComponent>();

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
                        resolution->collidingState = eCollisionState::EXIT_COLLISION;
                }
            }
        });
        ++nbEntities;
    });

    MonitoringDebugWindow::getInstance()->updateSystem(_monitoringKey, timer.getElapsedTime(), nbEntities);
}

bool    CollisionSystem::isColliding(Entity *firstEntity, Entity *secondEntity)
{
    if (firstEntity->getComponent<sSphereColliderComponent>() != nullptr && secondEntity->getComponent<sSphereColliderComponent>() != nullptr)
    {
        sTransformComponent* firstTransform = firstEntity->getComponent<sTransformComponent>();
        sTransformComponent* secondTransform = secondEntity->getComponent<sTransformComponent>();
        sSphereColliderComponent* firstSphereCollider = firstEntity->getComponent<sSphereColliderComponent>();
        sSphereColliderComponent* secondSphereCollider = secondEntity->getComponent<sSphereColliderComponent>();

        return (Collisions::sphereVSsphere(
            firstSphereCollider->pos + firstTransform->pos,
            firstSphereCollider->radius * std::max({ firstTransform->scale.x, firstTransform->scale.y, firstTransform->scale.z }) * SIZE_UNIT,
            secondSphereCollider->pos + secondTransform->pos,
            secondSphereCollider->radius * std::max({ secondTransform->scale.x, secondTransform->scale.y, secondTransform->scale.z }) * SIZE_UNIT
            ));
    }

    return (false);
}
