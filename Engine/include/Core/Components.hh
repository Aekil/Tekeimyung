#pragma once

#include <iostream>
#include "Component.hh"
#include <vector>
#include <string>
#include <cstdint>
#include "Window/Keyboard.hpp"
#include "Graphics/Animation.hpp"
#include "Graphics/Sprite.hpp"
#include <glm/vec2.hpp>


struct sRenderComponent: sComponent
{
    sRenderComponent() = default;
    sRenderComponent(Sprite::eType type, const std::string& texture, bool animated = false, uint32_t nbFrames = 0, const std::vector<eOrientation>& orientations = {}, const glm::vec2& spriteSize = {0, 0})
    : texture(texture), type(type), animated(animated), nbFrames(nbFrames), orientations(orientations), spriteSize(spriteSize), _sprite(nullptr) {}

    virtual sComponent* clone()
    {
        sRenderComponent* component = new sRenderComponent();

        component->texture = this->texture;
        component->type = this->type;
        component->animated = this->animated;
        component->nbFrames = this->nbFrames;
        component->spriteSize = this->spriteSize;
        component->orientations = this->orientations;

        return (component);
    }

    std::string texture;
    Sprite::eType type;
    bool animated;
    uint32_t nbFrames;

    // Sprite width and height
    glm::vec2 spriteSize;
    Sprite* _sprite;

    std::vector<eOrientation> orientations;
};

struct sPositionComponent: sComponent
{
    sPositionComponent() = default;
    sPositionComponent(glm::vec2 value, float z): value(value), z(z) {}

    virtual sComponent* clone()
    {
        sPositionComponent* component = new sPositionComponent();

        component->value = this->value;
        component->z = this->z;

        return (component);
    }

    glm::vec2 value;

    // Layer number
    float z;
};

struct sInputComponent: sComponent {
    sInputComponent() = default;
    sInputComponent(Keyboard::eKey moveLeft,
                    Keyboard::eKey moveRight,
                    Keyboard::eKey moveUp,
                    Keyboard::eKey moveDown): moveLeft(moveLeft), moveRight(moveRight), moveUp(moveUp), moveDown(moveDown) {}

    virtual sComponent* clone()
    {
        sInputComponent* component = new sInputComponent();

        component->moveLeft = this->moveLeft;
        component->moveRight = this->moveRight;
        component->moveUp = this->moveUp;
        component->moveDown = this->moveDown;

        return (component);
    }

    Keyboard::eKey          moveLeft;
    Keyboard::eKey          moveRight;
    Keyboard::eKey          moveUp;
    Keyboard::eKey          moveDown;
};

struct sDirectionComponent : sComponent
{
    sDirectionComponent() = default;
    sDirectionComponent(glm::vec2) : value(value), orientation(eOrientation::N), moved(false), speed(1.0f) {}

    virtual sComponent* clone()
    {
        sDirectionComponent* component = new sDirectionComponent();

        component->value = this->value;
        component->orientation = this->orientation;
        component->speed = this->speed;
        component->moved = this->moved;

        return (component);
    }

    glm::vec2 value;
    eOrientation orientation;
    float speed;
    bool moved;
};

struct sHitBoxComponent : sComponent
{
    sHitBoxComponent() = default;
    sHitBoxComponent(glm::vec2 min, glm::vec2 max) : min(min), max(max) {}

    virtual sComponent* clone()
    {
        sHitBoxComponent* component = new sHitBoxComponent();

        component->min = this->min;
        component->max = this->max;

        return (component);
    }

    glm::vec2 min;
    glm::vec2 max;
};

struct sCircleHitBoxComponent : sComponent
{
    sCircleHitBoxComponent() = default;
    sCircleHitBoxComponent(glm::vec2 center, float radius) : center(center), radius(radius) {}

    virtual sComponent* clone()
    {
        sCircleHitBoxComponent* component = new sCircleHitBoxComponent();

        component->center = this->center;
        component->radius = this->radius;

        return (component);
    }

    glm::vec2 center;
    float radius;
};

struct sGravityComponent : sComponent
{
    sGravityComponent() = default;
    sGravityComponent(glm::vec2 value) : value(value) {}

    virtual sComponent* clone()
    {
        sGravityComponent* component = new sGravityComponent();

        component->value = this->value;

        return (component);
    }

    glm::vec2 value;
};

enum class eEntityType
{
    CHARACTER = 0,
    OBJECT = 1,
    TILE_WALKABLE = 2,
    TILE_NOT_WALKABLE = 3,
    TILE_STAIRS_UP = 4,
    TILE_STAIRS_DOWN = 5
};

struct sTypeComponent: sComponent
{
    sTypeComponent() = default;
    sTypeComponent(eEntityType type): type(type) {}

    virtual sComponent* clone()
    {
        sTypeComponent* component = new sTypeComponent();

        component->type = this->type;

        return (component);
    }

    eEntityType type;
};

struct sAIComponent : sComponent
{
    sAIComponent() = default;

    virtual sComponent* clone()
    {
        sAIComponent* component = new sAIComponent();

        return (component);
    }
};
