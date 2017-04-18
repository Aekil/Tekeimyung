/**
* @Author   Simon AMBROISE
*/

#pragma once

#include <Game/Weapons/IWeapon.hpp>

class LaserWeapon final : public IWeapon
{
public:
    LaserWeapon();
    ~LaserWeapon() = default;

public:
    void    fire(Player* player, sTransformComponent* playerTransform, sRenderComponent* playerRender, glm::vec3 playerDirection) override final;
    void    reload() override final;
    void    upgradeByLevel() override final;
};
