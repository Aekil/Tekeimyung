#pragma once

#include "Component.hh"
#include <string>


struct sRenderComponent: sComponent
{
    sRenderComponent(const std::string& texture): texture(texture) {}

    std::string texture;
};

struct sPositionComponent: sComponent
{
    sPositionComponent(float x, float y): x(x), y(y) {}

    float x;
    float y;
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