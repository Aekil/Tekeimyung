/**
* @Author   Guillaume Labey
*/

#pragma once

#include <Engine/Graphics/Geometries/Geometry.hpp>

class Cone: public Geometry {
public:
    struct sInfo
    {
        float bottomRadius;
        float height;
    };

public:
    Cone(Cone::sInfo& info);
    ~Cone();
};
