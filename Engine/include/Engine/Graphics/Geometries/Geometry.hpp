#pragma once

#include <string>
#include <cstdint>
#include <memory>

#include <Engine/Graphics/Model.hpp>

class Geometry: public Model {
public:
    Geometry();
    ~Geometry();

    void                setMaterial(const Material& material);
    Material&           getMaterial() const;
};
