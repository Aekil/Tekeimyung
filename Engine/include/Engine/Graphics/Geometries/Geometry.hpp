/**
* @Author   Guillaume Labey
*/

#pragma once

#include <vector>
#include <string>
#include <cstdint>
#include <unordered_map>

#include <Engine/Graphics/Model.hpp>
#include <Engine/Utils/Helper.hpp>

// unit used for geometries base size
# define SIZE_UNIT 10

#define GEOMETRY_TYPES(PROCESS)\
    PROCESS(MESH)\
    PROCESS(PLANE)\
    PROCESS(BOX)\
    PROCESS(SPHERE)\
    PROCESS(CIRCLE)\

class Geometry: public Model {
public:
    REGISTER_ENUM(eType, uint8_t, GEOMETRY_TYPES)

public:
    Geometry(eType type);
    ~Geometry();

    void                setMaterial(const Material& material);
    Material&           getMaterial() const;
    eType               getType() const;

private:
    eType&              _type;
};

REGISTER_ENUM_MANAGER(Geometry::eType, uint8_t, GEOMETRY_TYPES)
