#pragma once

#include <vector>
#include <string>
#include <cstdint>
#include <unordered_map>

#include <Engine/Graphics/Model.hpp>

// unit used for geometries base size
# define SIZE_UNIT 10

#define GEOMETRY_TYPES(PROCESS)\
    PROCESS(MESH),\
    PROCESS(PLANE),\
    PROCESS(BOX),\
    PROCESS(SPHERE),\
    PROCESS(CIRCLE)\

#define GENERATE_GEOMETRY_ENUM(ENUM) ENUM
#define GENERATE_GEOMETRY_STRING(STRING) #STRING
#define GENERATE_GEOMETRY_FROM_STRING_PAIRS(GEOMETRY) { #GEOMETRY, Geometry::eType::GEOMETRY }
#define GENERATE_GEOMETRY_FROM_ENUM_PAIRS(GEOMETRY) { Geometry::eType::GEOMETRY, #GEOMETRY }

class Geometry: public Model {
public:
    enum class eType: uint8_t
    {
        GEOMETRY_TYPES(GENERATE_GEOMETRY_ENUM)
    };

public:
    Geometry(eType type);
    ~Geometry();

    void                setMaterial(const Material& material);
    Material&           getMaterial() const;
    eType               getType() const;

    static Geometry::eType getGeometryType(const std::string& typeString);
    static const std::string& getGeometryTypeString(Geometry::eType type);
    static const std::vector<const char*>& getTypesString();

private:
    eType&              _type;

    static std::unordered_map<std::string, Geometry::eType> _typesFromString;
    static std::unordered_map<Geometry::eType, std::string> _typesFromEnum;
    static std::vector<const char*>                         _typesString;
};
