/**
* @Author   Guillaume Labey
*/

#pragma once

#include <ECS/EntityManager.hpp>

#include <Engine/Utils/DebugWindow.hpp>

class EditorMenuDebugWindow: public DebugWindow
{
public:
    EditorMenuDebugWindow(EntityManager* em, const glm::vec2& pos, const glm::vec2& size);
    virtual ~EditorMenuDebugWindow();

    virtual void        build(float elapsedTime);

    void                displaySaveAsPopup();
    void                displayLoadPopup();

private:
    EntityManager*      _em;
    std::string         _currentLevel;

    std::string         _tmpLoadLevel;
};
