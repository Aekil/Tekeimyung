#include <iostream>
#include <Engine/Window/GameWindow.hpp>

#include <Game/Systems/RenderingSystem.hpp>
#include <Game/EntityFactory.hpp>

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

    _world.addSystem<RenderingSystem>(nullptr, nullptr);
    return (true);
}

bool    PauseState::update(float elapsedTime)
{
    auto &&keyboard = GameWindow::getInstance()->getKeyboard();

    if (keyboard.getStateMap()[Keyboard::eKey::ESCAPE] == Keyboard::eKeyState::KEY_PRESSED)
    {
        _gameStateManager->removeCurrentState();
    }

    if (_playStateRendersystem)
        _playStateRendersystem->update(*_playStateWorld->getEntityManager(), 0);

    return (GameState::update(elapsedTime));
}
