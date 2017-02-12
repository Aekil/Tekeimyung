/*
** @Author : Simon AMBROISE
*/

#pragma once

#include <Engine/Core/ScriptFactory.hpp>

#include <Game/Scripts/Health.hpp>

class Enemy : public BaseScript, public Health
{
public:
    Enemy() {};
    ~Enemy() = default;

public:
    virtual void Start();
    virtual void Update(float dt);
    virtual void Death();
    bool TakeDamage(int damage) override final;

private:
    Entity* _healthBarEmpty;
    sTransformComponent* _healthBarEmptyTransform;
    Entity* _healthBar;
    sTransformComponent* _healthBarTransform;

    sTransformComponent* _transform;
    sRenderComponent* _render;
    sRenderComponent* _healthRender;
};

REGISTER_SCRIPT(Enemy);
