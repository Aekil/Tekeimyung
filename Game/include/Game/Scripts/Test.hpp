#pragma once

#include <Engine/Core/BaseScript.hpp>
#include <Engine/Core/ScriptFactory.hpp>

class Test final : public BaseScript
{
public:
    Test() = default;
    ~Test() = default;

    void start() override final;
    void update(float dt) override final;
};
