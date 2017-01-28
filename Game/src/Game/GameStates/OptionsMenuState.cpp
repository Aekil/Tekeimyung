/**
* @Author   Mathieu Chassara
*/

#include    <iostream>
#include    <Engine/Window/GameWindow.hpp>

#include    <Engine/Systems/RenderingSystem.hpp>
#include    <Engine/Systems/MenuSystem.hpp>
#include    <Engine/Components.hh>

#include    <Game/GameStates/OptionsMenuState.hpp>

OptionsMenuState::~OptionsMenuState() {}

void    OptionsMenuState::onEnter() {}

void    OptionsMenuState::setupSystems()
{
    _world.addSystem<ParticleSystem>();
    _world.addSystem<RenderingSystem>(&_camera, _world.getSystem<ParticleSystem>()->getEmitters());
    _world.addSystem<MenuSystem>();
}

bool            OptionsMenuState::init()
{
    uint32_t    statesNb = (uint32_t) _gameStateManager->getStates().size();

    ASSERT(statesNb >= 2, "The Options menu state should not be the second state.");

    auto        playState = _gameStateManager->getStates()[statesNb - 2];

    _playstateWorld = &playState->getWorld();
    _playstateRenderSystem = _playstateWorld->getSystem<RenderingSystem>();

    createToggleWindowModeButton();
    _returnButton = EntityFactory::createOrGetEntity(eArchetype::BUTTON_RETURN);

    return (true);
}

bool        OptionsMenuState::update(float elapsedTime)
{
    auto    &&keyboard = GameWindow::getInstance()->getKeyboard();

    // Display the play game state
    if (_playstateRenderSystem)
        _playstateRenderSystem->update(*_playstateWorld->getEntityManager(), 0);

    bool    success = GameState::update(elapsedTime);

    // Quit the state
    if (!handleButtons() ||
        keyboard.getStateMap()[Keyboard::eKey::ESCAPE] == Keyboard::eKeyState::KEY_PRESSED)
    {
        return (false);
    }

    return (success);
}

bool                    OptionsMenuState::handleButtons()
{
    auto                &&keyboard = GameWindow::getInstance()->getKeyboard();
    auto                &&mouse = GameWindow::getInstance()->getMouse();

    sButtonComponent*   toggleWindowMode = _toggleWindowModeButton->getComponent<sButtonComponent>();
    sButtonComponent*   returnButton = _returnButton->getComponent<sButtonComponent>();

    ASSERT(toggleWindowMode != nullptr, "\"Toggle window mode\" button should have a sButtonComponent.");
    ASSERT(returnButton != nullptr, "\"Return\" button should have a sButtonComponent.");

    bool    spacebarPressed = keyboard.getStateMap()[Keyboard::eKey::ENTER] == Keyboard::eKeyState::KEY_PRESSED;
    bool    mouseClicked = mouse.getStateMap()[Mouse::eButton::MOUSE_BUTTON_1] == Mouse::eButtonState::CLICK_PRESSED;

    //  "Toggle fullscreen / windowed mode" button
    if ((spacebarPressed && toggleWindowMode->selected) ||
        (mouseClicked && toggleWindowMode->hovered))
    {
        GameWindow::getInstance()->toggleFullscreen();
        createToggleWindowModeButton();
    }

    //  "Return" button
    else if ((spacebarPressed && returnButton->selected) ||
        (mouseClicked && returnButton->hovered))
    {
        return (false);
    }

    return (true);
}

void                    OptionsMenuState::createToggleWindowModeButton()
{
    eArchetype          buttonArchetype;

   /* if (_toggleWindowModeButton != nullptr)
        _world.getEntityManager()->destroyEntity(_toggleWindowModeButton);*/
    buttonArchetype = (GameWindow::getInstance()->isFullscreen() ?
                       eArchetype::BUTTON_TOGGLE_WINDOWED :
                       eArchetype::BUTTON_TOGGLE_FULLSCREEN);

    // Create the button for the first time
    if (!_toggleWindowModeButton)
    {
        _toggleWindowModeButton = EntityFactory::createOrGetEntity(buttonArchetype);
    }
    // Change the button render
    else
    {
        Entity* newButton = EntityFactory::createEntity(buttonArchetype);
        sRenderComponent* newButtonRender = newButton->getComponent<sRenderComponent>();
        sRenderComponent* toggleWindowModeButtonRender = _toggleWindowModeButton->getComponent<sRenderComponent>();

        toggleWindowModeButtonRender->update(newButtonRender);
        _world.getEntityManager()->destroyEntity(newButton);
    }
}
