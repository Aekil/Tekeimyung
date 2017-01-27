/**
* @Author   Guillaume Labey
*/

#pragma once

#include <Engine/Core/GameState.hpp>

#include <ECS/System.hpp>
#include <ECS/World.hpp>

START_GAMESTATE(HowToPlayState)
 public:
    virtual ~HowToPlayState();

    virtual void                        onEnter();
    virtual bool                        init();
    virtual bool                        update(float elapsedTime);

    void                                initMenu();

    Entity*                             createButton(eArchetype type, const glm::vec2& pos);
    void                                handleButtons();
private:
    Camera                              _camera;

    Entity*                             _returnButton = nullptr;
END_GAMESTATE(HowToPlayState)
