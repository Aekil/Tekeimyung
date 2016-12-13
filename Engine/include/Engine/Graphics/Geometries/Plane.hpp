/**
* @Author   Guillaume Labey
*/

#pragma once

#include <string>

#include <Engine/Graphics/Geometries/Geometry.hpp>

class Plane: public Geometry {
public:
    struct sInfo
    {
        float width;
        float height;
        std::string texturePath;
    };

public:
    Plane(Plane::sInfo& info);
    ~Plane();
};
