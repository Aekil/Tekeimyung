/**
* @Author   Guillaume Labey
*/

#pragma once

#include <Engine/Utils/DebugWindow.hpp>

class LevelEntitiesDebugWindow: public DebugWindow
{
public:
    LevelEntitiesDebugWindow(const glm::vec2& pos, const glm::vec2& size);
    virtual ~LevelEntitiesDebugWindow();

    void                build(std::shared_ptr<GameState> gameState, float elapsedTime) override final;

    static uint32_t     getSelectedEntityId();

    GENERATE_ID(LevelEntitiesDebugWindow);

private:
    void                displayEntityDebug(EntityManager* em, Entity* entity);
    void                createTemplate(Entity* entity, const std::string& newTypeName);

private:
    static uint32_t     _selectedEntityId;
};
