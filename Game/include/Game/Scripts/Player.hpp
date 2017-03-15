/*
** Author : Simon AMBROISE
*/

#pragma once

#include <glm/vec3.hpp>

#include <Engine/Utils/EventSound.hpp>
#include <Engine/Core/BaseScript.hpp>
#include <Engine/Core/ScriptFactory.hpp>

#include <Game/Scripts/Health.hpp>

class Player final : public BaseScript, public Health
{
public:
    Player() = default;
    ~Player() = default;

private:
    void updateDirection();
    void checkBuildableZone();
    void movement(float elapsedTime);
    void handleShoot();

    float buildableRadius;

public:
    void start() override final;
    void update(float dt) override final;

    virtual void onHoverEnter();
    virtual void onHoverExit();

    void death() override final;

    void onCollisionEnter(Entity* entity) override final;
    void onCollisionExit(Entity* entity) override final;

private:
    glm::vec3 _direction;
    sTransformComponent* _transform;
    sRenderComponent* _render;
    bool _buildEnabled;
    int _damage;
    tEventSound* _shootSound = nullptr;
};

REGISTER_SCRIPT(Player);
