/**
* @Author   Julien CHARDON
*/

#pragma once

#include <Engine/Core/BaseScript.hpp>
#include <Engine/Core/ScriptFactory.hpp>
#include <Engine/Components.hh>

#define N_BUILDABLE_ITEMS   5

typedef struct  s_itemCase
{
    int         id;
    // texture
    char        key; // str ?
    std::string name;
    std::string price;
} t_itemCase;

typedef struct  s_itemSelection
{
    bool        selected;
    int         idSelected;
} t_itemSelection;

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

private:
    t_itemCase          _sidebarItems[N_BUILDABLE_ITEMS];
    t_itemSelection     _itemSelection;
};

REGISTER_SCRIPT(SidebarBuildings);