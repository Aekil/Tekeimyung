/**
* @Author   Guillaume Labey
*/

#include <iostream>
#include <imgui.h>
#include <imgui_impl_glfw_gl3.h>
#include <ImGuizmo.h>

#include <Engine/EditorMenuDebugWindow.hpp>
#include <Engine/EditorState.hpp>
#include <Engine/EntitiesTemplateDebugWindow.hpp>
#include <Engine/Graphics/Geometries/GeometryFactory.hpp>
#include <Engine/LevelEntitiesDebugWindow.hpp>
#include <Engine/MaterialDebugWindow.hpp>
#include <Engine/SoundEditorWindow.hpp>
#include <Engine/Utils/LevelLoader.hpp>
#include <Engine/Utils/LogDebugWindow.hpp>
#include <Engine/Utils/Logger.hpp>
#include <Engine/Utils/MonitoringDebugWindow.hpp>
#include <Engine/Utils/OverlayDebugWindow.hpp>
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
                LOG_WARN("Game: No game states in the game state manager");
                return (true);
            }

            auto &&currentState = _gameStateManager.getCurrentState();
            currentState->bindEntityManager();

            // Clear color buffer
            glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            ImGui_ImplGlfwGL3_NewFrame();
            {
                ImGuizmo::BeginFrame();
                {
                    // Update debug windows
                    for (auto&& debugWindow: _debugWindows)
                    {
                        if (debugWindow->isDisplayed())
                            debugWindow->build(currentState, elapsedTime);
                    }

                    if (currentState->update(elapsedTime) == false)
                    {
                        _gameStateManager.removeCurrentState();
                    }
                }
            }


            // Display imgui windows
            ImGui::Render();

            // Display screen
            GameWindow::getInstance()->display();
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

bool    Engine::initDebugWindows(int ac, char** av)
{
    // Don't add debug windows if we are not in debug mode
    #if defined(ENGINE_DEBUG) && ENGINE_DEBUG == true
        // We expect that initStartGameState is call before
        // so the first state should be EditorState
        auto& frontState = _gameStateManager.getStates().front();
        ASSERT(frontState->getId() == EditorState::identifier, "The first state of the engine should be EditorState in debug mode");
        addDebugWindow<EditorMenuDebugWindow>(&_gameStateManager, frontState->getWorld().getEntityManager(), glm::vec2(0, 0), glm::vec2(0, 0));
        addDebugWindow<OverlayDebugWindow>(glm::vec2(10, 10), glm::vec2(0, 0));
        addDebugWindow<LevelEntitiesDebugWindow>(glm::vec2(0, 80), glm::vec2(600, 350));
        addDebugWindow<EntitiesTemplateDebugWindow>(glm::vec2(600, 80), glm::vec2(300, 200));
        addDebugWindow<MonitoringDebugWindow>(MonitoringDebugWindow::getInstance());
        addDebugWindow<LogDebugWindow>(Logger::getInstance(), glm::vec2(0, 430), glm::vec2(300, 200));
        addDebugWindow<MaterialDebugWindow>(glm::vec2(GameWindow::getInstance()->getBufferWidth() - 500,
                                                    GameWindow::getInstance()->getBufferHeight() - 300),
                                            glm::vec2(500, 300));
        addDebugWindow<SoundEditorWindow>(glm::vec2(1200, 80), glm::vec2(450, 450));
        addDebugWindow<LogDebugWindow>(Logger::getInstance(), glm::vec2(0, 430), glm::vec2(300, 200));

        // Handle command line for EditorMenuDebugWindow
        if (ac >= 2)
        {
            // AV[1] => Level name (Ex: "Options")
            if (!LevelLoader::getInstance()->hasLevel(av[1]))
            {
                LOG_ERROR("Can't find level \"%s\"", av[1]);
                return (false);
            }

            auto debugMenu = getDebugWindow<EditorMenuDebugWindow>();
            ASSERT(debugMenu != nullptr, "Engine should have EditorMenuDebugWindow in Debug mode");
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
    #if defined(ENGINE_DEBUG) && ENGINE_DEBUG == true
        startGameState = std::make_shared<EditorState>(&_gameStateManager);
    #endif

    if (!_gameStateManager.addState(startGameState))
    {
        return (false);
    }

    return (true);
}
