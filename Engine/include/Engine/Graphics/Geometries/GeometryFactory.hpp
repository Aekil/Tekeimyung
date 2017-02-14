/**
* @Author   Guillaume Labey
*/

#pragma once

#include <memory>

#include <Engine/Graphics/Geometries/Geometry.hpp>

class GeometryFactory
{
public:
    GeometryFactory();
    ~GeometryFactory();

    static void                         initGeometries();
    static Geometry*                    getGeometry(Geometry::eType type);
};
