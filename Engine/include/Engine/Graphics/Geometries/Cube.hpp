#pragma once

#include <Engine/Graphics/Geometries/Geometry.hpp>

class Cube: public Geometry {
public:
    Cube();
    Cube(float size);
    ~Cube();
};
