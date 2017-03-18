/**
* @Author   Guillaume Labey
*/

#include <iostream>

#include <Engine/Window/GameWindow.hpp>

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
    return (true);
}

bool    HowToPlayState::update(float elapsedTime)
{
    auto &&keyboard = GameWindow::getInstance()->getKeyboard();
    bool success = GameState::update(elapsedTime);

    // Quit the state
    if (keyboard.getStateMap()[Keyboard::eKey::ESCAPE] == Keyboard::eKeyState::KEY_PRESSED)
    {
        return (false);
    }

    return (success);
}
