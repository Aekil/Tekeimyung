#pragma once

#include <ECS/System.hpp>

#include <Engine/Utils/MonitoringDebugWindow.hpp>

#define PROJECTILE_SYSTEM_NAME "Projectile system"

START_SYSTEM(ProjectileSystem)
public:
    explicit        ProjectileSystem();
    virtual         ~ProjectileSystem();
    virtual void    update(EntityManager &em, float elapsedTime);

private:
    std::vector<Entity*>    _projectiles;
    uint16_t                _keyMonitoring;
    tMonitoring             _data;
END_SYSTEM(ProjectileSystem)
