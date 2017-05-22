/**
* @Author   Simon AMBROISE
*/

#pragma once

#include <Engine/Utils/EventSound.hpp>

#include <Game/Weapons/IWeapon.hpp>


class LaserWeapon final : public IWeapon
{
    friend class Player;

public:
    LaserWeapon();
    ~LaserWeapon() = default;

public:
    void fire(Player* player, sTransformComponent* playerTransform, sRenderComponent* playerRender, glm::vec3 playerDirection) override final;
    void reload() override final;
    void upgradeByLevel() override final;
    void clean() override final;

    virtual std::string getName()
    {
        return this->Name;
    }

    const static std::string Name;

private:
    void fireOneEnemy(Player* player, sTransformComponent* playerTransform, sRenderComponent* playerRender, glm::vec3 playerDirection);
    void fireMultipleEnemy(Player* player, sTransformComponent* playerTransform, sRenderComponent* playerRender, glm::vec3 playerDirection);
    Entity* farestEnemy(sTransformComponent* playerTransform, std::vector<Entity*> entities);

private:
    bool _timeDamage = false;
    std::vector<Entity*> _oldEntitiesHited;
    float _timeDamageDealt = 0.05f;
    bool _pierceEnemy = false;

    bool _timeExplosion = false;
    float _timeExplosionPercent = 0.17f;
    Entity* _laser = nullptr;

    tEventSound* _shootSound = nullptr;
};
