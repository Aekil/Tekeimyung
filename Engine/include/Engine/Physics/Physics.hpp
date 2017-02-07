/**
* @Author   Simon Ambroise
*/

#pragma once

#include <glm/vec3.hpp>

#include <ECS/Entity.hpp>

class Physics
{
public:
    Physics();
    ~Physics();

    static bool raycast(const glm::vec3& rayOrigin, const glm::vec3& rayDir, Entity** hitEntity);
    static bool raycastAll(const glm::vec3& rayOrigin, const glm::vec3& rayDir, std::vector<Entity*> hitEntities);
private:

};
