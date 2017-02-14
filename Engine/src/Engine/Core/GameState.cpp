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
#include <Engine/MaterialDebugWindow.hpp>
#include <Engine/SoundEditorWindow.hpp>
#include <Engine/Systems/RenderingSystem.hpp>
#include <Engine/Utils/LevelLoader.hpp>
#include <Engine/Utils/LogDebugWindow.hpp>
#include <Engine/Utils/OverlayDebugWindow.hpp>
#include <Engine/Utils/MonitoringDebugWindow.hpp>
#include <Engine/Utils/Debug.hpp>
#include <Engine/Utils/Exception.hpp>
#include <Engine/Window/GameWindow.hpp>
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
                    Timer timer;
                    system->update(*_world.getEntityManager(), elapsedTime);
                    MonitoringDebugWindow::getInstance()->updateSystem(system->getId(), timer.getElapsedTime(), system->getEntitiesNb(), system->getName());
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

void    GameState::setLevelFile(const std::string& levelFile)
{
    _levelFile = levelFile;
}

void    GameState::cloneEntityManager(EntityManager* em)
{
    EntityFactory::copyEntityManager(_world.getEntityManager(), em);
}

void    GameState::initDebugWindows()
{
    EntityManager* em = _world.getEntityManager();

    #if defined(ENGINE_DEBUG) && ENGINE_DEBUG == true
        addDebugWindow<EditorMenuDebugWindow>(_gameStateManager, em, glm::vec2(0, 0), glm::vec2(0, 0));
        addDebugWindow<OverlayDebugWindow>(glm::vec2(10, 10), glm::vec2(0, 0));
        addDebugWindow<LevelEntitiesDebugWindow>(em, glm::vec2(0, 80), glm::vec2(600, 350));
        addDebugWindow<EntitiesTemplateDebugWindow>(em, glm::vec2(600, 80), glm::vec2(300, 200));
        addDebugWindow<MonitoringDebugWindow>(MonitoringDebugWindow::getInstance());
        addDebugWindow<LogDebugWindow>(Logger::getInstance(), glm::vec2(0, 430), glm::vec2(300, 200));
        addDebugWindow<MaterialDebugWindow>(glm::vec2(GameWindow::getInstance()->getBufferWidth() - 500,
                                                    GameWindow::getInstance()->getBufferHeight() - 300),
                                            glm::vec2(500, 300));
        addDebugWindow<SoundEditorWindow>(glm::vec2(1200, 80), glm::vec2(450, 450));
    #endif

    #if defined(ENGINE_DEBUG) && ENGINE_DEBUG == true
        addDebugWindow<LogDebugWindow>(Logger::getInstance(), glm::vec2(0, 430), glm::vec2(300, 200));
    #endif
}

void    GameState::renderPreviousStates(const std::vector<uint32_t>& filterIds)
{
    uint32_t gameStateIdx = 0;
    uint32_t statesNb = (uint32_t)_gameStateManager->getStates().size();

    // Find GameState index in states vector
    for (gameStateIdx; gameStateIdx < statesNb; ++gameStateIdx)
    {
        if (_gameStateManager->getStates()[gameStateIdx].get() == this)
        {
            break;
        }
    }
    ASSERT(gameStateIdx != statesNb, "The GameState should be in states vector");

    if (gameStateIdx == 0)
    {
        return;
    }

    for (uint32_t i = 0; i < gameStateIdx; ++i)
    {
        auto state = _gameStateManager->getStates()[i];
        World& stateWorld = state->getWorld();

        if (filterIds.size() > 0 &&
            std::find(filterIds.begin(), filterIds.end(), state->getId()) == filterIds.end())
            continue;

        #if defined(ENGINE_DEBUG)
            if (state->getId() == EditorState::identifier && gameStateIdx > 1)
                continue;
        #endif

        System* stateRenderSystem = stateWorld.getSystem<RenderingSystem>();

        if (stateRenderSystem)
        {
            stateRenderSystem->update(*stateWorld.getEntityManager(), 0);
        }
    }
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
        LOG_INFO("Loading level \"%s\"", _levelFile.c_str());
        LevelLoader::getInstance()->load(_levelFile, _world.getEntityManager());
    }
}
