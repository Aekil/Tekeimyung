#pragma once

#include <glm/vec4.hpp>
#include <memory>
#include <string>

#include <ECS/Component.hh>
#include <Engine/Graphics/Geometries/Geometry.hpp>
#include <Engine/Graphics/Geometries/GeometryFactory.hpp>
#include <Engine/Graphics/ModelInstance.hpp>
#include <Engine/Utils/ResourceManager.hpp>

START_COMPONENT(sParticleEmitterComponent)
virtual sComponent* clone()
{
    sParticleEmitterComponent* component = new sParticleEmitterComponent();
    component->update(this);

    return (component);
}

virtual void update(sParticleEmitterComponent* component)
{
    this->rate = component->rate;
    this->maxParticles = component->maxParticles;
    this->emitterLife = component->emitterLife;
    this->life = component->life;
    this->lifeVariance = component->lifeVariance;
    this->angle = component->angle;
    this->angleVariance = component->angleVariance;
    this->speed = component->speed;
    this->speedVariance = component->speedVariance;

    this->colorStart = component->colorStart;
    this->colorFinish = component->colorFinish;

    this->sizeStart = component->sizeStart;
    this->sizeFinish = component->sizeFinish;
    this->sizeStartVariance = component->sizeStartVariance;
    this->sizeFinishVariance = component->sizeFinishVariance;

    this->displayOnlyParticles = component->displayOnlyParticles;

    this->modelFile = component->modelFile;
    this->type = component->type;
    if (component->_modelInstance)
    {
        this->_modelInstance = std::make_unique<ModelInstance>(*component->_modelInstance);
    }
}

virtual void update(sComponent* component)
{
    update(static_cast<sParticleEmitterComponent*>(component));
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

// Spawn particles each rate second
float rate;

// Limit the number of particles
uint32_t maxParticles;

// Emitter life, in seconds
float emitterLife;

// Particle life time in frame
uint32_t life;
uint32_t lifeVariance;

// Particles spawn angle
float angle;
float angleVariance;

// Particles speed
float speed;
float speedVariance;

// Particles color
glm::vec4 colorStart;
glm::vec4 colorFinish;

// Particles size
float sizeStart;
float sizeFinish;
float sizeStartVariance;
float sizeFinishVariance;

// Particles model
std::string modelFile;
Geometry::eType type;
std::unique_ptr<ModelInstance> _modelInstance = nullptr;

// Set this boolean to display only particles, not the sRenderComponent
bool displayOnlyParticles = true;
END_COMPONENT(sParticleEmitterComponent)
