/**
* @Author   Mathieu Chassara
*/

#include <iostream>

#include <Engine/Window/GameWindow.hpp>
#include <Engine/Sound/SoundManager.hpp>

#include <Engine/Systems/RenderingSystem.hpp>
#include <Engine/Systems/UISystem.hpp>
#include <Engine/Systems/ButtonSystem.hpp>
#include <Engine/Utils/LevelLoader.hpp>
#include <Engine/EntityFactory.hpp>
#include <Engine/Core/Components/Components.hh>

#include <Game/GameStates/CreditsState.hpp>

CreditsState::~CreditsState() {}

void    CreditsState::onEnter() {}

void    CreditsState::setupSystems()
{
    this->_world.addSystem<ParticleSystem>();
    this->_world.addSystem<UISystem>();
    this->_world.addSystem<ButtonSystem>(_gameStateManager);
    this->_world.addSystem<RenderingSystem>(_world.getSystem<ParticleSystem>()->getEmitters());
}

bool    CreditsState::init()
{
    this->retrievePages();
    this->updateLevelDisplayed();
    this->retrieveButtons();
    this->updatePaging();
    return (true);
}

bool    CreditsState::update(float elapsedTime)
{
    auto &&keyboard = GameWindow::getInstance()->getKeyboard();
    bool success = GameState::update(elapsedTime);

    // Quit the state
    if (keyboard.getStateMap()[Keyboard::eKey::ESCAPE] == Keyboard::eKeyState::KEY_PRESSED/* ||
                                                                                          keyboard.getStateMap()[Keyboard::eKey::H] == Keyboard::eKeyState::KEY_PRESSED*/)
    {
        return (false);
    }

    if (this->handleButtons() == false)
        return (false);

    return (success);
}

void    CreditsState::retrievePages()
{
    this->_pages.push_back("PageCredits");
    //this->_pages.push_back("PageLicences");
}

void    CreditsState::retrieveButtons()
{
    int size = this->_pages.size();

    this->_buttonLeftChevron = EntityFactory::createOrGetEntity("BUTTON_LEFT_CHEVRON");
    this->_buttonRightChevron = EntityFactory::createOrGetEntity("BUTTON_RIGHT_CHEVRON");

    if (size == 1)
    {
        this->setButton(this->_buttonLeftChevron, false);
        this->setButton(this->_buttonRightChevron, false);
    }
    else if (size > 1)
    {
        this->setButton(this->_buttonLeftChevron, false);
        this->setButton(this->_buttonRightChevron, true);
    }
}

void    CreditsState::updatePaging()
{
    //LOG_DEBUG("Current page index : %d", this->_currentPageIndex);
    this->setButton(this->_buttonLeftChevron, false);
    this->setButton(this->_buttonRightChevron, false);

    if (this->_pages.size() > 1)
    {
        if (this->_currentPageIndex == 0)
            this->setButton(this->_buttonRightChevron, true);
        else if (this->_currentPageIndex == this->_pages.size() - 1)
            this->setButton(this->_buttonLeftChevron, true);
        else
        {
            this->setButton(this->_buttonLeftChevron, true);
            this->setButton(this->_buttonRightChevron, true);
        }
    }

    this->updatePagingText();
}

void        CreditsState::updatePagingText()
{
    if (this->_tmpCurrentPageIndex != this->_currentPageIndex)
    {
        Entity* entityPageText = EntityFactory::createOrGetEntity("TEXT_PAGE_NUMBER");

        if (entityPageText != nullptr)
        {
            sTextComponent* textComp = entityPageText->getComponent<sTextComponent>();
            char            pagingText[MAX_SIZE_TEXT_PAGES];

            sprintf_s(pagingText, "PAGE : %d / %d", this->_currentPageIndex + 1, this->_pages.size());
            textComp->text.setContent(pagingText);
        }

        this->_tmpCurrentPageIndex = this->_currentPageIndex;
    }
}

bool        CreditsState::handleButtons()
{
    auto&&  keyboard = GameWindow::getInstance()->getKeyboard();
    auto&&  mouse = GameWindow::getInstance()->getMouse();
    bool    enterPressed = keyboard.getStateMap()[Keyboard::eKey::ENTER] == Keyboard::eKeyState::KEY_PRESSED;
    bool    mouseClicked = mouse.getStateMap()[Mouse::eButton::MOUSE_BUTTON_1] == Mouse::eButtonState::CLICK_PRESSED;

    sButtonComponent*   leftChevron = this->_buttonLeftChevron->getComponent<sButtonComponent>();
    sButtonComponent*   rightChevron = this->_buttonRightChevron->getComponent<sButtonComponent>();

    if ((enterPressed == true && (leftChevron->selected == true || rightChevron->selected == true)) ||
        (mouseClicked == true && (leftChevron->hovered == true || rightChevron->hovered == true)))
    {
        if (leftChevron->selected == true || leftChevron->hovered == true)
            this->_currentPageIndex = std::max(0, this->_currentPageIndex - 1);
        else if (rightChevron->selected == true || rightChevron->hovered == true)
            this->_currentPageIndex = std::min((int) this->_pages.size() - 1, this->_currentPageIndex + 1);
        this->updateLevelDisplayed();
        this->retrieveButtons();
        this->updatePaging();
    }

    return (true);
}

void        CreditsState::updateLevelDisplayed()
{
    auto    em = EntityFactory::getBindedEntityManager();

    em->destroyAllEntities();
    LevelLoader::getInstance()->load(this->_pages[this->_currentPageIndex], em);
    this->_world.getSystem<ButtonSystem>()->setupSelectedIcon();
}

void        CreditsState::setButton(Entity* buttonEntity, bool enabled)
{
    sNameComponent*     nameComponent;
    sRenderComponent*   renderComponent;
    sUiComponent*       uiComponent;
    sButtonComponent*   buttonComponent;

    nameComponent = buttonEntity->getComponent<sNameComponent>();
    renderComponent = buttonEntity->getComponent<sRenderComponent>();
    uiComponent = buttonEntity->getComponent<sUiComponent>();
    buttonComponent = buttonEntity->getComponent<sButtonComponent>();

    if (renderComponent == nullptr)
    {
        LOG_WARN("Could not find %s on Entity with archetype %s", "sRenderComponent", nameComponent->value.c_str());
        return;
    }
    if (uiComponent == nullptr)
    {
        LOG_WARN("Could not find %s on Entity with archetype %s", "sUiComponent", nameComponent->value.c_str());
        return;
    }
    if (buttonComponent == nullptr)
    {
        LOG_WARN("Could not find %s on Entity with archetype %s", "sButtonComponent", nameComponent->value.c_str());
        return;
    }

    renderComponent->enabled = enabled;
    uiComponent->enabled = enabled;
    buttonComponent->enabled = enabled;
}
