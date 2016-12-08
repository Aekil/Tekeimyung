#pragma once

#include <ECS/System.hpp>
#include <ECS/EntityManager.hpp>

#include <Game/Map.hpp>

#include <Engine/Utils/MonitoringDebugWindow.hpp>

#define BUILD_SYSTEM_NAME "Build system"

START_SYSTEM(BuildSystem)
public:
    BuildSystem(EntityManager& em, Map* map);
    virtual ~BuildSystem();

    virtual bool                        init();
    virtual void                        update(EntityManager &em, float elapsedTime);

    void                                drawRange(EntityManager &em, const glm::vec4& color, const glm::ivec3& playerPos, uint32_t range);
    bool                                isInRange(const glm::ivec3& playerPos, const glm::ivec3& objPos, uint32_t range) const;
    void                                buildOnTile(const glm::ivec3& pos);

private:
    ADD_MONITORING_VAR

    Map*                                _map;
    EntityManager&                      _em;

    Entity*                             _tower;
END_SYSTEM(BuildSystem)
