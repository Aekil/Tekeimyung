/**
* @Author   Guillaume Labey
*/

#pragma once

#include <Engine/Core/GameState.hpp>
#include <Engine/Graphics/Camera.hpp>

#include <ECS/System.hpp>
#include <ECS/World.hpp>

struct sTransformComponent;
struct sRenderComponent;
struct sUiComponent;

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
    void                                handleObjectSelection();

    void                                renderCameraPreview(Camera& camera);

private:
    Camera                              _camera;

    Entity*                             _cameraPreview{nullptr};
    sTransformComponent*                _cameraPreviewTransform{nullptr};
    sRenderComponent*                   _cameraPreviewRender{nullptr};
    sUiComponent*                       _cameraPreviewUI{nullptr};
END_GAMESTATE(EditorState)
