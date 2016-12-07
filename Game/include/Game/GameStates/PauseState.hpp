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

    void                                initCamera();

    Entity*                             createButton(eArchetype type, const glm::vec2& pos);
    void                                handleButtons();

private:
    System*                             _playStateRendersystem;
    World*                              _playStateWorld;

    Camera                              _camera;

    Entity*                             _resumeButton;
    Entity*                             _quitButton;
END_GAMESTATE(PauseState)
