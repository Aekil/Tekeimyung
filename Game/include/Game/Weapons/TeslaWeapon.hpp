/**
* @Author   Simon AMBROISE
*/

#pragma once

#include <Engine/Core/BaseScript.hpp>
#include <Engine/Core/ScriptFactory.hpp>

#include <Game/Weapons/IWeapon.hpp>

class TeslaWeapon final : public BaseScript, public IWeapon
{
public:
    TeslaWeapon() = default;
    ~TeslaWeapon() = default;

public:
    virtual void fire();
    virtual void reload();
    void start() override final;
    void update(float dt) override final;
};

REGISTER_SCRIPT(TeslaWeapon);