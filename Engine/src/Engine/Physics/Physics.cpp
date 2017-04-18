/**
* @Author   Simon Ambroise
*/

#include <Engine/Components.hh>
#include <Engine/EntityFactory.hpp>
#include <Engine/Physics/Collisions.hpp>
#include <Engine/Physics/Physics.hpp>

Physics::Physics() {}

Physics::~Physics() {}

bool        Physics::raycast(const Ray& ray, Entity** hitEntity, std::vector<Entity*> entitiesFilter, bool useRange)
{
    float                   nearestHitDist = 0.0f;
    Entity*                 nearestEntity = nullptr;
    EntityManager*          em = EntityFactory::getBindedEntityManager();
    std::vector<Entity*>    entities = em->getEntities();

    for (Entity* entity : entities)
    {
        //  If the entity doesn't belong to the entitiesFilter passed as a parameter !
        if (std::find(entitiesFilter.begin(), entitiesFilter.end(), entity) == entitiesFilter.end())
        {
            sRenderComponent*       render = entity->getComponent<sRenderComponent>();
            sTransformComponent*    transform = entity->getComponent<sTransformComponent>();

            // We can't select entity that is not displayed or has model not initialized
            if (!render || !render->getModel() || render->ignoreRaycast)
                continue;

            // Model box collider position
            glm::vec3 boxMin = glm::vec3(render->getModel()->getMin().x, render->getModel()->getMin().y, render->getModel()->getMin().z);
            glm::vec3 boxMax = glm::vec3(render->getModel()->getMax().x, render->getModel()->getMax().y, render->getModel()->getMax().z);

            boxMin *= transform->getScale();
            boxMax *= transform->getScale();

            // Convert box collider to world position
            boxMin += transform->getPos();
            boxMax += transform->getPos();

            float   distance = Collisions::rayVSAABB(ray, boxMin, boxMax);
            if (distance != 0 && (nearestEntity == nullptr || distance <= nearestHitDist))
            {
                nearestHitDist = distance;
                nearestEntity = entity;
            }
        }
    }

    *hitEntity = nearestEntity;
    return (nearestEntity != nullptr);
}

bool    Physics::raycast(const Ray& ray, Entity** hitEntity)
{
    float nearestHitDist = 0.0f;
    Entity* nearestEntity = nullptr;

    EntityManager* em = EntityFactory::getBindedEntityManager();
    for (Entity* entity: em->getEntities())
    {
        sRenderComponent* render = entity->getComponent<sRenderComponent>();
        sTransformComponent* transform = entity->getComponent<sTransformComponent>();

        // We can't select entity that is not displayed or has model not initialized
        if (!render || !render->getModel() || render->ignoreRaycast)
            continue;

        // Model box collider position
        glm::vec3 boxMin = glm::vec3(render->getModel()->getMin().x, render->getModel()->getMin().y, render->getModel()->getMin().z);
        glm::vec3 boxMax = glm::vec3(render->getModel()->getMax().x, render->getModel()->getMax().y, render->getModel()->getMax().z);

        boxMin *= transform->getScale();
        boxMax *= transform->getScale();

        // Convert box collider to world position
        boxMin += transform->getPos();
        boxMax += transform->getPos();
            
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

bool    Physics::raycastAll(const Ray& ray, std::vector<Entity*>& hitEntities, std::vector<Entity*> entitiesFilter, bool useRange)
{
    EntityManager* em = EntityFactory::getBindedEntityManager();
    for (Entity* entity : em->getEntities())
    {
        //  If the entity doesn't belong to the entitiesFilter passed as a parameter !
        if (std::find(entitiesFilter.begin(), entitiesFilter.end(), entity) == entitiesFilter.end())
        {
            sRenderComponent* render = entity->getComponent<sRenderComponent>();
            sTransformComponent* transform = entity->getComponent<sTransformComponent>();

            // We can't select entity that is not displayed or has model not initialized
            if (!render || !render->getModel() || render->ignoreRaycast)
                continue;

            // Model box collider position
            glm::vec3 boxMin = glm::vec3(render->getModel()->getMin().x, render->getModel()->getMin().y, render->getModel()->getMin().z);
            glm::vec3 boxMax = glm::vec3(render->getModel()->getMax().x, render->getModel()->getMax().y, render->getModel()->getMax().z);

            boxMin *= transform->getScale();
            boxMax *= transform->getScale();

            // Convert box collider to world position
            boxMin += transform->getPos();
            boxMax += transform->getPos();

            float distance = Collisions::rayVSAABB(ray, boxMin, boxMax);
            if (distance != 0)
            {
                hitEntities.push_back(entity);
            }
        }
    }

    return (hitEntities.size() > 0);
}

bool    Physics::raycastAll(const Ray& ray, std::vector<Entity*>& hitEntities)
{
    EntityManager* em = EntityFactory::getBindedEntityManager();
    for (Entity* entity: em->getEntities())
    {
        sRenderComponent* render = entity->getComponent<sRenderComponent>();
        sTransformComponent* transform = entity->getComponent<sTransformComponent>();

        // We can't select entity that is not displayed or has model not initialized
        if (!render || !render->getModel() || render->ignoreRaycast)
            continue;

        // Model box collider position
        glm::vec3 boxMin = glm::vec3(render->getModel()->getMin().x, render->getModel()->getMin().y, render->getModel()->getMin().z);
        glm::vec3 boxMax = glm::vec3(render->getModel()->getMax().x, render->getModel()->getMax().y, render->getModel()->getMax().z);

        boxMin *= transform->getScale();
        boxMax *= transform->getScale();

        // Convert box collider to world position
        boxMin += transform->getPos();
        boxMax += transform->getPos();

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

float   Physics::distance(const glm::vec3& origin, const glm::vec3& position)
{
    return (glm::distance(origin, position));
}