#pragma once

#include <Engine/Core/BaseScript.hpp>
#include <Engine/Core/ScriptFactory.hpp>

#include <Game/Scripts/Health.hpp>

class Castle final : public BaseScript, public Health
{
public:
    Castle() = default;
    ~Castle() = default;

public:
    void start() override final;
    void update(float dt) override final;
    void onCollisionEnter(Entity* entity) override final;
    void death() override final;
};

REGISTER_SCRIPT(Castle);
