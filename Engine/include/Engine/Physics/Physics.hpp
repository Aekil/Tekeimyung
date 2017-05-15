/**
* @Author   Simon Ambroise
*/

#pragma once

#include <glm/vec3.hpp>

#include <ECS/Entity.hpp>
#include <Engine/Graphics/Ray.hpp>

class Physics
{
public:
    Physics();
    ~Physics();

    static bool raycast(const Ray& ray,
                        Entity** hitEntity,
                        const std::vector<Entity*>& entitiesFilter = {},
                        float range = -1.0f);
    static bool raycastAll(const Ray& ray,
                            std::vector<Entity*>& hitEntities,
                            const std::vector<Entity*>& entitiesFilter = {},
                            float range = -1.0f);

    static bool raycastPlane(const Ray& ray, const glm::vec3& planeNormal, const glm::vec3& planePos, float& hitDistance);

    static float    distance(const glm::vec3& origin, const glm::vec3& position);
private:

};
