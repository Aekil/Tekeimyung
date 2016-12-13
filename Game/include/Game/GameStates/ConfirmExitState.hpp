/**
* @Author   Guillaume Labey
*/

#pragma once

#include <utility>

#include <Engine/Core/GameState.hpp>

#include <ECS/System.hpp>
#include <ECS/World.hpp>

START_GAMESTATE(ConfirmExitState)
 public:
    virtual ~ConfirmExitState();

    virtual void                        onEnter();
    virtual bool                        init();
    virtual bool                        update(float elapsedTime);

    void                                initCamera();
    void                                initPreviousStatesRender();
    void                                initPopup();

    Entity*                             createButton(eArchetype type, const glm::vec2& pos);
    void                                handleButtons();

private:
    std::vector<System*>                _previousStatesSystems;
    std::vector<World*>                 _previousStatesWorlds;

    Camera                              _camera;

    Entity*                             _confirmText = nullptr;
    Entity*                             _confirmPopup = nullptr;
    Entity*                             _yesButton = nullptr;
    Entity*                             _noButton = nullptr;
END_GAMESTATE(ConfirmExitState)
