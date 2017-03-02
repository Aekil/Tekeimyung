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
        if (entity->getComponent<sSphereColliderComponent>())
        {
            this->forEachEntity(em, [&](Entity* entityB)
            {
                if (entityB->getComponent<sSphereColliderComponent>())
                {
                    if (entity->id != entityB->id)
                    {
                        sRigidBodyComponent* rigidBody = entity->getComponent<sRigidBodyComponent>();

                        if (rigidBody->collisionsEnabled && this->isColliding(entity, entityB))
                        {
                            //if (!entityB->getComponent<sSphereColliderComponent>()->isTrigger)
                            //    LOG_DEBUG("NOT TRIGGER");

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
    if (firstEntity->getComponent<sSphereColliderComponent>() != nullptr && secondEntity->getComponent<sSphereColliderComponent>() != nullptr)
    {
        sTransformComponent* firstTransform = firstEntity->getComponent<sTransformComponent>();
        sTransformComponent* secondTransform = secondEntity->getComponent<sTransformComponent>();
        sSphereColliderComponent* firstSphereCollider = firstEntity->getComponent<sSphereColliderComponent>();
        sSphereColliderComponent* secondSphereCollider = secondEntity->getComponent<sSphereColliderComponent>();

        return (Collisions::sphereVSsphere(
            firstSphereCollider->pos + firstTransform->getPos(),
            firstSphereCollider->radius * std::max({ firstTransform->getScale().x, firstTransform->getScale().y, firstTransform->getScale().z }) * SIZE_UNIT,
            secondSphereCollider->pos + firstTransform->getPos(),
            secondSphereCollider->radius * std::max({ secondTransform->getScale().x, secondTransform->getScale().y, secondTransform->getScale().z }) * SIZE_UNIT
            ));
    }

    return (false);
}
