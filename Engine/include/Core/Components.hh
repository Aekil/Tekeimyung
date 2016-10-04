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
    sPositionComponent(float x, float y, float z): x(x), y(y), z(z) {}

    float x;
    float y;

    // Layer number
    float z;
};

struct sInputComponent: sComponent {
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
    sDirectionComponent(float x, float y) : x(x), y(y), orientation(eOrientation::N) {}

    float x;
    float y;
    eOrientation orientation;
};

struct sHitBoxComponent : sComponent
{
    sHitBoxComponent(glm::vec2 min, glm::vec2 max) : min(min), max(max) {}

    glm::vec2 min;
    glm::vec2 max;
};

struct sGravityComponent : sComponent
{
    sGravityComponent(glm::vec2 value) : value(value) {}

    glm::vec2 value;
};