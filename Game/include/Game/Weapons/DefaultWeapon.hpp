/**
* @Author   Simon AMBROISE
*/

#pragma once

#include <Engine/Utils/EventSound.hpp>

#include <Game/Weapons/IWeapon.hpp>

class DefaultWeapon final : public IWeapon
{
public:
    DefaultWeapon();
    ~DefaultWeapon() = default;

public:
    virtual void fire(sTransformComponent* playerTransform, sRenderComponent* playerRender, glm::vec3 playerDirection);
    virtual void reload();
    virtual void upgradeByLevel();

private:
    std::string _tag;
    tEventSound* _shootSound = nullptr;
};