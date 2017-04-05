/*
** @Author : Julien CHARDON
*/

#pragma once

#include <Engine/Core/BaseScript.hpp>
#include <Engine/Core/ScriptFactory.hpp>

class HudDisplayGold : public BaseScript
{
public:
    HudDisplayGold() = default;
    virtual ~HudDisplayGold() = default;

    void    start();
    void    update(float dt);

private:

};

REGISTER_SCRIPT(HudDisplayGold);