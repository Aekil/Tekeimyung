#include <Engine/Window/GameWindow.hpp>

#include <Game/GameStates/PauseState.hpp>


PauseState::~PauseState() {}

bool    PauseState::init()
{
    return (true);
}

bool    PauseState::update(float elapsedTime)
{
    auto &&keyboard = GameWindow::getInstance()->getKeyboard();

    if (keyboard.getStateMap()[Keyboard::eKey::ESCAPE] == Keyboard::eKeyState::KEY_PRESSED)
    {
        _gameStateManager->removeCurrentState();
    }

    return (GameState::update(elapsedTime));
}
