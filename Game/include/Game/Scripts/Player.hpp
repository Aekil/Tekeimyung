/*
** Author : Simon AMBROISE
*/

#pragma once

#include <glm/vec3.hpp>

#include <Engine/Core/BaseScript.hpp>
#include <Engine/Core/ScriptFactory.hpp>

#include <Game/Scripts/Health.hpp>

class Player : public BaseScript, public Health
{
public:
    Player() {};
    ~Player() {};

private:
    void updateDirection();
    void CheckBuildableZone();
    void Movement(float dt);
    void handleShoot();

    float buildableRadius;

public:
    virtual void Start();
    virtual void Update(float dt);

    virtual void OnHoverEnter();
    virtual void OnHoverExit();

    virtual void Death();

    virtual void OnCollisionEnter(Entity* entity);

private:
    glm::vec3 _direction;
    sTransformComponent* _transform;
    sRenderComponent* _render;
    bool _buildEnabled;
};

REGISTER_SCRIPT(Player);
