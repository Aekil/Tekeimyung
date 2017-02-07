/*
** Author : Simon AMBROISE
*/

#pragma once

#include <Engine/Core/BaseScript.hpp>
#include <Engine/Core/ScriptFactory.hpp>

#include <Game/Scripts/Health.hpp>

class Tile
{
public:
    Tile();
    void SetBuildable(bool);
};

class Player : public BaseScript, public Health
{
public:
    Player() {};
    ~Player() {};

private:
    void Movement(float dt);
    void CheckBuildableZone();

    float buildableRadius;

public:
    virtual void Start();
    virtual void Update(float dt);

    virtual void OnHoverEnter();
    virtual void OnHoverExit();

    virtual void Death();

    virtual void OnCollisionEnter(Entity* entity);

private:
    glm::vec2 _direction;
};

REGISTER_SCRIPT(Player);
