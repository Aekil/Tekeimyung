/*
** @Author : Julien CHARDON
*/

#pragma once

#include <Engine/Core/BaseScript.hpp>
#include <Engine/Core/ScriptFactory.hpp>

#define HUD_DISPLAY_CASTLELIFE_TAG  "HudDisplayCastleLife"

#define MAX_SIZE_TEXT_CASTLELIFE    256

class HudDisplayCastleLife : public BaseScript
{
public:
    HudDisplayCastleLife() = default;
    virtual ~HudDisplayCastleLife() = default;

    void    start();
    void    update(float dt);

private:
    EntityManager*  _em;
    const Entity*   _castle;
    const Entity*   _castleLifeHudDisplay;

    int             _tmpCastleLife = 0;
    int             _tmpMaxCastleLife = 0;
};

REGISTER_SCRIPT(HudDisplayCastleLife);
