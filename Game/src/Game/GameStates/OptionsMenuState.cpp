/**
* @Author   Mathieu Chassara
*/

#include    <iostream>
#include    <Engine/Window/GameWindow.hpp>

#include    <Engine/Systems/RenderingSystem.hpp>
#include    <Engine/Systems/UISystem.hpp>
#include    <Engine/Systems/ButtonSystem.hpp>
#include    <Engine/Components.hh>

#include    <Game/GameStates/OptionsMenuState.hpp>
#include    <Game/GameStates/PlayState.hpp>

OptionsMenuState::~OptionsMenuState() {}

void    OptionsMenuState::onEnter() {}

void    OptionsMenuState::setupSystems()
{
    _world.addSystem<ParticleSystem>();
    _world.addSystem<UISystem>();
    _world.addSystem<ButtonSystem>(_gameStateManager);
    _world.addSystem<RenderingSystem>(_world.getSystem<ParticleSystem>()->getEmitters());
}

bool            OptionsMenuState::init()
{
    createToggleWindowModeButton();
    return (true);
}

bool        OptionsMenuState::update(float elapsedTime)
{
    auto    &&keyboard = GameWindow::getInstance()->getKeyboard();

    // Display the previous states
    renderPreviousStates({PlayState::identifier});

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

    ASSERT(toggleWindowMode != nullptr, "\"Toggle window mode\" button should have a sButtonComponent.");

    bool    spacebarPressed = keyboard.getStateMap()[Keyboard::eKey::ENTER] == Keyboard::eKeyState::KEY_PRESSED;
    bool    mouseClicked = mouse.getStateMap()[Mouse::eButton::MOUSE_BUTTON_1] == Mouse::eButtonState::CLICK_PRESSED;

    //  "Toggle fullscreen / windowed mode" button
    if ((spacebarPressed && toggleWindowMode->selected) ||
        (mouseClicked && toggleWindowMode->hovered))
    {
        GameWindow::getInstance()->toggleFullscreen();
        createToggleWindowModeButton();
    }

    return (true);
}

void                    OptionsMenuState::createToggleWindowModeButton()
{
    std::string          buttonArchetype;

   /* if (_toggleWindowModeButton != nullptr)
        _world.getEntityManager()->destroyEntity(_toggleWindowModeButton);*/
    buttonArchetype = (GameWindow::getInstance()->isFullscreen() ?
                       "BUTTON_TOGGLE_WINDOWED" :
                       "BUTTON_TOGGLE_FULLSCREEN");

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
