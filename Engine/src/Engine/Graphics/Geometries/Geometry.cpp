#include <Engine/Utils/Debug.hpp>

#include <Engine/Graphics/Geometries/Geometry.hpp>


Geometry::Geometry() {}

Geometry::~Geometry() {}

void    Geometry::setMaterial(const Material& material)
{
    ASSERT(_meshs.size() == 1, "A geometry should have 1 mesh");

    _meshs[0]->material = material;
}

Material&   Geometry::getMaterial() const
{
    ASSERT(_meshs.size() == 1, "A geometry should have 1 mesh");

    return _meshs[0]->material;
}
