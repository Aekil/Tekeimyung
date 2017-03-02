/**
* @Author   Guillaume Labey
*/

#include <iostream>

#include <Engine/Window/GameWindow.hpp>
#include <Engine/Sound/SoundManager.hpp>

#include <Engine/Systems/RenderingSystem.hpp>
#include <Engine/Systems/UISystem.hpp>
#include <Engine/Systems/ButtonSystem.hpp>
#include <Engine/EntityFactory.hpp>
#include <Engine/Components.hh>
#include <Game/GameStates/ConfirmExitState.hpp>
#include <Game/GameStates/HowToPlayState.hpp>
#include <Game/GameStates/OptionsMenuState.hpp>


#include <Game/GameStates/PauseState.hpp>
#include <Game/GameStates/PlayState.hpp>


PauseState::~PauseState() {}

void    PauseState::onEnter() {}

void    PauseState::setupSystems()
{
    _world.addSystem<ParticleSystem>();
    _world.addSystem<UISystem>();
    _world.addSystem<ButtonSystem>(_gameStateManager);
    _world.addSystem<RenderingSystem>(_world.getSystem<ParticleSystem>()->getEmitters());
}

bool    PauseState::init()
{
    SoundManager::getInstance()->setVolume(0.3f);
    return (true);
}

bool    PauseState::update(float elapsedTime)
{
    auto &&keyboard = GameWindow::getInstance()->getKeyboard();

    // Display the previous states
    renderPreviousStates({PlayState::identifier});

    bool success = GameState::update(elapsedTime);

    // Unpause the game
    if (keyboard.getStateMap()[Keyboard::eKey::ESCAPE] == Keyboard::eKeyState::KEY_PRESSED)
    {
        SoundManager::getInstance()->setVolume(1.0f);
        return (false);
    }

    return (success);
}
