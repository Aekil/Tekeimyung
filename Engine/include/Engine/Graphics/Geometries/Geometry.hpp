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
    PROCESS(CONE)\

class Geometry: public Model {
public:
    REGISTER_ENUM(eType, uint8_t, GEOMETRY_TYPES)

public:
    Geometry();
    Geometry(eType type);
    ~Geometry();

    void                setMaterial(Material* material);
    Material*           getMaterial() const;
    eType               getType() const;

    bool                isGeometry() const override final;

    static Resource::eType      getResourceType() { return Resource::eType::GEOMETRY; }

    bool                        loadFromFile(const std::string &fileName) override final { return (nullptr); }

private:
    eType               _type;
};

REGISTER_ENUM_MANAGER(Geometry::eType, GEOMETRY_TYPES)
