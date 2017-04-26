/**
* @Author   Guillaume Labey
*/

#pragma once

#include <ECS/EntityManager.hpp>

#include <Engine/Debug/DebugWindow.hpp>

class EntitiesTemplateDebugWindow: public DebugWindow
{
public:
    EntitiesTemplateDebugWindow();
    EntitiesTemplateDebugWindow(const ImVec2& pos, const ImVec2& size);
    virtual ~EntitiesTemplateDebugWindow();

    void                build(std::shared_ptr<GameState> gameState, float elapsedTime) override final;

    GENERATE_ID(EntitiesTemplateDebugWindow);

private:
    void                cloneTemplate(const std::string& cloneTypeName, const std::string& newTypeName);

private:
    std::string         _selectedEntityTemplate;
};
