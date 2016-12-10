#include    <iostream>
#include    <Engine/Window/GameWindow.hpp>

#include    <Game/Systems/RenderingSystem.hpp>
#include    <Game/Systems/MenuSystem.hpp>
#include    <Game/EntityDebugWindow.hpp>
#include    <Game/Components.hh>

#include    <Game/GameStates/OptionsMenuState.hpp>

OptionsMenuState::~OptionsMenuState() {}

void            OptionsMenuState::onEnter()
{
    EntityFactory::bindEntityManager(_world.getEntityManager());
}

bool            OptionsMenuState::init()
{
    uint32_t    statesNb = (uint32_t) _gameStateManager->getStates().size();

    ASSERT(statesNb >= 2, "The Options menu state should not be the second state.");

    auto        playState = _gameStateManager->getStates()[statesNb - 2];

    _playstateWorld = &playState->getWorld();
    _playstateRenderSystem = _playstateWorld->getSystem<RenderingSystem>();

    _world.addSystem<ParticleSystem>();
    _world.addSystem<RenderingSystem>(&_camera, nullptr, _world.getSystem<ParticleSystem>()->getEmitters());
    _world.addSystem<MenuSystem>();

    EntityManager* em = _world.getEntityManager();
    addDebugWindow<EntityDebugWindow>(em, nullptr, glm::vec2(0, 80), glm::vec2(600, 350));

    initCamera();

    createToggleWindowModeButton();
    _returnButton = createButton(eArchetype::BUTTON_RETURN, glm::vec2(0.0f, 0.0f));
    return (true);
}

bool        OptionsMenuState::update(float elapsedTime)
{
    auto    &&keyboard = GameWindow::getInstance()->getKeyboard();

    // Display the play game state
    if (_playstateRenderSystem)
        _playstateRenderSystem->update(*_playstateWorld->getEntityManager(), 0);

    // Disable depth test to display 2D
    glDisable(GL_DEPTH_TEST);

    bool    success = GameState::update(elapsedTime);
    // Enable depth test to display 3D
    glEnable(GL_DEPTH_TEST);

    handleButtons();

    // Unpause the game
    if (keyboard.getStateMap()[Keyboard::eKey::ESCAPE] == Keyboard::eKeyState::KEY_PRESSED)
    {
        _gameStateManager->removeCurrentState();
        return (false);
    }

    return (success);
}

void        OptionsMenuState::initCamera()
{
    auto    gameWindow = GameWindow::getInstance();

    // Set camera screen
    Camera::sScreen screen;

    screen.right = (float) gameWindow->getBufferWidth();
    screen.left = 0;
    screen.top = (float) gameWindow->getBufferHeight();
    screen.bottom = 0;
    _camera.setScreen(screen);
    _camera.setProjType(Camera::eProj::ORTHOGRAPHIC_2D);
}

Entity*                     OptionsMenuState::createButton(eArchetype type, const glm::vec2& pos)
{
    Entity*                 button;
    sTransformComponent*    transform;

    button = EntityFactory::createEntity(type);
    transform = button->getComponent<sTransformComponent>();
    transform->pos += glm::vec3(pos, 0.0f);
    transform->needUpdate = true;

    return (button);
}

void                    OptionsMenuState::handleButtons()
{
    auto                &&keyboard = GameWindow::getInstance()->getKeyboard();
    sButtonComponent*   toggleWindowMode = _toggleWindowModeButton->getComponent<sButtonComponent>();
    sButtonComponent*   returnButton = _returnButton->getComponent<sButtonComponent>();

    ASSERT(toggleWindowMode != nullptr, "\"Toggle window mode\" button should have a sButtonComponent.");
    ASSERT(returnButton != nullptr, "\"Return\" button should have a sButtonComponent.");

    //  Space bar pressed, handle buttons action
    if (keyboard.getStateMap()[Keyboard::eKey::ENTER] == Keyboard::eKeyState::KEY_PRESSED)
    {
        //  Quit button
        if (toggleWindowMode->selected)
        {
            GameWindow::getInstance()->toggleFullscreen();
            createToggleWindowModeButton();
        }
        //  Resume button
        else if (returnButton->selected)
        {
            _gameStateManager->removeCurrentState();
        }
    }
}

void                OptionsMenuState::createToggleWindowModeButton()
{
    eArchetype      buttonArchetype;

    //if (_toggleWindowModeButton != nullptr)
        //_world.getEntityManager()->destroyEntity(_toggleWindowModeButton);
    buttonArchetype = (GameWindow::getInstance()->isFullscreen() ?
                       eArchetype::BUTTON_TOGGLE_WINDOWED :
                       eArchetype::BUTTON_TOGGLE_FULLSCREEN);
    _toggleWindowModeButton = createButton(buttonArchetype, glm::vec2(0.0f, 80.0f));
}