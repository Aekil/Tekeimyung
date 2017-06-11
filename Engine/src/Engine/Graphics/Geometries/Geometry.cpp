/**
* @Author   Guillaume Labey
*/

#include <Engine/Debug/Debug.hpp>

#include <Engine/Graphics/Geometries/Geometry.hpp>

DECLARE_ENUM_MANAGER(Geometry::eType)

Geometry::Geometry() {}

Geometry::Geometry(Geometry::eType type): _type(type) {}

Geometry::~Geometry() {}

void    Geometry::setMaterial(Material* material)
{
    ASSERT(_meshs.size() == 1, "A geometry should have 1 mesh");

    _meshs[0]->setMaterial(material);
}

Material*   Geometry::getMaterial() const
{
    ASSERT(_meshs.size() == 1, "A geometry should have 1 mesh");

    return (_meshs[0]->getMaterial());
}

Geometry::eType Geometry::getType() const
{
    return (_type);
}

bool    Geometry::isGeometry() const
{
    return (true);
}
