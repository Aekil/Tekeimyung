/*
** @Author : Julien CHARDON
*/

#pragma once

#include <Engine/Core/BaseScript.hpp>
#include <Engine/Core/ScriptFactory.hpp>

#define HUD_DISPLAY_CASTLELIFE_TAG  "HudDisplayCastleLife"

#define MAX_SIZE_TEXT_CASTLELIFE    256

class DisplayCastleLife : public BaseScript
{
public:
    DisplayCastleLife() = default;
    virtual ~DisplayCastleLife() = default;

    void    start();
    void    update(float dt);

private:
    EntityManager*  _em;
    const Entity*   _castle;
    const Entity*   _castleLifeHudDisplay;

    int             _tmpCastleLife = 0;
    int             _tmpMaxCastleLife = 0;
};

REGISTER_SCRIPT(DisplayCastleLife);
