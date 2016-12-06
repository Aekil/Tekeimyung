#pragma once

#include <Engine/Core/GameState.hpp>

START_GAMESTATE(PauseState)
 public:
    virtual ~PauseState();

    virtual bool                        init();
    virtual bool                        update(float elapsedTime);
END_GAMESTATE(PauseState)
