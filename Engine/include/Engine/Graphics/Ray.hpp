/**
* @Author   Guillaume Labey
*/

#pragma once

#include <glm/vec3.hpp>

class Ray
{
public:
    Ray();
    Ray(const glm::vec3& origin, const glm::vec3& direction);
    ~Ray();

    glm::vec3   getPoint(float distance);

public:
    glm::vec3   origin;
    glm::vec3   direction;
};
