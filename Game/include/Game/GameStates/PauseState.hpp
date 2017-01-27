/**
* @Author   Guillaume Labey
*/

#pragma once

#include <Engine/Core/GameState.hpp>

#include <ECS/System.hpp>
#include <ECS/World.hpp>

START_GAMESTATE(PauseState)
 public:
    virtual ~PauseState();

    virtual void                        onEnter();
    virtual bool                        init();
    virtual bool                        update(float elapsedTime);

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
