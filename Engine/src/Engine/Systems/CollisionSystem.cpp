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
    this->addDependency<sRigidBodyComponent>();

}

void    CollisionSystem::update(EntityManager &em, float elapsedTime)
{
    this->forEachEntity(em, [&](Entity* entity)
    {
        if (entity->getComponent<sSphereColliderComponent>() || entity->getComponent<sBoxColliderComponent>())
        {
            this->forEachEntity(em, [&](Entity* entityB)
            {
                if (entityB->getComponent<sSphereColliderComponent>() || entityB->getComponent<sBoxColliderComponent>())
                {
                    if (entity->id != entityB->id)
                    {
                        sRigidBodyComponent* rigidBody = entity->getComponent<sRigidBodyComponent>();

                        if (rigidBody->collisionsEnabled && this->isColliding(entity, entityB))
                        {
                            if (entityB->getComponent<sSphereColliderComponent>() != nullptr && !entityB->getComponent<sSphereColliderComponent>()->isTrigger
                                || entityB->getComponent<sBoxColliderComponent>() != nullptr && !entityB->getComponent<sBoxColliderComponent>()->isTrigger)
                            {
                                //LOG_DEBUG("NOT TRIGGER");
                            }

                            if (rigidBody->collisions.find(entityB->id) == rigidBody->collisions.end() || rigidBody->collisions[entityB->id] == eCollisionState::NO_COLLISION)
                            {
                                rigidBody->collisions[entityB->id] = eCollisionState::ENTERING_COLLISION;
                            }
                        }
                        else if (rigidBody->collisionsEnabled)
                        {
                            if (rigidBody->collisions[entityB->id] == eCollisionState::IS_COLLIDING)
                                rigidBody->collisions[entityB->id] = eCollisionState::EXIT_COLLISION;
                        }
                    }
                }

            });
        }

    });
}

bool    CollisionSystem::isColliding(Entity *firstEntity, Entity *secondEntity)
{
    sTransformComponent* firstTransform = firstEntity->getComponent<sTransformComponent>();
    sTransformComponent* secondTransform = secondEntity->getComponent<sTransformComponent>();

    if (firstEntity->getComponent<sSphereColliderComponent>() != nullptr && secondEntity->getComponent<sSphereColliderComponent>() != nullptr)
    {
        sSphereColliderComponent* firstSphereCollider = firstEntity->getComponent<sSphereColliderComponent>();
        sSphereColliderComponent* secondSphereCollider = secondEntity->getComponent<sSphereColliderComponent>();

        return (Collisions::sphereVSsphere(
            firstSphereCollider->pos + firstTransform->getPos(),
            firstSphereCollider->radius * std::max({ firstTransform->getScale().x, firstTransform->getScale().y, firstTransform->getScale().z }) * SIZE_UNIT,
            secondSphereCollider->pos + secondTransform->getPos(),
            secondSphereCollider->radius * std::max({ secondTransform->getScale().x, secondTransform->getScale().y, secondTransform->getScale().z }) * SIZE_UNIT
        ));
    }

    if (firstEntity->getComponent<sBoxColliderComponent>() != nullptr && secondEntity->getComponent<sSphereColliderComponent>() != nullptr)
    {
        sBoxColliderComponent* firstBoxCollider = firstEntity->getComponent<sBoxColliderComponent>();
        sSphereColliderComponent* secondSphereCollider = secondEntity->getComponent<sSphereColliderComponent>();

        return (Collisions::sphereVSAABB(
            secondSphereCollider->pos + secondTransform->getPos(),
            secondSphereCollider->radius * std::max({ secondTransform->getScale().x, secondTransform->getScale().y, secondTransform->getScale().z }) * SIZE_UNIT,
            firstBoxCollider->pos + firstTransform->getPos(),
            glm::vec3(firstBoxCollider->size.x * firstTransform->getScale().x, firstBoxCollider->size.y * firstTransform->getScale().y, firstBoxCollider->size.z * firstTransform->getScale().z)
        ));
    }

    if (secondEntity->getComponent<sBoxColliderComponent>() != nullptr && firstEntity->getComponent<sSphereColliderComponent>() != nullptr)
    {
        sBoxColliderComponent* boxCollider = secondEntity->getComponent<sBoxColliderComponent>();
        sSphereColliderComponent* sphereCollider = firstEntity->getComponent<sSphereColliderComponent>();

        return (Collisions::sphereVSAABB(
            sphereCollider->pos + firstTransform->getPos(),
            sphereCollider->radius * std::max({ firstTransform->getScale().x, firstTransform->getScale().y, firstTransform->getScale().z }) * SIZE_UNIT,
            boxCollider->pos + secondTransform->getPos(),
            glm::vec3(boxCollider->size.x * secondTransform->getScale().x, boxCollider->size.y * secondTransform->getScale().y, boxCollider->size.z * secondTransform->getScale().z)
        ));
    }

    return (false);
}
