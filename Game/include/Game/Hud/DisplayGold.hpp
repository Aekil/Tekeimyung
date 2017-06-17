/*
** @Author : Julien CHARDON
*/

#pragma once

#include <Engine/Core/BaseScript.hpp>
#include <Engine/Core/ScriptFactory.hpp>

#define HUD_DISPLAY_GOLD_TAG    "HudDisplayGold"

#define MAX_SIZE_TEXT_GOLDS     256

class DisplayGold : public BaseScript
{
public:
    DisplayGold() = default;
    virtual ~DisplayGold() = default;

    void    start();
    void    update(float dt);

private:
    EntityManager*  _em;
    const Entity*   _gameManager;
    const Entity*   _goldHudDisplay;

    int             _tmpGolds = -1;
};
