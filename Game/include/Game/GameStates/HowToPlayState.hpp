/**
* @Author   Guillaume Labey
*/

#pragma once

#include <Engine/Core/GameState.hpp>

#include <ECS/System.hpp>
#include <ECS/World.hpp>

START_GAMESTATE(HowToPlayState, "HowToPlay")
 public:
    ~HowToPlayState();

    void                                onEnter() override final;
    void                                setupSystems() override final;
    bool                                init() override final;
    bool                                update(float elapsedTime) override final;

    void                                handleButtons();
private:
    Camera                              _camera;

    Entity*                             _returnButton = nullptr;
END_GAMESTATE(HowToPlayState)
