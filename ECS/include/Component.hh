#pragma once

#include <typeinfo>

struct sComponent
{
    virtual const std::type_info& getTypeInfo()
    {
        return (typeid(*this));
    }
};

struct sPositionComponent : sComponent
{
    sPositionComponent(float x, float y) : x(x), y(y) {}

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