/*
** @Author : Simon AMBROISE
*/

#pragma once

#include <Engine/Core/ScriptFactory.hpp>

class Enemy : public BaseScript
{
public:
    Enemy() {};
    ~Enemy() {};
public:
    virtual void Start();
    virtual void Update(float dt);
};

REGISTER_SCRIPT(Enemy);
