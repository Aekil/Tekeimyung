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

bool    ConfirmExitState::init()
{
    initPreviousStatesRender();

    _world.addSystem<ParticleSystem>();
    _world.addSystem<RenderingSystem>(&_camera, _world.getSystem<ParticleSystem>()->getEmitters());
    _world.addSystem<MenuSystem>();

    EntityFactory::createEntity(eArchetype::QUIT_CONFIRM_POPUP);
    EntityFactory::createEntity(eArchetype::QUIT_CONFIRM_TEXT);
    _noButton = EntityFactory::createEntity(eArchetype::BUTTON_CONFIRM_NO);
    _yesButton = EntityFactory::createEntity(eArchetype::BUTTON_CONFIRM_YES);

    return (true);
}

bool    ConfirmExitState::update(float elapsedTime)
{
    auto &&keyboard = GameWindow::getInstance()->getKeyboard();

    // Display the previous states
    for (int i = (int)_previousStatesSystems.size() - 1; i >= 0; --i)
    {
        _previousStatesSystems[i]->update(*_previousStatesWorlds[i]->getEntityManager(), 0);
    }

    bool success = GameState::update(elapsedTime);

    handleButtons();

    // Unpause the game
    if (keyboard.getStateMap()[Keyboard::eKey::ESCAPE] == Keyboard::eKeyState::KEY_PRESSED)
    {
        return (false);
    }

    return (success);
}

void    ConfirmExitState::initPreviousStatesRender()
{
    uint32_t statesNb = (uint32_t)_gameStateManager->getStates().size();

    ASSERT(statesNb >= 1, "The ConfirmExitState should not be the first state");

    auto previousState = _gameStateManager->getStates()[statesNb - 1];
    World* previousStateWorld = &previousState->getWorld();
    System* previousStateRendersystem = previousStateWorld->getSystem<RenderingSystem>();

    _previousStatesSystems.push_back(previousStateRendersystem);
    _previousStatesWorlds.push_back(previousStateWorld);

    // Add playState render
    if (previousState->getId() == PauseState::identifier)
    {
        ASSERT(statesNb >= 2, "The Pause State should not be the first state");

        auto playState = _gameStateManager->getStates()[statesNb - 2];
        World* playStateWorld = &playState->getWorld();
        System* playStateRendersystem = playStateWorld->getSystem<RenderingSystem>();

        _previousStatesSystems.push_back(playStateRendersystem);
        _previousStatesWorlds.push_back(playStateWorld);
    }
}

void    ConfirmExitState::handleButtons()
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
        _gameStateManager->removeCurrentState();
    }
}
