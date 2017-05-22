/**
* @Author   Simon AMBROISE
*/

#pragma once

#include <Engine/Utils/EventSound.hpp>

#include <Game/Weapons/IWeapon.hpp>

#define DEFAULT_WEAPON_NAME "Default"

class DefaultWeapon final : public IWeapon
{
public:
    DefaultWeapon();
    ~DefaultWeapon() = default;

public:
    void    fire(Player* player, sTransformComponent* playerTransform, sRenderComponent* playerRender, glm::vec3 playerDirection) override final;
    void    reload() override final;
    void    upgradeByLevel() override final;
    void    clean() override final;

    virtual std::string getName()
    {
        return this->Name;
    }

    const static std::string Name;

private:
    std::string _tag;
    tEventSound* _shootSound = nullptr;
};