/**
* @Author   Guillaume Labey
*/

#pragma once

#include <glm/vec3.hpp>

#include <ECS/Component.hh>
#include <Engine/Graphics/Transform.hpp>

START_COMPONENT_INHERIT(sTransformComponent, Transform)
// TODO: Remove this and use getter/setter in animations ?
friend class EntityFactory;

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
