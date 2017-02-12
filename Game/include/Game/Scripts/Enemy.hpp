/*
** @Author : Simon AMBROISE
*/

#pragma once

#include <Engine/Core/ScriptFactory.hpp>

#include <Game/Scripts/Health.hpp>

class Enemy final : public BaseScript, public Health
{
public:
    Enemy() = default;
    ~Enemy() = default;

public:
    void start() override final;
    void update(float dt) override final;
    void death() override final;
    bool takeDamage(int damage) override final;

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
