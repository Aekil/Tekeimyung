/**
* @Author   Guillaume Labey
*/

#pragma once

#include <memory>
#include <vector>

#include <Engine/Core/GameState.hpp>
#include <Engine/EditorMenuDebugWindow.hpp>
#include <Engine/EntitiesTemplateDebugWindow.hpp>
#include <Engine/LevelEntitiesDebugWindow.hpp>
#include <Engine/SoundEditorWindow.hpp>
#include <Engine/Utils/LogDebugWindow.hpp>
#include <Engine/Utils/OverlayDebugWindow.hpp>
#include <Engine/Utils/MonitoringDebugWindow.hpp>

class GameState;

class GameStateManager
{
public:
    GameStateManager();
    virtual ~GameStateManager();

    template<typename T>
    bool                                        addState()
    {
        std::shared_ptr<T> gameState = std::make_shared<T>(this);
        gameState->onEnter();
        if (!gameState->init() || !gameState->initSystems())
        {
            return (false);
        }

        // Don't add debug windows if we are not in debug mode
        #if defined(ENGINE_DEBUG)
            EntityManager* em = gameState->getWorld().getEntityManager();
            gameState->addDebugWindow<OverlayDebugWindow>();
            gameState->addDebugWindow<LevelEntitiesDebugWindow>(em, glm::vec2(0, 80), glm::vec2(600, 350));
            gameState->addDebugWindow<LogDebugWindow>(Logger::getInstance(), glm::vec2(0, 430), glm::vec2(300, 200));
            gameState->addDebugWindow<SoundEditorWindow>(glm::vec2(1200, 80), glm::vec2(450, 450));
            gameState->addDebugWindow<EditorMenuDebugWindow>(em, glm::vec2(0, 0), glm::vec2(0, 0));
            gameState->addDebugWindow<EntitiesTemplateDebugWindow>(em, glm::vec2(600, 80), glm::vec2(300, 200));
            gameState->addDebugWindow<MonitoringDebugWindow>(MonitoringDebugWindow::getInstance());
        #endif

        _states.push_back(gameState);

        return (true);
    }

    void                                        removeCurrentState();
    std::shared_ptr<GameState>                  getCurrentState() const;
    bool                                        hasStates() const;

    std::vector<std::shared_ptr<GameState>>&    getStates()
    {
        return (_states);
    }

    void                                        clearStates();

private:
    // States vector
    std::vector<std::shared_ptr<GameState>>     _states;
};
