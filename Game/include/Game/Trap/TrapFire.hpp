/*
** @Author : Simon AMBROISE
*/

#pragma once

#include <Game/Trap/Trap.hpp>

class TrapFire final : public Trap
{
public:
    TrapFire() = default;
    ~TrapFire() = default;

    void start() override final;
    void update(float dt) override final;
};
