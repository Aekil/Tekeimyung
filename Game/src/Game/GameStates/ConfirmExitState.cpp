/**
* @Author   Guillaume Labey
*/

#include <iostream>
#include <Engine/Window/GameWindow.hpp>

#include <Engine/Systems/RenderingSystem.hpp>
#include <Engine/Systems/MenuSystem.hpp>
#include <Engine/EntityFactory.hpp>
#include <Engine/Components.hh>
#include <Game/GameStates/PauseState.hpp>

#include <Game/GameStates/ConfirmExitState.hpp>


ConfirmExitState::~ConfirmExitState() {}

void    ConfirmExitState::onEnter() {}

void    ConfirmExitState::setupSystems()
{
    _world.addSystem<ParticleSystem>();
    _world.addSystem<RenderingSystem>(&_camera, _world.getSystem<ParticleSystem>()->getEmitters());
    _world.addSystem<MenuSystem>();
}

bool    ConfirmExitState::init()
{
    _noButton = EntityFactory::createOrGetEntity(eArchetype::BUTTON_CONFIRM_NO);
    _yesButton = EntityFactory::createOrGetEntity(eArchetype::BUTTON_CONFIRM_YES);

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
    sButtonComponent* no = _noButton->getComponent<sButtonComponent>();

    bool enterPressed = keyboard.getStateMap()[Keyboard::eKey::ENTER] == Keyboard::eKeyState::KEY_PRESSED;
    bool mouseClicked = mouse.getStateMap()[Mouse::eButton::MOUSE_BUTTON_1] == Mouse::eButtonState::CLICK_PRESSED;

    // Yes button
    // Space bar pressed or mouse clicked
    if ((enterPressed && yes->selected) ||
        (mouseClicked && yes->hovered))
    {
        _gameStateManager->clearStates();
    }

    // No button
    // Space bar pressed or mouse clicked
    else if ((enterPressed && no->selected) ||
        (mouseClicked && no->hovered))
    {
        return (false);
    }

    return (true);
}
