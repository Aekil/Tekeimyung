/*
** @Author : Simon AMBROISE
*/

#pragma once

#include <Engine/Core/ScriptFactory.hpp>

class Spawner final : public BaseScript
{
public:
    Spawner() = default;
    ~Spawner() = default;

    void start() override final;
    void update(float dt) override final;
};

REGISTER_SCRIPT(Spawner);
