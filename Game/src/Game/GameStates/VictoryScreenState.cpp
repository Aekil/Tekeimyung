/**
* @Author   Mathieu Chassara
*/

#include    <Engine/Core/Components/RenderComponent.hh>
#include    <Engine/EntityFactory.hpp>
#include    <Engine/Systems/ButtonSystem.hpp>
#include    <Engine/Systems/RenderingSystem.hpp>
#include    <Engine/Systems/UISystem.hpp>

#include    <Game/GameStates/VictoryScreenState.hpp>

VictoryScreenState::~VictoryScreenState() {}

void    VictoryScreenState::onEnter()
{
    sRenderComponent*   renderComponent = this->_buttonTryAgainGreen->getComponent<sRenderComponent>();

    renderComponent->_animator.play("fading", true);

#if (ENABLE_SOUND)
    if (_victorySound->soundID != -1)
    {
        SoundManager::getInstance()->playSound(_victorySound->soundID);
    }
#endif
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
    _victorySound = EventSound::getEventByEventType(eEventSound::VICTORY_SCREEN);
    //SoundManager::getInstance()->setSoundVolume(_victorySound->soundID, 1.0f);

    //  Retrieving the "How to Play" button in order to play its animation.
    this->_buttonTryAgainGreen = EntityFactory::createOrGetEntity("BUTTON_TRY_AGAIN_GREEN");
    return (true);
}
