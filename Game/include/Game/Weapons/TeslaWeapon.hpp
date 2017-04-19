/**
* @Author   Simon AMBROISE
*/

#pragma once

#include <Game/Weapons/IWeapon.hpp>

#define TESLA_WEAPON_NAME "Tesla"

class TeslaWeapon final : public IWeapon
{
public:
    TeslaWeapon();
    ~TeslaWeapon() = default;

public:
    void    fire(Player* player, sTransformComponent* playerTransform, sRenderComponent* playerRender, glm::vec3 playerDirection) override final;
    void    reload() override final;
    void    upgradeByLevel() override final;

    virtual std::string getName()
    {
        return this->Name;
    }

    const static std::string Name;

private:

    void    fireLightning(Player* player, sTransformComponent* playerTransform, sRenderComponent* playerRender, glm::vec3 playerDirection);
    void    spreadLightning(Entity* entity, unsigned int hitLeft, std::vector<Entity*> entitiesAlreadyHit = {});
    void    triggerLightningEffect(Entity* entity);

    void    fireOrb(Player* player, sTransformComponent* playerTransform, sRenderComponent* playerRender, glm::vec3 playerDirection);
};
