/**
* @Author   Guillaume Labey
*/

#pragma once

#include <Engine/Graphics/Geometries/Geometry.hpp>

class Box: public Geometry {
public:
    struct sInfo
    {
        float width;
        float height;
        float length;
    };

public:
    Box(Box::sInfo& info);
    ~Box();
};
