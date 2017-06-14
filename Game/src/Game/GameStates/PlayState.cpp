/**
* @Author   Guillaume Labey
*/

#include <Engine/Window/Keyboard.hpp>
#include <Engine/Window/HandleFullscreenEvent.hpp>
#include <Engine/Window/GameWindow.hpp>
#include <Engine/Sound/SoundManager.hpp>

#include <Engine/Systems/RenderingSystem.hpp>
#include <Engine/Systems/RigidBodySystem.hpp>
#include <Engine/Systems/CollisionSystem.hpp>
#include <Engine/Systems/ParticleSystem.hpp>
#include <Engine/Systems/ScriptSystem.hpp>
#include <Engine/Systems/UISystem.hpp>
#include <Engine/Systems/MouseSystem.hpp>
#include <Engine/Utils/LevelLoader.hpp>
#include <Game/GameStates/OptionsMenuState.hpp>
#include <Game/GameStates/HowToPlayState.hpp>
#include <Game/GameStates/BuildingListState.hpp>
#include <Game/GameStates/PauseState.hpp>
#include <Game/Manager/TutoManagerMessage.hpp>
#include <Game/Manager/TutoManager.hpp>

#include <Game/GameStates/PlayState.hpp>


PlayState::~PlayState() {}

void    PlayState::onEnter()
{
    SoundManager::getInstance()->setMuteSfx(true);

    if (!SoundManager::getInstance()->getMuteState())
    {
        SoundManager::getInstance()->setVolumeAllChannels(DEFAULT_SOUND_VOL);
    }
    SoundManager::getInstance()->pauseSound(_backgroundMenuMusic->soundID);
    SoundManager::getInstance()->stopSound(_backgroundMenuMusic->soundID);
    SoundManager::getInstance()->resumeSound(_backgroundGameMusic->soundID);
}

void    PlayState::setupSystems()
{
    _world.addSystem<ScriptSystem>();
    _world.addSystem<MouseSystem>();
    _world.addSystem<CollisionSystem>();
    _world.addSystem<RigidBodySystem>();
    _world.addSystem<ParticleSystem>();
    _world.addSystem<UISystem>();
    _world.addSystem<RenderingSystem>(_world.getSystem<ParticleSystem>()->getEmitters());
}

bool    PlayState::init()
{
    _backgroundGameMusic = EventSound::getEventByEventType(eEventSound::BACKGROUND);
    SoundManager::getInstance()->setSoundVolume(_backgroundGameMusic->soundID, 0.2f);
    _backgroundMenuMusic = EventSound::getEventByEventType(eEventSound::BACKGROUND_MENU);

    // Load tutorial level
    if (!TutoManager::_tutorialDone)
    {
        auto em = _world.getEntityManager();
        em->destroyAllEntities();
        LevelLoader::getInstance()->load("Tutorial", em);
    }
    return (true);
}

bool    PlayState::update(float elapsedTime)
{
    auto& gameWindow = GameWindow::getInstance();
    auto &&keyboard = GameWindow::getInstance()->getKeyboard();
    auto &&mouse = GameWindow::getInstance()->getMouse();
    auto soundManager = SoundManager::getInstance();

    /*if (keyboard.getStateMap()[Keyboard::eKey::B] == Keyboard::eKeyState::KEY_PRESSED)
    {
        _gameStateManager->addState<BuildingListState>();
        TutoManagerMessage::getInstance()->sendMessage(eTutoState::CHECK_BUILDLIST);
    }*/
    if (!TutoManagerMessage::getInstance()->tutorialDone())
    {
        if (keyboard.getStateMap()[Keyboard::eKey::K] == Keyboard::eKeyState::KEY_RELEASED ||
            (keyboard.getStateMap()[Keyboard::eKey::T] == Keyboard::eKeyState::KEY_RELEASED &&
            TutoManagerMessage::getInstance()->stateOnGoing(eTutoState::TUTO_DONE)))
        {
            TutoManager::_tutorialDone = true;
            _gameStateManager->replaceState<PlayState>();
            return (true);
        }
    }
    if (keyboard.getStateMap()[Keyboard::eKey::ESCAPE] == Keyboard::eKeyState::KEY_PRESSED ||
        gameWindow->hasLostFocus())
    {
        _gameStateManager->addState<PauseState>();
    }
    else if (keyboard.getStateMap()[Keyboard::eKey::M] == Keyboard::eKeyState::KEY_PRESSED)
    {
        bool muted = soundManager->getMuteState();
        soundManager->setMuteState(!muted);
        soundManager->setVolumeAllChannels((muted == false ? 0.0f : DEFAULT_SOUND_VOL));
    }

    // Play background game music
    #if (ENABLE_SOUND)
        if (_backgroundGameMusic->soundID != -1 && !soundManager->isSoundPlaying(_backgroundGameMusic->soundID))
        {
            soundManager->playSound(_backgroundGameMusic->soundID);
        }
    #endif

    return (GameState::update(elapsedTime));
}
