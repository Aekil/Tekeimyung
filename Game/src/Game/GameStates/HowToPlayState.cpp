/**
* @Author   Guillaume Labey
*/

#include <iostream>

#include <Engine/Window/GameWindow.hpp>
#include <Engine/Utils/LogDebugWindow.hpp>

#include <Engine/Systems/RenderingSystem.hpp>
#include <Engine/Systems/MenuSystem.hpp>
#include <Engine/EntityDebugWindow.hpp>
#include <Engine/EntityFactory.hpp>
#include <Engine/Components.hh>

#include <Game/GameStates/HowToPlayState.hpp>


HowToPlayState::~HowToPlayState() {}

void    HowToPlayState::onEnter()
{
     EntityFactory::bindEntityManager(_world.getEntityManager());
}

bool    HowToPlayState::init()
{
    _world.addSystem<ParticleSystem>();
    _world.addSystem<RenderingSystem>(&_camera, nullptr, _world.getSystem<ParticleSystem>()->getEmitters());
    _world.addSystem<MenuSystem>();

    EntityManager* em = _world.getEntityManager();
    addDebugWindow<EntityDebugWindow>(em, nullptr, glm::vec2(0, 80), glm::vec2(600, 350));
    addDebugWindow<LogDebugWindow>(Logger::getInstance(), glm::vec2(0, 430), glm::vec2(300, 200));

    initCamera();
    initMenu();

    return (true);
}

bool    HowToPlayState::update(float elapsedTime)
{
    auto &&keyboard = GameWindow::getInstance()->getKeyboard();
    bool success = GameState::update(elapsedTime);

    handleButtons();

    // Quit the state
    if (keyboard.getStateMap()[Keyboard::eKey::ESCAPE] == Keyboard::eKeyState::KEY_PRESSED)
    {
        return (false);
    }

    return (success);
}

void    HowToPlayState::initCamera()
{
    auto gameWindow = GameWindow::getInstance();

    // Set camera screen
    Camera::sScreen screen;
    screen.right = (float)gameWindow->getBufferWidth();
    screen.left = 0;
    screen.top = (float)gameWindow->getBufferHeight();
    screen.bottom = 0;
    _camera.setScreen(screen);
    _camera.setProjType(Camera::eProj::ORTHOGRAPHIC_2D);
}

void    HowToPlayState::initMenu()
{
    Entity* background = EntityFactory::createEntity(eArchetype::BACKGROUND);
    Entity* menu = EntityFactory::createEntity(eArchetype::HOW_TO_PLAY_MENU);

    sRenderComponent* menuRender = menu->getComponent<sRenderComponent>();
    sTransformComponent* menuTransform = menu->getComponent<sTransformComponent>();

    // Menu content
    if (menuRender)
    {
        // Init the model to retrieve the size
        if (!menuRender->_model)
            menuRender->initModel();

        const glm::vec3& size = menuRender->_model->getSize() * menuTransform->scale;
        float windowWidth = (float)GameWindow::getInstance()->getBufferWidth();
        float windowHeight = (float)GameWindow::getInstance()->getBufferHeight();

        menuTransform->pos.x = (windowWidth / 2.0f) - (size.x / 2.0f);
        menuTransform->pos.y = (windowHeight / 2.0f) - (size.y / 2.0f);
        menuTransform->pos.z = -1.0f;
        menuTransform->needUpdate = true;
    }

    sRenderComponent* backgroundRender = background->getComponent<sRenderComponent>();
    sTransformComponent* backgroundTransform = background->getComponent<sTransformComponent>();

    // Background
    if (backgroundRender)
    {
        // Init the model to retrieve the size
        if (!backgroundRender->_model)
            backgroundRender->initModel();

        float windowWidth = (float)GameWindow::getInstance()->getBufferWidth();
        float windowHeight = (float)GameWindow::getInstance()->getBufferHeight();

        backgroundTransform->scale.x = windowWidth / SIZE_UNIT;
        backgroundTransform->scale.y = windowHeight / SIZE_UNIT;
        backgroundTransform->needUpdate = true;
    }

    _returnButton = createButton(eArchetype::BUTTON_RETURN, glm::vec2(0, -421.0f));
}

Entity* HowToPlayState::createButton(eArchetype type, const glm::vec2& pos)
{
    Entity* button = EntityFactory::createEntity(type);

    sTransformComponent* transform = button->getComponent<sTransformComponent>();
    transform->pos += glm::vec3(pos, 0.0f);
    transform->needUpdate = true;

    return (button);
}

void    HowToPlayState::handleButtons()
{
    auto &&keyboard = GameWindow::getInstance()->getKeyboard();
    auto &&mouse = GameWindow::getInstance()->getMouse();

    sButtonComponent* returnButton = _returnButton->getComponent<sButtonComponent>();

    bool spacebarPressed = keyboard.getStateMap()[Keyboard::eKey::ENTER] == Keyboard::eKeyState::KEY_PRESSED;
    bool mouseClicked = mouse.getStateMap()[Mouse::eButton::MOUSE_BUTTON_1] == Mouse::eButtonState::CLICK_PRESSED;

    //  "Return" button
    if ((spacebarPressed && returnButton->selected) ||
        (mouseClicked && returnButton->hovered))
    {
        _gameStateManager->removeCurrentState();
    }
}
