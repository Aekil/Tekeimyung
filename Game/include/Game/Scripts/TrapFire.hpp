/*
** @Author : Simon AMBROISE
*/

#pragma once

#include <Game/Scripts/Trap.hpp>

class TrapFire : public Trap
{
public:
    TrapFire() {};
    ~TrapFire() {};
public:
    virtual void Start();
    virtual void Update(float dt);
};

REGISTER_SCRIPT(TrapFire);
