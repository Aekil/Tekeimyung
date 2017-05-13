/**
* @Author   Mathieu Chassara
*/

#include    <Engine/Core/Components/RenderComponent.hh>
#include    <Engine/EntityFactory.hpp>
#include    <Engine/Systems/ButtonSystem.hpp>
#include    <Engine/Systems/RenderingSystem.hpp>
#include    <Engine/Systems/UISystem.hpp>

#include    <Game/GameStates/HomeScreenState.hpp>

HomeScreenState::~HomeScreenState() {}

void    HomeScreenState::onEnter()
{
    sRenderComponent*   renderComponent = this->_buttonHowToPlay->getComponent<sRenderComponent>();

    renderComponent->_animator.play("fading", true);
}

void    HomeScreenState::setupSystems()
{
    _world.addSystem<ParticleSystem>();
    _world.addSystem<UISystem>();
    _world.addSystem<ButtonSystem>(_gameStateManager);
    _world.addSystem<RenderingSystem>(_world.getSystem<ParticleSystem>()->getEmitters());
}

bool    HomeScreenState::init()
{
    //  Retrieving the "How to Play" button in order to play its animation.
    this->_buttonHowToPlay = EntityFactory::createOrGetEntity("BUTTON_HOW_TO_PLAY");
    return (true);
}
