/**
* @Author   Simon Ambroise
*/

#include <cmath>
#include <algorithm>

#include <Engine/Window/GameWindow.hpp>
#include <Engine/Physics/Collisions.hpp>
#include <Engine/Graphics/Geometries/Geometry.hpp>
#include <Engine/Utils/Logger.hpp>

#include <Engine/Components.hh>
#include <Engine/EntityFactory.hpp>

#include <Engine/Systems/CollisionSystem.hpp>

CollisionSystem::CollisionSystem()
{
    this->addDependency<sSphereColliderComponent>();
}

void    CollisionSystem::update(EntityManager &em, float elapsedTime)
{
    this->forEachEntity(em, [&](Entity* entity)
    {
        if (entity->getComponent<sBoxColliderComponent>() == nullptr && entity->getComponent<sSphereColliderComponent>() == nullptr) return;

        this->forEachEntity(em, [&](Entity* entityB)
        {
            if (entityB->getComponent<sBoxColliderComponent>() == nullptr && entityB->getComponent<sSphereColliderComponent>() == nullptr) return;

            if (entity->id != entityB->id)
            {
                sResolutionComponent* resolution = entity->getComponent<sResolutionComponent>();

                if (resolution == nullptr) return;

                if (this->isColliding(entity, entityB))
                {
                    //if (!entityB->getComponent<sSphereColliderComponent>()->isTrigger)
                    //    LOG_DEBUG("NOT TRIGGER");

                    if (resolution->resolutions.find(entityB->id) == resolution->resolutions.end() || resolution->resolutions[entityB->id] == eCollisionState::NO_COLLISION)
                    {
                        resolution->resolutions[entityB->id] = eCollisionState::ENTERING_COLLISION;
                    }
                }
                else
                {
                    if (resolution->resolutions[entityB->id] == eCollisionState::IS_COLLIDING)
                        resolution->resolutions[entityB->id] = eCollisionState::EXIT_COLLISION;
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

        return (Collisions::sphereVSsphere(
            firstSphereCollider->pos + firstTransform->pos,
            firstSphereCollider->radius * std::max({ firstTransform->scale.x, firstTransform->scale.y, firstTransform->scale.z }) * SIZE_UNIT,
            secondSphereCollider->pos + secondTransform->pos,
            secondSphereCollider->radius * std::max({ secondTransform->scale.x, secondTransform->scale.y, secondTransform->scale.z }) * SIZE_UNIT
            ));
    }

    return (false);
}
