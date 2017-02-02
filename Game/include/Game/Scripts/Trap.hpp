/*
** @Author : Simon AMBROISE
*/

#pragma once

#include <Engine/Core/ScriptFactory.hpp>

class Trap : public BaseScript
{
protected:
    int damage;

public:
    Trap() {};
    ~Trap() {};
public:
    virtual void Start() = 0;
    virtual void Update(float dt) = 0;

    virtual void OnCollisionEnter(Entity* entity);
};