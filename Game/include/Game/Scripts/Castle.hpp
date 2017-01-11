#pragma once

#include <Engine/Core/BaseScript.hpp>
#include <Engine/Core/ScriptFactory.hpp>

class Castle : public BaseScript
{
public:
    Castle() {};
    ~Castle() {};

public:
    virtual void Start();
    virtual void Update(float dt);
    virtual void OnCollisionEnter(Entity* entity);

private:
    int health = 500;
};

REGISTER_SCRIPT(Castle, "Castle");