/**
* @Author   Guillaume Labey
*/

#pragma once

#include <ECS/EntityManager.hpp>

#include <Engine/Utils/DebugWindow.hpp>
#include <Engine/Utils/LevelLoader.hpp>

class EditorMenuDebugWindow: public DebugWindow
{
public:
    EditorMenuDebugWindow(EntityManager* em, const glm::vec2& pos, const glm::vec2& size);
    virtual ~EditorMenuDebugWindow();

    virtual void        build(float elapsedTime);

private:
    LevelLoader         _levelLoader;
    EntityManager*      _em;
    std::string         _currentLevel;
};
