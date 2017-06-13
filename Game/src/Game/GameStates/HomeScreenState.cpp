/**
* @Author   Mathieu Chassara
*/

#include    <Engine/Core/Components/RenderComponent.hh>
#include    <Engine/EntityFactory.hpp>
#include    <Engine/Systems/ButtonSystem.hpp>
#include    <Engine/Systems/RenderingSystem.hpp>
#include    <Engine/Systems/UISystem.hpp>

#include    <Game/GameStates/OptionsMenuState.hpp>
#include    <Game/GameStates/HomeScreenState.hpp>

HomeScreenState::~HomeScreenState() {}

void    HomeScreenState::onEnter()
{
    sRenderComponent*   renderComponent = this->_buttonHowToPlay->getComponent<sRenderComponent>();

    renderComponent->_animator.play("fading", true);

    OptionsMenuState::setFromHome(true);
    
    if (!SoundManager::getInstance()->getMuteState())
    {
        SoundManager::getInstance()->setVolumeAllChannels(DEFAULT_SOUND_VOL);
    }

    SoundManager::getInstance()->pauseSound(_backgroundGameMusic->soundID);
    SoundManager::getInstance()->stopSound(_backgroundGameMusic->soundID);
    SoundManager::getInstance()->resumeSound(_backgroundMenuMusic->soundID);

#if (ENABLE_SOUND)
    if (_backgroundMenuMusic->soundID != -1 && !SoundManager::getInstance()->isSoundPlaying(_backgroundMenuMusic->soundID))
    {
        SoundManager::getInstance()->playSound(_backgroundMenuMusic->soundID);
    }
#endif
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
    _backgroundMenuMusic = EventSound::getEventByEventType(eEventSound::BACKGROUND_MENU);
    SoundManager::getInstance()->setSoundVolume(_backgroundMenuMusic->soundID, 1.0f);
    _backgroundGameMusic = EventSound::getEventByEventType(eEventSound::BACKGROUND);

    //  Retrieving the "How to Play" button in order to play its animation.
    this->_buttonHowToPlay = EntityFactory::createOrGetEntity("BUTTON_HOW_TO_PLAY");

    return (true);
}

/*bool    HomeScreenState::update(float elapsedTime)
{
    // Play background home menu music
#if (ENABLE_SOUND)
    if (_backgroundMenuMusic->soundID != -1 && !SoundManager::getInstance()->isSoundPlaying(_backgroundMenuMusic->soundID))
    {
        SoundManager::getInstance()->playSound(_backgroundMenuMusic->soundID);
    }
#endif
    return (true); // ok ?
}*/
