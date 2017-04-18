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
    void    fire(Player* player, sTransformComponent* playerTransform, sRenderComponent* playerRender, glm::vec3 playerDirection) override final;
    void    reload() override final;
    void    upgradeByLevel() override final;

private:

    void    fireLightning(Player* player, sTransformComponent* playerTransform, sRenderComponent* playerRender, glm::vec3 playerDirection);
    void    spreadLightning(Entity* entity, unsigned int hitLeft, std::vector<Entity*> entitiesAlreadyHit = {});
    void    triggerLightningEffect(Entity* entity);

    void    fireOrb(Player* player, sTransformComponent* playerTransform, sRenderComponent* playerRender, glm::vec3 playerDirection);
};
