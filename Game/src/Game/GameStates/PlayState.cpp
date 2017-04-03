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
#include <Game/GameStates/HowToPlayState.hpp>
#include <Game/GameStates/PauseState.hpp>

#include <Game/GameStates/PlayState.hpp>


PlayState::~PlayState() {}

void    PlayState::onEnter() {}

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
    EntityManager* em = _world.getEntityManager();

    _pair = std::make_pair(Keyboard::eKey::F, new HandleFullscreenEvent());

    _backgroundMusic = EventSound::getEventByEventType(eEventSound::BACKGROUND);

    return (true);
}

bool    PlayState::update(float elapsedTime)
{
    auto& gameWindow = GameWindow::getInstance();
    auto &&keyboard = GameWindow::getInstance()->getKeyboard();
    auto &&mouse = GameWindow::getInstance()->getMouse();

    if (keyboard.getStateMap()[_pair.first] == Keyboard::eKeyState::KEY_PRESSED)
        _pair.second->execute();

    if (keyboard.getStateMap()[Keyboard::eKey::H] == Keyboard::eKeyState::KEY_PRESSED)
    {
        _gameStateManager->addState<HowToPlayState>();
    }

    if (keyboard.getStateMap()[Keyboard::eKey::ESCAPE] == Keyboard::eKeyState::KEY_PRESSED ||
        gameWindow->hasLostFocus())
    {
        _gameStateManager->addState<PauseState>();
    }

    //updateCameraInputs(elapsedTime);

    // Play background music
    #if (ENABLE_SOUND)
        if (_backgroundMusic->soundID != -1 && !SoundManager::getInstance()->isSoundPlaying(_backgroundMusic->soundID))
        {
            SoundManager::getInstance()->playSound(_backgroundMusic->soundID);
        }
    #endif

    return (GameState::update(elapsedTime));
}
