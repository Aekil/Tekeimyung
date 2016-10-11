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
    LOG_INFO("Initialization complete !");
    return (true);
}

bool    Engine::run()
{
    // FPS counter
    Timer       timer;
    double      timePerFrame;

    bool windowImgui = true; // imgui tmp

    timePerFrame = 1.0f / 60.0f;
    while (_window->isRunning())
    {
        // Run one frame each 16ms
        if (timer.getElapsedTime() >= timePerFrame)
        {
            _window->getKeyboard().updateKeyboardState();
            _window->getMouse().resetReleasedButtons();
            _window->pollEvents();
            if (!_gameStateManager.hasStates())
            {
                return (true);
            }

            ImGui_ImplGlfwGL3_NewFrame();
            {
                ImGui::SetNextWindowPos(ImVec2(50, 650), ImGuiSetCond_FirstUseEver);
                ImGui::SetNextWindowSize(ImVec2(400, 50), ImGuiSetCond_FirstUseEver);
                ImGui::Begin("Framerate", &windowImgui);
                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
                ImGui::End();
            }

            auto &&currentState = _gameStateManager.getCurrentState();

            if (!currentState->update(timer.getElapsedTime()))
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
    Logger::getInstance()->shutdown();
    return (true);
}

GameStateManager&   Engine::getGameStateManager()
{
    return _gameStateManager;
}