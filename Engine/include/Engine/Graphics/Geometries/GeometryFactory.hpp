#pragma once

#include <memory>

#include <Engine/Graphics/Geometries/Geometry.hpp>

class GeometryFactory
{
public:
    GeometryFactory();
    ~GeometryFactory();

    static std::shared_ptr<Geometry>   create(Geometry::eType& type, const std::string& texture);
};
