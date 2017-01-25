/*
** @Author : Simon AMBROISE
*/

#pragma once

#include <Engine/Core/ScriptFactory.hpp>

class Tile : public BaseScript
{
public:
    Tile() {};
    ~Tile() {};

    bool onHover = false;
public:
    virtual void Start();
    virtual void Update(float dt);

    virtual void OnHoverEnter();
    virtual void OnHoverExit();

    void SetBuildable(bool);
private:
    Entity* preview = nullptr;
    std::vector<std::string> buildableItems;
    int currentIdx = 0;
    bool m_buildable = false;
};

REGISTER_SCRIPT(Tile, "Tile");