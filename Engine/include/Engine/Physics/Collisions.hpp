#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

class Collisions
{
public:
    Collisions();
    ~Collisions();

    static bool         sphereVSsphere(glm::vec3 pos1, float radius1, glm::vec3 pos2, float radius2);

    static bool         pointVSAABB(glm::vec3 point, glm::vec3 boxPosition, glm::vec3 boxSize);

    static bool         sphereVSAABB(glm::vec3 spherePosition, float sphereRadius, glm::vec3 boxPosition, glm::vec3 boxSize);
private:

};