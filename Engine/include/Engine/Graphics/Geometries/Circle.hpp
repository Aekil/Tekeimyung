/**
* @Author   Guillaume Labey
*/

#pragma once

#include <Engine/Graphics/Geometries/Geometry.hpp>

class Circle: public Geometry {
public:
    struct sInfo
    {
        float innerRadius;
        float outerRadius;
    };

public:
    Circle(Circle::sInfo& info);
    ~Circle();
};
