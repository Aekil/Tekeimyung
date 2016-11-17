#pragma once

#include <Engine/Graphics/Model.hpp>

class Plane: public Model {
public:
    Plane();
    Plane(float width, float height);
    ~Plane();
};
