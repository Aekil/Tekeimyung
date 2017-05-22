/*
** @Author : Simon AMBROISE
*/

#pragma once

#include <Engine/Core/ScriptFactory.hpp>

class Trap : public BaseScript
{
public:
    Trap() = default;
    ~Trap() = default;

    virtual void start() = 0;
    virtual void update(float dt) = 0;

    void onCollisionEnter(Entity* entity) override final;

protected:
    int damage;
    int usage{5};
};
