/**
* @Author   Guillaume Labey
*/

#pragma once

#include <Engine/Core/GameState.hpp>

START_GAMESTATE(BasicState)
 public:
    ~BasicState();

    bool                                init() override final { return (true); }
    void                                setupSystems() override final;
END_GAMESTATE(BasicState)
