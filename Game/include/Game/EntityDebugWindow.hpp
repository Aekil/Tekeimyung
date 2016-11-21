#pragma once

#include <ECS/EntityManager.hpp>

#include <Engine/Utils/DebugWindow.hpp>

#include <Game/Map.hpp>

class EntityDebugWindow: public DebugWindow
{
public:
    EntityDebugWindow(EntityManager* em, Map* map, const glm::vec2& pos, const glm::vec2& size);
    virtual ~EntityDebugWindow();

    virtual void        build();

private:
    void                saveEntityTemplateToJson(const std::string& typeName);
    void                saveEntityTemplate(const std::string& typeName, Entity* entity);
    glm::vec3           getRandomPos();
    void                spawnEntity(const std::string& typeName);

private:
    // Selected entity in IMGUI select box
    int                 _selectedEntity;

    Map*                _map;

    EntityManager*      _em;
};
