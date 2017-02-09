/**
* @Author   Simon Ambroise
*/

#include <Engine/Components.hh>

#include <Engine/Systems/RigidBodySystem.hpp>

RigidBodySystem::RigidBodySystem()
{
    this->addDependency<sRigidBodyComponent>();
    this->addDependency<sTransformComponent>();
}

void RigidBodySystem::update(EntityManager &em, float elapsedTime)
{
    this->forEachEntity(em, [&](Entity* entity) {
        sRigidBodyComponent* rigidBody = entity->getComponent<sRigidBodyComponent>();
        sTransformComponent* transform = entity->getComponent<sTransformComponent>();

        rigidBody->velocity += rigidBody->gravity * elapsedTime;
        transform->pos += rigidBody->velocity * elapsedTime;
        transform->needUpdate = true;
    });
}
