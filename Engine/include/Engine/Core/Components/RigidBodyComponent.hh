#pragma once

#include <glm/vec3.hpp>
#include <unordered_map>
#include <vector>

#include <ECS/Component.hh>
#include <ECS/Entity.hpp>

enum class eCollisionState : uint8_t
{
    NO_COLLISION = 0,
    ENTERING_COLLISION = 1,
    IS_COLLIDING = 2,
    EXIT_COLLISION = 3
};

START_COMPONENT(sRigidBodyComponent)
virtual sComponent* clone()
{
    sRigidBodyComponent* component = new sRigidBodyComponent();
    component->update(this);

    return (component);
}

virtual void update(sRigidBodyComponent* component)
{
    this->gravity = component->gravity;
    this->velocity = component->velocity;
    this->collisionsEnabled = component->collisionsEnabled;
    this->ignoredTags = component->ignoredTags;
}

virtual void update(sComponent* component)
{
    update(static_cast<sRigidBodyComponent*>(component));
}

glm::vec3 gravity;
glm::vec3 velocity;
std::unordered_map<Entity::sHandle, eCollisionState> collisions;
std::vector<std::string> ignoredTags;
bool collisionsEnabled;

int selectedTags = -1;
END_COMPONENT(sRigidBodyComponent)
