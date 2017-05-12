/**
* @Author   Julien Chardon
*/

#include <iostream>

#include <Engine/Window/GameWindow.hpp>
#include <Engine/Sound/SoundManager.hpp>

#include <Engine/Systems/RenderingSystem.hpp>
#include <Engine/Systems/UISystem.hpp>
#include <Engine/Systems/ButtonSystem.hpp>
#include <Engine/EntityFactory.hpp>
#include <Engine/Core/Components/Components.hh>

#include <Game/GameStates/BuildingListState.hpp>


BuildingListState::~BuildingListState() {}

void    BuildingListState::onEnter() {}

void    BuildingListState::setupSystems()
{
    _world.addSystem<ParticleSystem>();
    _world.addSystem<UISystem>();
    _world.addSystem<ButtonSystem>(_gameStateManager);
    _world.addSystem<RenderingSystem>(_world.getSystem<ParticleSystem>()->getEmitters());
}

bool    BuildingListState::init()
{
    SoundManager::getInstance()->setVolumeAllChannels(0.3f); // same as for PauseState since it pause the game
    return (true);
}

bool    BuildingListState::update(float elapsedTime)
{
    auto &&keyboard = GameWindow::getInstance()->getKeyboard();
    bool success = GameState::update(elapsedTime);

    // Quit the state
    if (keyboard.getStateMap()[Keyboard::eKey::ESCAPE] == Keyboard::eKeyState::KEY_PRESSED ||
        keyboard.getStateMap()[Keyboard::eKey::B] == Keyboard::eKeyState::KEY_PRESSED)
    {
        SoundManager::getInstance()->setVolumeAllChannels(1.0f);
        return (false);
    }

    return (success);
}
