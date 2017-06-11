/**
* @Author   Guillaume Labey
*/

#include <glm/glm.hpp>

#include <Engine/Graphics/Ray.hpp>

Ray::Ray() {}

Ray::Ray(const glm::vec3& origin, const glm::vec3& direction): origin(origin), direction(glm::normalize(direction)) {}

Ray::~Ray() {}

glm::vec3   Ray::getPoint(float distance)
{
    return origin + (direction * distance);
}
