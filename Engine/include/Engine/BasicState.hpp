/**
* @Author   Guillaume Labey
*/

#pragma once

#include <Engine/Core/GameState.hpp>
#include <Engine/Graphics/Camera.hpp>

START_GAMESTATE(BasicState)
 public:
    ~BasicState();

    bool                                init() override final;
    bool                                update(float elapsedTime);
    void                                setupSystems() override final;


private:
    void                                initCamera();
    void                                updateCameraInputs(float elapsedTime);

private:
    Camera                              _camera;
END_GAMESTATE(BasicState)
