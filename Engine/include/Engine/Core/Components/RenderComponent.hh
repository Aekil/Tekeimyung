/**
* @Author   Guillaume Labey
*/

#pragma once

#include <ECS/Component.hh>
#include <Engine/Graphics/Animator.hpp>
#include <Engine/Graphics/ModelInstance.hpp>
#include <Engine/Graphics/Geometries/Box.hpp>
#include <Engine/Graphics/Geometries/Geometry.hpp>
#include <Engine/Graphics/Geometries/GeometryFactory.hpp>
#include <Engine/Graphics/Geometries/Plane.hpp>
#include <Engine/Graphics/Geometries/Sphere.hpp>
#include <Engine/Utils/ResourceManager.hpp>

START_COMPONENT(sRenderComponent)
virtual sComponent* clone()
{
    sRenderComponent* component = new sRenderComponent();
    component->update(this);

    return (component);
}

virtual void update(sRenderComponent* component)
{
    this->modelFile = component->modelFile;
    this->color = component->color;
    this->animated = component->animated;

    if (component->_modelInstance)
    {
        this->_modelInstance = std::make_unique<ModelInstance>(*component->_modelInstance);
    }

    this->type = component->type;
    this->_animator = component->_animator;
    this->display = component->display;
    this->ignoreRaycast = component->ignoreRaycast;
    this->dynamic = component->dynamic;
    this->hideDynamic = component->hideDynamic;
}

virtual void update(sComponent* component)
{
    update(static_cast<sRenderComponent*>(component));
}

void initModelInstance()
{
    if (type == Geometry::eType::MESH)
    {
        Model* model = ResourceManager::getInstance()->getResource<Model>(modelFile);
        _modelInstance = std::make_unique<ModelInstance>(model);
    }
    else
    {
        Model* model = GeometryFactory::getGeometry(type);
        _modelInstance = std::make_unique<ModelInstance>(model);
    }
}

ModelInstance* getModelInstance()
{
    if (!_modelInstance)
        initModelInstance();
    return _modelInstance.get();
}

const Model* getModel()
{
    if (!_modelInstance)
        initModelInstance();
    return _modelInstance->getModel();
}

std::string modelFile;
bool animated;

std::unique_ptr<ModelInstance> _modelInstance = nullptr;

Animator                _animator;

// Model type
Geometry::eType type;
bool                    display = true;
bool                    ignoreRaycast = false;
bool                    dynamic = false;
bool                    hideDynamic = false;

glm::vec4 color;

// Last color since ubo update
glm::vec4 lastColor;

END_COMPONENT(sRenderComponent)
