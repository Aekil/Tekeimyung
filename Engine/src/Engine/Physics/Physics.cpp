/**
* @Author   Simon Ambroise
*/

#include <Engine/Components.hh>
#include <Engine/EntityFactory.hpp>
#include <Engine/Physics/Collisions.hpp>
#include <Engine/Physics/Physics.hpp>

Physics::Physics()
{}

Physics::~Physics()
{}

bool    Physics::raycast(const Ray& ray, Entity** hitEntity)
{
    float nearestHitDist = 0.0f;
    Entity* nearestEntity = nullptr;

    EntityManager* em = EntityFactory::getBindedEntityManager();
    for (auto it : em->getEntities())
    {
        Entity* entity = it.second;

        sRenderComponent* render = entity->getComponent<sRenderComponent>();
        sTransformComponent* transform = entity->getComponent<sTransformComponent>();

        // We can't select entity that is not displayed or has model not initialized
        if (!render || !render->_model || render->ignoreRaycast)
            continue;

        // Model box collider position
        glm::vec3 boxMin = glm::vec3(render->_model->getMin().x, render->_model->getMin().y, render->_model->getMin().z);
        glm::vec3 boxMax = glm::vec3(render->_model->getMax().x, render->_model->getMax().y, render->_model->getMax().z);

        boxMin *= transform->scale;
        boxMax *= transform->scale;

        // Convert box collider to world position
        boxMin += transform->pos;
        boxMax += transform->pos;

        float distance = Collisions::rayVSAABB(ray, boxMin, boxMax);
        if (distance != 0 && (nearestEntity == nullptr || distance <= nearestHitDist))
        {
            nearestHitDist = distance;
            nearestEntity = entity;
        }
    }

    *hitEntity = nearestEntity;
    return (nearestEntity != nullptr);
}

bool    Physics::raycastAll(const Ray& ray, std::vector<Entity*> hitEntities)
{
    EntityManager* em = EntityFactory::getBindedEntityManager();
    for (auto it : em->getEntities())
    {
        Entity* entity = it.second;

        sRenderComponent* render = entity->getComponent<sRenderComponent>();
        sTransformComponent* transform = entity->getComponent<sTransformComponent>();

        // We can't select entity that is not displayed or has model not initialized
        if (!render || !render->_model || render->ignoreRaycast)
            continue;

        // Model box collider position
        glm::vec3 boxMin = glm::vec3(render->_model->getMin().x, render->_model->getMin().y, render->_model->getMin().z);
        glm::vec3 boxMax = glm::vec3(render->_model->getMax().x, render->_model->getMax().y, render->_model->getMax().z);

        boxMin *= transform->scale;
        boxMax *= transform->scale;

        // Convert box collider to world position
        boxMin += transform->pos;
        boxMax += transform->pos;

        float distance = Collisions::rayVSAABB(ray, boxMin, boxMax);
        if (distance != 0)
        {
            hitEntities.push_back(entity);
        }
    }

    return (hitEntities.size() > 0);
}

bool    Physics::raycastPlane(const Ray& ray, const glm::vec3& planeNormal, const glm::vec3& planePos, float& hitDistance)
{
    float denom = glm::dot(planeNormal, ray.direction);
    if (abs(denom) > 0.0001f)
    {
        glm::vec3 point = planePos - ray.origin;
        hitDistance = glm::dot(point, planeNormal) / denom;
        return hitDistance >= 0;
    }
    return false;
}
