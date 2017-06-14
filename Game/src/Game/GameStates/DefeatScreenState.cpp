/**
* @Author   Mathieu Chassara
*/

#include    <Engine/Core/Components/RenderComponent.hh>
#include    <Engine/EntityFactory.hpp>
#include    <Engine/Systems/ButtonSystem.hpp>
#include    <Engine/Systems/RenderingSystem.hpp>
#include    <Engine/Systems/UISystem.hpp>

#include    <Game/GameStates/DefeatScreenState.hpp>

DefeatScreenState::~DefeatScreenState() {}

void    DefeatScreenState::onEnter()
{
    sRenderComponent*   renderComponent = this->_buttonTryAgainRed->getComponent<sRenderComponent>();

    renderComponent->_animator.play("fading", true);

#if (ENABLE_SOUND)
    if (_defeatSound->soundID != -1)
    {
        SoundManager::getInstance()->playSound(_defeatSound->soundID);
    }
#endif
}

void    DefeatScreenState::setupSystems()
{
    _world.addSystem<ParticleSystem>();
    _world.addSystem<UISystem>();
    _world.addSystem<ButtonSystem>(_gameStateManager);
    _world.addSystem<RenderingSystem>(_world.getSystem<ParticleSystem>()->getEmitters());
}

bool    DefeatScreenState::init()
{
    _defeatSound = EventSound::getEventByEventType(eEventSound::DEFEAT_SCREEN);
    SoundManager::getInstance()->setSoundVolume(_defeatSound->soundID, 0.9f);

    //  Retrieving the "How to Play" button in order to play its animation.
    this->_buttonTryAgainRed = EntityFactory::createOrGetEntity("BUTTON_TRY_AGAIN_RED");
    return (true);
}
