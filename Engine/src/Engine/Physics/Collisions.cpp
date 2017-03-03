/**
* @Author   Simon Ambroise
*/

#include <limits>
#include <cmath>
#include <algorithm>

#include <Engine/Physics/Collisions.hpp>

Collisions::Collisions()
{}

Collisions::~Collisions()
{}

bool    Collisions::sphereVSsphere(const glm::vec3& pos1, float radius1, const glm::vec3& pos2, float radius2)
{
    float distance = pow((pos1.x - pos2.x), 2) + pow((pos1.y - pos2.y), 2) + pow((pos1.z - pos2.z), 2);

    float sumRadius = pow(radius2 + radius1, 2);
    return (distance <= sumRadius);
}

bool    Collisions::pointVSAABB(const glm::vec3& point, const glm::vec3& boxPosition, const glm::vec3& boxSize)
{
    return  (point.x >= boxPosition.x && point.x <= boxPosition.x + boxSize.x) &&
        (point.y >= boxPosition.y && point.y <= boxPosition.y + boxSize.y) &&
        (point.z >= boxPosition.z && point.z <= boxPosition.z + boxSize.z);
}

bool    Collisions::pointVSAABB2D(const glm::vec2& point, const glm::vec2& boxPosition, const glm::vec2& boxSize)
{
    return  (point.x >= boxPosition.x && point.x <= boxPosition.x + boxSize.x) &&
        (point.y >= boxPosition.y && point.y <= boxPosition.y + boxSize.y);
}

double SquaredDistPointAABB(const glm::vec3& p, const glm::vec3& min, const glm::vec3& max)
{
    auto check = [&](
        const double pn,
        const double bmin,
        const double bmax) -> double
    {
        double out = 0;
        double v = pn;

        if (v < bmin)
        {
            double val = (bmin - v);
            out += val * val;
        }

        if (v > bmax)
        {
            double val = (v - bmax);
            out += val * val;
        }

        return out;
    };

    double sq = 0.0;

    sq += check(p.x, min.x, max.x);
    sq += check(p.y, min.y, max.y);
    sq += check(p.z, min.z, max.z);

    return sq;
}

bool    Collisions::sphereVSAABB(const glm::vec3& spherePosition, float sphereRadius, const glm::vec3& boxPosition, const glm::vec3& boxSize)
{
    double squaredDistance = SquaredDistPointAABB(spherePosition, boxPosition, boxPosition + boxSize);

    return squaredDistance <= (sphereRadius * sphereRadius);
}

float Collisions::rayVSAABB(const Ray& ray, const glm::vec3& boxMin, const glm::vec3& boxMax)
{
    float tNear = FLT_MIN;
    float tFar = FLT_MAX;

    // X planes
    {
        if (ray.direction.x == 0) // Ray parallel to the plane
        {
            if (ray.origin.x < boxMin.x || ray.origin.x > boxMax.x) // Ray not beteween the two planes
            {
                return (0);
            }
        }
        float t1 = (boxMin.x - ray.origin.x) / ray.direction.x;
        float t2 = (boxMax.x - ray.origin.x) / ray.direction.x;
        if (t1 > t2)
        {
            float tmp = t1;
            t1 = t2;
            t2 = tmp;
        }
        if (t1 > tNear)
            tNear = t1;
        if (t2 < tFar)
            tFar = t2;
        if (tNear > tFar)
            return (0);
        if (tFar < 0)
            return (0);

    }
    // Y planes
    {
        if (ray.direction.y == 0) // Ray parallel to the plane
        {
            if (ray.origin.y < boxMin.y || ray.origin.y > boxMax.y) // Ray not beteween the two planes
            {
                return (0);
            }
        }
        float t1 = (boxMin.y - ray.origin.y) / ray.direction.y;
        float t2 = (boxMax.y - ray.origin.y) / ray.direction.y;
        if (t1 > t2)
        {
            float tmp = t1;
            t1 = t2;
            t2 = tmp;
        }
        if (t1 > tNear)
            tNear = t1;
        if (t2 < tFar)
            tFar = t2;
        if (tNear > tFar)
            return (0);
        if (tFar < 0)
            return (0);

    }
    // Z planes
    {
        if (ray.direction.z == 0) // Ray parallel to the plane
        {
            if (ray.origin.z < boxMin.z || ray.origin.z > boxMax.z) // Ray not beteween the two planes
            {
                return (0);
            }
        }
        float t1 = (boxMin.z - ray.origin.z) / ray.direction.z;
        float t2 = (boxMax.z - ray.origin.z) / ray.direction.z;
        if (t1 > t2)
        {
            float tmp = t1;
            t1 = t2;
            t2 = tmp;
        }
        if (t1 > tNear)
            tNear = t1;
        if (t2 < tFar)
            tFar = t2;
        if (tNear > tFar)
            return (0);
        if (tFar < 0)
            return (0);

    }
    return (tNear);
}
