/**
* @Author   Guillaume Labey
*/

#pragma once

#include <glm/vec3.hpp>
#include <memory>

#include <ECS/Component.hh>
#include <Engine/Graphics/ModelInstance.hpp>

START_COMPONENT(sSphereColliderComponent)
virtual sComponent* clone()
{
    sSphereColliderComponent* component = new sSphereColliderComponent();
    component->update(this);

    return (component);
}

virtual void update(sSphereColliderComponent* component)
{
    this->pos = component->pos;
    this->radius = component->radius;
    this->isTrigger = component->isTrigger;
}

virtual void update(sComponent* component)
{
    update(static_cast<sSphereColliderComponent*>(component));
}

glm::vec3 pos;
float radius;

// isTrigger
bool isTrigger;

std::unique_ptr<ModelInstance> sphere;
bool display = true;
END_COMPONENT(sSphereColliderComponent)
