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
    this->addDependency<sDynamicComponent>();
}

void    CollisionSystem::update(EntityManager &em, float elapsedTime)
{
    const std::vector<Entity*>& entities = em.getEntitiesByComponent<sRigidBodyComponent>();
    
    this->forEachEntity(em, [&](Entity* entity)
    {
        if (entity->getComponent<sSphereColliderComponent>() || entity->getComponent<sBoxColliderComponent>())
        {
            for (std::vector<Entity*>::const_iterator it = entities.begin(); it != entities.end(); ++it)
            {
                if ((*it)->getComponent<sSphereColliderComponent>() || (*it)->getComponent<sBoxColliderComponent>())
                {
                    if (entity->id != (*it)->id)
                    {
                        sRigidBodyComponent* rigidBody = entity->getComponent<sRigidBodyComponent>();
                        sRigidBodyComponent* rigidBodyB = (*it)->getComponent<sRigidBodyComponent>();
                        auto colliding = this->isColliding(entity, (*it));

                        if (rigidBody->collisionsEnabled && colliding)
                        {
                            if ((*it)->getComponent<sSphereColliderComponent>() != nullptr && !(*it)->getComponent<sSphereColliderComponent>()->isTrigger
                                || (*it)->getComponent<sBoxColliderComponent>() != nullptr && !(*it)->getComponent<sBoxColliderComponent>()->isTrigger)
                            {
                                //LOG_DEBUG("NOT TRIGGER");
                            }

                            if (rigidBody->collisions.find((*it)->id) == rigidBody->collisions.end() || rigidBody->collisions[(*it)->id] == eCollisionState::NO_COLLISION)
                            {
                                rigidBody->collisions[(*it)->id] = eCollisionState::ENTERING_COLLISION;
                            }
                        }
                        else if (rigidBody->collisionsEnabled)
                        {
                            if (rigidBody->collisions[(*it)->id] == eCollisionState::IS_COLLIDING)
                                rigidBody->collisions[(*it)->id] = eCollisionState::EXIT_COLLISION;
                        }

                        if (rigidBodyB->collisionsEnabled && colliding && (*it)->getComponent<sDynamicComponent>() == nullptr)
                        {
                            if (rigidBodyB->collisions.find(entity->id) == rigidBodyB->collisions.end() || rigidBodyB->collisions[entity->id] == eCollisionState::NO_COLLISION)
                            {
                                rigidBodyB->collisions[entity->id] = eCollisionState::ENTERING_COLLISION;
                            }
                        }
                        else if (rigidBodyB->collisionsEnabled && (*it)->getComponent<sDynamicComponent>() == nullptr)
                        {
                            if (rigidBodyB->collisions[entity->id] == eCollisionState::IS_COLLIDING)
                                rigidBodyB->collisions[entity->id] = eCollisionState::EXIT_COLLISION;
                        }
                    }
                }
            }
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
