#include "Utils/Logger.hpp"
#include "Utils/Timer.hpp"

#include "Core/Engine.hpp"

Engine::Engine() {}

Engine::~Engine() {}

bool    Engine::init()
{
    _window = std::make_shared<GameWindow>();
    if (!_window->initialize())
        return (false);

    _logger = Logger::getInstance();
    if (!_logger->initialize())
        return (false);

    _soundManager = SoundManager::getInstance();
    if (!_soundManager->initialize())
        return (false);

    GameWindow::setInstance(_window);
    return (true);
}

bool    Engine::run()
{
    // FPS counter
    Timer       timer;
    float       timePerFrame;

    timePerFrame = 1.0f / 60.0f;
    while (_window->isRunning())
    {
        // Run one frame each 16ms
        if (timer.getElapsedTime() >= timePerFrame)
        {
            float elapsedTime = timer.getElapsedTime();
            timer.reset();
            _window->getKeyboard().updateKeyboardState();
            _window->getMouse().updateMouseState();
            _window->pollEvents();

            _soundManager->update();

            if (!_gameStateManager.hasStates())
            {
                return (true);
            }

            auto &&currentState = _gameStateManager.getCurrentState();

            if (currentState->update(elapsedTime) == false)
            {
                _gameStateManager.removeCurrentState();
            }
            timer.reset();
        }
    }
    return (true);
}

bool    Engine::stop()
{
    _soundManager->shutdown();
    Logger::getInstance()->shutdown();
    return (true);
}

GameStateManager&   Engine::getGameStateManager()
{
    return _gameStateManager;
}
