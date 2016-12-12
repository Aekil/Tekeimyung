/**
* @Author   Mathieu Chassara
*/

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
    
    ADD_MONITORING_VAR
END_SYSTEM(ProjectileSystem)
