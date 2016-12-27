/**
* @Author   Guillaume Labey
*/

#pragma once

#include <memory>
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

#include <Engine/Window/Keyboard.hpp>
#include <Engine/Graphics/Model.hpp>
#include <Engine/Graphics/Geometries/Geometry.hpp>
#include <Engine/Graphics/Geometries/GeometryFactory.hpp>
#include <Engine/Graphics/Geometries/Box.hpp>
#include <Engine/Graphics/Geometries/Sphere.hpp>
#include <Engine/Graphics/Animator.hpp>
#include <Engine/Utils/RessourceManager.hpp>
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
    this->_model = component->_model;
    this->type = component->type;
    this->texture = component->texture;
    this->_animator = component->_animator;
    this->_display = component->_display;
}

virtual void update(sComponent* component)
{
    update(static_cast<sRenderComponent*>(component));
}

void initModel()
{
    if (type == Geometry::eType::MESH)
        _model = RessourceManager::getInstance()->getModel(modelFile);
    else
        _model = GeometryFactory::create(type, texture);
}

std::string modelFile;
glm::vec4 color;
bool animated;

std::shared_ptr<Model> _model;

Animator                _animator;

// Model type
Geometry::eType type;
bool                    _display = true;

// texture for geometries
std::string texture;
END_COMPONENT(sRenderComponent)

START_COMPONENT(sPositionComponent)
virtual sComponent* clone()
{
    sPositionComponent* component = new sPositionComponent();
    component->update(this);

    return (component);
}

virtual void update(sPositionComponent* component)
{
    this->value = component->value;
    this->z = component->z;
}

virtual void update(sComponent* component)
{
    update(static_cast<sPositionComponent*>(component));
}

//  X and Y coordinates
glm::vec2 value;

//  Z coordinates (aka layer)
float   z;
END_COMPONENT(sPositionComponent)

START_COMPONENT(sDirectionComponent)
sDirectionComponent(const glm::vec2& dir, float speed = 1.0f) : value(dir), moved(false), speed(speed), sComponent(sDirectionComponent::identifier) {}

virtual sComponent* clone()
{
    sDirectionComponent* component = new sDirectionComponent();
    component->update(this);

    return (component);
}

virtual void update(sDirectionComponent* component)
{
    this->value = component->value;
    this->orientation = component->orientation;
    this->speed = component->speed;
    this->moved = component->moved;
}

virtual void update(sComponent* component)
{
    update(static_cast<sDirectionComponent*>(component));
}

glm::vec2 value;
glm::vec3 orientation;
float speed = 1.0f;
bool moved = false;
END_COMPONENT(sDirectionComponent)

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
std::shared_ptr<Box> box;
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

std::shared_ptr<Sphere> sphere;
bool display = true;
END_COMPONENT(sSphereColliderComponent)

START_COMPONENT(sGravityComponent)
virtual sComponent* clone()
{
    sGravityComponent* component = new sGravityComponent();
    component->update(this);

    return (component);
}

virtual void update(sGravityComponent* component)
{
    this->value = component->value;
}

virtual void update(sComponent* component)
{
    update(static_cast<sGravityComponent*>(component));
}

glm::vec2 value;
END_COMPONENT(sGravityComponent)

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
    this->spawnNb = component->spawnNb;
    this->emitterLife = component->emitterLife;
    this->life = component->life;
    this->lifeVariance = component->lifeVariance;
    this->angle = component->angle;
    this->angleVariance = component->angleVariance;
    this->speed = component->speed;
    this->speedVariance = component->speedVariance;

    this->colorStart = component->colorStart;
    this->colorFinish = component->colorFinish;
    this->colorStartVariance = component->colorStartVariance;
    this->colorFinishVariance = component->colorFinishVariance;

    this->sizeStart = component->sizeStart;
    this->sizeFinish = component->sizeFinish;
    this->sizeStartVariance = component->sizeStartVariance;
    this->sizeFinishVariance = component->sizeFinishVariance;

    this->texture = component->texture;
}

virtual void update(sComponent* component)
{
    update(static_cast<sParticleEmitterComponent*>(component));
}

// Spawn particles each rate second
float rate;

// Spawn spawnNb particles each rate
uint32_t spawnNb;

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
glm::vec4 colorStartVariance;
glm::vec4 colorFinishVariance;

// Particles size
float sizeStart;
float sizeFinish;
float sizeStartVariance;
float sizeFinishVariance;

// Particles texture
std::string texture;
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

enum class eCollisionState : int
{
    NO_COLLISION = 0,
    ENTERING_COLLISION,
    IS_COLLIDING,
    EXIT_COLLISION,
};

START_COMPONENT(sResolutionComponent)
virtual sComponent* clone()
{
    sResolutionComponent* component = new sResolutionComponent();
    component->update(this);

    return (component);
}

virtual void update(sResolutionComponent* component)
{
    this->entityId = component->entityId;
    this->collidingState = component->collidingState;
}

virtual void update(sComponent* component)
{
    update(static_cast<sResolutionComponent*>(component));
}

//Entity id with which is colliding
int entityId;

//Boolean if we need to resolve collisions or not
eCollisionState collidingState;
END_COMPONENT(sResolutionComponent)

START_COMPONENT(sTransformComponent)
virtual sComponent* clone()
{
    sTransformComponent* component = new sTransformComponent();
    component->update(this);

    return (component);
}

virtual void update(sTransformComponent* component)
{
    this->pos = component->pos;
    this->scale = component->scale;
    this->rotation = component->rotation;
    this->transform = component->transform;
}

virtual void update(sComponent* component)
{
    update(static_cast<sTransformComponent*>(component));
}

void updateTransform()
{
    glm::quat newRotate(glm::vec3(glm::radians(rotation.x), glm::radians(rotation.y), glm::radians(rotation.z)));

    needUpdate = false;
    glm::mat4 newTranslate = glm::translate(glm::mat4(1.0), glm::vec3(pos.x, pos.y, pos.z));
    glm::mat4 newScale = glm::scale(glm::mat4(1.0), scale);
    transform = newTranslate * glm::mat4_cast(newRotate) * newScale;
}

const glm::mat4& getTransform()
{
    if (needUpdate)
    {
        updateTransform();
    }
    return (transform);
}

glm::vec3   pos;
glm::vec3   scale = { 1.0f, 1.0f, 1.0f };
glm::vec3   rotation;
glm::mat4   transform = glm::mat4(1.0f);
bool        needUpdate = true;
END_COMPONENT(sTransformComponent)


START_COMPONENT(sButtonComponent)
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
}

virtual void update(sComponent* component)
{
    update(static_cast<sButtonComponent*>(component));
}

bool        selected = false;
bool        hovered = false;
END_COMPONENT(sButtonComponent)


START_COMPONENT(sTileComponent)
virtual sComponent* clone()
{
    sTileComponent* component = new sTileComponent();
    component->update(this);

    return (component);
}

virtual void update(sTileComponent* component) {}

virtual void update(sComponent* component)
{
    update(static_cast<sTileComponent*>(component));
}
END_COMPONENT(sTileComponent)

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
    this->scriptNames = component->scriptNames;
    this->isInitialized = component->isInitialized;
}

virtual void            update(sComponent* component)
{
    update(static_cast<sScriptComponent*>(component));
}

std::vector<BaseScript*> scriptInstances;
std::vector<std::string> scriptNames;
bool isInitialized;

END_COMPONENT(sScriptComponent)