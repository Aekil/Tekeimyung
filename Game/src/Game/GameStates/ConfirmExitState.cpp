/**
* @Author   Guillaume Labey
*/

#include <iostream>
#include <Engine/Window/GameWindow.hpp>

#include <Engine/Systems/RenderingSystem.hpp>
#include <Engine/Systems/UISystem.hpp>
#include <Engine/Systems/ButtonSystem.hpp>
#include <Engine/EntityFactory.hpp>
#include <Engine/Core/Components/Components.hh>
#include <Game/GameStates/PauseState.hpp>

#include <Game/GameStates/ConfirmExitState.hpp>


ConfirmExitState::~ConfirmExitState() {}

void    ConfirmExitState::onEnter() {}

void    ConfirmExitState::setupSystems()
{
    _world.addSystem<ParticleSystem>();
    _world.addSystem<UISystem>();
    _world.addSystem<ButtonSystem>(_gameStateManager);
    _world.addSystem<RenderingSystem>(_world.getSystem<ParticleSystem>()->getEmitters());
}

bool    ConfirmExitState::init()
{
    _yesButton = EntityFactory::createOrGetEntity("BUTTON_CONFIRM_YES");

    return (true);
}

bool    ConfirmExitState::update(float elapsedTime)
{
    auto &&keyboard = GameWindow::getInstance()->getKeyboard();

    // Display the previous states
    renderPreviousStates();

    bool success = GameState::update(elapsedTime);

    // Quit the state
    if (!handleButtons() ||
        keyboard.getStateMap()[Keyboard::eKey::ESCAPE] == Keyboard::eKeyState::KEY_PRESSED)
    {
        return (false);
    }

    return (success);
}

bool    ConfirmExitState::handleButtons()
{
    auto &&keyboard = GameWindow::getInstance()->getKeyboard();
    auto &&mouse = GameWindow::getInstance()->getMouse();

    sButtonComponent* yes = _yesButton->getComponent<sButtonComponent>();

    bool enterPressed = keyboard.getStateMap()[Keyboard::eKey::ENTER] == Keyboard::eKeyState::KEY_PRESSED;
    bool mouseClicked = mouse.getStateMap()[Mouse::eButton::MOUSE_BUTTON_1] == Mouse::eButtonState::CLICK_PRESSED;

    // Yes button
    // Space bar pressed or mouse clicked
    if ((enterPressed && yes->selected) ||
        (mouseClicked && yes->hovered))
    {
        _gameStateManager->clearStates();
    }

    return (true);
}
