/**
* @Author   Guillaume Labey
*/

#pragma once

#include <Engine/Utils/DebugWindow.hpp>

class EditorState;

class LevelEntitiesDebugWindow: public DebugWindow
{
// Access LevelEntitiesDebugWindow::setSelectedEntityId in EditorState::handleObjectSelection
friend EditorState;

public:
    LevelEntitiesDebugWindow(const glm::vec2& pos, const glm::vec2& size);
    virtual ~LevelEntitiesDebugWindow();

    void                build(std::shared_ptr<GameState> gameState, float elapsedTime) override final;

    static uint32_t     getSelectedEntityId();

    GENERATE_ID(LevelEntitiesDebugWindow);

private:
    void                displayEntityDebug(EntityManager* em, Entity* entity);
    void                createTemplate(Entity* entity, const std::string& newTypeName);

    static void         setSelectedEntityId(uint32_t id);

private:
    static uint32_t     _selectedEntityId;
};
