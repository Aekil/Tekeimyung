#include <Engine/Window/GameWindow.hpp>

#include <Game/Systems/RenderingSystem.hpp>
#include <Game/Systems/MenuSystem.hpp>
#include <Game/EntityDebugWindow.hpp>
#include <Game/EntityFactory.hpp>
#include <Game/Components.hh>

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

    initCamera();
    _world.addSystem<RenderingSystem>(&_camera, nullptr, nullptr);
    _world.addSystem<MenuSystem>();

    EntityManager* em = _world.getEntityManager();
    addDebugWindow<EntityDebugWindow>(em, nullptr, glm::vec2(0, 80), glm::vec2(600, 350));


    _resumeButton = createButton(eArchetype::BUTTON_RESUME, glm::vec2(0.0f, 80.0f));
    _quitButton = createButton(eArchetype::BUTTON_QUIT, glm::vec2(0.0f, 0.0f));
    return (true);
}

bool    PauseState::update(float elapsedTime)
{
    auto &&keyboard = GameWindow::getInstance()->getKeyboard();

    // Display the play game state
    if (_playStateRendersystem)
        _playStateRendersystem->update(*_playStateWorld->getEntityManager(), 0);

    // Disable depth test to display 2D
    glDisable(GL_DEPTH_TEST);

    bool success = GameState::update(elapsedTime);
    // Enable depth test to display 3D
    glEnable(GL_DEPTH_TEST);

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

    sButtonComponent* resume = _resumeButton->getComponent<sButtonComponent>();
    sButtonComponent* quit = _quitButton->getComponent<sButtonComponent>();

    // Space bar pressed, handle buttons action
    if (keyboard.getStateMap()[Keyboard::eKey::SPACE] == Keyboard::eKeyState::KEY_PRESSED)
    {
        // Resume button
        if (resume->selected)
        {
            _gameStateManager->removeCurrentState();
        }
        // Quit button
        else if (quit->selected)
        {
            _gameStateManager->clearStates();
        }
    }
}
