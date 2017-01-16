/*
** @Author : Simon AMBROISE
*/

#pragma once

#include <Game/Scripts/Trap.hpp>

class TrapCutter : public Trap
{
public:
    TrapCutter() {};
    ~TrapCutter() {};
public:
    virtual void Start();
    virtual void Update(float dt);
};

REGISTER_SCRIPT(TrapCutter, "TrapCutter");