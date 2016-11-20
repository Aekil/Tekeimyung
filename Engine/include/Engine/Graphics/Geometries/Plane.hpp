#pragma once

#include <string>

#include <Engine/Graphics/Geometries/Geometry.hpp>

class Plane: public Geometry {
public:
    Plane();
    Plane(float width, float height);
    Plane(float width, float height, const std::string& texturePath);
    ~Plane();
};
