/**
* @Author   Guillaume Labey
*/

#pragma once

#include <Engine/Graphics/Geometries/Geometry.hpp>

class Cylinder: public Geometry {
public:
    struct sInfo
    {
        float radius;
        float height;
    };

public:
    Cylinder(Cylinder::sInfo& info);
    ~Cylinder();
};
