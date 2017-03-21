/**
* @Author   Mathieu Chassara
*/

#include    <Engine/Components.hh>
#include    <Engine/EntityFactory.hpp>

#include    <Engine/Systems/RenderingSystem.hpp>
#include    <Engine/Systems/UISystem.hpp>
#include    <Engine/Systems/ButtonSystem.hpp>

#include    <Game/GameStates/HomeScreenState.hpp>

HomeScreenState::~HomeScreenState() {}

void    HomeScreenState::onEnter()
{
    sRenderComponent*   renderComponent = this->_playGameButton->getComponent<sRenderComponent>();

    renderComponent->_animator.play("color_fading", true);
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
    this->createPlayGameButton();
    return (true);
}

void    HomeScreenState::createPlayGameButton()
{
    std::string buttonArchetype = "BUTTON_PLAY_GAME";

    this->_playGameButton = EntityFactory::createOrGetEntity(buttonArchetype);
}