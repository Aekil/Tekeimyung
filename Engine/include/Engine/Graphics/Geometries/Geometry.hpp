#pragma once

#include <string>
#include <cstdint>
#include <memory>

#include <Engine/Graphics/Model.hpp>

class Geometry: public Model {
public:
    enum class eType: uint8_t
    {
        PLANE = 0,
        CUBE = 1
    };

public:
    Geometry(eType type);
    ~Geometry();

    void                setMaterial(const Material& material);
    Material&           getMaterial() const;
    eType               getType() const;

private:
    eType&              _type;
};
