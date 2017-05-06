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
    this->_sidebarItems.resize(N_BUILDABLE_ITEMS);

    for (int i = 0; i < N_BUILDABLE_ITEMS; i++)
    {
        this->_sidebarItems[i].id = i;
        //_sideBarItems[i].texture = ?;
    }

    // Hard init
    this->_sidebarItems[0].key = '1';
    this->_sidebarItems[0].description = "Tower Base";
    this->_sidebarItems[0].price = "50";

    this->_sidebarItems[1].key = '2';
    this->_sidebarItems[1].description = "Wall";
    this->_sidebarItems[1].price = "30";

    this->_sidebarItems[2].key = '3';
    this->_sidebarItems[2].description = "Tower";
    this->_sidebarItems[2].price = "50";

    for (auto _sidebarItem : this->_sidebarItems)
    {
        auto item = this->Instantiate("SIDEBAR_ITEM");

        auto uiComponent = item->getComponent<sUiComponent>();

        //uiComponent->offset = glm::vec2();
    }

    //this->_sidebarItems[2].key = '3';
    //this->_sidebarItems[2].description = "Trap 1";
    //this->_sidebarItems[2].price = "40";

    //this->_sidebarItems[3].key = '4';
    //this->_sidebarItems[3].description = "Trap 2";
    //this->_sidebarItems[3].price = "40";

    //this->_sidebarItems[4].key = '5';
    //this->_sidebarItems[4].description = "Trap 3";
    //this->_sidebarItems[4].price = "40";

    // Init selection
    this->_itemSelection.selected = false;
    this->_itemSelection.idSelected = -1;
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