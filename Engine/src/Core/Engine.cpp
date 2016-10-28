#include <Core/Engine.hpp>
#include <Utils/Logger.hpp>
#include <Utils/Timer.hpp>

Engine::Engine() {}

Engine::~Engine() {}

bool    Engine::init()
{
    std::shared_ptr<Logger> logger;

    _window = std::make_shared<GameWindow>();
    if (!_window->initialize())
        return (false);

    logger = std::make_shared<Logger>();
    if (!logger->initialize())
        return (false);

    GameWindow::setInstance(_window);
    Logger::setInstance(logger);
    return (true);
}

bool    Engine::run()
{
    // FPS counter
    Timer       timer;
    double      timePerFrame;

    timePerFrame = 1.0f / 60.0f;
    while (_window->isRunning())
    {
        // Run one frame each 16ms
        if (timer.getElapsedTime() >= timePerFrame)
        {
            _window->getKeyboard().updateKeyboardState();
            _window->getMouse().updateMouseState();
            _window->pollEvents();
            if (!_gameStateManager.hasStates())
            {
                return (true);
            }

            auto &&currentState = _gameStateManager.getCurrentState();

            if (currentState->update(timer.getElapsedTime()) == false)
            {
                _gameStateManager.removeCurrentState();
            }
            //std::cout << "Time: " << timer.getElapsedTime() * 1000 << std::endl;
            timer.reset();
        }
    }
    return (true);
}

bool    Engine::stop()
{
    Logger::getInstance()->shutdown();
    return (true);
}

GameStateManager&   Engine::getGameStateManager()
{
    return _gameStateManager;
}
