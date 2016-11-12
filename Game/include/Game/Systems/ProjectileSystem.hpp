#pragma once

#include <ECS/System.hpp>

class               ProjectileSystem : public System
{
public:
    explicit        ProjectileSystem();
    virtual         ~ProjectileSystem();
    virtual void    update(EntityManager &em, float elapsedTime);
private:
    std::vector<Entity*>    _projectiles;
};