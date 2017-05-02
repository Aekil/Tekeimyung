/**
* @Author   Simon Ambroise
*/

#include <cmath>
#include <algorithm>

#include <Engine/Window/GameWindow.hpp>
#include <Engine/Physics/Collisions.hpp>
#include <Engine/Graphics/Geometries/Geometry.hpp>
#include <Engine/Debug/Logger.hpp>

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

                        if (std::find(rigidBody->ignoredTags.begin(), rigidBody->ignoredTags.end(), (*it)->getTag()) != rigidBody->ignoredTags.end())
                            continue;
                        
                        auto colliding = Collisions::isColliding(entity, (*it));

                        if (rigidBody->collisionsEnabled && colliding)
                        {
                            if (((*it)->getComponent<sSphereColliderComponent>() != nullptr && !(*it)->getComponent<sSphereColliderComponent>()->isTrigger
                                || (*it)->getComponent<sBoxColliderComponent>() != nullptr && !(*it)->getComponent<sBoxColliderComponent>()->isTrigger)
                                && ((entity->getComponent<sSphereColliderComponent>() != nullptr && !entity->getComponent<sSphereColliderComponent>()->isTrigger)
                                    || entity->getComponent<sBoxColliderComponent>() != nullptr && !entity->getComponent<sBoxColliderComponent>()->isTrigger))
                            {
                                rigidBody->velocity = glm::vec3(0.0f);
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
