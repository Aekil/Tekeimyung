#pragma once

#include <ECS/System.hpp>

#include <Game/Map.hpp>

#include <Engine/Utils/MonitoringDebugWindow.hpp>

#define BUILD_SYSTEM_NAME "Build system"

START_SYSTEM(BuildSystem)
public:
    BuildSystem(Map* map);
    virtual ~BuildSystem();

    virtual bool                        init();
    virtual void                        update(EntityManager &em, float elapsedTime);

    void                                drawRange(EntityManager &em, const glm::vec4& color, const glm::ivec3& playerPos, uint32_t range);

private:
    ADD_MONITORING_VAR

    Map*                                _map;
END_SYSTEM(BuildSystem)
