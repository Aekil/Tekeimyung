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

bool    Collisions::sphereVSAABB(glm::vec3 spherePosition, float sphereRadius, glm::vec3 boxPosition, glm::vec3 boxSize)
{
    float x = std::max(boxPosition.x, std::min(spherePosition.x, boxPosition.x + boxSize.x));
    float y = std::max(boxPosition.y, std::min(spherePosition.y, boxPosition.y + boxSize.y));
    float z = std::max(boxPosition.z, std::min(spherePosition.z, boxPosition.z + boxSize.z));

    float distance = sqrt(  (x - spherePosition.x) * (x - spherePosition.x) +
                            (y - spherePosition.y) * (y - spherePosition.y) +
                            (z - spherePosition.z) * (z - spherePosition.z));

    return (distance < sphereRadius);
}