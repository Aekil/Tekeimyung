#include <Engine/Utils/Debug.hpp>

#include <Engine/Graphics/Geometries/Geometry.hpp>


std::unordered_map<std::string, Geometry::eType>  Geometry::_typesFromString = { GEOMETRY_TYPES(GENERATE_GEOMETRY_FROM_STRING_PAIRS) };
std::unordered_map<Geometry::eType, std::string>  Geometry::_typesFromEnum = { GEOMETRY_TYPES(GENERATE_GEOMETRY_FROM_ENUM_PAIRS) };
std::vector<const char*>  Geometry::_typesString = { GEOMETRY_TYPES(GENERATE_GEOMETRY_STRING) };

Geometry::Geometry(Geometry::eType type): _type(type) {}

Geometry::~Geometry() {}

void    Geometry::setMaterial(const Material& material)
{
    ASSERT(_meshs.size() == 1, "A geometry should have 1 mesh");

    _meshs[0]->material = material;
}

Material&   Geometry::getMaterial() const
{
    ASSERT(_meshs.size() == 1, "A geometry should have 1 mesh");

    return (_meshs[0]->material);
}

Geometry::eType   Geometry::getType() const
{
    return (_type);
}

Geometry::eType Geometry::getGeometryType(const std::string& typeString)
{
    ASSERT(_typesFromString.find(typeString) != _typesFromString.end(), "The type \"%s\" should exist", typeString);

    return (_typesFromString[typeString]);
}

const std::string& Geometry::getGeometryTypeString(Geometry::eType type)
{
    ASSERT(_typesFromEnum.find(type) != _typesFromEnum.end(), "The type should exist");

    return (_typesFromEnum[type]);
}

const std::vector<const char*>& Geometry::getTypesString()
{
    return (_typesString);
}
