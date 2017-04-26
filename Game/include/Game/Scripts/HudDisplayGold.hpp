/*
** @Author : Julien CHARDON
*/

#pragma once

#include <Engine/Core/BaseScript.hpp>
#include <Engine/Core/ScriptFactory.hpp>

#define HUD_DISPLAY_GOLD_TAG    "HudDisplayGold"

#define MAX_SIZE_TEXT_GOLDS     256

class HudDisplayGold : public BaseScript
{
public:
    HudDisplayGold() = default;
    virtual ~HudDisplayGold() = default;

    void    start();
    void    update(float dt);

private:
    EntityManager*  _em;
    const Entity*   _gameManager;
    const Entity*   _goldHudDisplay;
};

REGISTER_SCRIPT(HudDisplayGold);