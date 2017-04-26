/**
* @Author   Guillaume Labey
*/

#include <algorithm>

#include <glm/gtx/string_cast.hpp>

#include <Engine/Window/GameWindow.hpp>
#include <Engine/Debug/Logger.hpp>
#include <Engine/Utils/LevelLoader.hpp>
#include <Engine/Physics/Collisions.hpp>

#include <Engine/Components.hh>
#include <Engine/EntityFactory.hpp>

#include <Engine/Systems/ButtonSystem.hpp>

ButtonSystem::ButtonSystem(GameStateManager* gameStateManager): _gameStateManager(gameStateManager)
{
    addDependency<sRenderComponent>();
    addDependency<sButtonComponent>();
    addDependency<sTransformComponent>();

    _currentSelected = -1;
    _buttonHovered = false;
    setupSelectedIcon();
}

ButtonSystem::~ButtonSystem() {}

void    ButtonSystem::update(EntityManager& em, float elapsedTime)
{
    uint32_t    nbEntities = (uint32_t)_entities.size();

    float       windowHeight = (float)GameWindow::getInstance()->getBufferHeight();
    auto&&      cursor = GameWindow::getInstance()->getMouse().getCursor();
    glm::vec2   cursorPos = glm::vec2(cursor.getX(), windowHeight - cursor.getY());
    bool buttonHovered = _buttonHovered;

    _buttonHovered = false;

    for (uint32_t i = 0; i < nbEntities; ++i) {
        Entity* entity = em.getEntity(_entities[i]);

        handleButtonMouseHover(em, entity, i, cursorPos);
    }

    // Remove button selection if a button was hovered by the mouse
    // But now is not hovered
    if (buttonHovered && !_buttonHovered)
    {
        // Remove selection
        removeSelected(em, _currentSelected);
        _currentSelected = -1;
    }

    handleButtonsKeys(em);
    handleButtonsActions(em);
}

void    ButtonSystem::handleButtonMouseHover(EntityManager& em, Entity* entity, uint32_t entityIdx, const glm::vec2& cursorPos)
{
    sRenderComponent*       render = entity->getComponent<sRenderComponent>();
    sTransformComponent*    transform = entity->getComponent<sTransformComponent>();
    const glm::vec2&        size = glm::vec2(render->getModel()->getSize() * transform->getScale());
    glm::vec2               pos = glm::vec2(transform->getPos()) - (size / 2.0f);

    // Check the mouse is in the button (2D AABB collision)
    if (render->enabled == true && Collisions::pointVSAABB2D(cursorPos, pos, size))
    {
        removeSelected(em, _currentSelected);
        _currentSelected = entityIdx;
        setSelected(em, _currentSelected, true);
        _buttonHovered = true;
    }
}

void    ButtonSystem::handleButtonsKeys(EntityManager &em)
{
    auto        &&keyboard = GameWindow::getInstance()->getKeyboard();
    bool        upPressed = keyboard.getStateMap()[Keyboard::eKey::UP] == Keyboard::eKeyState::KEY_PRESSED;
    bool        downPressed = keyboard.getStateMap()[Keyboard::eKey::DOWN] == Keyboard::eKeyState::KEY_PRESSED;

    if (_entities.size() > 0 && _currentSelected != -1)
    {
        if (upPressed)
        {
            removeSelected(em, _currentSelected);
            _currentSelected = getPreviousButtonEnabled(em, _currentSelected);
            setSelected(em, _currentSelected);
        }
        else if (downPressed)
        {
            removeSelected(em, _currentSelected);
            _currentSelected = getNextButtonEnabled(em, _currentSelected);
            setSelected(em, _currentSelected);
        }

    }
    // No button is currently selected, select default 0 when a key is pressed
    else if (_entities.size() > 0 && _currentSelected == -1)
    {
        if (upPressed || downPressed)
        {
            // Get the next button from the end of the list
            // It allows to skip all disabled buttons of the start of the list
            _currentSelected = _entities.size() > 0 ? _entities.size() - 1 : 0;
            _currentSelected = getNextButtonEnabled(em, _currentSelected);
            setSelected(em, _currentSelected);
        }

    }
}

void    ButtonSystem::handleButtonsActions(EntityManager& em)
{
    if (_currentSelected == -1)
        return;

    Entity* entity = em.getEntity(_entities[_currentSelected]);
    if (!entity)
        return;

    auto    &&keyboard = GameWindow::getInstance()->getKeyboard();
    auto    &&mouse = GameWindow::getInstance()->getMouse();
    bool    spacebarPressed = keyboard.getStateMap()[Keyboard::eKey::ENTER] == Keyboard::eKeyState::KEY_PRESSED;
    bool    mouseClicked = mouse.getStateMap()[Mouse::eButton::MOUSE_BUTTON_1] == Mouse::eButtonState::CLICK_PRESSED;

    // Launch button action
    sButtonComponent* button = entity->getComponent<sButtonComponent>();
    if ((spacebarPressed && button->selected) ||
        (mouseClicked && button->hovered))
    {
        if (button->action == sButtonComponent::eAction::ADD_LEVEL &&
            button->actionLevel.size() > 0)
        {
            std::shared_ptr<GameState> gameState = LevelLoader::getInstance()->createLevelState(button->actionLevel, _gameStateManager);
            _gameStateManager->addState(gameState);
        }
        else if (button->action == sButtonComponent::eAction::REPLACE_CURRENT_LEVEL &&
            button->actionLevel.size() > 0)
        {
            std::shared_ptr<GameState> gameState = LevelLoader::getInstance()->createLevelState(button->actionLevel, _gameStateManager);
            _gameStateManager->replaceState(gameState);
        }
        else if (button->action == sButtonComponent::eAction::REMOVE_CURRENT_LEVEL)
        {
            _gameStateManager->removeCurrentState();
        }
    }
}

void    ButtonSystem::setSelected(EntityManager &em, int buttonIdx, bool hovered)
{
    // Out of range
    if (buttonIdx < 0 || buttonIdx >= _entities.size())
        return;

    Entity* entity = em.getEntity(_entities[buttonIdx]);
    setSelected(entity, hovered);
}

void    ButtonSystem::setSelected(Entity* entity, bool hovered)
{
    // The entity does not exist
    if (!entity)
        return;

    sButtonComponent* button = entity->getComponent<sButtonComponent>();
    button->hovered = hovered;

    // Display icon
    _iconRender->_display = true;

    // The button is already selected
    if (button->selected)
        return;

    //  Calculate icon selected new position.
    sTransformComponent*    iconTransform = _iconSelected->getComponent<sTransformComponent>();
    sRenderComponent*       iconRender = _iconSelected->getComponent<sRenderComponent>();
    sTransformComponent*    buttonTransform = entity->getComponent<sTransformComponent>();
    sRenderComponent*       buttonRender = entity->getComponent<sRenderComponent>();

    const glm::vec3& buttonSize = buttonRender->getModel()->getSize() * buttonTransform->getScale();
    const glm::vec3& iconSize = iconRender->getModel()->getSize() * iconTransform->getScale();

    glm::vec3 iconPos = iconTransform->getPos();
    iconPos.x = buttonTransform->getPos().x - (buttonSize.x / 2.0f) - 32.0f;
    iconPos.y = buttonTransform->getPos().y;
    iconTransform->setPos(iconPos);

    button->selected = true;
}

void    ButtonSystem::removeSelected(EntityManager &em, int buttonIdx)
{
    // Out of range
    if (buttonIdx < 0 || buttonIdx >= _entities.size())
        return;

    Entity* entity = em.getEntity(_entities[_currentSelected]);

    // The entity does not exist
    if (!entity)
        return;

    // Hide icon
    _iconRender->_display = false;

    sButtonComponent* button = entity->getComponent<sButtonComponent>();
    button->hovered = false;

    // The button is not selected
    if (!button->selected)
        return;

    button->selected = false;
}

void    ButtonSystem::setupSelectedIcon()
{
    _iconSelected = EntityFactory::createOrGetEntity("ICON_SELECTED");
    _iconRender = _iconSelected->getComponent<sRenderComponent>();
    _iconRender->_display = false;
}

uint32_t    ButtonSystem::getNextButtonEnabled(EntityManager& em, uint32_t buttonIdx, uint32_t recurse)
{
    // We went across all buttons, and no prev was found (All disabled ?)
    if (recurse == _entities.size() - 1)
        return (buttonIdx);

    int nextButtonIdx = buttonIdx + 1;
    nextButtonIdx = nextButtonIdx % _entities.size();

    Entity* nextButtonEntity = em.getEntity(_entities[nextButtonIdx]);
    sButtonComponent* nextButton = nextButtonEntity->getComponent<sButtonComponent>();

    if (nextButton->enabled)
    {
        return (nextButtonIdx);
    }

    return (getNextButtonEnabled(em, nextButtonIdx, recurse + 1));
}

uint32_t    ButtonSystem::getPreviousButtonEnabled(EntityManager& em, uint32_t buttonIdx, uint32_t recurse)
{
    // We went across all buttons, and no prev was found (All disabled ?)
    if (recurse == _entities.size() - 1)
        return (buttonIdx);

    int prevButtonIdx = buttonIdx - 1;
    if (prevButtonIdx < 0)
        prevButtonIdx = (int)_entities.size() - 1;

    Entity* prevButtonEntity = em.getEntity(_entities[prevButtonIdx]);
    sButtonComponent* prevButton = prevButtonEntity->getComponent<sButtonComponent>();

    if (prevButton->enabled)
    {
        return (prevButtonIdx);
    }

    return (getPreviousButtonEnabled(em, prevButtonIdx, recurse + 1));
}
