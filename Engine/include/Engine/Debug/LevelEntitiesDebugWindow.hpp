/**
* @Author   Guillaume Labey
*/

#pragma once

#include <Engine/Debug/DebugWindow.hpp>
#include <Engine/Debug/IInspectorImplementation.hpp>

class EditorState;

class LevelEntitiesDebugWindow: public DebugWindow, public IInspectorImplementation
{
// Access LevelEntitiesDebugWindow::setSelectedEntityId in EditorState::handleObjectSelection
friend EditorState;

public:
    LevelEntitiesDebugWindow();
    LevelEntitiesDebugWindow(const ImVec2& pos, const ImVec2& size);
    virtual ~LevelEntitiesDebugWindow();

    void                build(std::shared_ptr<GameState> gameState, float elapsedTime) override final;

    static uint32_t     getSelectedEntityId();

    //  IInspectorImplementation.hpp
    void                populateInspector() override final;

    GENERATE_ID(LevelEntitiesDebugWindow);

private:
    void                displayEntityDebug(EntityManager* em, Entity* entity);
    void                createTemplate(Entity* entity, const std::string& newTypeName);

    static void         setSelectedEntityId(uint32_t id);

private:
    static uint32_t     _selectedEntityId;
};
