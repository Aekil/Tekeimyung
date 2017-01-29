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
    void                                setupSystems() override final;

    void                                initCamera();

private:
    Camera                              _camera;
END_GAMESTATE(BasicState)
