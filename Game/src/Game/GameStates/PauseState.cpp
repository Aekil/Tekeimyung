#include <iostream>

#include <Engine/Window/GameWindow.hpp>
#include <Engine/Utils/LogDebugWindow.hpp>

#include <Game/Systems/RenderingSystem.hpp>
#include <Game/Systems/MenuSystem.hpp>
#include <Game/EntityDebugWindow.hpp>
#include <Game/EntityFactory.hpp>
#include <Game/Components.hh>
#include <Game/GameStates/ConfirmExitState.hpp>
#include <Game/GameStates/HowToPlayState.hpp>
#include <Game/GameStates/OptionsMenuState.hpp>


#include <Game/GameStates/PauseState.hpp>


PauseState::~PauseState() {}

void    PauseState::onEnter()
{
     EntityFactory::bindEntityManager(_world.getEntityManager());
}

bool    PauseState::init()
{
    uint32_t statesNb = (uint32_t)_gameStateManager->getStates().size();

    ASSERT(statesNb >= 1, "The pause State should not be the first state");

    auto playState = _gameStateManager->getStates()[statesNb - 1];

    _playStateWorld = &playState->getWorld();
    _playStateRendersystem = _playStateWorld->getSystem<RenderingSystem>();

    _world.addSystem<ParticleSystem>();
    _world.addSystem<RenderingSystem>(&_camera, nullptr, _world.getSystem<ParticleSystem>()->getEmitters());
    initCamera();
    _world.addSystem<MenuSystem>();

    EntityManager* em = _world.getEntityManager();
    addDebugWindow<EntityDebugWindow>(em, nullptr, glm::vec2(0, 80), glm::vec2(600, 350));
    addDebugWindow<LogDebugWindow>(Logger::getInstance(), glm::vec2(0, 430), glm::vec2(300, 200));


    _resumeButton = createButton(eArchetype::BUTTON_RESUME, glm::vec2(0.0f, 240.0f));
    _howToPlayButton = createButton(eArchetype::BUTTON_HOW_TO_PLAY, glm::vec2(0.0f, 160.0f));
    _optionsButton = createButton(eArchetype::BUTTON_OPTIONS, glm::vec2(0.0f, 80.0f));
    _quitButton = createButton(eArchetype::BUTTON_QUIT, glm::vec2(0.0f, 0.0f));
    return (true);
}

bool    PauseState::update(float elapsedTime)
{
    auto &&keyboard = GameWindow::getInstance()->getKeyboard();

    // Display the play game state
    if (_playStateRendersystem)
        _playStateRendersystem->update(*_playStateWorld->getEntityManager(), 0);

    bool success = GameState::update(elapsedTime);

    handleButtons();

    // Unpause the game
    if (keyboard.getStateMap()[Keyboard::eKey::ESCAPE] == Keyboard::eKeyState::KEY_PRESSED)
    {
        return (false);
    }

    return (success);
}

void    PauseState::initCamera()
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

Entity* PauseState::createButton(eArchetype type, const glm::vec2& pos)
{
    Entity* button = EntityFactory::createEntity(type);

    sTransformComponent* transform = button->getComponent<sTransformComponent>();
    transform->pos += glm::vec3(pos, 0.0f);
    transform->needUpdate = true;

    return (button);
}

void    PauseState::handleButtons()
{
    auto &&keyboard = GameWindow::getInstance()->getKeyboard();
    auto &&mouse = GameWindow::getInstance()->getMouse();

    sButtonComponent* resume = _resumeButton->getComponent<sButtonComponent>();
    sButtonComponent* howToPlay = _howToPlayButton->getComponent<sButtonComponent>();
    sButtonComponent* options = _optionsButton->getComponent<sButtonComponent>();
    sButtonComponent* quit = _quitButton->getComponent<sButtonComponent>();

    ASSERT(resume != nullptr, "\"Resume Game\" button should have sButtonComponent");
    ASSERT(howToPlay != nullptr, "\"How to Play\" button should have sButtonComponent");
    ASSERT(options != nullptr, "\"Options\" button should have sButtonComponent");
    ASSERT(quit != nullptr, "\"Quit Game\" button should have sButtonComponent");

    bool spacebarPressed = keyboard.getStateMap()[Keyboard::eKey::SPACE] == Keyboard::eKeyState::KEY_PRESSED;
    bool mouseClicked = mouse.getStateMap()[Mouse::eButton::MOUSE_BUTTON_1] == Mouse::eButtonState::CLICK_PRESSED;
    // Resume button
    // Space bar pressed or mouse clicked
    if ((spacebarPressed && resume->selected) ||
        (mouseClicked && resume->hovered))
    {
        _gameStateManager->removeCurrentState();
    }
    // Quit button
    // Space bar pressed or mouse clicked
    else if ((spacebarPressed && quit->selected) ||
        (mouseClicked && quit->hovered))
    {
        _gameStateManager->addState<ConfirmExitState>();
    }
}
