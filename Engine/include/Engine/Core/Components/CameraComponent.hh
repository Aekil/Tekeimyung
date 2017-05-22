#pragma once

#include <memory>

#include <ECS/Component.hh>
#include <Engine/Graphics/Camera.hpp>
#include <Engine/Graphics/Geometries/Geometry.hpp>
#include <Engine/Graphics/ModelInstance.hpp>

START_COMPONENT(sCameraComponent)
virtual sComponent* clone()
{
    sCameraComponent* component = new sCameraComponent();
    component->update(this);

    return (component);
}

virtual void update(sCameraComponent* component)
{
    this->camera = component->camera;
}

virtual void update(sComponent* component)
{
    update(static_cast<sCameraComponent*>(component));
}

Camera camera;
// View model instance
std::unique_ptr<ModelInstance> _cameraView = nullptr;

// Camera perspective need to create a trapeze with specific dimensions
// so we can't use a model from GeometryFactory and just scale it
std::unique_ptr<Geometry> _cameraPerspective = nullptr;
END_COMPONENT(sCameraComponent)
