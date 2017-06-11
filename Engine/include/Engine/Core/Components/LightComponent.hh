/**
* @Author   Guillaume Labey
*/

#pragma once

#include <glm/vec3.hpp>
#include <memory>

#include <ECS/Component.hh>
#include <Engine/Graphics/Light.hpp>
#include <Engine/Graphics/ModelInstance.hpp>

START_COMPONENT(sLightComponent)
virtual sComponent* clone()
{
    sLightComponent* component = new sLightComponent();
    component->update(this);

    return (component);
}

virtual void update(sLightComponent* component)
{
    this->light = component->light;
}

virtual void update(sComponent* component)
{
    update(static_cast<sLightComponent*>(component));
}

Light light;
std::unique_ptr<ModelInstance> _lightCone = nullptr;

glm::vec3 lastRotation;
END_COMPONENT(sLightComponent)
