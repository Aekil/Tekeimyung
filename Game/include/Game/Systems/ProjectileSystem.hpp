#pragma once

#include <ECS/System.hpp>

#include <Engine/Utils/MonitoringDebugWindow.hpp>

#define MONITORING_NAME "Projectile system"

class               ProjectileSystem : public System
{
public:
    explicit        ProjectileSystem();
    virtual         ~ProjectileSystem();
    virtual void    update(EntityManager &em, float elapsedTime);

private:
    std::vector<Entity*>    _projectiles;
    uint16_t                _keyMonitoring;
};