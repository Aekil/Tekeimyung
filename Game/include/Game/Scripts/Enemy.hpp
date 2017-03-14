/*
** @Author : Simon AMBROISE
*/

#pragma once

#include <glm/vec3.hpp>

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

    void setPath(const std::vector<glm::vec3>& path);

private:
    Entity* _healthBarEmpty;
    sTransformComponent* _healthBarEmptyTransform;
    Entity* _healthBar;
    sTransformComponent* _healthBarTransform;

    sTransformComponent* _transform;
    sRenderComponent* _render;
    sRenderComponent* _healthRender;

    std::vector<glm::vec3> _path;
    uint32_t _pathProgress{0};
};

REGISTER_SCRIPT(Enemy);
