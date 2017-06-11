/**
* @Author   Guillaume Labey
*/

#pragma once

#include <glm/vec3.hpp>
#include <memory>

#include <ECS/Component.hh>
#include <Engine/Graphics/ModelInstance.hpp>

START_COMPONENT(sBoxColliderComponent)
virtual sComponent* clone()
{
    sBoxColliderComponent* component = new sBoxColliderComponent();
    component->update(this);

    return (component);
}

virtual void update(sBoxColliderComponent* component)
{
    this->pos = component->pos;
    this->size = component->size;
    this->isTrigger = component->isTrigger;
}

virtual void update(sComponent* component)
{
    update(static_cast<sBoxColliderComponent*>(component));
}

// Relative position to sTransformComponent
glm::vec3 pos;

// Scale
glm::vec3 size;

// isTrigger
bool isTrigger;

// Box model
std::unique_ptr<ModelInstance> box;
bool display = true;
END_COMPONENT(sBoxColliderComponent)
