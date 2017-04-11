/**
* @Author   Simon AMBROISE
*/

#pragma once

#include <Engine/Core/BaseScript.hpp>
#include <Engine/Core/ScriptFactory.hpp>

#include <Game/Weapons/IWeapon.hpp>

class LaserWeapon final : public BaseScript, public IWeapon
{
public:
    LaserWeapon() = default;
    ~LaserWeapon() = default;

public:
    virtual void fire();
    virtual void reload();
};

REGISTER_SCRIPT(LaserWeapon);