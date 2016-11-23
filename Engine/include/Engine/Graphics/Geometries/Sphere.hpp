#pragma once

#include <Engine/Graphics/Geometries/Geometry.hpp>

class Sphere: public Geometry {
public:
    struct sInfo
    {
        float radius;
    };

public:
    Sphere(Sphere::sInfo& info);
    ~Sphere();
};
