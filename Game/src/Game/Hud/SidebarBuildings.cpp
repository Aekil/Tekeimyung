/**
* @Author   Julien CHARDON
*/

#include <Engine/EntityFactory.hpp>
#include <Engine/Graphics/Model2DRenderer.hpp>
#include <Game/Hud/SidebarBuildings.hpp>
#include <Game/Hud/SidebarItem.hpp>
#include <Game/Hud/SidebarText.hpp>

SidebarBuildings::SidebarBuildings()
{}

SidebarBuildings::~SidebarBuildings()
{}

void SidebarBuildings::init()
{
    this->_sidebarItems.resize(N_BUILDABLE_ITEMS);

    // Hard init
    this->_sidebarItems[0].key = '1';
    this->_sidebarItems[0].description = "Tower Base\nUse to block enemies.\nCan build turret on it.";
    this->_sidebarItems[0].archetype = "TILE_BASE_TURRET";
    this->_sidebarItems[0].price = "50";

    this->_sidebarItems[1].key = '2';
    this->_sidebarItems[1].description = "Wall\nUse to block enemies.\nCanno't build turret on it.";
    this->_sidebarItems[1].archetype = "TILE_WALL";
    this->_sidebarItems[1].price = "30";

    this->_sidebarItems[2].key = '3';
    this->_sidebarItems[2].description = "Tower\nUse to kill enemies.\nRange : 6 tiles";
    this->_sidebarItems[2].archetype = "TOWER_FIRE";
    this->_sidebarItems[2].price = "50";

    for (int i = 0; i < N_BUILDABLE_ITEMS; i++)
    {
        this->_sidebarItems[i].id = i;
        auto item = this->Instantiate("SIDEBAR_ITEM");

        item->setTag(this->_sidebarItems[i].archetype + "_SIDEBAR_ITEM");

        auto itemTextSidebarItem = this->getEntityScript<SidebarItem>(item, "SidebarItem");

        if (itemTextSidebarItem == nullptr)
            EXCEPT(NullptrException, "Entity with archetype %s doesn't have script %s", "SIDEBAR_ITEM", "SidebarItem");

        itemTextSidebarItem->setDescription(this->_sidebarItems[i].description);

        auto uiComponent = item->getComponent<sUiComponent>();

        auto modelRenderer = Model2DRenderer::getInstance();
        modelRenderer->renderModelOnPlane(this->_sidebarItems[i].archetype, item, 128, 128); // HUD_SIDEBAR_SLOT

        uiComponent->offset = glm::vec2(i * 6, 5);

        auto itemCadre = this->Instantiate("SIDEBAR_ITEM_CADRE");

        itemTextSidebarItem->setBorderEntity(itemCadre);
        auto uiComponentCadre = itemCadre->getComponent<sUiComponent>();

        uiComponentCadre->offset = glm::vec2(i * 6, 5);

        this->initTextBelow(i, uiComponent);
        this->initTextTopLeft(i, uiComponent);
    }
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

    char text[64];

    sprintf_s(text, "%s coins", this->_sidebarItems[index].price.c_str());

    itemTextTextComponent->text.setContent(text);

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

    char text[64];

    sprintf_s(text, "%c", this->_sidebarItems[index].key);

    itemTextTextComponent->text.setContent(text);
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
