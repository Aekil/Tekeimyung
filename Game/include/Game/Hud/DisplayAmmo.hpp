/*
** @Author : Julien CHARDON
*/

#pragma once

#include <Engine/Core/BaseScript.hpp>
#include <Engine/Core/ScriptFactory.hpp>

#define MAX_SIZE_TEXT_AMMO    256

class DisplayAmmo : public BaseScript
{
public:
    DisplayAmmo() = default;
    virtual ~DisplayAmmo() = default;

    void    start();
    void    update(float dt);

private:
    EntityManager*  _em;
    const Entity*   _player;

    int             _previousAmmo = -1;
};
