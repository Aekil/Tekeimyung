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
}

MenuSystem::~MenuSystem() {}

bool    MenuSystem::init()
{
    _currentSelected = 0;
    return (true);
}

void    MenuSystem::update(EntityManager &em, float elapsedTime)
{
    Timer timer;
    uint32_t nbEntities = 0;
    auto &&keyboard = GameWindow::getInstance()->getKeyboard();

    bool upPressed = keyboard.getStateMap()[Keyboard::eKey::UP] == Keyboard::eKeyState::KEY_PRESSED;
    bool downPressed = keyboard.getStateMap()[Keyboard::eKey::DOWN] == Keyboard::eKeyState::KEY_PRESSED;

    if (_entities.size() > 0)
    {
        if (upPressed)
        {
            removeSelected(em.getEntity(_entities[_currentSelected]));
            _currentSelected -= 1;
            if (_currentSelected < 0)
                _currentSelected = (int)_entities.size() - 1;
        }
        else if (downPressed)
        {
            removeSelected(em.getEntity(_entities[_currentSelected]));
            _currentSelected += 1;
            _currentSelected = _currentSelected % _entities.size();
        }

        setSelected(em.getEntity(_entities[_currentSelected]));
    }


    MonitoringDebugWindow::getInstance()->updateSystem(_monitoringKey, timer.getElapsedTime(), nbEntities);
}

void    MenuSystem::setSelected(Entity* entity)
{
    // The entity does not exist
    if (!entity)
        return;

    sButtonComponent* button = entity->getComponent<sButtonComponent>();

    // The button is already selected
    if (button->selected)
        return;

    // Set new color of selected button
    sRenderComponent* render = entity->getComponent<sRenderComponent>();
    render->color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
    button->selected = true;
}

void    MenuSystem::removeSelected(Entity* entity)
{
    // The entity does not exist
    if (!entity)
        return;

    sButtonComponent* button = entity->getComponent<sButtonComponent>();

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
        return (true);
    }
    return (false);
}
