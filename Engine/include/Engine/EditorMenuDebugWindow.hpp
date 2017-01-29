/**
* @Author   Guillaume Labey
*/

#pragma once

#include <ECS/EntityManager.hpp>

#include <Engine/Utils/DebugWindow.hpp>
#include <Engine/Core/GameStateManager.hpp>

class EditorMenuDebugWindow: public DebugWindow
{
public:
    EditorMenuDebugWindow(GameStateManager* gameStateManager, EntityManager* em, const glm::vec2& pos, const glm::vec2& size);
    virtual ~EditorMenuDebugWindow();

    virtual void        build(float elapsedTime);

    void                displayLevelsMenu();
    void                displayPlayStopMenu();

    void                displayStateNotRegistered();

    void                displaySaveAsPopup();
    void                displayLoadPopup();

    GENERATE_ID(EditorMenuDebugWindow);

private:
    EntityManager*      _em;

    std::string         _currentLevel;
    std::string         _tmpLoadLevel;

    GameStateManager*   _gameStateManager;
};
