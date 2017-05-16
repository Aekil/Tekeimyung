/**
* @Author   Mathieu Chassara
*/

#include    <Engine/Core/Components/Components.hh>
#include    <Engine/EntityFactory.hpp>

#include    <Engine/Systems/RenderingSystem.hpp>
#include    <Engine/Systems/UISystem.hpp>
#include    <Engine/Systems/ButtonSystem.hpp>

#include    <Game/GameStates/VictoryScreenState.hpp>

VictoryScreenState::~VictoryScreenState() {}

void    VictoryScreenState::onEnter()
{
    sRenderComponent*   renderComponent = this->_buttonTryAgainGreen->getComponent<sRenderComponent>();

    renderComponent->_animator.play("fading", true);
}

void    VictoryScreenState::setupSystems()
{
    _world.addSystem<ParticleSystem>();
    _world.addSystem<UISystem>();
    _world.addSystem<ButtonSystem>(_gameStateManager);
    _world.addSystem<RenderingSystem>(_world.getSystem<ParticleSystem>()->getEmitters());
}

bool    VictoryScreenState::init()
{
    //  Retrieving the "How to Play" button in order to play its animation.
    this->_buttonTryAgainGreen = EntityFactory::createOrGetEntity("BUTTON_TRY_AGAIN_GREEN");
    return (true);
}