/**
* @Author   Simon AMBROISE
*/

#pragma once

#include <Game/Weapons/IWeapon.hpp>

class TeslaWeapon final : public IWeapon
{
public:
    TeslaWeapon();
    ~TeslaWeapon() = default;

public:
    virtual void fire(sTransformComponent* playerTransform, sRenderComponent* playerRender, glm::vec3 playerDirection) override final;
    virtual void reload() override final;
    virtual void upgradeByLevel() override final;
};
