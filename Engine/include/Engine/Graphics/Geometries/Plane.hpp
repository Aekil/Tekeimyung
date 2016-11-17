#pragma once

#include <cstdint>

#include <Engine/Graphics/Model.hpp>

class Plane: public Model {
public:
    Plane();
    Plane(uint32_t width, uint32_t height);
    ~Plane();
};
