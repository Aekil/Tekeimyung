#pragma once

#include "Utils/DebugWindow.hpp"

class EntityDebugWindow: public DebugWindow
{
public:
    EntityDebugWindow(const glm::vec2& pos, const glm::vec2& size);
    virtual ~EntityDebugWindow();

    virtual void        build();

private:
    void                saveEntityToJson(const std::string& typeName);

private:
    // Selected entity in IMGUI select box
    int                 _selectedEntity;
};
