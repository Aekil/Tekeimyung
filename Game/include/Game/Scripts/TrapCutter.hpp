/*
** @Author : Simon AMBROISE
*/

#pragma once

#include <Game/Scripts/Trap.hpp>

class TrapCutter final : public Trap
{
public:
    TrapCutter() = default;
    ~TrapCutter() = default;

    void start() override final;
    void update(float dt) override final;
};

REGISTER_SCRIPT(TrapCutter);
