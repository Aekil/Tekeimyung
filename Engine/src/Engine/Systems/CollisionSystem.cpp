/**
* @Author   Simon Ambroise
*/

#include <algorithm>
#include <cmath>

#include <Engine/Core/Components/BoxColliderComponent.hh>
#include <Engine/Core/Components/DynamicComponent.hh>
#include <Engine/Core/Components/SphereColliderComponent.hh>
#include <Engine/Core/Components/RigidBodyComponent.hh>
#include <Engine/Core/Components/TransformComponent.hh>
#include <Engine/Debug/Logger.hpp>
#include <Engine/EntityFactory.hpp>
#include <Engine/Graphics/Geometries/Geometry.hpp>
#include <Engine/Physics/Collisions.hpp>
#include <Engine/Window/GameWindow.hpp>

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
                    if (entity->handle != (*it)->handle)
                    {
                        sRigidBodyComponent* rigidBody = entity->getComponent<sRigidBodyComponent>();
                        sRigidBodyComponent* rigidBodyB = (*it)->getComponent<sRigidBodyComponent>();

                        if (std::find(rigidBody->ignoredTags.begin(), rigidBody->ignoredTags.end(), (*it)->getTag()) != rigidBody->ignoredTags.end())
                            continue;

                        if (std::find(rigidBodyB->ignoredTags.begin(), rigidBodyB->ignoredTags.end(), entity->getTag()) != rigidBodyB->ignoredTags.end())
                            continue;

                        sTransformComponent* firstTransform = entity->getComponent<sTransformComponent>();
                        sTransformComponent* secondTransform = (*it)->getComponent<sTransformComponent>();

                        // Pre-calculate new position for collision detection
                        firstTransform->translate(rigidBody->velocity * elapsedTime);
                        secondTransform->translate(rigidBodyB->velocity * elapsedTime);

                        auto colliding = Collisions::isColliding(entity, (*it));

                        // Rollback translate
                        firstTransform->translate(-rigidBody->velocity * elapsedTime);
                        secondTransform->translate(-rigidBodyB->velocity * elapsedTime);

                        if (rigidBody->collisionsEnabled && rigidBodyB->collisionsEnabled)
                        {
                            if (colliding)
                            {
                                if (((*it)->getComponent<sSphereColliderComponent>() != nullptr && !(*it)->getComponent<sSphereColliderComponent>()->isTrigger
                                    || (*it)->getComponent<sBoxColliderComponent>() != nullptr && !(*it)->getComponent<sBoxColliderComponent>()->isTrigger)
                                    && ((entity->getComponent<sSphereColliderComponent>() != nullptr && !entity->getComponent<sSphereColliderComponent>()->isTrigger)
                                        || entity->getComponent<sBoxColliderComponent>() != nullptr && !entity->getComponent<sBoxColliderComponent>()->isTrigger))
                                {
                                    rigidBody->velocity = glm::vec3(0.0f);
                                }

                                if (rigidBody->collisions.find((*it)->handle) == rigidBody->collisions.end() || rigidBody->collisions[(*it)->handle] == eCollisionState::NO_COLLISION)
                                {
                                    rigidBody->collisions[(*it)->handle] = eCollisionState::ENTERING_COLLISION;
                                }
                            }

                            if (rigidBody->collisions[(*it)->handle] == eCollisionState::IS_COLLIDING)
                                rigidBody->collisions[(*it)->handle] = eCollisionState::EXIT_COLLISION;

                            if (colliding && (*it)->getComponent<sDynamicComponent>() == nullptr)
                            {
                                if (rigidBodyB->collisions.find(entity->handle) == rigidBodyB->collisions.end() || rigidBodyB->collisions[entity->handle] == eCollisionState::NO_COLLISION)
                                {
                                    rigidBodyB->collisions[entity->handle] = eCollisionState::ENTERING_COLLISION;
                                }
                            }
                            else if ((*it)->getComponent<sDynamicComponent>() == nullptr)
                            {
                                if (rigidBodyB->collisions[entity->handle] == eCollisionState::IS_COLLIDING)
                                    rigidBodyB->collisions[entity->handle] = eCollisionState::EXIT_COLLISION;
                            }
                        }
                    }
                }
            }
        }
    });
}
