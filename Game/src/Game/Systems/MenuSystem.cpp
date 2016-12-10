#include <algorithm>

#include <Engine/Window/GameWindow.hpp>
#include <Engine/Utils/Logger.hpp>

#include <Game/Components.hh>

#include <Game/Systems/MenuSystem.hpp>

MenuSystem::MenuSystem()
{
    addDependency<sRenderComponent>();
    addDependency<sButtonComponent>();
    addDependency<sTransformComponent>();

    _monitoringKey = MonitoringDebugWindow::getInstance()->registerSystem(MENU_SYSTEM_NAME);

    _currentSelected = 0;
    _buttonHovered = false;
}

MenuSystem::~MenuSystem() {}

bool    MenuSystem::init()
{
    return (true);
}

void    MenuSystem::update(EntityManager &em, float elapsedTime)
{
    Timer timer;
    uint32_t nbEntities = 0;
    auto &&keyboard = GameWindow::getInstance()->getKeyboard();

    bool upPressed = keyboard.getStateMap()[Keyboard::eKey::UP] == Keyboard::eKeyState::KEY_PRESSED;
    bool downPressed = keyboard.getStateMap()[Keyboard::eKey::DOWN] == Keyboard::eKeyState::KEY_PRESSED;

    handleMouseHover(em);

    if (_entities.size() > 0 && _currentSelected != -1)
    {
        if (upPressed)
        {
            removeSelected(em, _currentSelected);
            _currentSelected -= 1;
            if (_currentSelected < 0)
                _currentSelected = (int)_entities.size() - 1;
            setSelected(em, _currentSelected);
        }
        else if (downPressed)
        {
            removeSelected(em, _currentSelected);
            _currentSelected += 1;
            _currentSelected = _currentSelected % _entities.size();
            setSelected(em, _currentSelected);
        }

    }
    // No button is currently selected, select default 0 when a key is pressed
    else if (_entities.size() > 0 && _currentSelected == -1)
    {
        if (upPressed || downPressed)
        {
            _currentSelected = 0;
            setSelected(em, _currentSelected);
        }

    }

    MonitoringDebugWindow::getInstance()->updateSystem(_monitoringKey, timer.getElapsedTime(), nbEntities);
}

void    MenuSystem::handleMouseHover(EntityManager &em)
{
    float windowHeight = (float)GameWindow::getInstance()->getBufferHeight();
    auto &&cursor = GameWindow::getInstance()->getMouse().getCursor();
    uint32_t nbEntities = (uint32_t)_entities.size();

    glm::vec2 cursorPos = glm::vec2(cursor.getX(), windowHeight - cursor.getY());

    for (uint32_t i = 0; i < nbEntities; ++i) {
        Entity* entity = em.getEntity(_entities[i]);

        sRenderComponent* render = entity->getComponent<sRenderComponent>();
        sTransformComponent* transform = entity->getComponent<sTransformComponent>();
        const glm::vec3& size = render->_model->getSize() * transform->scale;

        // Check the mouse is in the button (2D AABB collision)
        if (cursorPos.x >= transform->pos.x &&
            cursorPos.x <= transform->pos.x + size.x &&
            cursorPos.y >= transform->pos.y &&
            cursorPos.y <= transform->pos.y + size.y)
        {
            removeSelected(em, _currentSelected);
            _currentSelected = i;
            setSelected(em, _currentSelected, true);
            _buttonHovered = true;
            return;
        }
    }

    // Remove button selection if the button was hovered by the mouse
    // But now is not hovered
    if (_buttonHovered)
    {
        _buttonHovered = false;
        removeSelected(em, _currentSelected);
        _currentSelected = -1;
    }
}

void    MenuSystem::setSelected(EntityManager &em, int buttonIdx, bool hovered)
{
    // Out of range
    if (buttonIdx < 0 || buttonIdx >= _entities.size())
        return;

    Entity* entity = em.getEntity(_entities[_currentSelected]);
    setSelected(entity, hovered);
}

void    MenuSystem::setSelected(Entity* entity, bool hovered)
{
    // The entity does not exist
    if (!entity)
        return;

    sButtonComponent* button = entity->getComponent<sButtonComponent>();
    button->hovered = hovered;

    // The button is already selected
    if (button->selected)
        return;

    // Set new color of selected button
    sRenderComponent* render = entity->getComponent<sRenderComponent>();
    render->color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
    button->selected = true;
}

void    MenuSystem::removeSelected(EntityManager &em, int buttonIdx)
{
    // Out of range
    if (buttonIdx < 0 || buttonIdx >= _entities.size())
        return;

    Entity* entity = em.getEntity(_entities[_currentSelected]);

    // The entity does not exist
    if (!entity)
        return;

    sButtonComponent* button = entity->getComponent<sButtonComponent>();
    button->hovered = false;

    // The button is not selected
    if (!button->selected)
        return;

    // Reset button color
    sRenderComponent* render = entity->getComponent<sRenderComponent>();
    render->color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    button->selected = false;
}

bool    MenuSystem::onEntityNewComponent(Entity* entity, sComponent* component)
{
    // An entity has been added to the system
    // Init the button position
    if (System::onEntityNewComponent(entity, component))
    {
        sRenderComponent* render = entity->getComponent<sRenderComponent>();

        // Init the model to retrieve the size
        if (!render->_model)
            render->initModel();

        sTransformComponent* transform = entity->getComponent<sTransformComponent>();
        const glm::vec3& size = render->_model->getSize() * transform->scale;
        float windowWidth = (float)GameWindow::getInstance()->getBufferWidth();
        float windowHeight = (float)GameWindow::getInstance()->getBufferHeight();

        transform->pos.x = (windowWidth / 2.0f) - (size.x / 2.0f);
        transform->pos.y = (windowHeight / 2.0f) - (size.y / 2.0f);
        transform->pos.z = -1.0f;
        transform->needUpdate = true;

        // Select by default the first button
        if (_entities.size() == 1)
            setSelected(entity);

        return (true);
    }
    return (false);
}
