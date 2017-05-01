/**
* @Author   Julien CHARDON
*/

#include <Engine/EntityFactory.hpp>
#include <Engine/Graphics/Model2DRenderer.hpp>
#include <Game/Scripts/SidebarBuildings.hpp>

SidebarBuildings::SidebarBuildings()
{}

SidebarBuildings::~SidebarBuildings()
{}

void SidebarBuildings::init()
{
    for (int i = 0; i < N_BUILDABLE_ITEMS; i++)
    {
        _sidebarItems[i].id = i;
        //_sideBarItems[i].texture = ?;
    }

    // HARD INIT
    _sidebarItems[0].key = '1';
    _sidebarItems[0].name = "Tower Base";
    _sidebarItems[0].price = "50";

    _sidebarItems[1].key = '2';
    _sidebarItems[1].name = "Wall";
    _sidebarItems[1].price = "30";

    _sidebarItems[2].key = '3';
    _sidebarItems[2].name = "Trap 1";
    _sidebarItems[2].price = "40";

    _sidebarItems[3].key = '4';
    _sidebarItems[3].name = "Trap 2";
    _sidebarItems[3].price = "40";

    _sidebarItems[4].key = '5';
    _sidebarItems[4].name = "Trap 3";
    _sidebarItems[4].price = "40";

    // Init selection
    _itemSelection.selected = false;
    _itemSelection.idSelected = -1;
}

void SidebarBuildings::start()
{
    this->init();

    
}

void SidebarBuildings::update(float dt)
{
    // if construction mode == true
    //  -> selected = false
    //  -> idSelected = ?
    // else
    //  -> return

    //auto modelRenderer = Model2DRenderer::getInstance();
    //modelRenderer->renderModelOnPlane("TOWER_FIRE", "SidebarItem", 64, 64); // HUD_SIDEBAR_SLOT
    
}