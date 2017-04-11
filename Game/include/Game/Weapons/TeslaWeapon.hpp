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
};

REGISTER_SCRIPT(TeslaWeapon);