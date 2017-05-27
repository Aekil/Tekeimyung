/**
* @Author   Simon AMBROISE
*/

#pragma once

#include <Engine/Utils/EventSound.hpp>

#include <Game/Weapons/IWeapon.hpp>
#include <Game/Weapons/TeslaOrb.hpp>

#define TESLA_WEAPON_NAME "Tesla"

class TeslaWeapon final : public IWeapon
{
    // Allow TeslaOrb to access TeslaWeapon class in triggerExplosion
    // because it needs to access triggerLightningEffect
    friend void TeslaOrb::triggerExplosion();

public:
    TeslaWeapon();
    ~TeslaWeapon();

public:
    void    fire(Player* player, sTransformComponent* playerTransform, sRenderComponent* playerRender, glm::vec3 playerDirection) override final;
    void    reload() override final;
    void    upgradeByLevel() override final;
    void    update(float dt, Mouse& mouse) override final;
    void    clean() override final;
    void    setFiredOrb(TeslaOrb* firedOrb);

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

private:
    tEventSound* _shootSound = nullptr;

    TeslaOrb* _firedOrb = nullptr;

    float _lastOrbDestruction = 0.0f;
};
