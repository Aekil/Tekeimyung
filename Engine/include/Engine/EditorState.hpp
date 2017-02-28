/**
* @Author   Guillaume Labey
*/

#pragma once

#include <Engine/Core/GameState.hpp>
#include <Engine/Graphics/Camera.hpp>

#include <ECS/System.hpp>
#include <ECS/World.hpp>

START_GAMESTATE(EditorState)
 public:
    ~EditorState();

    void                                onEnter() override final;
    void                                setupSystems() override final;
    bool                                init() override final;
    bool                                update(float elapsedTime) override final;

private:
    void                                initCamera();
    void                                updateCamera(float elapsedTime);

private:
    Camera                              _camera;
END_GAMESTATE(EditorState)
