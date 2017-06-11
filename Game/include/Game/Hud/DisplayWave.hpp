/*
** @Author : Julien CHARDON
*/

#pragma once

#include <Engine/Core/BaseScript.hpp>
#include <Engine/Core/ScriptFactory.hpp>

#define HUD_DISPLAY_WAVE_TAG    "HudDisplayWave"

#define MAX_SIZE_TEXT_WAVES     256

class DisplayWave : public BaseScript
{
public:
    DisplayWave() = default;
    virtual ~DisplayWave() = default;

    void    start();
    void    update(float dt);

private:
    EntityManager*  _em;
    const Entity*   _gameManager;
    const Entity*   _waveHudDisplay;

    int             _tmpNbWaves = -1;
    int             _tmpcurrentWave = -1;
};
