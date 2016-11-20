#pragma once

#include <Engine/Graphics/Geometries/Geometry.hpp>

class Cube: public Geometry {
public:
    struct sInfo
    {
        float size;
    };

public:
    Cube(Cube::sInfo& info);
    ~Cube();
};
