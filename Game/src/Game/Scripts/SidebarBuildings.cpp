/**
* @Author   Julien CHARDON
*/

#include <Engine/EntityFactory.hpp>
#include <Engine/Graphics/Model2DRenderer.hpp>
#include <Game/Scripts/SidebarBuildings.hpp>
#include <Game/Scripts/SidebarItem.hpp>
#include <Game/Scripts/SidebarText.hpp>

SidebarBuildings::SidebarBuildings()
{}

SidebarBuildings::~SidebarBuildings()
{}

void SidebarBuildings::init()
{
    this->_sidebarItems.resize(N_BUILDABLE_ITEMS);

    // Hard init
    this->_sidebarItems[0].key = '1';
    this->_sidebarItems[0].description = "Tower Base";
    this->_sidebarItems[0].archetype = "TILE_BASE_TURRET";
    this->_sidebarItems[0].price = "50";

    this->_sidebarItems[1].key = '2';
    this->_sidebarItems[1].description = "Wall";
    this->_sidebarItems[1].archetype = "TILE_WALL";
    this->_sidebarItems[1].price = "30";

    this->_sidebarItems[2].key = '3';
    this->_sidebarItems[2].description = "Tower";
    this->_sidebarItems[2].archetype = "TOWER_FIRE";
    this->_sidebarItems[2].price = "50";

    for (int i = 0; i < N_BUILDABLE_ITEMS; i++)
    {
        this->_sidebarItems[i].id = i;
        auto item = this->Instantiate("SIDEBAR_ITEM");

        auto itemTextSidebarItem = getEntityScript<SidebarItem>(item, "SidebarItem");

        if (itemTextSidebarItem == nullptr)
            EXCEPT(NullptrException, "Entity with archetype %s doesn't have script %s", "SIDEBAR_ITEM", "SidebarItem");

        itemTextSidebarItem->setDescription(this->_sidebarItems[i].description);

        auto uiComponent = item->getComponent<sUiComponent>();

        auto modelRenderer = Model2DRenderer::getInstance();
        modelRenderer->renderModelOnPlane(this->_sidebarItems[i].archetype, item, 128, 128); // HUD_SIDEBAR_SLOT

        uiComponent->offset = glm::vec2(i * 5, 5);

        auto itemCadre = this->Instantiate("SIDEBAR_ITEM_CADRE");

        auto uiComponentCadre = itemCadre->getComponent<sUiComponent>();

        uiComponentCadre->offset = glm::vec2(i * 5, 5);

        this->initTextBelow(i, uiComponent);
        this->initTextTopLeft(i, uiComponent);

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

void SidebarBuildings::initTextBelow(int index, sUiComponent* uiComponent)
{
    auto itemText = this->Instantiate("HUD_SIDEBAR_TEXT");

    auto itemTextScript = itemText->getComponent<sScriptComponent>();
    auto itemTextTextComponent = itemText->getComponent<sTextComponent>();

    if (itemTextTextComponent == nullptr)
        EXCEPT(NullptrException, "Entity with archetype %s doesn't have component %s", "SIDEBAR_TEXT", "sTextComponent");

    if (itemTextScript == nullptr)
        EXCEPT(NullptrException, "Entity with archetype %s doesn't have component %s", "SIDEBAR_TEXT", "sScriptComponent");

    char                waveText[64];

    sprintf_s(waveText, "%s coins", this->_sidebarItems[index].price.c_str());

    itemTextTextComponent->text.setContent(waveText);

    auto itemTextSidebarText = itemTextScript->getScript<SidebarText>("SidebarText");

    if (itemTextSidebarText == nullptr)
        EXCEPT(NullptrException, "Entity with archetype %s doesn't have script %s", "SIDEBAR_TEXT", "SidebarText");

    itemTextSidebarText->setParentPos(uiComponent->offset, ePosition::BELOW);
}

void SidebarBuildings::initTextTopLeft(int index, sUiComponent* uiComponent)
{
    auto itemText = this->Instantiate("HUD_SIDEBAR_TEXT");

    auto itemTextScript = itemText->getComponent<sScriptComponent>();
    auto itemTextTextComponent = itemText->getComponent<sTextComponent>();

    if (itemTextTextComponent == nullptr)
        EXCEPT(NullptrException, "Entity with archetype %s doesn't have component %s", "SIDEBAR_TEXT", "sTextComponent");

    if (itemTextScript == nullptr)
        EXCEPT(NullptrException, "Entity with archetype %s doesn't have component %s", "SIDEBAR_TEXT", "sScriptComponent");

    char                waveText[64];

    sprintf_s(waveText, "%c", this->_sidebarItems[index].key);

    itemTextTextComponent->text.setContent(waveText);
    itemTextTextComponent->text.setFontSize(25);

    auto itemTextSidebarText = itemTextScript->getScript<SidebarText>("SidebarText");

    if (itemTextSidebarText == nullptr)
        EXCEPT(NullptrException, "Entity with archetype %s doesn't have script %s", "SIDEBAR_TEXT", "SidebarText");

    itemTextSidebarText->setParentPos(uiComponent->offset, ePosition::TOP_LEFT);
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
