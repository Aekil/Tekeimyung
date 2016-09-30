#pragma once

#include "Component.hh"
#include <string>


enum class eSpriteType
{
    TILE,
    OBJECT
};

struct sRenderComponent: sComponent
{
    sRenderComponent(eSpriteType type, const std::string& texture): texture(texture), type(type) {}

    std::string texture;
    eSpriteType type;
};

struct sPositionComponent: sComponent
{
    sPositionComponent(float x, float y, float z): x(x), y(y), z(z) {}

    float x;
    float y;

    // Layer number
    float z;
};

struct sDirectionComponent : sComponent
{
    sDirectionComponent(float x, float y) : x(x), y(y) {}

    float x;
    float y;
};

struct sHitBoxComponent : sComponent
{
    sHitBoxComponent(float x, float y, float width, float height) : x(x), y(y), width(width), height(height) {}

    float x;
    float y;
    float width;
    float height;
};