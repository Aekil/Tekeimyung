/**
* @Author   Guillaume Labey
*/

#pragma once

#include <ECS/EntityManager.hpp>
#include <Engine/Utils/DebugWindow.hpp>
#include <Engine/Core/GameStateManager.hpp>

class Engine;

class EditorMenuDebugWindow: public DebugWindow
{
friend Engine;

public:
    EditorMenuDebugWindow(GameStateManager* gameStateManager, EntityManager* em, const glm::vec2& pos, const glm::vec2& size);
    virtual ~EditorMenuDebugWindow();

    void                build(std::shared_ptr<GameState> gameState, float elapsedTime) override final;

    void                loadLevel(const std::string& levelName);

    GENERATE_ID(EditorMenuDebugWindow);

private:
    void                displayLevelsMenu();
    void                displayPlayStopMenu();

    void                displaySaveAsPopup();
    void                displayLoadPopup();

    void                play();

private:
    std::string         _currentLevel;
    std::string         _tmpLoadLevel;

    GameStateManager*   _gameStateManager;
    // Entity manager of EditorState
    EntityManager*      _em;
};
