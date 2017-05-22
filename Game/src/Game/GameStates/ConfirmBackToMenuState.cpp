/**
* @Author   Guillaume Labey
*/

#include <Engine/Systems/RenderingSystem.hpp>
#include <Engine/Systems/UISystem.hpp>
#include <Engine/Systems/ButtonSystem.hpp>

#include <Game/GameStates/ConfirmBackToMenuState.hpp>


ConfirmBackToMenuState::~ConfirmBackToMenuState() {}

void    ConfirmBackToMenuState::onEnter() {}

void    ConfirmBackToMenuState::setupSystems()
{
    _world.addSystem<ParticleSystem>();
    _world.addSystem<UISystem>();
    _world.addSystem<ButtonSystem>(_gameStateManager);
    _world.addSystem<RenderingSystem>(_world.getSystem<ParticleSystem>()->getEmitters());
}

bool    ConfirmBackToMenuState::init()
{
    return (true);
}

bool    ConfirmBackToMenuState::update(float elapsedTime)
{
    // Display the previous states
    renderPreviousStates();

    return (GameState::update(elapsedTime));
}
