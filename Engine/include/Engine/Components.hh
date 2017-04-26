/**
* @Author   Guillaume Labey
*/

#pragma once

#include <memory>
#include <map>
#include <vector>
#include <string>
#include <cstdint>
#include <functional>
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <ECS/Component.hh>

#include <Engine/Core/ScriptFactory.hpp>
#include <Engine/Window/Keyboard.hpp>
#include <Engine/Graphics/Camera.hpp>
#include <Engine/Graphics/Light.hpp>
#include <Engine/Graphics/Model.hpp>
#include <Engine/Graphics/ModelInstance.hpp>
#include <Engine/Graphics/Geometries/Plane.hpp>
#include <Engine/Graphics/Geometries/Geometry.hpp>
#include <Engine/Graphics/Geometries/GeometryFactory.hpp>
#include <Engine/Graphics/Geometries/Box.hpp>
#include <Engine/Graphics/Geometries/Sphere.hpp>
#include <Engine/Graphics/Animator.hpp>
#include <Engine/Graphics/Transform.hpp>
#include <Engine/Graphics/UI/Text.hpp>
#include <Engine/Graphics/UniformBuffer.hpp>
#include <Engine/Utils/Helper.hpp>
#include <Engine/Utils/ResourceManager.hpp>
#include <Engine/Utils/Timer.hpp>

#include <Engine/Core/BaseScript.hpp>

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
    this->_display = component->_display;
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
bool                    _display = true;
bool                    ignoreRaycast = false;
bool                    dynamic = false;
bool                    hideDynamic = false;

glm::vec4 color;

// Last color since ubo update
glm::vec4 lastColor;

END_COMPONENT(sRenderComponent)


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
}

virtual void update(sComponent* component)
{
    update(static_cast<sRigidBodyComponent*>(component));
}

glm::vec3 gravity;
glm::vec3 velocity;
std::map<uint32_t, eCollisionState> collisions;
bool collisionsEnabled;
END_COMPONENT(sRigidBodyComponent)


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


enum class eEntityType
{
    CHARACTER = 0,
    OBJECT = 1,
    TILE_WALKABLE = 2,
    TILE_NOT_WALKABLE = 3,
    TILE_STAIRS_UP = 4,
    TILE_STAIRS_DOWN = 5
};

START_COMPONENT(sTypeComponent)
virtual sComponent* clone()
{
    sTypeComponent* component = new sTypeComponent();
    component->update(this);

    return (component);
}

virtual void update(sTypeComponent* component)
{
    this->type = component->type;
}

virtual void update(sComponent* component)
{
    update(static_cast<sTypeComponent*>(component));
}

eEntityType type;
END_COMPONENT(sTypeComponent)

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

START_COMPONENT(sNameComponent)
sNameComponent(const std::string& name) : value(name), sComponent(sNameComponent::identifier) {}

virtual sComponent* clone()
{
    sNameComponent* component = new sNameComponent();
    component->update(this);

    return (component);
}

virtual void update(sNameComponent* component)
{
    this->value = component->value;
}

virtual void update(sComponent* component)
{
    update(static_cast<sNameComponent*>(component));
}

std::string value;
END_COMPONENT(sNameComponent)


class EntityFactory;

START_COMPONENT_INHERIT(sTransformComponent, Transform)
// TODO: Remove this and use getter/setter in animations ?
friend EntityFactory;

virtual sComponent* clone()
{
    sTransformComponent* component = new sTransformComponent();
    component->update(this);

    return (component);
}

virtual void update(sTransformComponent* component)
{
    this->_pos = component->_pos;
    this->_scale = component->_scale;
    this->_rotation = component->_rotation;
    this->_transform = component->_transform;
    this->_direction = component->_direction;
    this->_up = component->_up;
    this->_right = component->_right;
    this->_posOffsetLocal = component->_posOffsetLocal;
    this->_posOffsetWorld = component->_posOffsetWorld;
}

virtual void update(sComponent* component)
{
    update(static_cast<sTransformComponent*>(component));
}

// Used only for ImgGuizmo display
glm::vec3 _posOffsetLocal;
glm::vec3 _posOffsetWorld;
END_COMPONENT(sTransformComponent)


#define HORIZONTAL_ALIGNMENT(PROCESS)       \
    PROCESS(NONE)                           \
    PROCESS(LEFT)                           \
    PROCESS(MIDDLE)                         \
    PROCESS(RIGHT)

REGISTER_ENUM(eHorizontalAlignment, uint8_t, HORIZONTAL_ALIGNMENT);
REGISTER_ENUM_MANAGER(eHorizontalAlignment, HORIZONTAL_ALIGNMENT);

#define VERTICAL_ALIGNMENT(PROCESS)         \
    PROCESS(NONE)                           \
    PROCESS(TOP)                            \
    PROCESS(MIDDLE)                         \
    PROCESS(BOTTOM)

REGISTER_ENUM(eVerticalAlignment, uint8_t, VERTICAL_ALIGNMENT);
REGISTER_ENUM_MANAGER(eVerticalAlignment, VERTICAL_ALIGNMENT);

START_COMPONENT(sUiComponent)
virtual sComponent* clone()
{
    sUiComponent* component = new sUiComponent();
    component->update(this);

    return (component);
}

virtual void update(sUiComponent* component)
{
    this->horizontalAlignment = component->horizontalAlignment;
    this->verticalAlignment = component->verticalAlignment;
    this->offset = component->offset;
    this->percentageSize = component->percentageSize;
    this->size = component->size;
    this->layer = component->layer;
    this->needUpdate = component->needUpdate;
}

virtual void update(sComponent* component)
{
    update(static_cast<sUiComponent*>(component));
}

eHorizontalAlignment    horizontalAlignment = eHorizontalAlignment::MIDDLE;
eVerticalAlignment      verticalAlignment = eVerticalAlignment::MIDDLE;
glm::vec2               offset{0.0f, 0.0f}; // Percentage offset
bool                    percentageSize{false}; // Use percentage for ui size
glm::vec2               size{0.1f, 0.1f}; // Percentage size
int                     layer{0}; // Layer used for UI ordering
bool                    needUpdate = true;

END_COMPONENT(sUiComponent)


START_COMPONENT(sTextComponent)
virtual sComponent* clone()
{
    sTextComponent* component = new sTextComponent();
    component->update(this);

    return (component);
}

virtual void update(sTextComponent* component)
{
    this->text = component->text;
    this->horizontalAlignment = component->horizontalAlignment;
    this->verticalAlignment = component->verticalAlignment;
    this->offset = component->offset;
}

virtual void update(sComponent* component)
{
    update(static_cast<sTextComponent*>(component));
}

BufferPool::SubBuffer*  getBuffer() const
{
    return (_buffer);
}

BufferPool::SubBuffer*  getBuffer(BufferPool* bufferPool)
{
    if (!_buffer)
    {
        _buffer = bufferPool->allocate();
    }
    return (_buffer);
}

Text text;
eHorizontalAlignment    horizontalAlignment = eHorizontalAlignment::MIDDLE;
eVerticalAlignment      verticalAlignment = eVerticalAlignment::MIDDLE;
glm::vec2 offset; // Offset to align text

private:
BufferPool::SubBuffer* _buffer{nullptr};
END_COMPONENT(sTextComponent)


#define BUTTON_ACTION(PROCESS)              \
    PROCESS(NONE)                           \
    PROCESS(REMOVE_CURRENT_LEVEL)           \
    PROCESS(REPLACE_CURRENT_LEVEL)          \
    PROCESS(ADD_LEVEL)


START_COMPONENT(sButtonComponent)
REGISTER_ENUM(eAction, uint8_t, BUTTON_ACTION);

virtual sComponent* clone()
{
    sButtonComponent* component = new sButtonComponent();
    component->update(this);

    return (component);
}

virtual void update(sButtonComponent* component)
{
    this->selected = component->selected;
    this->hovered = component->hovered;
    this->action = component->action;
    this->actionLevel = component->actionLevel;
}

virtual void update(sComponent* component)
{
    update(static_cast<sButtonComponent*>(component));
}

bool        selected = false;
bool        hovered = false;
eAction     action = eAction::NONE;
std::string actionLevel;
END_COMPONENT(sButtonComponent)
REGISTER_ENUM_MANAGER(sButtonComponent::eAction, BUTTON_ACTION);

START_COMPONENT(sSoundComponent)
virtual sComponent*     clone()
{
    sSoundComponent*    component = new sSoundComponent();
    component->update(this);

    return (component);
}

virtual void            update(sSoundComponent* component)
{

}

virtual void            update(sComponent* component)
{
    update(static_cast<sSoundComponent*>(component));
}

END_COMPONENT(sSoundComponent)

START_COMPONENT(sScriptComponent)
virtual sComponent*     clone()
{
    sScriptComponent*    component = new sScriptComponent();
    component->update(this);

    return (component);
}

virtual void            update(sScriptComponent* component)
{
    for (auto& script: component->scripts)
    {
        JsonValue json = script->saveToJson();

        auto scriptInstance = ScriptFactory::create(script->getName());

        // Little trick to update the component ._.
        scriptInstance->loadFromJson(json);

        this->scripts.push_back(std::move(scriptInstance));
    }
}

virtual void            update(sComponent* component)
{
    update(static_cast<sScriptComponent*>(component));
}

bool                    hasScript(const char* name)
{
    for (const auto& script: scripts)
    {
        if (script->getName() == name)
        {
            return (true);
        }
    }
    return (false);
}

BaseScript*             getScript(const char* name)
{
    uint32_t i = 0;
    for (const auto& script: scripts)
    {
        if (script->getName() == name)
        {
            if (!script->getEntity())
                script->setEntity(entity);

            // If the user need the script before it's initialized, auto initialized it
            if (!script->isInitialized)
            {
                script->start();
                script->isInitialized = true;
            }
            return (script.get());
        }
        i++;
    }

    return nullptr;
}

template <typename T>
T*             getScript(const char* name)
{
    BaseScript* script = getScript(name);
    if (script)
        return static_cast<T*>(script);
    return nullptr;
}

std::vector<std::unique_ptr<BaseScript> > scripts;

BaseScript* selectedScript{nullptr}; // Only used for editor
END_COMPONENT(sScriptComponent)

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


START_COMPONENT(sDynamicComponent)
virtual sComponent* clone()
{
    sDynamicComponent* component = new sDynamicComponent();
    component->update(this);

    return (component);
}

virtual void update(sDynamicComponent* component)
{
}

virtual void update(sComponent* component)
{
    update(static_cast<sDynamicComponent*>(component));
}
END_COMPONENT(sDynamicComponent)
