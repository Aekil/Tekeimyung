/*
** @Author : Simon AMBROISE
*/

#pragma once

#include <Game/Trap/Trap.hpp>

class TrapNeedle final : public Trap
{
public:
    TrapNeedle() = default;
    ~TrapNeedle() = default;

    void start() override final;
    void update(float dt) override final;
};

REGISTER_SCRIPT(TrapNeedle);
