#pragma once

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
    : texture(texture), type(type), animated(animated), nbFrames(nbFrames), orientations(orientations), spriteSize(spriteSize) {}

    std::string texture;
    Sprite::eType type;
    bool animated;
    uint32_t nbFrames;

    // Sprite width and height
    glm::vec2 spriteSize;

    std::vector<eOrientation> orientations;
};

struct sPositionComponent: sComponent
{
    sPositionComponent() = default;
    sPositionComponent(glm::vec2 value, float z): value(value), z(z) {}

    glm::vec2 value;

    // Layer number
    float z;
};

struct sInputComponent: sComponent {
    sInputComponent() = default;
    sInputComponent(Keyboard::eKey moveLeft,
                    Keyboard::eKey moveRight,
                    Keyboard::eKey moveUp,
                    Keyboard::eKey moveDown): moveLeft(moveLeft), moveRight(moveRight), moveUp(moveUp), moveDown(moveDown), keyPressed(false) {}

    Keyboard::eKey          moveLeft;
    Keyboard::eKey          moveRight;
    Keyboard::eKey          moveUp;
    Keyboard::eKey          moveDown;
    bool                    keyPressed;
};

struct sDirectionComponent : sComponent
{
    sDirectionComponent() = default;
    sDirectionComponent(glm::vec2) : value(value), orientation(eOrientation::N) {}

    glm::vec2 value;
    eOrientation orientation;
};

struct sHitBoxComponent : sComponent
{
    sHitBoxComponent() = default;
    sHitBoxComponent(glm::vec2 min, glm::vec2 max) : min(min), max(max) {}

    glm::vec2 min;
    glm::vec2 max;
};

struct sCircleHitBoxComponent : sComponent
{
    sCircleHitBoxComponent() = default;
    sCircleHitBoxComponent(glm::vec2 center, float radius) : center(center), radius(radius) {}

    glm::vec2 center;
    float radius;
};

struct sGravityComponent : sComponent
{
    sGravityComponent() = default;
    sGravityComponent(glm::vec2 value) : value(value) {}

    glm::vec2 value;
};

enum class eEntityType
{
    CHARACTER = 0,
    OBJECT = 1,
    TILE = 2,
    TILE_STAIRS_UP = 3,
    TILE_STAIRS_DOWN = 4
};

struct sTypeComponent: sComponent
{
    sTypeComponent() = default;
    sTypeComponent(eEntityType type): type(type) {}

    eEntityType type;
};
