/*
** Author : Simon AMBROISE
*/

#pragma once

#include <Engine/Core/BaseScript.hpp>
#include <Engine/Core/ScriptFactory.hpp>

class Player : public BaseScript
{
public:
    Player() {};
    ~Player() {};

    int m_health;
private:
    void Death();

public:
    virtual void Start();
    virtual void Update(float dt);
};

REGISTER_SCRIPT(Player, "Player");