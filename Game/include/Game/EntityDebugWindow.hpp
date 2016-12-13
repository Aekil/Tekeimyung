/**
* @Author   Guillaume Labey
*/

#pragma once

#include <ECS/EntityManager.hpp>

#include <Engine/Utils/DebugWindow.hpp>

#include <Game/Map.hpp>

class EntityDebugWindow: public DebugWindow
{
public:
    EntityDebugWindow(EntityManager* em, Map* map, const glm::vec2& pos, const glm::vec2& size);
    virtual ~EntityDebugWindow();

    virtual void        build(float elapsedTime);

    static uint32_t     getSelectedEntityId();

private:
    void                displayEntityDebug(Entity* entity);

    void                saveEntityTemplateToJson(const std::string& typeName);
    void                saveEntityTemplate(const std::string& typeName, Entity* entity);
    void                cloneTemplate(Entity* entity, const std::string& newTypeName);

    glm::vec3           getRandomPos();
    void                spawnEntity(const std::string& typeName);

private:
    Map*                _map;

    EntityManager*      _em;

    static uint32_t     _selectedEntityId;
};
