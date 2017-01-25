/*
** @Author : Simon AMBROISE
*/

#pragma once

#include <Engine/Core/ScriptFactory.hpp>

class Spawner : public BaseScript
{
public:
    Spawner() {};
    ~Spawner() {};
public:
    virtual void Start();
    virtual void Update(float dt);
};

REGISTER_SCRIPT(Spawner, "Spawner");
