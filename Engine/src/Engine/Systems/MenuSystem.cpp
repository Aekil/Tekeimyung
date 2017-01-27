/**
* @Author   Guillaume Labey
*/

#include <algorithm>

#include <glm/gtx/string_cast.hpp>

#include <Engine/Window/GameWindow.hpp>
#include <Engine/Utils/Logger.hpp>

#include <Engine/Components.hh>
#include <Engine/EntityFactory.hpp>

#include <Engine/Systems/MenuSystem.hpp>

MenuSystem::MenuSystem()
{
    addDependency<sRenderComponent>();
    addDependency<sUiComponent>();
    addDependency<sTransformComponent>();

    _monitoringKey = MonitoringDebugWindow::getInstance()->registerSystem(MENU_SYSTEM_NAME);

    _currentSelected = -1;
    setupSelectedIcon();
    _buttonHovered = false;
}

MenuSystem::~MenuSystem() {}

bool    MenuSystem::init()
{
    //  For future purposes, it is commented for now.
    //if (!GameWindow::getInstance()->isCursorVisible())
    //    GameWindow::getInstance()->setCursorVisible(true);
    return (true);
}

void    MenuSystem::update(EntityManager& em, float elapsedTime)
{
    Timer       timer;
    uint32_t    nbEntities = (uint32_t)_entities.size();

    float       windowHeight = (float)GameWindow::getInstance()->getBufferHeight();
    auto&&      cursor = GameWindow::getInstance()->getMouse().getCursor();
    glm::vec2   cursorPos = glm::vec2(cursor.getX(), windowHeight - cursor.getY());
    bool buttonHovered = _buttonHovered;

    _buttonHovered = false;

    uint32_t buttonsIdx = 0;
    for (uint32_t i = 0; i < nbEntities; ++i) {
        Entity* entity = em.getEntity(_entities[i]);

        sButtonComponent*       button = entity->getComponent<sButtonComponent>();

        handleAlignment(em, entity, i);
        if (button)
        {
            handleButtonMouseHover(em, entity, buttonsIdx++, cursorPos);
        }
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

    MonitoringDebugWindow::getInstance()->updateSystem(_monitoringKey, timer.getElapsedTime(), nbEntities);
}

void    MenuSystem::onWindowResize(EntityManager &em)
{
    uint32_t    nbEntities = (uint32_t)_entities.size();

    for (uint32_t i = 0; i < nbEntities; ++i) {
        Entity* entity = em.getEntity(_entities[i]);

        handleAlignment(em, entity, i, true);
    }
}

void    MenuSystem::handleButtonMouseHover(EntityManager& em, Entity* entity, uint32_t entityIdx, const glm::vec2& cursorPos)
{
    sRenderComponent*       render = entity->getComponent<sRenderComponent>();
    sTransformComponent*    transform = entity->getComponent<sTransformComponent>();
    const glm::vec3&        size = render->_model->getSize() * transform->scale;

    // Check the mouse is in the button (2D AABB collision)
    if (cursorPos.x >= transform->pos.x &&
        cursorPos.x <= transform->pos.x + size.x &&
        cursorPos.y >= transform->pos.y &&
        cursorPos.y <= transform->pos.y + size.y)
    {
        removeSelected(em, _currentSelected);
        _currentSelected = entityIdx;
        setSelected(em, _currentSelected, true);
        _buttonHovered = true;
    }
}

void    MenuSystem::handleButtonsKeys(EntityManager &em)
{
    auto        &&keyboard = GameWindow::getInstance()->getKeyboard();
    bool        upPressed = keyboard.getStateMap()[Keyboard::eKey::UP] == Keyboard::eKeyState::KEY_PRESSED;
    bool        downPressed = keyboard.getStateMap()[Keyboard::eKey::DOWN] == Keyboard::eKeyState::KEY_PRESSED;

    if (_buttons.size() > 0 && _currentSelected != -1)
    {
        if (upPressed)
        {
            removeSelected(em, _currentSelected);
            _currentSelected -= 1;
            if (_currentSelected < 0)
                _currentSelected = (int)_buttons.size() - 1;
            setSelected(em, _currentSelected);
        }
        else if (downPressed)
        {
            removeSelected(em, _currentSelected);
            _currentSelected += 1;
            _currentSelected = _currentSelected % _buttons.size();
            setSelected(em, _currentSelected);
        }

    }
    // No button is currently selected, select default 0 when a key is pressed
    else if (_buttons.size() > 0 && _currentSelected == -1)
    {
        if (upPressed || downPressed)
        {
            _currentSelected = 0;
            setSelected(em, _currentSelected);
        }

    }
}

void    MenuSystem::handleAlignment(EntityManager& em, Entity* entity, uint32_t entityIdx, bool forceUpdate)
{
    sUiComponent* ui = entity->getComponent<sUiComponent>();

    if (ui && (ui->needUpdate || forceUpdate))
    {
        sRenderComponent* render = entity->getComponent<sRenderComponent>();
        sTransformComponent* transform = entity->getComponent<sTransformComponent>();

        // Init the model to retrieve the size
        if (!render->_model)
            render->initModel();

        const glm::vec3& size = render->_model->getSize() * transform->scale;
        float windowWidth = (float) GameWindow::getInstance()->getBufferWidth();
        float windowHeight = (float) GameWindow::getInstance()->getBufferHeight();

        // Horizontal alignments
        if (ui->horizontalAlignment == eHorizontalAlignment::LEFT)
        {
            transform->pos.x = 0;
        }
        else if (ui->horizontalAlignment == eHorizontalAlignment::MIDDLE)
        {
            transform->pos.x = (windowWidth / 2.0f) - (size.x / 2.0f);
        }
        else if (ui->horizontalAlignment == eHorizontalAlignment::RIGHT)
        {
            transform->pos.x = windowWidth - size.x;
        }

        // Vertical alignments
        if (ui->verticalAlignment == eVerticalAlignment::TOP)
        {
            transform->pos.y = 0;
        }
        else if (ui->verticalAlignment == eVerticalAlignment::MIDDLE)
        {
            transform->pos.y = (windowHeight / 2.0f) - (size.y / 2.0f);
        }
        else if (ui->verticalAlignment == eVerticalAlignment::BOTTOM)
        {
            transform->pos.y = windowHeight - size.y;
        }

        // Offsets
        transform->pos.x += windowWidth * ui->offset.x / 100.0f;
        transform->pos.y += windowHeight * ui->offset.y / 100.0f;
        transform->needUpdate = true;

        // Calculate new cursor position
        if (entityIdx == _currentSelected)
        {
            sButtonComponent* button = entity->getComponent<sButtonComponent>();
            button->selected = false; // Force to recalculate cursor position
            setSelected(entity);
        }
    }

    ui->needUpdate = false;
}

void    MenuSystem::setSelected(EntityManager &em, int buttonIdx, bool hovered)
{
    // Out of range
    if (buttonIdx < 0 || buttonIdx >= _buttons.size())
        return;

    Entity* entity = em.getEntity(_buttons[buttonIdx]);
    setSelected(entity, hovered);
}

void    MenuSystem::setSelected(Entity* entity, bool hovered)
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

    //  Move the "selected" icon next to the selected button.
    sTransformComponent*    iconTransform = _iconSelected->getComponent<sTransformComponent>();
    sTransformComponent*    buttonTransform = entity->getComponent<sTransformComponent>();

    iconTransform->pos.x = buttonTransform->pos.x - 42.0f;
    iconTransform->pos.y = buttonTransform->pos.y + 4.0f;
    iconTransform->needUpdate = true;

    button->selected = true;
}

void    MenuSystem::removeSelected(EntityManager &em, int buttonIdx)
{
    // Out of range
    if (buttonIdx < 0 || buttonIdx >= _buttons.size())
        return;

    Entity* entity = em.getEntity(_buttons[_currentSelected]);

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

void    MenuSystem::setupSelectedIcon()
{
    _iconSelected = EntityFactory::createEntity(eArchetype::ICON_SELECTED);
    _iconRender = _iconSelected->getComponent<sRenderComponent>();
    _iconRender->_display = false;
}

bool    MenuSystem::onEntityNewComponent(Entity* entity, sComponent* component)
{
    System::onEntityNewComponent(entity, component);

    if (component->id == sButtonComponent::identifier)
    {
        // The entity is not already in the buttons entities vector
        if (std::find(_buttons.cbegin(), _buttons.cend(), entity->id) == _buttons.cend())
        {
            _buttons.push_back(entity->id);
        }
        return (true);
    }
    return (false);
}

bool    MenuSystem::onEntityRemovedComponent(Entity* entity, sComponent* component)
{
    System::onEntityRemovedComponent(entity, component);

    if (component->id == sButtonComponent::identifier)
    {
        // The entity is in the button entities vector
        auto foundEntity = std::find(_buttons.cbegin(), _buttons.cend(), entity->id);
        if (foundEntity != _buttons.cend())
        {
            _buttons.erase(foundEntity);
            return (true);
        }
    }
    return (false);
}

bool    MenuSystem::onEntityDeleted(Entity* entity)
{
    System::onEntityDeleted(entity);

    auto foundEntity = std::find(_buttons.cbegin(), _buttons.cend(), entity->id);
    // The entity is in the system list
    if (foundEntity != _buttons.cend())
    {
        _buttons.erase(foundEntity);
        return (true);
    }
    return (false);
}
