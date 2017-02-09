/**
* @Author   Simon Ambroise
*/

#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

class Collisions
{
public:
    Collisions();
    ~Collisions();

    static bool         sphereVSsphere(const glm::vec3& pos1, float radius1, const glm::vec3& pos2, float radius2);

    static bool         pointVSAABB(const glm::vec3& point, const glm::vec3& boxPosition, const glm::vec3& boxSize);

    static bool         sphereVSAABB(const glm::vec3& spherePosition, float sphereRadius, const glm::vec3& boxPosition, const glm::vec3& boxSize);

    static float        rayVSAABB(const glm::vec3& rayPos, const glm::vec3& rayDir, const glm::vec3& boxMin, const glm::vec3& boxMax);
private:

};
