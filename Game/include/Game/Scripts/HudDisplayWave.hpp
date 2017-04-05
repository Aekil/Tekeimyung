/*
** @Author : Julien CHARDON
*/

#pragma once

#include <Engine/Core/BaseScript.hpp>
#include <Engine/Core/ScriptFactory.hpp>

class HudDisplayWave : public BaseScript
{
public:
    HudDisplayWave() = default;
    virtual ~HudDisplayWave() = default;

    void    start();
    void    update(float dt);

private:

};

REGISTER_SCRIPT(HudDisplayWave);