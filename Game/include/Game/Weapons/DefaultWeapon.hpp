/**
* @Author   Simon AMBROISE
*/

#pragma once

#include <Engine/Core/BaseScript.hpp>
#include <Engine/Core/ScriptFactory.hpp>

#include <Game/Weapons/IWeapon.hpp>

class DefaultWeapon final : public BaseScript, public IWeapon
{
public:
    DefaultWeapon() = default;
    ~DefaultWeapon() = default;

public:
    virtual void fire();
    virtual void feload();
    void start() override final;
    void update(float dt) override final;
};

REGISTER_SCRIPT(DefaultWeapon);