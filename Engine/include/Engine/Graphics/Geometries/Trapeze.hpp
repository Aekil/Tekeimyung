/**
* @Author   Guillaume Labey
*/

#pragma once

#include <Engine/Graphics/Geometries/Geometry.hpp>

class Trapeze: public Geometry {
public:
    struct sInfo
    {
        float height;

        struct sTop
        {
            float width;
            float length;
        } top;

        struct sBottom
        {
            float width;
            float length;
        } bottom;
    };

public:
    Trapeze(Trapeze::sInfo& info);
    ~Trapeze();
};
