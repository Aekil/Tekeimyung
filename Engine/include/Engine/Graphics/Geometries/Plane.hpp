#pragma once

#include <string>

#include <Engine/Graphics/Model.hpp>

class Plane: public Model {
public:
    Plane();
    Plane(float width, float height);
    Plane(float width, float height, const std::string& texturePath);
    ~Plane();

    void                setMaterial(const Material& material);
    Material&           getMaterial() const;
};
