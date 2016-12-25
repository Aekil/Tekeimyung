/*
** @Author : Simon AMBROISE
*/

#pragma once

#include <Engine/Core/BaseScript.hpp>
#include <Engine/Core/ScriptFactory.hpp>

class Tile : public BaseScript
{
public:
    Tile() {};
    ~Tile() {};

public:
    virtual void Start();
    virtual void Update(float dt);

    virtual void OnHoverEnter();
    virtual void OnHoverExit();
};

REGISTER_SCRIPT(Tile, "Tile");