/**
* @Author   Guillaume Labey
*/

#include <iostream>
#include <imgui.h>
#include <imgui_impl_glfw_gl3.h>
#include <ImGuizmo.h>

#include <Engine/EditorMenuDebugWindow.hpp>
#include <Engine/EntitiesTemplateDebugWindow.hpp>
#include <Engine/LevelEntitiesDebugWindow.hpp>
#include <Engine/SoundEditorWindow.hpp>
#include <Engine/Utils/LevelLoader.hpp>
#include <Engine/Utils/LogDebugWindow.hpp>
#include <Engine/Utils/OverlayDebugWindow.hpp>
#include <Engine/Utils/MonitoringDebugWindow.hpp>
#include <Engine/Utils/Exception.hpp>
#include <Engine/EditorState.hpp>
#include <Engine/EntityFactory.hpp>

#include <Engine/Core/GameState.hpp>

GameState::GameState(GameStateManager* gameStateManager, uint32_t id, const std::string& levelFile):
                    _gameStateManager(gameStateManager), _id(id), _levelFile(levelFile) {}

GameState::~GameState() {}

bool    GameState::initSystems()
{
    for (auto& system: _world.getSystems())
    {
        if (!system->init())
        {
            return (false);
        }
    }

    return (true);
}

bool    GameState::update(float elapsedTime)
{
    try
    {
        ImGui_ImplGlfwGL3_NewFrame();
        {
            ImGuizmo::BeginFrame();
            {
                // Update GameState debug windows
                for (auto&& debugWindow: _debugWindows)
                {
                    if (debugWindow->isDisplayed())
                        debugWindow->build(elapsedTime);
                }

                // Update GameState systems
                for (auto&& system: _world.getSystems())
                {
                    system->update(*_world.getEntityManager(), elapsedTime);
                }
            }
        }
    }
    catch(const Exception &e)
    {
        std::cerr << e.what() << std::endl;
        return (false);
    }

    return (true);
}

uint32_t    GameState::getId() const
{
    return (_id);
}

World&  GameState::getWorld()
{
    return (_world);
}

void    GameState::initDebugWindows()
{
    EntityManager* em = _world.getEntityManager();

    addDebugWindow<EditorMenuDebugWindow>(_gameStateManager, em, glm::vec2(0, 0), glm::vec2(0, 0));
    if (_id == EditorState::identifier)
    {
        addDebugWindow<LevelEntitiesDebugWindow>(em, glm::vec2(0, 80), glm::vec2(600, 350));
        addDebugWindow<LogDebugWindow>(Logger::getInstance(), glm::vec2(0, 430), glm::vec2(300, 200));
        addDebugWindow<SoundEditorWindow>(glm::vec2(1200, 80), glm::vec2(450, 450));
        addDebugWindow<EntitiesTemplateDebugWindow>(em, glm::vec2(600, 80), glm::vec2(300, 200));
        addDebugWindow<MonitoringDebugWindow>(MonitoringDebugWindow::getInstance());
    }
    //addDebugWindow<OverlayDebugWindow>(glm::vec2(10, 10), glm::vec2(0, 0));
}

void    GameState::bindEntityManager()
{
    EntityFactory::bindEntityManager(_world.getEntityManager());
}

void    GameState::loadLevel()
{
    // Load level
    if (_levelFile.size() > 0)
    {
        LevelLoader::getInstance()->load(_levelFile, _world.getEntityManager());
    }
}
