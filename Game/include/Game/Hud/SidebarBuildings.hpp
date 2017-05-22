/**
* @Author   Julien CHARDON
*/

#pragma once

#include <Engine/Core/BaseScript.hpp>
#include <Engine/Core/ScriptFactory.hpp>
#include <Engine/Core/Components/Components.hh>

#define N_BUILDABLE_ITEMS   3

typedef struct  s_itemCase
{
    int         id;
    // texture
    char        key; // str ?
    std::string price;
    std::string archetype;
    std::string description;
} t_itemCase;

class SidebarBuildings final : public BaseScript
{
public:
    SidebarBuildings();
    ~SidebarBuildings();

public:
    void start() override final;
    void update(float dt) override final;

private:
    void init();

    void initTextBelow(int index, sUiComponent* uiComponent);
    void initTextTopLeft(int index, sUiComponent* uiComponent);

private:
    std::vector<t_itemCase> _sidebarItems;
};

REGISTER_SCRIPT(SidebarBuildings);
