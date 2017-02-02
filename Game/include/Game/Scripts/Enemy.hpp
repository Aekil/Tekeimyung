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
    ~Enemy() {};
public:
    virtual void Start();
    virtual void Update(float dt);
    virtual void Death();
};

REGISTER_SCRIPT(Enemy);
