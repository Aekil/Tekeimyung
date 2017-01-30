/*
** @Author : Simon AMBROISE
*/

#pragma once

#include <Game/Scripts/Trap.hpp>

class TrapNeedle : public Trap
{
public:
    TrapNeedle() {};
    ~TrapNeedle() {};
public:
    virtual void Start();
    virtual void Update(float dt);
};

REGISTER_SCRIPT(TrapNeedle);
