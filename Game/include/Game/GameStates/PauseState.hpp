/**
* @Author   Guillaume Labey
*/

#pragma once

#include <Engine/Core/GameState.hpp>

#include <ECS/System.hpp>
#include <ECS/World.hpp>

START_GAMESTATE(PauseState, "Pause")
 public:
    ~PauseState();

    void                                onEnter() override final;
    void                                setupSystems() override final;
    bool                                init() override final;
    bool                                update(float elapsedTime) override final;

    void                                handleButtons();

private:
    System*                             _playStateRendersystem;
    World*                              _playStateWorld;

    Camera                              _camera;

    Entity*                             _resumeButton = nullptr;
    Entity*                             _howToPlayButton = nullptr;
    Entity*                             _optionsButton = nullptr;
    Entity*                             _quitButton = nullptr;
END_GAMESTATE(PauseState)
