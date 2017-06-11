/**
* @Author   Guillaume Labey
*/

#pragma once

#include <Engine/Debug/DebugWindow.hpp>
#include <Engine/Debug/IInspectorImplementation.hpp>

class EditorState;

class LevelEntitiesDebugWindow: public DebugWindow, public IInspectorImplementation
{
// Access LevelEntitiesDebugWindow::setSelectedEntityHandler in EditorState::handleObjectSelection
friend EditorState;

public:
    LevelEntitiesDebugWindow();
    LevelEntitiesDebugWindow(const glm::vec2& pos, const glm::vec2& size);
    virtual ~LevelEntitiesDebugWindow();

    void                build(std::shared_ptr<GameState> gameState, float elapsedTime) override final;

    static Entity::sHandle     getSelectedEntityHandler();

    //  IInspectorImplementation.hpp
    void                populateInspector() override final;

    GENERATE_ID(LevelEntitiesDebugWindow);

private:
    void                displayEntityDebug(EntityManager* em, Entity* entity);
    void                createTemplate(Entity* entity, const std::string& newTypeName);

    static void         setSelectedEntityHandler(Entity::sHandle handle);

private:
    static Entity::sHandle     _selectedEntityHandler;
};
