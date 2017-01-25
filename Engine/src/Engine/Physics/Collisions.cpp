/**
* @Author   Simon Ambroise
*/

#include <cmath>
#include <algorithm>

#include <Engine/Physics/Collisions.hpp>

Collisions::Collisions()
{}

Collisions::~Collisions()
{}

bool    Collisions::sphereVSsphere(glm::vec3 pos1, float radius1, glm::vec3 pos2, float radius2)
{
    float distance = pow((pos1.x - pos2.x), 2) + pow((pos1.y - pos2.y), 2) + pow((pos1.z - pos2.z), 2);

    float sumRadius = pow(radius2 + radius1, 2);
    return (distance <= sumRadius);
}

bool    Collisions::pointVSAABB(glm::vec3 point, glm::vec3 boxPosition, glm::vec3 boxSize)
{
    return  (point.x >= boxPosition.x && point.x <= boxPosition.x + boxSize.x) &&
        (point.y >= boxPosition.y && point.y <= boxPosition.y + boxSize.y) &&
        (point.z >= boxPosition.z && point.z <= boxPosition.z + boxSize.z);
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

bool    Collisions::sphereVSAABB(glm::vec3 spherePosition, float sphereRadius, glm::vec3 boxPosition, glm::vec3 boxSize)
{
    double squaredDistance = SquaredDistPointAABB(spherePosition, boxPosition, boxPosition + boxSize);

    return squaredDistance <= (sphereRadius * sphereRadius);
}
