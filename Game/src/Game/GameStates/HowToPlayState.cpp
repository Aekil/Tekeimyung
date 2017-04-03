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

#include <Game/GameStates/HowToPlayState.hpp>


HowToPlayState::~HowToPlayState() {}

void    HowToPlayState::onEnter() {}

void    HowToPlayState::setupSystems()
{
    _world.addSystem<ParticleSystem>();
    _world.addSystem<UISystem>();
    _world.addSystem<ButtonSystem>(_gameStateManager);
    _world.addSystem<RenderingSystem>(_world.getSystem<ParticleSystem>()->getEmitters());
}

bool    HowToPlayState::init()
{
    SoundManager::getInstance()->setVolume(0.3f); // same as for PauseState since it pause the game
    return (true);
}

bool    HowToPlayState::update(float elapsedTime)
{
    auto &&keyboard = GameWindow::getInstance()->getKeyboard();
    bool success = GameState::update(elapsedTime);

    // Quit the state
    if (keyboard.getStateMap()[Keyboard::eKey::ESCAPE] == Keyboard::eKeyState::KEY_PRESSED ||
        keyboard.getStateMap()[Keyboard::eKey::H] == Keyboard::eKeyState::KEY_PRESSED)
    {
        SoundManager::getInstance()->setVolume(1.0f);
        return (false);
    }

    return (success);
}
