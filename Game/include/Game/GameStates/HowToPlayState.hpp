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

    void                                initCamera();

    void                                initMenu();
private:
    Camera                              _camera;
END_GAMESTATE(HowToPlayState)
