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
    void fire(sTransformComponent* playerTransform, sRenderComponent* playerRender, glm::vec3 playerDirection) override final;
    void reload() override final;
    void upgradeByLevel() override final;
};
