/**
* @Author   Guillaume Labey
*/

#include <iostream>

#include <Engine/EditorState.hpp>
#include <Engine/Graphics/Geometries/GeometryFactory.hpp>
#include <Engine/Debug/Logger.hpp>
#include <Engine/Debug/MenuBarDebugWindow.hpp>
#include <Engine/Debug/EntitiesTemplateDebugWindow.hpp>
#include <Engine/Debug/LevelEntitiesDebugWindow.hpp>
#include <Engine/Debug/MaterialDebugWindow.hpp>
#include <Engine/Debug/SoundEditorWindow.hpp>
#include <Engine/Debug/LogDebugWindow.hpp>
#include <Engine/Debug/MonitoringDebugWindow.hpp>
#include <Engine/Debug/OverlayDebugWindow.hpp>
#include <Engine/Debug/InspectorDebugWindow.hpp>
#include <Engine/Utils/LevelLoader.hpp>
#include <Engine/Utils/Timer.hpp>

#include <Engine/Core/Engine.hpp>

Engine::Engine() {}

Engine::~Engine() {}

bool    Engine::init()
{
    _logger = Logger::getInstance();
    if (!_logger->initialize())
    {
        std::cerr << "Engine: Failed to initialize logger" << std::endl;
        return (false);
    }

    _window = std::make_shared<GameWindow>(&_gameStateManager);
    if (!_window->initialize())
    {
        LOG_ERROR("Engine: Failed to initialize window");
        return (false);
    }
    GameWindow::setInstance(_window);

    _soundManager = SoundManager::getInstance();
    if (!_soundManager->initialize())
    {
        LOG_ERROR("Engine: Failed to initialize sound manager");
        return (false);
    }

    _renderer = Renderer::getInstance();
    if (!_renderer->initialize())
    {
        LOG_ERROR("Engine: Failed to initialize renderer");
        return (false);
    }

    return (true);
}

bool    Engine::run(int ac, char** av, std::shared_ptr<GameState> startGameState)
{
    // FPS counter
    Timer&       timer = _window->getTimer();
    float       timePerFrame;

    timer.reset();
    timePerFrame = 1.0f / 60.0f;

    // TODO: move initStartGameState and initDebugWindows in Engine::init
    // (need to find a way to initialize resources in Engine::init)
    if (!initStartGameState(startGameState) ||
        !initDebugWindows(ac, av))
        {
            return (false);
        }

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
                LOG_WARN("Engine::run: No game states in the game state manager");
                return (true);
            }

            auto &&currentState = _gameStateManager.getCurrentState();
            currentState->bindEntityManager();

            _renderer->beginFrame();

            // Update state before debug windows because it can remove
            // states (So we don't want the removed state to update)
            if (currentState->update(elapsedTime) == false)
            {
                _gameStateManager.removeCurrentState();
                auto &&currentState = _gameStateManager.getCurrentState();
            }

            // Update debug windows
            if (_gameStateManager.hasStates())
            {
                if (_debugWindows.size() > 0)
                {
                    DebugWindow::applyGlobalStyle();
                    for (auto&& debugWindow : _debugWindows)
                    {
                        if (debugWindow->isDisplayed())
                            debugWindow->build(currentState, elapsedTime);
                    }
                }
            }

            ImGui::ShowTestWindow();

            _renderer->endFrame();
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
    return (_gameStateManager);
}

const std::vector<std::shared_ptr<DebugWindow>>&    Engine::getDebugWindows() const
{
    return (_debugWindows);
}

void    Engine::toggleDebugWindowsDisplay(bool displayed)
{
    for (auto& debugWindow: _debugWindows)
    {
        if (debugWindow->getId() != MenuBarDebugWindow::identifier)
        {
            debugWindow->isDisplayed(displayed);
        }
    }
}

bool    Engine::initDebugWindows(int ac, char** av)
{
    // Don't add debug windows if we are not in debug mode
    #if defined(ENGINE_DEBUG)
        // We expect that initStartGameState is call before
        // so the first state should be EditorState
        auto& frontState = _gameStateManager.getStates().front();
        ASSERT(frontState->getId() == EditorState::identifier, "The first state of the engine should be EditorState in debug mode");
        addDebugWindow<MenuBarDebugWindow>(this, frontState->getWorld().getEntityManager());
        addDebugWindow<OverlayDebugWindow>();
        addDebugWindow<LevelEntitiesDebugWindow>();
        addDebugWindow<EntitiesTemplateDebugWindow>();
        addDebugWindow<MaterialDebugWindow>();
        addDebugWindow<SoundEditorWindow>();
        addDebugWindow<LogDebugWindow>(Logger::getInstance());
        addDebugWindow<MonitoringDebugWindow>(MonitoringDebugWindow::getInstance());
        addDebugWindow<InspectorDebugWindow>();

        this->getDebugWindow<InspectorDebugWindow>()->bindPopulateFunction(this->getDebugWindow<LevelEntitiesDebugWindow>());

        // Handle command line for MenuBarDebugWindow
        if (ac >= 2)
        {
            // AV[1] => Level name (Ex: "Options")
            if (!LevelLoader::getInstance()->hasLevel(av[1]))
            {
                LOG_ERROR("Can't find level \"%s\"", av[1]);
                return (false);
            }

            auto debugMenu = getDebugWindow<MenuBarDebugWindow>();
            ASSERT(debugMenu != nullptr, "Engine should have MenuBarDebugWindow in Debug mode");
            debugMenu->loadLevel(av[1]);

            // AV[2] => options (Ex: "-a")
            if (ac >= 3)
            {
                // Auto play level
                if (av[2] == std::string("-a"))
                {
                    debugMenu->play();
                }
            }
        }
    #endif

        return (true);
}

bool    Engine::initStartGameState(std::shared_ptr<GameState> startGameState)
{
    #if defined(ENGINE_DEBUG)
        startGameState = std::make_shared<EditorState>(&_gameStateManager);
    #endif

    if (!_gameStateManager.addState(startGameState))
    {
        return (false);
    }

    return (true);
}
