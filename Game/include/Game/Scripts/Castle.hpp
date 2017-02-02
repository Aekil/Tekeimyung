#pragma once

#include <Engine/Core/BaseScript.hpp>
#include <Engine/Core/ScriptFactory.hpp>

#include <Game/Scripts/Health.hpp>

class Castle : public BaseScript, public Health
{
public:
    Castle() {};
    ~Castle() {};

public:
    virtual void Start();
    virtual void Update(float dt);
    virtual void OnCollisionEnter(Entity* entity);
    virtual void Death();
};

REGISTER_SCRIPT(Castle);
