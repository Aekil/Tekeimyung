/**
* @Author   Guillaume Labey
*/

#pragma once

#include <ECS/EntityManager.hpp>

#include <Engine/Utils/DebugWindow.hpp>

class LevelEntitiesDebugWindow: public DebugWindow
{
public:
    LevelEntitiesDebugWindow(EntityManager* em, const glm::vec2& pos, const glm::vec2& size);
    virtual ~LevelEntitiesDebugWindow();

    virtual void        build(float elapsedTime);

    static uint32_t     getSelectedEntityId();

    GENERATE_ID(LevelEntitiesDebugWindow);

private:
    void                displayEntityDebug(Entity* entity);
    void                createTemplate(Entity* entity, const std::string& newTypeName);

private:
    EntityManager*      _em;

    static uint32_t     _selectedEntityId;
};
