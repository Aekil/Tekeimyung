/**
* @Author   Simon Ambroise
*/

#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include <ECS/Entity.hpp>

#include <Engine/Graphics/Ray.hpp>

class Collisions
{
public:
    Collisions();
    ~Collisions();

    static bool         sphereVSsphere(const glm::vec3& pos1, float radius1, const glm::vec3& pos2, float radius2);

    static bool         pointVSAABB(const glm::vec3& point, const glm::vec3& boxPosition, const glm::vec3& boxSize);
    static bool         pointVSAABB2D(const glm::vec2& point, const glm::vec2& boxPosition, const glm::vec2& boxSize);

    static bool         AABBVSAABB(const glm::vec3& boxPos1, const glm::vec3& boxSize1, const glm::vec3& boxPos2, const glm::vec3& boxSize2);
    static bool         sphereVSAABB(const glm::vec3& spherePosition, float sphereRadius, const glm::vec3& boxPosition, const glm::vec3& boxSize);

    static float        rayVSAABB(const Ray& ray, const glm::vec3& boxMin, const glm::vec3& boxMax);

    static bool         isColliding(Entity*, Entity*);
    static bool         isCollidingSphereAndEntity(glm::vec3, float, Entity*);
private:

};

double      SquaredDistPointAABB(const glm::vec3& p, const glm::vec3& min, const glm::vec3& max);
