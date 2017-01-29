/**
* @Author   Guillaume Labey
*/

#pragma once

#include <ECS/EntityManager.hpp>

#include <Engine/Utils/DebugWindow.hpp>

class EntitiesTemplateDebugWindow: public DebugWindow
{
public:
    EntitiesTemplateDebugWindow(EntityManager* em, const glm::vec2& pos, const glm::vec2& size);
    virtual ~EntitiesTemplateDebugWindow();

    virtual void        build(float elapsedTime);

    GENERATE_ID(EntitiesTemplateDebugWindow);

private:
    void                cloneTemplate(const std::string& cloneTypeName, const std::string& newTypeName);

private:
    EntityManager*      _em;
    std::string         _selectedEntityTemplate;
};
